/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    user_gpio_proc.c
  * @brief   This file provides code for the configuration
  *          of IO-Link ISDU.
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "IOL_ISDU.h"
#include "IOL_Base.h"

// IOL_ISDUPacket_t IOL_ISDUpacket;
IOL_ISDUPacket_t isdudata;
IOL_IndexTable IOL_indextable;

IOL_ISDUPacket_t ISDU_WritePacketframe[30] = 
{
    // [iservice]                   [length]                  [index]                 [subindex]          [chkpdu]
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_directParameter_page1,           0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_directParameter_page2,           0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_Systemcommand,                   0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_Datastorageindex,                0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_DeviceAccessLocks,               0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_ProfileCharacteristic,           0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_PDInputDescriptor,               0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_PDOutputDescriptor,              0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_VendorName,                      0       ,         0          , "Seengrip"   },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_VendorText,                      0       ,         0          , "Seengrip, Inc"  },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_ProductName,                     0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_ProductID,                       0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_ProductText,                     0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_SerialNumber,                    0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_HardwareRevision,                0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_FirmwareRevision,                0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_ApplicationSpecificTag,          0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_FunctionTag,                     0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_LocationTag,                     0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_ProductURI,                      0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_directParameter_page1,           0       ,         0             },
    { IOL_Iservice_M_Wreq8bit,          0     , IOL_Index_directParameter_page1,           0       ,         0             },
};

uint8_t extlength_flag = 0;
uint8_t IOL_OP_ISDU_OD_Res_cnt = 0;


// static uint8_t device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH]; // + 1   CKS
uint8_t device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH]; // + 1   CKS
uint8_t device_ProcessDataOut_Arr[IOL_OP_ISDU_OUT_PROCESSDATALENGTH];

extern UART_HandleTypeDef huart1;
extern uint8_t ProcessDataIn_cnt;

static uint8_t IOL_OP_ProductName[IOL_OP_ISDU_PRODUCTNAME_LENGTH][IOL_OP_ISDU_OD_LENGTH] = {
    {0xd1, 0x1a},
    {0x53, 0x65},
    {0x65, 0x6e},
    {0x67, 0x72},
    {0x69, 0x70},
    {0x2d, 0x49},
    {0x4f, 0x4c},
    {0x2d, 0x47},
    {0x72, 0x69},
    {0x70, 0x70},
    {0x65, 0x72},
    {0x2d, 0x30},
    {0x31, 0xd7}
};

#if 0
static uint8_t IOL_OP_SerialNumber[IOL_OP_ISDU_SERIALNUMBER_LENGTH][IOL_OP_ISDU_OD_LENGTH] = {
    {0xd1, 0x13},
    {0x46, 0x33},
    {0x30, 0x36},
    {0x30, 0x32},
    {0x35, 0x37},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0xb1, 0x00}
};
#else
static uint8_t IOL_OP_SerialNumber[IOL_OP_ISDU_SERIALNUMBER_LENGTH][IOL_OP_ISDU_OD_LENGTH] = {
    {0xda, 0x46},
    {0x33, 0x30},
    {0x36, 0x30},
    {0x32, 0x35},
    {0x37, 0xa9}
};
#endif

static uint8_t IOL_OP_ApplicationSpecificTag[IOL_OP_ISDU_APPLICATIONSPECIFICTAG_LENGTH][IOL_OP_ISDU_OD_LENGTH] = {
    {0xd1, 0x23},
    {0x2a, 0x2a},
    {0x2a, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0xd8, 0x00}
};

#if 0
static uint8_t IOL_OP_ProductID[IOL_OP_ISDU_PRODUCTID_LENGTH][IOL_OP_ISDU_OD_LENGTH] = {
    {0xd1, 0x0c},
    {0x67, 0x72},
    {0x69, 0x70},
    {0x70, 0x65},
    {0x72, 0x30},
    {0x31, 0xb7}
};
#else
static uint8_t IOL_OP_ProductID[IOL_OP_ISDU_PRODUCTID_LENGTH][IOL_OP_ISDU_OD_LENGTH] = {
    {0xdb, 0x67},
    {0x72, 0x69},
    {0x70, 0x70},
    {0x65, 0x72},
    {0x30, 0x31},
    {0xb1, 0x00}
};
#endif

