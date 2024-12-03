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
#include "IOL_Page.h"
#include "IOL_Base.h"

static uint8_t IOL_ISDUPage_value[16] = { 0 };
static uint8_t IOL_Page1_SeqValue[13] = {0x49, 0x49, 0x2b, 0x11, 0x83, 0x83, 0xff, 0xff, 0x00, 0x04, 0x5e, 0x00, 0x00};

static uint8_t IOL_OP_OD_Page_Res_cnt = 0;

static uint8_t device_Page_OD_arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH] = { 0 };



// for 문 수정이 필요. j 카운트 부분이 쓸모없이 중복 실행.
void IOL_ConnectToIFM_Read (void)
{
    uint8_t i, j = 0;
    uint8_t setpage1first[2] = {0x00, 0x22};
    // uint8_t Pagevalue[16] = { 0 };

    for (i = 2; i < 16; i++)
    {
        for(j = 0; j < 2; j++)
        {
            IOL_ISDUPage_value[j] = setpage1first[j];
        }
        IOL_ISDUPage_value[i] = IOL_Page1_SeqValue[i - 1];
    }
}

uint8_t IOL_State_OP_Page_ReadProcess (void)
{
    IOL_ConnectToIFM_Read();

    device_Page_OD_arr[0] = IOL_ISDUPage_value[IOL_OP_OD_Page_Res_cnt];
    device_Page_OD_arr[1] = IOL_ISDUPage_value[IOL_OP_OD_Page_Res_cnt + 1];
    IOL_OP_OD_Page_Res_cnt++;

    if (IOL_OP_OD_Page_Res_cnt >= 16)
    {
        IOL_OP_OD_Page_Res_cnt = 0;
    }

    device_Page_OD_arr[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1] = OP_CKS_GetChecksum(&device_Page_OD_arr[0], (IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1), 0);

    IOL_ENABLE;
    
    if (HAL_UART_Transmit_IT(&huart1, device_Page_OD_arr, IOL_OP_ISDU_IN_PROCESSDATALENGTH) != HAL_OK)
    {
        Error_Handler();
    }
}

uint8_t IOL_State_OP_Page_WriteProcess (void)
{
    
}
