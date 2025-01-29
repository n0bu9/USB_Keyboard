#ifndef __UART_H__
#define __UART_H__

#include "main.h"

void uart_send_string(uint8_t uart_x, const int8_t *str);
void uart_send_byte(uint8_t uart_x, uint8_t byte);

#endif