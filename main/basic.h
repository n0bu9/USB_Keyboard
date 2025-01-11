#ifndef __BASIC_H__
#define __BASIC_H__

#include "stdint.h"

#define ATOMIC_RESTORESTATE     E_DIS
//#define ATOMIC_BLOCK(type)      for (__ToDo = 1, type = 1; __ToDo; __ToDo = 0, type = 0)  // 原子操作
#define ATOMIC_BLOCK(type)      for (__ToDo=1;__ToDo==1;__ToDo=0)

#define TIMER_DIFF_16(a, b)    ((uint16_t)(a - b))
#define TIMER_DIFF_32(a, b)    ((uint32_t)(a - b))

void basic_init(void);
void timer_clear(void);
uint16_t timer_read(void);
uint32_t timer_read32(void);
uint16_t timer_elapsed(uint16_t last);
uint32_t timer_elapsed32(uint32_t last);
void feed_wdt(void);
void stop_wdt(void);
void enable_wdt(void);

#endif
