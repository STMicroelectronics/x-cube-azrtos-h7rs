/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
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
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PLL_TIMEOUT  50U  /* 50 ms */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TX_THREAD tx_app_thread;
TX_SEMAPHORE tx_app_semaphore;
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void SystemClock_Restore(void);
static VOID App_Delay(ULONG Delay);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  /* USER CODE BEGIN App_ThreadX_MEM_POOL */

  /* USER CODE END App_ThreadX_MEM_POOL */
  CHAR *pointer;

  /* Allocate the stack for Main Thread  */
  if (tx_byte_allocate(byte_pool, (VOID**) &pointer,
                       TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }
  /* Create Main Thread.  */
  if (tx_thread_create(&tx_app_thread, "Main Thread", MainThread_Entry, 0, pointer,
                       TX_APP_STACK_SIZE, TX_APP_THREAD_PRIO, TX_APP_THREAD_PREEMPTION_THRESHOLD,
                       TX_APP_THREAD_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Create Semaphore.  */
  if (tx_semaphore_create(&tx_app_semaphore, "Semaphore", 0) != TX_SUCCESS)
  {
    return TX_SEMAPHORE_ERROR;
  }

  /* USER CODE BEGIN App_ThreadX_Init */

  /* USER CODE END App_ThreadX_Init */

  return ret;
}
/**
  * @brief  Function implementing the MainThread_Entry thread.
  * @param  thread_input: Hardcoded to 0.
  * @retval None
  */
void MainThread_Entry(ULONG thread_input)
{
  /* USER CODE BEGIN MainThread_Entry */
  (void) thread_input;
  UINT i = 0;
  /* Infinite loop */
  while (1)
  {
    if (tx_semaphore_get(&tx_app_semaphore, TX_WAIT_FOREVER) == TX_SUCCESS)
    {
      for (i=0; i<10; i++)
      {
      /* Toggle LED to indicate status*/
      HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
      App_Delay(50);
      }
    }
  }
  /* USER CODE END MainThread_Entry */
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/**
  * @brief  App_ThreadX_LowPower_Enter
  * @param  None
  * @retval None
  */
void App_ThreadX_LowPower_Enter(void)
{
  /* USER CODE BEGIN  App_ThreadX_LowPower_Enter */
  HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
  /* Enter to the stop mode */
  HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
  /* USER CODE END  App_ThreadX_LowPower_Enter */
}

/**
  * @brief  App_ThreadX_LowPower_Exit
  * @param  None
  * @retval None
  */
void App_ThreadX_LowPower_Exit(void)
{
  /* USER CODE BEGIN  App_ThreadX_LowPower_Exit */
  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
  /* Reconfigure the system clock*/
  SystemClock_Restore();
  /* USER CODE END  App_ThreadX_LowPower_Exit */
}

/* USER CODE BEGIN 1 */
/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_13)
  {
      /* Put the semaphore to release the MainThread and specify ceiling to 1 to avoid
      multiple semaphore puts by successively clicking on the user button */
      tx_semaphore_ceiling_put(&tx_app_semaphore,1);
  }
}
/**
  * @brief  Application Delay function.
  * @param  Delay : number of ticks to wait
  * @retval None
  */
void App_Delay(ULONG Delay)
{
  ULONG initial_time = tx_time_get();
  while ((tx_time_get() - initial_time) < Delay);
}
/**
  * @brief  Configures system clock after wake-up from STOP: enable HSE, PLL
  *         and select PLL as system clock source.
  * @param  None
  * @retval None
  */
void SystemClock_Restore(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  uint32_t tickstart;

  /* Reconfigure the main internal regulator output voltage to highest level */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE0) != HAL_OK)
  {
    Error_Handler();
  }

  /* Re-enable PLL1 used for System clock */
  __HAL_RCC_PLL1_ENABLE();

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Wait till PLL1 is ready */
  while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL1RDY) == 0U)
  {
    if ((HAL_GetTick() - tickstart) > PLL_TIMEOUT)
    {
      Error_Handler();
    }
  }

  /* Select PLL1 as SYSCLK source */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }

  /* Disable clock protection to re-enable PLL2 */
  HAL_RCCEx_DisableClockProtection(RCC_CLOCKPROTECT_XSPI);

  /* Re-enable PLL2 used for XSPI kernel clock */
  __HAL_RCC_PLL2_ENABLE();

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Wait till PLL2 is ready */
  while ((__HAL_RCC_GET_FLAG(RCC_FLAG_PLL2RDY) == 0U))
  {
    if ((HAL_GetTick() - tickstart) > PLL_TIMEOUT)
    {
      Error_Handler();
    }
  }

  /* Re-enable clock protection */
  HAL_RCCEx_EnableClockProtection(RCC_CLOCKPROTECT_XSPI);
}
/* USER CODE END 1 */
