
/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.C
* Author             : WCH
* Version            : V1.1
* Date               : 2017/11/18
* Description        : GPIO 设置与使用和GPIO中断使用示例   
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
********************************************************************************/                                                 
#include "Debug.H"
#include "gpio.h"
#include "timer.h"
#include "keyboard.h"
#include "basic.h"

#include "stdio.h"
#include <string.h>

#pragma  NOAREGS

sbit LED0 = P3^0;

void led_flash(void)
{
    static UINT16 led_count = timer_read();

    if(timer_elapsed(led_count) > 500) {
        LED0 = !LED0;
        led_count = timer_read();
    }
}

void main( ) 
{
    UINT16 j = 0;
    CfgFsys( );                                                                //CH554时钟选择配置   
    mDelaymS(20);                                                              //修改主频，建议稍加延时等待主频稳定
    mInitSTDIO( );                                                             //串口0初始化
    Port3Cfg(1,0);  
    basic_init();   
	EA = 1;                                                                    //开启总中断                 

    mTimer0RunCTL(1);
    keyboard_init();
    LED0 = 0;
    printf("Run\n"); 
    while(1){		
      	led_flash();	
    }
}

