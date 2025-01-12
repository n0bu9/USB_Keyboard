#include "uart.h"
#include "main.h"
#include "usb_hid.h"
#include "ch55x_uart.h"

/*******************************************************************************
* Function Name  : uart_send_string(uint8_t uart_x, const char *str)
* Description    : 发送字符串
* Input          : uart_x: 0: uart0, 1: uart1
*                  str: 字符串
* Output         : None
* Return         : None
*******************************************************************************/
void uart_send_string(uint8_t uart_x, const int8_t *str) 
{
    if(uart_x == 0)
    {
        #if USE_UART0_ENABLE
        while (*str) {
            uart0_send_byte(*str++);
        }
        #endif
    }
    else if(uart_x == 1)
    {
        #if USE_UART1_ENABLE
        while (*str) {
            uart1_send_byte(*str++);
        }
        #endif
    }
}

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
