#include "debounce.h"
#include "keyboard.h"
#include "timer.h"
#include "string.h"

#ifndef DEBOUNCE
#    define DEBOUNCE 5
#endif

void sym_defer_get_debounce_init(void){}

// Maximum debounce: 255ms
#if DEBOUNCE > UINT8_MAX
#    undef DEBOUNCE
#    define DEBOUNCE UINT8_MAX
#endif

static volatile bool     debouncing = FALSE;
static volatile uint16_t debouncing_time;

/*
Basic global debounce algorithm. Used in 99% of keyboards at time of implementation
When no state changes have occured for DEBOUNCE milliseconds, we push the state.
*/

bool sym_defer_get_debounce(matrix_row_t raw_key_state[], matrix_row_t cooked_key_state[], uint8_t num_rows, bool changed) 
{
    bool cooked_changed = FALSE;
    uint16_t matrix_size = 0;

    if (changed) {
        debouncing      = TRUE;
        debouncing_time = timer_read();
    } else if (debouncing && timer_elapsed(debouncing_time) >= DEBOUNCE) {
        matrix_size = num_rows * sizeof(matrix_row_t);
        if (memcmp(cooked_key_state, raw_key_state, matrix_size) != 0) {
            memcpy(cooked_key_state, raw_key_state, matrix_size);
            cooked_changed = TRUE;
        }
        debouncing = FALSE;
    }

    return cooked_changed;
}
