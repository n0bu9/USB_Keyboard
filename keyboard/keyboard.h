#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "main.h"
#include "matrix.h"
#define ROWS_1 PORT_3_PIN_0
#define ROWS_2 PORT_3_PIN_1
#define ROWS_3 PORT_1_PIN_4
#define ROWS_4 PORT_3_PIN_2
#define COLS_1 PORT_1_PIN_2
#define COLS_2 PORT_1_PIN_3  // 复用于UART0 
#define COLS_3 PORT_3_PIN_5
#define COLS_4 PORT_3_PIN_3
#define COLS_5 PORT_1_PIN_0


void keyboard_init(void);
void keyboard_scan_line(uint8_t line);
void keyboard_scan_all(void);
matrix_row_t *get_raw_key_state_ptr(void);
matrix_row_t *get_cooked_key_state_ptr(void);
bool if_state_changed(void);

// void move_state_to_last_line(uint8_t line);
// void move_state_to_last_all(void);
// bool compare_state_line(uint8_t line);
// bool compare_state_all(void);

#endif
