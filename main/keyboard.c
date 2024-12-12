#include "keyboard.h"
#include "gpio.h"                                               
#include "Debug.H"

#pragma  NOAREGS

sbit KEY1 = P1^4;
sbit KEY2 = P1^5;
sbit KEY3 = P1^6;

void keyboard_init() {
    // Initialize the keyboard
    Port1Cfg(3,4);
    Port1Cfg(3,5);
    Port1Cfg(3,6); 
    KEY1 = 0;
    KEY2 = KEY3 = 1;
}
