
#include "main.h"

#if 1
uint8 Rtc_Wlarm_Enable;
uint32 ProtoAnaly_RtcLocalTime;
uint32 ProtoAnaly_RtcLocalTimeBak;

Rtc_Type ProtoAnaly_Rtcinfo;


Std_ReturnType Rtc_ReadNBytes(Rtc_CommType Cmd, BYTE *pDataBuf, uint8 NumBytes)
{
	return	I2cReadBytes(Cmd, pDataBuf, NumBytes); 
}


Std_ReturnType Rtc_WriteNBytes(Rtc_CommType Cmd, BYTE *pDataBuf, uint16 NumBytes)
{
	return I2cWriteBytes(Cmd, pDataBuf, NumBytes); 
}
/****************************************************************************************************
**Function:
void Rtc_Init(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Rtc_Init(void)
{
	uint8 DataBuffer[2];
	
	//I2C_SOFT_Init();
	DataBuffer[0] = 0x40;     
	Rtc_WriteNBytes(RTC_ADD|RTC_COMM_STATUS_REG1,DataBuffer,1); 
	
	DataBuffer[0] = 0;
	Rtc_WriteNBytes(RTC_ADD|RTC_COMM_STATUS_REG2,DataBuffer,1);
}
/****************************************************************************************************
**Function:
   uint8 Rtc_SwapByte(uint8 Data)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint8 Rtc_SwapByte(uint8 Data)
{
	uint8 tempdata ;
	tempdata = 0;
	tempdata |= (0x01&Data)<<7;
	tempdata |= (0x02&Data)<<5;
	tempdata |= (0x04&Data)<<3;
	tempdata |= (0x08&Data)<<1;
	tempdata |= (0x10&Data)>>1;
	tempdata |= (0x20&Data)>>3;
	tempdata |= (0x40&Data)>>5;
	tempdata |= (0x80&Data)>>7;
	return (tempdata);
}

/******************************************************************************
************************Function:
uint8 Main_GetWeekFromDay(unsigned char year,unsigned char month,unsigned 
char day) 
**Author: lxw
**Description:
**Input: 
**Output: 
*******************************************************************************
*********************/
// 计算2000～2099年任一天星期几 
// year    : 00-99 
// month: 01-12 
// day     : 01-31 
uint8 Main_GetWeekFromDay(unsigned char year,unsigned char month,unsigned char day) 
{ 
    if( month == 1 || month == 2 )   
    { 
        month += 12; 
        if( year > 0 ) 
            year--; 
        else 
            year = 4; 
    } 

    // 返回星期几(星期一用1表示，而星期天用7表示) 
    return 1+(( day + 2*month + 3*(month+1)/5 + year + year/4 ) %7); 
}


