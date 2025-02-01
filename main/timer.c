#include "timer.h"
#include "ch55x_timer.h"

static uint32d_t timer_count = 0;

/*  ******************************************************************************
* Function Name  : timer_init()
* Description    : 初始化心跳时间
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void timer_init(void)
{
    mTimer0Clk12DivFsys();                                                     //Timer0时钟选择
    timer_mod_init(0,1);                                                     //Timer0设置为16位定时器
    timer_set_data(0,1000);                                                  //设置定时器0初值 = 1ms
    ET0 = 1;                                                                   //开启定时器0中断
}

/*  ******************************************************************************
* Function Name  : timer_clear()
* Description    : 清除心跳时间
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void timer_clear(void)
{
    #if defined(__C51__)
    uint8_t __ToDo;
    #endif

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        timer_count = 0;
    }
}

/*  ******************************************************************************
* Function Name  : timer_read()
* Description    : 以16位读取当前心跳时间
* Input          : None
* Output         : None
* Return         : 当前心跳时间(16位)
*******************************************************************************/
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

/*  ******************************************************************************
* Function Name  : timer_read32()
* Description    : 以32位读取当前心跳时间
* Input          : None
* Output         : None
* Return         : 当前心跳时间(32位)
*******************************************************************************/
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

/*  ******************************************************************************
* Function Name  : timer_elapsed()
* Description    : 计算时间差(当前心跳时间 - 上一次心跳时间)
* Input          : last:上一次心跳时间
* Output         : None
* Return         : 时间差
*******************************************************************************/
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

/*  ******************************************************************************
* Function Name  : timer_elapsed32()
* Description    : 计算时间差(当前心跳时间 - 上一次心跳时间)
* Input          : last:上一次心跳时间
* Output         : None
* Return         : 时间差(32位)
*******************************************************************************/
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

/*  ******************************************************************************
* Function Name  : timer_delay()
* Description    : 阻塞延时
* Input          : ms:延时时间
* Output         : None
* Return         : None
*******************************************************************************/
void timer_delay(uint32_t ms)
{
    uint32_t start = timer_read();
    while (timer_elapsed(start) < ms);
}

/*  ******************************************************************************
* Function Name  : timer0_interrupt()
* Description    : 定时器0中断服务程序
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void timer0_interrupt( void ) interrupt INT_NO_TMR0 using 3     //1MS         //timer0中断服务程序,使用寄存器组3
{
    timer_count++;                                                             //清除定时器0中断标志
    timer_set_data(0, 1000);                                                  //设置定时器0初值 = 1ms
}

