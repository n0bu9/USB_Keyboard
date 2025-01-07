#ifndef __CH55X_HAL_GPIO_H__
#define __CH55X_HAL_GPIO_H__

#include "stdint.h"

typedef enum
{
    PORT_RST = -1,
    PORT_1_PIN_0 = 0,
    PORT_1_PIN_1,
    PORT_1_PIN_2,
    PORT_1_PIN_3,
    PORT_1_PIN_4,
    PORT_1_PIN_5,
    PORT_1_PIN_6,
    PORT_1_PIN_7,
    PORT_DIVIDER,
    PORT_3_PIN_0,
    PORT_3_PIN_1,
    PORT_3_PIN_2,
    PORT_3_PIN_3,
    PORT_3_PIN_4,
    PORT_3_PIN_5,
    PORT_3_PIN_6,
    PORT_3_PIN_7,
}port_pin_enum;

typedef enum
{
    GPIO_INPUT_NO_PULL = 0,  // 浮空输入，无上拉
    GPIO_OUT_INPUT_PP = 1, // 推挽输入输出
    GPIO_OUT_INPUT_OD_NO_PULL = 2, // 开漏输入输出，无上拉
    GPIO_OUT_INPUT_OD = 3, // 类51模式，开漏输入输出，有上拉，内部电路可以加速由低到高的电平爬升
}gpio_mode_enum;

typedef struct
{
    port_pin_enum port_pin;
    #if defined(__C51__)
    bool status;
    #else
    uint8_t status;
    #endif
}gpio_type;

void gpio_init(gpio_type _io, gpio_mode_enum mode);
void gpio_digital_write(gpio_type _io,uint8_t value);
uint8_t gpio_digital_read(gpio_type _io);

#endif
