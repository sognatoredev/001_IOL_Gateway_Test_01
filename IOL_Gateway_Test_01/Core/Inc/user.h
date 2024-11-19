/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    user.h
  * @brief   This file contains all the function prototypes
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_H__
#define __USER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "queue.h"
#include "usb_device.h"

// #include "usbd_def.h"
// #include "queue.h"

// #include "user_uart_proc.h"
// #include "debug_buf.h"
/* USER CODE END Includes */
#ifndef DEBUG
#define DEBUG
#endif
#ifdef DEBUG
#define CONCAT_3(p1, p2, p3)  CONCAT_3_(p1, p2, p3)
/** Auxiliary macro used by @ref CONCAT_3 */
#define CONCAT_3_(p1, p2, p3) p1##p2##p3

#define STRINGIFY_(val) #val
/** Converts a macro argument into a character constant.*/
#define STRINGIFY(val)  STRINGIFY_(val)

/* FW and Board Information */
#define PROJECT_NAME                "IO-Link TEST 01"

/* board revision */
#define BD_VER_MAJOR                0
#define BD_VER_MINOR                1
/* firmware version */
#define FW_VER_MAJOR                0
#define FW_VER_MINOR                1

/* board revision */
#define HW_BD_REV_01    ( ( 0 << 4 ) | ( 1 ) )
#define STR_BD_VER      "v" STRINGIFY( BD_VER_MAJOR ) "." STRINGIFY( BD_VER_MINOR )
#define DEF_BD_VER      CONCAT_3( HW_BD_REV_01, BD_VER_MAJOR, BD_VER_MINOR )
/* firmware version */
#define STR_FW_VER                      "v" STRINGIFY( FW_VER_MAJOR ) "." STRINGIFY( FW_VER_MINOR ) "." STRINGIFY( FW_VER_PATCH )
#define STR_FW_VER_LEN                  6
#define STR_HW_VER                      "v" STRINGIFY( BD_VER_MAJOR ) "." STRINGIFY( BD_VER_MINOR )
#endif

extern uint32_t TIM1_CNT_1;
extern uint32_t TIM1_CNT_2;

#define LED_RED_PORT            GPIOC
#define LED_GREEN_PORT          GPIOC
#define LED_ALL_PORT            GPIOC

#define LED_RED_PIN             GPIO_PIN_14
#define LED_GREEN_PIN           GPIO_PIN_15
#define LED_ALL_PIN             (GPIO_PIN_14 | GPIO_PIN_15)

#define LED_RED_ON              HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET)
#define LED_GREEN_ON            HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET)
#define LED_ALL_ON              HAL_GPIO_WritePin(LED_ALL_PORT, LED_ALL_PIN, GPIO_PIN_SET)

#define LED_RED_OFF             HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET)
#define LED_GREEN_OFF           HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET)
#define LED_ALL_OFF             HAL_GPIO_WritePin(LED_ALL_PORT, LED_ALL_PIN, GPIO_PIN_RESET)

#define LED_ALL_TOGGLE          HAL_GPIO_TogglePin(LED_ALL_PORT, LED_ALL_PIN)

extern void User_Toggle_Led (void);

#define IOL_ENABLE                  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET)
#define IOL_DISABLE                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)

#define DEBUG_GPIO_TOGGLE           HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0)
#define DEBUG_GPIO_ON               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)
#define DEBUG_GPIO_OFF              HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_REET)


#define CDC_RXDATA_SIZE                     256
#define CDC_TXDATA_SIZE                     256

extern void BootMessagePrint (void);
extern void GetClockSourcePrint (void);

#define quedata_arraylength           1000

extern uint8_t queData[];
extern uint8_t USB_TX_Data[];
extern uint16_t queDataNum;

extern Q_queue_t USB_TX_Q;

#define UART_RX_IDLE_BUFSIZE        100

extern DMA_HandleTypeDef hdma_usart1_rx;
extern uint32_t uart_rx_IDLE_TotalCnt;
extern uint16_t iol_processdata_cnt;
extern uint8_t uart1_rx_IDLE_buf[];
extern uint8_t TestValue[];

extern uint8_t UserButton_Flag;
#if 0 
#define quedata_arraylength       5000

extern uint8_t queData[];
extern uint16_t queDataNum;
extern uint8_t USB_TX_Data[];

extern uint8_t TestValue[];

/* USER CODE BEGIN Private defines */

#ifndef DEBUG
#define DEBUG
#endif
#ifdef DEBUG
#define CONCAT_3(p1, p2, p3)  CONCAT_3_(p1, p2, p3)
/** Auxiliary macro used by @ref CONCAT_3 */
#define CONCAT_3_(p1, p2, p3) p1##p2##p3

#define STRINGIFY_(val) #val
/** Converts a macro argument into a character constant.*/
#define STRINGIFY(val)  STRINGIFY_(val)

