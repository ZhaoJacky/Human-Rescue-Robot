#include "movement.h"


/*
motor 1: Right Motor
enable/speed: D1
phase: D0
*/

/*
ronaldo: Left Motor
enable/speed: A7
phase: D0
*/

/*
Name: init_motor
Purpose: Initializes the GPIO pins on tthe STM32 to be used for the mo
Arguments:
Return:
Effect:
*/
void init_motor(PIN enable1, PIN enable2, PIN phase,PIN mode) {
    //configure Timer 1 and Timer 2 with 2000 hz frequency
    timer_config_pwm(TIM1, 1000);
    timer_config_pwm(TIM2, 1000);

    //set speed using PWM, TIM1, and TIM2
    timer_config_channel_pwm(TIM1, enable1, 700); 
    timer_config_channel_pwm(TIM2, enable2, 700);

    //direction phase/output
    GPIO_moder(phase, OUTPUT);
    GPIO_pupdr(phase, NO_PUPD);
    GPIO_otyper(phase, PUSH_PULL);
    GPIO_ospeedr(phase, LOW);
    GPIO_write(phase, 0);

    GPIO_moder(mode, OUTPUT);
    GPIO_pupdr(mode, NO_PUPD);
    GPIO_otyper(mode, PUSH_PULL);
    GPIO_ospeedr(mode, LOW);
    GPIO_write(mode, 1);
    for(volatile int i = 0; i < 1000; i++){}
}

/*
Name: move_forward
Purpose: 
Arguments:
Return:
Effect:
*/
void move_forward(PIN phase) {
    GPIO_write(phase, 1);
}

/*
Name: move_backward
Purpose: 
Arguments:
Return:
Effect:
*/
void move_backward(PIN phase) {
    GPIO_write(phase, 0);
}

/*
Name: spin_right
Purpose: 
Arguments:
Return:
Effect:
*/
void spin_right(PIN enable1, PIN enable2) {
    timer_config_channel_pwm(TIM1, enable1, 1023);
    //ronaldo moves
    timer_config_channel_pwm(TIM2, enable2, 600);
}

/*
Name:
Purpose: 
Arguments:
Return:
Effect:
*/
void spin_left(PIN enable1, PIN enable2) {
    //right motor turns
    timer_config_channel_pwm(TIM1, enable1, 600);
    timer_config_channel_pwm(TIM2, enable2, 1023);
}

/*
Name: stop
Purpose: 
Arguments:
Return:
Effect:
*/
void stop(PIN enable1, PIN enable2) {
    timer_config_channel_pwm(TIM1, enable1, 1023);
    timer_config_channel_pwm(TIM2, enable2, 1023);
}

/*
Name: modify_speed
Purpose: 
Arguments:
Return:
Effect:
*/
void modify_speed(PIN enable1, PIN enable2, int speed) {
    timer_config_channel_pwm(TIM1, enable1, speed);
    timer_config_channel_pwm(TIM2, enable2, speed);
}