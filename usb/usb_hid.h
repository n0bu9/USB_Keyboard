#ifndef __USB_HID_H__
#define __USB_HID_H__

#include "main.h"
#include "keymap.h"

#define ENP1_IN_EVT(usbdata, len)  do{\
                                        FLAG = 0;\
                                        enp1_in_evt(usbdata, len);\
                                        while(FLAG == 0);\
                                        FLAG = 0;\
                                    } while (0);


void usb_device_init(void);
void usb_clear_flag(void);
void keycode_fill_report(key_code_enum keycode);
void keycode_input_proc(void);

#endif  // __USB_HID_H__
