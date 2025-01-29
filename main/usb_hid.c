#include "usb_hid.h"
#include "main.h"
#include "string.h"

#define THIS_ENDP0_SIZE         DEFAULT_ENDP0_SIZE
#define BUFMAX 8
#pragma  NOAREGS

static uint8_t xdata Ep0Buffer[8>(THIS_ENDP0_SIZE+2)?8:(THIS_ENDP0_SIZE+2)] _at_ 0x0000;    //端点0 OUT&IN缓冲区，必须是偶地址
static uint8_t xdata Ep1Buffer[64>(MAX_PACKET_SIZE+2)?64:(MAX_PACKET_SIZE+2)] _at_ 0x000a;  //端点1 OUT&IN缓冲区，必须是偶地址

uint8_t hid_report_descriptor[62] = {
    0x05,0x01,0x09,0x06,0xA1,0x01,0x05,0x07,
    0x19,0xe0,0x29,0xe7,0x15,0x00,0x25,0x01,
    0x75,0x01,0x95,0x08,0x81,0x02,0x95,0x01,
    0x75,0x08,0x81,0x01,0x95,0x03,0x75,0x01,
    0x05,0x08,0x19,0x01,0x29,0x03,0x91,0x02,
    0x95,0x05,0x75,0x01,0x91,0x01,0x95,0x06,
    0x75,0x08,0x26,0xff,0x00,0x05,0x07,0x19,
    0x00,0x29,0x91,0x81,0x00,0xC0
};

uint8_t device_descriptor[18] = {
    0x12, // 设备描述符的字节数大小(bLength)
    0x01, // 设备类型(bDescriptorType)
    0x00, 0x02, // USB版本(bcdUSB)
    0x00, // 设备类代码(bDeviceClass)
    0x00, // 设备子类代码(bDeviceSubClass)
    0x00, // 设备协议代码(bDeviceProtocol)
    DEFAULT_ENDP0_SIZE, // 端点0的最大包大小(bMaxPacketSize0)
    0x3d,0x41, // 厂商编号(idVendor)
    0x07,0x21, // 产品编号(idProduct)
    0x00,0x00, // 设备出厂编号(bcdDevice)
    0x00, // 制造商字符串索引(iManufacturer)
    0x00, // 产品字符串索引(iProduct)
    0x00, // 序列号字符串索引(iSerialNumber)
    0x01, // 配置数量(bNumConfigurations)
};

uint8_t config_descriptor[34] = {
    /* 配置描述符 9B */
    0x09, // 配置描述符的字节数大小(bLength)
    0x02, // 配置描述符的类型(bDescriptorType)
    0x22,0x00, // 返回整个数据的长度.指此配置返回的配置描述符,接口描述符以及端点描述符的全部大小(wTotalLength)
    0x01, // 此配置所支持的接口数量(bNumInterfaces)
    0x01, // Set_Configuration命令需要的参数值(bConfigurationValue)
    0x00, // 描述该配置的字符串的索引值(iConfiguration)
    0xA0, // 供电模式的选择(Bit4-0保留,D7:总线供电,D6:自供电,D5:远程唤醒)(bmAttributes)
    0x32, // 设备从总线提取的最大电流(以2mA为单位)(bMaxPower)
    /* 接口描述符 9B */
    0x09, // 设备描述符的字节数大小(bLength)
    0x04, // 接口描述符的类型(bDescriptorType)
    0x00, // 接口编号(bInterfaceNumber)
    0x00, // 描述符的索引值(bAlternateSetting)
    0x01, // 该接口使用端点数(bNumEndpoints)
    0x03, // 类型代码(bInterfaceClass)
    0x00, // 子类代码(bInterfaceSubClass)
    0x00, // 协议代码(bInterfaceProtocol)
    0x00, // 字符串描述符的索引(iInterface)
    /* HID描述符 9B */
    0x09, // 设备描述符的字节数大小(bLength)
    0x21, // HID描述符的类型(bDescriptorType)
    0x11,0x01, // HID规范版本号(bcdHID,为1.11)
    0x00, // 国家代码(bCountryCode)
    0x01, // 类别描述符数目(至少有一个报表描述符)(bNumDescriptors)
    0x22, // 该类别描述符的类型(bDescriptorType)
    sizeof(hid_report_descriptor),0x00, // 该类别描述符的总长度(wDescriptorLength)
    /* 端点描述符 7B */
    0x07, // 端点描述符的字节数大小(bLength)
    0x05, // 端点描述符的类型(bDescriptorType)
    0x81, // USB设备的端点地址(bEndpointAddress)
    0x03, // 端点方向(bmAttributes)
    DEFAULT_ENDP0_SIZE,0x00, // 本端点接收或发送的最大信息包大小(wMaxPacketSize)
    0x01, // 端点间隔时间(bInterval)
};

