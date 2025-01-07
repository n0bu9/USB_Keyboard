#ifndef __BASIC_H__
#define __BASIC_H__

#include "main.h"

#ifdef __C51__
#define inline
#endif

#define ATOMIC_RESTORESTATE     E_DIS
#define ATOMIC_BLOCK(type)      //for (UINT8 __ToDo = 1; __ToDo; __ToDo = 0)  // 原子操作

#define TIMER_DIFF_16(a, b)    ((uint16_t)(a - b))
#define TIMER_DIFF_32(a, b)    ((uint32_t)(a - b))

void basic_init(void);
void timer_clear(void);
uint16_t timer_read(void);
uint32_t timer_read32(void);
uint16_t timer_elapsed(uint16_t last);
uint32_t timer_elapsed32(uint32_t last);
void uart0_init(void);
void feed_wdt(void);
void stop_wdt(void);
void enable_wdt(void);

#endif