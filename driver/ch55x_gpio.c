#include "ch55x_conf.h"

void gpio_init(gpio_type _io, gpio_mode_enum mode) //其初始化
{
    #if defined(__C51__)
    if (_io < PORT_DIVIDER){
        switch(mode){
        case GPIO_INPUT_NO_PULL:
            P1_MOD_OC = P1_MOD_OC & ~(1<<_io);
            P1_DIR_PU = P1_DIR_PU &	~(1<<_io);
            break;
        case GPIO_OUT_INPUT_PP:
            P1_MOD_OC = P1_MOD_OC & ~(1<<_io);
            P1_DIR_PU = P1_DIR_PU |	(1<<_io);
        break;
        case GPIO_OUT_INPUT_OD_NO_PULL:
            P1_MOD_OC = P1_MOD_OC | (1<<_io);
            P1_DIR_PU = P1_DIR_PU &	~(1<<_io);
            break;
        case GPIO_OUT_INPUT_OD:
            P1_MOD_OC = P1_MOD_OC | (1<<_io);
            P1_DIR_PU = P1_DIR_PU |	(1<<_io);
            break;
        default:
            break;
        }
    }else if (_io > PORT_DIVIDER){
        switch(mode){
        case GPIO_INPUT_NO_PULL:
            P3_MOD_OC = P3_MOD_OC & ~(1<<(_io-PORT_DIVIDER-1));
            P3_DIR_PU = P3_DIR_PU &	~(1<<(_io-PORT_DIVIDER-1));
            break;
        case GPIO_OUT_INPUT_PP:
            P3_MOD_OC = P3_MOD_OC & ~(1<<(_io-PORT_DIVIDER-1));
            P3_DIR_PU = P3_DIR_PU |	(1<<(_io-PORT_DIVIDER-1));
            break;
        case GPIO_OUT_INPUT_OD_NO_PULL:
            P3_MOD_OC = P3_MOD_OC | (1<<(_io-PORT_DIVIDER-1));
            P3_DIR_PU = P3_DIR_PU &	~(1<<(_io-PORT_DIVIDER-1));
            break;
        case GPIO_OUT_INPUT_OD:
            P3_MOD_OC = P3_MOD_OC | (1<<(_io-PORT_DIVIDER-1));
            P3_DIR_PU = P3_DIR_PU |	(1<<(_io-PORT_DIVIDER-1));
            break;
        default:
            break;
        }
    }
    #else   
    #endif
}

#if defined(__C51__)
void gpio_digital_write(gpio_type _io,bool value) //写函数
{
    if (_io < PORT_DIVIDER) {
        if(value){
            P1 = P1 | (1<<_io);
        }else{
            P1 = P1 & ~(1<<_io);
        }
    }else if (_io > PORT_DIVIDER){
        if(value){
            P3 = P3 | (1<<(_io-PORT_DIVIDER-1));
        }else{
            P3 = P3 & ~(1<<(_io-PORT_DIVIDER-1));
        }
    }
}
#else
#endif

#if defined(__C51__)
bool gpio_digital_read(gpio_type _io) //读函数
{
    if (_io < PORT_DIVIDER) {
        return (bool)(P1^_io);
    }else if (_io > PORT_DIVIDER){
        return (bool)(P3^(_io-PORT_DIVIDER-1));
    }
    return (bool)0;
}
#else
#endif
