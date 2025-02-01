#include "keymap.h"
#include "keyboard.h"
#include "usb_hid.h"

#define _BL_ 0
#define _FL_ 1
#define _________ KC_NONE

/* 用户定义的键盘映射表 */
const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {  
            {
                {KC_NUM_LOCK_CLEAR, KC_P_SLASH,      KC_P_ASTERISK,      KC_P_MINUS}, // LSB ---> MSB
                {KC_P_7_HOME,       KC_P_8_UP_ARROW, KC_P_9_PAGE_UP,     KC_P_PLUS },
                {KC_P_4_LEFT_ARROW, KC_P_5_BEGIN,    KC_P_6_RIGHT_ARROW, _________ },
                {KC_P_1_END,        KC_P_2_DOWN,     KC_P_3_PAGE_DOWN,   _________ },
                {KC_P_0_INSERT,     _________,       KC_P_DOT_DELETE,    KC_P_ENTER}
            }
};

/*  ******************************************************************************
* Function Name  : report_key_by_map()
* Description    : 通过映射表上报键盘码
* Input          : cooked_key_state:逻辑键盘状态
* Output         : None
* Return         : None
*******************************************************************************/
void report_key_by_map(matrix_row_t *cooked_key_state) 
{
    uint8_t i = 0;
    uint8_t j = 0;
    for (i = 0; i < MATRIX_ROWS; i++) {
        if (cooked_key_state[i] != 0) {
            for (j = 0; j < MATRIX_COLS; j++) {
                if (cooked_key_state[i] & (1 << j)) {
                    keycode_fill_report(keymaps[0][i][j]);
                }
            }
        }
    }
}


