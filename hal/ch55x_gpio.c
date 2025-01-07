#include "ch55x_conf.h"

void io_mode(io_type _io,GPIOModeTypeDef mode) //其初始化
{
    if (_io.port==PORT_A) {
        GPIOA_ModeCfg( _io.pin, mode);
    }else{
        GPIOB_ModeCfg( _io.pin, mode);
    }
}

void io_digital_write(io_type _io,uint8_t value) //写函数
{
    if (_io.port==PORT_A) {
        if(value){
            GPIOA_SetBits(_io.pin);
            _io.status=1;
        }else{
            GPIOA_ResetBits(_io.pin);
            _io.status=0;
        }
    }else{
        if(value){
            GPIOB_SetBits(_io.pin);
            _io.status=1;
        }else{
            GPIOB_ResetBits(_io.pin);
            _io.status=0;
        }
    }
}
uint8_t io_digital_read(io_type _io) //读函数
{
    if (_io.port==PORT_A) {
        _io.status=GPIOA_ReadPortPin(_io.pin)?1:0;
    }else{
        _io.status=GPIOB_ReadPortPin(_io.pin)?1:0;
    }
    return _io.status;
}
