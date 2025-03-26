/**n
  ******************************************************************************
  * @file    stm32h7rsxx_nucleo.c
  * @author  MCD Application Team
  * @brief   This file includes a standard driver for the XSPI
  *          memories mounted on STM32H7RSXX_NUCLEO board.
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
  [..]
   (#) This driver is used to drive the MX25UW25645G Octal NOR
       external memories mounted on STM32H7RSXX_NUCLEO board.

   (#) This driver need specific component driver (MX25UW25645G) to be included with.

   (#) MX25UW25645G Initialization steps:
       (++) Initialize the OPSI external memory using the BSP_XSPI_NOR_Init() function. This
            function includes the MSP layer hardware resources initialization and the
            XSPI interface with the external memory.

   (#) MX25UW25645G Octal NOR memory operations
       (++) XSPI memory can be accessed with read/write operations once it is
            initialized.
            Read/write operation can be performed with AHB access using the functions
            BSP_XSPI_NOR_Read()/BSP_XSPI_NOR_Write().
       (++) The function BSP_XSPI_NOR_GetInfo() returns the configuration of the XSPI memory.
            (see the XSPI memory data sheet)
       (++) Perform erase block operation using the function BSP_XSPI_NOR_Erase_Block() and by
            specifying the block address. You can perform an erase operation of the whole
            chip by calling the function BSP_XSPI_NOR_Erase_Chip().
       (++) The function BSP_XSPI_NOR_GetStatus() returns the current status of the XSPI memory.
            (see the XSPI memory data sheet)
       (++) The memory access can be configured in memory-mapped mode with the call of
            function BSP_XSPI_NOR_EnableMemoryMapped(). To go back in indirect mode, the
            function BSP_XSPI_NOR_DisableMemoryMapped() should be used.
       (++) The erase operation can be suspend and resume with using functions
            BSP_XSPI_NOR_SuspendErase() and BSP_XSPI_NOR_ResumeErase()
       (++) It is possible to put the memory in deep power-down mode to reduce its consumption.
            For this, the function BSP_XSPI_NOR_EnterDeepPowerDown() should be called. To leave
            the deep power-down mode, the function BSP_XSPI_NOR_LeaveDeepPowerDown() should be called.
       (++) The function BSP_XSPI_NOR_ReadID() returns the identifier of the memory
            (see the XSPI memory data sheet)
       (++) The configuration of the interface between peripheral and memory is done by
            the function BSP_XSPI_NOR_ConfigFlash(), three modes are possible :
            - SPI : instruction, address and data on one line
            - STR OPI : instruction, address and data on eight lines with sampling on one edge of clock
            - DTR OPI : instruction, address and data on eight lines with sampling on both edgaes of clock

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32h7rsxx_nucleo_xspi.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H7RSXX_NUCLEO
  * @{
  */

/** @defgroup STM32H7RSXX_NUCLEO_XSPI XSPI
  * @{
  */

/* Exported variables --------------------------------------------------------*/
/** @addtogroup STM32H7RSXX_NUCLEO_XSPI_NOR_Exported_Variables
  * @{
  */
XSPI_HandleTypeDef hxspi_nor[XSPI_NOR_INSTANCES_NUMBER] = {0};
XSPI_NOR_Ctx_t Xspi_Nor_Ctx[XSPI_NOR_INSTANCES_NUMBER] = {{
    XSPI_ACCESS_NONE,
    MX25UW25645G_SPI_MODE,
    MX25UW25645G_STR_TRANSFER
  }
};
/**
  * @}
  */

/* Private constants --------------------------------------------------------*/
/** @defgroup STM32H7S78_DK_XSPI_NOR_Private_Constants XSPI_NOR Private Constants
  * @{
  */
#if (DUMMY_CYCLES_READ_OCTAL == 20U)
#define XSPI_NOR_MAX_FREQ 133000000U /* Fmax of memory is 133 MHz */
#elif (DUMMY_CYCLES_READ_OCTAL == 18U)
#define XSPI_NOR_MAX_FREQ 133000000U /* Fmax of memory is 133 MHz */
#elif (DUMMY_CYCLES_READ_OCTAL == 16U)
#define XSPI_NOR_MAX_FREQ 133000000U /* Fmax of memory is 133 MHz */
#elif (DUMMY_CYCLES_READ_OCTAL == 14U)
#define XSPI_NOR_MAX_FREQ 133000000U /* Fmax of memory is 133 MHz */
#elif (DUMMY_CYCLES_READ_OCTAL == 12U)
#define XSPI_NOR_MAX_FREQ 104000000U /* Fmax of memory is 104 MHz */
#elif (DUMMY_CYCLES_READ_OCTAL == 10U)
#define XSPI_NOR_MAX_FREQ 104000000U /* Fmax of memory is 104 MHz */
#elif (DUMMY_CYCLES_READ_OCTAL == 8U)
#define XSPI_NOR_MAX_FREQ 84000000U /* Fmax of memory is 84 MHz */
#elif (DUMMY_CYCLES_READ_OCTAL == 6U)
#define XSPI_NOR_MAX_FREQ 66000000U /* Fmax of memory is 66 MHz */
#endif /* DUMMY_CYCLES_READ_OCTAL */
/**
  * @}
  */
