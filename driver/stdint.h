#ifndef __STDINT_H__
#define __STDINT_H__

#if defined(__C51__)

// #define inline
#define SUCCESS 0
#define FAIL    0xFF
#define START   1
#define STOP    0
#define TRUE    1
#define FALSE   0

#define bool bit
#define uint8_t unsigned char
#define UINT8_MAX 0xFF
#define uint16_t unsigned short
#define UINT16_MAX 0xFFFF
#define uint32_t unsigned long
#define UINT32_MAX 0xFFFFFFFF
#define uint8d_t UINT8D
#define uint16d_t UINT16D
#define uint32d_t UINT32D
#define int8_t signed char
#define int16_t signed short
#define int32_t signed long

#else

#define xdata
#define code
#define idata
#define pdata
#define sfr
#define sbit
#define _at_

#endif

#endif
