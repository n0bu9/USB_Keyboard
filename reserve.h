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
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xc0,                          // END_COLLECTION

    // 0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined Page 1)
    // 0x09, 0x02,                    // USAGE (Vendor Usage 2)
    // 0xa1, 0x01,                    // COLLECTION (Application)
    // 0x85, 0x04,                    //   REPORT_ID (4)
    // 0x09, 0x02,                    //   USAGE (Vendor Usage 2)
    // 0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    // 0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    // 0x95, 0x0a,                    //   REPORT_COUNT (10)
    // 0x75, 0x08,                    //   REPORT_SIZE (8)
    // 0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    // 0xc0                           // END_COLLECTION