/* Private variables ---------------------------------------------------------*/
/** @defgroup STM32H7RSXX_NUCLEO_XSPI_NOR_Private_Variables XSPI_NOR Private Variables
  * @{
  */
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1)
static uint32_t XspiNor_IsMspCbValid[XSPI_NOR_INSTANCES_NUMBER] = {0};
#endif /* USE_HAL_XSPI_REGISTER_CALLBACKS */
/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/

/** @defgroup STM32H7RSXX_NUCLEO_XSPI_NOR_Private_Functions XSPI_NOR Private Functions
  * @{
  */
static void    XSPI_NOR_MspInit(XSPI_HandleTypeDef *hxspi);
static void    XSPI_NOR_MspDeInit(XSPI_HandleTypeDef *hxspi);
static int32_t XSPI_NOR_ResetMemory(uint32_t Instance);
static int32_t XSPI_NOR_EnterDOPIMode(uint32_t Instance);
static int32_t XSPI_NOR_EnterSOPIMode(uint32_t Instance);
static int32_t XSPI_NOR_ExitOPIMode(uint32_t Instance);
/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/

/** @addtogroup STM32H7RSXX_NUCLEO_XSPI_NOR_Exported_Functions
  * @{
  */

/**
  * @brief  Initializes the XSPI interface.
  * @param  Instance   XSPI Instance
  * @param  Init       XSPI Init structure
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_Init(uint32_t Instance, BSP_XSPI_NOR_Init_t *Init)
{
  int32_t ret;
  BSP_XSPI_NOR_Info_t pInfo;
  MX_XSPI_InitTypeDef xspi_init;
  uint32_t xspi_clk;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if the instance is already initialized */
    if (Xspi_Nor_Ctx[Instance].IsInitialized == XSPI_ACCESS_NONE)
    {
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 0)
      /* Msp XSPI initialization */
      XSPI_NOR_MspInit(&hxspi_nor[Instance]);
