

#ifndef FLASH_H
#define FLASH_H

#include "main.h"

/******************************************************************************
**********************Macro definition*******************************************
******************************************************************************/

#define FLASH_CMD_WRITE     0x02  /* Write to Memory instruction  Page Program */
#define FLASH_CMD_WRSR      0x01  /* Write Status Register instruction */
#define FLASH_CMD_WREN      0x06  /* Write enable instruction */
#define FLASH_CMD_READ      0x03  /* Read from Memory instruction */
#define FLASH_CMD_RDSR      0x05  /* Read Status Register instruction  */
#define FLASH_CMD_RDID      0x9F  /* Read identification */
#define FLASH_CMD_SE        0x20//0xD8  /* Sector Erase instruction */
#define FLASH_CMD_BL        0xD8//0xD8  /* BLock Erase instruction */
#define FLASH_CMD_BE        0xC7  /* Bulk Erase instruction */
#define WIP_FLAG            0x01  /* Write In Progress (WIP) flag */
#define FLASH_CMD_PD			  0xB9  /* power down */
#define FLASH_CMD_RPD	      0xAB  /* Release power down */


#define DUMMY_BYTE          0xA5



#define SPI_FLASH_PAGESIZE    256
#define SPI_FLASH_SECTORSIZE  128
#define SPI_FLASH_BLOCKSIZE   4


//#define FLASH_SIZE	        SPI_FLASH_PAGESIZE*SPI_FLASH_SECTORSIZE*SPI_FLASH_BLOCKSIZE


/******************************************************************************
**********************Type statement*******************************************
******************************************************************************/
typedef struct
{
	uint16	   Lenth;
	uint8      *pData;
	uint32	   Add;
} Flash_ComType;


typedef enum
{
	FLASH_ERASE_SECT = 0,
	FLASH_ERASE_BLOCK ,
	FLASH_ERASE_CHIP,
	FLASH_CMD_MAX
} Flash_CmdType;
/******************************************************************************
**********************Variable definition*******************************************
******************************************************************************/

/******************************************************************************
**********************Function declared*******************************************
******************************************************************************/
extern uint16 SPI_FLASH_ReadID(void);

extern Std_ReturnType Flash_Read(void *pData);
extern Std_ReturnType Flash_Write(void *pData);
extern Std_ReturnType Flash_IoCtl(uint8 Cmd,void *pData);

extern const Dr_OpertonType FlashDrive ;


extern void Flash_SpiBufferRead(uint8* pBuffer, uint32 ReadAddr, uint16 NumByteToRead);
extern void Flash_SpiPageWrite(uint8* pBuffer, uint32 WriteAddr, uint16 NumByteToWrite);
extern Std_ReturnType Flash_SpiSectorErase(void *SectorAddr);
extern void Flash_Powerdown(void);
extern void Flash_WAKEUP(void);


#endif	
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
