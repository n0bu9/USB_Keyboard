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
#include "iap.h"
#include "main.h"

#include "stdio.h"
#include <string.h>

#pragma  NOAREGS

sbit LED0 = P3^0;

void led_flash(void)
{
    static uint16_t led_count = 0;
    static uint8_t is_init = 1;   // 添加初始化标志

    if(is_init) {                 // 首次运行时初始化
        led_count = timer_read();
        is_init = 0;
    }

    if(timer_elapsed(led_count) > 500) {
        LED0 = !LED0;
        led_count = timer_read();
    }
}

void uart_debug(void) {
    static uint16_t uart_count = 0;
    static uint8_t is_init = 1;   // 添加初始化标志

    if (is_init) {
        uart_count = timer_read();
        is_init = 0;
    }
    if (timer_elapsed(uart_count) > 700) {
        //printf("P36 Pin Level: %d\n", P3^6);
        uart_count = timer_read();
    }
}

void keyboard_test(void)
{
    static uint16_t keyboard_count = 0;
    static uint8_t is_init = 1;   // 添加初始化标志

    if (is_init) {
        keyboard_count = timer_read();
        is_init = 0;
    }
    if (timer_elapsed(keyboard_count) > 50) {
        if (keyboard_test_scan()) {
            iap_app();
        }
        keyboard_count = timer_read();
    }
}

void main( )
{
    UINT16 j = 0;
    CfgFsys( );                                                                //CH554时钟选择配置
    mDelaymS(20);                                                              //修改主频，建议稍加延时等主频稳定
    mInitSTDIO( );                                                             //串口0初始化
    Port3Cfg(1,0);
    basic_init();
	EA = 1;                                                                    //开启总中断

    mTimer0RunCTL(1);
    keyboard_test_init();
    LED0 = 0;
    printf("Run\n");
    while(1){
      	led_flash();
        keyboard_test();
        uart_debug();
    }
}