#if 0
static uint8_t IOL_OP_ExampleParameter[IOL_OP_ISDU_PRODUCTID_LENGTH][IOL_OP_ISDU_OD_LENGTH] = {
    {0xd1, 0x05},
    {0x03, 0xe8},
    {0x3f, 0x00}
};
#else
static uint8_t IOL_OP_ExampleParameter[IOL_OP_ISDU_PRODUCTID_LENGTH][IOL_OP_ISDU_OD_LENGTH] = {
    {0xd4, 0x03},
    {0xe8, 0x3f},
    {0x00, 0x00}
};
#endif


static uint8_t IOL_OP_DeviceStatus[IOL_OP_ISDU_DEVICESTATUS_LENGTH][IOL_OP_ISDU_OD_LENGTH] = {
    {0xd3, 0x00},
    {0xd3, 0x00}
};

static uint8_t IOL_OP_DetailedDeviceStatus[IOL_OP_ISDU_DETAILEDDEVICESTATUS_LENGTH][IOL_OP_ISDU_OD_LENGTH] = {
    {0xd5, 0x00},
    {0x00, 0x00},
    {0xd5, 0x00}
};

static uint8_t IOL_OP_VendorName[IOL_OP_ISDU_VENDORNAME_LENGTH][IOL_OP_ISDU_OD_LENGTH] = {
    {0xd1, 0x43},
    {0x53, 0x65},
    {0x65, 0x6e},
    {0x67, 0x72},
    {0x69, 0x70},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0xeb, 0x00},
};

static uint8_t IOL_OP_HWRevision[IOL_OP_ISDU_HWREVISION_LENGTH][IOL_OP_ISDU_OD_LENGTH] = {
    {0xd1, 0x43},
    {0x31, 0x2e},
    {0x30, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0xbd, 0x00},
};

static uint8_t IOL_OP_FWRevision[IOL_OP_ISDU_FWREVISION_LENGTH][IOL_OP_ISDU_OD_LENGTH] = {
    {0xd1, 0x43},
    {0x31, 0x2e},
    {0x30, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0x00, 0x00},
    {0xbd, 0x00},
};

//IO-Link ISDU 의 Iservice
static uint8_t IOL_Get_ISDU_Iservice (uint8_t * pData)
{
    uint8_t first_od = 0;
    // IOL_ISDUPacket_t isdudata;

    first_od = pData[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1];
    first_od = (first_od >> 4 ) & 0x0F;

    isdudata.iservice = first_od;
  
    return isdudata.iservice;
}

// IO-lInk ISDU 의 extLength 
static uint8_t IOL_Get_ISDU_ExtLength (uint8_t * pData)
{
    uint8_t od_extlength = 0;

    od_extlength = pData[IOL_OP_ISDU_IN_PROCESSDATALENGTH];
    
    return od_extlength;
}

// IO-Link ISDU 의 Length
static uint8_t IOL_Get_ISDU_Length (uint8_t * pData)
{
    uint8_t first_od = 0;
    uint8_t extlength = 0;
    // IOL_ISDUPacket_t isdudata;

    first_od = pData[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1];
    extlength = first_od & 0x0F;

    // extlength 있는 경우
    if (extlength == 1)
    {
        extlength_flag = 1;
        extlength = IOL_Get_ISDU_ExtLength(pData);
        isdudata.length = extlength;
    }
    else // extlegnth 없는 경우
    {
        isdudata.length = extlength;
    }

    return isdudata.length;
}

static uint8_t IOL_Get_ISDU_Index (uint8_t * pData)
{
    //extlegnth 있는 경우
    if (extlength_flag == 1)
    {
        isdudata.index = isdudata.isdu_od[2];
        extlength_flag = 0;
    }
    else // extlength 없는 경우
    {
        isdudata.index = isdudata.isdu_od[1];
    }

    // 마스터에서 8bit index 쓰기 요청이 있는 경우
    if(isdudata.iservice == 0x01)
    {
        // 8bit 쓰기 요청 플래그 셋
        isdudata.isdu_od_writeReq8bit_flag = 1;
    }

    return isdudata.index;
}