#else
      /* Register the XSPI MSP Callbacks */
      if (XspiNor_IsMspCbValid[Instance] == 0UL)
      {
        if (BSP_XSPI_NOR_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
        {
          return BSP_ERROR_PERIPH_FAILURE;
        }
      }
#endif /* USE_HAL_XSPI_REGISTER_CALLBACKS */

      /* Get Flash information of one memory */
      (void)MX25UW25645G_GetFlashInfo(&pInfo);

      /* Fill config structure */
      xspi_clk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_XSPI2);
      xspi_init.ClockPrescaler = (xspi_clk / XSPI_NOR_MAX_FREQ);
      if ((xspi_clk % XSPI_NOR_MAX_FREQ) == 0U)
      {
        xspi_init.ClockPrescaler = xspi_init.ClockPrescaler - 1U;
      }
      xspi_init.MemorySize     = (uint32_t)POSITION_VAL((uint32_t)pInfo.FlashSize);
      xspi_init.SampleShifting = HAL_XSPI_SAMPLE_SHIFT_NONE;
      xspi_init.TransferRate   = (uint32_t) Init->TransferRate;

      /* STM32 XSPI interface initialization */
      if (MX_XSPI_NOR_Init(&hxspi_nor[Instance], &xspi_init) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
      /* XSPI memory reset */
      else if (XSPI_NOR_ResetMemory(Instance) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      /* Check if memory is ready */
      else if (MX25UW25645G_AutoPollingMemReady(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                                Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      /* Configure the memory */
      else if (BSP_XSPI_NOR_ConfigFlash(Instance, Init->InterfaceMode, Init->TransferRate) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief  De-Initializes the XSPI interface.
  * @param  Instance   XSPI Instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if the instance is already initialized */
    if (Xspi_Nor_Ctx[Instance].IsInitialized != XSPI_ACCESS_NONE)
    {
      /* Disable Memory mapped mode */
      if (Xspi_Nor_Ctx[Instance].IsInitialized == XSPI_ACCESS_MMP)
      {
        if (BSP_XSPI_NOR_DisableMemoryMappedMode(Instance) != BSP_ERROR_NONE)
        {
          return BSP_ERROR_COMPONENT_FAILURE;
        }
      }

      /* Set default Xspi_Nor_Ctx values */
      Xspi_Nor_Ctx[Instance].IsInitialized = XSPI_ACCESS_NONE;
      Xspi_Nor_Ctx[Instance].InterfaceMode = BSP_XSPI_NOR_SPI_MODE;
      Xspi_Nor_Ctx[Instance].TransferRate  = BSP_XSPI_NOR_STR_TRANSFER;

#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 0)
      XSPI_NOR_MspDeInit(&hxspi_nor[Instance]);
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 0) */

      /* Call the DeInit function to reset the driver */
      if (HAL_XSPI_DeInit(&hxspi_nor[Instance]) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Initializes the XSPI interface.
  * @param  hxspi          XSPI handle
  * @param  Init           XSPI config structure
  * @retval BSP status
  */
__weak HAL_StatusTypeDef MX_XSPI_NOR_Init(XSPI_HandleTypeDef *hxspi, MX_XSPI_InitTypeDef *Init)
{
  /* XSPI initialization */
  hxspi->Instance = XSPI2;

  hxspi->Init.FifoThresholdByte       = 4;
  hxspi->Init.MemoryMode              = HAL_XSPI_SINGLE_MEM;
  hxspi->Init.MemorySize              = Init->MemorySize; /* 512 MBits */
  hxspi->Init.ChipSelectHighTimeCycle = 2;
  hxspi->Init.FreeRunningClock        = HAL_XSPI_FREERUNCLK_DISABLE;
  hxspi->Init.ClockMode               = HAL_XSPI_CLOCK_MODE_0;
  hxspi->Init.WrapSize                = HAL_XSPI_WRAP_NOT_SUPPORTED;
  hxspi->Init.ClockPrescaler          = Init->ClockPrescaler;
  hxspi->Init.SampleShifting          = Init->SampleShifting;
  hxspi->Init.ChipSelectBoundary      = 0;
#if defined (OCTOSPI_DCR1_DLYBYP)
  hxspi->Init.DelayBlockBypass           = HAL_XSPI_DELAY_BLOCK_BYPASS;
#endif

  if (Init->TransferRate == (uint32_t) BSP_XSPI_NOR_DTR_TRANSFER)
  {
    hxspi->Init.MemoryType            = HAL_XSPI_MEMTYPE_MACRONIX;
    hxspi->Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_ENABLE;
  }
  else
  {
    hxspi->Init.MemoryType            = HAL_XSPI_MEMTYPE_MICRON;
    hxspi->Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_DISABLE;
  }

  return HAL_XSPI_Init(hxspi);
}

#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1)
/**
  * @brief Default BSP XSPI Msp Callbacks
  * @param Instance      XSPI Instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_RegisterDefaultMspCallbacks(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_XSPI_RegisterCallback(&hxspi_nor[Instance], HAL_XSPI_MSP_INIT_CB_ID, XSPI_NOR_MspInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_XSPI_RegisterCallback(&hxspi_nor[Instance], HAL_XSPI_MSP_DEINIT_CB_ID, XSPI_NOR_MspDeInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      XspiNor_IsMspCbValid[Instance] = 1U;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief BSP XSPI Msp Callback registering
  * @param Instance     XSPI Instance
  * @param CallBacks    pointer to MspInit/MspDeInit callbacks functions
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_RegisterMspCallbacks(uint32_t Instance, BSP_XSPI_Cb_t *CallBacks)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_XSPI_RegisterCallback(&hxspi_nor[Instance], HAL_XSPI_MSP_INIT_CB_ID, CallBacks->pMspInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_XSPI_RegisterCallback(&hxspi_nor[Instance], HAL_XSPI_MSP_DEINIT_CB_ID,
                                       CallBacks->pMspDeInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      XspiNor_IsMspCbValid[Instance] = 1U;
    }
  }

  /* Return BSP status */
  return ret;
}
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 1) */

/**
  * @brief  Reads an amount of data from the XSPI memory.
  * @param  Instance  XSPI instance
  * @param  pData     Pointer to data to be read
  * @param  ReadAddr  Read start address
  * @param  Size      Size of data to read
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_Read(uint32_t Instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (Xspi_Nor_Ctx[Instance].TransferRate == BSP_XSPI_NOR_STR_TRANSFER)
    {
      if (MX25UW25645G_ReadSTR(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                               MX25UW25645G_4BYTES_SIZE, pData, ReadAddr, Size) != MX25UW25645G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    else
    {
      if (MX25UW25645G_ReadDTR(&hxspi_nor[Instance], pData, ReadAddr, Size) != MX25UW25645G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Writes an amount of data to the XSPI memory.
  * @param  Instance  XSPI instance
  * @param  pData     Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size      Size of data to write
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_Write(uint32_t Instance, uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t end_addr;
  uint32_t current_size;
  uint32_t current_addr;
  uint32_t data_addr;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Calculation of the size between the write address and the end of the page */
    current_size = MX25UW25645G_PAGE_SIZE - (WriteAddr % MX25UW25645G_PAGE_SIZE);

    /* Check if the size of the data is less than the remaining place in the page */
    if (current_size > Size)
    {
      current_size = Size;
    }

    /* Initialize the address variables */
    current_addr = WriteAddr;
    end_addr = WriteAddr + Size;
    data_addr = (uint32_t)pData;

    /* Perform the write page by page */
    do
    {

      /* Check if Flash busy ? */
      if (MX25UW25645G_AutoPollingMemReady(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                           Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }/* Enable write operations */
      else if (MX25UW25645G_WriteEnable(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                        Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        if (Xspi_Nor_Ctx[Instance].TransferRate == BSP_XSPI_NOR_STR_TRANSFER)
        {
          /* Issue page program command */
          if (MX25UW25645G_PageProgram(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                       MX25UW25645G_4BYTES_SIZE, (uint8_t *)data_addr, current_addr,
                                       current_size) != MX25UW25645G_OK)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
        }
        else
        {
          /* Issue page program command */
          if (MX25UW25645G_PageProgramDTR(&hxspi_nor[Instance], (uint8_t *)data_addr, current_addr,
                                          current_size) != MX25UW25645G_OK)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
        }

        if (ret == BSP_ERROR_NONE)
        {
          /* Configure automatic polling mode to wait for end of program */
          if (MX25UW25645G_AutoPollingMemReady(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                               Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            /* Update the address and size variables for next page programming */
            current_addr += current_size;
            data_addr += current_size;
            current_size = ((current_addr + MX25UW25645G_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : \
                           MX25UW25645G_PAGE_SIZE;
          }
        }
      }
    } while ((current_addr < end_addr) && (ret == BSP_ERROR_NONE));
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Erases the specified block of the XSPI memory.
  * @param  Instance     XSPI instance
  * @param  BlockAddress Block address to erase
  * @param  BlockSize    Erase Block size
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_Erase_Block(uint32_t Instance, uint32_t BlockAddress, BSP_XSPI_NOR_Erase_t BlockSize)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check Flash busy ? */
    if (MX25UW25645G_AutoPollingMemReady(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                         Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Enable write operations */
    else if (MX25UW25645G_WriteEnable(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                      Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Issue Block Erase command */
    else if (MX25UW25645G_BlockErase(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                     Xspi_Nor_Ctx[Instance].TransferRate, MX25UW25645G_4BYTES_SIZE,
                                     BlockAddress, BlockSize) != MX25UW25645G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Erases the entire XSPI memory.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_Erase_Chip(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check Flash busy ? */
    if (MX25UW25645G_AutoPollingMemReady(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                         Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Enable write operations */
    else if (MX25UW25645G_WriteEnable(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                      Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Issue Chip erase command */
    else if (MX25UW25645G_ChipErase(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                    Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Reads current status of the XSPI memory.
  * @param  Instance  QSPI instance
  * @retval XSPI memory status: whether busy or not
  */
int32_t BSP_XSPI_NOR_GetStatus(uint32_t Instance)
{
  static uint8_t reg[2];
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (MX25UW25645G_ReadSecurityRegister(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                          Xspi_Nor_Ctx[Instance].TransferRate, reg) != MX25UW25645G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Check the value of the register */
    else if ((reg[0] & (MX25UW25645G_SECR_P_FAIL | MX25UW25645G_SECR_E_FAIL)) != 0U)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else if ((reg[0] & (MX25UW25645G_SECR_PSB | MX25UW25645G_SECR_ESB)) != 0U)
    {
      ret = BSP_ERROR_XSPI_SUSPENDED;
    }
    else if (MX25UW25645G_ReadStatusRegister(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                             Xspi_Nor_Ctx[Instance].TransferRate, reg) != MX25UW25645G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Check the value of the register */
    else if ((reg[0] & MX25UW25645G_SR_WIP) != 0U)
    {
      ret = BSP_ERROR_BUSY;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Return the configuration of the XSPI memory.
  * @param  Instance  XSPI instance
  * @param  pInfo     pointer on the configuration structure
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_GetInfo(uint32_t Instance, BSP_XSPI_NOR_Info_t *pInfo)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    (void)MX25UW25645G_GetFlashInfo(pInfo);
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Configure the XSPI in memory-mapped mode
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_EnableMemoryMappedMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (Xspi_Nor_Ctx[Instance].TransferRate == BSP_XSPI_NOR_STR_TRANSFER)
    {
      if (MX25UW25645G_EnableSTRMemoryMappedMode(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                                 MX25UW25645G_4BYTES_SIZE) != MX25UW25645G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else /* Update XSPI context if all operations are well done */
      {
        Xspi_Nor_Ctx[Instance].IsInitialized = XSPI_ACCESS_MMP;
      }
    }
    else
    {
      if (MX25UW25645G_EnableDTRMemoryMappedMode(&hxspi_nor[Instance],
                                                 Xspi_Nor_Ctx[Instance].InterfaceMode) != MX25UW25645G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else /* Update XSPI context if all operations are well done */
      {
        Xspi_Nor_Ctx[Instance].IsInitialized = XSPI_ACCESS_MMP;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Exit form memory-mapped mode
  *         Only 1 Instance can running MMP mode. And it will lock system at this mode.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_DisableMemoryMappedMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t tickstart = HAL_GetTick();

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (Xspi_Nor_Ctx[Instance].IsInitialized != XSPI_ACCESS_MMP)
    {
      ret = BSP_ERROR_XSPI_MMP_UNLOCK_FAILURE;
    }/* Abort MMP back to indirect mode */
    else if (HAL_XSPI_Abort(&hxspi_nor[Instance]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      /* Nothing to do */
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get flash ID 3 Bytes:
  *         Manufacturer ID, Memory type, Memory density
  * @param  Instance  XSPI instance
  * @param  Id Pointer to flash ID bytes
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_ReadID(uint32_t Instance, uint8_t *Id)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MX25UW25645G_ReadID(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                               Xspi_Nor_Ctx[Instance].TransferRate, Id) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set Flash to desired Interface mode. And this instance becomes current instance.
  *         If current instance running at MMP mode then this function doesn't work.
  *         Indirect -> Indirect
  * @param  Instance  XSPI instance
  * @param  Mode      XSPI mode
  * @param  Rate      XSPI transfer rate
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_ConfigFlash(uint32_t Instance, BSP_XSPI_NOR_Interface_t Mode, BSP_XSPI_NOR_Transfer_t Rate)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if MMP mode locked ************************************************/
    if (Xspi_Nor_Ctx[Instance].IsInitialized == XSPI_ACCESS_MMP)
    {
      ret = BSP_ERROR_XSPI_MMP_LOCK_FAILURE;
    }
    else
    {
      /* Setup Flash interface ***************************************************/
      switch (Xspi_Nor_Ctx[Instance].InterfaceMode)
      {
        case BSP_XSPI_NOR_OPI_MODE :  /* 8-8-8 commands */
          if ((Mode != BSP_XSPI_NOR_OPI_MODE) || (Rate != Xspi_Nor_Ctx[Instance].TransferRate))
          {
            /* Exit OPI mode */
            ret = XSPI_NOR_ExitOPIMode(Instance);

            if ((ret == BSP_ERROR_NONE) && (Mode == BSP_XSPI_NOR_OPI_MODE))
            {

              if (Xspi_Nor_Ctx[Instance].TransferRate == BSP_XSPI_NOR_STR_TRANSFER)
              {
                /* Enter DTR OPI mode */
                ret = XSPI_NOR_EnterDOPIMode(Instance);
              }
              else
              {
                /* Enter STR OPI mode */
                ret = XSPI_NOR_EnterSOPIMode(Instance);
              }
            }
          }
          break;

        case BSP_XSPI_NOR_SPI_MODE :  /* 1-1-1 commands, Power on H/W default setting */
        default :
          if (Mode == BSP_XSPI_NOR_OPI_MODE)
          {
            if (Rate == BSP_XSPI_NOR_STR_TRANSFER)
            {
              /* Enter STR OPI mode */
              ret = XSPI_NOR_EnterSOPIMode(Instance);
            }
            else
            {
              /* Enter DTR OPI mode */
              ret = XSPI_NOR_EnterDOPIMode(Instance);
            }
          }
          break;
      }

      /* Update XSPI context if all operations are well done */
      if (ret == BSP_ERROR_NONE)
      {
        /* Update current status parameter *****************************************/
        Xspi_Nor_Ctx[Instance].IsInitialized = XSPI_ACCESS_INDIRECT;
        Xspi_Nor_Ctx[Instance].InterfaceMode = Mode;
        Xspi_Nor_Ctx[Instance].TransferRate  = Rate;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function suspends an ongoing erase command.
  * @param  Instance  QSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_SuspendErase(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check whether the device is busy (erase operation is in progress). */
  else if (BSP_XSPI_NOR_GetStatus(Instance) != BSP_ERROR_BUSY)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25UW25645G_Suspend(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (BSP_XSPI_NOR_GetStatus(Instance) != BSP_ERROR_XSPI_SUSPENDED)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function resumes a paused erase command.
  * @param  Instance  QSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_ResumeErase(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check whether the device is busy (erase operation is in progress). */
  else if (BSP_XSPI_NOR_GetStatus(Instance) != BSP_ERROR_XSPI_SUSPENDED)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25UW25645G_Resume(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                               Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /*
  When this command is executed, the status register write in progress bit is set to 1, and
  the flag status register program erase controller bit is set to 0. This command is ignored
  if the device is not in a suspended state.
  */
  else if (BSP_XSPI_NOR_GetStatus(Instance) != BSP_ERROR_BUSY)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function enter the XSPI memory in deep power down mode.
  * @param  Instance  QSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_EnterDeepPowerDown(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MX25UW25645G_EnterPowerDown(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                       Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* ---          Memory takes 10us max to enter deep power down          --- */

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function leave the XSPI memory from deep power down mode.
  * @param  Instance  QSPI instance
  * @retval BSP status
  */
int32_t BSP_XSPI_NOR_LeaveDeepPowerDown(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MX25UW25645G_ReleaseFromPowerDown(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                             Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* --- A NOP command is sent to the memory, as the nCS should be low for at least 20 ns --- */
  /* ---                  Memory takes 30us min to leave deep power down                  --- */

  /* Return BSP status */
  return ret;
}
/**
  * @}
  */

/** @addtogroup XSPI_NOR_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the XSPI MSP.
  * @param  hxspi XSPI handle
  * @retval None
  */
void XSPI_NOR_MspInit(XSPI_HandleTypeDef *hxspi)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* hxspi unused argument(s) compilation warning */
  UNUSED(hxspi);

  /* Enable the XSPI memory interface clock */
  XSPI_CLK_ENABLE();

  /* Enable XSPIM2 power */
  HAL_PWREx_EnableXSPIM2();

  /* high speed low voltage config */
  HAL_SBS_EnableIOSpeedOptimize(SBS_IO_XSPI2_HSLV);

  /* Reset the XSPI memory interface */
  XSPI_FORCE_RESET();
  XSPI_RELEASE_RESET();

  /* Enable GPIO clocks */
  XSPI_CLK_GPIO_CLK_ENABLE();
  XSPI_DQS_GPIO_CLK_ENABLE();
  XSPI_CS_GPIO_CLK_ENABLE();
  XSPI_D0_GPIO_CLK_ENABLE();
  XSPI_D1_GPIO_CLK_ENABLE();
  XSPI_D2_GPIO_CLK_ENABLE();
  XSPI_D3_GPIO_CLK_ENABLE();
  XSPI_D4_GPIO_CLK_ENABLE();
  XSPI_D5_GPIO_CLK_ENABLE();
  XSPI_D6_GPIO_CLK_ENABLE();
  XSPI_D7_GPIO_CLK_ENABLE();

  /* XSPI CS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_CS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = XSPI_CS_PIN_AF;
  HAL_GPIO_Init(XSPI_CS_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI CLK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_CLK_PIN;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Alternate = XSPI_CLK_PIN_AF;
  HAL_GPIO_Init(XSPI_CLK_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_D0_PIN;
  GPIO_InitStruct.Alternate = XSPI_D0_PIN_AF;
  HAL_GPIO_Init(XSPI_D0_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D1 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_D1_PIN;
  GPIO_InitStruct.Alternate = XSPI_D1_PIN_AF;
  HAL_GPIO_Init(XSPI_D1_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D2 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_D2_PIN;
  GPIO_InitStruct.Alternate = XSPI_D2_PIN_AF;
  HAL_GPIO_Init(XSPI_D2_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D3 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_D3_PIN;
  GPIO_InitStruct.Alternate = XSPI_D3_PIN_AF;
  HAL_GPIO_Init(XSPI_D3_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D4 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_D4_PIN;
  GPIO_InitStruct.Alternate = XSPI_D4_PIN_AF;
  HAL_GPIO_Init(XSPI_D4_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D5 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_D5_PIN;
  GPIO_InitStruct.Alternate = XSPI_D5_PIN_AF;
  HAL_GPIO_Init(XSPI_D5_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D6 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_D6_PIN;
  GPIO_InitStruct.Alternate = XSPI_D6_PIN_AF;
  HAL_GPIO_Init(XSPI_D6_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI D7 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_D7_PIN;
  GPIO_InitStruct.Alternate = XSPI_D7_PIN_AF;
  HAL_GPIO_Init(XSPI_D7_GPIO_PORT, &GPIO_InitStruct);

  /* XSPI DQS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = XSPI_DQS_PIN;
  GPIO_InitStruct.Alternate = XSPI_DQS_PIN_AF;
  HAL_GPIO_Init(XSPI_DQS_GPIO_PORT, &GPIO_InitStruct);

  /* Configure the NVIC for XSPI */
  /* NVIC configuration for XSPI interrupt */
  HAL_NVIC_SetPriority(XSPI1_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(XSPI1_IRQn);
}

/**
  * @brief  De-Initializes the XSPI MSP.
  * @param  hxspi XSPI handle
  * @retval None
  */
static void XSPI_NOR_MspDeInit(XSPI_HandleTypeDef *hxspi)
{
  /* hxspi unused argument(s) compilation warning */
  UNUSED(hxspi);

  /* Reset the XSPI memory interface */
  XSPI_FORCE_RESET();
  XSPI_RELEASE_RESET();

  /* Disable the XSPI memory interface clock */
  XSPI_CLK_DISABLE();


}

/**
  * @brief  This function reset the XSPI memory.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
static int32_t XSPI_NOR_ResetMemory(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MX25UW25645G_ResetEnable(&hxspi_nor[Instance], BSP_XSPI_NOR_SPI_MODE,
                                    BSP_XSPI_NOR_STR_TRANSFER) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25UW25645G_ResetMemory(&hxspi_nor[Instance], BSP_XSPI_NOR_SPI_MODE,
                                    BSP_XSPI_NOR_STR_TRANSFER) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25UW25645G_ResetEnable(&hxspi_nor[Instance], BSP_XSPI_NOR_OPI_MODE,
                                    BSP_XSPI_NOR_STR_TRANSFER) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25UW25645G_ResetMemory(&hxspi_nor[Instance], BSP_XSPI_NOR_OPI_MODE,
                                    BSP_XSPI_NOR_STR_TRANSFER) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25UW25645G_ResetEnable(&hxspi_nor[Instance], BSP_XSPI_NOR_OPI_MODE,
                                    BSP_XSPI_NOR_DTR_TRANSFER) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX25UW25645G_ResetMemory(&hxspi_nor[Instance], BSP_XSPI_NOR_OPI_MODE,
                                    BSP_XSPI_NOR_DTR_TRANSFER) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Xspi_Nor_Ctx[Instance].IsInitialized = XSPI_ACCESS_INDIRECT;      /* After reset S/W setting to indirect access  */
    Xspi_Nor_Ctx[Instance].InterfaceMode = BSP_XSPI_NOR_SPI_MODE;     /* After reset H/W back to SPI mode by default */
    Xspi_Nor_Ctx[Instance].TransferRate  = BSP_XSPI_NOR_STR_TRANSFER; /* After reset S/W setting to STR mode         */

    /* After SWreset CMD, wait in case SWReset occurred during erase operation */
    HAL_Delay(MX25UW25645G_RESET_MAX_TIME);
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function enables the octal DTR mode of the memory.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
static int32_t XSPI_NOR_EnterDOPIMode(uint32_t Instance)
{
  int32_t ret;
  uint8_t reg[2];

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Enable write operations */
  else if (MX25UW25645G_WriteEnable(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                    Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Write Configuration register 2 (with new dummy cycles) */
  else if (MX25UW25645G_WriteCfg2Register(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                          Xspi_Nor_Ctx[Instance].TransferRate, MX25UW25645G_CR2_REG3_ADDR,
                                          MX25UW25645G_CR2_DC_20_CYCLES) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Enable write operations */
  else if (MX25UW25645G_WriteEnable(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                    Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Write Configuration register 2 (with Octal I/O SPI protocol) */
  else if (MX25UW25645G_WriteCfg2Register(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                          Xspi_Nor_Ctx[Instance].TransferRate, MX25UW25645G_CR2_REG1_ADDR,
                                          MX25UW25645G_CR2_DOPI) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Wait that the configuration is effective and check that memory is ready */
    HAL_Delay(MX25UW25645G_WRITE_REG_MAX_TIME);

    /* Reconfigure the memory type of the peripheral */
    hxspi_nor[Instance].Init.MemoryType            = HAL_XSPI_MEMTYPE_MACRONIX;
    hxspi_nor[Instance].Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_ENABLE;
    if (HAL_XSPI_Init(&hxspi_nor[Instance]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    /* Check Flash busy ? */
    else if (MX25UW25645G_AutoPollingMemReady(&hxspi_nor[Instance], BSP_XSPI_NOR_OPI_MODE,
                                              BSP_XSPI_NOR_DTR_TRANSFER) != MX25UW25645G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    /* Check the configuration has been correctly done */
    else if (MX25UW25645G_ReadCfg2Register(&hxspi_nor[Instance], BSP_XSPI_NOR_OPI_MODE, BSP_XSPI_NOR_DTR_TRANSFER,
                                           MX25UW25645G_CR2_REG1_ADDR, reg) != MX25UW25645G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else if (reg[0] != MX25UW25645G_CR2_DOPI)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function enables the octal STR mode of the memory.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
static int32_t XSPI_NOR_EnterSOPIMode(uint32_t Instance)
{
  int32_t ret;
  uint8_t reg[2];

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Enable write operations */
  else if (MX25UW25645G_WriteEnable(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                    Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Write Configuration register 2 (with new dummy cycles) */
  else if (MX25UW25645G_WriteCfg2Register(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                          Xspi_Nor_Ctx[Instance].TransferRate, MX25UW25645G_CR2_REG3_ADDR,
                                          MX25UW25645G_CR2_DC_6_CYCLES) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Enable write operations */
  else if (MX25UW25645G_WriteEnable(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                    Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Write Configuration register 2 (with Octal I/O SPI protocol) */
  else if (MX25UW25645G_WriteCfg2Register(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                          Xspi_Nor_Ctx[Instance].TransferRate, MX25UW25645G_CR2_REG1_ADDR,
                                          MX25UW25645G_CR2_SOPI) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Wait that the configuration is effective and check that memory is ready */
    HAL_Delay(MX25UW25645G_WRITE_REG_MAX_TIME);

    /* Check Flash busy ? */
    if (MX25UW25645G_AutoPollingMemReady(&hxspi_nor[Instance], BSP_XSPI_NOR_OPI_MODE,
                                         BSP_XSPI_NOR_STR_TRANSFER) != MX25UW25645G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    /* Check the configuration has been correctly done */
    else if (MX25UW25645G_ReadCfg2Register(&hxspi_nor[Instance], BSP_XSPI_NOR_OPI_MODE, BSP_XSPI_NOR_STR_TRANSFER,
                                           MX25UW25645G_CR2_REG1_ADDR, reg) != MX25UW25645G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else if (reg[0] != MX25UW25645G_CR2_SOPI)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function disables the octal DTR or STR mode of the memory.
  * @param  Instance  XSPI instance
  * @retval BSP status
  */
static int32_t XSPI_NOR_ExitOPIMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  uint8_t reg[2];

  /* Check if the instance is supported */
  if (Instance >= XSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Enable write operations */
  else if (MX25UW25645G_WriteEnable(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                    Xspi_Nor_Ctx[Instance].TransferRate) != MX25UW25645G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Write Configuration register 2 (with SPI protocol) */
    reg[0] = 0;
    reg[1] = 0;
    if (MX25UW25645G_WriteCfg2Register(&hxspi_nor[Instance], Xspi_Nor_Ctx[Instance].InterfaceMode,
                                       Xspi_Nor_Ctx[Instance].TransferRate, MX25UW25645G_CR2_REG1_ADDR,
                                       reg[0]) != MX25UW25645G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Wait that the configuration is effective and check that memory is ready */
      HAL_Delay(MX25UW25645G_WRITE_REG_MAX_TIME);

      if (Xspi_Nor_Ctx[Instance].TransferRate == BSP_XSPI_NOR_DTR_TRANSFER)
      {
        /* Reconfigure the memory type of the peripheral */
        hxspi_nor[Instance].Init.MemoryType            = HAL_XSPI_MEMTYPE_MICRON;
        hxspi_nor[Instance].Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_DISABLE;
        if (HAL_XSPI_Init(&hxspi_nor[Instance]) != HAL_OK)
        {
          ret = BSP_ERROR_PERIPH_FAILURE;
        }
      }

      if (ret == BSP_ERROR_NONE)
      {
        /* Check Flash busy ? */
        if (MX25UW25645G_AutoPollingMemReady(&hxspi_nor[Instance], BSP_XSPI_NOR_SPI_MODE,
                                             BSP_XSPI_NOR_STR_TRANSFER) != MX25UW25645G_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        /* Check the configuration has been correctly done */
        else if (MX25UW25645G_ReadCfg2Register(&hxspi_nor[Instance], BSP_XSPI_NOR_SPI_MODE, BSP_XSPI_NOR_STR_TRANSFER,
                                               MX25UW25645G_CR2_REG1_ADDR, reg) != MX25UW25645G_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else if (reg[0] != 0U)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          /* Nothing to do */
        }
      }
    }
  }

  /* Return BSP status */
  return ret;
}

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

