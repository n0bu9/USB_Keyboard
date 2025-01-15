#include "ch55x_conf.h"
#include "CH554.H"

/*******************************************************************************
* Function Name  : ch55x_wdt_mode_select(uint8_t mode)
* Description    : CH554看门狗模式选择
* Input          : uint8_t mode
                   0  timer
                   1  watchDog
* Output         : None
* Return         : None
*******************************************************************************/
void wdt_mode_select(uint8_t mode)
{
   SAFE_MOD = 0x55;
   SAFE_MOD = 0xaa;                                                             //进入安全模式
   if(mode){
     GLOBAL_CFG |= bWDOG_EN;                                                    //启动看门狗复位
   }
   else GLOBAL_CFG &= ~bWDOG_EN;	                                            //启动看门狗仅仅作为定时器
   SAFE_MOD = 0x00;                                                             //退出安全模式
   WDOG_COUNT = 0;                                                              //看门狗赋初值
}

/*******************************************************************************
* Function Name  : ch55x_wdt_set_time(uint8_t tim)
* Description    : CH554看门狗定时时间设置
* Input          : uint8_t tim 看门狗复位时间设置
                   00H(6MHz)=2.8s
                   80H(6MHz)=1.4s
* Output         : None
* Return         : None
*******************************************************************************/
void wdt_set_time(uint8_t tim)
{
   WDOG_COUNT = tim;                                                             //看门狗计数器赋值
}

/*******************************************************************************
* Function Name  : ch55x_wdt_feed(void)
* Description    : CH554看门狗复位(喂狗)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void wdt_feed(void)
{
    WDOG_COUNT = 0;
}

