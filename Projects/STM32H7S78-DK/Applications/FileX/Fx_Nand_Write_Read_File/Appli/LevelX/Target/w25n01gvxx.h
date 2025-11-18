/**
  ******************************************************************************
  * @file    w25n01gvxx.h
  * @modify  MCD Application Team
  * @brief   This file contains all the description of the
  *          W25N01GVXX NAND flash memory.
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

#ifndef W25N01GVXX_H
#define W25N01GVXX_H

#include "w25n01gvxx_conf.h"


#define W25N01GVXX_CHIPID                               0xEFAA21                                                                 /*!< W25N01GVZEIG ID                                */

#define W25N01GVXX_CS_PIN_SET                           1                                                                        /*!< Select the W25N01GVXX memory                   */
#define W25N01GVXX_CS_PIN_RESET                         0                                                                        /*!< Deselect the W25N01GVXX memory                 */


#define STM32_UNUSED(x) ((void)(x))

#define W25N01GVXX_DUMMY_BYTE                           0x00                                                                     /*!< Dummy byte value                               */


#define W25N01GVXX_NUM_OF_BLOCK                         1024U                                                                    /*!< 1024 blocks                                    */

#define W25N01GVXX_NUM_PAGE_PER_BLOCK                   64                                                                       /*!< Number of pages per block => 64 pages          */
#define W25N01GVXX_NUM_SPARE_SECTOR_IN_PAGE             4                                                                        /*! < Number of spare columns per page              */
#define W25N01GVXX_SPARE_SECTOR_SIZE                    16                                                                       /*! < Size of each spare column                     */
#define W25N01GVXX_NUM_OF_SECTOR_IN_PAGE                4                                                                        /*!< Each page contain 4 sectors                    */
#define W25N01GVXX_SECTOR_SIZE                          512                                                                      /*!< Sector size is 512-Byte                        */

#define W25N01GVXX_PAGE_COUNT                           (W25N01GVXX_NUM_PAGE_PER_BLOCK * W25N01GVXX_NUM_OF_BLOCK)                /*!< Total number of pages => 65536                 */

#define W25N01GVXX_PAGE_MEM_SIZE                        (W25N01GVXX_NUM_OF_SECTOR_IN_PAGE * W25N01GVXX_SECTOR_SIZE)              /*!< Each page size 2048                            */

#define W25N01GVXX_PAGE_SPARE_SIZE                      (W25N01GVXX_NUM_SPARE_SECTOR_IN_PAGE * W25N01GVXX_SPARE_SECTOR_SIZE)     /*!< Size of spare Area per page                    */

#define W25N01GVXX_PAGE_SIZE1                           (W25N01GVXX_PAGE_MEM_SIZE)						 /*!< In CONTINUOUS read mode : 2048                 */

#define W25N01GVXX_PAGE_SIZE2                           (W25N01GVXX_PAGE_MEM_SIZE + W25N01GVXX_PAGE_SPARE_SIZE)	                 /*!< In BUFFER read mode : 2112                     */

#define W25N01GVXX_FLASH_SIZE                           (W25N01GVXX_PAGE_SIZE1*W25N01GVXX_PAGE_COUNT)                            /*!< Flash Size  (2048*65536) = 128 M-Bytes         */

#define W25N01GVXX_BLOCK_SIZE                           (W25N01GVXX_NUM_PAGE_PER_BLOCK * W25N01GVXX_PAGE_SIZE1)                  /*!< Erase Block Size                               */


/*************************************************************************************************************************************
  * @brief  Error Codes
  ***********************************************************************************************************************************/


#define	W25N01GVXX_OK                                   (0)
#define	W25N01GVXX_ERROR                                (-1)
#define	W25N01GVXX_SUSPENDED                            (-2)
#define	W25N01GVXX_BUSY                                 (-3)

/*************************************************************************************************************************************
  * @brief  W25N01GVXX Commands
  ***********************************************************************************************************************************/