/* FW and Board Information */
#define PROJECT_NAME                "STM32F429ZI TEST"

/* board revision */
#define BD_VER_MAJOR                0
#define BD_VER_MINOR                1
/* firmware version */
#define FW_VER_MAJOR                0
#define FW_VER_MINOR                1

/* board revision */
#define HW_BD_REV_01    ( ( 0 << 4 ) | ( 1 ) )
#define STR_BD_VER      "v" STRINGIFY( BD_VER_MAJOR ) "." STRINGIFY( BD_VER_MINOR )
#define DEF_BD_VER      CONCAT_3( HW_BD_REV_01, BD_VER_MAJOR, BD_VER_MINOR )
/* firmware version */
#define STR_FW_VER                      "v" STRINGIFY( FW_VER_MAJOR ) "." STRINGIFY( FW_VER_MINOR ) "." STRINGIFY( FW_VER_PATCH )
#define STR_FW_VER_LEN                  6
#define STR_HW_VER                      "v" STRINGIFY( BD_VER_MAJOR ) "." STRINGIFY( BD_VER_MINOR )
#endif

extern void BootMessagePrint (void);
extern void GetClockSourcePrint (void);

extern void User_CDC_Transmit_FS(uint8_t * pdata, uint16_t datalength);

/*
    GPIO LED Pin Number.

    GPIO_PIN_0
    GPIO_PIN_7
    GPIO_PIN_14

*/

#define USER_BUTTON_PORT        GPIOC

#define USER_BUTTON_PIN         GPIO_PIN_13

#define UART_DEBUG_PORT          GPIOG

#define DEBUG_TEST_PIN          GPIO_PIN_5
#define DEBUG_TEST_UART_PIN_1   GPIO_PIN_4
#define UART_TX_CPLT_TIME_PIN   GPIO_PIN_3

#define IOLINK_WAKEUP_PORT      GPIOG

#define IOLINK_WAKEUP_PIN       GPIO_PIN_8

#define LED_RED_PORT            GPIOB
#define LED_GREEN_PORT          GPIOB
#define LED_BLUE_PORT           GPIOB

#define LED_RED_PIN             GPIO_PIN_14
#define LED_GREEN_PIN           GPIO_PIN_0
#define LED_BLUE_PIN            GPIO_PIN_7

#define LED_ALL_PORT            GPIOB
#define LED_ALL_PIN             GPIO_PIN_14 | GPIO_PIN_0 | GPIO_PIN_7

#define UART_RXDATA_MAX                     1
#define UART_TXDATA_MAX                     1

extern uint32_t TIM1_CNT_1;
extern uint32_t TIM1_CNT_2;
extern uint32_t TIM1_CNT_3;

extern uint8_t USB_CdcRxBuffer_FS[];
extern uint8_t USB_CdcTxBuffer_FS[];

extern uint32_t USB_CdcRxBuffer_FS_cnt;
#define USB_CDC_RX_CNT      (USB_CdcRxBuffer_FS_cnt - 2)

extern uint8_t User_Str[];

extern uint8_t uart2_rx_buf[];
extern uint8_t uart2_tx_buf[];
extern uint8_t uart3_rx_buf[];
extern uint8_t uart3_tx_buf[];

extern uint16_t uart2_rx_index;
extern uint16_t uart2_tx_index;
extern uint16_t uart3_rx_index;
extern uint16_t uart3_tx_index;

extern uint8_t uart2_rx_flag;
extern uint8_t uart3_rx_flag;

extern uint16_t uart2_rxcpltcallback_cnt;

extern uint8_t UserButton_Flag;

extern uint32_t uart_rx_cnt_total;
extern uint16_t uart2_rx_cnt;
extern uint16_t uart3_rx_cnt;

extern uint8_t uart2_rx_ready;
extern uint8_t uart3_rx_ready;
extern uint8_t uart2_rx_tail_1;
extern uint8_t urat3_rx_tail_1;

// #define UART_RX_IDLE_BUFSIZE        10000
#define UART_RX_IDLE_BUFSIZE        5000
// #define UART_RX_IDLE_BUFSIZE        256

extern uint8_t uart2_rx_stack_buf[];

extern uint8_t uart2_rx_IDLE_buf[];
extern uint8_t uart3_rx_IDLE_buf[];
extern uint8_t uart6_rx_IDLE_buf[];

extern uint32_t uart_rx_IDLE_TotalCnt;

extern uint16_t iol_processdata_cnt;

extern uint8_t debug_uarttest_value;

extern uint32_t IOL_Master_RxEventCallback_Cnt;

extern uint16_t prv_uart6_size;

extern uint8_t uart6_rx_dumnmy_buf[];
extern uint8_t uart6_rx_cnt;

/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */
#endif

#ifdef __cplusplus
}
#endif
#endif /*__ USER_GPIO_PROC_H__ */


