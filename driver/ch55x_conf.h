#ifndef __CH55X_CONF_H__
#define __CH55X_CONF_H__

#define GPIO_MODULE_ENABLED
#define UART_MODULE_ENABLED
#define TIMER_MODULE_ENABLED
// #define I2C_MODULE_ENABLED
// #define WDG_MODULE_ENABLED

#include "system_ch55x.h"

#if defined(GPIO_MODULE_ENABLED)
#include "ch55x_gpio.h"
#endif

#if defined(I2C_MODULE_ENABLED)
#include "ch55x_i2c.h"
#endif

#if defined(UART_MODULE_ENABLED)
#include "ch55x_uart.h"
#endif

#if defined(TIMER_MODULE_ENABLED)
#include "ch55x_timer.h"
#endif

#endif
