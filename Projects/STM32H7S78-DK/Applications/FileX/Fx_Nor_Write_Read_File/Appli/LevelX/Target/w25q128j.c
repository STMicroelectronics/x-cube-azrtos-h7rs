/**
  ******************************************************************************
  * @file    w25q128j.c
  * @brief   This file provides the w25q128j SPI drivers.
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

/* Includes ------------------------------------------------------------------*/
#include "w25q128j.h"

static w25q128j_status_t w25q128j_select(void);
static w25q128j_status_t w25q128j_deselect(void);
static w25q128j_status_t w25q128j_send_cmd(SPI_HandleTypeDef *phspi, uint8_t *p_data, uint32_t size);
static w25q128j_status_t w25q128j_receive_cmd(SPI_HandleTypeDef *phspi, uint8_t *p_cmd, uint8_t cmd_len_byte,
                                              uint8_t *p_data, uint32_t data_len_byte);

/**
  * @brief  Initializes the memory
  * @param  phspi     SPI handle pointer
  * @retval error status
  */
w25q128j_status_t w25q128j_Init(SPI_HandleTypeDef *phspi)
{
  w25q128j_status_t ret = W25Q128J_OK;

  if (w25q128j_WriteEnable(phspi) != W25Q128J_OK)
  {
    ret = W25Q128J_ERROR;
  }
  else if (w25q128j_Reset(phspi) != W25Q128J_OK)
  {
    ret = W25Q128J_ERROR;
  }
  else
  {
  }

  return ret;
}

/**
  * @brief  Resets the flash memory
  * @param  phspi     SPI handle pointer
  * @retval error status
  */
w25q128j_status_t w25q128j_Reset(SPI_HandleTypeDef *phspi)
{
  w25q128j_status_t ret = W25Q128J_OK;
  uint8_t stat_reg = 0;

  /* Initialize the enable reset command */
  uint8_t tData = W25Q128J_RESET_ENABLE_CMD;
  /* Check the BUSY bit in the status register */
  do
  {
    if (w25q128j_ReadStatusRegister(phspi, &stat_reg) != W25Q128J_OK)
    {
      return W25Q128J_ERROR;
    }
  } while ((stat_reg & W25Q128J_SR1_BUSY) == W25Q128J_SR1_BUSY);

  /* Send the reset enable command via SPI */
  if ((w25q128j_send_cmd(phspi, &tData, W25Q128J_RESET_ENABLE_CMD_SIZE)) != W25Q128J_OK)
  {
    /* Failed to send command */
    ret = W25Q128J_ERROR;
  }

  /* Initialize the reset command */
  tData = W25Q128J_RESET_MEMORY_CMD;

  /* Send the reset command via SPI */
  if ((w25q128j_send_cmd(phspi, &tData, W25Q128J_RESET_CMD_SIZE)) != W25Q128J_OK)
  {
    /* Failed to send command */
    ret = W25Q128J_ERROR;
  }

  /* Return PART status */
  return ret;
}

/**
  * @brief  Sets the Write Enable Latch (WEL) bit in the Status Register to a 1.
  * @param  phspi   SPI handle pointer
  * @retval error status
  */
w25q128j_status_t w25q128j_WriteEnable(SPI_HandleTypeDef *phspi)
{
  w25q128j_status_t ret = W25Q128J_OK ;

  /* Initialize the write enable command */
  uint8_t cmd = W25Q128J_WRITE_ENABLE_CMD;

  /* Send the write enable command via SPI */
  if ((w25q128j_send_cmd(phspi, &cmd, W25Q128J_WRITE_ENABLE_CMD_SIZE)) != W25Q128J_OK)
  {
    /* Failed to send command */
    ret = W25Q128J_ERROR;
  }

  /* Return PART status */
  return ret;
}

/**
  * @brief  Reads the w25q128j status register1
  * @param  phspi                 SPI handle pointer
  * @param  p_value               : Pointer to status register1 value
  * @retval error status
  */
