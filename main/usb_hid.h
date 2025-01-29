#ifndef __USB_HID_H__
#define __USB_HID_H__

#include "main.h"
#include "keymap.h"

void usb_device_init(void);
void hid_value_handle_proc(void);
void get_keyboard_data(uint8_t in_data);
void usb_clear_flag(void);
void keycode_input(key_code_enum keycode);

#endif  // __USB_HID_H__
