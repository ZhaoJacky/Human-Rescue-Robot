#include "stm32l432xx.h"

void SysTick_initialize() {

    // Step 1.
    SysTick->CTRL = 0; // Disables the interrupt in order to configure it.

    // Step 2.
    SysTick->LOAD = 3999; // Sets reload value, so that the counter reaches 0 every 1 kHz (1 ms)

    // Step 3. // Sets Priority Number to Lowest Number (Highest Priority)
    NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1); // Makes SysTick interrupt the highest priority

    // Step 4. 
    SysTick->VAL = 0; // Ensures that the counter's initial value is 0 at initialization of SysTick
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; // Makes the clock of SysTick the processor clock.
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; // Makes it so SysTick generates an interrupt.
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // Turns SysTick On.


}