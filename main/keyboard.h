#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "main.h"

typedef enum {
    KEY1_2 = 0,
    KEY1_3,
    KEY1_4,
    KEY1_5,
    KEY2_1,
    KEY2_3,
    KEY2_4,
    KEY2_5,
    KEY3_1,
    KEY3_2,
    KEY3_4,
    KEY4_1,
    KEY4_2,
    KEY4_3,
    KEY5_1,
    KEY5_2,
    KEY5_3,
    KEY_NUM,
} KEY_t;

void keyboard_init(void);
void keyboard_scan(void);
// void keyboard_get_state(uint8_t *state);
void keyboard_test_init(void);
// uint8_t keyboard_test_scan(void);

#endif