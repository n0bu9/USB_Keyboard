#ifndef __USB_HID_H__
#define __USB_HID_H__

#include "main.h"
#include "keymap.h"

#define ENP1_IN_EVT(usbdata, len)  do{\
                                        edp0_idle = 0;\
                                        enp1_in_evt(usbdata, len);\
                                        while(edp0_idle == 0);\
                                        edp0_idle = 0;\
                                    } while (0);

#define HIGH_BYTE(hid_report)   ((sizeof(hid_report)) >> 8  )
#define LOW_BYTE(hid_report)    ((sizeof(hid_report)) & 0xFF)


void usb_device_init(void);
void usb_clear_flag(void);
void keycode_fill_report(key_code_enum keycode);
void keycode_input_proc(void);

#endif  // __USB_HID_H__
