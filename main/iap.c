#include "iap.h"
#include "Debug.H"

typedef void( *pTaskFn)( void );
pTaskFn tasksArr[1];

void iap_run(void)
{
    EA = 0;                                                                    //关闭总中断，必加
	tasksArr[0] = BOOT_ADDR;
    mDelaymS(100);
    P3_6 = 1;
    (tasksArr[0])( );                                                          //跳至BOOT升级程序,使用ISP工具升级
    while(1);
}