#define W25N01GVXX_CMD_RESET                            0xFF                   /*!< W25N01GVZEIG Reset Memory                       */
#define W25N01GVXX_CMD_DEVICE_ID                        0x9F                   /*!< W25N01GVZEIG read ID                            */
#define W25N01GVXX_CMD_READ_REG_STATUS1                 0x05                   /*!< Read status register                            */
#define W25N01GVXX_CMD_READ_REG_STATUS2                 0x0F                   /*!< Read status register                            */
#define W25N01GVXX_CMD_WRITE_REG_STATUS1                0x01                   /*!< Write status register                           */
#define W25N01GVXX_CMD_WRITE_REG_STATUS2                0x1F                   /*!< Write status register                           */
#define W25N01GVXX_CMD_WRITE_ENABLE                     0x06                   /*!< Write Enable                                    */
#define W25N01GVXX_CMD_WRITE_DISABLE                    0x04                   /*!< Write disable                                   */
#define W25N01GVXX_CMD_BAD_BLOCK_MANAGE                 0xA1                   /*!< Bad Block Management command                    */
#define W25N01GVXX_CMD_READ_BBM_TABLE                   0xA5                   /*!< Read BBM Look Up Table                          */
#define W25N01GVXX_CMD_LAST_ECC_FAIL_PAGE               0xA9                   /*!< Last ECC Failure Page Address                   */
#define W25N01GVXX_CMD_BLOCK_ERASE                      0xD8                   /*!< 128KB Block Erase                               */
#define W25N01GVXX_CMD_LOAD_DATA                        0x02                   /*!< Load Program Data                               */
#define W25N01GVXX_CMD_RND_LOAD_DATA                    0x84                   /*!< Random Load Program Data                        */
#define W25N01GVXX_CMD_QUAD_LOAD_DATA                   0x32                   /*!< Quad Load Program Data                          */
#define W25N01GVXX_CMD_QUAD_RND_LOAD_DATA               0x34                   /*!< Quad Random Load Program Data                   */
#define W25N01GVXX_CMD_EXECUTE                          0x10                   /*!< Program Execute                                 */
#define W25N01GVXX_CMD_PAGE_READ_DATA                   0x13                   /*!< Page Data Read                                  */
#define W25N01GVXX_CMD_READ_DATA                        0x03                   /*!< Read Data                                       */
#define W25N01GVXX_CMD_FAST_READ                        0x0B                   /*!< Fast Read                                       */
#define W25N01GVXX_CMD_FAST_READ_4BYTE_ADDR             0x0C                   /*!< Fast Read with 4-Byte Address                   */
#define W25N01GVXX_CMD_FAST_READ_DUAL_OUT               0x3B                   /*!< Fast Read Dual Output                           */
#define W25N01GVXX_CMD_FAST_READ_DUAL_OUT_4BYTE_ADDR    0x3C                   /*!< Fast Read Dual Output with 4-Byte Address       */
#define W25N01GVXX_CMD_FAST_READ_QUAD_OUT               0x6B                   /*!< Fast Read Quad Output                           */
#define W25N01GVXX_CMD_FAST_READ_QUAD_OUT_4BYTE_ADDR    0x6C                   /*!< Fast Read Quad Output with 4-Byte Address       */
#define W25N01GVXX_CMD_FAST_READ_DUAL_IO                0xBB                   /*!< Fast Read Dual I/O                              */
#define W25N01GVXX_CMD_FAST_READ_IO_4BYTE_ADDR          0xBC                   /*!< Fast Read Dual I/O with 4-Byte Address          */
#define W25N01GVXX_CMD_FAST_READ_QUAD_IO                0xEB                   /*!< Fast Read Quad I/O                              */
#define W25N01GVXX_CMD_FAST_READ_QUAD_IO_4BYTE_ADDDR    0xEC                   /*!< Fast Read Quad I/O with 4-Byte Address          */





/*************************************************************************************************************************************
  * @brief  W25N01GVXX Registers
  ***********************************************************************************************************************************/

#define W25N01GVXX_REG_STATUS_1                         0xA0                  /*!< Protection Register                              */
#define W25N01GVXX_REG_STATUS_2                         0xB0                  /*!< Configuration Register                           */
#define W25N01GVXX_REG_STATUS_3                         0xC0                  /*!< Status Only Register                             */

