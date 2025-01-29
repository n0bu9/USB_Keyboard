#include "keymap.h"
#include "keyboard.h"
#include "usb_hid.h"

#define _BL_ 0
#define _FL_ 1
#define _________ KC_NONE

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {  
            {
                {KC_NUM_LOCK_CLEAR, KC_P_SLASH,      KC_P_ASTERISK,      KC_P_MINUS}, // LSB ---> MSB
                {KC_P_7_HOME,       KC_P_8_UP_ARROW, KC_P_9_PAGE_UP,     KC_P_PLUS },
                {KC_P_4_LEFT_ARROW, KC_P_5_BEGIN,    KC_P_6_RIGHT_ARROW, _________ },
                {KC_P_1_END,        KC_P_2_DOWN,     KC_P_3_PAGE_DOWN,   _________ },
                {KC_P_0_INSERT,     _________,       KC_P_DOT_DELETE,    KC_P_ENTER}
            }
};

void report_key_by_map(uint16_t which_row, matrix_row_t cols_mask) 
{
    matrix_row_t j = 0;
    for (j = 0; j < MATRIX_COLS; j++) {
        if (cols_mask & (1 << j)) {
            get_keyboard_data('A');
        }
    }
}
