/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    IOL_ISDU.h
  * @brief   This file contains all the function prototypes for
  *          the IOL_ISDU.c file
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IOL_ISDU_H__
#define __IOL_ISDU_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

typedef struct IOL_ISDUPacket
{
    uint8_t iservice;
    uint8_t length;
    uint8_t index;
    uint8_t subindex;
    uint8_t chkpdu;
} IOL_ISDUPacket_t


/* USER CODE BEGIN Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ IOL_ISDU_H__ */