/* Protection Register -------------------------------*/

#define W25N01GVXX_RS1_WRITE_PROTECTION_ENABLE          0x02                   /*!< WP Enable                                       */
#define W25N01GVXX_RS1_SRP1_ENABLE                      0x01                   /*!< Status Register Protect 1 enable                */
#define W25N01GVXX_RS1_SRP0_ENABLE                      0x80                   /*!< Status Register Protect 0 enable                */
#define W25N01GVXX_RS1_WRITE_PROTECTION_DISABLE         0x00                   /*!< Disable protection                              */
#define W25N01GVXX_RS1_PROTECT_LOWER_64MB               0x4C
#define W25N01GVXX_RS1_PROTECT_LOWER_32MB               0x44
#define W25N01GVXX_RS1_PROTECT_LOWER_16MB               0x3C
#define W25N01GVXX_RS1_PROTECT_LOWER_8MB                0x34
#define W25N01GVXX_RS1_PROTECT_LOWER_4MB                0x2C

#define W25N01GVXX_RS1_PROTECT_UPPER_64MB               0x48
#define W25N01GVXX_RS1_PROTECT_UPPER_32MB               0x20
#define W25N01GVXX_RS1_PROTECT_UPPER_16MB               0x38
#define W25N01GVXX_RS1_PROTECT_UPPER_8MB                0x30
#define W25N01GVXX_RS1_PROTECT_UPPER_4MB                0x28

#define W25N01GVXX_RS1_DEFAULT                          0x7C                   /*!< Register status 1 Default Value                 */

/* Configuration Register ----------------------------*/


#define W25N01GVXX_RS2_PAGE_READ_MODE                   0x08                   /*!< Buffer Read Mode                                */
#define W25N01GVXX_RS2_PAGE_CONTINOUS_READ_MODE         0x00                   /*!< Continuous Read Mode Bit                        */
#define W25N01GVXX_RS2_ECC_ENABLED                      0x10                   /*!< ECC Enable Bit (ECC-E)                          */
#define W25N01GVXX_RS2_OTP_LOCK                         0x80                   /*!< One Time Program Lock Bit (OTP-L)               */
#define W25N01GVXX_RS2_OTP_ACCESS                       0x40                   /*!< Enter OTP Access Mode Bit (OTP-E)               */
#define W25N01GVXX_RS2_REG_STATUS_1_LOCK                0x20                   /*!< Status Register-1 Lock Bit (SR1-L)              */

#define W25N01GVXX_RS2_DEFAULT                          0x18                   /*!< Register status 2 Default Value                 */



/* Status Only ---------------------------------------*/


#define W25N01GVXX_RS3_S0_BUSY                          0x01                   /*!< Erase/Program In Progress (BUSY)                */
#define W25N01GVXX_RS3_S1_WEL                           0x02                   /*!< Write Enable Latch (WEL)                        */
#define W25N01GVXX_RS3_S2_EFAIL                         0x04                   /*!< Erase Failure                                   */
#define W25N01GVXX_RS3_S3_PFAIL                         0x08                   /*!< Program Failure                                 */
#define W25N01GVXX_RS3_S4_ECC0                          0x10                   /*!< Cumulative ECC Status ECC0                      */
#define W25N01GVXX_RS3_S5_ECC1                          0x20                   /*!< Cumulative ECC Status ECC1                      */
#define W25N01GVXX_RS3_S6_LUTF                          0x40                   /*!< Look-Up Table Full (LUT-F)                      */

#define W25N01GVXX_RS3_DEFAULT                          0x00                   /*!< Register status 3 Default Value                 */


#define SECTOR0_OFFSET_ADDRESS		                      0x0000
#define SECTOR1_OFFSET_ADDRESS		                      0x0200
#define SECTOR2_OFFSET_ADDRESS		                      0x0400
#define SECTOR3_OFFSET_ADDRESS		                      0x0600
#define SECTORS_SIZE                                    512

#define SPARE0_OFFSET_ADDRESS                           0x0800
#define SPARE1_OFFSET_ADDRESS                           0x0810
#define SPARE2_OFFSET_ADDRESS                           0x0820
#define SPARE3_OFFSET_ADDRESS                           0x0830
#define SPARES_SIZE                                     16