/****************************************************************************************************
**Function:
   void  Rtc_Set(Rtc_Type *pData)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void  Rtc_Set(Rtc_Type *pData)
{
	uint8 DataBuffer[8];
	Rtc_Init();

	pData->Week = Main_GetWeekFromDay(pData->Year,pData->Month,pData->Day);
	DataBuffer[0] = Rtc_SwapByte(((pData->Year/10)<<4)|(pData->Year%10)); 
	DataBuffer[1] = Rtc_SwapByte(((pData->Month/10)<<4)|(pData->Month%10));//(pData->Month); 
	DataBuffer[2] = Rtc_SwapByte(((pData->Day/10)<<4)|(pData->Day%10));//(pData->Day); 
	DataBuffer[3] = Rtc_SwapByte(((pData->Week/10)<<4)|(pData->Week%10));; 
	DataBuffer[4] = Rtc_SwapByte(((pData->Hour/10)<<4)|(pData->Hour%10));//(pData->Hour); 
	DataBuffer[5] = Rtc_SwapByte(((pData->Minute/10)<<4)|(pData->Minute%10));//(pData->Minute);
	DataBuffer[6] = Rtc_SwapByte(((pData->Second/10)<<4)|(pData->Second%10));//(pData->Second);
	Rtc_WriteNBytes(RTC_ADD|RTC_COMM_ALL_REALTIME,DataBuffer,7);
	
	memset(DataBuffer,0,sizeof(DataBuffer));
}
/****************************************************************************************************
**Function:
   void  Rtc_Set(Rtc_Type *pData)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void  Rtc_WlarmSet(Rtc_WlarmType *pData)
{
	uint8 DataBuffer[3];
	Rtc_Init();
	Rtc_ReadNBytes(RTC_ADD|RTC_COMM_STATUS_REG1,DataBuffer,1);
	if(pData->Flag == STD_TRUE)
	{
		DataBuffer[0] = 0x20;
		Rtc_WriteNBytes(RTC_ADD|RTC_COMM_STATUS_REG2,DataBuffer,1);
		DataBuffer[0] = 0;//Rtc_SwapByte(((pData->Week/10)<<4)|(pData->Week%10)) ;
		DataBuffer[1] = Rtc_SwapByte(((pData->Hour/10)<<4)|(pData->Hour%10))| 0x01; //(pData->Hour); 
		DataBuffer[2] = Rtc_SwapByte(((pData->Minute/10)<<4)|(pData->Minute%10))| 0x01; //(pData->Minute);
		Rtc_WriteNBytes(RTC_ADD|RTC_COMM_INT1,DataBuffer,3);
	}
	else if(pData->Flag == STD_FALSE)
	{
		DataBuffer[0] = 0;
		Rtc_WriteNBytes(RTC_ADD|RTC_COMM_STATUS_REG2,DataBuffer,1);
	}
	else 
	{
		DataBuffer[0] = 0x0;
		Rtc_WriteNBytes(RTC_ADD|RTC_COMM_STATUS_REG2,DataBuffer,1);
	}
}

const Dr_OpertonType RtcDrive=
{
	Rtc_Open,NULL, Rtc_Read, NULL,Rtc_Ioctl,
};



/****************************************************************************************************
**Function:
   int Rtc_Open(void *pData)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Rtc_Open(void *pData)
{
	Rtc_Type Rtc_data;
	
	Rtc_Init();
	RtcDrive.read(&Rtc_data);
	if((Rtc_data.Year == 0) ||(Rtc_data.Year > 99) ||(Rtc_data.Year < 18))
	{
		Rtc_data.Year = BUILD_YEAR(AccRcord.TimerBk);
		Rtc_data.Month = BUILD_MONTH(AccRcord.TimerBk);
		Rtc_data.Day = BUILD_DAY(AccRcord.TimerBk);
		Rtc_data.Hour = BUILD_HOUR(AccRcord.TimerBk);
		Rtc_data.Minute = BUILD_MINUTE(AccRcord.TimerBk);
		Rtc_data.Second = BUILD_SECOND(AccRcord.TimerBk);
		RtcDrive.ioctl(RTC_CLOCKSET,&Rtc_data);
		ProtoAnaly_RtcLocalTime = AccRcord.TimerBk;
	}
}
/****************************************************************************************************
**Function:
   int8 Rtc_Read(void *pData)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
Std_ReturnType Rtc_Read(void *pData)
{
	uint8 DataBuffer[8];
	static uint8 cnt;
	if(Rtc_ReadNBytes(RTC_ADD|RTC_COMM_ALL_REALTIME,DataBuffer,7) == RTC_SUCCESS)
	{
		((Rtc_Type *)pData)->Year = ((Rtc_SwapByte(DataBuffer[0])&0xf0)>>4)*10 + (Rtc_SwapByte(DataBuffer[0])&0x0f);
		((Rtc_Type *)pData)->Month =((Rtc_SwapByte(DataBuffer[1])&0xf0)>>4)*10 + (Rtc_SwapByte(DataBuffer[1])&0x0f);
		((Rtc_Type *)pData)->Day = ((Rtc_SwapByte(DataBuffer[2])&0xf0)>>4)*10 + (Rtc_SwapByte(DataBuffer[2])&0x0f);
		((Rtc_Type *)pData)->Week = ((Rtc_SwapByte(DataBuffer[3])&0xf0)>>4)*10 + (Rtc_SwapByte(DataBuffer[3])&0x0f);
		((Rtc_Type *)pData)->Hour =((Rtc_SwapByte(DataBuffer[4])&0x30)>>4)*10 + (Rtc_SwapByte(DataBuffer[4])&0x0f);
		((Rtc_Type *)pData)->Minute = ((Rtc_SwapByte(DataBuffer[5])&0xf0)>>4)*10 + (Rtc_SwapByte(DataBuffer[5])&0x0f);
		((Rtc_Type *)pData)->Second = ((Rtc_SwapByte(DataBuffer[6])&0xf0)>>4)*10 + (Rtc_SwapByte(DataBuffer[6])&0x0f);

		if (((Rtc_Type *)pData)->Second < 58 )
			{
			  if ( ++cnt >= 5)
			  	{
			  	  cnt = 0;
			  		((Rtc_Type *)pData)->Second += 2;
						Rtc_Ioctl(RTC_CLOCKSET,(Rtc_Type *)pData);
			  	}
			}
		return E_OK;
	}
	return E_NOT_OK;
}


/****************************************************************************************************
**Function:
   Std_ReturnType Rtc_Ioctl(Rtc_CmdType Cmd,void *pData)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
Std_ReturnType Rtc_Ioctl(uint8 Cmd,void *pData)
{
	Std_ReturnType Err;
	Err = E_INPAR_ERR;
	if(Cmd < SETRTCCONTROLMAX )
	{
		switch(Cmd)
		{
			case RTC_CLOCKSET:
			{
				Rtc_Set(pData);
				Err = E_OK;
			//RecordList_StorePara(0); /* 存储备份时间*/
			}break;
			case RTC_WLARMSET:
			{
				Rtc_WlarmSet(pData);
			}break;
			default:break; 
		}
	}
	return Err;
}
//Rtc_Type Rtc_rdata;

/****************************************************************************************************
**Function:
   int8 ADCUser_Ioctl(void *pData)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void RTC_Check(void)
{
	Rtc_Type Rtc_data;
	//Rtc_Open(NULL);
	Rtc_data.Year = 19;
	
	Rtc_data.Month = 11;
	Rtc_data.Day = 4;
	Rtc_data.Hour = 16;
	Rtc_data.Minute = 29;
	Rtc_data.Second = 00;
	//Rtc_Ioctl(RTC_CLOCKSET,&Rtc_data);
	
	Rtc_Set(&Rtc_data);


}
#endif
