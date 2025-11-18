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

#include "lx_stm32_nand_custom_driver.h"

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern SPI_HandleTypeDef hspi4;
static uint8_t flash_is_initialized = 0;
static uint8_t  nand_flash_rw_buffer[CUSTOM_WORDS_PER_PHYSICAL_PAGE] = {0};
static UCHAR  Buffer_Spare_Area[CUSTOM_SPARE_BYTES_PER_PAGE];
/* USER CODE END PV */

/* Exported functions prototypes ---------------------------------------------*/

static UINT  lx_nand_driver_read(ULONG block, ULONG page, ULONG *destination, ULONG words);
static UINT  lx_nand_driver_write(ULONG block, ULONG page, ULONG *source, ULONG words);

static UINT  lx_nand_driver_block_erase(ULONG block, ULONG erase_count);
static UINT  lx_nand_driver_block_erased_verify(ULONG block);
static UINT  lx_nand_driver_page_erased_verify(ULONG block, ULONG page);

static UINT  lx_nand_driver_block_status_get(ULONG block, UCHAR *bad_block_byte);
static UINT  lx_nand_driver_block_status_set(ULONG block, UCHAR bad_block_byte);

static UINT  lx_nand_driver_extra_bytes_get(ULONG block, ULONG page, UCHAR *destination, UINT size);
static UINT  lx_nand_driver_extra_bytes_set(ULONG block, ULONG page, UCHAR *source, UINT size);

static UINT  lx_nand_driver_system_error(UINT error_code, ULONG block, ULONG page);

static UINT  lx_nand_flash_driver_pages_read(ULONG block, ULONG page, UCHAR *main_buffer, UCHAR *spare_buffer, ULONG pages);
static UINT  lx_nand_flash_driver_pages_write(ULONG block, ULONG page, UCHAR *main_buffer, UCHAR *spare_buffer, ULONG pages);
static UINT  lx_nand_flash_driver_pages_copy(ULONG source_block, ULONG source_page, ULONG destination_block, ULONG destination_page, ULONG pages, UCHAR *data_buffer);

/* USER CODE BEGIN EFP */
static UINT  _lx_nand_flash_erase_all_driver(VOID);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

#ifndef CUSTOM_WORDS_PER_PHYSICAL_PAGE
#define CUSTOM_WORDS_PER_PHYSICAL_PAGE 512
#endif

