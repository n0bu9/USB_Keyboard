#include "usb_hid.h"
#include "main.h"
#include "string.h"

#define THIS_ENDP0_SIZE         32UL
#define THIS_ENDP1_SIZE         32UL
#define THIS_ENDP2_SIZE         32UL
#pragma  NOAREGS

static uint8_t xdata Ep0Buffer[8>(THIS_ENDP0_SIZE+2)?8:(THIS_ENDP0_SIZE+2)] _at_ 0x0000;    //端点0 OUT&IN缓冲区，必须是偶地址
static uint8_t xdata Ep1Buffer[64>(MAX_PACKET_SIZE+2)?64:(MAX_PACKET_SIZE+2)] _at_ 0x00022;  //端点1 OUT&IN缓冲区，必须是偶地址
static uint8_t xdata Ep2Buffer[64>(MAX_PACKET_SIZE+2)?64:(MAX_PACKET_SIZE+2)] _at_ 0x00064;  //端点2 OUT&IN缓冲区，必须是偶地址

uint8_t hid_report_descriptor_1[] = {
    /* 键盘-6键无冲突 */
    0x05, 0x01,        // Usage Page (Generic Desktop)
    0x09, 0x06,        // Usage (Keyboard)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x01,        //   Report ID (1)
    0x05, 0x07,        //   Usage Page (Keyboard)
    0x19, 0xE0,        //   Usage Minimum (0xE0)
    0x29, 0xE7,        //   Usage Maximum (0xE7)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x75, 0x01,        //   Report Size (1)
    0x95, 0x08,        //   Report Count (8)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x01,        //   Report Count (1)
    0x75, 0x08,        //   Report Size (8)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x03,        //   Report Count (3)
    0x75, 0x01,        //   Report Size (1)
    0x05, 0x08,        //   Usage Page (LEDs)
    0x19, 0x01,        //   Usage Minimum (Num Lock)
    0x29, 0x03,        //   Usage Maximum (Scroll Lock)
    0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x95, 0x05,        //   Report Count (5)
    0x75, 0x01,        //   Report Size (1)
    0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x95, 0x06,        //   Report Count (6)
    0x75, 0x08,        //   Report Size (8)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
    0x19, 0x00,        //   Usage Minimum (0)
    0x29, 0x91,        //   Usage Maximum (91)
    0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection

    /* 键盘-扩展全键无冲 */
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x02,                    //   REPORT_ID (2)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x04,                    //   USAGE_MINIMUM (Keyboard a and A)
    0x29, 0x67,                    //   USAGE_MAXIMUM (Keypad =)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x63,                    //   REPORT_COUNT (99)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0xc0,                          // END_COLLECTION

    /* 通信 */
    0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined Page 1)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x03,                    //   REPORT_ID (3)
    0x09, 0x02,                    //   USAGE (Vendor Usage 2)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x95, 0x0a,                    //   REPORT_COUNT (10)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x06,                    //   INPUT (Data,Var,Rel)
    0x09, 0x02,                    //   USAGE (Vendor Usage 2)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x95, 0x0a,                    //   REPORT_COUNT (10)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x91, 0x06,                    //   OUTPUT (Data,Var,Rel)
    0xc0,                          // END_COLLECTION
};

// uint8_t hid_report_descriptor_2[] = {
//     /* 通信 */
//     0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined Page 1)
//     0x09, 0x01,                    // USAGE (Vendor Usage 1)
//     0xa1, 0x01,                    // COLLECTION (Application)
//     0x85, 0x03,                    //   REPORT_ID (3)
//     0x09, 0x02,                    //   USAGE (Vendor Usage 2)
//     0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
//     0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
//     0x95, 0x0a,                    //   REPORT_COUNT (10)
//     0x75, 0x08,                    //   REPORT_SIZE (8)
//     0x81, 0x06,                    //   INPUT (Data,Var,Rel)
//     0x09, 0x02,                    //   USAGE (Vendor Usage 2)
//     0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
//     0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
//     0x95, 0x0a,                    //   REPORT_COUNT (10)
//     0x75, 0x08,                    //   REPORT_SIZE (8)
//     0x91, 0x06,                    //   OUTPUT (Data,Var,Rel)
//     0xc0,                          // END_COLLECTION
// };

