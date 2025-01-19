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
#include "uart.h"
#include "keyboard.h"
//#include "basic.h"
// #include "iap.h"
#include "main.h"
#include "usb_hid.h"
#include "stdio.h"
// #include <string.h>

#pragma  NOAREGS

void led_flash_proc(void)
{
    static uint16_t led_count = 0;
    static uint8_t is_init = 1;   // 添加初始化标志
    static gpio_type led0;

    if(is_init) {                 // 首次运行时初始化
        led_count = timer_read();
        is_init = 0;
        led0 = PORT_3_PIN_0;
        gpio_init(led0, GPIO_OUT_INPUT_PP);
    }

    if(timer_elapsed(led_count) > 1000) {
        gpio_digital_write(led0, !gpio_digital_read(led0));
        led_count = timer_read();
    }
}

void uart_debug_proc(void)
{
    static uint16_t uart_count = 0;
    static uint8_t is_init = 1;   // 添加初始化标志

    if (is_init) {
        uart_count = timer_read();
        is_init = 0;
    }
    if (timer_elapsed(uart_count) > 1000) {
        uart_send_string(0, "uart_debug\r\n");
        uart_count = timer_read();
    }
}

void keyboard_proc(void)
{
    static uint16_t keyboard_count = 0;
    static uint8_t is_init = 1;   // 添加初始化标志
    key_rec_enum key = KEY_NONE;

    if (is_init) {
        keyboard_count = timer_read();
        is_init = 0;
    }
    if (timer_elapsed(keyboard_count) > 100) {
        key = keyboard_scan();
        if (key != KEY_NONE) {
            switch (key) {
                case KEY_LOCK:
                    // uart_send_string(0, "LOCK\r\n");
                    break;
                case KEY_DIVIDE:
                    // uart_send_string(0, "DIVIDE\r\n");
                    break;
                case KEY_MULTIPLY:
                    break;
                case KEY_MINUS:
                    break;
                case KEY_PLUS:
                    break;
                case KEY_9:
                    get_keyboard_data('9');
                    break;
                case KEY_8:
                    get_keyboard_data('8');
                    break;
                case KEY_7:
                    get_keyboard_data('7');
                    break;
                case KEY_6:
                    get_keyboard_data('6');
                    break;
                case KEY_5:
                    get_keyboard_data('5');
                    break;
                case KEY_4:
                    get_keyboard_data('4');
                    break;
                case KEY_3:
                    get_keyboard_data('3');
                    break;
                case KEY_2:
                    get_keyboard_data('2');
                    break;
                case KEY_1:
                    get_keyboard_data('1');
                    break;
                case KEY_0:
                    get_keyboard_data('0');
                    break;
                case KEY_DOT:
                    break;
                case KEY_ENTER:
                    break;
            }

        }
        keyboard_count = timer_read();
    }
}


void main( )
{
    system_init();                                                             //CH554时钟选择配置
    delay_ms(20);                                                              //修改主频，建议稍加延时等主频稳定
    uart0_init();                                                              //串口0初始化
    ch55x_uart0_alter();                                                       //串口0引脚映射 RX-P1.2和TX-P1.3
    printf("start\r\n");
    usb_device_init();                                                         //USB设备初始化
    timer_init();                                                              //基本外设初始化
    wdt_set_time(0x00);                                                        //看门狗复位时间设置
    wdt_mode_select(1);                                                        //看门狗作为复位
    keyboard_init();                                                           //键盘初始化
	EA = 1;                                                                    //开启总中断

    mTimer0RunCTL(1);                                                          //启动定时器0
    usb_clear_flag();                                                          //清除USB标志
    while(1){
        led_flash_proc();
        // uart_debug_proc();
        keyboard_proc();
        hid_value_handle_proc();                                              //处理USB HID数据

        wdt_feed();                                                           //喂狗
    }
}


