// Citation: Professor Steven Bell's EE14 (Embedded Systems) I2C code!

#include "stm32l432xx.h"
#include "i2c.h"

void i2c_initialize(I2C_TypeDef * i2c_device, PIN scl, PIN sda) {
    // The pins for SCL and SDA are configured as open-drain.
    // This is because if they were configured as push-pull, then if they generate
    // DIFFERENT output, then a current is produced that could fry the microcontroller.
    GPIO_otyper(scl, OPEN_DRAIN);
    GPIO_otyper(sda, OPEN_DRAIN);

    // Since SDA and SCL are open-drain, that means they need to be PULLED UP to HIGH.
    GPIO_pupdr(scl, PU);
    GPIO_pupdr(sda, PU);

    // Then SCL and SDA need to be configure to alternate function because
    // that is what enables the pins to be used by I2C.
    // Alternate function basically allows SDA and SCL to be used by serial
    // protocols like SPI, UART, and I2C.
    GPIO_afr(sda, 4); // I2C uses Alternate Function 4.
    GPIO_afr(scl, 4);

    // Enables the clock based on the I2C Device.
    if (i2c_device == I2C1) {
        RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN; // Enables the APB1 Clock for I2C
        RCC->CCIPR &= ~RCC_CCIPR_I2C1SEL; // Resets the CLK source for I2C.
        // CCIPR = Clock Configuration Independent Peripheral Register
        RCC->CCIPR |= 0b01 << RCC_CCIPR_I2C1SEL_Pos; // Sets SYSCLK as CLK source for I2C

        // This resets the I2C module.
        RCC->APB1RSTR1 |= RCC_APB1RSTR1_I2C1RST; // Resets the I2C hardware peripheral
        RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C1RST; // Enables it again.

    } else if (i2c_device == I2C3) {
        RCC->APB1ENR1 |= RCC_APB1ENR1_I2C3EN;
        RCC->CCIPR &= ~RCC_CCIPR_I2C3SEL;
        RCC->CCIPR |= 0b01 << RCC_CCIPR_I2C3SEL_Pos;

        RCC->APB1RSTR1 |= RCC_APB1RSTR1_I2C3RST;
        RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C3RST;
    } else {
        return -3;
    }

    // Sets the I2C clock to around a 400 kHz timing.
    i2c_device->TIMINGR |= 0 << I2C_TIMINGR_PRESC_Pos |
                    0 << I2C_TIMINGR_SCLDEL_Pos | 
                    1 << I2C_TIMINGR_SCLH_Pos |
                    1 << I2C_TIMINGR_SCLL_Pos;

    i2c_device->CR1 |= I2C_CR1_PE; // Enables the I2C Peripheral
}

bool i2c_write(I2C_TypeDef* i2c_device, unsigned char receiver_address, unsigned char* data, unsigned char length) {
    while(i2c_device->ISR & I2C_ISR_BUSY) {} // Checks if I2C is busy and waits until it is not.
    // Interrupt and Status Register = ISR

    uint32_t cr2 = I2C_CR2_AUTOEND | // Sends a STOP bit after sending data (NBYTES)
                   length << I2C_CR2_NBYTES_Pos | // Sets the number of bytes that are being sent.
                   receiver_address << 1 | // Only bits 7:1 matter for 7-bit address
                   I2C_CR2_START; // Sends a START bit.
    i2c_device->CR2 = cr2;

    for(int i = 0; i < length; i++){
        
        while(!(i2c_device->ISR & I2C_ISR_TXIS)) {} // Wait until it's ready for the next byte
        i2c_device->TXDR = data[i]; // TXIS is cleared when we write the next byte
    }

    // Wait for the transaction to complete (stop bit flag to be set)
    while(!(i2c_device->ISR & I2C_ISR_STOPF)) {}
    i2c_device->ICR = I2C_ICR_STOPCF;

    // Check whether we got ACK or NACK
    // This code runs if we receive a NACK, basically the write failed.
    if(i2c_device->ISR & I2C_ISR_NACKF){
        i2c_device->ICR = I2C_ICR_NACKCF; // Clear the NACK flag
        return false;
    }

    return true; // Returning true means that we received an ACK, so the write succeeded.
}

bool i2c_read(I2C_TypeDef* i2c_device, unsigned char receiver_address, unsigned char* data, unsigned char length) {
    // Checks if I2C is busy and waits until it is not busy anymore.
    while(i2c_device->ISR & I2C_ISR_BUSY) {}

    // Sets the address of the receiver, number of bytes of data,
    // generates the START bit and automatically generates the 
    // STOP bit once the data is finished being read.
    uint32_t control_reg2 = I2C_CR2_AUTOEND | 
                            I2C_CR2_RD_WRN |
                            length << I2C_CR2_NBYTES_Pos |
                            receiver_address << 1 |
                            I2C_CR2_START;

    // Sets the control register 2 of the i2c device.
    i2c_device->CR2 = control_reg2;

    // Reads the data byte by byte and checks the RXNE flag to see if
    // more data can be read.
    for(int i = 0; i < length; i++) {
        while(!(i2c_device->ISR & I2C_ISR_RXNE)) {}
        data[i] = i2c_device->RXDR;
    }

    // Checks to see if a NACK flag is generated, which means the data was
    // not read successfully.
    if(i2c_device->ISR & I2C_ISR_NACKF) {
        i2c_device->ISR = I2C_ISR_NACKF;
        return false;
    }

    return true; // Data has been read successfully!
}