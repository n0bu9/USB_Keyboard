
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
#include "CH554.H"                                                   
#include "Debug.H"
#include "gpio.h"
#include "timer.h"
#include "stdio.h"
#include <string.h>

#pragma  NOAREGS

sbit LED0 = P3^0;

void main( ) 
{
    UINT16 j = 0;
    CfgFsys( );                                                                //CH554时钟选择配置   
    mDelaymS(20);
    mInitSTDIO( );                                                             //串口0初始化
    printf("start ...\n"); 
    Port3Cfg(1,0);  
    mTimer0Clk12DivFsys();                                                     //Timer0时钟选择
    mTimer_x_ModInit(0,1);                                                     //Timer0设置为16位定时器
    mTimer_x_SetData(0,8000);                                                  //设置定时器0初值 = 1ms
    ET0 = 1;                                                                   //开启定时器0中断
	EA = 1;                                                                    //开启总中断                 

    mTimer0RunCTL(1);
    LED0 = 0;
    printf("Run"); 
    while(1){
//      printf(".");
    //   LED0 = ~LED0;		
      mDelaymS(100);			
    }
}

void mTimer0Interrupt( void ) interrupt INT_NO_TMR0 using 1                //timer0中断服务程序,使用寄存器组1
{
    mTimer_x_SetData(0,8000);                                                  //设置定时器0初值 = 1ms                                                                           //方式3时，TH0使用Timer1的中断资源
    LED0 = ~LED0;	
//     mTimer_x_SetData(0,0x0000);                                          //非自动重载方式需重新给TH0和TL0赋值      
}