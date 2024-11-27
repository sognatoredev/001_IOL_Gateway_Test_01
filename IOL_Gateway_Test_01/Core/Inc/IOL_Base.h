/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    IOL_Base.h
  * @brief   This file contains all the function prototypes for
  *          the IOL_Base.c file
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IOL_BASE_H__
#define __IOL_BASE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "user.h"

typedef struct M2DRxPacketParse 
{
    uint8_t rw;
    uint8_t commchannel;
    uint8_t addr;
    uint8_t mseqtype;
    uint8_t ckt;
} M2DRxPacketParse_t;

extern uint8_t IOL_Parse_Rx_data (uint8_t * pData);
extern void IOL_Print_ParsingM2Ddata (void);
/* USER CODE BEGIN Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ USER_USB_PROC_H__ */

