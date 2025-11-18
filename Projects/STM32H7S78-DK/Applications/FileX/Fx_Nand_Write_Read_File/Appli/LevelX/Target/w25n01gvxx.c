/**
  ******************************************************************************
  * @file    w25n01gvxx.c
  * @modify  MCD Application Team
  * @brief   This file provides the W25N01GVXX NAND flash memory drivers.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ----------------------------------------------------------------------- */
#include <stdio.h>
#include "w25n01gvxx.h"


/**
 * @brief  Selects the W25N01GVXX flash memory by setting the CS line low.
 * @param  None (No parameters required for this function)
 * @retval ret  Returns W25N01GVXX_OK if the CS line is successfully set to low,
 */
static int32_t w25n01gvxx_Select(void)
{
   int32_t ret = W25N01GVXX_OK;

   /* Set the CS pin LOW */
   HAL_GPIO_WritePin(W25N01GVXX_CS_PORT, W25N01GVXX_CS_PIN, GPIO_PIN_RESET);

   /*  Verify if the CS pin is indeed LOW */
   GPIO_PinState pinState = HAL_GPIO_ReadPin(W25N01GVXX_CS_PORT, W25N01GVXX_CS_PIN);

   if (pinState != GPIO_PIN_RESET)
   {
     /* Failed to set CS pin LOW */
     ret = W25N01GVXX_ERROR;
   }
   /* Return PART status */
   return ret;
}


/**
 * @brief  De-selects the W25N01GVXX flash memory by setting the CS pin HIGH.
 * @param  None (No parameters required for this function)
 * @retval ret  Returns W25N01GVXX_OK if the CS line is successfully set to HIGH,
 */
static int32_t w25n01gvxx_Deselect(void)
{
   int32_t ret = W25N01GVXX_OK;

   /* Set the CS pin LOW */
   HAL_GPIO_WritePin(W25N01GVXX_CS_PORT, W25N01GVXX_CS_PIN, GPIO_PIN_SET);

   /*  Verify if the CS pin is indeed HIGH */
   GPIO_PinState pinState = HAL_GPIO_ReadPin(W25N01GVXX_CS_PORT, W25N01GVXX_CS_PIN);

   if (pinState != GPIO_PIN_SET)
   {
     /* Failed to set CS pin HIGH */
     ret = W25N01GVXX_ERROR;
   }
   /* Return PART status */
   return ret;
}


/**
 * @brief  Sets the Write Enable Latch (WEL) bit in the Status Register to a 1.
 * @param  phspi                 SPI handle pointer
 * @retval ret                   W25n01xx flash memory Status
 */
