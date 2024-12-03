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

uint8_t extlength_flag = 0;
uint8_t IOL_OP_ISDU_OD_Res_cnt = 0;


static uint8_t device_ProcessDataIn_arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH]; // + 1   CKS
static uint8_t device_ProcessDataOut_arr[IOL_OP_ISDU_OUT_PROCESSDATALENGTH];

extern UART_HandleTypeDef huart1;

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
    {0xb1, 0x00},
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

    return isdudata.index;
}

// ISDU Write OD 데이터 수신 처리
uint8_t IOL_Get_ISDU_WR_ODArr (uint8_t * pData)
{
    
    isdudata.isdu_od[isdudata.isdu_od_cnt++] = pData[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1];
    isdudata.isdu_od[isdudata.isdu_od_cnt++] = pData[IOL_OP_ISDU_IN_PROCESSDATALENGTH];
    
    // 첫 OD 데이터 수신시 Iservice값과 ISDU의 데이터 길이를 구하기 위함
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

static void IOL_Make_Resp_ProductName (void)
{
    device_ProcessDataIn_arr[0] = IOL_OP_ProductName[IOL_OP_ISDU_OD_Res_cnt][0];   // OD 데이터를  Index에 대한 응답 ISDU 구조의 사이즈에 맞게 나눠서 보내기 위함  
    device_ProcessDataIn_arr[1] = IOL_OP_ProductName[IOL_OP_ISDU_OD_Res_cnt][1];   // 현재 씬그립 디바이스의 IODD는 2 octet.
    IOL_OP_ISDU_OD_Res_cnt++;        // ISDU 요청에 대한 Index 응답의 사이즈에 맞게 나눠서 보내기 위한 카운트값.

    if (IOL_OP_ISDU_OD_Res_cnt >= 13)
    {
        isdudata.isdu_od_writereq_flag = 0;
        IOL_OP_ISDU_OD_Res_cnt = 0;
    }

    device_ProcessDataIn_arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
}

static void IOL_Make_Resp_SerialNumber (void)
{
    device_ProcessDataIn_arr[0] = IOL_OP_ProductName[IOL_OP_ISDU_OD_Res_cnt][0];   // OD 데이터를  Index에 대한 응답 ISDU 구조의 사이즈에 맞게 나눠서 보내기 위함  
    device_ProcessDataIn_arr[1] = IOL_OP_ProductName[IOL_OP_ISDU_OD_Res_cnt][1];   // 현재 씬그립 디바이스의 IODD는 2 octet.
    IOL_OP_ISDU_OD_Res_cnt++;        // ISDU 요청에 대한 Index 응답의 사이즈에 맞게 나눠서 보내기 위한 카운트값.

    if (IOL_OP_ISDU_OD_Res_cnt >= 10)
    {
        isdudata.isdu_od_writereq_flag = 0;
        IOL_OP_ISDU_OD_Res_cnt = 0;
    }

    device_ProcessDataIn_arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
}

static void IOL_Make_Resp_default (void)
{
    // device_ProcessDataIn_arr[OP_ISDU_IN_PROCESSDATALENGTH - 2] = ProcessDataIn_cnt++; // Test cnt Value 
    // device_ProcessDataIn_arr[OP_ISDU_IN_PROCESSDATALENGTH - 2] = ProcessDataIn_cnt; // Test cnt Value 
    device_ProcessDataIn_arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataIn_arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
}

// ISDU OD Process
uint8_t IOL_State_OP_ISDU_ReadProcess (void)
{
    if (isdudata.isdu_od_writereq_flag == 1)
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

        }
        // device_ProcessDataOut_arr[IOL_OP_ISDU_OUT_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataOut_arr[0], IOL_OP_ISDU_OUT_PROCESSDATALENGTH - 1, 0); // CKS 생성
    }
    else
    {
        IOL_Make_Resp_default();
    }

    IOL_ENABLE;
    
    if (HAL_UART_Transmit_IT(&huart1, device_ProcessDataIn_arr, IOL_OP_ISDU_IN_PROCESSDATALENGTH) != HAL_OK)
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
    device_ProcessDataOut_arr[IOL_OP_ISDU_OUT_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_ProcessDataOut_arr[0], IOL_OP_ISDU_OUT_PROCESSDATALENGTH - 1, 0); // CKS 생성

    IOL_ENABLE;
    
    if (HAL_UART_Transmit_IT(&huart1, device_ProcessDataOut_arr, IOL_OP_ISDU_OUT_PROCESSDATALENGTH) != HAL_OK)
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