UINT lx_stm32_nand_custom_driver_initialize(LX_NAND_FLASH *nand_flash)
{
  UINT ret = LX_SUCCESS;

  /* USER CODE BEGIN Init_Section_0 */
  uint8_t device_id[4];

  if (!flash_is_initialized)
  {
    /* Initialize w25n01gvxx interface */
    if (w25n01gvxx_Init(&hspi4) == W25N01GVXX_ERROR)
    {
      return (LX_ERROR);
    }

    /* Read the NAND memory ID */
    w25n01gvxx_ReadID(&hspi4, device_id);

    /* Test the NAND ID correctness */
    if (((uint32_t)device_id[3] | ((uint32_t)device_id[2] << 8) | ((uint32_t)device_id[1] << 16)) != W25N01GVXX_CHIPID)
    {
      return (LX_ERROR);
    }
    /* If ERASE_CHIP enabled erase the NAND device */
#ifdef LX_DRIVER_ERASES_FLASH_AFTER_INIT
      if (_lx_nand_flash_erase_all_driver() != LX_SUCCESS)
      {
        return (LX_ERROR);
      }
#endif
    flash_is_initialized = 1;
  }
  /*USER CODE END Init_Section_0 */

  nand_flash->lx_nand_flash_total_blocks =                  CUSTOM_TOTAL_BLOCKS;
  nand_flash->lx_nand_flash_pages_per_block =               CUSTOM_PHYSICAL_PAGES_PER_BLOCK;
  nand_flash->lx_nand_flash_bytes_per_page =                CUSTOM_BYTES_PER_PHYSICAL_PAGE;

  nand_flash -> lx_nand_flash_spare_data1_offset =          CUSTOM_SPARE_DATA1_OFFSET;
  nand_flash -> lx_nand_flash_spare_data1_length =          CUSTOM_SPARE_DATA1_LENGTH;

  nand_flash -> lx_nand_flash_spare_data2_offset =          CUSTOM_SPARE_DATA2_OFFSET;
  nand_flash -> lx_nand_flash_spare_data2_length =          CUSTOM_SPARE_DATA2_LENGTH;

  nand_flash -> lx_nand_flash_spare_total_length =          CUSTOM_SPARE_BYTES_PER_PAGE;

  /* USER CODE BEGIN Init_Section_1 */

  /*USER CODE END Init_Section_1 */

  nand_flash->lx_nand_flash_driver_read =                   lx_nand_driver_read;
  nand_flash->lx_nand_flash_driver_write =                  lx_nand_driver_write;

  nand_flash->lx_nand_flash_driver_block_erase =            lx_nand_driver_block_erase;
  nand_flash->lx_nand_flash_driver_block_erased_verify =    lx_nand_driver_block_erased_verify;
  nand_flash->lx_nand_flash_driver_page_erased_verify =     lx_nand_driver_page_erased_verify;

  nand_flash->lx_nand_flash_driver_block_status_get =       lx_nand_driver_block_status_get;
  nand_flash->lx_nand_flash_driver_block_status_set =       lx_nand_driver_block_status_set;

  nand_flash->lx_nand_flash_driver_extra_bytes_get =        lx_nand_driver_extra_bytes_get;
  nand_flash->lx_nand_flash_driver_extra_bytes_set =        lx_nand_driver_extra_bytes_set;

  nand_flash->lx_nand_flash_driver_system_error =           lx_nand_driver_system_error;

  nand_flash->lx_nand_flash_driver_pages_read =             lx_nand_flash_driver_pages_read;
  nand_flash->lx_nand_flash_driver_pages_write =            lx_nand_flash_driver_pages_write;
  nand_flash->lx_nand_flash_driver_pages_copy =             lx_nand_flash_driver_pages_copy;

  /* USER CODE BEGIN Init_Section_2 */

  /*USER CODE END Init_Section_2 */

  return ret;

}

static UINT  lx_nand_driver_read(ULONG block, ULONG page, ULONG *destination, ULONG words)
{

  UINT ret = LX_SUCCESS;

 /* USER CODE BEGIN driver_read */
  /* Not used parameter */
  LX_PARAMETER_NOT_USED(words);
  if (w25n01gvxx_ReadPage(&hspi4, block, page, (UCHAR *)destination, W25N01GVXX_PAGE_MEM_SIZE) == W25N01GVXX_ERROR)
  {
    return (LX_ERROR);
  }
 /* USER CODE END driver_read */

  return ret;
}

static UINT  lx_nand_driver_write(ULONG block, ULONG page, ULONG *source, ULONG words)
{
  UINT ret = LX_SUCCESS;

 /* USER CODE BEGIN driver_write */
  /* Not used parameter */
  LX_PARAMETER_NOT_USED(words);
  if ((w25n01gvxx_WriteData(&hspi4, block, page, (uint8_t *)source, W25N01GVXX_PAGE_MEM_SIZE)) == W25N01GVXX_ERROR)
  {
    return (LX_ERROR);
  }
 /* USER CODE END driver_write */

  return ret;
}

static UINT  lx_nand_driver_block_erase(ULONG block, ULONG erase_count)
{
  UINT ret = LX_SUCCESS;

 /* USER CODE BEGIN block_erase */
  LX_PARAMETER_NOT_USED(erase_count);

  if (w25n01gvxx_EraseBlock(&hspi4, block) != W25N01GVXX_OK)
  {
    return (LX_ERROR);
  }
 /* USER CODE END block_erase */

  return ret;
}

