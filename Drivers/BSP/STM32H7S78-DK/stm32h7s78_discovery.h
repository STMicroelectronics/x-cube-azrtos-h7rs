/**
  ******************************************************************************
  * @file    stm32h7s78_discovery.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for STM32H7S78-DK:
  *          LEDs
  *          push-buttons
  *          COM ports
  *          hardware resources.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32H7S78_DISCOVERY_H
#define STM32H7S78_DISCOVERY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7s78_discovery_conf.h"
#include "stm32h7s78_discovery_errno.h"

#if (USE_BSP_COM_FEATURE > 0)
#include <stdio.h>
#endif /* USE_BSP_COM_FEATURE */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H7S78_DK
  * @{
  */

/** @addtogroup STM32H7S78_DK_LOW_LEVEL
  * @{
  */

/** @defgroup STM32H7S78_DK_LOW_LEVEL_Exported_Types Exported Types
  * @{
  */

typedef enum
{
  LD1 = 0U,
  LED_GREEN  = LD1,
  LD2 = 1U,
  LED_ORANGE = LD2,
  LD3 = 2U,
  LED_RED    = LD3,
  LD4 = 3U,
  LED_BLUE   = LD4,
  LEDn
} Led_TypeDef;

typedef enum
{
  BUTTON_USER = 0U,
  BUTTONn
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0U,
  BUTTON_MODE_EXTI = 1U
} ButtonMode_TypeDef;

#if (USE_BSP_COM_FEATURE > 0)
typedef enum
{
  COM1 = 0U,
  COM2 = 1U,
  COMn
} COM_TypeDef;

typedef enum
{
  COM_STOPBITS_1     =   UART_STOPBITS_1,
  COM_STOPBITS_2     =   UART_STOPBITS_2,
} COM_StopBitsTypeDef;

typedef enum
{
  COM_PARITY_NONE     =  UART_PARITY_NONE,
  COM_PARITY_EVEN     =  UART_PARITY_EVEN,
  COM_PARITY_ODD      =  UART_PARITY_ODD,
} COM_ParityTypeDef;

typedef enum
{
  COM_HWCONTROL_NONE    =  UART_HWCONTROL_NONE,
  COM_HWCONTROL_RTS     =  UART_HWCONTROL_RTS,
  COM_HWCONTROL_CTS     =  UART_HWCONTROL_CTS,
  COM_HWCONTROL_RTS_CTS =  UART_HWCONTROL_RTS_CTS,
} COM_HwFlowCtlTypeDef;

typedef enum
{
  COM_WORDLENGTH_7B = UART_WORDLENGTH_7B,
  COM_WORDLENGTH_8B = UART_WORDLENGTH_8B,
  COM_WORDLENGTH_9B = UART_WORDLENGTH_9B,
} COM_WordLengthTypeDef;

typedef struct
{
  uint32_t              BaudRate;
  COM_WordLengthTypeDef WordLength;
  COM_StopBitsTypeDef   StopBits;
  COM_ParityTypeDef     Parity;
  COM_HwFlowCtlTypeDef  HwFlowCtl;
} COM_InitTypeDef;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
typedef struct
{
  void (* pMspInitCb)(UART_HandleTypeDef *);
  void (* pMspDeInitCb)(UART_HandleTypeDef *);
} BSP_COM_Cb_t;
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 1) */
#endif /* USE_BSP_COM_FEATURE > 0 */

/**
  * @}
  */

/** @defgroup STM32H7S78_DK_LOW_LEVEL_Exported_Constants Exported Constants
  * @{
  */

/**
  * @brief  Define for STM32H7S78_DK board
  */

/**
  * @brief STM32H7S78 Discovery BSP Driver version number
  */
#define STM32H7S78_DK_BSP_VERSION_MAIN   (0x01U) /*!< [31:24] main version */
#define STM32H7S78_DK_BSP_VERSION_SUB1   (0x00U) /*!< [23:16] sub1 version */
#define STM32H7S78_DK_BSP_VERSION_SUB2   (0x00U) /*!< [15:8]  sub2 version */
#define STM32H7S78_DK_BSP_VERSION_RC     (0x00U) /*!< [7:0]  release candidate */
#define STM32H7S78_DK_BSP_VERSION        ((STM32H7S78_DK_BSP_VERSION_MAIN << 24)\
                                          |(STM32H7S78_DK_BSP_VERSION_SUB1 << 16)\
                                          |(STM32H7S78_DK_BSP_VERSION_SUB2 << 8 )\
                                          |(STM32H7S78_DK_BSP_VERSION_RC))

#define STM32H7S78_DK_BSP_BOARD_NAME  "STM32H7S78-DK"
#define STM32H7S78_DK_BSP_BOARD_ID    "MB1736C"

/** @defgroup STM32H7S78_DK_LOW_LEVEL_LED EVAL LOW LEVEL LED
  * @{
  */
#define LD1_GPIO_PORT                   GPIOO
#define LD1_PIN                         GPIO_PIN_1

#define LD2_GPIO_PORT                   GPIOO
#define LD2_PIN                         GPIO_PIN_5

#define LD3_GPIO_PORT                   GPIOM
#define LD3_PIN                         GPIO_PIN_2

#define LD4_GPIO_PORT                   GPIOM
#define LD4_PIN                         GPIO_PIN_3


