/* Citation: Professor Steven Bell's EE14 Pulse-Width Modulation Code! */

#include "pwm.h"
#include "car.h"

int timer_config_pwm(TIM_TypeDef* const timer, const unsigned int freq_hz){
    // Enable the clock for the timer
    if(timer == TIM1){
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    }
    else if(timer == TIM2){
        RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    }
    else if(timer == TIM15){
        RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
    }
    else if(timer == TIM16){
        RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
    }
    else{
        return EE14Lib_Err_NOT_IMPLEMENTED;
    }

    // Top-level control registers are fine with defaults (except for turning it on, later)

    // Calculates and sets the minimum pre-scaler value by using the desired
    // frequency, freq_hz.
    timer->PSC = (4000000 / freq_hz) / 1023;

    // Frequency of the clock after getting scaled by the pre-scaler.
    unsigned int freq_psc = ((4000000) / (timer->PSC + 1));

    // Calculates the reload value based on the frequency of the pre-scaled clock,
    // and the desired frequency.
    unsigned int reload_value = ((freq_psc) / (freq_hz)) - 1;

    // Set the reload value
    timer->ARR = reload_value;

    // Set the main output enable
    // timer->BDTR |= TIM_BDTR_MOE;

    // if (timer == TIM1) {
    timer->BDTR |= TIM_BDTR_MOE;
    // }

    // And enable the timer itself
    timer->CR1 |= TIM_CR1_CEN;

    return EE14Lib_Err_OK;
}

// Mapping of GPIO pin to timer channel
// Lowest bit is whether this is an inverted channel (N),
// remaining bits are zero-indexed (0-3 for TIMx_1-TIMx_4)
//   1  -> 0
//   1N -> 1
//   2  -> 2
//   2N -> 3
//   3  -> 4
//   3N -> 5
//   4  -> 6
// -1 for pins which are not PWM-capable / not on this timer.
int g_Timer1Channel[D13+1] = {
  -1,-1,-1,-1,  // A0=PA0,A1=PA1,A2=PA3,A3=PA4
  -1,-1, 1,-1,  // A4=PA5,A5=PA6,A6=PA7,A7=PA2
   4, 2,-1, 3,  // D0=PA10,D1=PA9,D2=PA12,D3=PB0
  -1,-1, 5,-1,  // D4=PB7,D5=PB6,D6=PB1,D7=PC14
  -1, 0, 6,-1,  // D8=PC15,D9=PA8,D10=PA11,D11=PB5
  -1,-1         // D12=PB4,D13=PB3.
};

int g_Timer2Channel[D13+1] = {
   0, 2, 6,-1,  // A0=PA0,A1=PA1,A2=PA3,A3=PA4
   0,-1,-1, 4,  // A4=PA5,A5=PA6,A6=PA7,A7=PA2
  -1,-1,-1,-1,  // D0=PA10,D1=PA9,D2=PA12,D3=PB0
  -1,-1,-1,-1,  // D4=PB7,D5=PB6,D6=PB1,D7=PC14
  -1,-1,-1,-1,  // D8=PC15,D9=PA8,D10=PA11,D11=PB5
  -1, 2         // D12=PB4,D13=PB3.
};

int g_Timer15Channel[D13+1] = {
  -1, 1, 2,-1,  // A0=PA0,A1=PA1,A2=PA3,A3=PA4
  -1,-1,-1, 0,  // A4=PA5,A5=PA6,A6=PA7,A7=PA2
  -1,-1,-1,-1,  // D0=PA10,D1=PA9,D2=PA12,D3=PB0
  -1,-1,-1,-1,  // D4=PB7,D5=PB6,D6=PB1,D7=PC14
  -1,-1,-1,-1,  // D8=PC15,D9=PA8,D10=PA11,D11=PB5
  -1,-1         // D12=PB4,D13=PB3.
};

int g_Timer16Channel[D13+1] = {
  -1,-1,-1,-1,  // A0=PA0,A1=PA1,A2=PA3,A3=PA4
  -1, 0,-1,-1,  // A4=PA5,A5=PA6,A6=PA7,A7=PA2
  -1,-1,-1,-1,  // D0=PA10,D1=PA9,D2=PA12,D3=PB0
  -1, 1,-1,-1,  // D4=PB7,D5=PB6,D6=PB1,D7=PC14
  -1,-1,-1,-1,  // D8=PC15,D9=PA8,D10=PA11,D11=PB5
  -1,-1         // D12=PB4,D13=PB3.
};

/*
timer_config_channel_pwm
Input: pointer to timer struct of the pin, the pin being used, and the duty cycle
Description: 
- assigns appropriate channel according to pin number
- calculates and assigns appropriate CCR value
Output: whether or not an error occurred due to a pin not being PWM capable
- returns EE14Lib_ERR_INVALID_CONFIG if the pin has no corresponding channel  
(not PWM capable)
- otherwise returns EE14Lib_Err_OK
*/
int timer_config_channel_pwm(TIM_TypeDef* const timer, const PIN pin, const unsigned int duty){
    int channel = -1;
    if(timer == TIM1){
        channel = g_Timer1Channel[pin];
    } else if(timer  == TIM2){
        channel = g_Timer2Channel[pin];
    } else if(timer == TIM15){
        channel = g_Timer15Channel[pin];
    } else if(timer == TIM16){
        channel = g_Timer16Channel[pin];
    }

    if(channel < 0){
        return EE14Lib_ERR_INVALID_CONFIG;
    }

    int channel_idx = channel >> 1; // Lowest bit is N

    // There are 4 CCR registers (one for each possible channel), so the funky
    // pointer math `timer + 13 + channel_idx` is just picking the right one to
    // set.  You shouldn't have to touch that part, just replace the value on
    // the right side of the assignment.

    // Calculates the compare_value (the value that determines when the
    // output of a pin is set to HIGH) using the duty value.
    unsigned int compare_value = ((timer->ARR + 1)*(1023 - duty)) / 1023;

    // Set the compare value
    // Timer CCR registers are 0x34 through 0x40
    *((unsigned int*)timer + 13 + channel_idx) = compare_value;

    //calculates and assignes appropriate CCR value based of given duty cycle
    //*((unsigned int*)timer + 13 + channel_idx) = (duty)*((timer -> ARR)+1)/1023;

    // Enable PWM mode, and set preload enable (only update counter on rollover)
    if(channel_idx == 0){
        timer->CCMR1 &= ~(TIM_CCMR1_OC1M);
        timer->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
        timer->CCMR1 |= TIM_CCMR1_OC1PE;
    } else if(channel_idx == 1){
        timer->CCMR1 &= ~(TIM_CCMR1_OC2M);
        timer->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
        timer->CCMR1 |= TIM_CCMR1_OC2PE;
    } else if(channel_idx == 2){
        timer->CCMR2 &= ~(TIM_CCMR2_OC3M);
        timer->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);
        timer->CCMR2 |= TIM_CCMR2_OC3PE;
    } else { // Must be 3
        timer->CCMR2 &= ~(TIM_CCMR2_OC4M);
        timer->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1);
        timer->CCMR2 |= TIM_CCMR2_OC4PE;
    }

    // Enable the capture/compare output
    timer->CCER |= 1 << (2*channel); // Primary enables are 0, 4, 8, 12; inverted are 2, 6

    if(timer == TIM1 || timer == TIM2){
        GPIO_afr(pin, 1); // AFR = 1 is timer mode for timers 1 & 2
    } else {
        GPIO_afr(pin, 14); // AFR = 14 for timers 15 & 16
    }

    return EE14Lib_Err_OK;
}

