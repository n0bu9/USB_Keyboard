#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "ch55x_conf.h"
#include "uart.h"
#include "keyboard.h"

// #pragma  NOAREGS

/* 软件矩阵 */
volatile matrix_row_t raw_key_state[MATRIX_ROWS];
volatile matrix_row_t last_raw_key_state[MATRIX_ROWS];
volatile matrix_row_t cooked_key_state[MATRIX_ROWS];

/*
 * @brief 初始化键盘硬件
 * @return 无
 */
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
    gpio_digital_write(COLS_1, 1);
    gpio_digital_write(COLS_2, 1);
    gpio_digital_write(COLS_3, 1);
    gpio_digital_write(COLS_4, 1);
    gpio_digital_write(COLS_5, 1);
    memset(raw_key_state, 0, sizeof(raw_key_state));
    memset(cooked_key_state, 0, sizeof(cooked_key_state));
}

/*                                                映射
                                        硬件矩阵-------->软件矩阵

         COLS1  COLS2  COLS3  COLS4  COLS5                     COLS1  COLS2  COLS3  COLS4 
  ROWS1  LOCK     +      4      3      0      ------->  ROWS1  LOCK     /      *      -
  ROWS2    /      9      5      2      .      ------->  ROWS2    7      8      9      +
  ROWS3    *      8      6      1    ENTER    ------->  ROWS3    4      5      6      
  ROWS4    -      7                           ------->  ROWS4    1      2      3
                                              ------->  ROWS5    0             .    ENTER
*/

/*
 * @brief 扫描第一行
 * @return 无
 */
void __keyboard_scan_line0(void)
{
    // 扫描第一行
    gpio_digital_write(ROWS_1, 0);
    if (gpio_digital_read(COLS_1) == 0) {  // DOWN
        raw_key_state[0] |= (1 << 0);
    }else {                                // UP  
        raw_key_state[0] &= ~(1 << 0);
    }
    #ifdef DEBUG
    #else
    if (gpio_digital_read(COLS_2) == 0) {
        raw_key_state[1] |= (1 << 3);
    }else {
        raw_key_state[1] &= ~(1 << 3);
    }
    #endif
    if (gpio_digital_read(COLS_3) == 0) {
        raw_key_state[2] |= (1 << 0);
    }else {
        raw_key_state[2] &= ~(1 << 0);
    }
    if (gpio_digital_read(COLS_4) == 0) {
        raw_key_state[3] |= (1 << 2);
    }else {
        raw_key_state[3] &= ~(1 << 2);
    }
    if (gpio_digital_read(COLS_5) == 0) {
        raw_key_state[4] |= (1 << 0);
    }else {
        raw_key_state[4] &= ~(1 << 0);
    }
    gpio_digital_write(ROWS_1, 1);
}

void __keyboard_scan_line1(void)
{
    // 扫描第二行
    gpio_digital_write(ROWS_2, 0);
    if (gpio_digital_read(COLS_1) == 0) {
        raw_key_state[0] |= (1 << 1);
    }else {
        raw_key_state[0] &= ~(1 << 1);
    }
    #ifdef DEBUG
    #else
    if (gpio_digital_read(COLS_2) == 0) {
        raw_key_state[1] |= (1 << 2);
    }else {
        raw_key_state[1] &= ~(1 << 2);
    }
    #endif
    if (gpio_digital_read(COLS_3) == 0) {
        raw_key_state[2] |= (1 << 1);
    }else {
        raw_key_state[2] &= ~(1 << 1);
    }
    if (gpio_digital_read(COLS_4) == 0) {
        raw_key_state[3] |= (1 << 1);
    }else {
        raw_key_state[3] &= ~(1 << 1);
    }
    if (gpio_digital_read(COLS_5) == 0) {
        raw_key_state[4] |= (1 << 2);
    }else {
        raw_key_state[4] &= ~(1 << 2);
    }
    gpio_digital_write(ROWS_2, 1);
}

