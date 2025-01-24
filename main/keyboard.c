#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "ch55x_conf.h"

#include "keyboard.h"

#pragma  NOAREGS



void keyboard_init(void)
{
    // Initialize the keyboard
    // 开漏输入输出，有上拉
    gpio_init(ROWS_1, GPIO_OUT_INPUT_OD);  // ROWS1
    gpio_init(ROWS_2, GPIO_OUT_INPUT_OD);  // ROWS2
    gpio_init(ROWS_3, GPIO_OUT_INPUT_OD);  // ROWS3
    gpio_init(ROWS_4, GPIO_OUT_INPUT_OD);  // ROWS4
    gpio_init(COLS_1, GPIO_OUT_INPUT_OD);  // COLS1
    gpio_init(COLS_2, GPIO_OUT_INPUT_OD);  // COLS2
    gpio_init(COLS_3, GPIO_OUT_INPUT_OD);  // COLS3
    gpio_init(COLS_4, GPIO_OUT_INPUT_OD);  // COLS4
    gpio_init(COLS_5, GPIO_OUT_INPUT_OD);  // COLS5
    gpio_digital_write(ROWS_1, 1);
    gpio_digital_write(ROWS_2, 1);
    gpio_digital_write(ROWS_3, 1);
    gpio_digital_write(ROWS_4, 1);
}

key_rec_enum keyboard_scan_line1(void)
{
    // 扫描第一行
    gpio_digital_write(ROWS_1, 0);
    if (gpio_digital_read(COLS_1) == 0) {
        return KEY_LOCK;
    }
    #ifdef DEBUG
    #else
    if (gpio_digital_read(COLS_2) == 0) {
        return KEY_PLUS;
    }
    #endif
    if (gpio_digital_read(COLS_3) == 0) {
        return KEY_4;
    }
    if (gpio_digital_read(COLS_4) == 0) {
        return KEY_3;
    }
    if (gpio_digital_read(COLS_5) == 0) {
        return KEY_0;
    }
    gpio_digital_write(ROWS_1, 1);

    return KEY_NONE;
}

key_rec_enum keyboard_scan_line2(void)
{
    // 扫描第二行
    gpio_digital_write(ROWS_2, 0);
    if (gpio_digital_read(COLS_1) == 0) {
        return KEY_DIVIDE;
    }
    #ifdef DEBUG
    #else
    if (gpio_digital_read(COLS_2) == 0) {
        return KEY_9;
    }
    #endif
    if (gpio_digital_read(COLS_3) == 0) {
        return KEY_5;
    }
    if (gpio_digital_read(COLS_4) == 0) {
        return KEY_2;
    }
    if (gpio_digital_read(COLS_5) == 0) {
        return KEY_DOT;
    }
    gpio_digital_write(ROWS_2, 1);

    return KEY_NONE;
}

key_rec_enum keyboard_scan_line3(void)
{
    // 扫描第三行
    gpio_digital_write(ROWS_3, 0);
    if (gpio_digital_read(COLS_1) == 0) {
        return KEY_MULTIPLY;
    }
    #ifdef DEBUG
    #else
    if (gpio_digital_read(COLS_2) == 0) {
        return KEY_8;
    }
    #endif
    if (gpio_digital_read(COLS_3) == 0) {
        return KEY_6;
    }
    if (gpio_digital_read(COLS_4) == 0) {
        return KEY_1;
    }
    if (gpio_digital_read(COLS_5) == 0) {
        return KEY_ENTER;
    }
    gpio_digital_write(ROWS_3, 1);

    return KEY_NONE;
}

key_rec_enum keyboard_scan_line4(void)
{
    // 扫描第四行
    gpio_digital_write(ROWS_4, 0);
    if (gpio_digital_read(COLS_1) == 0) {
        return KEY_MINUS;
    }
    #ifdef DEBUG
    #else
    if (gpio_digital_read(COLS_2) == 0) {
        return KEY_7;
    }
    #endif
    gpio_digital_write(ROWS_4, 1);

    return KEY_NONE;
}

key_rec_enum keyboard_scan_all(void)
{
    // 扫描键盘
    {
        // 扫描第一行
        gpio_digital_write(ROWS_1, 0);
        if (gpio_digital_read(COLS_1) == 0) {
            return KEY_LOCK;
        }
        #ifdef DEBUG
        #else
        if (gpio_digital_read(COLS_2) == 0) {
            return KEY_PLUS;
        }
        #endif
        if (gpio_digital_read(COLS_3) == 0) {
            return KEY_4;
        }
        if (gpio_digital_read(COLS_4) == 0) {
            return KEY_3;
        }
        if (gpio_digital_read(COLS_5) == 0) {
            return KEY_0;
        }
        gpio_digital_write(ROWS_1, 1);
    }
    {
        // 扫描第二行
        gpio_digital_write(ROWS_2, 0);
        if (gpio_digital_read(COLS_1) == 0) {
            return KEY_DIVIDE;
        }
        #ifdef DEBUG
        #else
        if (gpio_digital_read(COLS_2) == 0) {
            return KEY_9;
        }
        #endif
        if (gpio_digital_read(COLS_3) == 0) {
            return KEY_5;
        }
        if (gpio_digital_read(COLS_4) == 0) {
            return KEY_2;
        }
        if (gpio_digital_read(COLS_5) == 0) {
            return KEY_DOT;
        }
        gpio_digital_write(ROWS_2, 1);
    }
    {
        // 扫描第三行
        gpio_digital_write(ROWS_3, 0);
        if (gpio_digital_read(COLS_1) == 0) {
            return KEY_MULTIPLY;
        }
        #ifdef DEBUG
        #else
        if (gpio_digital_read(COLS_2) == 0) {
            return KEY_8;
        }
        #endif
        if (gpio_digital_read(COLS_3) == 0) {
            return KEY_6;
        }
        if (gpio_digital_read(COLS_4) == 0) {
            return KEY_1;
        }
        if (gpio_digital_read(COLS_5) == 0) {
            return KEY_ENTER;
        }
        gpio_digital_write(ROWS_3, 1);
    }
    {
        // 扫描第四行
        gpio_digital_write(ROWS_4, 0);
        if (gpio_digital_read(COLS_1) == 0) {
            return KEY_MINUS;
        }
        #ifdef DEBUG
        #else
        if (gpio_digital_read(COLS_2) == 0) {
            return KEY_7;
        }
        #endif
        gpio_digital_write(ROWS_4, 1);
    }
    return KEY_NONE;
}

