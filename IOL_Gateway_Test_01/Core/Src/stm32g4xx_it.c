/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// #include "user_uart_proc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_FS;
extern TIM_HandleTypeDef htim1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */
extern uint8_t IOL_RX_CONTINUE_FLAG;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles USB low priority interrupt remap.
  */
void USB_LP_IRQHandler(void)
{
  /* USER CODE BEGIN USB_LP_IRQn 0 */

  /* USER CODE END USB_LP_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
  /* USER CODE BEGIN USB_LP_IRQn 1 */

  /* USER CODE END USB_LP_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt and TIM16 global interrupt.
  */
void TIM1_UP_TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM16_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM16_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
  if ((__HAL_UART_GET_FLAG(&huart1, UART_FLAG_FE)) || (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE)))
  {
    __HAL_UART_CLEAR_OREFLAG(&huart1);
    __HAL_UART_CLEAR_FEFLAG(&huart1);
  }
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  uint16_t num = 0;

  if ( (num = Q_NumContents(&USB_TX_Q)) > 0 )
  {
    if (CDC_Transmit_Is_Busy() != USBD_BUSY)
    {
      queDataNum += num;

      Q_Read(&USB_TX_Q, (uint8_t *)queData, num);

      CDC_Transmit_FS(queData, num);
    }
  }

  if (htim->Instance == TIM1)
  {
    TIM1_CNT_1++;
    TIM1_CNT_2++;
  }
}

uint8_t IOL_Rx_IDLEFlag = 0;
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  uart_rx_IDLE_TotalCnt += Size;
  
  if (huart->Instance == USART1)
  {


    if (uart_rx_IDLE_TotalCnt >= 3)
    {
      // DEBUG_GPIO_TOGGLE;
      if (IOL_Rx_IDLEFlag == 1)
      {
        IOL_Rx_IDLEFlag = 0;
      }
      else
      {
        DEBUG_GPIO_TOGGLE;
        IOL_Rx_IDLEFlag = 1;
        // IOL_PageTest(Size);
        IOL_Parse_Rx_data(uart1_rx_IDLE_buf);
        // IOL_Parse_Rx_data();
        IOL_StateM_Process();
        DEBUG_GPIO_TOGGLE;
        // IOL_StartUp_Seq_Page(Size);
      }

    }

    __HAL_DMA_DISABLE(&hdma_usart1_rx);
    hdma_usart1_rx.Instance->CNDTR = UART_RX_IDLE_BUFSIZE;
    __HAL_DMA_ENABLE(&hdma_usart1_rx);

    __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE);
    ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *) uart1_rx_IDLE_buf, UART_RX_IDLE_BUFSIZE);
    __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
    // if (IOL_Rx_IDLEFlag == 1)
    // {
    //   DEBUG_GPIO_TOGGLE;
    //   IOL_Rx_IDLEFlag = 0;
    // }
    // else if((IOL_Rx_IDLEFlag == 0) && (uart_rx_IDLE_TotalCnt >= 3))
    // {
    //   DEBUG_GPIO_TOGGLE;
    //   IOL_Rx_IDLEFlag = 1;
    //   IOL_PageTest(Size);
    // }

    // __HAL_DMA_DISABLE(&hdma_usart1_rx);
    // hdma_usart1_rx.Instance->CNDTR = UART_RX_IDLE_BUFSIZE;
    // __HAL_DMA_ENABLE(&hdma_usart1_rx);

    // __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE);
    // ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);
    // HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *) uart1_rx_IDLE_buf, UART_RX_IDLE_BUFSIZE);
    // __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
    // mseq_upload_master(Size);
    // HAL_UART_DMAResume(&huart1);

    // HAL_NVIC_EnableIRQ(USART1_IRQn);
    // HAL_UART_DMAResume(&huart1);

  }
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    IOL_DISABLE;
    IOL_PD_Buffer_Clear();
    // HAL_UART_MspInit(&huart1);

    // HAL_NVIC_EnableIRQ(USART1_IRQn);
    // HAL_UART_DMAResume(&huart1);

    // __HAL_DMA_DISABLE(&hdma_usart1_rx);
    // hdma_usart1_rx.Instance->CNDTR = UART_RX_IDLE_BUFSIZE;
    // __HAL_DMA_ENABLE(&hdma_usart1_rx);

    // __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE);
    // ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);
    // HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *) uart1_rx_IDLE_buf, UART_RX_IDLE_BUFSIZE);
    // __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
  }
}
/* USER CODE END 1 */
