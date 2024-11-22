/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    user_uart_proc.c
  * @brief   This file provides code for the configuration
  *          of all used uart process.
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "user_uart_proc.h"


/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/

static uint8_t IOL_Page1_SeqValue[13] = {0x49, 0x49, 0x2b, 0x11, 0x83, 0x83, 0xff, 0xff, 0x00, 0x04, 0x5e, 0x00, 0x00};
static uint8_t IOL_PreOP_Packet[8][8] = {
                                        {0x81, 0x54, 0xff, 0x91, 0x00, 0x00, 0x00, 0x00}, // R Diag
                                        {0x54, 0xff, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00}, // R Diag
                                        {0xff, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // R Diag
                                        {0x91, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // R Diag
                                        {0xd1, 0x13, 0x46, 0x33, 0x30, 0x36, 0x30, 0x32}, // R ISDU
                                        {0x35, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // R ISDU
                                        {0x00, 0x00, 0xb1, 0x00, 0x00, 0x00, 0x00, 0x00}, // R ISDU
                                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // R ISDU
                                        };

static uint8_t IOL_OP_ProductName[OP_ISDU_OD_LENGTH][OP_ISDU_PRODUCTNAME_LENGTH] = {
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
    {0x31, 0x00}
};

uint8_t IOL_OP_ProcessData_WriteRequest_Flag = 0;

uint8_t ProcessDataIn_cnt = 0;
uint8_t ProcessDataOut_cnt = 0;

uint8_t IOL_RX_CONTINUE_FLAG = 0;

// static uint8_t IOL_Page1_SeqValue[13] = {0x0e, 0x0e, 0x2b, 0x11, 0x8c, 0x88, 0xff, 0xff, 0x00, 0x04, 0x5e, 0x00, 0x00};
static uint8_t IOL_Page1_Packet[2] = {0};
static uint8_t IOL_Checksum_SeedValue = 0x52;

static uint8_t Page1_seq = 0;
static uint8_t PreOP_seq_cnt = 0;

static uint8_t Decode_CKS_EventFlag (uint8_t Data);
static uint8_t Decode_MC_ReadWrite (uint8_t Data);
static uint8_t Print_MC_CommunicationChannel (uint8_t Data);

extern  MSEQ_t mseq[MAX_MSEQ];

uint16_t mseq_cnt;
MSEQ_t mseq[MAX_MSEQ];
StateIOLSeq stateIOLseq;
IOL_CommChannel IOL_commchannel;
IOL_RW_Access IOL_rw_access;

uint8_t ck6 = 0;

bool flag_end;

uint32_t uart2_rx_stackcnt_total = 0;

static uint8_t Decode_CalChecksum(const uint8_t * pData, uint8_t length)  // CKT 에 대한 Checksum 검증
{
    uint8_t ck8 = 0x52;
    uint8_t checksumsize = 0;

    checksumsize = length;

    // ck8 ^= *pData++; // MC Check 

    // ck8 ^= *pData++ & 0xC0; // CKT 6b clear. CKT Check
    // for (uint8_t i = 0; i < checksumsize; i++)
    for (uint8_t i = 0; i < checksumsize; i++)
    {
        if ( i == 1 )
        {
            ck8 ^= *pData++ & 0xC0;
        }
        else
        {
            ck8 ^= *pData++;
        }
    }
    // if (checksumsize < 2)
    // {
    //     return 0;
    // }
    // if (checksumsize == 2)
    // {
    //     ck8 ^= *pData++; // MC Check 

    //     ck8 ^= *pData++ & 0xC0; // CKT 6b clear. CKT Check
        
    // }
    // else if (checksumsize > 2)
    // {
    //     ck8 ^= *pData++; // MC Check 

    //     ck8 ^= *pData++ & 0xC0; // CKT 6b clear. CKT Check

    //     for (uint8_t i = 0; i < length -2; i++)
    //     {
    //         ck8 ^= *pData++;
    //     }
    // }

    // for (uint8_t i = 2; i < (length - 2); i++)
    // {
    //         ck8 ^= *pData++;
    // }

    //Section A.1.6
    uint8_t bit5 = ((ck8 >> 7) & 1U) ^ ((ck8 >> 5) & 1U) ^ ((ck8 >> 3) & 1U) ^ ((ck8 >> 1) & 1U);
    uint8_t bit4 = ((ck8 >> 6) & 1U) ^ ((ck8 >> 4) & 1U) ^ ((ck8 >> 2) & 1U) ^ (ck8 & 1U);
    uint8_t bit3 = ((ck8 >> 7) & 1U) ^ ((ck8 >> 6) & 1U);
    uint8_t bit2 = ((ck8 >> 5) & 1U) ^ ((ck8 >> 4) & 1U);
    uint8_t bit1 = ((ck8 >> 3) & 1U) ^ ((ck8 >> 2) & 1U);
    uint8_t bit0 = ((ck8 >> 1) & 1U) ^ ((ck8 & 1U));
    uint8_t ck6 =   bit5 << 5 |
                    bit4 << 4 |
                    bit3 << 3 |
                    bit2 << 2 |
                    bit1 << 1 |
                    bit0;
    return ck6;
}

static uint8_t Decode_CKT_GetChecksum (const uint8_t * pData, uint8_t length)
{
    uint8_t checksum6 = 0;
    uint8_t master_checksum = 0;
    uint8_t checksum_mctypebit = 0;
    uint8_t calculated_checksum = 0;
    
    checksum6 = Decode_CalChecksum((uint8_t *) pData, length);
    checksum_mctypebit = mseq[mseq_cnt].CKT & 0xC0;
    calculated_checksum = checksum_mctypebit | checksum6;

    return (mseq[mseq_cnt].CKT == calculated_checksum) ? 1 : 0;
    // if (mseq[mseq_cnt].CKT == calculated_checksum)
    // {
    //     return 1;
    // }
    // else
    // {
    //     return 0;
    // }
}

static uint8_t Decode_CKS_GetChecksum (const uint8_t * pData, uint8_t length)
{
    uint8_t ck8 = 0x52;
    uint8_t checksumsize = 0;

    checksumsize = length;

    // ck8 ^= *pData++; // MC Check 

    // ck8 ^= *pData++ & 0xC0; // CKT 6b clear. CKT Check
    // for (uint8_t i = 0; i < checksumsize; i++)
    for (uint8_t i = 0; i < checksumsize; i++)
    {
        ck8 ^= *pData++;
    }
    // if (checksumsize < 2)
    // {
    //     return 0;
    // }
    // if (checksumsize == 2)
    // {
    //     ck8 ^= *pData++; // MC Check 

    //     ck8 ^= *pData++ & 0xC0; // CKT 6b clear. CKT Check
        
    // }
    // else if (checksumsize > 2)
    // {
    //     ck8 ^= *pData++; // MC Check 

    //     ck8 ^= *pData++ & 0xC0; // CKT 6b clear. CKT Check

    //     for (uint8_t i = 0; i < length -2; i++)
    //     {
    //         ck8 ^= *pData++;
    //     }
    // }

    // for (uint8_t i = 2; i < (length - 2); i++)
    // {
    //         ck8 ^= *pData++;
    // }

    //Section A.1.6
    uint8_t bit5 = ((ck8 >> 7) & 1U) ^ ((ck8 >> 5) & 1U) ^ ((ck8 >> 3) & 1U) ^ ((ck8 >> 1) & 1U);
    uint8_t bit4 = ((ck8 >> 6) & 1U) ^ ((ck8 >> 4) & 1U) ^ ((ck8 >> 2) & 1U) ^ (ck8 & 1U);
    uint8_t bit3 = ((ck8 >> 7) & 1U) ^ ((ck8 >> 6) & 1U);
    uint8_t bit2 = ((ck8 >> 5) & 1U) ^ ((ck8 >> 4) & 1U);
    uint8_t bit1 = ((ck8 >> 3) & 1U) ^ ((ck8 >> 2) & 1U);
    uint8_t bit0 = ((ck8 >> 1) & 1U) ^ ((ck8 & 1U));
    uint8_t ck6 =   bit5 << 5 |
                    bit4 << 4 |
                    bit3 << 3 |
                    bit2 << 2 |
                    bit1 << 1 |
                    bit0;
    return ck6;
}

void Verification_CKTChecksum (void)
{
    uint8_t checksum6 = 0;
    uint8_t master_checksum = 0;
    uint8_t checksum_mctypebit = 0;
    uint8_t calculated_checksum = 0;
    uint8_t ChecksumTorF = 0;
    
    checksum6 = Decode_CalChecksum(TestValue, 10);
    checksum_mctypebit = TestValue[1] & 0xC0;
    calculated_checksum = checksum_mctypebit | checksum6;

    if (TestValue[1] == calculated_checksum)
    {
        ChecksumTorF = 1;
    }
    else
    {
        ChecksumTorF = 0;
    }

    // printf(" Checksum 6bit Value : 0x%02X\r\n", checksum6);
    // printf(" Checksum calculated_checksum Value : 0x%02X\r\n", calculated_checksum);
    // printf(" Checksum Pass : %d\r\n", ChecksumTorF);
}

// PreOP Mode R/W 체크
static uint8_t IOL_PreOP_ReadWriteCheck (void)
{
    // R/W   Read 체크
    if (Decode_MC_ReadWrite(uart1_rx_IDLE_buf[0]) == IOL_RW_Read)
    {
        return IOL_RW_Read;
    }
    // R/W   Write 체크
    else if (Decode_MC_ReadWrite(uart1_rx_IDLE_buf[0]) == IOL_RW_Write)
    {
        return IOL_RW_Write;
    }
}

//preOP 모드 Event 비트 포함 CKS 생성
static uint8_t PreOP_CKS_GetChecksum (const uint8_t * pData, uint8_t length, uint8_t eventflag)
{
    uint8_t ck8 = 0x52;
    uint8_t checksumsize = 0;
    uint8_t eventbitset = 0x80;

    checksumsize = length;

    if (eventflag == 0)
    {
        for (uint8_t i = 0; i < checksumsize; i++)
        {
            ck8 ^= *pData++;
        }
        //Section A.1.6
        uint8_t bit5 = ((ck8 >> 7) & 1U) ^ ((ck8 >> 5) & 1U) ^ ((ck8 >> 3) & 1U) ^ ((ck8 >> 1) & 1U);
        uint8_t bit4 = ((ck8 >> 6) & 1U) ^ ((ck8 >> 4) & 1U) ^ ((ck8 >> 2) & 1U) ^ (ck8 & 1U);
        uint8_t bit3 = ((ck8 >> 7) & 1U) ^ ((ck8 >> 6) & 1U);
        uint8_t bit2 = ((ck8 >> 5) & 1U) ^ ((ck8 >> 4) & 1U);
        uint8_t bit1 = ((ck8 >> 3) & 1U) ^ ((ck8 >> 2) & 1U);
        uint8_t bit0 = ((ck8 >> 1) & 1U) ^ ((ck8 & 1U));
        uint8_t ck6 =   bit5 << 5 |
                        bit4 << 4 |
                        bit3 << 3 |
                        bit2 << 2 |
                        bit1 << 1 |
                        bit0;
        
        return ck6;
    }
    else if (eventflag == 1)
    {
        for (uint8_t i = 0; i < checksumsize; i++)
        {
            ck8 ^= *pData++;
        }

        ck8 ^= eventbitset;

        //Section A.1.6
        uint8_t bit5 = ((ck8 >> 7) & 1U) ^ ((ck8 >> 5) & 1U) ^ ((ck8 >> 3) & 1U) ^ ((ck8 >> 1) & 1U);
        uint8_t bit4 = ((ck8 >> 6) & 1U) ^ ((ck8 >> 4) & 1U) ^ ((ck8 >> 2) & 1U) ^ (ck8 & 1U);
        uint8_t bit3 = ((ck8 >> 7) & 1U) ^ ((ck8 >> 6) & 1U);
        uint8_t bit2 = ((ck8 >> 5) & 1U) ^ ((ck8 >> 4) & 1U);
        uint8_t bit1 = ((ck8 >> 3) & 1U) ^ ((ck8 >> 2) & 1U);
        uint8_t bit0 = ((ck8 >> 1) & 1U) ^ ((ck8 & 1U));
        uint8_t ck6 =   bit5 << 5 |
                        bit4 << 4 |
                        bit3 << 3 |
                        bit2 << 2 |
                        bit1 << 1 |
                        bit0;
        
        return eventbitset | ck6;
    }
}

// I-Service 분석
uint8_t IOL_ISDU_GetIservice (uint8_t * pdata)
{
    uint8_t iservice = 0;

    iservice = *pdata >> 4;

    switch (iservice)
    {
    case 0x09:
    case 0x0A:
    case 0x0B:

        break;
    
    default:
        break;
    }
    return ;
}

uint8_t IOL_ISDU_GetLength (uint8_t * pData)
{
    return ;
}

uint8_t IOL_ISDU_GetIndex (uint8_t * pData)
{
    return;
}

uint8_t IOL_ISDU_GetCHKPDU (uint8_t * pData)
{
    return ;
}

// 요청받은 OD 데이터 분석
uint8_t IOL_Receive_Request_OD (uint8_t *pData)
{
    uint8_t OD_IService = 0;
    uint8_t OD_length = 0;
    uint8_t OD_index = 0;
    uint8_t OD_CHKPDU = 0;
    
    return ;
}

// 응답할 OD 데이터패킷 생성
uint8_t IOL_MakePacket_Response_OD (uint8_t * pData)
{
    return ;
}

// if (stateIOLseq == IOL_OP)
void IOL_State_OP (void)
{
    uint8_t i, j = 0;
    uint8_t IOL_Commchannel_value = 0;
    
    static uint8_t device_ProcessDataIn_arr[OP_ISDU_IN_PROCESSDATALENGTH]; // + 1   CKS
    static uint8_t device_ProcessDataOut_arr[OP_ISDU_OUT_PROCESSDATALENGTH];

    // static uint8_t preop_data_arr[PREOP_DATA_LENGTH + 1] = {0}; // + 1   CKS 
    // uint8_t Page_Write_ChecksumValue[0] = {0};

    IOL_Commchannel_value = Print_MC_CommunicationChannel(uart1_rx_IDLE_buf[0]);
    // uint8_t Page_Write_ChecksumValue[1] = {0};

    if (IOL_PreOP_ReadWriteCheck() == IOL_RW_Read)
    {
        if (IOL_Commchannel_value == IOL_Channel_ISDU)
        {
            device_ProcessDataIn_arr[OP_ISDU_IN_PROCESSDATALENGTH - 2] = ProcessDataIn_cnt++; // Test cnt Value 
            device_ProcessDataIn_arr[OP_ISDU_IN_PROCESSDATALENGTH - 1] = PreOP_CKS_GetChecksum(&device_ProcessDataIn_arr[0], (OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);
        }

        IOL_ENABLE;
        if (HAL_UART_Transmit_IT(&huart1, device_ProcessDataIn_arr, OP_ISDU_IN_PROCESSDATALENGTH) != HAL_OK)
        {
            Error_Handler();
        }
    }
    else if (IOL_PreOP_ReadWriteCheck() == IOL_RW_Write)
    {
        if (IOL_Commchannel_value == IOL_Channel_ISDU)
        {
            device_ProcessDataOut_arr[OP_ISDU_OUT_PROCESSDATALENGTH - 1] = PreOP_CKS_GetChecksum(&device_ProcessDataOut_arr[0], OP_ISDU_OUT_PROCESSDATALENGTH - 1, 0);
        }

        IOL_ENABLE;
        if (HAL_UART_Transmit_IT(&huart1, device_ProcessDataOut_arr, OP_ISDU_OUT_PROCESSDATALENGTH) != HAL_OK)
        {
            Error_Handler();
        }
    }
}

// if (stateIOLseq == IOL_PreOP)
void IOL_State_PreOP (void)
{
    uint8_t i, j = 0;
    uint8_t IOL_Commchannel_value = 0;
    static uint8_t preop_data_arr[PREOP_DATA_LENGTH + 1] = {0}; // + 1   CKS 
    // uint8_t Page_Write_ChecksumValue[0] = {0};

    IOL_Commchannel_value = Print_MC_CommunicationChannel(uart1_rx_IDLE_buf[0]);
    // uint8_t Page_Write_ChecksumValue[1] = {0};

    if (IOL_PreOP_ReadWriteCheck() == IOL_RW_Read)
    {
        for (i=0; i < PREOP_DATA_LENGTH; i++)
        {
            preop_data_arr[i]= IOL_PreOP_Packet[PreOP_seq_cnt][i];
        }

        // PreOP Mode에서 Diagnosis 는 Event flag 있음 PreOP_CKS_GetChecksum 함수의 3번째 인자 1 = event set, 0 = event reset.
        if (IOL_Commchannel_value == IOL_Channel_Diagnosis)
        {
            preop_data_arr[i] = PreOP_CKS_GetChecksum(&preop_data_arr[0], PREOP_DATA_LENGTH, 1);
        }
        else if (IOL_Commchannel_value == IOL_Channel_ISDU)
        {
            preop_data_arr[i] = PreOP_CKS_GetChecksum(&preop_data_arr[0], PREOP_DATA_LENGTH, 0);
        }
        else
        {
            preop_data_arr[i] = PreOP_CKS_GetChecksum(&preop_data_arr[0], PREOP_DATA_LENGTH, 0);
        }
        
        PreOP_seq_cnt++;

        IOL_ENABLE;
        if (HAL_UART_Transmit_IT(&huart1, preop_data_arr, 9) != HAL_OK)
        {
            Error_Handler();
        }
    }
    else if (IOL_PreOP_ReadWriteCheck() == IOL_RW_Write)
    {
        // PreOP Mode에서 Diagnosis 는 Event flag 있고, ISDU Write 경우 Event 플래그 있음 
        if ((IOL_Commchannel_value == IOL_Channel_Diagnosis) || (IOL_Commchannel_value == IOL_Channel_ISDU))
        {
            preop_data_arr[0] = PreOP_CKS_GetChecksum(&preop_data_arr[0], 0, 1);
        }
        else
        {
            preop_data_arr[0] = PreOP_CKS_GetChecksum(&preop_data_arr[0], 0, 0);
            
            //Master Command to OP.
            if(uart1_rx_IDLE_buf[2] == 0x99)
            {
                stateIOLseq = IOL_OP;
            }
        }

        IOL_ENABLE;
        if (HAL_UART_Transmit_IT(&huart1, (uint8_t *) preop_data_arr, 1) != HAL_OK)
        {
            Error_Handler();
        }
    }
}

void IOL_State_StartUp (void)
{
    uint8_t Page_Write_ChecksumValue = 0;
    // R/W   Read 체크
    if (Decode_MC_ReadWrite(uart1_rx_IDLE_buf[0]) == IOL_RW_Read)
    {
        
        IOL_Page1_Packet[0] = IOL_Page1_SeqValue[Page1_seq++];

        // IOL_Page1_Packet[1] = Decode_CKS_GetChecksum(IOL_Page1_Packet[0], 1);
        IOL_Page1_Packet[1] = Decode_CKS_GetChecksum(&IOL_Page1_Packet[0], 1);

        IOL_ENABLE;
        if (HAL_UART_Transmit_IT(&huart1, (uint8_t *) IOL_Page1_Packet, 2) != HAL_OK)
        {
            Error_Handler();
        }
        // HAL_UART_Transmit(&huart1,(uint8_t *) IOL_Page1_Packet, 2, 10);
        // IOL_DISABLE;
    }
    // R/W   Write 체크
    else if (Decode_MC_ReadWrite(uart1_rx_IDLE_buf[0]) == IOL_RW_Write)
    {
        Page_Write_ChecksumValue = Decode_CKS_GetChecksum(&IOL_Checksum_SeedValue, 0);
        // if (HAL_UART_Transmit_IT(&huart1, (uint8_t *) Decode_CKS_GetChecksum(0x52, 0), 1) != HAL_OK)
        IOL_ENABLE;
        if (HAL_UART_Transmit_IT(&huart1, (uint8_t *) Page_Write_ChecksumValue, 1) != HAL_OK)
        {
            Error_Handler();
        }

        //Master Command to PreOP.
        if (uart1_rx_IDLE_buf[2] == 0x9A)
        {
            stateIOLseq = IOL_PreOP;
        }
        
    }
}

void IOL_StartUp_Seq_Page (uint16_t size)
{
    uint8_t rxdataSize = 0;
    uint8_t eventdebug = 0;
    uint8_t Page_Write_ChecksumValue[0] = {0};
    
    rxdataSize = (uint8_t)size;

    // State = StartUP ,   Channel = Page인지 구분
    if ((stateIOLseq == IOL_StartUp) && (Print_MC_CommunicationChannel(uart1_rx_IDLE_buf[0]) == IOL_Channel_Page))
    {
        // R/W   Read 체크
        if (Decode_MC_ReadWrite(uart1_rx_IDLE_buf[0]) == IOL_RW_Read)
        {
            
            IOL_Page1_Packet[0] = IOL_Page1_SeqValue[Page1_seq++];

            // IOL_Page1_Packet[1] = Decode_CKS_GetChecksum(IOL_Page1_Packet[0], 1);
            IOL_Page1_Packet[1] = Decode_CKS_GetChecksum(&IOL_Page1_Packet[0], 1);

            IOL_ENABLE;
            if (HAL_UART_Transmit_IT(&huart1, (uint8_t *) IOL_Page1_Packet, 2) != HAL_OK)
            {
                Error_Handler();
            }
            // HAL_UART_Transmit(&huart1,(uint8_t *) IOL_Page1_Packet, 2, 10);
            // IOL_DISABLE;
        }
        // R/W   Write 체크
        else if (Decode_MC_ReadWrite(uart1_rx_IDLE_buf[0]) == IOL_RW_Write)
        {
            Page_Write_ChecksumValue[0] = Decode_CKS_GetChecksum(&IOL_Checksum_SeedValue, 0);
            // if (HAL_UART_Transmit_IT(&huart1, (uint8_t *) Decode_CKS_GetChecksum(0x52, 0), 1) != HAL_OK)
            IOL_ENABLE;
            if (HAL_UART_Transmit_IT(&huart1, (uint8_t *) Page_Write_ChecksumValue, 1) != HAL_OK)
            {
                Error_Handler();
            }

            //Master Command to PreOP.
            if (uart1_rx_IDLE_buf[2] == 0x9A)
            {
                stateIOLseq = IOL_PreOP;
            }
            
        }
    }
    // PreOperate Mode
    else if (stateIOLseq == IOL_PreOP)
    {
        IOL_State_PreOP();
    }
    // Operate Mode
    else if (stateIOLseq == IOL_OP)
    {
        IOL_State_OP();
    }
}

#if 1
void IOL_PageTest (uint16_t size)
{
    uint8_t rxdataSize = 0;

    uint8_t i = 0;

    rxdataSize = (uint8_t)size;

    IOL_ENABLE;
    // IOL_Page1_Packet[0] = 0x49;
    IOL_Page1_Packet[0] = IOL_Page1_SeqValue[Page1_seq++];;
    
    IOL_Page1_Packet[1] = Decode_CKS_GetChecksum(&IOL_Page1_Packet[0], 1);

    if (HAL_UART_Transmit_IT(&huart1, (uint8_t *) IOL_Page1_Packet, 2) != HAL_OK)
    {
        Error_Handler();
    }
    // HAL_UART_Transmit(&huart1,(uint8_t *) IOL_Page1_Packet, 2, 10);
    // IOL_DISABLE;
}
#else
void IOL_PageTest (uint16_t size)
{
    uint8_t rxdataSize = 0;

    uint8_t i = 0;

    rxdataSize = (uint8_t)size;

    IOL_ENABLE;
    // IOL_Page1_Packet[0] = 0x49;
    IOL_Page1_Packet[0] = IOL_Page1_SeqValue[Page1_seq++];;
    
    IOL_Page1_Packet[1] = Decode_CKS_GetChecksum(&IOL_Page1_Packet[0], 1);

    if (HAL_UART_Transmit_IT(&huart1, (uint8_t *) IOL_Page1_Packet, 2) != HAL_OK)
    {
        Error_Handler();
    }
    // HAL_UART_Transmit(&huart1,(uint8_t *) IOL_Page1_Packet, 2, 10);
    // IOL_DISABLE;
}
#endif

#if 0
void mseq_upload_device (uint16_t size)
{
    uint16_t rxdataSize = 0;
    uint16_t cks_offset = 1;

    rxdataSize = size;
    mseq[mseq_cnt + cks_offset].Device_octet_cnt = rxdataSize;
    // mseq[mseq_cnt].CKS = uart3_rx_IDLE_buf[rxdataSize - 1];
    
    mseq[(mseq_cnt + cks_offset)].CKS = uart6_rx_IDLE_buf[rxdataSize - 1];
    // Decode_GetChecksum(uart6_rx_IDLE_buf[0], rxdataSize);

    mseq_cnt++;
}
#else
void mseq_upload_device (uint16_t size)
{
    // uint16_t rxdataSize = 0;
    uint8_t rxdataSize = 0;
    uint16_t cks_offset = 1;

    // rxdataSize = (uint8_t)size;

    // HAL_GPIO_TogglePin(UART_DEBUG_PORT, DEBUG_TEST_PIN);
    mseq[mseq_cnt].Device_octet_cnt = (uint8_t)size;
    // mseq[mseq_cnt - cks_offset].Device_octet_cnt = (uint8_t)size;
    // HAL_GPIO_TogglePin(UART_DEBUG_PORT, DEBUG_TEST_PIN);

    // mseq[mseq_cnt].Device_octet_cnt = rxdataSize;
    // mseq[mseq_cnt].CKS = uart6_rx_IDLE_buf[rxdataSize - 1];

    // mseq[mseq_cnt + 1].Device_octet_cnt = rxdataSize;
    
    // mseq[mseq_cnt].CKS = uart3_rx_IDLE_buf[rxdataSize - 1];
    
    // mseq[(mseq_cnt + cks_offset)].CKS = uart6_rx_IDLE_buf[rxdataSize - 1];
    
    // Decode_GetChecksum(uart6_rx_IDLE_buf[0], rxdataSize);

    // mseq_cnt++;
}
#endif

void mseq_uploaderror_device (void)
{
    // mseq[mseq_cnt].Device_octet_cnt = UART_RX_IDLE_BUFSIZE - hdma_usart6_rx.Instance->NDTR;
}

void mseq_display (void)
{
    uint16_t i = 0;
    uint8_t OffsetValue = 0;

    for (i = 0; i < mseq_cnt; ++i)
    {
        // printf("%d,%d,%d,%d\r\n", i, mseq[i].MC, mseq[i].CKT, mseq[i].CKS);
        printf("%d,%d,%d,%d,", i, mseq[i+OffsetValue].MC, mseq[i+OffsetValue].CKT, mseq[i+OffsetValue].CKS);
        Mseq_Display_PacketFrame(i);
        // HAL_Delay(1);
    }
    // printf("ProcessData cnt : %d\r\n", iol_processdata_cnt);

    iol_processdata_cnt = 0;
}


void debug_buf_read (void)
{
  if (uart_rx_IDLE_TotalCnt >= MAX_RX_DATA)
  {
    uart_rx_IDLE_TotalCnt = 2;
    // uart_rx_IDLE_TotalCnt = 0;

    #if 0
    mseq_display();
    // Mseq_Display_PacketFrame();
    
    HAL_UART_DMAStop(&huart2);
    // HAL_UART_DMAStop(&huart6);
    HAL_UART_Abort(&huart6);
    HAL_NVIC_DisableIRQ(USART6_IRQn);
    #else
    // HAL_UART_DMAStop(&huart2);
    // // HAL_UART_DMAStop(&huart6);
    // HAL_UART_Abort(&huart6);
    // HAL_NVIC_DisableIRQ(USART6_IRQn);

    mseq_display();
    // Mseq_Display_PacketFrame();
    #endif
  }
}

static uint8_t Decode_MC_ReadWrite (uint8_t Data)
{
    uint8_t MCdata = 0;

    MCdata = Data;
    MCdata = (MCdata >> 7) & 0x01;

    switch (MCdata)
    {
        case 0 :
            return IOL_RW_Write;
        case 1 :
            return IOL_RW_Read;
        default :
            // printf("Unexpected value\n");
            return 'X';
    }
}

// static uint8_t Decode_MC_CommunicationChannel (uint8_t pData)
// {
//     uint8_t MCdata = 0;

//     MCdata = pData;
//     MCdata = MCdata >> 5;

//     return MCdata & 0x03;
// }

static uint8_t Print_MC_CommunicationChannel (uint8_t Data)
{
    uint8_t MCdata = 0;

    MCdata = Data;
    MCdata = (MCdata >> 5) & 0x03;
    
    switch (MCdata)
    {
        case 0 :
            // printf("%s,", CommunicationChannel.MC_Com_Ch_0);
            return IOL_Channel_Process;
        case 1 :
            // printf("%s,", CommunicationChannel.MC_Com_Ch_1);
            return IOL_Channel_Page;
        case 2 :
            // printf("%s,", CommunicationChannel.MC_Com_Ch_2);
            return IOL_Channel_Diagnosis;
        case 3 :
            // printf("%s,", CommunicationChannel.MC_Com_Ch_3);
            return IOL_Channel_ISDU;
    }
}

static uint8_t Decode_MC_Address (uint8_t Data)
{
    uint8_t MCdata = 0;

    MCdata = Data;
    
    return MCdata & 0x1F;
}

static uint8_t Decode_CKT_Type (uint8_t Data)
{
    uint8_t MCdata = 0;
    
    MCdata = Data;

    return (MCdata >> 6) & 0x03;
}

static uint8_t Decode_CKT_Checksum (uint8_t Data)
{
    uint8_t MCdata = 0;

    MCdata = Data;

    return MCdata & 0x3F;
}

static uint8_t Decode_CKS_EventFlag (uint8_t Data)
{
    uint8_t MCdata = 0;

    MCdata = Data;

    #if 0
    return (MCdata >> 7) & 0x01;
    #else
    MCdata = (MCdata >> 7) & 0x01;

    switch (MCdata)
    {
        case 0 :
            return 'N';
        case 1 :
            return 'E';
        // default :
            // printf("Unexpected value\n");
            // return 'X';
    }
    #endif
}

static uint8_t Decode_CKS_PDStatus (uint8_t Data)
{
    uint8_t MCdata = 0;

    MCdata = Data;

    #if 0
    return (MCdata >> 6) & 0x01;
    #else
    MCdata = (MCdata >> 6) & 0x01;

    switch (MCdata)
    {
        case 0 :
            return 'V';
        case 1 :
            return 'N';
        default :
            printf("Unexpected value\n");
            return 'X';
    }
    #endif
}

static uint8_t Decode_CKS_Checksum (uint8_t Data)
{
    uint8_t MCdata = 0;

    MCdata = Data;

    return MCdata & 0x3F;
}

static uint8_t Decode_CKT_AllDataChecksum (uint8_t Data)
{
    uint8_t MCdata = 0;

    MCdata = Data;
    MCdata = (MCdata >> 5) & 0x03;
    
    switch (MCdata)
    {
        case 0 :
            printf("%s,", MseqChecksum.Checksum_Pass);
            break;
        case 1 :
            printf("%s,", MseqChecksum.Checksum_Error);
            break;
    }
}

static uint8_t Decode_CKS_AllDataChecksum (uint8_t Data)
{
    uint8_t MCdata = 0;

    MCdata = Data;
    MCdata = (MCdata >> 5) & 0x03;
    
    switch (MCdata)
    {
        case 0 :
            printf("%s,", MseqChecksum.Checksum_Pass);
            break;
        case 1 :
            printf("%s,", MseqChecksum.Checksum_Error);
            break;
    }
}



void Mseq_Display_PacketFrame (uint16_t cnt)
{
    uint16_t i, j = 0;
    uint8_t OffsetValue = 0;
    cnt += OffsetValue;

    printf("%c,",Decode_MC_ReadWrite(mseq[cnt].MC));
    Print_MC_CommunicationChannel(mseq[cnt].MC);
    printf("%02X,%d,%02X,%c,%c,%02X,", 
                                        Decode_MC_Address(mseq[cnt].MC),
                                        Decode_CKT_Type(mseq[cnt].CKT),
                                        Decode_CKT_Checksum(mseq[cnt].CKT),
                                        Decode_CKS_EventFlag(mseq[cnt].CKS),
                                        Decode_CKS_PDStatus(mseq[cnt].CKS),
                                        Decode_CKS_Checksum(mseq[cnt].CKS)
                                        );
    #if 0
    printf("%d,%d,%02X,%02X,%02X,%02X\r\n", 
                                        mseq[i].Master_octet_cnt,
                                        mseq[i].Device_octet_cnt,
                                        mseq[i].Master_checksum,
                                        mseq[i].Device_checksum,
                                        mseq[i].Start_Octet,
                                        mseq[i].End_Octet
                                        );
    #else
    printf("%d,%d,%02X,%02X,%02X,%02X,", 
                                        mseq[cnt].Master_octet_cnt,
                                        mseq[cnt].Device_octet_cnt,
                                        mseq[cnt].Master_checksum,
                                        mseq[cnt].Device_checksum,
                                        mseq[cnt].Start_Octet,
                                        mseq[cnt].End_Octet
                                        );
    
    // for (i = 0; i < (mseq[cnt].Master_octet_cnt - mseq[cnt].Device_octet_cnt); i++)
    for (i = 0; i < mseq[cnt].Master_octet_cnt; i++)
    {
        printf("%02X,", *(mseq[cnt].Start_Octet + i));
    }
    printf("\r\n");   
    #endif

}

static uint8_t UserButton_BufferClear (void)
{
    uint8_t buttonstatus = 0;

    if (TIM1_CNT_2 >= 500)
    {
        TIM1_CNT_2 = 0;

        if (UserButton_Flag == 1)
        {
            buttonstatus = UserButton_Flag;

            UserButton_Flag = 0;

            // memset(uart2_rx_IDLE_buf, 0, UART_RX_IDLE_BUFSIZE);
            // memset(uart6_rx_IDLE_buf, 0, UART_RX_IDLE_BUFSIZE);
            // memset(uart6_rx_IDLE_buf, 0, UART_RX_IDLE_BUFSIZE * 4);
            // memset(mseq, 0, 1000);
        }
    }
    return !buttonstatus;
}

#if 0
void UART_RX_BufferClear (void)
{
    if(UserButton_BufferClear() == HAL_OK)
    {
        printf("\r\nUART RX Buffer Clear.\r\n\n");
        
        memset(uart2_rx_IDLE_buf, 0, UART_RX_IDLE_BUFSIZE);
        memset(uart6_rx_IDLE_buf, 0, UART_RX_IDLE_BUFSIZE);
        memset(uart2_rx_stack_buf, 0, UART_RX_IDLE_BUFSIZE * 4);

        uart2_rx_stackcnt_total = 0;
        uart_rx_IDLE_TotalCnt = 2;
        // uart_rx_IDLE_TotalCnt = 0;
        mseq_cnt = 0;

        HAL_UART_DMAResume(&huart2);
        // HAL_UART_DMAResume(&huart6);
        
        __HAL_DMA_DISABLE(&hdma_usart2_rx);
        hdma_usart2_rx.Instance->NDTR = UART_RX_IDLE_BUFSIZE;
        __HAL_DMA_ENABLE(&hdma_usart2_rx);

        // __HAL_DMA_DISABLE(&hdma_usart6_rx);
        // hdma_usart6_rx.Instance->NDTR = UART_RX_IDLE_BUFSIZE;
        // __HAL_DMA_ENABLE(&hdma_usart6_rx);

        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *) uart2_rx_IDLE_buf, UART_RX_IDLE_BUFSIZE);
        __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

        HAL_NVIC_EnableIRQ(USART6_IRQn);
        HAL_UART_Receive_IT(&huart6, &uart6_rx_dumnmy_buf, 1);
        // HAL_UARTEx_ReceiveToIdle_DMA(&huart6, (uint8_t *)uart6_rx_IDLE_buf, UART_RX_IDLE_BUFSIZE);
        // // HAL_UART_Receive_DMA(&huart6, (uint8_t *)uart6_rx_IDLE_buf, UART_RX_IDLE_BUFSIZE);
        // __HAL_DMA_DISABLE_IT(&hdma_usart6_rx, DMA_IT_HT);
        // // __HAL_DMA_DISABLE_IT(&hdma_usart6_rx, DMA_IT_TC);
        // HAL_UART_Receive_DMA(&huart2, uart2_rx_buf, UART_RXDATA_MAX);
        // HAL_UART_Receive_DMA(&huart3, uart3_rx_buf, UART_RXDATA_MAX);
    }
}
#endif

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