static UINT  lx_nand_driver_block_erased_verify(ULONG block)
{
  UINT ret = LX_SUCCESS;

 /* USER CODE BEGIN block_erase_verify */
  uint32_t index = 0;

  for (index = 0; index < W25N01GVXX_NUM_PAGE_PER_BLOCK; index++)
  {
    if (lx_nand_driver_page_erased_verify(block, index) != LX_SUCCESS)
    {
      return (LX_ERROR);
    }
  }
 /* USER CODE END block_erase_verify */

  return ret;
}

static UINT  lx_nand_driver_page_erased_verify(ULONG block, ULONG page)
{
  UINT ret = LX_SUCCESS;

 /* USER CODE BEGIN page_erased_verify */

  /* Initialize NAND instance */
  UCHAR  *word_ptr;
  ULONG  words;

  memset(nand_flash_rw_buffer, 0, sizeof(nand_flash_rw_buffer));

  if ( w25n01gvxx_ReadPage(&hspi4, block, page, nand_flash_rw_buffer, W25N01GVXX_PAGE_SIZE2) != W25N01GVXX_OK)
  {
    return (LX_ERROR);
  }

  word_ptr = (UCHAR *) & (nand_flash_rw_buffer[0]);

  /* Calculate the number of words in a page.  */
  words =  W25N01GVXX_PAGE_SIZE2;

  /* Loop to read flash.  */
  while (words--)
  {
    /* Is this word erased?  */
    if (*word_ptr++ != 0xFF)
    {
      return (LX_ERROR);
    }
  }
 /* USER CODE END page_erased_verify */

  return ret;
}

static UINT  lx_nand_driver_block_status_get(ULONG block, UCHAR *bad_block_byte)
{
  UINT ret = LX_SUCCESS;

 /* USER CODE BEGIN block_status_get */
  if(w25n01gvxx_ReadSpareArea(&hspi4, block, 0,  0, Buffer_Spare_Area) != W25N01GVXX_OK)
  {
    return (LX_ERROR);
  }

  *bad_block_byte = Buffer_Spare_Area[0];

 /* USER CODE END block_status_get*/

  return ret;
}

static UINT  lx_nand_driver_block_status_set(ULONG block, UCHAR bad_block_byte)
{
  UINT ret = LX_SUCCESS;

 /* USER CODE BEGIN block_status_set */
  if(w25n01gvxx_ReadSpareArea(&hspi4, block, 0,  0, Buffer_Spare_Area) != W25N01GVXX_OK)
  {
    return (LX_ERROR);
  }

  Buffer_Spare_Area[CUSTOM_BAD_BLOCK_POSITION] = bad_block_byte;


  if(w25n01gvxx_WriteSpareArea(&hspi4, block, 0, 0, (uint8_t *) Buffer_Spare_Area) !=W25N01GVXX_OK)
  {
    return (LX_ERROR);
  }

 /* USER CODE END block_status_set */

  return ret;
}

static UINT  lx_nand_driver_extra_bytes_get(ULONG block, ULONG page, UCHAR *destination, UINT size)
{
  UINT ret = LX_SUCCESS;

 /* USER CODE BEGIN extra_bytes_get */
  if(w25n01gvxx_ReadSpareArea(&hspi4, block, page, 0, Buffer_Spare_Area) != W25N01GVXX_OK)
  {
    return (LX_ERROR);
  }

  memcpy(destination, Buffer_Spare_Area, size);

 /* USER CODE END extra_bytes_get */

  return ret;
}

static UINT  lx_nand_driver_extra_bytes_set(ULONG block, ULONG page, UCHAR *source, UINT size)
{
  UINT ret = LX_SUCCESS;

 /* USER CODE BEGIN extra_bytes_set */
  if(w25n01gvxx_ReadSpareArea(&hspi4, block, page, 0, Buffer_Spare_Area) != W25N01GVXX_OK)
  {
    return (LX_ERROR);
  }

  memcpy(Buffer_Spare_Area, source, size);

  if(w25n01gvxx_WriteSpareArea(&hspi4, block, page, 0, (uint8_t *) Buffer_Spare_Area) !=W25N01GVXX_OK)
  {
    return (LX_ERROR);
  }
 /* USER CODE END extra_bytes_set */

  return ret;
}

