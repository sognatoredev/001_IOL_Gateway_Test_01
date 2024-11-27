/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    user_gpio_proc.c
  * @brief   This file provides code for the configuration
  *          of IO-Link Process.
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "IOL_Base.h"
#include "main.h"
#include "user.h"
#include "user_uart_proc.h"


M2DRxPacketParse_t IOL_M2D_rxpacket;

static uint8_t IOL_Checksum_SeedValue = 0x52;
static uint8_t IOL_Page1_SeqValue[13] = {0x49, 0x49, 0x2b, 0x11, 0x83, 0x83, 0xff, 0xff, 0x00, 0x04, 0x5e, 0x00, 0x00};
static uint8_t Page1_seq = 0;

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

static uint8_t Decode_MC_CommunicationChannel (uint8_t Data)
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

static uint8_t Decode_CKS_GetChecksum (const uint8_t * pData, uint8_t length)
{
    uint8_t ck8 = 0x52;
    uint8_t checksumsize = 0;

    checksumsize = length;

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

static uint8_t* IOL_StartUp_Read_MakePacket (uint8_t * pData)
{
    uint8_t i = 0;
    
    pData[0] = IOL_Page1_SeqValue[Page1_seq++];
    pData[1] = Decode_CKS_GetChecksum(&pData[0], 1);
    
    return pData;
}

static uint8_t* IOL_StartUp_Write_MakePacket (uint8_t * pData)
{
    uint8_t i = 0;
    
    pData[0] = Decode_CKS_GetChecksum(&IOL_Checksum_SeedValue, 0);
    
    return pData;
}

static uint8_t IOL_State_StartUp (void)
{
    uint8_t Page_ChecksumValue = 0;
    uint8_t Length = 0;
    uint8_t * pData;

    switch (IOL_M2D_rxpacket.rw)
    {
        case IOL_RW_Read :
            //to do
            Length = 2;
            pData = IOL_StartUp_Read_MakePacket(uart1_rx_IDLE_buf);
            break;
        case IOL_RW_Write :
            // to do
            Length = 1;
            pData = IOL_StartUp_Write_MakePacket(uart1_rx_IDLE_buf);
            break;
    }

    IOL_ENABLE;
    if (HAL_UART_Transmit_IT(&huart1, (uint8_t *) pData, Length) != HAL_OK)
    {
        Error_Handler();
    }

    //Master Command to PreOP.
    if (uart1_rx_IDLE_buf[2] == 0x9A)
    {
        stateIOLseq = IOL_PreOP;
    }

    return ;
}

static uint8_t IOL_State_OPProcess (void)
{
    switch (IOL_M2D_rxpacket.commchannel)
    {
        case IOL_Channel_Process :
            
            
            break;

        case IOL_Channel_Page :
            

            break;

        case IOL_Channel_Diagnosis :
            

            break;

        case IOL_Channel_ISDU :
            

            break;
    }

    return ;
}

// 마스터로부터 받은 데이터를 분석하기 위해 파싱하여 해당 구조체멤버에 넣어줌
uint8_t IOL_Parse_Rx_data (uint8_t * pData)
{
    IOL_M2D_rxpacket.rw = Decode_MC_ReadWrite(*pData);
    IOL_M2D_rxpacket.commchannel = Decode_MC_CommunicationChannel(*pData);
    IOL_M2D_rxpacket.addr = Decode_MC_Address(*pData);

    IOL_M2D_rxpacket.mseqtype = Decode_CKT_Type(*(++pData));
    IOL_M2D_rxpacket.ckt = Decode_CKT_Checksum(*pData);

    return 0;
}

void IOL_Print_ParsingM2Ddata (void)
{
    printf("M2D Rx Data Parsing :  %02X, %02X, %02X, %02X, %02X\r\n", IOL_M2D_rxpacket.rw
                                                                    , IOL_M2D_rxpacket.commchannel
                                                                    , IOL_M2D_rxpacket.addr
                                                                    , IOL_M2D_rxpacket.mseqtype
                                                                    , IOL_M2D_rxpacket.ckt);

    printf("Device State : %d\r\n", stateIOLseq);
}

// #include "user_uart_proc.

// #include "queue.h"
// #include <stdint.h>
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

