#include "keyboard.h"
#include "gpio.h"                                               
#include "Debug.H"

#pragma  NOAREGS

sbit KEY_IO1 = P1^2;
sbit KEY_IO2 = P1^3;
sbit KEY_IO3 = P3^5;
sbit KEY_IO4 = P3^3;
sbit KEY_IO5 = P1^0;

static volatile UINT8 key_state[KEY_NUM];

void keyboard_init() {
    // Initialize the keyboard
    // 开漏输入输出，有上拉
    Port1Cfg(3,2);  // KEY_IO1
    Port1Cfg(3,3);  // KEY_IO2
    Port3Cfg(3,5);  // KEY_IO3
    Port3Cfg(3,3);  // KEY_IO4
    Port1Cfg(3,0);  // KEY_IO5
}

void keyboard_scan() {
    // 扫描键盘
    KEY_IO1 = 1;KEY_IO2 = 1;KEY_IO3 = 1;KEY_IO4 = 1;KEY_IO5 = 1;
    {
        // 扫描第一行
        KEY_IO1 = 0;
        if (KEY_IO2 == 0) {
            key_state[KEY1_2] = 1;
        } else {
            key_state[KEY1_2] = 0;
        }
        if (KEY_IO3 == 0) {
            key_state[KEY1_3] = 1;
        } else {
            key_state[KEY1_3] = 0;
        }
        if (KEY_IO4 == 0) {
            key_state[KEY1_4] = 1;
        } else {
            key_state[KEY1_4] = 0;
        }
        if (KEY_IO5 == 0) {
            key_state[KEY1_5] = 1;
        } else {
            key_state[KEY1_5] = 0;
        }
        KEY_IO1 = 1;
    } 
    {
        // 扫描第二行
        KEY_IO2 = 0;
        if (KEY_IO1 == 0) {
            key_state[KEY2_1] = 1;
        } else {
            key_state[KEY2_1] = 0;
        }
        if (KEY_IO3 == 0) {
            key_state[KEY2_3] = 1;
        } else {
            key_state[KEY2_3] = 0;
        }
        if (KEY_IO4 == 0) {
            key_state[KEY2_4] = 1;
        } else {
            key_state[KEY2_4] = 0;
        }
        if (KEY_IO5 == 0) {
            key_state[KEY2_5] = 1;
        } else {
            key_state[KEY2_5] = 0;
        }
        KEY_IO2 = 1;
    } 
    {
        // 扫描第三行
        KEY_IO3 = 0;
        if (KEY_IO1 == 0) {
            key_state[KEY3_1] = 1;
        } else {
            key_state[KEY3_1] = 0;
        }   
        if (KEY_IO2 == 0) {
            key_state[KEY3_2] = 1;
        } else {
            key_state[KEY3_2] = 0;
        }   
        if (KEY_IO4 == 0) {
            key_state[KEY3_4] = 1;
        } else {
            key_state[KEY3_4] = 0;
        }
        KEY_IO3 = 1;    
    }
    {
        // 扫描第四行
        KEY_IO4 = 0;
        if (KEY_IO1 == 0) {
            key_state[KEY4_1] = 1;
        } else {
            key_state[KEY4_1] = 0;
        }
        if (KEY_IO2 == 0) {
            key_state[KEY4_2] = 1;
        } else {
            key_state[KEY4_2] = 0;
        }
        if (KEY_IO3 == 0) {
            key_state[KEY4_3] = 1;
        } else {
            key_state[KEY4_3] = 0;
        }
        KEY_IO4 = 1;
    }   
    {
        // 扫描第五行
        KEY_IO5 = 0;
        if (KEY_IO1 == 0) {
            key_state[KEY5_1] = 1;
        } else {
            key_state[KEY5_1] = 0;
        }
        if (KEY_IO2 == 0) {
            key_state[KEY5_2] = 1;
        } else {
            key_state[KEY5_2] = 0;
        }
        if (KEY_IO3 == 0) {
            key_state[KEY5_3] = 1;
        } else {
            key_state[KEY5_3] = 0;
        }
        KEY_IO5 = 1;
    }   
}
