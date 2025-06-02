#ifndef PWM_H
#define PWM_H

#include "stm32l432xx.h"
#include <stdbool.h>

int pwm_timer_config(TIM_TypeDef* const timer, const unsigned int freq_hz);


#endif