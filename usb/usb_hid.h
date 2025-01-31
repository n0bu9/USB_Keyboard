#ifndef __USB_HID_H__
#define __USB_HID_H__

#include "main.h"
#include "keymap.h"

void usb_device_init(void);
void usb_clear_flag(void);
void keycode_input(key_code_enum keycode[], uint8_t keycode_len);
void keycode_input_none(void);

#endif  // __USB_HID_H__
