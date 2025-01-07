#include "ch55x_conf.h"

void gpio_init(gpio_type _io, gpio_mode_enum mode) //其初始化
{
    if (_io.port_pin < PORT_DIVIDER){
        switch(mode){
        case GPIO_INPUT_NO_PULL:
            P1_MOD_OC = P1_MOD_OC & ~(1<<_io.port_pin);
            P1_DIR_PU = P1_DIR_PU &	~(1<<_io.port_pin);
            break;
        case GPIO_OUT_INPUT_PP:
            P1_MOD_OC = P1_MOD_OC & ~(1<<_io.port_pin);
            P1_DIR_PU = P1_DIR_PU |	(1<<_io.port_pin);
        break;
        case GPIO_OUT_INPUT_OD_NO_PULL:
            P1_MOD_OC = P1_MOD_OC | (1<<_io.port_pin);
            P1_DIR_PU = P1_DIR_PU &	~(1<<_io.port_pin);
            break;
        case GPIO_OUT_INPUT_OD:
            P1_MOD_OC = P1_MOD_OC | (1<<_io.port_pin);
            P1_DIR_PU = P1_DIR_PU |	(1<<_io.port_pin);
            break;
        default:
            break;
        }
    }else if (_io.port_pin > PORT_DIVIDER){
        switch(Mode){
        case GPIO_INPUT_NO_PULL:
            P3_MOD_OC = P3_MOD_OC & ~(1<<(_io.port_pin-PORT_DIVIDER-1));
            P3_DIR_PU = P3_DIR_PU &	~(1<<(_io.port_pin-PORT_DIVIDER-1));
            break;
        case GPIO_OUT_INPUT_PP:
            P3_MOD_OC = P3_MOD_OC & ~(1<<(_io.port_pin-PORT_DIVIDER-1));
            P3_DIR_PU = P3_DIR_PU |	(1<<(_io.port_pin-PORT_DIVIDER-1));
            break;
        case GPIO_OUT_INPUT_OD_NO_PULL:
            P3_MOD_OC = P3_MOD_OC | (1<<(_io.port_pin-PORT_DIVIDER-1));
            P3_DIR_PU = P3_DIR_PU &	~(1<<(_io.port_pin-PORT_DIVIDER-1));
            break;
        case GPIO_OUT_INPUT_OD:
            P3_MOD_OC = P3_MOD_OC | (1<<(_io.port_pin-PORT_DIVIDER-1));
            P3_DIR_PU = P3_DIR_PU |	(1<<(_io.port_pin-PORT_DIVIDER-1));
            break;
        default:
            break;
    }
    }
}

void gpio_digital_write(gpio_type _io,uint8_t value) //写函数
{
    if (_io.port_pin < PORT_DIVIDER) {
        if(value){
            P1^_io.port_pin = 1;
            _io.status=1;
        }else{
            P1^_io.port_pin = 0;
            _io.status=0;
        }
    }else if (_io.port_pin > PORT_DIVIDER){
        if(value){
            P3^(_io.port_pin-PORT_DIVIDER-1) = 1;
            _io.status=1;
        }else{
            P3^(_io.port_pin-PORT_DIVIDER-1) = 0;
            _io.status=0;
        }
    }
}
uint8_t gpio_digital_read(io_type _io) //读函数
{
    if (_io.port_pin < PORT_DIVIDER) {
        _io.status=P1^_io.port_pin?1:0;
    }else if (_io.port_pin > PORT_DIVIDER){
        _io.status=P3^(_io.port_pin-PORT_DIVIDER-1)?1:0;
    }
    return _io.status;
}
