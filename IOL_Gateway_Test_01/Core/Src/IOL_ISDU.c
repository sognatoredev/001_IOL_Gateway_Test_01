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

// IOL_ISDUPacket_t IOL_ISDUpacket;
IOL_ISDUPacket_t isdudata;

uint8_t extlength_flag = 0;


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
        isdudata.length = first_od;
    }

    return isdudata.length;
}


static uint8_t IOL_Get_ISDU_ODArr (uint8_t * pData)
{
    if (isdudata.isdu_od_cnt < isdudata.length)
    {
        isdudata.isdu_od[isdudata.isdu_od_cnt++] = pData[IOL_OP_ISDU_IN_PROCESSDATALENGTH - 1];
        isdudata.isdu_od[isdudata.isdu_od_cnt++] = pData[IOL_OP_ISDU_IN_PROCESSDATALENGTH];
    }

    if (isdudata.isdu_od_cnt >= isdudata.length)
    {
        isdudata.isdu_od_cnt = 0; // od length clear
        isdudata.isdu_od_cplt = 1;
    }
    
    return ;
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

static uint8_t IOL_Set_ISDU_Index (void)
{
    switch (isdudata.index)
    {
        case 25:
            break;

        case 26:
            break;
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

