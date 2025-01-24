#include "keymap.h"
#include "keyboard.h"

#define _BL_ 0
#define _FL_ 1
#define _______ KEY_NONE

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BL_] = {
                {KC_LOCK, KC_DIVIDE, KC_MULTIPLY, KC_MINUS},
                {KC_7,    KC_8,      KC_9,        KC_PLUS },
                {KC_4,    KC_5,      KC_6,                },
                {KC_1,    KC_2,      KC_3,                },
                {KC_0,               KC_DOT,      KC_ENTER}
            }
};