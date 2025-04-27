#ifndef __USB_HID_H__
#define __USB_HID_H__

#include "main.h"
#include "keymap.h"

#define ENP1_IN_EVT(usbdata, len)  do{\
                                        edp1_idle = 0;\
                                        enp1_in_evt(usbdata, len);\
                                        while(edp1_idle == 0);\
                                        edp1_idle = 0;\
                                    } while (0);

#define ENP_2_OUT_EVT()

#define HIGH_BYTE(hid_report)   ((sizeof(hid_report)) >> 8  )
#define LOW_BYTE(hid_report)    ((sizeof(hid_report)) & 0xFF)


void usb_device_init(void);
void usb_clear_flag(void);
void keycode_fill_report(key_code_enum keycode);
void keycode_input_proc(void);

void keyboard_info_get(uint8_t *buff);

#endif  // __USB_HID_H__
