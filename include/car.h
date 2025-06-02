#ifndef CAR_H
#define CAR_H

#include "stm32l432xx.h"
#include <stdbool.h>

//all 22 pins on STM32, in ascending port & pin #s
typedef enum {A0, A1, A2, A3, A4, A5, A6, A7, 
            D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13} PIN;
//8 pins on port A

//MODER
#define INPUT 0b00
#define OUTPUT 0b01
#define ALT_FUNC 0b10
#define ANALOG 0b11

//OTYPER
#define PUSH_PULL 0b0
#define OPEN_DRAIN 0b1

//OSPEEDR
#define LOW 0b00
#define MED 0b01
#define HI 0b10
#define V_HI 0b11

//PUPDR
#define NO_PUPD 0b00
#define PU 0b01
#define PD 0b10
#define RES 0b11

/* -------------------------- GPIO Functions --------------------------------*/
static void GPIO_enable_port(GPIO_TypeDef *port);
int GPIO_moder(PIN pin, unsigned int mode);
int GPIO_pupdr(PIN pin, unsigned int mode);
int GPIO_otyper(PIN pin, unsigned int mode);
int GPIO_ospeedr(PIN pin, unsigned int mode);
int GPIO_afr(PIN pin, unsigned int func);
void GPIO_write(PIN pin, bool value);
bool GPIO_idr(PIN pin);
/* --------------------------------------------------------------------------*/


#endif
