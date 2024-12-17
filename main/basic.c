#include "basic.h"
#include "timer.h"

UINT32 volatile data timer_count = 0;

void basic_init(void) 
{
    // 初始化定时器0
    mTimer0Clk12DivFsys();                                                      //Timer0时钟选择
    mTimer_x_ModInit(0,1);                                                     //Timer0设置为16位定时器
    mTimer_x_SetData(0,1000);                                                  //设置定时器0初值 = 1ms
    ET0 = 1;                                                                    //开启定时器0中断

}

// 清除定时器0计数
inline void timer_clear(void)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        timer_count = 0;
    }
}

// 以16位读取定时器0计数
inline uint16_t timer_read(void) {
    uint32_t t;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = timer_count;
    }

    return (t & 0xFFFF);
}

// 以32位读取定时器0计数
inline uint32_t timer_read32(void) {
    uint32_t t;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = timer_count;
    }

    return t;
}

// 以16位读取定时器0计数
inline uint16_t timer_elapsed(uint16_t last) {
    uint32_t t;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = timer_count;
    }

    return TIMER_DIFF_16((t & 0xFFFF), last);
}

// 以32位读取定时器0计数
inline uint32_t timer_elapsed32(uint32_t last) {
    uint32_t t;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = timer_count;
    }

    return TIMER_DIFF_32(t, last);
}

// 定时器0中断服务程序
void mTimer0Interrupt( void ) interrupt INT_NO_TMR0 using 1     //1MS         //timer0中断服务程序,使用寄存器组1
{
    timer_count++;
    TF0 = 0;                                                                   //清除定时器0中断标志
    mTimer_x_SetData(0,1000);                                                  //设置定时器0初值 = 1ms 
}

