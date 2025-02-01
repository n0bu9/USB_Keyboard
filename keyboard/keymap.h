#ifndef KEYMAP_H
#define KEYMAP_H

#include "stdint.h"
#include "main.h"
#include "matrix.h"

typedef enum {
    KC_NONE = 0,  // reserved
    KC_ERR_ROLLOVER, // error rollover
    KC_POST_FAIL, // post fail
    KC_ERR_UNDEFINED, // error undefined
    KC_a_A, // a A
    KC_b_B, // b B
    KC_c_C, // c C
    KC_d_D, // d D
    KC_e_E, // e E
    KC_f_F, // f F
    KC_g_G, // g G
    KC_h_H, // h H
    KC_i_I, // i I
    KC_j_J, // j J
    KC_k_K, // k K
    KC_l_L, // l L
    KC_m_M, // m M
    KC_n_N, // n N
    KC_o_O, // o O
    KC_p_P, // p P
    KC_q_Q, // q Q
    KC_r_R, // r R
    KC_s_S, // s S
    KC_t_T, // t T
    KC_u_U, // u U
    KC_v_V, // v V
    KC_w_W, // w W
    KC_x_X, // x X
    KC_y_Y, // y Y
    KC_z_Z,
    KC_1_EXCALMATORY_MARK, // 1 !
    KC_2_AT, // 2 @
    KC_3_HASH, // 3 #
    KC_4_DOLLAR, // 4 $
    KC_5_PERCENT, // 5 %
    KC_6_CARET, // 6 ^
    KC_7_AMPERSAND, // 7 &
    KC_8_ASTERISK, // 8 *
    KC_9_LEFT_PARENTHESIS, // 9 (
    KC_0_RIGHT_PARENTHESIS,// 0 )
    KC_ENTER, // enter
    KC_ESC, // esc
    KC_BACKSPACE, // backspace
    KC_TAB, // tab
    KC_SPACEBAR, // space
    KC_MINUS_UNDERSCORE,  // - _
    KC_EQUAL_PLUS, // = +
    KC_LEFT_SQUARE_BRACKET_RIGHT_SQUARE_BRACKET,  // [ {
    KC_RIGHT_SQUARE_BRACKET_LEFT_SQUARE_BRACKET,  // ] }
    KC_BACKSLASH_PIPE,  // \ |
    KC_NON_US_HASH_TILDE,  // # ~
    KC_SEMICOLON_COLON,  // ; :
    KC_QUOTE_DOUBLE_QUOTE,  // ' "
    KC_GRAVE_ACCENT_AND_TILDE,
    KC_COMMA_LESS_THAN,  // , <
    KC_DOT_GREATER_THAN,  // . >
    KC_SLASH_QUESTION_MARK, // / ?
    KC_CAPS_LOCK, // caps lock
    KC_F1, // f1
    KC_F2, // f2
    KC_F3, // f3
    KC_F4, // f4
    KC_F5, // f5
    KC_F6, // f6
    KC_F7, // f7
    KC_F8, // f8
    KC_F9, // f9
    KC_F10, // f10
    KC_F11, // f11
    KC_F12, // f12
    KC_PRINT_SCREEN, // print screen
    KC_SCROLL_LOCK, // scroll lock
    KC_PAUSE, // pause
    KC_INSERT, // insert
    KC_HOME, // home
    KC_PAGE_UP, // page up
    KC_DELETE, // delete
    KC_END, // end
    KC_PAGE_DOWN, // page down
    KC_RIGHT_ARROW, // right
    KC_LEFT_ARROW, // left
    KC_DOWN_ARROW, // down
    KC_UP_ARROW, // up
    KC_NUM_LOCK_CLEAR, // num lock
    KC_P_SLASH, // pad /
    KC_P_ASTERISK, // pad *
    KC_P_MINUS, // pad -
    KC_P_PLUS, // pad +
    KC_P_ENTER, // pad enter
    KC_P_1_END, // pad 1
    KC_P_2_DOWN, // pad 2
    KC_P_3_PAGE_DOWN, // pad 3
    KC_P_4_LEFT_ARROW, // pad 4
    KC_P_5_BEGIN, // pad 5
    KC_P_6_RIGHT_ARROW, // pad 6
    KC_P_7_HOME, // pad 7
    KC_P_8_UP_ARROW, // pad 8
    KC_P_9_PAGE_UP, // pad 9
    KC_P_0_INSERT, // pad 0
    KC_P_DOT_DELETE, // pad .
    KC_NON_US_BACKSLASH_BAR_PIPE,
    KC_APPLICATION, // application
    KC_POWER, // power
    KC_P_EQUAL, // pad =
    // KC_F13, // f13
    // KC_F14, // f14
    // KC_F15, // f15
    // KC_F16, // f16
    // KC_F17, // f17
    // KC_F18, // f18
    // KC_F19, // f19
    // KC_F20, // f20
    // KC_F21, // f21
    // KC_F22, // f22
    // KC_F23, // f23
    // KC_F24, // f24
    // KC_EXECUTE, // execute
    // KC_HELP, // help
    // KC_MENU, // menu
    // KC_SELECT, // select
    // KC_STOP, // stop
    // KC_AGAIN, // again
    // KC_UNDO, // undo
    // KC_CUT, // cut
    // KC_COPY, // copy
    // KC_PASTE, // paste
    // KC_FIND, // find
    // KC_MUTE, // mute
    // KC_VOLUME_UP, // volume up
    // KC_VOLUME_DOWN, // volume down
    // KC_LOCK_CAPS_LOCK, // caps lock 
    // KC_LOCK_NUM_LOCK, // num lock
    // KC_LOCK_SCROLL_LOCK, // scroll lock
    // KC_P_COMMA, // pad ,
    // KC_P_EQUAL_SIGN, // pad =
    // KC_INTERNATIONAL_1,
    // KC_INTERNATIONAL_2,
    // KC_INTERNATIONAL_3,
    // KC_INTERNATIONAL_4,
    // KC_INTERNATIONAL_5,
    // KC_INTERNATIONAL_6,
    // KC_INTERNATIONAL_7,
    // KC_INTERNATIONAL_8,
    // KC_INTERNATIONAL_9,
    // KC_LONG1,
    // KC_LONG2,

    KC_LEFT_CTRL = 0xE0,  // left ctrl
    KC_LEFT_SHIFT = 0xE1, // left shift
    KC_LEFT_ALT = 0xE2, // left alt
    KC_LEFT_GUI = 0xE3, // left gui
    KC_RIGHT_CTRL = 0xE4, // right ctrl
    KC_RIGHT_SHIFT = 0xE5, // right shift
    KC_RIGHT_ALT = 0xE6, // right alt
    KC_RIGHT_GUI = 0xE7  // right gui
}key_code_enum;

void report_key_by_map(matrix_row_t *cooked_key_state);

#endif