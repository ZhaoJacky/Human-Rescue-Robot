/*
i2c.h

Authors: Jacky Zhao and Charlotte Wong
Date: 05/25/25
Purpose: File contains the function declarations for the i2c.c and
systick.c files. These functions enable systick and i2c to work in the RC
car.

*/

#ifndef I2C_H
#define I2C_H

#include "car.h"

/* ------------------------SysTick Functions---------------------------- */
void SysTick_initialize();


/* ----------------------------------------------------------------------*/

/* --------------------------I2C Functions-------------------------------*/
void i2c_initialize(I2C_TypeDef * i2c_port, PIN scl, PIN sda);


#endif