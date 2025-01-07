#ifndef __STDINT_H__
#define __STDINT_H__

#if defined(__C51__)

#define inline
#define bit bool;
#define uint8_t unsigned char;
#define uint16_t unsigned short;
#define uint32_t unsigned long;
#define int8_t signed char;
#define int16_t signed short;
#define int32_t signed long;
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
