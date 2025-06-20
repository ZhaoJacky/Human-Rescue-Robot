#include <stdio.h>
#include "car.h"
#include "i2c.h"
#include "uart.h"
#include "pwm.h"
#include "movement.h"

int _write(int file, char *data, int len) {
    serial_write(USART2, data, len);
    return len;
}

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

void enable_button(PIN button) {
    GPIO_moder(button, INPUT);
    GPIO_pupdr(button, PD);
    
}

void buttons(PIN backward, PIN stop_pin, PIN forward, PIN phase, PIN enable1, PIN enable2){
    enable_button(backward);
    enable_button(stop_pin);
    enable_button(forward);

    if(GPIO_idr(backward)){
        move_backward(phase);
    }else if(GPIO_idr(stop_pin)){
        stop(enable1, enable2);
    }else if(GPIO_idr(forward)){
        move_forward(phase);
    }
}



int main() {

    host_serial_init();

    init_motor(D1, A7, D0, D4);
    // move_forward(D0);
    // for(volatile int i = 0; i < 1500000; i++){}
    // move_backward(D0);
    // for(volatile int i = 0; i < 1500000; i++){}
    // stop(D1, A7);                                                                                                                        ````````````    ``````````````  ``````````````````````````````````````````````````````````````````````  `
    // for(volatile int i = 0; i < 1500000; i++){}
    // modify_speed(D1, A7, 700);
    // for(volatile int i = 0; i < 1500000; i++){}
    // move_forward(D0);
    // for(volatile int i = 0; i < 1500000; i++){}
    // spin_left(D1, A7);
    // for(volatile int i = 0; i < 1500000; i++){}
    // spin_right(D1, A7);

    while(1) {
        buttons(D12, A3, D8, D0, D1, A7);
    }
}