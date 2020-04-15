
#include"main.h"

/****************************************************************************************************
**Function:
  void Sys_GenFunDelayUs (void) 
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Sys_GenFunDelayUs (uint16 xus)
{  
  delay_us( xus );
}
/****************************************************************************************************
**Function:
  void Sys_GenFunDelayMs (void) 
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Sys_GenFunDelayMs (uint16 xms)
{	
	delay_ms( xms );
}
/****************************************************************************************************
**Function:
  uint8 Sys_GenFunChecksum(uint8 *pData)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint8 Sys_GenFunChecksum(uint8 *pData)
{     
	uint8 length;
	uint8 checksum = 0;
	
	checksum = 0;
	length = *pData;
	for(; length > 0; length--)
	{
		checksum ^= *pData++;
	}
	return(checksum);
}
/****************************************************************************************************
**Function:
  uint8 Sys_GenFunLChecksum(uint16 Lenth,uint8 *pData)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint8 Sys_GenFunLChecksum(uint16 Lenth,uint8 *pData)
{     
	uint8 checksum = 0;
	
	checksum = 0;
	for(; Lenth > 0; Lenth--)
	{
		checksum ^= *pData++;
	}
	return(checksum);
}

/****************************************************************************************************
**Function:
  uint32 Sys_GenFun8To32(uint8 *pData)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint32 Sys_GenFun8To32(uint8 *pData)
{
	uint32 tempdata;
	
	tempdata = *pData;
	tempdata <<= 8;
	pData++;
	tempdata |= *pData;
	tempdata <<= 8;
	pData++;
	tempdata |= *pData;
	tempdata <<= 8;
	pData++;
	tempdata |= *pData;
	return tempdata;
}
/****************************************************************************************************
**Function:
  uint16 Sys_GenFun8To16(uint8 *pData)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint16 Sys_GenFun8To16(uint8 *pData)
{
	uint32 tempdata;
	
	tempdata = *pData;
	tempdata <<= 8;
	pData++;
	tempdata |= *pData;
	return tempdata;
}
/****************************************************************************************************
**Function:
  void Sys_GenFun16To8(uint16 Data,uint8 *pData)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Sys_GenFun16To8(uint16 Data,uint8 *pData)
{
	*pData =(Data & 0x0000ff00)>>8;
	pData++;
	*pData = Data&0xff;
}
/****************************************************************************************************
**Function:
  void Sys_GenFun8To32(uint8 *pData)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Sys_GenFun32To8(uint32 Data,uint8 *pData)
{
	*pData =(Data & 0xff000000)>>24;
	pData++;
	*pData =(Data & 0x00ff0000)>>16;
	pData++;
	*pData =(Data & 0x0000ff00)>>8;
	pData++;
	*pData = Data&0xff;
}
/****************************************************************************************************
**Function:
	uint16 Sys_GenFunhtons(uint16 n)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint16 Sys_GenFunhtons(uint16 Data)
{
	char nTmp;
	nTmp = ((char*)&Data)[0];
	((char*)&Data)[0] = ((char*)&Data)[1];
	((char*)&Data)[1] = nTmp;

	return Data;
}
/****************************************************************************************************
**Function:
	int16 Sys_GenFunitons(int16 Data)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint16 Sys_GenFunitons(uint16 Data)
{
	char nTmp;
	nTmp = ((char*)&Data)[0];
	((char*)&Data)[0] = ((char*)&Data)[1];
	((char*)&Data)[1] = nTmp;

	return Data;
}
/****************************************************************************************************
**Function:
	uint32 Sys_GenFunhtonl(uint32 n)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint32 Sys_GenFunhtonl(uint32 Data)
{
	char nTmp;

	nTmp = ((char*)&Data)[0];
	((char*)&Data)[0] = ((char*)&Data)[3];
	((char*)&Data)[3] = nTmp;

	nTmp = ((char*)&Data)[1];
	((char*)&Data)[1] = ((char*)&Data)[2];
	((char*)&Data)[2] = nTmp;	

	return Data;
}

/****************************************************************************************************
**Function:
	uint32 Sys_GenFunhtonl(uint32 n)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
float Sys_GenFunhtonf(float Data)
{
	char nTmp;

	nTmp = ((char*)&Data)[0];
	((char*)&Data)[0] = ((char*)&Data)[3];
	((char*)&Data)[3] = nTmp;

	nTmp = ((char*)&Data)[1];
	((char*)&Data)[1] = ((char*)&Data)[2];
	((char*)&Data)[2] = nTmp;	

	return Data;
}

