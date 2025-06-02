#ifndef UART_H
#define UART_H

#include "car.h"

void UART_write_byte (USART_TypeDef *USARTx, char data);
void host_serial_init();

// Very basic function: send a character string to the UART, one byte at a time.
// Spin wait after each byte until the UART is ready for the next byte.
void serial_write(USART_TypeDef *USARTx, const char *buffer, int len);

// Spin wait until we have a byte.
char serial_read(USART_TypeDef *USARTx);

#endif