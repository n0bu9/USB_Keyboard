#include "ch55x_conf.h"
#include "basic.h"
#include "main.h"
#include "Debug.h"
#if DEBUG
#include "usb_hid.h"
#endif


/*******************************************************************************
* Function Name  : QueryUART1Interrupt(void)
* Description    : UART0中断服务程序
*******************************************************************************/
#if DEBUG
void query_uart0_interrupt(void) interrupt INT_NO_UART0 using 2             //UART0中断服务程序,使用寄存器组2
{
    uint8_t rec;
    if(RI)
    {
        rec = SBUF;
        RI = 0;
        get_keyboard_data(rec);
    }
    TI = 0;
}
#endif
