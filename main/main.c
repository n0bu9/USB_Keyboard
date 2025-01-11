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
// #include "timer.h"
// #include "keyboard.h"
#include "basic.h"
// #include "iap.h"
// #include "main.h"
// #include "usb_hid.h"
#include "stdio.h"
// #include <string.h>

#pragma  NOAREGS

void timer_init(void)
{
    mTimer0Clk12DivFsys();                                                     //Timer0时钟选择
    mTimer_x_ModInit(0,1);                                                     //Timer0设置为16位定时器
    mTimer_x_SetData(0,1000);                                                  //设置定时器0初值 = 1ms
    ET0 = 1;                                                                   //开启定时器0中断
}

void led_flash(void)
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

void uart_debug(void) {
    static uint16_t uart_count = 0;
    static uint8_t is_init = 1;   // 添加初始化标志
    const uint8_t send_data[] = "uart_debug\r\n";
    uint8_t send_size = 0;

    if (is_init) {
        uart_count = timer_read();
        is_init = 0;
    }
    if (timer_elapsed(uart_count) > 500) {
        for (send_size = 0; send_size < sizeof(send_data); send_size++) {
            uart0_send_byte(send_data[send_size]);
        }
        uart_count = timer_read();
    }
}

// void keyboard_test(void)
// {
//     static uint16_t keyboard_count = 0;
//     static uint8_t is_init = 1;   // 添加初始化标志
//     uint8_t key_state[KEY_NUM];
//     uint8_t i;

//     if (is_init) {
//         keyboard_count = timer_read();
//         is_init = 0;
//     }
//     if (timer_elapsed(keyboard_count) > 50) {
//         keyboard_get_state(key_state);
//         for (i = 0; i < KEY_NUM; i++) {
//             if (key_state[i] == 1) {
//                 printf("Key %d is pressed\n", i);
//             }
//         }
//         keyboard_count = timer_read();
//     }
// }

// void feed_wdt_proc(void)
// {
//     feed_wdt();
// }

void main( )
{
    uint32_t i = 0;
    uint32_t time = 0;
    system_init();                                                             //CH554时钟选择配置
    delay_ms(20);                                                              //修改主频，建议稍加延时等主频稳定
    uart0_init();                                                              //串口0初始化
    ch55x_uart0_alter();                                                       //串口0引脚映射 RX-P1.2和TX-P1.3       
    printf("start\n");
    // usb_device_init();                                                      //USB设备初始化
    timer_init();                                                              //基本外设初始化
    // CH554WDTFeed(0x00);                                                     //看门狗复位时间设置
    // CH554WDTModeSelect(1);                                                  //看门狗作为复位
	EA = 1;                                                                    //开启总中断

    mTimer0RunCTL(1);                                                          //启动定时器0   // usb_clear_flag();                                                       //清除USB设备状态
    while(1){
        // feed_wdt_proc();
        // led_flash();
        // keyboard_test();
        // keyboard_scan();
        // uart_debug();
        // hid_value_handle();
        i++;
        if (i > 10000) {
            i = 0;
            time = timer_read();
            if (time > 10000) uart0_send_byte('0' + time/10000);
            if (time > 1000) uart0_send_byte('0' + time%10000/1000);
            if (time > 100) uart0_send_byte('0' + time%1000/100);
            if (time > 10) uart0_send_byte('0' + time%100/10);
            uart0_send_byte('0' + time%10);
            uart0_send_byte('\n');
        }
    }
}


