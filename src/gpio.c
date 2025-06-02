// Citation: Professor Steven Bell's EE14 (Embedded Systems) GPIO code!

#include "car.h"

//map each pin to its corresponding port 
static GPIO_TypeDef * port[D13 + 1] = {
    GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, 
    GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, 
    GPIOA, GPIOB, GPIOB, GPIOB, GPIOB,
    GPIOC, GPIOA, GPIOA, GPIOB, GPIOB,
    GPIOB, GPIOB
}; 

//PC 14 & 15?

/*A0 = PA0; A1 = PA1; A2 = PA3; A3 = PA4; A4 = PA5;
A5 = PA6; A6 = PA7; A7 = PA2; D0 = PA10; D1 = PA9;
D2 = PA12; D3 = PB0; D4 = PB7; D5 = PB6; D6 = PB1;
D7 = PC14; D8 = PC15; D9 = PA8; D10 = PA11; D11 = PB5; 
D12 = PB4; D13 = PB3*/

//A5 is shorted to D5
//A4 is shorted to D4

static uint8_t pin_arr[D13 + 1] = {
    0, 1, 3, 4, 5,
    6, 7, 2, 10, 9,
    12, 0, 7, 6, 1,
    14, 15, 8, 11, 5,
    4, 3
};

/*
* Input: pointer to port of that the pin is on
* Function: determines the port & enables that corresponding clock through a 
* bitwise or
* Output: void function; no output
*/
static void GPIO_enable_port(GPIO_TypeDef *port) {
    unsigned long bit_set; //32 bit
    if(port == GPIOA){
        bit_set = RCC_AHB2ENR_GPIOAEN; //.h file defined constant for 0x00000001
    }else if(port == GPIOB){
        bit_set = RCC_AHB2ENR_GPIOBEN; //0x00000002
    }else if(port == GPIOC){
        bit_set = RCC_AHB2ENR_GPIOCEN; //0x00000004
    }else{
        bit_set = RCC_AHB2ENR_GPIOHEN;
    }           
    RCC -> AHB2ENR |= bit_set; //AHB2ENR peripheral clock enable register
}

/* GPIO_moder
* Input: pointer to port that the pin is on, the pin number necessary
* Function: determines the port which the pin is located on and ensures it is 
* enabled. Clears the bits corresponding to the pin. Sets the corresponding 2 
* bits to the mode passed in.
* Output: -1 if the mode passed in is invalid, 0 if no errors
*/
int GPIO_moder(PIN pin, unsigned int mode){
    GPIO_TypeDef* find_port = port[pin];
    uint8_t offset = pin_arr[pin];

    if(mode & ~0b11UL){ //if bits other than bottom 2 are 1s, mode is invalid
        return -1;
    }

    GPIO_enable_port(find_port); //enable again, just in case

    find_port -> MODER &= ~(0b11 << offset*2);
    find_port -> MODER |= (mode << offset*2);
    return 0;
}

int GPIO_pupdr(PIN pin, unsigned int mode){
    GPIO_TypeDef* find_port = port[pin];
    uint8_t offset = pin_arr[pin];

    if(mode & ~0b11UL){ //if bits other than bottom 2 are 1s, mode is invalid
        return -1;
    }

    find_port -> PUPDR &= ~(0b11 << offset*2);
    find_port -> PUPDR |= (mode << offset*2);

    return 0;
}

int GPIO_otyper(PIN pin, unsigned int mode){
    GPIO_TypeDef* find_port = port[pin];
    uint8_t offset = pin_arr[pin];

    if(mode & ~0b1UL){ //if bits other than bottom 1 are 1s, mode is invalid
        return -1;
    }

    find_port -> OTYPER &= ~(0b1 << offset);
    find_port -> OTYPER |= (mode << offset);

    return 0;
}

int GPIO_ospeedr(PIN pin, unsigned int mode){
    GPIO_TypeDef* find_port = port[pin];
    uint8_t offset = pin_arr[pin];

    if(mode & ~0b11UL){ //if bits other than bottom 2 are 1s, mode is invalid
        return -1;
    }

    find_port -> OSPEEDR &= ~(0b11 << offset);
    find_port -> OSPEEDR |= (mode << offset);

    return 0;
}

int GPIO_afr(PIN pin, unsigned int func){
    GPIO_TypeDef* find_port = port[pin];
    uint8_t offset = pin_arr[pin];

    GPIO_enable_port(find_port);

    find_port -> MODER &= ~(0b11 << offset*2);
    find_port -> MODER |= (0b10 << offset*2);

    unsigned int afr_offset = offset*4;

    find_port -> AFR[afr_offset >> 5] &= ~(0b1111 << (0x1F & afr_offset));
    find_port -> AFR[afr_offset >> 5] |= (func << (0x1F & afr_offset));

    return 0;
}

void GPIO_write(PIN pin, bool value){
    GPIO_TypeDef *find_port = port[pin];
    uint8_t offset = pin_arr[pin];

    if(value){
        find_port -> BSRR |= 1 << offset;
    }else{
        find_port -> BRR |= 1 << offset;
    }
}

bool GPIO_idr(PIN pin){
    GPIO_TypeDef *find_port = port[pin];
    uint8_t offset = pin_arr[pin];

    return ((find_port -> IDR >> offset) & 0b1);
}
