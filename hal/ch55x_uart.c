#include "ch55x_conf.h"
#include "system_ch55x.h"


#if USE_UART0_ENABLE
/*******************************************************************************
* Function Name  : ch55x_uart0_init()
* Description    : CH554串口0初始化,默认使用T1作UART0的波特率发生器,也可以使用T2
                   作为波特率发生器
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void uart0_init(void)
{
    uint32_t x;
    uint8_t x2;

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
    TI = 1;                                                                    //串口0发送使能                           
    REN = 1;                                                                   //串口0接收使能
}
/*******************************************************************************
* Function Name  : ch55x_uart0_alter()
* Description    : CH554串口0引脚映射,串口映射到P1.2和P1.3
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ch55x_uart0_alter(void)
{
    PIN_FUNC |= bUART0_PIN_X;                                                  //串口映射到P1.2和P1.3
}
/*******************************************************************************
* Function Name  : hal_uart0_rcv_byte()
* Description    : CH554 UART0接收一个字节
* Input          : None
* Output         : None
* Return         : SBUF
*******************************************************************************/
uint8_t uart0_rcv_byte(void)
{
    while(RI == 0);                                                            //查询接收，中断方式可不用
    RI = 0;
    return SBUF;
}
/*******************************************************************************
* Function Name  : hal_uart0_send_byte(uint8_t SendDat)
* Description    : CH554 UART0发送一个字节
* Input          : uint8_t SendDat；要发送的数据
* Output         : None
* Return         : None
*******************************************************************************/
void uart0_send_byte(uint8_t SendDat)
{
    SBUF = SendDat;
    while(TI == 0);
    TI = 0;
}
#endif

#if USE_UART1_ENABLE
/*******************************************************************************
* Function Name  : hal_uart1_init()
* Description    : CH554串口1初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void uart1_init(void)
{
	U1SM0 = 0;                                                                   //UART1选择8位数据位
    #if UART1_FAST_MODE_ENABLE
	U1SMOD = 1;                                                                  //快速模式
    SBAUD1 = 0 - FREQ_SYS/16/UART1_BUAD;
    #else
    U1SMOD = 0;                                                                  //慢速模式
    SBAUD1 = 0 - FREQ_SYS/32/UART1_BUAD;
    #endif
    U1REN = 1;                                                                   //使能接收
	U1TI = 0;
}

void ch55x_uart1_alter(void)
{
    PIN_FUNC |= bUART1_PIN_X;                                                   //映射到P34(R)、P32(T)
}

/*******************************************************************************
* Function Name  : hal_uart1_rcv_byte()
* Description    : CH554 UART1接收一个字节
* Input          : None
* Output         : None
* Return         : SBUF1
*******************************************************************************/
uint8_t uart1_rcv_byte(void)
{
    while(U1RI == 0);                                                           //查询接收，中断方式可不用
    U1RI = 0;
    return SBUF1;
}

/*******************************************************************************
* Function Name  : hal_uart1_send_byte(uint8_t SendDat)
* Description    : CH554 UART1发送一个字节
* Input          : uint8_t SendDat；要发送的数据
* Output         : None
* Return         : None
*******************************************************************************/
void uart1_send_byte(uint8_t SendDat)
{
	SBUF1 = SendDat;                                                             //查询发送，中断方式可不用下面2条语句,但发送前需TI=0
	while(U1TI ==0);
	U1TI = 0;
}
#endif