/*************************************************************************************************************************************
  * @brief  Exported Types
  ***********************************************************************************************************************************/

/**
  * @brief Structure defining the W25N01GVXX information.
  */
typedef struct
{
  uint32_t              Flash_size;                /*!< Size of the flash                             */
  uint32_t              SectorSize;                /*!< Size of sector                                */
  uint32_t              SectorCount;               /*!< Number of sectors                             */
  uint32_t              BlockSize;                 /*!< Size of block                                 */
  uint32_t              BlockCount;                /*!< Number of blocks                              */
  uint32_t              PageCount;                 /*!< Number of pages for the program operation     */
  uint16_t              PageSize;                  /*!< Size of pages for the program operation       */
  uint8_t               SectorsPerPage;            /*!< Number of sectors per page                    */
  uint8_t               SpareAreaSize;             /*!< Size of spare Area per page                   */
  uint8_t               SpareSectors;              /*!< Number of spare sectors per page              */
  uint8_t               SpareSectorSize;            /*!< Size of spare sector                         */
} w25n01gvxx_info_t;

/*************************************************************************************************************************************
  * @brief  Functions
  ***********************************************************************************************************************************/

/* Memory Reset -----------------------------------*/
int32_t w25n01gvxx_Reset(SPI_HandleTypeDef *phspi);

/* Memory initialization --------------------------*/
int32_t w25n01gvxx_Init(SPI_HandleTypeDef *phspi);
int32_t w25n01gvxx_DeInit(SPI_HandleTypeDef *phspi);

/* Memory information and status ------------------*/
int32_t w25n01gvxx_ReadID(SPI_HandleTypeDef *phspi, uint8_t *pid);
int32_t w25n01gvxx_GetInfo(w25n01gvxx_info_t *pinfo);

/* Read/Write status registers --------------------*/
int32_t w25n01gvxx_ReadProtectionRegister(SPI_HandleTypeDef *phspi, uint8_t *pValue);
int32_t w25n01gvxx_ReadConfigRegister(SPI_HandleTypeDef *phspi, uint8_t *pValue);
int32_t w25n01gvxx_ReadStatusRegister(SPI_HandleTypeDef *phspi, uint8_t *pValue);
int32_t w25n01gvxx_WriteProtectionRegister(SPI_HandleTypeDef *phspi, uint8_t pValue);
int32_t w25n01gvxx_WriteConfigRegister(SPI_HandleTypeDef *phspi, uint8_t pValue);

/* Write enable/disable functions -----------------*/
int32_t w25n01gvxx_WriteEnable(SPI_HandleTypeDef *phspi);
int32_t w25n01gvxx_WriteDisable(SPI_HandleTypeDef *phspi);

/* Read/Write page Commands -----------------------*/
int32_t w25n01gvxx_WriteData(SPI_HandleTypeDef *phspi, uint16_t block, uint8_t page, uint8_t *data, uint32_t size);
int32_t w25n01gvxx_ReadPage(SPI_HandleTypeDef *phspi, uint16_t block, uint8_t page, uint8_t *pBuffer, uint32_t size);

/* Read/Write spare area functions ----------------*/
int32_t w25n01gvxx_ReadSpareArea(SPI_HandleTypeDef *phspi, uint16_t block, uint8_t page, uint8_t sector, uint8_t *pBuffer);
int32_t w25n01gvxx_WriteSpareArea(SPI_HandleTypeDef *phspi, uint16_t block, uint8_t page, uint8_t sector, uint8_t *pBuffer);

/* Memory erase functions -------------------------*/
int32_t w25n01gvxx_EraseBlock(SPI_HandleTypeDef *phspi, uint16_t block);
int32_t w25n01gvxx_FullChipErase(SPI_HandleTypeDef *phspi);
int32_t w25n01gvxx_PagesCopy(SPI_HandleTypeDef *phspi, uint16_t src_block, uint8_t src_page, uint16_t dest_block, uint8_t dest_page, uint32_t size);

#endif /* W25N01GVZEIG_H */
