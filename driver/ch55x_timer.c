#include "ch55x_conf.h"

/*******************************************************************************
* Function Name  : mTimer_x_ModInit(uint8_t x ,uint8_t mode)
* Description    : CH554定时计数器x模式设置
* Input          : uint8_t mode,Timer模式选择
                   0：模式0，13位定时器，TLn的高3位无效
                   1：模式1，16位定时器
                   2：模式2，8位自动重装定时器
                   3：模式3，两个8位定时器  Timer0
                   3：模式3，Timer1停止		
                   uint8_t x 定时器  0 1 2
* Output         : None
* Return         : 成功  SUCCESS
                   失败  FAIL
*******************************************************************************/
uint8_t mTimer_x_ModInit(uint8_t x, uint8_t mode)
{
    if(x == 0){TMOD = TMOD & 0xf0 | mode;}
    else if(x == 1){TMOD = TMOD & 0x0f | (mode<<4);}
    else if(x == 2){RCLK = 0;TCLK = 0;CP_RL2 = 0;}                               //16位自动重载定时器
    else return FAIL;
    return SUCCESS;
}
/*******************************************************************************
* Function Name  : mTimer_x_SetData
* Description    : CH554Timer 
* Input          : uint8_t x,定时器 0 1 2
                   uint16_t dat,定时器赋值
* Output         : None
* Return         : None
*******************************************************************************/
void mTimer_x_SetData(uint8_t x,uint16_t dat)
{
    uint16_t tmp;
    tmp = 65536 - dat;	
    if(x == 0){TL0 = tmp & 0xff;TH0 = (tmp>>8) & 0xff;}
    else if(x == 1){TL1 = tmp & 0xff;TH1 = (tmp>>8) & 0xff;}
    else if(x == 2){
        RCAP2L = TL2 = tmp & 0xff;                                               //16位自动重载定时器
        RCAP2H = TH2 = (tmp>>8) & 0xff;
    }  
}
/*******************************************************************************
* Function Name  : CAP2Init(uint8_t mode)
* Description    : CH554定时计数器2 T2EX引脚捕捉功能初始化
                   uint8_t mode,边沿捕捉模式选择
                   0:T2ex从下降沿到下一个下降沿
                   1:T2ex任意边沿之间
                   3:T2ex从上升沿到下一个上升沿
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAP2Init(uint8_t mode)
{
    RCLK = 0;
    TCLK = 0;	
    C_T2  = 0;
    EXEN2 = 1; 
    CP_RL2 = 1;                                                                //启动T2ex的捕捉功能
    T2MOD |= mode << 2;                                                        //边沿捕捉模式选择
}

/*******************************************************************************
* Function Name  : CAP1Init(uint8_t mode)
* Description    : CH554定时计数器2 T2引脚捕捉功能初始化T2
                   uint8_t mode,边沿捕捉模式选择
                   0:T2ex从下降沿到下一个下降沿
                   1:T2ex任意边沿之间
                   3:T2ex从上升沿到下一个上升沿
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAP1Init(uint8_t mode)
{
    RCLK = 0;
    TCLK = 0;
    CP_RL2 = 1;
    C_T2 = 0;
    T2MOD = T2MOD & ~T2OE | (mode << 2) | bT2_CAP1_EN;                         //使能T2引脚捕捉功能,边沿捕捉模式选择
}

/*中断函数参考*/

// /*******************************************************************************
// * Function Name  : mTimer0Interrupt()
// * Description    : CH554定时计数器0定时计数器中断处理函数
// *******************************************************************************/
// void	mTimer0Interrupt( void ) interrupt INT_NO_TMR0 using 1                //timer0中断服务程序,使用寄存器组1
// {                                                                           //方式3时，TH0使用Timer1的中断资源
//     SCK = ~SCK;
// //     mTimer_x_SetData(0,0x0000);                                          //非自动重载方式需重新给TH0和TL0赋值      
// }

// /*******************************************************************************
// * Function Name  : mTimer1Interrupt()
// * Description    : CH554定时计数器0定时计数器中断处理函数
// *******************************************************************************/
// void	mTimer1Interrupt( void ) interrupt INT_NO_TMR1 using 2                //timer1中断服务程序,使用寄存器组2
// {                                                                           //方式3时，Timer1停止
//     SCK = ~SCK;
// //     mTimer_x_SetData(1,0x0000);                                          //非自动重载方式需重新给TH1和TL1赋值      
// }

// /*******************************************************************************
// * Function Name  : mTimer2Interrupt()
// * Description    : CH554定时计数器0定时计数器中断处理函数
// *******************************************************************************/
// void	mTimer2Interrupt( void ) interrupt INT_NO_TMR2 using 3                //timer2中断服务程序,使用寄存器组3
// {                                                                             
//     mTimer2RunCTL( 0 );                                                     //关定时器
//     if(EXF2)                                                                //T2ex电平变化中断中断标志
//     {
//         SCK = ~SCK;                                                         //P17电平指示监控
//         Cap[FLAG++] = RCAP2;                                                //T2EX
//         printf("RCAP2 %04x  \n",RCAP2);                                  			
//         EXF2 = 0;                                                           //清空T2ex捕捉中断标志		
//     }
//     if(CAP1F)                                                               //T2电平捕捉中断标志
//     {
//         Cap[FLAG++] = T2CAP1;                                               //T2;	  	
//         printf("T2CAP1 %04x  \n",T2CAP1);				
//         CAP1F = 0;                                                          //清空T2捕捉中断标志
//     }
//     if(TF2)
//     {
//         TF2 = 0;                                                             //清空定时器2溢出中断	                                                      
//         SCK = ~SCK;                                                          //P17电平指示监控
//     }
//     mTimer2RunCTL( 1 );                                                      //开定时器   
// }

