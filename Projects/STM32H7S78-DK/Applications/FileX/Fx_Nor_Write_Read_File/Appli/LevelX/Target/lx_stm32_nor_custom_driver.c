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

#include "lx_stm32_nor_custom_driver.h"

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Private includes ----------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
#if defined(LX_NOR_ENABLE_OBSOLETE_COUNT_CACHE) || defined(LX_NOR_ENABLE_MAPPING_BITMAP)
UCHAR lx_stm32_nor_custom_extended_cache_memory[LX_STM32_CUSTOM_OBSOLETE_COUNT_CACHE_SIZE + LX_STM32_CUSTOM_MAPPING_BITMAP_CACHE_SIZE];
#endif
/* USER CODE BEGIN PV */
extern SPI_HandleTypeDef hspi4;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

static UINT  lx_nor_driver_read(ULONG *flash_address, ULONG *destination, ULONG words);
static UINT  lx_nor_driver_write(ULONG *flash_address, ULONG *source, ULONG words);

static UINT  lx_nor_driver_block_erase(ULONG block, ULONG erase_count);
static UINT  lx_nor_driver_block_erased_verify(ULONG block);

/* USER CODE BEGIN USER_CODE_SECTION_1 */

/* USER CODE END USER_CODE_SECTION_1 */

#ifndef LX_DIRECT_READ

#ifndef NOR_SECTOR_BUFFER_SIZE
#define NOR_SECTOR_BUFFER_SIZE 512
#endif

static ULONG nor_sector_memory[NOR_SECTOR_BUFFER_SIZE];
#endif

UINT  lx_stm32_nor_custom_driver_initialize(LX_NOR_FLASH *nor_flash)
{
  UINT ret = LX_SUCCESS;

  ULONG total_blocks = 0;
  ULONG words_per_block = 0;

  /* USER CODE BEGIN Init_Section_0 */
  w25q128j_status_t w25q128j_status = W25Q128J_ERROR;

  /* get the memory part characteristics */
   w25q128j_status = w25q128j_Init(&hspi4);
   if(w25q128j_status != W25Q128J_OK)
   {
    return LX_ERROR;
   }

  total_blocks = (W25Q128J_FLASH_SIZE / W25Q128J_BLOCK_SIZE);
  words_per_block = W25Q128J_BLOCK_SIZE / sizeof(ULONG);
  nor_flash->lx_nor_flash_base_address = (ULONG *)0;
  /* USER CODE END Init_Section_0 */

  nor_flash->lx_nor_flash_total_blocks    = total_blocks;
  nor_flash->lx_nor_flash_words_per_block = words_per_block;

  /* USER CODE BEGIN Init_Section_1 */

  /* USER CODE END Init_Section_1 */

  nor_flash->lx_nor_flash_driver_read = lx_nor_driver_read;
  nor_flash->lx_nor_flash_driver_write = lx_nor_driver_write;

  nor_flash->lx_nor_flash_driver_block_erase = lx_nor_driver_block_erase;
  nor_flash->lx_nor_flash_driver_block_erased_verify = lx_nor_driver_block_erased_verify;

#ifndef LX_DIRECT_READ
    nor_flash->lx_nor_flash_sector_buffer = nor_sector_memory;
#endif

  /* USER CODE BEGIN Init_Section_2 */

  /* USER CODE END Init_Section_2 */

    return ret;
}

/* USER CODE BEGIN USER_CODE_SECTION_2 */

/* USER CODE END USER_CODE_SECTION_2 */

static UINT lx_nor_driver_read(ULONG *flash_address, ULONG *destination, ULONG words)
{
    UINT ret = LX_SUCCESS;

    /* USER CODE BEGIN NOR_READ */
  /* perform the read operation */
  ret = w25q128j_Read(&hspi4, (uint8_t *) destination, (uint32_t ) flash_address, words * sizeof(ULONG));
  if (ret != W25Q128J_OK)
  {
    return LX_ERROR;
  }
    /* USER CODE END  NOR_READ */

    return ret;
}

static UINT lx_nor_driver_write(ULONG *flash_address, ULONG *source, ULONG words)
{
    UINT ret = LX_SUCCESS;

    /* USER CODE BEGIN NOR_DRIVER_WRITE */
  /* perform the write operation */
  ret = w25q128j_Write(&hspi4, (uint8_t *) source,
                          (uint32_t)flash_address, words * sizeof(ULONG));
  if (ret != W25Q128J_OK)
  {
    return LX_ERROR;
  }
    /* USER CODE END  NOR_DRIVER_WRITE */

    return ret;
}

static UINT lx_nor_driver_block_erase(ULONG block, ULONG erase_count)
{

    UINT ret = LX_SUCCESS;

    /* USER CODE BEGIN NOR_DRIVER_BLOCK  */
  /* perform the block erase operation */
  ret = w25q128j_Erase(&hspi4, block, W25Q128J_ERASE_64K_BLOCK);
  if (ret != W25Q128J_OK)
  {
    return LX_ERROR;
  }
    /* USER CODE END  NOR_DRIVER_BLOCK  */

    return ret;
}

static UINT lx_nor_driver_block_erased_verify(ULONG block)
{
    UINT ret = LX_SUCCESS;

    /* USER CODE BEGIN NOR_DRIVER_VERIFY  */

    /* USER CODE END  NOR_DRIVER_VERIFY  */

    return ret;
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
