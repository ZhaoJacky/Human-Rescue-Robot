#ifndef I2C_H
#define I2C_H

#include "car.h"

/* ------------------------SysTick Functions---------------------------- */
void SysTick_initialize();

/* ----------------------------------------------------------------------*/

/* --------------------------I2C Functions-------------------------------*/
void i2c_initialize(I2C_TypeDef * i2c_device, PIN scl, PIN sda);
bool i2c_write(I2C_TypeDef* i2c_device, unsigned char receiver_address, 
                                        unsigned char* data, 
                                        unsigned char length);
bool i2c_read(I2C_TypeDef* i2c_device, unsigned char receiver_address, 
                                       unsigned char* data, 
                                       unsigned char length);

/* ------------------------------------------------------------------------*/
#endif