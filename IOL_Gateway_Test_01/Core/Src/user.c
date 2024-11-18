/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    user_gpio_proc.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "user.h"

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

uint32_t TIM1_CNT_1 = 0;
uint32_t TIM1_CNT_2 = 0;

uint16_t queDataNum = 0;

uint8_t USB_CdcRxBuffer_FS[CDC_RXDATA_SIZE] = { 0 };
uint8_t USB_CdcTxBuffer_FS[CDC_TXDATA_SIZE] = { 0 };

Q_queue_t USB_TX_Q;

uint8_t queData[quedata_arraylength] = { 0 };
uint8_t USB_TX_Data[quedata_arraylength] = { 0 };

extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

uint32_t uart_rx_IDLE_TotalCnt = 0;
uint16_t iol_processdata_cnt = 0;
uint8_t uart1_rx_IDLE_buf[UART_RX_IDLE_BUFSIZE] = { 0 };

uint8_t TestValue[10] = {0x70, 0x5D, 0x93, 0x15, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00};             //checksum Test.

uint8_t UserButton_Flag = 0;

int _write(int file, char *ptr, int len)
{
    // CDC_Transmit_FS(ptr, len);
    Q_Write(&USB_TX_Q, ptr, len);
    // User_CDC_Transmit_FS(ptr, len);
    return (len);
}

void User_Toggle_Led (void)
{
    if (TIM1_CNT_1 >= 1000)
    {
        TIM1_CNT_1 = 0;

        // HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14);
        LED_ALL_TOGGLE;
    }
}

#if 0
/* Display Boot Message */
void BootMessagePrint (void)
{
    sprintf(USB_CdcTxBuffer_FS, "-------------------------------------------\r\n");
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, " Project Name        : %s\r\n", PROJECT_NAME );
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, " - HW VERSION        : %s\r\n", STR_HW_VER );
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, " - FW VERSION        : %s\r\n", STR_FW_VER );
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, " - BUILD TIME        : %s, %s\r\n", __DATE__, __TIME__ );
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, "-------------------------------------------\r\n" );
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));
}

/* Display Clcok Source Message */
void GetClockSourcePrint (void)
{
    sprintf(USB_CdcTxBuffer_FS, "-------------------------------------------\r\n");
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, " - SYSTEM Clock Frequency        : %lu MHz\r\n", (HAL_RCC_GetSysClockFreq() /1000000));
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, " - HCLK Clock   Frequency        : %lu MHz\r\n", (HAL_RCC_GetHCLKFreq() / 1000000));
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, " - PCLK1 Clock  Frequency        : %lu MHz\r\n", (HAL_RCC_GetPCLK1Freq() / 1000000));
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, " - PCLK2 Clock  Frequency        : %lu MHz\r\n", (HAL_RCC_GetPCLK2Freq() / 1000000));
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, "-------------------------------------------\r\n" );
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));
}
#else
/* Display Boot Message */
void BootMessagePrint (void)
{
    printf("-------------------------------------------------------------------\r\n");
    
    printf(" Project Name        : %s\r\n", PROJECT_NAME );
    
    printf(" - HW VERSION        : %s\r\n", STR_HW_VER );
    
    printf(" - FW VERSION        : %s\r\n", STR_FW_VER );
    
    printf(" - BUILD TIME        : %s, %s\r\n", __DATE__, __TIME__ );
    
    printf("-------------------------------------------------------------------\r\n" );
}

/* Display Clcok Source Message */
void GetClockSourcePrint (void)
{
    printf("-------------------------------------------------------------------\r\n");
    
    printf(" - SYSTEM Clock Frequency        : %lu MHz\r\n", (HAL_RCC_GetSysClockFreq() /1000000));
    
    printf(" - HCLK Clock   Frequency        : %lu MHz\r\n", (HAL_RCC_GetHCLKFreq() / 1000000));
    
    printf(" - PCLK1 Clock  Frequency        : %lu MHz\r\n", (HAL_RCC_GetPCLK1Freq() / 1000000));
    
    printf(" - PCLK2 Clock  Frequency        : %lu MHz\r\n", (HAL_RCC_GetPCLK2Freq() / 1000000));
    
    printf("-------------------------------------------------------------------\r\n" );
}
#endif

void User_CDC_Transmit_FS(uint8_t * pdata, uint16_t datalength)
{
    // while(CDC_Transmit_FS((uint8_t *) pdata, datalength) == USBD_BUSY);
    CDC_Transmit_FS((uint8_t *) pdata, datalength);
}

#if 0 
uint8_t TestValue[10] = {0x70, 0x5D, 0x93, 0x15, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00};             //checksum Test.

uint8_t queData[quedata_arraylength] = { 0 };
uint16_t queDataNum = 0;
uint8_t USB_TX_Data[quedata_arraylength] = { 0 };

