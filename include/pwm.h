#ifndef PWM_H
#define PWM_H

#include "car.h"

#define EE14Lib_Err_OK 0
#define EE14Lib_Err_INEXPLICABLE_FAILURE -1
#define EE14Lib_Err_NOT_IMPLEMENTED -2
#define EE14Lib_ERR_INVALID_CONFIG -3

int timer_config_pwm(TIM_TypeDef* const timer, const unsigned int freq_hz);
int timer_config_channel_pwm(TIM_TypeDef* const timer, const PIN pin, const unsigned int duty);

#endif