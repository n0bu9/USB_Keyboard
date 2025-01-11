#include "ch55x_conf.h"
#include "basic.h"
#include "main.h"
#include "Debug.h"
#if DEBUG
#include "usb_hid.h"
#endif


uint8d_t timer_count = 0;

void basic_init(void)
{
    // 初始化定时器0
    mTimer0Clk12DivFsys();                                                     //Timer0时钟选择
    mTimer_x_ModInit(0,1);                                                     //Timer0设置为16位定时器
    mTimer_x_SetData(0,1000);                                                  //设置定时器0初值 = 1ms
    ET0 = 1;                                                                   //开启定时器0中断

}

// 清除定时器0计数
void timer_clear(void)
{
    #if defined(__C51__)
    uint8_t __ToDo;
    #endif

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        timer_count = 0;
    }
}

// 以16位读取定时器0计数
uint16_t timer_read(void)
{
    uint32_t t;
    #if defined(__C51__)
    uint8_t __ToDo;
    #endif

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = timer_count;
    }

    return (t & 0xFFFF);
}

// 以32位读取定时器0计数
uint32_t timer_read32(void) {
    uint32_t t;
    #if defined(__C51__)
    uint8_t __ToDo;
    #endif

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = timer_count;
    }

    return t;
}

// 以16位读取定时器0计数
uint16_t timer_elapsed(uint16_t last)
{
    uint32_t t;
    #if defined(__C51__)
    uint8_t __ToDo;
    #endif

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = timer_count;
    }

    return TIMER_DIFF_16((t & 0xFFFF), last);
}

// 以32位读取定时器0计数
uint32_t timer_elapsed32(uint32_t last)
{
    uint32_t t;
    #if defined(__C51__)
    uint8_t __ToDo;
    #endif

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = timer_count;
    }

    return TIMER_DIFF_32(t, last);
}

// 定时器0中断服务程序
void timer0_interrupt( void ) interrupt INT_NO_TMR0 using 3     //1MS         //timer0中断服务程序,使用寄存器组3
{
    timer_count++;                                                             //清除定时器0中断标志
    mTimer_x_SetData(0,1000);                                                  //设置定时器0初值 = 1ms
}

void feed_wdt(void)
{
    WDOG_COUNT = 0;
}

void stop_wdt(void)
{
    GLOBAL_CFG &= ~bWDOG_EN;
}

void enable_wdt(void)
{
    GLOBAL_CFG |= bWDOG_EN;
}

/*******************************************************************************
* Function Name  : QueryUART1Interrupt(void)
* Description    : UART0中断服务程序
*******************************************************************************/
#if DEBUG
void query_uart0_interrupt(void) interrupt INT_NO_UART0 using 2             //UART0中断服务程序,使用寄存器组2
{
    uint8_t rec;
    if(RI)
    {
        rec = SBUF;
        RI = 0;
        get_keyboard_data(rec);
    }
    TI = 0;
}
#endif