/*键盘数据*/
static uint8_t HIDKey[8] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
uint8_t data_buf[BUFMAX] = {0};
uint16_t data_len = 0;
uint16_t send_point = 0;
uint16_t recv_point = 0;
static uint8_t SetupReq = 0,SetupLen = 0,Ready = 0,Count = 0,FLAG = 0,UsbConfig = 0;
static uint8_t LED_VALID = 0;
static uint8_t *pDescr; // USB配置标志
static USB_SETUP_REQ   SetupReqBuf; // 暂存Setup包


#define UsbSetupBuf     ((PUSB_SETUP_REQ)Ep0Buffer)
/*******************************************************************************
* Function Name  : USBDeviceInit()
* Description    : USB设备模式配置,设备模式启动，收发端点配置，中断开启
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usb_device_init(void)
{
    IE_USB = 0;                                                                // 关闭USB中断
	USB_CTRL = 0x00;                                                           // 先设定USB设备模式
    UEP0_DMA = Ep0Buffer;                                                      //端点0数据传输地址
    UEP1_DMA = Ep1Buffer;                                                      //端点1数据传输地址
    UEP4_1_MOD = ~(bUEP4_RX_EN | bUEP4_TX_EN | bUEP1_RX_EN | bUEP1_BUF_MOD) | bUEP4_TX_EN;;   //端点1单64字节收发缓冲区,端点0收发
    UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;                                 //OUT事务返回ACK，IN事务返回NAK
    UEP1_CTRL = bUEP_T_TOG | UEP_T_RES_NAK;                                    //IN事务返回NAK，OUT事务返回ACK

    USB_DEV_AD = 0x00;
    UDEV_CTRL = bUD_PD_DIS;                                                    // 禁止DP/DM下拉电阻
    USB_CTRL = bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN;                      // 启动USB设备及DMA，在中断期间中断标志未清除前自动返回NAK
    UDEV_CTRL |= bUD_PORT_EN;                                                  // 允许USB端口
    USB_INT_FG = 0xFF;                                                         // 清中断标志
    USB_INT_EN = bUIE_SUSPEND | bUIE_TRANSFER | bUIE_BUS_RST;                  // 开启总线挂起、传输、总线复位中断、USB传输完成中断
    IE_USB = 1;                                                                // 开启USB中断
}

/*******************************************************************************
* Function Name  : enp1_in_evt()
* Description    : USB设备模式端点1的中断上传
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void enp1_in_evt(void)
{
    memcpy(Ep1Buffer, HIDKey, sizeof(HIDKey));                              //加载上传数据
    UEP1_T_LEN = sizeof(HIDKey);                                             //上传数据长度
    UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK;                //有数据时上传数据并应答ACK
}

/*******************************************************************************
* Function Name  : device_interrupt()
* Description    : CH554USB中断处理函数
*******************************************************************************/
void device_interrupt(void) interrupt INT_NO_USB using 1                        //USB中断服务程序,使用寄存器组1
{
    uint8_t len;
    if(UIF_TRANSFER)                                                            //USB传输完成标志
    {
        switch (USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP))
        {
        case UIS_TOKEN_IN | 2:                                                  //endpoint 2# 中断端点上传
            UEP2_T_LEN = 0;                                                     //预使用发送长度一定要清空
//            UEP1_CTRL ^= bUEP_T_TOG;                                          //如果不设置自动翻转则需要手动翻转
            UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;           //默认应答NAK
            break;
        case UIS_TOKEN_IN | 1:                                                  //endpoint 1# 中断端点上传
            UEP1_T_LEN = 0;                                                     //预使用发送长度一定要清空
//            UEP2_CTRL ^= bUEP_T_TOG;                                          //如果不设置自动翻转则需要手动翻转
            UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;           //默认应答NAK
            FLAG = 1;                                                           /*传输完成标志*/
            break;
        case UIS_TOKEN_SETUP | 0:                                                //SETUP事务
            len = USB_RX_LEN;
            if(len == (sizeof(USB_SETUP_REQ)))
            {
                SetupLen = UsbSetupBuf->wLengthL;
                if(UsbSetupBuf->wLengthH || SetupLen > 0x7F)
                {
                    SetupLen = 0x7F;    // 限制总长度
                }
                len = 0;                                                        // 默认为成功并且上传0长度
                SetupReq = UsbSetupBuf->bRequest;
                if ((UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK) != USB_REQ_TYP_STANDARD) /* HID类命令 */
                {
									switch( SetupReq )
									{
                                    case 0x01://GetReport
                                            break;
                                    case 0x02://GetIdle
                                            break;
                                    case 0x03://GetProtocol
                                            break;
                                    case 0x09://SetReport
                                            break;
                                    case 0x0A://SetIdle
                                            break;
                                    case 0x0B://SetProtocol
                                            break;
                                    default:
                                            len = 0xFF;  								 					            /*命令不支持*/
                                            break;
								    }
                }
                else
                {//标准请求
                    switch(SetupReq)                                        //请求码
                    {
                    case USB_GET_DESCRIPTOR:
                        switch(UsbSetupBuf->wValueH)
                        {
                        case 1:                                             //设备描述符
                            pDescr = device_descriptor;                               //把设备描述符送到要发送的缓冲区
                            len = sizeof(device_descriptor);
                            break;
                        case 2:                                             //配置描述符
                            pDescr = config_descriptor;                               //把设备描述符送到要发送的缓冲区
                            len = sizeof(config_descriptor);
                            break;
                        case 0x22:                                          //报表描述符
                            if(UsbSetupBuf->wIndexL == 0)                   //接口0报表描述符
                            {
                                pDescr = hid_report_descriptor;                        //数据准备上传
                                len = sizeof(hid_report_descriptor);
                                Ready = 1;                                  //如果有更多接口，该标准位应该在最后一个接口配置完成后有效
                                #if defined(DEBUG_UART_ISR)
                                ES = 1;                                     //开启串口中断
                                #endif
                            }
                            else
                            {
                                len = 0xff;                                 //本程序只有2个接口，这句话正常不可能执行
                            }
                            break;
                        default:
                            len = 0xff;                                     //不支持的命令或者出错
                            break;
                        }
                        if ( SetupLen > len )
                        {
                            SetupLen = len;    //限制总长度
                        }
                        len = SetupLen >= 8 ? 8 : SetupLen;                  //本次传输长度
                        memcpy(Ep0Buffer, pDescr, len);                        //加载上传数据
                        SetupLen -= len;
                        pDescr += len;
                        break;
                    case USB_SET_ADDRESS:
                        SetupLen = UsbSetupBuf->wValueL;                     //暂存USB设备地址
                        break;
                    case USB_GET_CONFIGURATION:
                        Ep0Buffer[0] = UsbConfig;
                        if ( SetupLen >= 1 )
                        {
                            len = 1;
                        }
                        break;
                    case USB_SET_CONFIGURATION:
                        UsbConfig = UsbSetupBuf->wValueL;
                        break;
                    case 0x0A:
                        break;
                    case USB_CLEAR_FEATURE:                                            //Clear Feature
                        if ( ( UsbSetupBuf->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_ENDP )// 端点
                        {
                            switch( UsbSetupBuf->wIndexL )
                            {
                            case 0x82:
                                UEP2_CTRL = UEP2_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
                                break;
                            case 0x81:
                                UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
                                break;
                            case 0x01:
                                UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_R_TOG | MASK_UEP_R_RES ) | UEP_R_RES_ACK;
                                break;
                            default:
                                len = 0xFF;                                            // 不支持的端点
                                break;
                            }
                        }
                        else
                        {
                            len = 0xFF;                                                // 不是端点不支持
                        }
                        break;
                    case USB_SET_FEATURE:                                              /* Set Feature */
                        if( ( UsbSetupBuf->bRequestType & 0x1F ) == 0x00 )             /* 设置设备 */
                        {
                            if( ( ( ( UINT16 )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x01 )
                            {
                                if( config_descriptor[ 7 ] & 0x20 )
                                {
                                    /* 设置唤醒使能标志 */
                                }
                                else
                                {
                                    len = 0xFF;                                        /* 操作失败 */
                                }
                            }
                            else
                            {
                                len = 0xFF;                                            /* 操作失败 */
                            }
                        }
                        else if( ( UsbSetupBuf->bRequestType & 0x1F ) == 0x02 )        /* 设置端点 */
                        {
                            if( ( ( ( UINT16 )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x00 )
                            {
                                switch( ( ( UINT16 )UsbSetupBuf->wIndexH << 8 ) | UsbSetupBuf->wIndexL )
                                {
                                case 0x82:
                                    UEP2_CTRL = UEP2_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点2 IN STALL */
                                    break;
                                case 0x02:
                                    UEP2_CTRL = UEP2_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;/* 设置端点2 OUT Stall */
                                    break;
                                case 0x81:
                                    UEP1_CTRL = UEP1_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点1 IN STALL */
                                    break;
                                default:
                                    len = 0xFF;                               //操作失败
                                    break;
                                }
                            }
                            else
                            {
                                len = 0xFF;                                   //操作失败
                            }
                        }
                        else
                        {
                            len = 0xFF;                                      //操作失败
                        }
                        break;
                    case USB_GET_STATUS:
                        Ep0Buffer[0] = 0x00;
                        Ep0Buffer[1] = 0x00;
                        if ( SetupLen >= 2 )
                        {
                            len = 2;
                        }
                        else
                        {
                            len = SetupLen;
                        }
                        break;
                    default:
                        len = 0xff;                                           //操作失败
                        break;
                    }
                }
            }
            else
            {
                len = 0xff;                                                   //包长度错误
            }
            if(len == 0xff)
            {
                SetupReq = 0xFF;
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;//STALL
            }
            else if(len <= 8)                                                //上传数据或者状态阶段返回0长度包
            {
                UEP0_T_LEN = len;
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1，返回应答ACK
            }
            else
            {
                UEP0_T_LEN = 0;  //虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1,返回应答ACK
            }
            break;
        case UIS_TOKEN_IN | 0:                                               //endpoint0 IN
            switch(SetupReq)
            {
            case USB_GET_DESCRIPTOR:
                len = SetupLen >= 8 ? 8 : SetupLen;                          //本次传输长度
                memcpy( Ep0Buffer, pDescr, len );                            //加载上传数据
                SetupLen -= len;
                pDescr += len;
                UEP0_T_LEN = len;
                UEP0_CTRL ^= bUEP_T_TOG;                                     //同步标志位翻转
                break;
            case USB_SET_ADDRESS:
                USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
                UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                break;
            default:
                UEP0_T_LEN = 0;                                              //状态阶段完成中断或者是强制上传0长度数据包结束控制传输
                UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                break;
            }
            break;
        case UIS_TOKEN_OUT | 0:  // endpoint0 OUT
            len = USB_RX_LEN;
            if((SetupReq == 0x09)&& (len == 1))
            {
              LED_VALID = Ep0Buffer[0];
            }
            else if((SetupReq == 0x09) && (len == 8)){//SetReport
            }
            UEP0_T_LEN = 0;  //虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
            UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA0,返回应答ACK
            break;
        default:
            break;
        }
        UIF_TRANSFER = 0;                                                 //写0清空中断
    }
    if(UIF_BUS_RST)                                                       //设备模式USB总线复位中断
    {
        UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        UEP1_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK;
        UEP2_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;
        USB_DEV_AD = 0x00;
        UIF_SUSPEND = 0;
        UIF_TRANSFER = 0;
        UIF_BUS_RST = 0;                                                 //清中断标志
    }
    if (UIF_SUSPEND)                                                     //USB总线挂起/唤醒完成
    {
        UIF_SUSPEND = 0;
    }
    else {                                                               //意外的中断,不可能发生的情况
        USB_INT_FG = 0xFF;                                               //清中断标志
    }
}

void key_code_correspond(key_code_enum keyCode)
{
    if (keyCode != KC_NONE) {
        HIDKey[2] = keyCode;
    }
}

/*******************************************************************************
* Function Name  : key_code_correspond()
* Description    : 键码比对表，由数值对应键盘值。
* Input          : UINT8 keyCode
* Output         : None
* Return         : None
*******************************************************************************/
void _key_code_correspond(uint8_t keyCode)
{
	HIDKey[0] = 0;
	if((keyCode>='a')&&(keyCode<='z')){                                       //键值a-z

		if(LED_VALID&0x02)
		{
			FLAG = 0;
			HIDKey[2] = 0x39;
			enp1_in_evt();
			while(FLAG == 0);
			FLAG = 0;
			memset(&HIDKey[0],0,8);
			enp1_in_evt();
			while(FLAG == 0);
		}
		keyCode -= 0x5D;
		HIDKey[2] = keyCode;

	}else if((keyCode>='A')&&(keyCode<='Z')){
		if((LED_VALID&0x02) == 0){
			FLAG = 0;
			HIDKey[2] = 0x39;
			enp1_in_evt();
			while(FLAG == 0);
			FLAG = 0;
			memset(&HIDKey[0],0,8);
			enp1_in_evt();
			while(FLAG == 0);
		}
	    keyCode -= 0x3D;
        HIDKey[2] = keyCode;
        // HIDKey[0]	= 0x02;	                                                  //shift+
    }else if((keyCode>='1')&&(keyCode<='9')){
		keyCode -= 0x13;                                                        //字母区数字键
		HIDKey[2] = keyCode;
	}else if(keyCode=='0'){
		HIDKey[2] = 0x27;
	}else if(keyCode <= 0x2f){
		if(keyCode == 0x20){
			HIDKey[2] = 0x2C;                                                     //空格
		}else if(keyCode == 0x21){//'!'
			HIDKey[2] = 0x1E;
			HIDKey[0]	= 0x02;	                                                  //shift+
		}else if(keyCode == 0x22){//'"'
			HIDKey[2] = 0x34;
			HIDKey[0]	= 0x02;	                                                  //shift+
		}else if(keyCode == 0x23){//'#'
			HIDKey[2] = 0x20;
			HIDKey[0]	= 0x02;	                                                  //shift+
		}else if(keyCode == 0x24){//'$'
			HIDKey[2] = 0x21;
			HIDKey[0]	= 0x02;	                                                  //shift+
		}else if(keyCode == 0x25){//'%'
			HIDKey[2] = 0x22;
			HIDKey[0]	= 0x02;	                                                  //shift+
		}else if(keyCode == 0x26){//'&'
			HIDKey[2] = 0x24;
			HIDKey[0]	= 0x02;	                                                  //shift+
		}else if(keyCode == 0x27){//'%'
			HIDKey[2] = 0x34;
		}else if(keyCode == 0x28){/*(*/
			HIDKey[2] = 0x26;
			HIDKey[0]	= 0x02;	                                                  //shift+
		}else if(keyCode == 0x29){/*)*/
			HIDKey[2] = 0x27;
			HIDKey[0]	= 0x02;	                                                  //shift+
		}else if(keyCode == 0x2a){/***/
			HIDKey[2] = 0x25;
			HIDKey[0]	= 0x02;	                                                  //shift+
		}else if(keyCode == 0x2b){/*+*/
			HIDKey[2] = 0x2e;
			HIDKey[0]	= 0x02;	                                                  //shift+
		}else if(keyCode == 0x2c){/*,*/
			HIDKey[2] = 0x36;
		}else if(keyCode == 0x2d){/*-*/
			HIDKey[2] = 0x2d;
		}else if(keyCode == 0x2e){/*,*/
			HIDKey[2] = 0x37;
		}else if(keyCode == 0x2f){/*/*/
			HIDKey[2] = 0x38;
		}
    }else if(keyCode <= 0x3f){
		if(keyCode == 0x3a){/*:*/
			HIDKey[2] = 0x33;
			HIDKey[0]	|= 0x02;	                                                  //shift+
		}else if(keyCode == 0x3b){/*;*/
			HIDKey[2] = 0x33;
		}else if(keyCode == 0x3c){/*<*/
			HIDKey[2] = 0x36;
			HIDKey[0]	|= 0x02;	                                                  //shift+
		}else if(keyCode == 0x3d){/*=*/
			HIDKey[2] = 0x2e;
		}else if(keyCode == 0x3e){/*>*/
			HIDKey[2] = 0x37;
			HIDKey[0]	|= 0x02;	                                                  //shift+
		}else if(keyCode == 0x3f){/*?*/
			HIDKey[2] = 0x38;
			HIDKey[0]	|= 0x02;	                                                  //shift+
		}
    }else if(keyCode == 0x40){/*@*/
        HIDKey[2] = 0x1f;
        HIDKey[0]	= 0x02;	                                                  //shift+
    }else if((keyCode >= 0x5b)&&(keyCode <= 0x60)){
		if(keyCode == 0x5b){/*[*/
			HIDKey[2] = 0x2f;
		}else if(keyCode == 0x5c){/*\*/
			HIDKey[2] = 0x31;
		}else if(keyCode == 0x5d){/*:*/
			HIDKey[2] = 0x30;
		}else if(keyCode == 0x5e){/*:*/
			HIDKey[2] = 0x23;
			HIDKey[0]	= 0x02;	                                                  //shift+
		}else if(keyCode == 0x5f){/*:*/
			HIDKey[2] = 0x2d;
			HIDKey[0]	= 0x02;	                                                  //shift+
		}else if(keyCode == 0x60){/*`*/
			HIDKey[2] = 0x35;
		}
	}else if((keyCode >= 0x7b)&&(keyCode <= 0x7f)){
		if(keyCode == 0x7b){/*{*/
			HIDKey[2] = 0x2f;
			HIDKey[0]	= 0x02;	                                                  //shift+
		}
		else if(keyCode == 0x7c){/*|*/
			HIDKey[2] = 0x31;
			HIDKey[0]	= 0x02;	                                                  //shift+
		}
		else if(keyCode == 0x7d){/*}*/
			HIDKey[2] = 0x30;
			HIDKey[0]	= 0x02;	                                                  //shift+
		}
		else if(keyCode == 0x7e){/*~*/
			HIDKey[2] = 0x35;
			HIDKey[0]	= 0x02;	                                                  //shift+
		}
		else if(keyCode == 0x7f){/*{*/
			HIDKey[2] = 0x4c;
		}
	}else if((keyCode >= 0x80)&&(keyCode <= 0x87)){/*left ctl*/
        keyCode &= 0x0f;
        HIDKey[0]	= (1<<keyCode);	                                          //
    }else if((keyCode >= 0xd7)&&(keyCode <= 0xda)){/**/
        keyCode -= 0x88;
        HIDKey[2]	= keyCode;
    }else if((keyCode >= 0xb0)&&(keyCode <= 0xb3)){/**/
        keyCode -= 0x88;
        HIDKey[2]	= keyCode;
    }else if((keyCode >= 0xd1)&&(keyCode <= 0xd5)){/**/
        keyCode -= 0x88;
        HIDKey[2]	= keyCode;
    }else if((keyCode >= 0xC1)&&(keyCode <= 0xCD)){/**/
        keyCode -= 0x88;
        HIDKey[2]	= keyCode;
    }
}

/*******************************************************************************
* Function Name  : usb_clear_flag()
* Description    : 清除USB设备状态
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usb_clear_flag(void)
{
    UEP1_T_LEN = 0;                                                       //预使用发送长度一定要清空
    FLAG = 0;
    Ready = 0;
	LED_VALID = 1;                                                        //给一个默认值
}

/*******************************************************************************
* Function Name  : __hid_value_handle()
* Description    : HID数据上报函数(运行函数)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void __hid_value_handle(void)
{
    if (data_buf[send_point] == KC_LEFT_SHIFT) {
        HIDKey[0] = 0x02;	
        send_point++;
	    if(send_point>=BUFMAX) send_point=0;
        return;
    }else {
        _key_code_correspond(data_buf[send_point]);
    }

	send_point++;
	if(send_point>=BUFMAX) send_point=0;

	FLAG = 0;
	enp1_in_evt();
	while(FLAG == 0);
	FLAG = 0;
	memset(&HIDKey[0],0,8);
	enp1_in_evt();
	while(FLAG == 0);
}

/*******************************************************************************
* Function Name  : hid_value_handle_proc()
* Description    : HID数据上报函数(运行函数)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hid_value_handle_proc(void)
{
    if(Ready&&(data_len>0)){
        __hid_value_handle();
        data_len--;

    }else{
        return;
    }
}

/*******************************************************************************
* Function Name  : get_keyboard_data()
* Description    : 获取键盘数据,并存入data_buf(通过hid_value_handle上报)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void get_keyboard_data(uint8_t in_data)
{
    data_buf[recv_point++] = in_data;
    data_len++;
    if(recv_point>=BUFMAX) recv_point = 0;
}


