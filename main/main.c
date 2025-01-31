// driver layer
#include "ch55x_conf.h"

// application layer
#include "timer.h"
#include "uart.h"
#include "keyboard.h"
#include "debounce.h"
#include "main.h"
#include "usb_hid.h"
#include "stdio.h"
#include "string.h"
#include "keymap.h"

#pragma  NOAREGS

extern volatile matrix_row_t raw_key_state[MATRIX_ROWS];
extern volatile matrix_row_t cooked_key_state[MATRIX_ROWS];

void led_flash_proc(void)
{
    static uint16_t time = 0;
    static uint8_t is_init = 1;   // 添加初始化标志
    static gpio_type led0;

    if(is_init) {                 // 首次运行时初始化
        is_init = 0;
        time = timer_read();
        led0 = PORT_3_PIN_0;
        gpio_init(led0, GPIO_OUT_INPUT_PP);
    }

    if(timer_elapsed(time) > 1000) {
        gpio_digital_write(led0, !gpio_digital_read(led0));
        time = timer_read();
    }
}

void uart_debug_proc(void)
{
    static uint16_t time = 0;
    static uint8_t is_init = 1;   // 添加初始化标志
    // int8_t debug_str[30];
    int i = 0;

    if (is_init) {
        time = timer_read();
        is_init = 0;
    }
    if (timer_elapsed(time) > 500) {
        time = timer_read();
    }
}

void keyboard_proc(void)
{
    static bool is_init = 1;   // 添加初始化标志
    static volatile matrix_row_t *raw_key_state;
    static volatile matrix_row_t *cooked_key_state;
    bool changed = FALSE;
    bool cooked_changed = FALSE;
    bool is_key_none = FALSE;

    if (is_init) {
        is_init = 0;
        raw_key_state = get_raw_key_state_ptr();
        cooked_key_state = get_cooked_key_state_ptr();
    }
    keyboard_scan_all();
    changed = if_state_changed();
    #ifdef DEBUG
        if (changed) uart_send_string(0, "True\r\n");
    #endif
    cooked_changed = sym_defer_get_debounce(raw_key_state, cooked_key_state, MATRIX_ROWS, changed);
    if (cooked_changed) report_key_by_map(cooked_key_state);
}


void main( )
{
    system_init();                                                             //CH554时钟选择配置
    delay_ms(20);                                                              //修改主频，建议稍加延时等主频稳定
    #ifdef DEBUG
        uart0_init();                                                              //串口0初始化
        ch55x_uart0_alter();                                                       //串口0引脚映射 RX-P1.2和TX-P1.3
        printf("start\r\n");
    #endif
    usb_device_init();                                                         //USB设备初始化
    timer_init();                                                              //基本外设初始化
    wdt_set_time(0x00);                                                        //看门狗复位时间设置
    wdt_mode_select(1);                                                        //看门狗作为复位
    keyboard_init();                                                           //键盘初始化
	EA = 1;                                                                    //开启总中断

    mTimer0RunCTL(1);                                                          //启动定时器0
    usb_clear_flag();                                                          //清除USB标志
    while(1){
        // led_flash_proc();
        // uart_debug_proc();
        keyboard_proc();

        wdt_feed();                                                           //喂狗
    }
}