static UINT  lx_nand_driver_system_error(UINT error_code, ULONG block, ULONG page)
{
  UINT ret = LX_SUCCESS;

 /* USER CODE BEGIN system_error */
  LX_PARAMETER_NOT_USED(error_code);
  LX_PARAMETER_NOT_USED(block);
  LX_PARAMETER_NOT_USED(page);

  /* Custom processing goes here...  all errors except for LX_NAND_ERROR_CORRECTED are fatal.  */
  ret = LX_ERROR;
 /* USER CODE END system_error */

  return ret;
}

static UINT  lx_nand_flash_driver_pages_read(ULONG block, ULONG page, UCHAR *main_buffer, UCHAR *spare_buffer, ULONG pages)
{
  UINT ret = LX_SUCCESS;

 /* USER CODE BEGIN driver_pages_read */
  ULONG    i;

  for (i = 0; i < pages; i++)
  {
    if (main_buffer)
    {
      if (lx_nand_driver_read(block, page + i, (ULONG*)(main_buffer + i * CUSTOM_BYTES_PER_PHYSICAL_PAGE), CUSTOM_WORDS_PER_PHYSICAL_PAGE) == LX_ERROR)
      {
        return (LX_ERROR);
      }
    }

    if (lx_nand_driver_extra_bytes_get(block, page + i, spare_buffer + i * CUSTOM_SPARE_BYTES_PER_PAGE, CUSTOM_SPARE_BYTES_PER_PAGE) == LX_ERROR)
    {
      return (LX_ERROR);
    }
  }
 /* USER CODE END driver_pages_read */

  return ret;
}

static UINT  lx_nand_flash_driver_pages_write(ULONG block, ULONG page, UCHAR *main_buffer, UCHAR *spare_buffer, ULONG pages)
{
  UINT ret = LX_SUCCESS;

 /* USER CODE BEGIN driver_pages_write */
  ULONG    i;

  for (i = 0; i < pages; i++)
  {
    if (lx_nand_driver_write(block, page + i, (ULONG*)(main_buffer + i * CUSTOM_BYTES_PER_PHYSICAL_PAGE), CUSTOM_WORDS_PER_PHYSICAL_PAGE) == LX_ERROR)
    {
      return (LX_ERROR);
    }

    if (lx_nand_driver_extra_bytes_set(block, page + i, spare_buffer + i * CUSTOM_SPARE_BYTES_PER_PAGE, CUSTOM_SPARE_BYTES_PER_PAGE) == LX_ERROR)
    {
      return (LX_ERROR);
    }
  }
 /* USER CODE END driver_pages_write */

  return ret;
}

static UINT  lx_nand_flash_driver_pages_copy(ULONG source_block, ULONG source_page, ULONG destination_block, ULONG destination_page, ULONG pages, UCHAR *data_buffer)
{
  UINT ret = LX_SUCCESS;

 /* USER CODE BEGIN driver_pages_copy */
  ULONG    i;

  /* Initialize NAND instance */
  for (i = 0; i < pages; i++)
  {
    if (w25n01gvxx_PagesCopy(&hspi4 , source_block, source_page, destination_block, destination_page, W25N01GVXX_PAGE_SIZE2) != W25N01GVXX_OK)
    {
      return (LX_ERROR);
    }
  }

 /* USER CODE END driver_pages_copy */

  return ret;
}

/* USER CODE BEGIN 2 */
static UINT  _lx_nand_flash_erase_all_driver(VOID)
{
  if (w25n01gvxx_FullChipErase(&hspi4) != W25N01GVXX_OK)
  {
    return (LX_ERROR);
  }

  return (LX_SUCCESS);
}
/* USER CODE END 2 */
