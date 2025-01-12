#ifndef __USB_HID_H__
#define __USB_HID_H__

#include "main.h"

void usb_device_init(void);
void hid_value_handle(void);
void get_keyboard_data(uint8_t in_data);
void usb_clear_flag(void);

#endif  // __USB_HID_H__