w25q128j_status_t w25q128j_ReadStatusRegister(SPI_HandleTypeDef *phspi, uint8_t *p_value)
{
  w25q128j_status_t ret =  W25Q128J_OK;
  /* Check parameters */
  if (p_value == NULL)
  {
    return W25Q128J_ERROR;
  }
  /* Prepare the command sequence for reading the STATUS register */
  uint8_t cmd = W25Q128J_READ_STATUS_REG1_CMD;

  /* Send the read Status Reg command via SPI */
  if ((w25q128j_receive_cmd(phspi, &cmd, W25Q128J_READ_STATUS_REG_CMD_SIZE,
                            p_value, W25Q128J_STATUS_REG_DATA_SIZE)) != W25Q128J_OK)
  {
    /* Failed to send the command */
    ret = W25Q128J_ERROR;
  }

  /* Return PART status */
  return ret;
}

/**
  * @brief  Erase a 64K block from the w25q128j NOR flash memory
  * @param  phspi                 SPI handle pointer
  * @param  block_address         : Block address
  * @retval error status
  */
static w25q128j_status_t w25q128j_erase_64k_block_cmd(SPI_HandleTypeDef *phspi, uint32_t block_address)
{
  w25q128j_status_t ret =  W25Q128J_OK;
  uint8_t stat_reg;

  /* Initialize the erase block instructions sequence */
  uint8_t cmd[4] = {W25Q128J_BLOCK_ERASE_64K_CMD, (uint8_t)((block_address & W25Q128J_MEM_ADDR_MASK1) >> 16),
                    (uint8_t)((block_address & W25Q128J_MEM_ADDR_MASK2) >> 8),
                    (uint8_t)(block_address & W25Q128J_MEM_ADDR_MASK3)
                   };

  /* make sure the memory is ready */
  do
  {
    if (w25q128j_ReadStatusRegister(phspi, &stat_reg) != W25Q128J_OK)
    {
      return W25Q128J_ERROR;
    }
  } while ((stat_reg & W25Q128J_SR1_BUSY) == W25Q128J_SR1_BUSY);

  /* Set write enable */
  if ((w25q128j_WriteEnable(phspi)) != W25Q128J_OK)
  {
    /* Failed set the write enable */
    ret = W25Q128J_ERROR;
  }

  /* Send the erase block instructions sequence via SPI */
  if ((w25q128j_send_cmd(phspi, cmd, W25Q128J_BLOCK_ERASE_CMD_SIZE)) != W25Q128J_OK)
  {
    /* Failed to send the command sequence */
    ret = W25Q128J_ERROR;
  }
  /* Return PART status */
  return ret;
}


/**
  * @brief  Erase a 32K block from the w25q128j NOR flash memory
  * @param  phspi                 SPI handle pointer
  * @param  block_address         : Block address
  * @retval error status
  */
static w25q128j_status_t w25q128j_erase_32k_block_cmd(SPI_HandleTypeDef *phspi, uint32_t block_address)
{
  w25q128j_status_t ret =  W25Q128J_OK;
  uint8_t stat_reg;

  /* Initialize the erase block instructions sequence */
  uint8_t cmd[4] = {W25Q128J_BLOCK_ERASE_32K_CMD, (uint8_t)((block_address & W25Q128J_MEM_ADDR_MASK1) >> 16),
                    (uint8_t)((block_address & W25Q128J_MEM_ADDR_MASK2) >> 8),
                    (uint8_t)(block_address & W25Q128J_MEM_ADDR_MASK3)
                   };

  /* make sure the memory is ready */
  do
  {
    if (w25q128j_ReadStatusRegister(phspi, &stat_reg) != W25Q128J_OK)
    {
      return W25Q128J_ERROR;
    }
  } while ((stat_reg & W25Q128J_SR1_BUSY) == W25Q128J_SR1_BUSY);

  /* Set write enable */
  if ((w25q128j_WriteEnable(phspi)) != W25Q128J_OK)
  {
    /* Failed set the write enable */
    ret = W25Q128J_ERROR;
  }

  /* Send the erase block instructions sequence via SPI */
  if ((w25q128j_send_cmd(phspi, cmd, W25Q128J_BLOCK_ERASE_CMD_SIZE)) != W25Q128J_OK)
  {
    /* Failed to send the command sequence */
    ret = W25Q128J_ERROR;
  }

  /* Erase block time */
  do
  {
    if (w25q128j_ReadStatusRegister(phspi, &stat_reg) != W25Q128J_OK)
    {
      return W25Q128J_ERROR;
    }
  } while ((stat_reg & W25Q128J_SR1_BUSY) == W25Q128J_SR1_BUSY);
  /* Return PART status */
  return ret;
}

