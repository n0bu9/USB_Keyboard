#include "CH554.H"

void Timer0_Init(void)
{
    TMOD |= 0x01;
    TH0 = 0xFC;
    TL0 = 0x18;
    ET0 = 1;
    EA = 1;
}
