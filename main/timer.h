#ifndef __TIMER_H__
#define __TIMER_H__

#include "stdint.h"

#define ATOMIC_RESTORESTATE     E_DIS
#define ATOMIC_BLOCK(type)      for (__ToDo = 1, type = 1; __ToDo; __ToDo = 0, type = 0)  // 原子操作
//#define ATOMIC_BLOCK(type)      for (__ToDo=1;__ToDo==1;__ToDo=0)

#define TIMER_DIFF_16(a, b)    ((uint16_t)(a - b))
#define TIMER_DIFF_32(a, b)    ((uint32_t)(a - b))

void timer_init(void); // 初始化定时器0
uint16_t timer_read(void); // 读取定时器0计数(16位)
uint32_t timer_read32(void); // 读取定时器0计数(32位)
uint16_t timer_elapsed(uint16_t start); // 计算时间差(16位)
uint32_t timer_elapsed32(uint32_t start); // 计算时间差(32位)
void timer_clear(void); // 清除定时器0计数
void timer_delay(uint32_t ms); // 精确延时ms

#endif
