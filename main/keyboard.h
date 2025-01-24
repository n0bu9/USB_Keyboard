#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "main.h"

#if (MATRIX_COLS <= 8)
typedef uint8_t matrix_row_t;
#elif (MATRIX_COLS <= 16)
typedef uint16_t matrix_row_t;
#elif (MATRIX_COLS <= 32)
typedef uint32_t matrix_row_t;
#else
#    error "MATRIX_COLS: invalid value"
#endif


#define ROWS_1 PORT_3_PIN_0
#define ROWS_2 PORT_3_PIN_1
#define ROWS_3 PORT_1_PIN_4
#define ROWS_4 PORT_3_PIN_2
#define COLS_1 PORT_1_PIN_2
#define COLS_2 PORT_1_PIN_3  // 复用于UART0 
#define COLS_3 PORT_3_PIN_5
#define COLS_4 PORT_3_PIN_3
#define COLS_5 PORT_1_PIN_0

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
key_rec_enum keyboard_scan_line1(void);
key_rec_enum keyboard_scan_line2(void);
key_rec_enum keyboard_scan_line3(void);
key_rec_enum keyboard_scan_line4(void);
key_rec_enum keyboard_scan_all(void);

#endif
