#include "basic.h"
#include "timer.h"
#include "main.h"
#if DEBUG
#include "Debug.h"
#include "usb_hid.h"
#endif


UINT32 data timer_count = 0;

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
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        timer_count = 0;
    }
}

// 以16位读取定时器0计数
uint16_t timer_read(void)
{
    uint32_t t;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = timer_count;
    }

    return (t & 0xFFFF);
}

// 以32位读取定时器0计数
uint32_t timer_read32(void) {
    uint32_t t;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = timer_count;
    }

    return t;
}

// 以16位读取定时器0计数
uint16_t timer_elapsed(uint16_t last)
{
    uint32_t t;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = timer_count;
    }

    return TIMER_DIFF_16((t & 0xFFFF), last);
}

// 以32位读取定时器0计数
uint32_t timer_elapsed32(uint32_t last)
{
    uint32_t t;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = timer_count;
    }

    return TIMER_DIFF_32(t, last);
}

// 定时器0中断服务程序
void timer0_interrupt( void ) interrupt INT_NO_TMR0 using 3     //1MS         //timer0中断服务程序,使用寄存器组3
{
    timer_count++;
    TF0 = 0;                                                                   //清除定时器0中断标志
    mTimer_x_SetData(0,1000);                                                  //设置定时器0初值 = 1ms
}


/*******************************************************************************
* Function Name  : uart0_init()
* Description    : CH554串口0初始化,默认使用T1作UART0的波特率发生器,也可以使用T2
                   作为波特率发生器
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void uart0_init(void)
{
    UINT32 x;
    UINT8 x2;

    SM0 = 0;
    SM1 = 1;
    SM2 = 0;                                                                   //串口0使用模式1
                                                                               //使用Timer1作为波特率发生器
    RCLK = 0;                                                                  //UART0接收时钟
    TCLK = 0;                                                                  //UART0发送时钟
    PCON |= SMOD;
    x = 10 * FREQ_SYS / UART0_BUAD / 16;                                       //如果更改主频，注意x的值不要溢出
    x2 = x % 10;
    x /= 10;
    if ( x2 >= 5 ) x ++;                                                       //四舍五入

    TMOD = TMOD & ~ bT1_GATE & ~ bT1_CT & ~ MASK_T1_MOD | bT1_M1;              //0X20，Timer1作为8位自动重载定时器
    T2MOD = T2MOD | bTMR_CLK | bT1_CLK;                                        //Timer1时钟选择
    TH1 = 0-x;                                                                 //12MHz晶振,buad/12为实际需设置波特率
    TR1 = 1;                                                                   //启动定时器1
    TI = 1;
    #if DEBUG
    REN = 1;                                                                   //串口0接收使能
    #endif
}

/*******************************************************************************
* Function Name  : QueryUART1Interrupt(void)
* Description    : UART0中断服务程序
*******************************************************************************/
#if DEBUG
void query_uart0_interrupt(void) interrupt INT_NO_UART0 using 2             //UART0中断服务程序,使用寄存器组1
{
    uint8_t rec;
    if(RI)
    {
        rec = SBUF;
        get_keyboard_data(rec);
        RI = 0;
    }
}
#endif