int32_t w25n01gvxx_WriteEnable(SPI_HandleTypeDef *phspi)
{
   int32_t ret = W25N01GVXX_OK ;

   /* Check parameters */
   if (phspi == NULL)
   {
     return W25N01GVXX_ERROR;
   }

   /* Initialize the write enable command */
   uint8_t cmd = W25N01GVXX_CMD_WRITE_ENABLE;

   /* Select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Select()) != W25N01GVXX_OK)
   {
     /* Failed to select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Send the write enable command via SPI */
   if ((HAL_SPI_Transmit(phspi, &cmd, 1, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to send command */
     ret = W25N01GVXX_ERROR;
   }

   /* Deselect the w25n01gvxx flash memory  */
   if((w25n01gvxx_Deselect()) != W25N01GVXX_OK)
   {
     /* Failed to de-select w25n01gvxx flash memory */
     ret = W25N01GVXX_ERROR;
   }

   /* Return PART status */
   return ret;
}


/**
 * @brief  Resets the Write Enable Latch (WEL) bit in the Status Register to a 0.
 * @param  phspi                 SPI handle pointer
 * @retval ret                   W25n01xx flash memory Status
 */
int32_t w25n01gvxx_WriteDisable(SPI_HandleTypeDef *phspi)
{
   int32_t ret = W25N01GVXX_OK ;

   /* Check parameters */
   if (phspi == NULL)
   {
     return W25N01GVXX_ERROR;
   }

   /* Initialize the write disable command */
   uint8_t cmd = W25N01GVXX_CMD_WRITE_DISABLE;

   /* Select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Select()) != W25N01GVXX_OK)
   {
     /* Failed to select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Send the write disable command via SPI */
   if((HAL_SPI_Transmit(phspi, &cmd, 1, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to send command */
     ret = W25N01GVXX_ERROR;
   }

   /* Deselect the w25n01gvxx flash memory  */
   if((w25n01gvxx_Deselect()) != W25N01GVXX_OK)
   {
     /* Failed to de-select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Return PART status */
   return ret;
}


/**
 * @brief  Resets the W25n01xx NAND flash memory
 * @param  phspi                 SPI handle pointer
 * @retval W25n01xx flash memory Status
 */
int32_t w25n01gvxx_Reset(SPI_HandleTypeDef *phspi)
{
   int32_t ret = W25N01GVXX_OK;
   uint8_t stat_reg;
   /* Check parameters */
   if (phspi == NULL)
   {
     return W25N01GVXX_ERROR;
   }

   /* Initialize the reset command */
   uint8_t tData = W25N01GVXX_CMD_RESET;

   /* Check the BUSY bit in the status register */
   do
   {

   if (w25n01gvxx_ReadStatusRegister(phspi, &stat_reg) != W25N01GVXX_OK)
   {
     return W25N01GVXX_ERROR;
   }

   }while((stat_reg & W25N01GVXX_RS3_S0_BUSY) == W25N01GVXX_RS3_S0_BUSY);

   /* Select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Select()) != W25N01GVXX_OK)
   {
     /* Failed to select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Send the reset command via SPI */
   if((HAL_SPI_Transmit(phspi, &tData, 1, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to send command */
     ret = W25N01GVXX_ERROR;
   }

   /* Deselect the flash memory */
   if((w25n01gvxx_Deselect()) != W25N01GVXX_OK)
   {
     /* Failed to de-select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Wait for BUSY bit in the status register to bet cleared */
   do
   {
     /* Read the status register value  */
   if (w25n01gvxx_ReadStatusRegister(phspi, &stat_reg) != W25N01GVXX_OK)
   {
     /* Failed to read status register */
     return W25N01GVXX_ERROR;
   }

   }while((stat_reg & W25N01GVXX_RS3_S0_BUSY) == W25N01GVXX_RS3_S0_BUSY);

   /* Return PART status */
   return ret;
}

/**
 * @brief  Reads the w25n01xx NAND flash memory ID.
 * @param  phspi                 SPI handle pointer
 * @param  pid                   Pointer to store the part ID
 * @retval W25n01xx flash memory Status
 */
int32_t w25n01gvxx_ReadID (SPI_HandleTypeDef *phspi, uint8_t *pid)
{
   int32_t ret = W25N01GVXX_OK;

   /* Check parameters */
   if (phspi == NULL || pid == NULL)
   {
     return W25N01GVXX_ERROR;
   }

   /* Initialize the read ID command */
   uint8_t cmd = W25N01GVXX_CMD_DEVICE_ID;

   /* Select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Select()) != W25N01GVXX_OK)
   {
     /* Failed to select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   else if((HAL_SPI_Transmit(phspi, &cmd, 1, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to send command */
     ret = W25N01GVXX_ERROR;
   }
   else if((HAL_SPI_Receive(phspi, pid, 4, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to receive data */
     ret = W25N01GVXX_ERROR;
   }

   else if((w25n01gvxx_Deselect()) != W25N01GVXX_OK)
   {
     /* Failed to de-select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }
   else
   {
     ret = W25N01GVXX_OK;
   }
   /* Return PART status */
   return ret;
}


/**
 * @brief  Gets Flash information.
 * @param  pinfo                 Pointer to information structure
 * @retval 0 (always succeeds)
 */
int32_t w25n01gvxx_GetInfo(w25n01gvxx_info_t *pinfo)
{

   /* Configure the information structure with the memory configuration */
   pinfo->Flash_size                = W25N01GVXX_FLASH_SIZE;
   pinfo->BlockSize                 = W25N01GVXX_BLOCK_SIZE;
   pinfo->BlockCount                = (W25N01GVXX_FLASH_SIZE / W25N01GVXX_BLOCK_SIZE);
   pinfo->SectorSize                = W25N01GVXX_SECTOR_SIZE;
   pinfo->SectorCount               = (W25N01GVXX_FLASH_SIZE / W25N01GVXX_SECTOR_SIZE);
   pinfo->PageSize                  = W25N01GVXX_PAGE_SIZE1;
   pinfo->PageCount                 = (W25N01GVXX_FLASH_SIZE / W25N01GVXX_PAGE_SIZE1);
   pinfo->SectorsPerPage            = W25N01GVXX_NUM_OF_SECTOR_IN_PAGE;
   pinfo->SpareAreaSize             = W25N01GVXX_PAGE_SPARE_SIZE;
   pinfo->SpareSectors              = W25N01GVXX_NUM_SPARE_SECTOR_IN_PAGE;
   pinfo->SpareSectorSize           = W25N01GVXX_SPARE_SECTOR_SIZE;

   /* Return PART status */
   return W25N01GVXX_OK;
}


/**
 * @brief  Initializes the w25n01xx NAND flash memory
 * @param  phspi                 SPI handle pointer
 * @retval W25n01xx flash memory Status
 */
int32_t w25n01gvxx_Init(SPI_HandleTypeDef *phspi)
{
   int32_t ret = W25N01GVXX_OK;

   /* Check parameters */
   if (phspi == NULL)
   {
     return W25N01GVXX_ERROR;
   }

   /* Enable write */
   if (w25n01gvxx_WriteEnable(phspi) != W25N01GVXX_OK)
   {
   /* Failed to enable write */
     ret = W25N01GVXX_ERROR;
   }
   /* Disable the Block protection from all the blocks */
   else if (w25n01gvxx_WriteProtectionRegister(phspi, W25N01GVXX_RS1_WRITE_PROTECTION_DISABLE) != W25N01GVXX_OK)
   {
   /* Failed to disable the protection */
     ret = W25N01GVXX_ERROR;
   }

   /* Initialize the buffer read mode */
   else if (w25n01gvxx_WriteConfigRegister(phspi, W25N01GVXX_RS2_PAGE_READ_MODE) != W25N01GVXX_OK)
   {
   /* Failed to initialize buffer read mode */
     ret = W25N01GVXX_ERROR;
   }
   /* reset part */
   else if (w25n01gvxx_Reset(phspi) != W25N01GVXX_OK)
   {
   /* failed to reset part */
     ret = W25N01GVXX_ERROR;
   }

   /* Return PART status */
   return ret;
}


/**
 * @brief  De-initializes the W25n01xx NAND flash memory
 * @param  phspi                 SPI handle pointer
 * @retval W25n01xx flash memory Status
 */
int32_t w25n01gvxx_DeInit(SPI_HandleTypeDef *phspi)
{

   /* Check parameters */
   if (phspi == NULL)
   {
     return W25N01GVXX_ERROR;
   }

   /* Disable write */
   if (w25n01gvxx_WriteDisable(phspi) != W25N01GVXX_OK)
   {
   /* Failed to disable write */
     return W25N01GVXX_ERROR;
   }
   /* Enable the Block protection to all the blocks */
   else if (w25n01gvxx_WriteProtectionRegister(phspi, W25N01GVXX_RS1_DEFAULT) != W25N01GVXX_OK)
   {
   /* Failed to enable the protection */
     return W25N01GVXX_ERROR;
   }

   /* Initialize the continuous read mode */
   else if (w25n01gvxx_WriteConfigRegister(phspi, W25N01GVXX_RS2_PAGE_CONTINOUS_READ_MODE) != W25N01GVXX_OK)
   {
   /* Failed to initialize buffer read mode */
     return W25N01GVXX_ERROR;
   }

   /* Return PART status */
   return W25N01GVXX_OK;
}


/**
 * @brief  Program the Data Buffer content into the physical memory page
 * @param  phspi                 SPI handle pointer
 * @param  pageAddr              Page address that we want to write the data to
 * @retval W25n01xx flash memory Status
*/
static int32_t w25n01gvxx_ExecuteLoad(SPI_HandleTypeDef *phspi, uint16_t pageAddr)
{
   int32_t ret = W25N01GVXX_OK;
   uint8_t stat_reg;

   /* Check parameters */
   if (phspi == NULL )
   {
     return W25N01GVXX_ERROR;
   }

   /* Initialize the program execute instruction */
   uint8_t cmd[4] = {W25N01GVXX_CMD_EXECUTE, W25N01GVXX_DUMMY_BYTE, (uint8_t) ((pageAddr & 0xFF00) >> 8), (uint8_t) ((pageAddr) & 0xFF)};

   /* Set write enable */
   if((w25n01gvxx_WriteEnable(phspi)) != W25N01GVXX_OK )
   {
     /* Failed to set the write enable */
     ret = W25N01GVXX_ERROR;
   }

   /* Select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Select()) != W25N01GVXX_OK)
   {
     /* Failed to select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Send the program execute instruction via SPI */
   if((HAL_SPI_Transmit(phspi, cmd, 4, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to send the command */
     ret = W25N01GVXX_ERROR;
   }

   /* De-select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Deselect()) != W25N01GVXX_OK)
   {
     /* Failed to de-select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Program execute time */
   /* Wait for BUSY bit in the status register to bet cleared */
   do
   {
     /* Read the status register value  */
   if (w25n01gvxx_ReadStatusRegister(phspi, &stat_reg) != W25N01GVXX_OK)
   {
     /* Failed to read status register */
     return W25N01GVXX_ERROR;
   }

   }while((stat_reg & W25N01GVXX_RS3_S0_BUSY) == W25N01GVXX_RS3_S0_BUSY);

   /* Return PART status */
   return ret;
}


/**
 * @brief  Load the program data into the w25n01gvxx Data Buffer (caching data)
 * @param  phspi                 SPI handle pointer
 * @param  column_addr           The offset we want to point to it into a single page
 * @param  data                  Pointer to source buffer to be loaded.
 * @param  size                  Size of data to load.
 * @retval W25n01xx flash memory Status
 */
static int32_t w25n01gvxx_LoadData(SPI_HandleTypeDef *phspi, uint16_t column_addr, uint8_t *data, uint16_t size)
{
   int32_t ret = W25N01GVXX_OK;

   /* Check parameters */
   if (phspi == NULL || data == NULL || column_addr > W25N01GVXX_PAGE_SIZE2)
   {
     return W25N01GVXX_ERROR;
   }

   /* Initialize the program data load instruction */
   uint8_t cmd[3] = {W25N01GVXX_CMD_LOAD_DATA, (column_addr >> 8) & 0xFF, (column_addr) & 0xFF};

   /* Set write enable */
   if((w25n01gvxx_WriteEnable(phspi)) != W25N01GVXX_OK )
   {
     /* Failed to set the write enable */
     ret = W25N01GVXX_ERROR;
   }

   /* Select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Select()) != W25N01GVXX_OK)
   {
     /* Failed to select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Send the program data load instruction via SPI */
   if((HAL_SPI_Transmit(phspi, cmd, 3, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to send command */
     ret = W25N01GVXX_ERROR;
   }

   /* Send the data we want to write via SPI */
   if((HAL_SPI_Transmit(phspi, data, size, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to send data buffer */
     ret = W25N01GVXX_ERROR;
   }

   /* De-select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Deselect()) != W25N01GVXX_OK)
   {
     /* Failed to de-select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Return PART status */
   return ret;
}


/**
 * @brief  Writes an amount of data into a memory page
 * @param  phspi                 SPI handle pointer
 * @param  block                 Block number that wa want to write into it
 * @param  page                  Page number that we want to write
 * @param  data                  Pointer to source buffer
 * @param  size                  Size of data to write.
 * @retval W25n01xx flash memory Status
 */
int32_t w25n01gvxx_WriteData(SPI_HandleTypeDef *phspi, uint16_t block, uint8_t page, uint8_t *data, uint32_t size)
{
   int32_t ret = W25N01GVXX_OK;

   /* Check parameters */
   if (phspi == NULL || data == NULL || block >= W25N01GVXX_NUM_OF_BLOCK || page >= W25N01GVXX_NUM_PAGE_PER_BLOCK)
   {
     return W25N01GVXX_ERROR;
   }

   /* Page address calculation */
   uint32_t mem_addr = (block * W25N01GVXX_NUM_PAGE_PER_BLOCK) + page;

   /* Load the program data into the w25n01gvxx Data Buffer (caching data) */
   if ((w25n01gvxx_LoadData(phspi, 0, data, size)) != W25N01GVXX_OK)
   {
     /* If data load failed */
     ret = W25N01GVXX_ERROR;
   }

   /* Program the Data Buffer content into the physical memory page */
   if((w25n01gvxx_ExecuteLoad(phspi,  mem_addr)) != W25N01GVXX_OK)
   {
     /* If program execute failed */
     ret = W25N01GVXX_ERROR;
   }

   /* Return PART status */
   return ret;
}

/**
 * @brief  allows one or more data bytes to be sequentially read from the Data Buffer (cache)
 * @param  phspi                 SPI handle pointer
 * @param  column_addr           The offset we want to point to it
 * @param  pBuffer               Pointer to destination read buffer
 * @param  size                  Size of data to read
 * @retval W25n01xx flash memory Status
 */
static int32_t w25n01gvxx_ReadData(SPI_HandleTypeDef *phspi, uint16_t column_addr, uint8_t *pBuffer, uint16_t size)
{
   int32_t ret = W25N01GVXX_OK;

   /* Check parameters */
   if (phspi == NULL || column_addr > W25N01GVXX_PAGE_SIZE2)
   {
     return W25N01GVXX_ERROR;
   }

   /* Initialize the read data instruction */
   uint8_t cmd[4] = {W25N01GVXX_CMD_READ_DATA, (column_addr >> 8) & 0xFF, column_addr & 0xFF, 0x00};

   /* Select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Select()) != W25N01GVXX_OK)
   {
     /* Failed to select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Send the read data command via SPI */
   if((HAL_SPI_Transmit(phspi, cmd, 4, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to send the command */
     ret = W25N01GVXX_ERROR;
   }

   /* Receive the data we want to read from the cache via SPI */
   if((HAL_SPI_Receive(phspi, pBuffer, size, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to receive the data */
     ret = W25N01GVXX_ERROR;
   }

   /* De-select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Deselect()) != W25N01GVXX_OK)
   {
     /* Failed to de-select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Return PART status */
   return ret;
}


/**
 * @brief  transfer the data of the specified memory page into the 2,112-Byte Data Buffer.
 * @param  phspi                 SPI handle pointer
 * @param  page_addr             Page address that we want to read
 * @retval W25n01xx flash memory Status
 */
static int32_t w25n01gvxx_Read(SPI_HandleTypeDef *phspi, uint16_t page_addr)
{
   int32_t ret = W25N01GVXX_OK;
   uint8_t stat_reg;
   /* Check parameters */
   if (phspi == NULL)
   {
     return W25N01GVXX_ERROR;
   }

   /* Initialize the page data read instruction */
   uint8_t cmd_buf_send[4] = { W25N01GVXX_CMD_PAGE_READ_DATA, W25N01GVXX_DUMMY_BYTE, (page_addr >> 8) & 0xFF, page_addr & 0xFF};

   /* Select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Select()) != W25N01GVXX_OK)
   {
     /* Failed to select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Send the page data read instruction via SPI to load the wanted data to cache before read it */
   if((HAL_SPI_Transmit(phspi, cmd_buf_send, 4, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to send the command */
     ret = W25N01GVXX_ERROR;
   }

   /* De-select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Deselect()) != W25N01GVXX_OK)
   {
     /* Failed to select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Read time */
   /* Wait for BUSY bit in the status register to bet cleared */
   do
   {
     /* Read the status register value  */
   if (w25n01gvxx_ReadStatusRegister(phspi, &stat_reg) != W25N01GVXX_OK)
   {
     /* Failed to read status register */
     return W25N01GVXX_ERROR;
   }

   }while((stat_reg & W25N01GVXX_RS3_S0_BUSY) == W25N01GVXX_RS3_S0_BUSY);


   /* Return PART status */
   return ret;
}


/**
 * @brief  allows one or more data bytes to be sequentially read from the w25n01gvxx physical memory
 * @param  phspi                 SPI handle pointer
 * @param  block                 Block number that wa want to read from it
 * @param  page                  Page number that we want to read
 * @param  pBuffer               Pointer to destination read buffer
 * @param  size                  Size of data to read
 * @retval W25n01xx flash memory Status
 */
int32_t w25n01gvxx_ReadPage(SPI_HandleTypeDef *phspi, uint16_t block, uint8_t page, uint8_t *pBuffer, uint32_t size)
{
   int32_t ret = W25N01GVXX_OK;

   /* Check parameters */
   if (phspi == NULL || pBuffer == NULL || block >= W25N01GVXX_NUM_OF_BLOCK || page >= W25N01GVXX_NUM_PAGE_PER_BLOCK)
   {
     return W25N01GVXX_ERROR;
   }

   /* Page address calculation */
   uint32_t mem_addr = (block * W25N01GVXX_NUM_PAGE_PER_BLOCK) + page;

   /* Load the data wa want to read into the w25n01gvxx Data Buffer (caching data) */
   if((w25n01gvxx_Read(phspi,  mem_addr)) != W25N01GVXX_OK)
   {
     ret = W25N01GVXX_ERROR;
   }
   /* Read the data from Data Buffer (caching data) */
   if((w25n01gvxx_ReadData(phspi, 0, pBuffer, size)) != W25N01GVXX_OK)
   {
     ret = W25N01GVXX_ERROR;
   }

   /* Return PART status */
   return ret;

}


/**
 * @brief  Reads the w25n01xx PROTECTION register only
 * @param  phspi                 SPI handle pointer
 * @param  pValue                Pointer to PROTECTION register value
 * @retval W25n01xx flash memory Status
 */
int32_t w25n01gvxx_ReadProtectionRegister(SPI_HandleTypeDef *phspi, uint8_t *pValue)
{
   int32_t ret =  W25N01GVXX_OK;

   /* Prepare the command sequence for reading the PROTECTION register */
   uint8_t tData[3];
   tData[0] = W25N01GVXX_CMD_READ_REG_STATUS2;
   tData[1] = W25N01GVXX_REG_STATUS_1;

   /* Select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Select()) != W25N01GVXX_OK)
   {
     /* Failed to select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Send the read Prot Reg command via SPI */
   if((HAL_SPI_Transmit(phspi, tData, 2, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to send the command */
     ret = W25N01GVXX_ERROR;
   }

   /* Receive the Prot reg value via SPI */
   if((HAL_SPI_Receive(phspi, pValue, 1, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to receive the register value */
     ret = W25N01GVXX_ERROR;
   }

   if((w25n01gvxx_Deselect()) != W25N01GVXX_OK)
   {
     /* Failed to de-select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Return PART status */
   return ret;
}


/**
 * @brief  Reads the w25n01xx CONFIGURATION register only
 * @param  phspi                 SPI handle pointer
 * @param  pValue                Pointer to CONFIGURATION register value
 * @retval W25n01xx flash memory Status
 */
int32_t w25n01gvxx_ReadConfigRegister(SPI_HandleTypeDef *phspi, uint8_t *pValue)
{
   int32_t ret =  W25N01GVXX_OK;

   /* Prepare the command sequence for reading the CONFIGURATION register */
   uint8_t tData[3];
   tData[0] = W25N01GVXX_CMD_READ_REG_STATUS1;
   tData[1] = W25N01GVXX_REG_STATUS_2;

   /* Select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Select()) != W25N01GVXX_OK)
   {
     /* Failed to select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Send the read Config Reg command via SPI */
   if((HAL_SPI_Transmit(phspi, tData, 2, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to send the command */
     ret = W25N01GVXX_ERROR;
   }

   /* Receive the Config reg value via SPI */
   if((HAL_SPI_Receive(phspi, pValue, 1, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to receive the register value */
     ret = W25N01GVXX_ERROR;
   }

   /* De-select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Deselect()) != W25N01GVXX_OK)
   {
      /* Failed to de-select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Return PART status */
   return ret;
}


/**
 * @brief  Reads the w25n01xx STATUS register only
 * @param  phspi                 SPI handle pointer
 * @param  pValue                Pointer to STATUS register value
 * @retval W25n01xx flash memory Status
 */
int32_t w25n01gvxx_ReadStatusRegister(SPI_HandleTypeDef *phspi, uint8_t *pValue)
{
   int32_t ret =  W25N01GVXX_OK;

   /* Check parameters */
   if (phspi == NULL || pValue == NULL)
   {
     return W25N01GVXX_ERROR;
   }

   /* Prepare the command sequence for reading the STATUS register */
   uint8_t cmd[2];
   cmd[0] = W25N01GVXX_CMD_READ_REG_STATUS2;
   cmd[1] = W25N01GVXX_REG_STATUS_3;

   /* Select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Select()) != W25N01GVXX_OK)
   {
     /* Failed to select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Send the read Status Reg command via SPI */
   if((HAL_SPI_Transmit(phspi, cmd, 2, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to send the command */
     ret = W25N01GVXX_ERROR;
   }

   /* Receive the Status reg value via SPI */
   if((HAL_SPI_Receive(phspi, pValue, 1, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to receive the register value */
     ret = W25N01GVXX_ERROR;
   }

   /* De-select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Deselect()) != W25N01GVXX_OK)
   {
      /* Failed to de-select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Return PART status */
   return ret;
}


/**
 * @brief  Writes the w25n01xx PROTECTION register
 * @param  phspi                 SPI handle pointer
 * @param  pValue                The 8-bits data we want to write into the PROTECTION register
 * @retval W25n01xx flash memory Status
 */
int32_t w25n01gvxx_WriteProtectionRegister(SPI_HandleTypeDef *phspi, uint8_t pValue)
{

   int32_t ret =  W25N01GVXX_OK;

   /* Check parameters */
   if (phspi == NULL)
   {
     return W25N01GVXX_ERROR;
   }

   /* Prepare the command sequence for writing the PROTECTION register */
   uint8_t cmd[3] = {W25N01GVXX_CMD_WRITE_REG_STATUS2, W25N01GVXX_REG_STATUS_1, pValue};

   /* Select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Select()) != W25N01GVXX_OK)
   {
     /* Failed to select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Send the command sequence via SPI */
   if((HAL_SPI_Transmit(phspi, cmd, 3, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to send the command sequence */
     ret = W25N01GVXX_ERROR;
   }

   /* De-select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Deselect()) != W25N01GVXX_OK)
   {
      /* Failed to de-select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Return PART status */
   return ret;
}


/**
 * @brief  Writes the w25n01xx CONFIGURATION register
 * @param  phspi                 SPI handle pointer
 * @param  pValue                Pointer to The 8-bits data we want to write into the CONFIGURATION register
 * @retval W25n01xx flash memory Status
 */
int32_t w25n01gvxx_WriteConfigRegister(SPI_HandleTypeDef *phspi, uint8_t pValue)
{
   int32_t ret =  W25N01GVXX_OK;

   /* Check parameters */
   if (phspi == NULL)
   {
     return W25N01GVXX_ERROR;
   }

   /* Prepare the command sequence for writing the CONFIGURATION register */
   uint8_t cmd[3] = {W25N01GVXX_CMD_WRITE_REG_STATUS2, W25N01GVXX_REG_STATUS_2, pValue};

   /* Select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Select()) != W25N01GVXX_OK)
   {
     /* Failed to select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Send the command sequence via SPI */
   if((HAL_SPI_Transmit(phspi, cmd, 3, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to send the command sequence */
     ret = W25N01GVXX_ERROR;
   }

   /* De-select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Deselect()) != W25N01GVXX_OK)
   {
      /* Failed to de-select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Return PART status */
   return ret;
}


/**
 * @brief  Reads the content of the spare area sector
 * @param  phspi                 SPI handle pointer
 * @param  block                 Block number that wa want to read from it
 * @param  page                  Page number that we want to read
 * @param  pBuffer               Pointer to destination buffer for the spare sector.
 * @retval W25n01xx flash memory Status
 */
int32_t w25n01gvxx_ReadSpareArea(SPI_HandleTypeDef *phspi, uint16_t block, uint8_t page, uint8_t sector, uint8_t *pBuffer)
{
   int32_t ret =  W25N01GVXX_OK;

   /* Check parameters */
   if (phspi == NULL || pBuffer == NULL)
   {
     return W25N01GVXX_ERROR;
   }

   uint32_t page_addr = (block * W25N01GVXX_NUM_PAGE_PER_BLOCK) + page;

   /* Load the page we want to read from it into cache buffer */
   if ((w25n01gvxx_Read(phspi, page_addr)) != W25N01GVXX_OK)
   {
   /* Load operation failed  */
     ret = W25N01GVXX_ERROR;
   }

   /* Read the spare sector that we want from the cache previously loaded */
   if ((w25n01gvxx_ReadData(phspi, W25N01GVXX_PAGE_MEM_SIZE + (sector*W25N01GVXX_SPARE_SECTOR_SIZE), pBuffer, W25N01GVXX_PAGE_SPARE_SIZE)) != W25N01GVXX_OK)
   {
     /* Read operation failed */
     ret = W25N01GVXX_ERROR;
   }

   /* Return PART status */
   return ret;
}


/**
 * @brief  Writes data into spare area sector
 * @param  phspi                 SPI handle pointer
 * @param  block                 Block number that wa want to read from it
 * @param  page                  Page number that we want to read
 * @param  pBuffer               Pointer to source buffer to write
 * @retval W25n01xx flash memory Status
 */
int32_t w25n01gvxx_WriteSpareArea(SPI_HandleTypeDef *phspi, uint16_t block, uint8_t page, uint8_t sector, uint8_t *pBuffer)
{
   int32_t ret =  W25N01GVXX_OK;

   /* Check parameters */
   if (phspi == NULL)
   {
     return W25N01GVXX_ERROR;
   }

   uint32_t page_addr = (block * W25N01GVXX_NUM_PAGE_PER_BLOCK) + page;

   /* Load the data we want to write to spare sector into memory cache buffer */
   if ((w25n01gvxx_LoadData(phspi, W25N01GVXX_PAGE_MEM_SIZE + (sector*W25N01GVXX_SPARE_SECTOR_SIZE) , pBuffer, W25N01GVXX_PAGE_SPARE_SIZE)) != W25N01GVXX_OK)
   {
     /* Load operation failed */
     ret = W25N01GVXX_ERROR;
   }

   /* Program the Data Buffer content into the physical memory spare sector */
   if ((w25n01gvxx_ExecuteLoad(phspi, page_addr)) != W25N01GVXX_OK)
   {
     /* Execute operation failed */
     ret = W25N01GVXX_ERROR;
   }

   /* Return PART status */
   return ret;
}


/**
 * @brief  Erase a block from the w25n01xx NAND flash memory
 * @param  phspi                 SPI handle pointer
 * @param  block                 Block number we want to erase
 * @retval W25n01xx flash memory Status
 */
int32_t w25n01gvxx_EraseBlock(SPI_HandleTypeDef *phspi, uint16_t block)
{
   int32_t ret =  W25N01GVXX_OK;
   uint8_t stat_reg;
   /* Check parameters */
   if (phspi == NULL)
   {
     return W25N01GVXX_ERROR;
   }

   /* Page address calculation to get the block address we want to erase */
   uint16_t page_addr = block * W25N01GVXX_NUM_PAGE_PER_BLOCK ;

   /* Initialize the erase block instructions sequence */
   uint8_t cmd[4] = {W25N01GVXX_CMD_BLOCK_ERASE, W25N01GVXX_DUMMY_BYTE, (uint8_t) ((page_addr & 0xFF00) >> 8), (uint8_t) (page_addr & 0xC0)};

   /* Set write enable */
   if((w25n01gvxx_WriteEnable(phspi)) != W25N01GVXX_OK )
   {
     /* Failed set the write enable */
     ret = W25N01GVXX_ERROR;
   }

   /* Select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Select()) != W25N01GVXX_OK)
   {
     /* Failed to select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Send the erase block instructions sequence via SPI */
   if((HAL_SPI_Transmit(phspi, cmd, 4, HAL_MAX_DELAY)) != HAL_OK)
   {
     /* Failed to send the command sequence */
     ret = W25N01GVXX_ERROR;
   }

   /* Select the w25n01gvxx flash memory  */
   if((w25n01gvxx_Deselect()) != W25N01GVXX_OK)
   {
      /* Failed to de-select w25n01gvxx flash memory  */
     ret = W25N01GVXX_ERROR;
   }

   /* Erase block time */
   do
   {

   if (w25n01gvxx_ReadStatusRegister(phspi, &stat_reg) != W25N01GVXX_OK)
   {
     return W25N01GVXX_ERROR;
   }

   }while((stat_reg & W25N01GVXX_RS3_S0_BUSY) == W25N01GVXX_RS3_S0_BUSY);

   /* Return PART status */
   return ret;
}


/**
 * @brief  Erase full w25n01gvxx chip
 * @param  phspi                 SPI handle pointer
 * @retval W25n01xx flash memory Status
 */
int32_t w25n01gvxx_FullChipErase(SPI_HandleTypeDef *phspi)
{
   int32_t ret =  W25N01GVXX_OK;

   /* Check parameters */
   if (phspi == NULL)
   {
     return W25N01GVXX_ERROR;
   }

   /* Erase the total number of blocks in the w25n01gvxx nand flash memory */
   for (uint16_t i = 0; i < W25N01GVXX_NUM_OF_BLOCK; i++)
   {
     if ((w25n01gvxx_EraseBlock(phspi, i) != W25N01GVXX_OK))
     {
       /* Failed to erase block */
       return W25N01GVXX_ERROR;
     }
   }
   /* Return PART status */
   return ret;
}


/**
 * @brief  Copy the content of a source page into a destination page
 * @param  phspi                 SPI handle pointer
 * @param  src_block             The source block we want to copy from it
 * @param  src_page              The source page we want to copy from it
 * @param  dest_block            The destination block we want to copy into
 * @param  dest_page             The destination page we want to copy into
 * @param  size                  The size of the data wa want to copy
 * @retval W25n01xx flash memory Status
 */

int32_t w25n01gvxx_PagesCopy(SPI_HandleTypeDef *phspi, uint16_t src_block, uint8_t src_page, uint16_t dest_block, uint8_t dest_page, uint32_t size)
{
   int32_t ret =  W25N01GVXX_OK;

   /* Check parameters */
   if (phspi == NULL)
   {
     return W25N01GVXX_ERROR;
   }

   /* Source page address calculation */
   uint32_t src_mem_addr = (src_block * W25N01GVXX_NUM_PAGE_PER_BLOCK) + src_page;

   /* Source page address calculation */
   uint32_t dest_mem_addr = (dest_block * W25N01GVXX_NUM_PAGE_PER_BLOCK) + dest_page;

   /* Load the data wa want to read into the w25n01gvxx Data Buffer (caching data) */
   if((w25n01gvxx_Read(phspi,  src_mem_addr)) != W25N01GVXX_OK)
   {
     ret = W25N01GVXX_ERROR;
   }

   /* Program the Data Buffer content into the physical memory destination  page */
   if((w25n01gvxx_ExecuteLoad(phspi,  dest_mem_addr)) != W25N01GVXX_OK)
   {
     /* If program execute failed */
     ret = W25N01GVXX_ERROR;
   }

   /* Return PART status */
   return ret;
}