#ifndef __CH55X_WDT_H__
#define __CH55X_WDT_H__

#include "stdint.h"

void wdt_set_time(uint8_t tim);
void wdt_mode_select(uint8_t mode);
void wdt_feed(void);

#endif