// ISDU Write OD 데이터 수신 처리
uint8_t IOL_Get_ISDU_WR_ODArr (uint8_t * pData)
{
    
    isdudata.isdu_od[isdudata.isdu_od_cnt++] = pData[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1];
    isdudata.isdu_od[isdudata.isdu_od_cnt++] = pData[IOL_OP_ISDU_IN_PROCESSDATALENGTH];
    
    // 첫 OD 데이터 수신시 Iservice값과 ISDU의 데이터 길이를 구하기 위함.
    if (isdudata.isdu_od_cnt == 2)
    {
        IOL_Get_ISDU_Iservice(pData);
        IOL_Get_ISDU_Length(pData);
    }

    // OD 데이터의 길이까지 데이터 쌓은 경우.
    if (isdudata.isdu_od_cnt >= isdudata.length)
    {
        IOL_Get_ISDU_Index(pData);

        isdudata.length = 0;
        isdudata.isdu_od_cnt = 0; // od length clear
        isdudata.isdu_od_rxcplt = 1;
    }

    return ;
}

uint8_t IOL_Get_ISDU_RD_ODArr (uint8_t * pData)
{

    // IOL_Get_ISDU_Iservice(pData);
    // IOL_Get_ISDU_Length(pData);
    // IOL_Get_ISDU_Index(pData);

    if (isdudata.isdu_od_cnt < isdudata.length)
    {
        isdudata.isdu_od[isdudata.isdu_od_cnt++] = pData[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1];
        isdudata.isdu_od[isdudata.isdu_od_cnt++] = pData[IOL_OP_ISDU_IN_PROCESSDATALENGTH];
    }

    if (isdudata.isdu_od_cnt >= isdudata.length)
    {
        isdudata.length = 0;
        isdudata.isdu_od_cnt = 0; // od length clear
        isdudata.isdu_od_rxcplt = 1;
    }

    return ;
}

// IO-Link 송/수신 데이터를 업데이트 및 갱신하기 전 송신완료시 버퍼를 클리어해 준다
void IOL_Clear_PDBuffer (void)
{
    memset(device_ProcessDataIn_Arr, 0, sizeof(device_ProcessDataIn_Arr));
    memset(device_ProcessDataOut_Arr, 0, sizeof(device_ProcessDataOut_Arr));
}

// 응답 데이터 생성
static void IOL_Make_Resp_ProductName (void)
{
    device_ProcessDataIn_Arr[0] = IOL_OP_ProductName[IOL_OP_ISDU_OD_Res_cnt][0];   // OD 데이터를  Index에 대한 응답 ISDU 구조의 사이즈에 맞게 나눠서 보내기 위함  
    device_ProcessDataIn_Arr[1] = IOL_OP_ProductName[IOL_OP_ISDU_OD_Res_cnt][1];   // 현재 씬그립 디바이스의 IODD는 2 octet.
    IOL_OP_ISDU_OD_Res_cnt++;        // ISDU 요청에 대한 Index 응답의 사이즈에 맞게 나눠서 보내기 위한 카운트값.

    if (IOL_OP_ISDU_OD_Res_cnt >= IOL_OP_ISDU_PRODUCTNAME_LENGTH)
    {
        isdudata.isdu_od_writereq_flag = 0;
        IOL_OP_ISDU_OD_Res_cnt = 0;
    }

    device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_Arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
}

static void IOL_Make_Resp_SerialNumber (void)
{
    device_ProcessDataIn_Arr[0] = IOL_OP_SerialNumber[IOL_OP_ISDU_OD_Res_cnt][0];   // OD 데이터를  Index에 대한 응답 ISDU 구조의 사이즈에 맞게 나눠서 보내기 위함  
    device_ProcessDataIn_Arr[1] = IOL_OP_SerialNumber[IOL_OP_ISDU_OD_Res_cnt][1];   // 현재 씬그립 디바이스의 IODD는 2 octet.
    IOL_OP_ISDU_OD_Res_cnt++;        // ISDU 요청에 대한 Index 응답의 사이즈에 맞게 나눠서 보내기 위한 카운트값.

    // if (IOL_OP_ISDU_OD_Res_cnt >= 10)
    if (IOL_OP_ISDU_OD_Res_cnt >= IOL_OP_ISDU_SERIALNUMBER_LENGTH)
    {
        isdudata.isdu_od_writereq_flag = 0;
        IOL_OP_ISDU_OD_Res_cnt = 0;
    }

    device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_Arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
}

