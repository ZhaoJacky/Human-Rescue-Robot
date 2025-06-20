#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "pwm.h"

void init_motor(PIN enable1, PIN enable2, PIN phase,PIN mode);
void move_forward(PIN phase);
void move_backward(PIN phase);
void spin_right(PIN enable1, PIN enable2);
void spin_left(PIN enable1, PIN enable2);
void stop(PIN enable1, PIN enable2);
void modify_speed(PIN enable1, PIN enable2, int speed);

#endif