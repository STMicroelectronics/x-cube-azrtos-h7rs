/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

#ifndef LX_STM32_NOR_DRIVER_H
#define LX_STM32_NOR_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Includes ------------------------------------------------------------------*/
#include "lx_api.h"

/* USER CODE BEGIN Includes */
#include "w25q128j.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#if !defined(LX_NOR_DISABLE_EXTENDED_CACHE) && (defined(LX_NOR_ENABLE_OBSOLETE_COUNT_CACHE) || defined(LX_NOR_ENABLE_MAPPING_BITMAP))
#error "[This error was thrown on purpose] : define the flags below related to the NOR Flash memory used"
#define LX_STM32_CUSTOM_SECTOR_SIZE                 1
#define LX_STM32_CUSTOM_FLASH_SIZE                  1

#ifdef LX_NOR_ENABLE_MAPPING_BITMAP
#define LX_STM32_CUSTOM_MAPPING_BITMAP_CACHE_SIZE ((LX_STM32_CUSTOM_FLASH_SIZE / LX_STM32_CUSTOM_SECTOR_SIZE) + 31) / 32
#else
#define LX_STM32_CUSTOM_MAPPING_BITMAP_CACHE_SIZE   0
#endif

#ifdef LX_NOR_ENABLE_OBSOLETE_COUNT_CACHE
#define LX_STM32_CUSTOM_OBSOLETE_COUNT_CACHE_SIZE (LX_STM32_CUSTOM_FLASH_SIZE / LX_STM32_CUSTOM_SECTOR_SIZE) * sizeof(LX_NOR_OBSOLETE_COUNT_CACHE_TYPE) / 4
#else
#define LX_STM32_CUSTOM_OBSOLETE_COUNT_CACHE_SIZE  0
#endif

#endif
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

UINT  lx_stm32_nor_custom_driver_initialize(LX_NOR_FLASH *nor_flash);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NOR_SECTOR_BUFFER_SIZE                      W25Q128J_SECTOR_SIZE/sizeof(ULONG)
/* USER CODE END PD */

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
#ifdef __cplusplus
}
#endif
#endif /* LX_STM32_NOR_DRIVER_H */