/* Timer ch 1 count var. */
uint32_t TIM1_CNT_1 = 0;
uint32_t TIM1_CNT_2 = 0;
uint32_t TIM1_CNT_3 = 0;

uint8_t UserButton_Flag = 0;

#define CDC_RXDATA_SIZE                     256
#define CDC_TXDATA_SIZE                     256


uint8_t USB_CdcRxBuffer_FS[CDC_RXDATA_SIZE] = { 0 };
uint8_t USB_CdcTxBuffer_FS[CDC_TXDATA_SIZE] = { 0 };

uint32_t USB_CdcRxBuffer_FS_cnt = 0;

#define USER_STR_SIZE           1024

uint8_t User_Str[USER_STR_SIZE] = { 0 };

uint8_t uart2_rx_buf[UART_RXDATA_MAX] = { 0 };
uint8_t uart2_tx_buf[UART_TXDATA_MAX] = { 0 };
uint8_t uart3_rx_buf[UART_RXDATA_MAX] = { 0 };
uint8_t uart3_tx_buf[UART_TXDATA_MAX] = { 0 };

uint8_t uart2_rx_stack_buf[UART_RX_IDLE_BUFSIZE * 4] = { 0 };

uint8_t uart2_rx_IDLE_buf[UART_RX_IDLE_BUFSIZE] = { 0 };
// uint8_t uart3_rx_IDLE_buf[UART_RX_IDLE_BUFSIZE] = { 0 };
// uint8_t uart6_rx_IDLE_buf[UART_RX_IDLE_BUFSIZE] = { 0 };
uint8_t uart6_rx_IDLE_buf[500] = { 0 };

uint8_t uart6_rx_dumnmy_buf[128] = { 0 };
uint8_t uart6_rx_cnt = 0;

uint32_t uart_rx_IDLE_TotalCnt = 0;

uint16_t uart2_rx_index = 0;
uint16_t uart2_tx_index = 0;

uint16_t uart3_rx_index = 0;
uint16_t uart3_tx_index = 0;

uint8_t uart2_rx_flag = 0;
uint8_t uart3_rx_flag = 0;

uint32_t uart_rx_cnt_total = 0;
uint16_t uart2_rx_cnt = 0;
uint16_t uart3_rx_cnt = 0;

uint8_t uart2_rx_ready = 0;
uint8_t uart3_rx_ready = 0;

uint8_t uart2_rx_tail_1 = 0;
uint8_t urat3_rx_tail_1 = 0;

uint16_t uart2_rxcpltcallback_cnt = 0;

uint16_t iol_processdata_cnt = 0;

uint8_t debug_uarttest_value = 0x00;

uint32_t IOL_Master_RxEventCallback_Cnt = 0;

uint16_t prv_uart6_size = 0;

/*******************************************************************************
 * PRIVATE DECLARATIONS            Defined here, used elsewhere
 *******************************************************************************/



/*******************************************************************************
 * PUBLIC FUNCTION DEFINITIONS
 *******************************************************************************/
/* Display Boot Message */
void BootMessagePrint (void)
{
    sprintf(USB_CdcTxBuffer_FS, "-------------------------------------------\r\n");
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, " Project Name        : %s\r\n", PROJECT_NAME );
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, " - HW VERSION        : %s\r\n", STR_HW_VER );
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, " - FW VERSION        : %s\r\n", STR_FW_VER );
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, " - BUILD TIME        : %s, %s\r\n", __DATE__, __TIME__ );
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, "-------------------------------------------\r\n" );
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));
}

/* Display Clcok Source Message */
void GetClockSourcePrint (void)
{
    sprintf(USB_CdcTxBuffer_FS, "-------------------------------------------\r\n");
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, " - SYSTEM Clock Frequency        : %lu MHz\r\n", (HAL_RCC_GetSysClockFreq() /1000000));
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, " - HCLK Clock   Frequency        : %lu MHz\r\n", (HAL_RCC_GetHCLKFreq() / 1000000));
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, " - PCLK1 Clock  Frequency        : %lu MHz\r\n", (HAL_RCC_GetPCLK1Freq() / 1000000));
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, " - PCLK2 Clock  Frequency        : %lu MHz\r\n", (HAL_RCC_GetPCLK2Freq() / 1000000));
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));

    sprintf(USB_CdcTxBuffer_FS, "-------------------------------------------\r\n" );
    User_CDC_Transmit_FS((uint8_t *) USB_CdcTxBuffer_FS, strlen(USB_CdcTxBuffer_FS));
}

void User_CDC_Transmit_FS(uint8_t * pdata, uint16_t datalength)
{
    // while(CDC_Transmit_FS((uint8_t *) pdata, datalength) == USBD_BUSY);
    CDC_Transmit_FS((uint8_t *) pdata, datalength);
    
}
#endif
/*******************************************************************************
 * PRIVATE FUNCTION DEFINITIONS
 *******************************************************************************/

