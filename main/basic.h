#ifndef __BASIC_H__
#define __BASIC_H__

#include "main.h"

#ifdef __C51__
#define inline
#endif

#define ATOMIC_RESTORESTATE     E_DIS
#define ATOMIC_BLOCK(type)      for(type, _to_do = 1; _to_do; _to_do, type = 0)  // 原子操作

#define TIMER_DIFF_16(a, b)    ((uint16_t)(a - b))
#define TIMER_DIFF_32(a, b)    ((uint32_t)(a - b))

void basic_init(void);
inline void timer_clear(void);
inline uint16_t timer_read(void);
inline uint32_t timer_read32(void);
inline uint16_t timer_elapsed(uint16_t last);
inline uint32_t timer_elapsed32(uint32_t last);

#endif