/**
  * @brief  Erase a 64K block from the w25q128j NOR flash memory
  * @param  phspi                 SPI handle pointer
  * @param  sector_address        : Sector address
  * @retval error status
  */
static w25q128j_status_t w25q128j_erase_sector_cmd(SPI_HandleTypeDef *phspi, uint32_t sector_address)
{
  w25q128j_status_t ret =  W25Q128J_OK;
  uint8_t stat_reg;

  /* Initialize the erase block instructions sequence */
  uint8_t cmd[4] = {W25Q128J_SECTOR_ERASE_CMD, (uint8_t)((sector_address & W25Q128J_MEM_ADDR_MASK1) >> 16),
                    (uint8_t)((sector_address & W25Q128J_MEM_ADDR_MASK2) >> 8),
                    (uint8_t)(sector_address & W25Q128J_MEM_ADDR_MASK3)
                   };

  /* make sure the memory is ready */
  do
  {
    if (w25q128j_ReadStatusRegister(phspi, &stat_reg) != W25Q128J_OK)
    {
      return W25Q128J_ERROR;
    }
  } while ((stat_reg & W25Q128J_SR1_BUSY) == W25Q128J_SR1_BUSY);

  /* Set write enable */
  if ((w25q128j_WriteEnable(phspi)) != W25Q128J_OK)
  {
    /* Failed set the write enable */
    ret = W25Q128J_ERROR;
  }

  /* Send the erase block instructions sequence via SPI */
  if ((w25q128j_send_cmd(phspi, cmd, W25Q128J_BLOCK_ERASE_CMD_SIZE)) != W25Q128J_OK)
  {
    /* Failed to send the command sequence */
    ret = W25Q128J_ERROR;
  }

  /* Erase block time */
  do
  {
    if (w25q128j_ReadStatusRegister(phspi, &stat_reg) != W25Q128J_OK)
    {
      return W25Q128J_ERROR;
    }
  } while ((stat_reg & W25Q128J_SR1_BUSY) == W25Q128J_SR1_BUSY);
  /* Return PART status */
  return ret;
}

/**
  * @brief  Erases the specified block of flash memory.
  *         W25Q128J supports 4K sector or 32K block or 64K erase commands.
  * @param  phspi      SPI handle pointer
  * @param  addr       : Sector/block Address to erase
  * @param  size       : size configuration W25Q128J_ERASE_SECTOR (4K) or W25Q128J_ERASE_32K_BLOCK (32K)
  *                      or  W25Q128J_ERASE_64K_BLOCK (64K)
  * @retval error status.
  */
