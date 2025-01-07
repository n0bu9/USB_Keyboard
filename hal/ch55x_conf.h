#ifndef __CH55X_HAL_CONF_H__
#define __CH55X_HAL_CONF_H__

#define GPIO_MODULE_ENABLED
#define UART_MODULE_ENABLED
// #define I2C_MODULE_ENABLED
// #define WDG_MODULE_ENABLED

#if defined(GPIO_MODULE_ENABLED)
#include "ch55x_gpio.h"
#endif

#if defined(I2C_MODULE_ENABLED)
#include "ch55x_i2c.h"
#endif

#if defined(UART_MODULE_ENABLED)
#include "ch55x_uart.h"
#endif

#endif