uint8_t device_descriptor[18] = {
    0x12, // 设备描述符的字节数大小(bLength)
    0x01, // 设备类型(bDescriptorType)
    0x00, 0x02, // USB版本(bcdUSB)
    0x00, // 设备类代码(bDeviceClass)
    0x00, // 设备子类代码(bDeviceSubClass)
    0x00, // 设备协议代码(bDeviceProtocol)
    THIS_ENDP0_SIZE, // 端点0的最大包大小(bMaxPacketSize0)
    0x3d,0x41, // 厂商编号(idVendor)
    0x07,0x21, // 产品编号(idProduct)
    0x00,0x00, // 设备出厂编号(bcdDevice)
    0x00, // 制造商字符串索引(iManufacturer)
    0x00, // 产品字符串索引(iProduct)
    0x00, // 序列号字符串索引(iSerialNumber)
    0x01, // 配置数量(bNumConfigurations)
};

uint8_t config_descriptor[41] = {
    /* 配置描述符 9B */
    0x09, // 配置描述符的字节数大小(bLength)
    0x02, // 配置描述符的类型(bDescriptorType)
    0x29,0x00, // 返回整个数据的长度.指此配置返回的配置描述符,接口描述符以及端点描述符的全部大小(wTotalLength)
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
    0x02, // 该接口使用端点数(bNumEndpoints)
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
    LOW_BYTE(hid_report_descriptor_1),HIGH_BYTE(hid_report_descriptor_1), // 该类别描述符的总长度(wDescriptorLength)
    /* 端点描述符 7B */
    0x07, // 端点描述符的字节数大小(bLength)
    0x05, // 端点描述符的类型(bDescriptorType)
    0x81, // USB设备的端点地址(bEndpointAddress)
    0x03, // 端点方向(bmAttributes)
    THIS_ENDP1_SIZE,0x00, // 本端点接收或发送的最大信息包大小(wMaxPacketSize)
    0x01, // 端点间隔时间(bInterval)
    /* 端点描述符 7B */
    0x07, // 端点描述符的字节数大小(bLength)
    0x05, // 端点描述符的类型(bDescriptorType)
    0x02, // USB设备的端点地址(bEndpointAddress)
    0x03, // 端点方向(bmAttributes)
    THIS_ENDP2_SIZE,0x00, // 本端点接收或发送的最大信息包大小(wMaxPacketSize)
    0x01, // 端点间隔时间(bInterval)
};

