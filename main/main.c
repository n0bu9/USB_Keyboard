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
// driver layer
#include "ch55x_conf.h"

// application layer
#include "timer.h"
#include "keyboard.h"
#include "basic.h"
#include "iap.h"
#include "main.h"
#include "usb_hid.h"
#include "stdio.h"
#include <string.h>

#pragma  NOAREGS

void led_flash(void)
{
    static uint16_t led_count = 0;
    static uint8_t is_init = 1;   // 添加初始化标志
    static gpio_type led0;

    if(is_init) {                 // 首次运行时初始化
        led_count = timer_read();
        is_init = 0;
        led0.port_pin = PORT_3_PIN_0;
        gpio_init(led0, GPIO_MODE_OUTPUT);
    }

    if(timer_elapsed(led_count) > 1000) {
        gpio_digital_write(led0, !gpio_digital_read(led0));
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
    if (timer_elapsed(uart_count) > 500) {
        printf("uart_debug\n");
        uart_count = timer_read();
    }
}

void keyboard_test(void)
{
    static uint16_t keyboard_count = 0;
    static uint8_t is_init = 1;   // 添加初始化标志
    uint8_t key_state[KEY_NUM];
    uint8_t i;

    if (is_init) {
        keyboard_count = timer_read();
        is_init = 0;
    }
    if (timer_elapsed(keyboard_count) > 50) {
        keyboard_get_state(key_state);
        for (i = 0; i < KEY_NUM; i++) {
            if (key_state[i] == 1) {
                printf("Key %d is pressed\n", i);
            }
        }
        keyboard_count = timer_read();
    }
}

void feed_wdt_proc(void)
{
    feed_wdt();
}

void main( )
{
    UINT16 j = 0;
    CfgFsys( );                                                                //CH554时钟选择配置
    mDelaymS(20);                                                              //修改主频，建议稍加延时等主频稳定
    uart0_init();                                                              //串口0初始化
    printf("start\n");
    // usb_device_init();                                                         //USB设备初始化
    // basic_init();                                                              //基本外设初始化
    // CH554WDTFeed(0x00);                                                        //看门狗复位时间设置
    // CH554WDTModeSelect(1);                                                     //看门狗作为复位
	EA = 1;                                                                    //开启总中断

    mTimer0RunCTL(1);                                                          //启动定时器0
    // usb_clear_flag();                                                          //清除USB设备状态
    while(1){
        // feed_wdt_proc();
        led_flash();
        // keyboard_test();
        // keyboard_scan();
        // uart_debug();
        // hid_value_handle();
    }
}


