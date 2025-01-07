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
}port_pin_type;

typedef struct
{
    port_pin_type port_pin;
    uint8_t status;
}io_type;


#endif