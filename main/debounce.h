#ifndef __DEBOUNCE_H__
#define __DEBOUNCE_H__

#include "stdint.h"
#include "matrix.h"

bool sym_defer_get_debounce(matrix_row_t raw_key_state[], matrix_row_t cooked_key_state[], uint8_t num_rows, bool changed);

#endif
