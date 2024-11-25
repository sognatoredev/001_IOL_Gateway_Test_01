/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    user_uart_proc.h
  * @brief   This file contains all the function prototypes for
  *          the user_uart_proc.c file
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_UART_PROC_H__
#define __USER_UART_PROC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "user.h"
#include "usart.h"

/* USER CODE BEGIN Private defines */
#include <stdbool.h>
#include <stdint.h>
/* USER CODE END Private defines */

#define CHECKSUM_SEEDVALUE        0x52
/* USER CODE BEGIN Prototypes */
extern uint16_t mseq_cnt;
extern uint8_t Test_M2D_TempValue;

typedef struct
{
    uint8_t cnt;
    uint8_t MC;
    uint8_t CKT;
    uint8_t CKS;

    uint8_t rw;
    uint8_t ch;
    uint8_t addr;
    uint8_t type;
    uint8_t ckt_cs;
    uint8_t event;
    uint8_t valid;
    uint8_t cks_cs;

    uint8_t Master_octet_cnt;
    uint8_t Device_octet_cnt;

    uint8_t Master_checksum;
    uint8_t Device_checksum;

    uint8_t * Start_Octet;
    uint8_t * End_Octet;
} MSEQ_t;

typedef struct MC_Communication_channel
{
  uint8_t * MC_Com_Ch_0;
  uint8_t * MC_Com_Ch_1;
  uint8_t * MC_Com_Ch_2;
  uint8_t * MC_Com_Ch_3;
} MC_Communication_channel_t;

// MC_Communication_channel_t CommunicationChannel = 
// {
//   .MC_Com_Ch_0 = "Process",
//   .MC_Com_Ch_1 = "Page",
//   .MC_Com_Ch_2 = "Diagnosis",
//   .MC_Com_Ch_3 = "ISDU",
// };

typedef struct Mseq_Checksum
{
  uint8_t * Checksum_Pass;
  uint8_t * Checksum_Error;
} Mseq_Checksum_t;

// Mseq_Checksum_t MseqChecksum = 
// {
//   .Checksum_Pass = "Pass",
//   .Checksum_Error = "Error",
// };

typedef enum
{
  IOL_RW_Write = 0,
  IOL_RW_Read
} IOL_RW_Access;

typedef enum
{
  IOL_StartUp = 0,
  IOL_PreOP,
  IOL_OP
} StateIOLSeq;

typedef enum
{
  IOL_Channel_Process = 0,
  IOL_Channel_Page,
  IOL_Channel_Diagnosis,
  IOL_Channel_ISDU
} IOL_CommChannel;

typedef struct 
{
  uint8_t Page_1_0_MasterCommand;
  uint8_t Page_1_1_MasterCycleTime;
  uint8_t Page_1_2_MinCycleTime;
  uint8_t Page_1_3_MSeqCapability;
  uint8_t Page_1_4_RevisionID;

  uint8_t Page_1_5_ProcessDataIn;
  uint8_t Page_1_6_ProcessDataOut;
  
  uint8_t Page_1_7_VendorID1;
  uint8_t Page_1_8_VendorID2;
  
  uint8_t Page_1_9_DeviceID1;
  uint8_t Page_1_10_DeviceID2;
  uint8_t Page_1_11_DeviceID3;

  uint8_t Page_1_12_FunctionID1;
  uint8_t Page_1_13_FunctionID2;

  uint8_t Page_1_14_Reserved1;
  uint8_t Page_1_15_SystemCommand;
} DirectParameterPage1_t;


#define MAX_RX_DATA                   1000

#define MAX_MSEQ                      100

#define PREOP_DATA_LENGTH             8

#define OP_ISDU_PD_LENGTH             4
#define OP_ISDU_OD_LENGTH             2
#define OP_ISDU_CKS_LENGTH            1
#define OP_ISDU_PRODUCTNAME_LENGTH    23
#define OP_ISDU_SERIALNUMBER_LENGTH   10

#define OP_ISDU_IN_PROCESSDATALENGTH    (OP_ISDU_OD_LENGTH + OP_ISDU_PD_LENGTH + OP_ISDU_CKS_LENGTH)
#define OP_ISDU_OUT_PROCESSDATALENGTH   (OP_ISDU_PD_LENGTH + OP_ISDU_CKS_LENGTH)

// #define MAX_MSEQ                1000
extern  MSEQ_t mseq[MAX_MSEQ];

extern void UART_RX_BufferClear (void);
extern void Mseq_Display_PacketFrame (uint16_t cnt);

extern void Verification_CKTChecksum (void);
extern void mseq_uploaderror_device (void);
extern void IOL_PageTest (uint16_t size);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ USER_USB_PROC_H__ */

