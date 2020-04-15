
#ifndef ENCPT_H
#define ENCPT_H

#include "main.h"
/******************************************************************************
**********************Macro definition*******************************************
******************************************************************************/
/******************************************************************************************************************
  TEA加密解密算法
  TEA(Tiny Encryption Algorithm)是一种简单高效的加密算法，以加密解密速度快，实现简单著称。
  算法很简单，TEA算法每一次可以操作64-bit(8-byte)，采用128-bit(16-byte)作为key，算法采用迭代的形式，
  推荐的迭代轮数是64轮，最少32轮。
******************************************************************************************************************/
#define MX 			(z>>5^y<<2)+(y>>3^z<<4)^(sum^y)+(k[p&3^e]^z)
#define DELTA 		0x9e3779b9
#define S_LOOPTIME  5
#define BLOCK_SIZE  8   //PAGE_SIZE,根据你所要加密的数据包长度修改此参数(单位:字节)

#define TEA_ID_ADDR       31*1024
#define EEPROM_CHECK_ADDR       31*1024 + 20
/******************************************************************************
**********************Type statement*******************************************
******************************************************************************/

/******************************************************************************
**********************Variable definition*******************************************
******************************************************************************/

/****************************************************************************/
/* Function                                                                 */
/****************************************************************************/
extern void Encpt_EncryptExt(uint8* InBuf);
extern void Encpt_DecrpytExt( uint8* InBuf);
extern void TEA_Encrypt( uint8* buf, uint8* key );
extern void TEA_Decrpyt( uint8* buf, uint8* key );
//extern void Sys_StartUpCheck(void);
extern uint8 Sys_MobileCheck(uint8* pData);
extern void Sys_EncodKeyToEeprom(void);
//extern uint8 sys_CheckGetDynamic(uint32 DeviceId, uint32 DevicePassword, uint8* pTouchKey, uint8 length);
extern uint8 sys_CheckGetDynamicFlat(uint32 DeviceId, uint32 DevicePassword, uint8* pTouchKey, uint8  length);
extern uint8 sys_CheckGetDynamicHotel(uint32 DeviceId, uint32 DevicePassword, uint8* pTouchKey, uint8  length);
extern uint8 sys_CheckGetDynamicDate(uint32 DeviceId, uint32 DevicePassword, uint8* pTouchKey, uint8  length);
extern uint8 sys_CheckGetDynamicHotel_V8(uint32 DeviceId, uint32 DevicePassword, uint8* pTouchKey, uint8  length);

#endif	

