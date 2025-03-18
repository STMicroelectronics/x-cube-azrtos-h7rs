/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7rsxx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
extern SPI_HandleTypeDef hspi4;
extern UART_HandleTypeDef huart4;

extern void mxchip_WIFI_ISR(uint16_t pin);
extern void nx_driver_emw3080_interrupt(void);

#define MXCHIP_SPI      hspi4
void mxchip_WIFI_ISR(uint16_t pin);
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MXCHIP_NOTIFY_Pin GPIO_PIN_3
#define MXCHIP_NOTIFY_GPIO_Port GPIOF
#define MXCHIP_NOTIFY_EXTI_IRQn EXTI3_IRQn
#define MXCHIP_BOOT_Pin GPIO_PIN_2
#define MXCHIP_BOOT_GPIO_Port GPIOF
#define LED_RED_Pin GPIO_PIN_2
#define LED_RED_GPIO_Port GPIOM
#define MXCHIP_FLOW_Pin GPIO_PIN_1
#define MXCHIP_FLOW_GPIO_Port GPIOF
#define MXCHIP_FLOW_EXTI_IRQn EXTI1_IRQn
#define MXCHIP_RESET_Pin GPIO_PIN_6
#define MXCHIP_RESET_GPIO_Port GPIOE
#define LED_ORANGE_Pin GPIO_PIN_5
#define LED_ORANGE_GPIO_Port GPIOO
#define MXCHIP_NSS_Pin GPIO_PIN_10
#define MXCHIP_NSS_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
