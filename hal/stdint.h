#ifndef __STDINT_H__
#define __STDINT_H__

#ifdef __C51__

#define inline
typedef bit BOOL;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
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