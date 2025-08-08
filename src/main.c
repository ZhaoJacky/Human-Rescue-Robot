#include <stdio.h>
#include "car.h"
#include "i2c.h"
#include "uart.h"
#include "pwm.h"
#include "movement.h"

volatile int press = 0;

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

    // if(GPIO_idr(backward)){
    //     move_backward(phase);
    if(GPIO_idr(stop_pin)){
        // press++;
        stop(enable1, enable2);
    }else if(GPIO_idr(forward)){
        // press++;
        move_forward(phase, enable1, enable2);
    }
}



int main() {

    host_serial_init();

    init_motor(D6, A0, D2, D4);
    stop(D6, A0);

    while(1) {

        char byte = '0';
        if (USART2->ISR & USART_ISR_RXNE) {
            byte = USART2->RDR;  // Always read, this clears RXNE and ORE
            printf("Got byte: %c\n", byte);
        }

        if(byte == 'a') {
            move_forward(D2, D6, A0);
        } else if (byte == 'b') {
            move_backward(D2);
        } else if (byte == 'c') {
            spin_left(D6, A0);
        } else if (byte == 'd') {
            spin_right(D6, A0);
        } else {
            stop(D6, A0);
        }

    }
}