static void IOL_Make_Resp_ApplicationSpecificTag (void)
{
    device_ProcessDataIn_Arr[0] = IOL_OP_ApplicationSpecificTag[IOL_OP_ISDU_OD_Res_cnt][0];   // OD 데이터를  Index에 대한 응답 ISDU 구조의 사이즈에 맞게 나눠서 보내기 위함  
    device_ProcessDataIn_Arr[1] = IOL_OP_ApplicationSpecificTag[IOL_OP_ISDU_OD_Res_cnt][1];   // 현재 씬그립 디바이스의 IODD는 2 octet.
    IOL_OP_ISDU_OD_Res_cnt++;        // ISDU 요청에 대한 Index 응답의 사이즈에 맞게 나눠서 보내기 위한 카운트값.

    if (IOL_OP_ISDU_OD_Res_cnt >= IOL_OP_ISDU_APPLICATIONSPECIFICTAG_LENGTH)
    {
        isdudata.isdu_od_writereq_flag = 0;
        IOL_OP_ISDU_OD_Res_cnt = 0;
    }

    device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_Arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
}

static void IOL_Make_Resp_ProductID (void)
{
    device_ProcessDataIn_Arr[0] = IOL_OP_ProductID[IOL_OP_ISDU_OD_Res_cnt][0];   // OD 데이터를  Index에 대한 응답 ISDU 구조의 사이즈에 맞게 나눠서 보내기 위함  
    device_ProcessDataIn_Arr[1] = IOL_OP_ProductID[IOL_OP_ISDU_OD_Res_cnt][1];   // 현재 씬그립 디바이스의 IODD는 2 octet.
    IOL_OP_ISDU_OD_Res_cnt++;        // ISDU 요청에 대한 Index 응답의 사이즈에 맞게 나눠서 보내기 위한 카운트값.

    if (IOL_OP_ISDU_OD_Res_cnt >= IOL_OP_ISDU_PRODUCTID_LENGTH)
    {
        isdudata.isdu_od_writereq_flag = 0;
        IOL_OP_ISDU_OD_Res_cnt = 0;
    }

    device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_Arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
}

static void IOL_Make_Resp_ExampleParameter (void)
{
    device_ProcessDataIn_Arr[0] = IOL_OP_ExampleParameter[IOL_OP_ISDU_OD_Res_cnt][0];   // OD 데이터를  Index에 대한 응답 ISDU 구조의 사이즈에 맞게 나눠서 보내기 위함  
    device_ProcessDataIn_Arr[1] = IOL_OP_ExampleParameter[IOL_OP_ISDU_OD_Res_cnt][1];   // 현재 씬그립 디바이스의 IODD는 2 octet.
    IOL_OP_ISDU_OD_Res_cnt++;        // ISDU 요청에 대한 Index 응답의 사이즈에 맞게 나눠서 보내기 위한 카운트값.

    if (IOL_OP_ISDU_OD_Res_cnt >= IOL_OP_ISDU_EXAMPLEPARAMETER_LENGTH)
    {
        isdudata.isdu_od_writereq_flag = 0;
        IOL_OP_ISDU_OD_Res_cnt = 0;
    }

    device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_Arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
}

static void IOL_Make_Resp_DeviceStatus (void)
{
    device_ProcessDataIn_Arr[0] = IOL_OP_DeviceStatus[IOL_OP_ISDU_OD_Res_cnt][0];   // OD 데이터를  Index에 대한 응답 ISDU 구조의 사이즈에 맞게 나눠서 보내기 위함  
    device_ProcessDataIn_Arr[1] = IOL_OP_DeviceStatus[IOL_OP_ISDU_OD_Res_cnt][1];   // 현재 씬그립 디바이스의 IODD는 2 octet.
    IOL_OP_ISDU_OD_Res_cnt++;        // ISDU 요청에 대한 Index 응답의 사이즈에 맞게 나눠서 보내기 위한 카운트값.

    if (IOL_OP_ISDU_OD_Res_cnt >= IOL_OP_ISDU_DEVICESTATUS_LENGTH)
    {
        isdudata.isdu_od_writereq_flag = 0;
        IOL_OP_ISDU_OD_Res_cnt = 0;
    }

    device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_Arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
}

static void IOL_Make_Resp_DetailedDeviceStatus (void)
{
    device_ProcessDataIn_Arr[0] = IOL_OP_DetailedDeviceStatus[IOL_OP_ISDU_OD_Res_cnt][0];   // OD 데이터를  Index에 대한 응답 ISDU 구조의 사이즈에 맞게 나눠서 보내기 위함  
    device_ProcessDataIn_Arr[1] = IOL_OP_DetailedDeviceStatus[IOL_OP_ISDU_OD_Res_cnt][1];   // 현재 씬그립 디바이스의 IODD는 2 octet.
    IOL_OP_ISDU_OD_Res_cnt++;        // ISDU 요청에 대한 Index 응답의 사이즈에 맞게 나눠서 보내기 위한 카운트값.

    if (IOL_OP_ISDU_OD_Res_cnt >= IOL_OP_ISDU_DETAILEDDEVICESTATUS_LENGTH)
    {
        isdudata.isdu_od_writereq_flag = 0;
        IOL_OP_ISDU_OD_Res_cnt = 0;
    }

    device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_Arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
}

