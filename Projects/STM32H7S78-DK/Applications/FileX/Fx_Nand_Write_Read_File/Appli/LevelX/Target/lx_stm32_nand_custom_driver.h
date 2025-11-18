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

#ifndef LX_STM32_NAND_DRIVER_H
#define LX_STM32_NAND_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Includes ------------------------------------------------------------------*/
#include "lx_api.h"

/* USER CODE BEGIN Includes */
#include "w25n01gvxx.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
/* Define constants for the NAND flash custom. */

#define CUSTOM_TOTAL_BLOCKS                        W25N01GVXX_NUM_OF_BLOCK
#define CUSTOM_BYTES_PER_PHYSICAL_PAGE             W25N01GVXX_PAGE_MEM_SIZE
#define CUSTOM_SPARE_BYTES_PER_PAGE                W25N01GVXX_PAGE_SPARE_SIZE
#define CUSTOM_PHYSICAL_PAGES_PER_BLOCK            W25N01GVXX_NUM_PAGE_PER_BLOCK
#define CUSTOM_WORDS_PER_PHYSICAL_PAGE             W25N01GVXX_PAGE_SIZE2

#define CUSTOM_SPARE_DATA1_OFFSET                  4             /* Offset of spare data 1 */
#define CUSTOM_SPARE_DATA1_LENGTH                  4             /* length of spare data 1 */
#define CUSTOM_SPARE_DATA2_OFFSET                  2             /* Offset of spare data 2 */
#define CUSTOM_SPARE_DATA2_LENGTH                  2             /* length of spare data 2 */

#define CUSTOM_BAD_BLOCK_POSITION                  0             /* 0 is the bad block byte position */
#define CUSTOM_EXTRA_BYTE_POSITION                 2             /* 2 is the extra bytes starting byte position */
#define CUSTOM_ECC_BYTE_POSITION                   8             /* 8 is the ECC starting byte position */

#define NAND_FLASH_SIZE                            (W25N01GVXX_NUM_OF_BLOCK * W25N01GVXX_NUM_PAGE_PER_BLOCK * W25N01GVXX_PAGE_MEM_SIZE)

#define LX_DRIVER_ERASES_FLASH_AFTER_INIT
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

UINT  lx_stm32_nand_custom_driver_initialize(LX_NAND_FLASH *nand_flash);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
#ifdef __cplusplus
}
#endif
#endif /* LX_STM32_NAND_DRIVER_H */