w25q128j_status_t w25q128j_Erase(SPI_HandleTypeDef *phspi, uint32_t addr, w25q128j_erase_t size)
{

  /* Send the Sector/Block erase command */
  if (size == W25Q128J_ERASE_SECTOR)
  {
    if (w25q128j_erase_sector_cmd(phspi, addr) != W25Q128J_OK)
    {
      return W25Q128J_ERROR;
    }
  }
  else if (size == W25Q128J_ERASE_32K_BLOCK)
  {
    if (w25q128j_erase_32k_block_cmd(phspi, addr) != W25Q128J_OK)
    {
      return W25Q128J_ERROR;
    }
  }
  else if (size == W25Q128J_ERASE_64K_BLOCK)
  {
    if (w25q128j_erase_64k_block_cmd(phspi, addr) != W25Q128J_OK)
    {
      return W25Q128J_ERROR;
    }
  }

  return W25Q128J_OK;
}

/**
  * @brief  allows one or more data bytes to be sequentially read from the w25q128j flash memory
  * @param  phspi                 SPI handle pointer
  * @param  p_data                : Pointer to read data
  * @param  read_addr             : Read start address
  * @param  size_byte             : Size of data to read
  * @retval  error status
  */
w25q128j_status_t w25q128j_Read(SPI_HandleTypeDef *phspi, uint8_t *p_data,
                                uint32_t read_addr, uint32_t size_byte)
{
  uint8_t stat_reg;
  w25q128j_status_t ret = W25Q128J_OK;

  /* Check parameters */
  if (p_data == NULL)
  {
    return W25Q128J_ERROR;
  }
  /* Check that the memory is ready */
  do
  {
    if (w25q128j_ReadStatusRegister(phspi, &stat_reg) != W25Q128J_OK)
    {
      return W25Q128J_ERROR;
    }
  } while ((stat_reg & W25Q128J_SR1_BUSY) == W25Q128J_SR1_BUSY);

  /* Initialize the read data instruction */
  uint8_t cmd[4] = {W25Q128J_READ_DATA_CMD, (uint8_t)((read_addr & W25Q128J_MEM_ADDR_MASK1) >> 16),
                    (uint8_t)((read_addr & W25Q128J_MEM_ADDR_MASK2) >> 8),
                    (uint8_t)(read_addr & W25Q128J_MEM_ADDR_MASK3)
                   };

  /* Send the read data command via SPI */
  if ((w25q128j_receive_cmd(phspi, cmd, W25Q128J_READ_DATA_CMD_SIZE, p_data, size_byte)) != W25Q128J_OK)
  {
    /* Failed to send the command */
    ret = W25Q128J_ERROR;
  }

  /* Return PART status */
  return ret;
}

/**
  * @brief  allows from one byte to 256 bytes (a page) of data to be programmed at
  *          previously erased (FFh) memory locations.
  * @param  phspi                 SPI handle pointer
  * @param  write_addr            : Write start address.
  * @param  p_data                : Pointer to write data
  * @param  size_byte             : Size of data to write
  * @retval  error status
  */
