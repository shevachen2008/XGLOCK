#ifndef _W25X20CL_API_H_
#define _W25X20CL_API_H_


#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

/*******************************************************/
/*	  0x000000~~~~~~~~0x0FFFFF	----- 汉字字库	       */
/*	  0x100003~~~~~~~~0x100FFF	----- 密码保存	       */
/*    0x100000~~~~~~~~0x100002  ----- 密码标志位       */
/*	  0x101000~~~~~~~~0x101FFF	----- NFC 保存	       */
/*	  0x102000~~~~~~~~0x102FFF	----- 蓝牙保存	       */
/*    0x103000~~~~~~~~0x103FFF	----- 缓存空间         */
/*    0x104000~~~~~~~~0x104001  ----- 语言标志位       */
/*    0x104002~~~~~~~~0x104003  ----- 语音标志位       */
/*    0x105000~~~~~~~~0x105FFF  ----- RTC 标志位       */
/*    0x106000~~~~~~~~0x106000  ----- 开门记录数目	   */
/*    0x106001~~~~~~~~0x106FFF  ----- 开门记录		   */
/*******************************************************/


#define Zk_startaddr		 	        0x000000
#define num_startaddr			      	0x100000


#define NFC_startaddr					0x101000
#define BLE_startaddr					0x102000
#define Buff_startaddr					0x103000
#define Language_startaddr 		      	0x104000
#define Voice_startaddr    				0x104001
#define RTC_startaddr					0x105000
#define Open_startaddr    				0x106000 


void W25_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
void W25_Erase_Sector(uint32_t addr);
void W25_Write_NoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void W25_Erase_Chip(void);


#endif