static void IOL_Make_Resp_VendorName (void)
{
    device_ProcessDataIn_Arr[0] = IOL_OP_VendorName[IOL_OP_ISDU_OD_Res_cnt][0];   // OD 데이터를  Index에 대한 응답 ISDU 구조의 사이즈에 맞게 나눠서 보내기 위함  
    device_ProcessDataIn_Arr[1] = IOL_OP_VendorName[IOL_OP_ISDU_OD_Res_cnt][1];   // 현재 씬그립 디바이스의 IODD는 2 octet.
    IOL_OP_ISDU_OD_Res_cnt++;        // ISDU 요청에 대한 Index 응답의 사이즈에 맞게 나눠서 보내기 위한 카운트값.

    if (IOL_OP_ISDU_OD_Res_cnt >= IOL_OP_ISDU_VENDORNAME_LENGTH)
    {
        isdudata.isdu_od_writereq_flag = 0;
        IOL_OP_ISDU_OD_Res_cnt = 0;
    }

    device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_Arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
}

static void IOL_Make_Resp_HWRevision (void)
{
    device_ProcessDataIn_Arr[0] = IOL_OP_HWRevision[IOL_OP_ISDU_OD_Res_cnt][0];   // OD 데이터를  Index에 대한 응답 ISDU 구조의 사이즈에 맞게 나눠서 보내기 위함  
    device_ProcessDataIn_Arr[1] = IOL_OP_HWRevision[IOL_OP_ISDU_OD_Res_cnt][1];   // 현재 씬그립 디바이스의 IODD는 2 octet.
    IOL_OP_ISDU_OD_Res_cnt++;        // ISDU 요청에 대한 Index 응답의 사이즈에 맞게 나눠서 보내기 위한 카운트값.

    if (IOL_OP_ISDU_OD_Res_cnt >= IOL_OP_ISDU_HWREVISION_LENGTH)
    {
        isdudata.isdu_od_writereq_flag = 0;
        IOL_OP_ISDU_OD_Res_cnt = 0;
    }

    device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_Arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
}

static void IOL_Make_Resp_FWRevision (void)
{
    device_ProcessDataIn_Arr[0] = IOL_OP_FWRevision[IOL_OP_ISDU_OD_Res_cnt][0];   // OD 데이터를  Index에 대한 응답 ISDU 구조의 사이즈에 맞게 나눠서 보내기 위함  
    device_ProcessDataIn_Arr[1] = IOL_OP_FWRevision[IOL_OP_ISDU_OD_Res_cnt][1];   // 현재 씬그립 디바이스의 IODD는 2 octet.
    IOL_OP_ISDU_OD_Res_cnt++;        // ISDU 요청에 대한 Index 응답의 사이즈에 맞게 나눠서 보내기 위한 카운트값.

    if (IOL_OP_ISDU_OD_Res_cnt >= IOL_OP_ISDU_FWREVISION_LENGTH)
    {
        isdudata.isdu_od_writereq_flag = 0;
        IOL_OP_ISDU_OD_Res_cnt = 0;
    }

    device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_Arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
}

// 각 index 응답 송신하는 부분을 모듈화하기 위한 함수
static void IOL_Make_Resp_PDOD (uint8_t * respArray, uint8_t size)
{
    uint8_t (* Resp_IndexArray)[IOL_OP_ISDU_OD_LENGTH];
    uint8_t od_size = 0;

    Resp_IndexArray = respArray;
    od_size = size;

    device_ProcessDataIn_Arr[0] = Resp_IndexArray[IOL_OP_ISDU_OD_Res_cnt][0];   // OD 데이터를  Index에 대한 응답 ISDU 구조의 사이즈에 맞게 나눠서 보내기 위함  
    device_ProcessDataIn_Arr[1] = Resp_IndexArray[IOL_OP_ISDU_OD_Res_cnt][1];   // 현재 씬그립 디바이스의 IODD는 2 octet.
    IOL_OP_ISDU_OD_Res_cnt++;        // ISDU 요청에 대한 Index 응답의 사이즈에 맞게 나눠서 보내기 위한 카운트값.

    if (IOL_OP_ISDU_OD_Res_cnt >= od_size)
    {
        isdudata.isdu_od_writereq_flag = 0;
        IOL_OP_ISDU_OD_Res_cnt = 0;
    }

    device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_Arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
}