#define LD1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOO_CLK_ENABLE()
#define LD1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOO_CLK_DISABLE()

#define LD2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOO_CLK_ENABLE()
#define LD2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOO_CLK_DISABLE()

#define LD3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOM_CLK_ENABLE()
#define LD3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOM_CLK_DISABLE()

#define LD4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOM_CLK_ENABLE()
#define LD4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOM_CLK_DISABLE()
/**
  * @}
  */

/** @defgroup STM32H7S78_DK_LOW_LEVEL_BUTTON LOW LEVEL BUTTON
  * @{
  */
/* Button state */
#define BUTTON_RELEASED                    0U
#define BUTTON_PRESSED                     1U

/**
  * @brief USER push-button
  */
#define BUTTON_USER_PIN                   GPIO_PIN_13
#define BUTTON_USER_GPIO_PORT             GPIOC
#define BUTTON_USER_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUTTON_USER_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUTTON_USER_EXTI_IRQn             EXTI13_IRQn
#define BUTTON_USER_EXTI_LINE             EXTI_LINE_13

/**
  * @}
  */

/** @defgroup STM32H7S78_DK_LOW_LEVEL_COM LOW LEVEL COM
  * @{
  */
#if (USE_BSP_COM_FEATURE > 0)
/**
  * @brief Definition for COM port1, connected to UART4
  */
#define COM1_UART                      UART4

#define COM1_CLK_ENABLE()             __HAL_RCC_UART4_CLK_ENABLE()
#define COM1_CLK_DISABLE()            __HAL_RCC_UART4_CLK_DISABLE()

#define COM1_TX_PIN                   GPIO_PIN_1
#define COM1_TX_GPIO_PORT             GPIOD
#define COM1_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define COM1_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOD_CLK_DISABLE()
#define COM1_TX_AF                    GPIO_AF8_UART4

#define COM1_RX_PIN                   GPIO_PIN_0
#define COM1_RX_GPIO_PORT             GPIOD
#define COM1_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define COM1_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOD_CLK_DISABLE()
#define COM1_RX_AF                    GPIO_AF8_UART4

/**
  * @brief Definition for COM port2, connected to UART7
  */
#define COM2_UART                      UART7
#define COM2_CLK_ENABLE()             __HAL_RCC_UART7_CLK_ENABLE()
#define COM2_CLK_DISABLE()            __HAL_RCC_UART7_CLK_DISABLE()

#define COM2_TX_PIN                   GPIO_PIN_8
#define COM2_TX_GPIO_PORT             GPIOE
#define COM2_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define COM2_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOE_CLK_DISABLE()
#define COM2_TX_AF                    GPIO_AF7_UART7

#define COM2_RX_PIN                   GPIO_PIN_7
#define COM2_RX_GPIO_PORT             GPIOE
#define COM2_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()
#define COM2_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOE_CLK_DISABLE()
#define COM2_RX_AF                    GPIO_AF7_UART7


#define COM_POLL_TIMEOUT              1000

#define MX_UART_InitTypeDef COM_InitTypeDef
#endif /* USE_BSP_COM_FEATURE > 0 */
/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup STM32H7S78_DK_LOW_LEVEL_Exported_Variables
  * @{
  */
extern EXTI_HandleTypeDef hpb_exti[];
#if (USE_BSP_COM_FEATURE > 0)
extern UART_HandleTypeDef hcom_uart[];
extern USART_TypeDef *COM_UART[];
#endif /* USE_BSP_COM_FEATURE > 0 */
/**
  * @}
  */

/** @defgroup STM32H7S78_DK_LOW_LEVEL_Exported_FunctionsPrototypes Exported Functions Prototypes
  * @{
  */
int32_t        BSP_GetVersion(void);
const uint8_t *BSP_GetBoardName(void);
const uint8_t *BSP_GetBoardID(void);

int32_t BSP_LED_Init(Led_TypeDef Led);
int32_t BSP_LED_DeInit(Led_TypeDef Led);
int32_t BSP_LED_On(Led_TypeDef Led);
int32_t BSP_LED_Off(Led_TypeDef Led);
int32_t BSP_LED_Toggle(Led_TypeDef Led);
int32_t BSP_LED_GetState(Led_TypeDef Led);

int32_t  BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
int32_t  BSP_PB_DeInit(Button_TypeDef Button);
int32_t  BSP_PB_GetState(Button_TypeDef Button);
void     BSP_PB_Callback(Button_TypeDef Button);

#if (USE_BSP_COM_FEATURE > 0)
int32_t BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init);
int32_t BSP_COM_DeInit(COM_TypeDef COM);
#if (USE_COM_LOG > 0)
int32_t  BSP_COM_SelectLogPort(COM_TypeDef COM);
#endif /* USE_COM_LOG > 0 */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM);
int32_t BSP_COM_RegisterMspCallbacks(COM_TypeDef COM, BSP_COM_Cb_t *Callback);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS   */
HAL_StatusTypeDef MX_UART4_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *COM_Init);
HAL_StatusTypeDef MX_UART7_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *COM_Init);
#endif /* (USE_BSP_COM_FEATURE > 0)  */

void BSP_PB_IRQHandler(Button_TypeDef Button);

/**
  * @}
  */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32H7S78_DISCOVERY_H */