static w25q128j_status_t w25q128j_page_program(SPI_HandleTypeDef *phspi, uint32_t write_addr,
                                               const uint8_t *p_data, uint32_t size_byte)
{
  w25q128j_status_t ret = W25Q128J_OK;
  uint8_t stat_reg;

  /* Check parameters */
  if (p_data == NULL)
  {
    return W25Q128J_ERROR;
  }

  /* Initialize the page program instruction */
  uint8_t cmd[4] = {W25Q128J_PAGE_PROG_CMD, (uint8_t)((write_addr & W25Q128J_MEM_ADDR_MASK1) >> 16),
                    (uint8_t)((write_addr & W25Q128J_MEM_ADDR_MASK2) >> 8),
                    (uint8_t)(write_addr & W25Q128J_MEM_ADDR_MASK3)
                   };

  /* Check that the memory is ready */
  do
  {
    if (w25q128j_ReadStatusRegister(phspi, &stat_reg) != W25Q128J_OK)
    {
      return W25Q128J_ERROR;
    }
  } while ((stat_reg & W25Q128J_SR1_BUSY) == W25Q128J_SR1_BUSY);

  /* Set write enable */
  if ((w25q128j_WriteEnable(phspi)) != W25Q128J_OK)
  {
    /* Failed to set the write enable */
    ret = W25Q128J_ERROR;
  }

  /* Select the w25q128j flash memory  */
  if ((w25q128j_select()) != W25Q128J_OK)
  {
    /* Failed to select w25q128j flash memory  */
    ret = W25Q128J_ERROR;
  }

  /* Send the program data load instruction via SPI */
  if ((HAL_SPI_Transmit(phspi, cmd, W25Q128J_PAGE_PROGRAM_CMD_SIZE, W25Q128J_SPI_POLL_TIMEOUT)) != HAL_OK)
  {
    /* Failed to send command */
    ret = W25Q128J_ERROR;
  }
  /* Send the data we want to write via SPI */
  if ((HAL_SPI_Transmit(phspi, p_data, size_byte, W25Q128J_SPI_POLL_TIMEOUT)) != HAL_OK)
  {
    /* Failed to send data buffer */
    ret = W25Q128J_ERROR;
  }

  /* De-select the w25q128j flash memory  */
  if ((w25q128j_deselect()) != W25Q128J_OK)
  {
    /* Failed to de-select w25q128j flash memory  */
    ret = W25Q128J_ERROR;
  }

  /* Wait for BUSY bit in the status register to bet cleared */
  do
  {
    if (w25q128j_ReadStatusRegister(phspi, &stat_reg) != W25Q128J_OK)
    {
      return W25Q128J_ERROR;
    }
  } while ((stat_reg & W25Q128J_SR1_BUSY) == W25Q128J_SR1_BUSY);
  /* Return PART status */
  return ret;
}

/**
  * @brief  Allows one or more data bytes to be sequentially written at previously erased (FFh) memory locations.
  * @param  phspi      SPI handle pointer
  * @param  p_data     : Pointer to data to be written.
  * @param  write_addr : Write start address.
  * @param  size_byte  : Size of data to write in bytes.
  * @retval error status
  */
w25q128j_status_t w25q128j_Write(SPI_HandleTypeDef *phspi, const uint8_t *p_data,
                                 uint32_t write_addr, uint32_t size_byte)
{
  w25q128j_status_t ret = W25Q128J_OK;
  uint32_t addr = write_addr;
  uint32_t write_index;

  if (size_byte == 0U)
  {
    /* nothing to write */
  }
  else
  {
    /* Calculate the maximum size of the first page */
    const uint32_t max_first_page = W25Q128J_PAGE_SIZE - (addr % W25Q128J_PAGE_SIZE);
    /* Amount of data in the first page */
    const uint32_t first_page = (max_first_page <= size_byte) ? max_first_page : size_byte;
    /* Number of full pages to write */
    const uint32_t full_pages = (size_byte - first_page) / W25Q128J_PAGE_SIZE;
    /* Size after the last full page */
    const uint32_t last_page = (size_byte - first_page) % W25Q128J_PAGE_SIZE;

    if (w25q128j_page_program(phspi, addr, p_data, first_page) != W25Q128J_OK)
    {
      ret = W25Q128J_ERROR;
    }
    else
    {
      write_index = first_page;
      addr += first_page;

      for (uint32_t ipage = 0; ipage < full_pages; ipage++)
      {
        /* Check if an error has occurred before writing to the next page */
        if (ret == W25Q128J_OK)
        {
          if (w25q128j_page_program(phspi, addr, &p_data[write_index],
                                    W25Q128J_PAGE_SIZE) != W25Q128J_OK)
          {
            ret = W25Q128J_ERROR;
          }
          write_index += W25Q128J_PAGE_SIZE;
          addr += W25Q128J_PAGE_SIZE;
        }
      }
      if (ret == W25Q128J_OK)
      {
        if (last_page != 0U)
        {
          if (w25q128j_page_program(phspi, addr, &p_data[write_index], last_page) != W25Q128J_OK)
          {
            ret = W25Q128J_ERROR;
          }
        }
      }
    }
  }
  return ret;
}