#if 0 // Event trigger 테스트 용도
static void IOL_Make_Resp_default (void)
{
    // device_ProcessDataIn_arr[OP_ISDU_IN_PROCESSDATALENGTH - 2] = ProcessDataIn_cnt++; // Test cnt Value 
    device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 2] = ProcessDataIn_cnt; // Test cnt Value 
    device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_Arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
}
#else
static void IOL_Make_Resp_default (void)
{
    // device_ProcessDataIn_arr[OP_ISDU_IN_PROCESSDATALENGTH - 2] = ProcessDataIn_cnt++; // Test cnt Value 
    device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 2] = ProcessDataIn_cnt; // Test cnt Value 

    if (ProcessDataIn_cnt == 40)
    {
        ProcessDataIn_cnt++;
        device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_Arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 1); 
        
    }
    else if (ProcessDataIn_cnt == 60)
    {
        ProcessDataIn_cnt++;
        device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_Arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 1); 
    }
    else
    {
        device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_Arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
    }
    
}
#endif

static void IOL_Make_WriteRespPlus (void)
{
    device_ProcessDataIn_Arr[0] = 0x52;   // OD 데이터를  Index에 대한 응답 ISDU 구조의 사이즈에 맞게 나눠서 보내기 위함  
    device_ProcessDataIn_Arr[1] = 0x52;   // 현재 씬그립 디바이스의 IODD는 2 octet.

    device_ProcessDataIn_Arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_Arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
}