void __keyboard_scan_line2(void)
{
    // 扫描第三行
    gpio_digital_write(ROWS_3, 0);
    if (gpio_digital_read(COLS_1) == 0) {
        raw_key_state[0] |= (1 << 2);
    }else {
        raw_key_state[0] &= ~(1 << 2);
    }
    #ifdef DEBUG
    #else
    if (gpio_digital_read(COLS_2) == 0) {
        raw_key_state[1] |= (1 << 1);
    }else {
        raw_key_state[1] &= ~(1 << 1);
    }
    #endif
    if (gpio_digital_read(COLS_3) == 0) {
        raw_key_state[2] |= (1 << 2);
    }else {
        raw_key_state[2] &= ~(1 << 2);
    }
    if (gpio_digital_read(COLS_4) == 0) {
        raw_key_state[3] |= (1 << 0);
    }else {
        raw_key_state[3] &= ~(1 << 0);
    }
    if (gpio_digital_read(COLS_5) == 0) {
        raw_key_state[4] |= (1 << 3);
    }else {
        raw_key_state[4] &= ~(1 << 3);
    }
    gpio_digital_write(ROWS_3, 1);
}

void __keyboard_scan_line3(void)
{
    // 扫描第四行
    gpio_digital_write(ROWS_4, 0);
    if (gpio_digital_read(COLS_1) == 0) {
        raw_key_state[0] |= (1 << 3);
    }else {
        raw_key_state[0] &= ~(1 << 3);
    }
    #ifdef DEBUG
    #else
    if (gpio_digital_read(COLS_2) == 0) {
        raw_key_state[1] |= (1 << 0);
    }else {
        raw_key_state[1] &= ~(1 << 0);
    }
    #endif
    gpio_digital_write(ROWS_4, 1);
}

/*
 * @brief 扫描键盘的某一行
 * @param line: 行号
 * @return 无
 */
void keyboard_scan_line(uint8_t line)
{
    memcpy(last_raw_key_state, raw_key_state, sizeof(matrix_row_t)*MATRIX_ROWS);
    switch (line)
    {
        case 0:
            __keyboard_scan_line0();
            break;
        case 1:
            __keyboard_scan_line1();
            break;
        case 2:
            __keyboard_scan_line2();
            break;
        case 3:
            __keyboard_scan_line3();
            break;
        default:
            break;
    }
}

/*
 * @brief 扫描键盘的全部行
 * @return 无
 */
void keyboard_scan_all(void)
{
    memcpy(last_raw_key_state, raw_key_state, sizeof(matrix_row_t)*MATRIX_ROWS);
    __keyboard_scan_line0();
    __keyboard_scan_line1();
    __keyboard_scan_line2();
    __keyboard_scan_line3();
}

bool if_state_changed(void)
{
    return memcmp(last_raw_key_state, raw_key_state, sizeof(matrix_row_t)*MATRIX_ROWS);
}

/*
 * @brief 获取当前状态的数组地址
 * @return 无
 */
matrix_row_t *get_raw_key_state_ptr(void)
{
    return raw_key_state;
}

/*
 * @brief 获取上一个状态的数组地址
 * @return 无
 */
matrix_row_t *get_cooked_key_state_ptr(void)
{
    return cooked_key_state;
}

// /*
//  * @brief 将当前第X状态移动到上一个状态
//  * @param line: 行号
//  * @return 无
//  */
// void move_state_to_last_line(uint8_t line)
// {
//     if (line < MATRIX_ROWS) {
//         memcpy(last_key_state[line], real_key_state[line], sizeof(matrix_row_t));
//     }
// }

// /*
//  * @brief 将当前状态移动到上一个状态
//  * @return 无
//  */
// void move_state_to_last_all(void)
// {
//     memcpy(last_key_state, real_key_state, sizeof(matrix_row_t)*MATRIX_ROWS);
// }

// /*
//  * @brief 比较当前第X行状态和上一个状态
//  * @param line: 行号
//  * @return 无
//  */
// bool compare_state_line(uint8_t line)
// {
//     if (line < MATRIX_ROWS) {
//         return memcmp(last_key_state[line], real_key_state[line], sizeof(matrix_row_t));
//     }
//     return TRUE;
// }

// /*
//  * @brief 比较当前状态和上一个状态
//  * @return 无
//  */
// bool compare_state_all(void)
// {
//     return memcmp(last_key_state, real_key_state, sizeof(matrix_row_t)*MATRIX_ROWS);
// }