/**
  * @brief  Selects the W25Q128J flash memory by setting the CS line low.
  * @param  None
  * @retval error status
  */
static w25q128j_status_t w25q128j_select(void)
{
  w25q128j_status_t ret =  W25Q128J_OK;

  /* Set the CS pin LOW */
  HAL_GPIO_WritePin(W25Q128J_CS_PORT, W25Q128J_CS_PIN, W25Q128J_CS_PIN_RESET);

  /*  Verify if the CS pin is indeed LOW */
  if (HAL_GPIO_ReadPin(W25Q128J_CS_PORT, W25Q128J_CS_PIN) != W25Q128J_CS_PIN_RESET)
  {
    /* Failed to set CS pin LOW */
    ret =  W25Q128J_ERROR;
  }
  return ret;
}

/**
  * @brief  De-selects the W25Q128J flash memory by setting the CS pin HIGH.
  * @param  None
  * @retval error status
  */
static w25q128j_status_t w25q128j_deselect(void)
{
  w25q128j_status_t ret =  W25Q128J_OK;

  /* Set the CS pin HIGH */
  HAL_GPIO_WritePin(W25Q128J_CS_PORT, W25Q128J_CS_PIN, W25Q128J_CS_PIN_SET);

  /*  Verify if the CS pin is indeed HIGH */
  if (HAL_GPIO_ReadPin(W25Q128J_CS_PORT, W25Q128J_CS_PIN) != W25Q128J_CS_PIN_SET)
  {
    /* Failed to set CS pin HIGH */
    ret =  W25Q128J_ERROR;
  }
  /* Return PART status */
  return ret;
}

static w25q128j_status_t w25q128j_send_cmd(SPI_HandleTypeDef *phspi, uint8_t *p_data, uint32_t size)
{
  w25q128j_status_t ret =  W25Q128J_OK;

  /* Select the w25q128j flash memory  */
  if ((w25q128j_select()) != W25Q128J_OK)
  {
    /* Failed to select w25q128j flash memory  */
    ret = W25Q128J_ERROR;
  }

  if ((HAL_SPI_Transmit(phspi, p_data, size, W25Q128J_SPI_POLL_TIMEOUT)) != HAL_OK)
  {
    /* Failed to send command */
    ret = W25Q128J_ERROR;
  }

  /* Deselect the w25q128j flash memory  */
  if ((w25q128j_deselect()) != W25Q128J_OK)
  {
    /* Failed to deselect w25q128j flash memory  */
    ret = W25Q128J_ERROR;
  }
  /* Return PART status */
  return ret;
}

static w25q128j_status_t w25q128j_receive_cmd(SPI_HandleTypeDef *phspi, uint8_t *p_cmd, uint8_t cmd_len_byte,
                                              uint8_t *p_data, uint32_t data_len_byte)
{
  w25q128j_status_t ret =  W25Q128J_OK;

  /* Select the w25q128j flash memory  */
  if ((w25q128j_select()) != W25Q128J_OK)
  {
    /* Failed to select w25q128j flash memory  */
    ret = W25Q128J_ERROR;
  }

  if ((HAL_SPI_Transmit(phspi, p_cmd, cmd_len_byte, W25Q128J_SPI_POLL_TIMEOUT)) != HAL_OK)
  {
    /* Failed to send command */
    ret = W25Q128J_ERROR;
  }

  if ((HAL_SPI_Receive(phspi, p_data, data_len_byte, W25Q128J_SPI_POLL_TIMEOUT)) != HAL_OK)
  {
    /* Failed to send command */
    ret = W25Q128J_ERROR;
  }
  /* Deselect the w25q128j flash memory  */
  if ((w25q128j_deselect()) != W25Q128J_OK)
  {
    /* Failed to deselect w25q128j flash memory  */
    ret = W25Q128J_ERROR;
  }
  /* Return PART status */
  return ret;
}