/*键盘数据*/
static uint8_t HIDKey[9] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};  // 1B-ReportID, 8B-KeyCode
static uint8_t HIDKey_last[9] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
static uint8_t HIDKey_extend[14] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}; // 1B-ReportID, 13B-KeyCode
static uint8_t HIDKey_extend_last[14] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
static uint8_t HID_out_info[11] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
static uint8_t hid_key_index = 3;  // 从第3个开始
static uint8_t SetupReq = 0,SetupLen = 0,Ready = 0,Count = 0,UsbConfig = 0;
static uint8_t LED_VALID = 0;
volatile bool edp1_idle = 0, out_info_flag = 0;
static uint8_t *pDescr; // USB配置标志
//static USB_SETUP_REQ   SetupReqBuf; // 暂存Setup包

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
    // EndPoint 0
    UEP0_DMA = Ep0Buffer;                                                      //端点0数据传输地址
    UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;                                 //OUT事务返回ACK，IN事务返回NAK
    // EndPoint 1
    UEP1_DMA = Ep1Buffer;                                                      //端点1数据传输地址
    UEP4_1_MOD = UEP4_1_MOD & ~bUEP1_BUF_MOD | bUEP1_TX_EN;                    //端点1单64字节发送(IN)缓冲区
    UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_STALL;               // 端点2自动翻转同步标志位，IN事务返回NAK，OUT返回STALL
    // EndPoint 2
    UEP2_DMA = Ep2Buffer;                                                      //
    UEP2_3_MOD = UEP2_3_MOD & ~bUEP2_BUF_MOD | bUEP2_RX_EN;                    // 端点2发送使能 64字节缓冲区
    UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_STALL | UEP_R_RES_ACK;               // 端点2自动翻转同步标志位，IN事务返回STALL，OUT返回ACK
    // Other Init
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
* Input          : data[]: 数据
*                  len: 数据长度
* Output         : None
* Return         : None
*******************************************************************************/
void enp1_in_evt(uint8_t usb_data[], uint8_t len)
{
    memcpy(Ep1Buffer, usb_data, len);                                            //加载上传数据
    UEP1_T_LEN = len;                                                        //上传数据长度
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
        case UIS_TOKEN_OUT | 2:                                                 //endpoint 2# 中断端点下传
            if ( U_TOG_OK )                                                     // 不同步的数据包将丢弃
            {
				len = USB_RX_LEN;                                               //接收数据长度，数据从Ep2Buffer首地址开始存放
                //UEP2_CTRL ^= bUEP_R_TOG;                                      //手动翻转
				memcpy(HID_out_info, Ep2Buffer, len);
                UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_R_RES | UEP_R_RES_ACK;       //默认应答ACK
                out_info_flag = 1;
            }
            break;
        case UIS_TOKEN_IN | 1:                                                  //endpoint 1# 中断端点上传
            UEP1_T_LEN = 0;                                                     //预使用发送长度一定要清空
//            UEP2_CTRL ^= bUEP_T_TOG;                                          //如果不设置自动翻转则需要手动翻转
            UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;           //默认应答NAK
            edp1_idle = 1;                                                      /*传输完成标志*/
            break;
        case UIS_TOKEN_SETUP | 0:                                                //SETUP事务
            len = USB_RX_LEN;
            if(len == (sizeof(USB_SETUP_REQ)))
            {
                SetupLen = UsbSetupBuf->wLengthL;
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
                            if(UsbSetupBuf->wIndexL == 0) {                 //接口0报表描述符
                                pDescr = hid_report_descriptor_1;           //数据准备上传
                                len = sizeof(hid_report_descriptor_1);
                                Ready = 1;                                  //如果有更多接口，该标准位应该在最后一个接口配置完成后有效
                            // }else if (UsbSetupBuf->wIndexL == 1){
                            //     pDescr = hid_report_descriptor_2;
                            //     len = sizeof(hid_report_descriptor_2);
                            //     Ready = 1;                                  //如果有更多接口，该标准位应该在最后一个接口配置完成后有效
                            }else {
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
                        len = SetupLen >= THIS_ENDP0_SIZE ? THIS_ENDP0_SIZE : SetupLen;                  //本次传输长度
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
            else if(len <= THIS_ENDP0_SIZE)                                                //上传数据或者状态阶段返回0长度包
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
                len = SetupLen >= THIS_ENDP0_SIZE ? THIS_ENDP0_SIZE : SetupLen; //本次传输长度
                memcpy( Ep0Buffer, pDescr, len );                               //加载上传数据
                SetupLen -= len;
                pDescr += len;
                UEP0_T_LEN = len;
                UEP0_CTRL ^= bUEP_T_TOG;                                        //同步标志位翻转
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
            else if((SetupReq == 0x09) && (len == THIS_ENDP0_SIZE)){//SetReport
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
        UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_STALL;
        UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_STALL | UEP_R_RES_ACK;      // 端点2自动翻转同步标志位，IN事务返回STALL，OUT返回ACK
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
    edp1_idle = 0;
    Ready = 0;
	LED_VALID = 1;                                                        //给一个默认值
}
/*******************************************************************************
* Function Name  : __keycode_fill_report()
* Description    : 填充基本HID键盘报文(内部调用)
* Input          : keycode:键盘键码
* Output         : None
* Return         : None
*******************************************************************************/
bool __keycode_fill_report(key_code_enum keycode)
{
    HIDKey[0] = 0x01;

    if (keycode >= KC_LEFT_CTRL && keycode <= KC_RIGHT_GUI) {
        HIDKey[1] |= 1 << (keycode - KC_LEFT_CTRL);
        return TRUE;
    }
    if (hid_key_index > 8) {  // 最多6个键，当超出6个键时，启用全键无冲扩展
        return FALSE;
    }
    HIDKey[hid_key_index] = keycode;
    hid_key_index++;

    return TRUE;
}
/*******************************************************************************
* Function Name  : __keycode_clear_report()
* Description    : 清除基本HID键盘报文(内部调用)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void __keycode_clear_report(void)
{
    memcpy(HIDKey_last, HIDKey, sizeof(HIDKey));  // 当前状态置为上一次状态
    memset(HIDKey, 0, sizeof(HIDKey));            // 清空当前状态
    HIDKey[0] = 0x01;                             // report ID
    hid_key_index = 3;                            // 重置键位索引
}
/*******************************************************************************
* Function Name  : __keycode_fill_report_extend()
* Description    : 填充全键无冲扩展键盘报文(内部调用)
* Input          : keycode:键盘键码
* Output         : None
* Return         : None
*******************************************************************************/
void __keycode_fill_report_extend(key_code_enum keycode)
{
    HIDKey_extend[0] = 0x02;  // report ID
    keycode = keycode - KC_a_A;
    if (keycode <= 7) {
        HIDKey_extend[1] |= 1 << keycode;
    } else {
        HIDKey_extend[1 + keycode / 8] |= 1 << (keycode % 8);
    }
}
/*******************************************************************************
* Function Name  : __keycode_clear_report_extend()
* Description    : 清除全键无冲扩展键盘报文(内部调用)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void __keycode_clear_report_extend(void)
{
    memcpy(HIDKey_extend_last, HIDKey_extend, sizeof(HIDKey_extend));  // 当前状态置为上一次状态
    memset(HIDKey_extend, 0, sizeof(HIDKey_extend));                   // 清空当前状态
    HIDKey_extend[0] = 0x02;                                           // report ID
}
/*******************************************************************************
* Function Name  : keycode_fill_report()
* Description    : 填充HID键盘报文(外部调用)
* Input          : keycode:键盘键码
* Output         : None
* Return         : None
*******************************************************************************/
void keycode_fill_report(key_code_enum keycode)
{
    bool ret = __keycode_fill_report(keycode);
    if (!ret) {
        __keycode_fill_report_extend(keycode);
    }
}

void keyboard_info_get(uint8_t *buff)
{
    if (out_info_flag){
        out_info_flag = 0;
        if (HID_out_info[0] == 0x03){
            memcpy(buff, HID_out_info + sizeof(uint8_t), sizeof(uint8_t)*10);
        }
    }
}
/*******************************************************************************
* Function Name  : keycode_input_proc()
* Description    : 键盘HID报文上报
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void keycode_input_proc(void)
{
    if (memcmp(HIDKey, HIDKey_last, sizeof(HIDKey)) != 0) {
        ENP1_IN_EVT(HIDKey, sizeof(HIDKey));
    }
    if (memcmp(HIDKey_extend, HIDKey_extend_last, sizeof(HIDKey_extend)) != 0) {
        ENP1_IN_EVT(HIDKey_extend, sizeof(HIDKey_extend));
    }
    __keycode_clear_report();
    __keycode_clear_report_extend();
}

