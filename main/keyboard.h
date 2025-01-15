#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "main.h"

typedef enum {
    ROWS_1 = 0, // P1.2
    ROWS_2,     // P1.3
    ROWS_3,     // P3.5
    ROWS_4,     // P3.3
    ROWS_5,     // P1.0
    COLS_1,     // P3.0
    COLS_2,     // P3.1
    COLS_3,     // P1.4
    COLS_4,     // P3.2
    ARRAY_NUM
} array_scan_enum;

typedef enum {
    KEY_LOCK = 0,
    KEY_DIVIDE,
    KEY_MULTIPLY,
    KEY_MINUS,
    KEY_PLUS,
    KEY_9,
    KEY_8,
    KEY_7,
    KEY_6,
    KEY_5,
    KEY_4,
    KEY_3,
    KEY_2,
    KEY_1,
    KEY_0,
    KEY_DOT,
    KEY_ENTER,
    DRIVER_KEY_NUM,
    KEY_NONE = 0xFF
} key_rec_enum;

void keyboard_init(void);
key_rec_enum keyboard_scan(void);

#endif
