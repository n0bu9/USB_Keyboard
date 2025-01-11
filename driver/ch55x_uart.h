#ifndef __CH55X_HAL_UART_H__
#define __CH55X_HAL_UART_H__

#include "stdint.h"

#define USE_UART0_ENABLE 1  // 1: 使用UART0，0: 不使用UART0
#define USE_UART1_ENABLE 0  // 1: 使用UART1，0: 不使用UART1


#if USE_UART0_ENABLE
#define  UART0_BUAD    57600                // UART0波特率

void uart0_init(void);
void ch55x_uart0_alter(void);
uint8_t uart0_rcv_byte(void);
void uart0_send_byte(uint8_t SendDat);

#endif


#if USE_UART1_ENABLE
#define  UART1_BUAD                 57600   // UART1波特率
#define  UART1_FAST_MODE_ENABLE     1       // 1: 快速模式，0: 慢速模式

void uart1_init(void);
void ch55x_uart1_alter(void);
uint8_t uart1_rcv_byte(void);
void uart1_send_byte(uint8_t SendDat);

#endif


#endif