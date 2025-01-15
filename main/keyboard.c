#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "ch55x_conf.h"

#include "keyboard.h"

#pragma  NOAREGS

sbit rows1 = P1^2;
sbit rows2 = P1^3;
sbit rows3 = P3^5;
sbit rows4 = P3^3;
sbit rows5 = P1^0;

sbit cols1 = P3^0;
sbit cols2 = P3^1;
sbit cols3 = P1^4;
sbit cols4 = P3^2;

// static volatile uint8_t key_state[KEY_NUM];

void keyboard_init(void)
{
    // Initialize the keyboard
    // 开漏输入输出，有上拉
    gpio_init(PORT_1_PIN_2, GPIO_OUT_INPUT_OD);  // ROWS1
    gpio_init(PORT_1_PIN_3, GPIO_OUT_INPUT_OD);  // ROWS2
    gpio_init(PORT_3_PIN_5, GPIO_OUT_INPUT_OD);  // ROWS3
    gpio_init(PORT_3_PIN_3, GPIO_OUT_INPUT_OD);  // ROWS4
    gpio_init(PORT_1_PIN_0, GPIO_OUT_INPUT_OD);  // ROWS5
    gpio_init(PORT_3_PIN_0, GPIO_OUT_INPUT_OD);  // COLS1
    gpio_init(PORT_3_PIN_1, GPIO_OUT_INPUT_OD);  // COLS2
    gpio_init(PORT_1_PIN_4, GPIO_OUT_INPUT_OD);  // COLS3
    gpio_init(PORT_3_PIN_2, GPIO_OUT_INPUT_OD);  // COLS4
}

key_rec_enum keyboard_scan(void)
{
    // 扫描键盘
    rows1 = 1;rows2 = 1;rows3 = 1;rows4 = 1;rows5 = 1;
    {
        // 扫描第一行
        rows1 = 0;
        if (cols1 == 0) {
            return KEY_LOCK;
        }
        if (cols2 == 0) {
            return KEY_DIVIDE;
        }
        if (cols3 == 0) {
            return KEY_MULTIPLY;
        }
        if (cols4 == 0) {
            return KEY_MINUS;
        }
        rows1 = 1;
    }
    {
        // 扫描第二行
        rows2 = 0;
        if (cols1 == 0) {
            return KEY_PLUS;
        }
        if (cols2 == 0) {
            return KEY_9;
        }
        if (cols3 == 0) {
            return KEY_8;
        }
        if (cols4 == 0) {
            return KEY_7;
        }
        rows2 = 1;
    }
    {
        // 扫描第三行
        rows3 = 0;
        if (cols1 == 0) {
            return KEY_6;
        }
        if (cols2 == 0) {
            return KEY_5;
        }
        if (cols3 == 0) {
            return KEY_4;
        }
        rows3 = 1;
    }
    {
        // 扫描第四行
        rows4 = 0;
        if (cols1 == 0) {
            return KEY_3;
        }
        if (cols2 == 0) {
            return KEY_2;
        }
        if (cols3 == 0) {
            return KEY_1;
        }
        rows4 = 1;
    }
    {
        // 扫描第五行
        rows5 = 0;
        if (cols1 == 0) {
            return KEY_0;
        }
        if (cols2 == 0) {
            return KEY_DOT;
        }
        if (cols3 == 0) {
            return KEY_ENTER;
        }
        rows5 = 1;
    }
    return KEY_NONE;
}

