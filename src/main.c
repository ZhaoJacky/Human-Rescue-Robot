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

int main() {
    i2c_initialize(I2C1, D1, D0);
    while(1) {
        
    }
}