// ISDU OD Read Process
uint8_t IOL_State_OP_ISDU_ReadProcess (void)
{
    #if 0
    if ((isdudata.isdu_od_writereq_flag == 1) && (isdudata.isdu_od_writeReq8bit_flag == 1))
    {
        isdudata.isdu_od_writeReq8bit_flag = 0;
        IOL_Make_WriteRespPlus();
    }
    else if (isdudata.isdu_od_writereq_flag == 1)
    {
        // isdudata.isdudata.isdu_od_writereq_flag = 0;

        switch (isdudata.index)
        {
            case IOL_Index_ProductName :
                IOL_Make_Resp_ProductName();
                break;
            case IOL_Index_SerialNumber :
                IOL_Make_Resp_SerialNumber();
                break;
            // case IOL_Index_VendorName :
            //     break;
            // case IOL_Index_VendorText :
            //     break;
            // case IOL_Index_ProductText :
            //     break;
            // case IOL_Index_ApplicationSpecificTag :
            //     break;
            // case IOL_Index_FunctionTag :
            //     break;
            // case IOL_Index_LocationTag :
            //     break;
        }
        // device_ProcessDataOut_arr[IOL_OP_ISDU_OUT_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataOut_arr[0], IOL_OP_ISDU_OUT_PROCESSDATALENGTH - 1, 0); // CKS 생성
    }
    #else
    if (isdudata.isdu_od_writereq_flag == 1)
    {
        // isdudata.isdudata.isdu_od_writereq_flag = 0;

        switch (isdudata.index)
        {
            case IOL_Index_ProductName :
                // IOL_Make_Resp_ProductName();
                IOL_Make_Resp_PDOD(IOL_OP_ProductName, IOL_OP_ISDU_PRODUCTNAME_LENGTH);
                break;

            case IOL_Index_SerialNumber :
                DEBUG_GPIO_TOGGLE;
                // IOL_Make_Resp_SerialNumber();
                IOL_Make_Resp_PDOD(IOL_OP_SerialNumber, IOL_OP_ISDU_SERIALNUMBER_LENGTH);
                break;

            // case IOL_Index_VendorName :
            //     break;
            // case IOL_Index_VendorText :
            //     break;
            // case IOL_Index_ProductText :
            //     break;
            case IOL_Index_ProductID :
                // IOL_Make_Resp_ProductID();
                IOL_Make_Resp_PDOD(IOL_OP_ProductID, IOL_OP_ISDU_PRODUCTID_LENGTH);
                break;

            case IOL_Index_ApplicationSpecificTag :
                // IOL_Make_Resp_ApplicationSpecificTag();
                IOL_Make_Resp_PDOD(IOL_OP_ApplicationSpecificTag, IOL_OP_ISDU_APPLICATIONSPECIFICTAG_LENGTH);
                break;

            case IOL_Index_FunctionTag :
                // IOL_Make_Resp_ApplicationSpecificTag();
                IOL_Make_Resp_PDOD(IOL_OP_ApplicationSpecificTag, IOL_OP_ISDU_APPLICATIONSPECIFICTAG_LENGTH);
                break;
                
            case IOL_Index_LocationTag :
                // IOL_Make_Resp_ApplicationSpecificTag();
                IOL_Make_Resp_PDOD(IOL_OP_ApplicationSpecificTag, IOL_OP_ISDU_APPLICATIONSPECIFICTAG_LENGTH);
                break;

            case IOL_Index_PreferredIndex :
                // DEBUG_GPIO_TOGGLE;
                // IOL_Make_Resp_ExampleParameter();
                IOL_Make_Resp_PDOD(IOL_OP_ExampleParameter, IOL_OP_ISDU_EXAMPLEPARAMETER_LENGTH);
                break;

            case IOL_Index_DeviceStatus :
                // IOL_Make_Resp_DeviceStatus();
                IOL_Make_Resp_PDOD(IOL_OP_DeviceStatus, IOL_OP_ISDU_DEVICESTATUS_LENGTH);
                break;

            case IOL_Index_DetailedDeviceStatus :
                // IOL_Make_Resp_DetailedDeviceStatus();
                IOL_Make_Resp_PDOD(IOL_OP_DetailedDeviceStatus, IOL_OP_ISDU_DETAILEDDEVICESTATUS_LENGTH);
                break;
            
            case IOL_Index_VendorName :
                // IOL_Make_Resp_VendorName();
                IOL_Make_Resp_PDOD(IOL_OP_VendorName, IOL_OP_ISDU_VENDORNAME_LENGTH);
                break;

            case IOL_Index_HardwareRevision :
                // IOL_Make_Resp_HWRevision();
                IOL_Make_Resp_PDOD(IOL_OP_HWRevision, IOL_OP_ISDU_HWREVISION_LENGTH);
                break;
            
            case IOL_Index_FirmwareRevision :
                // IOL_Make_Resp_FWRevision();
                IOL_Make_Resp_PDOD(IOL_OP_FWRevision, IOL_OP_ISDU_FWREVISION_LENGTH);
                break;
        }
        // device_ProcessDataOut_arr[IOL_OP_ISDU_OUT_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataOut_arr[0], IOL_OP_ISDU_OUT_PROCESSDATALENGTH - 1, 0); // CKS 생성
    }
    #endif
    else
    {
        IOL_Make_Resp_default();
    }

    IOL_ENABLE;
    
    if (HAL_UART_Transmit_IT(&huart1, device_ProcessDataIn_Arr, IOL_OP_ISDU_IN_PROCESSDATALENGTH) != HAL_OK)
    {
        Error_Handler();
    }

    return ;
}

// ISDU OD Process
uint8_t IOL_State_OP_ISDU_WriteProcess (void)
{
    if (isdudata.isdu_od_rxcplt == 1)
    {
        isdudata.isdu_od_rxcplt = 0;

        isdudata.isdu_od_writereq_flag = 1;
    }
    device_ProcessDataOut_Arr[IOL_OP_ISDU_OUT_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataOut_Arr[0], IOL_OP_ISDU_OUT_PROCESSDATALENGTH - 1, 0); // CKS 생성

    IOL_ENABLE;
    
    if (HAL_UART_Transmit_IT(&huart1, device_ProcessDataOut_Arr, IOL_OP_ISDU_OUT_PROCESSDATALENGTH) != HAL_OK)
    {
        Error_Handler();
    }

    return ;
}




/*******************************************************************************
 * EXTERNAL REFERENCES             NOTE: only use if not available in header file
 *******************************************************************************/
/*---- function prototypes ---------------------------------------------------*/
/*---- data declarations -----------------------------------------------------*/

/*******************************************************************************
 * PUBLIC DECLARATIONS             Defined here, used elsewhere
 *******************************************************************************/
/*---- context ---------------------------------------------------------------*/
/*---- function prototypes ---------------------------------------------------*/
/*---- data declarations -----------------------------------------------------*/

