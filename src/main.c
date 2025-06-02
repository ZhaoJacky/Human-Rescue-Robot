#include "stm32l432xx.h"
#include "car.h"
#include "i2c.h"

void led_test(PIN pin){
    GPIO_moder(pin, OUTPUT);
    GPIO_pupdr(pin, NO_PUPD);
    GPIO_otyper(pin, PUSH_PULL);
    GPIO_ospeedr(pin, LOW);
    //GPIO_afr(pin, unsigned int func);
    GPIO_write(pin, 1);
    for(volatile int i = 0; i < 1000; i++){}
    GPIO_write(pin, 0);
    for(volatile int i = 0; i < 1000; i++){}
    //GPIO_idr(pin);
}

void motor_test(PIN en, PIN ph){
    //configure Timer 1 with 2000 hz frequency
    timer_config_pwm(TIM1,2000);

    //configure each pin with duty cycles (0-1023) for an initial color
    //tim 1 d1 d9 d0 (en/pwm)
    timer_config_channel_pwm(TIM1, D1, 10); 

    //direction ph/output
    GPIO_moder(ph, OUTPUT);
    GPIO_pupdr(ph, NO_PUPD);
    GPIO_otyper(ph, PUSH_PULL);
    GPIO_ospeedr(ph, LOW);
    GPIO_write(ph, 1);
    for(volatile int i = 0; i < 1000; i++){}
}

int main() {
    i2c_initialize(I2C1, D1, D0);

    while(1) {
        
    }
}