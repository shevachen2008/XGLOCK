/****************************************************************************************
文件:   Comble.c
说明:   STM32 与 云平台 通信业务协议 
****************************************************************************************/
//头文件.
#include "main.h"


uint8_t REGStatus; 
uint8_t CodeType;
uint8_t CodeOtherType;

uint8_t MsgBuf[20];
uint8_t NbLogBuf[120];
uint8_t NbLogStatus;
uint8_t NBheartbeatFlag;
uint16_t NBHeartBeatCnt;

uint8_t MsgLength;
uint8_t NbLogLength;

uint32_t KeyIdSend;
uint32_t CardIdSend;

uint8_t Com_getNbBuf[50];
uint8_t Com_PutNbBuf[50];

void NB_PsmWake(void)
{
	NBINTHIGH(); 
  delay_ms(100);
	NBINTLOW(); 
}


void NB_REG(U8 Type, U32 psw,U8 result)
{
		uint8_t checksum;
		
    MsgBuf[ 0 ] = 0x5A;
    MsgBuf[ 1 ] = 0x0F;
    MsgBuf[ 2 ] = 0xB1;
    MsgBuf[ 3 ] = 0x88;
    MsgBuf[ 4 ] = Access_BatteryData;
    MsgBuf[ 5 ] = 0x01;
		MsgBuf[ 6 ] = Type;
		Sys_GenFun32To8(psw,(&MsgBuf[7]));
    MsgBuf[ 11 ] = result;
		ProtoAnaly_UpdateTime();
		Sys_GenFun32To8(ProtoAnaly_RtcLocalTime,(&MsgBuf[12]));

		checksum = Sys_GenFunChecksum(&MsgBuf[ 1 ]);
		MsgBuf[MsgBuf[ 1 ] + 1] = checksum;
		MsgLength = MsgBuf[ 1 ]+2;

    NBHeartBeatCnt = 0;

		NB_PsmWake();
		delay_ms(1000);
		Clear_Buffer();
		NBAT_SEND( "AT+QLWSTATUS?\r\n");
		delay_ms(200);
		REGStatus = 2;
		NBtimer = 600;
		PwrStopCnt = 60;
		cStatus = CONNECT_QLWSTATUS;
}

#if 0
void NB_LockMsgSend(void)
	{
	  switch ( CodeType )
			{
				case BLELockType:
					 NB_REG(BLELockType, Sys_PataCfg.Pswd,1);
				  break;
				case NBLockType:
					 NB_REG(NBLockType, 0xAAAAAAAA,1);
				  break;
				case KeyPadLockType:
					 NB_REG(KeyPadLockType, 0xAAAAAAAA,1);
					break;
				case AutoLockType:
					 NB_REG(AutoLockType, 0xAAAAAAAA,1);
					break;
				default:
					break;
	  	}
	}

void NB_UnLockMsgSend(void)
	{
	  switch ( CodeType )
			{
				case BLEUnLockType:
					 NB_REG(BLEUnLockType, Sys_PataCfg.Pswd,1);
				  break;
				case NBUnLockType:
					 NB_REG(NBUnLockType, 0xAAAAAAAA,1);
				  break;
				case PSWUnLockType:
					 NB_REG(PSWUnLockType, KeyIdSend,1);
					break;
				case FOBUnLockType:
					 NB_REG(FOBUnLockType, CardIdSend,1);
					break;
				default:
					break;
	  	}
	}

void NB_otherMsgSend(void)
	{
	  switch ( CodeOtherType )
			{
				case ContactSensorClosedType:
					 NB_REG(ContactSensorClosedType, 0xAAAAAAAA,1);
				  break;
				case ContactSensorOpenedType:
					 NB_REG(ContactSensorOpenedType, 0xAAAAAAAA,1);
				  break;
				case LowBatteryType:
					 NB_REG(PSWUnLockType, 0xAAAAAAAA,1);
					break;
				//case LockRebootType:
				//	 NB_REG(LockRebootType, 0xAAAAAAAA,1);
				//	break;
				case LockHallSwitch_CheckON:
					 NB_REG(LockHallSwitch_CheckON, 0xAAAAAAAA,1);
					break;
				case LockHallSwitch_CheckOF:
					 NB_REG(LockHallSwitch_CheckOF, 0xAAAAAAAA,1);
					break;
				default:
					break;
	  	}
	}
#endif
/****************************************************************************************
LOG
****************************************************************************************/

void NB_LockMsgLoad(void)
	{
	  U8 logTemp[4];
		//return;    //
		
		ProtoAnaly_UpdateTime();
	  switch ( CodeType )
			{
				case BLELockType: 
					if ( NBconectFlag)// && Log_EnableBLE())
						{
							NB_REG(BLELockType, Sys_PataCfg.Pswd,1);
						}
					else
						{
							Sys_GenFun32To8(Sys_PataCfg.Pswd,(&logTemp[0]));
							Access_WriteRecordFlash(logTemp, ProtoAnaly_RtcLocalTime, BLELockType,  1);
						}
					break;
				case NBLockType:
					if ( NBconectFlag && Log_EnableNB())
						{
							NB_REG(NBLockType, 0xAAAAAAAA,1);
						}
					else
						{
							memset(logTemp,0xaa,4 );
							Access_WriteRecordFlash(logTemp, ProtoAnaly_RtcLocalTime, NBLockType,  1);
						}
				  break;
				case KeyPadLockType:
					if( NBconectFlag ) // && Log_EnableKEYPAD())        ////
						{
							NB_REG(KeyPadLockType, 0xAAAAAAAA,1);
						}
					else
						{
							memset(logTemp,0xaa,4 );
							Access_WriteRecordFlash(logTemp, ProtoAnaly_RtcLocalTime, KeyPadLockType,  1);
						}
					break;
				case AutoLockType:
					if( NBconectFlag && Log_EnableAUTO())
						{
							NB_REG(AutoLockType, 0xAAAAAAAA,1);
						}
					else
						{
							memset(logTemp,0xaa,4 );
							Access_WriteRecordFlash(logTemp, ProtoAnaly_RtcLocalTime, AutoLockType,  1);
						}
					break;

				default:
					break;
	  	}
	}


void NB_UnLockMsgLoad(void)
	{
		U8 logTemp[4];
		//return;
		
		ProtoAnaly_UpdateTime();
	  switch ( CodeType )
			{
				case BLEUnLockType:
					if ( NBconectFlag && Log_EnableBLE())
						{
							NB_REG(BLEUnLockType, Sys_PataCfg.Pswd,1);
						}
					else
						{
							Sys_GenFun32To8(Sys_PataCfg.Pswd,(&logTemp[0]));
							Access_WriteRecordFlash(logTemp, ProtoAnaly_RtcLocalTime, BLEUnLockType,  1);
						}
				  break;
				case NBUnLockType:
					if ( NBconectFlag && Log_EnableNB())
						{
							NB_REG(NBUnLockType, 0xAAAAAAAA,1);
						}
					else
						{
						  memset(logTemp,0xaa,4 );
							Access_WriteRecordFlash(logTemp, ProtoAnaly_RtcLocalTime, NBUnLockType,  1);
						}
				  break;
				case PSWUnLockType:
					if ( NBconectFlag && Log_EnablePSW())
						{
							NB_REG(PSWUnLockType, KeyIdSend,1);
						}
					else
						{
							if ( 0 == initpswFlag )
								{
									Sys_GenFun32To8(KeyIdSend,(&logTemp[0]));
								}
							else
								{
									memcpy(logTemp,initpsw,4 );
								}
							Access_WriteRecordFlash(logTemp, ProtoAnaly_RtcLocalTime, PSWUnLockType,	1);
						}
				  break;
				case FOBUnLockType:
					if ( NBconectFlag && Log_EnablePSW())
						{
							NB_REG(FOBUnLockType, CardIdSend,1);
						}
					else
						{
							Sys_GenFun32To8(CardIdSend,(&logTemp[0]));
							Access_WriteRecordFlash(logTemp, ProtoAnaly_RtcLocalTime, FOBUnLockType,  1);
						}
					break;
				default:
					break;
	  	}
	}

void NB_otherMsgLoad(void)
	{
		U8 logTemp[4];
		//return;    //  
		
		ProtoAnaly_UpdateTime();
	  switch ( CodeOtherType )
			{
				case ContactSensorClosedType:
					if ( NBconectFlag && Log_EnableCONSENSOR())
						{
							NB_REG(ContactSensorClosedType, 0xAAAAAAAA,1);
						}
					else
						{
							memset(logTemp,0xaa,4 );
							Access_WriteRecordFlash(logTemp, ProtoAnaly_RtcLocalTime, ContactSensorClosedType,  1);
						}
					break;
				case ContactSensorOpenedType:
					if ( NBconectFlag && Log_EnableCONSENSOR())
						{
							NB_REG(ContactSensorOpenedType, 0xAAAAAAAA,1);
						}
					else
						{
							memset(logTemp,0xaa,4 );
							Access_WriteRecordFlash(logTemp, ProtoAnaly_RtcLocalTime, ContactSensorOpenedType,  1);
						}
				  break;
				case LowBatteryType:
					if ( NBconectFlag && Log_EnableLOWBAT())
						{
							NB_REG(LowBatteryType, 0xAAAAAAAA,1);
						}
					else
						{
							memset(logTemp,0xaa,4 );
							Access_WriteRecordFlash(logTemp, ProtoAnaly_RtcLocalTime, LowBatteryType,  1);
						}
					break;
				case LockRebootType:
					memset(logTemp,0xaa,4 );
					Access_WriteRecordFlash(logTemp, ProtoAnaly_RtcLocalTime, LockRebootType,  1);
					break;
				case LockHallSwitch_CheckON:
					if ( NBconectFlag && Log_EnableHALL())
						{
							NB_REG(LockHallSwitch_CheckON, 0xAAAAAAAA,1);
						}
					else
						{
							memset(logTemp,0xaa,4 );
							Access_WriteRecordFlash(logTemp, ProtoAnaly_RtcLocalTime, LockHallSwitch_CheckON,  1);
						}
					break;
				case LockHallSwitch_CheckOF:
					if ( NBconectFlag && Log_EnableHALL())
						{
							NB_REG(LockHallSwitch_CheckOF, 0xAAAAAAAA,1);
						}
					else
						{
							memset(logTemp,0xaa,4 );
							Access_WriteRecordFlash(logTemp, ProtoAnaly_RtcLocalTime, LockHallSwitch_CheckOF,  1);
						}
					break;
				case CutType:
					if ( NBconectFlag  )
						{
							NB_REG(CutType, 0xAAAAAAAA,1);
						}
					else
						{
							memset(logTemp,0xaa,4 );
							Access_WriteRecordFlash(logTemp, ProtoAnaly_RtcLocalTime, CutType,  1);
						}
				default:
					break;
	  	}
	}


void  NBLog_GetRecord(uint8 index,uint8 *poutdat)
{
	poutdat[0] = BUILD_TYPE(AccRcord.RecordList[index].TypeResult);
	poutdat[1] = TWOHI_UINT32(AccRcord.RecordList[index].Id);
	poutdat[2] = TWOLO_UINT32(AccRcord.RecordList[index].Id);
	poutdat[3] = ONEHI_UINT32(AccRcord.RecordList[index].Id);
	poutdat[4] = ONELO_UINT32(AccRcord.RecordList[index].Id);
	poutdat[5] = BUILD_ACTION(AccRcord.RecordList[index].TypeResult);
	poutdat[6] = TWOHI_UINT32(AccRcord.RecordList[index].Time);
	poutdat[7] = TWOLO_UINT32(AccRcord.RecordList[index].Time);
	poutdat[8] = ONEHI_UINT32(AccRcord.RecordList[index].Time);
	poutdat[9] = ONELO_UINT32(AccRcord.RecordList[index].Time);
} 

U8 NbLogLen;
#if 1
void NB_LogREG( U8 len )
{
  U8 i,checksum;
	
	if ( 0 == NBconectFlag )  return;
	NbLogBuf[ 0 ] = 0x5A;
	NbLogLen = 5 + (len * 10); 
	NbLogBuf[ 1 ]	= NbLogLen;
	NbLogBuf[ 2 ] = 0xB1;
	NbLogBuf[ 3 ] = 0x88;
	NbLogBuf[ 4 ] = Access_BatteryData;
	NbLogBuf[ 5 ] = len;

	for ( i = 0 ;i < len ;i++ )
		{
			NBLog_GetRecord( i ,&NbLogBuf[ 6 +(i*10)]);
		}		
	
	checksum = Sys_GenFunChecksum(&NbLogBuf[ 1 ]);
	NbLogBuf[NbLogBuf[ 1 ] + 1] = checksum;
	NbLogLength = NbLogBuf[ 1 ]+2;
 
  NBHeartBeatCnt = 0;

	NB_PsmWake();
	delay_ms(1000);
	Clear_Buffer();
	NBAT_SEND( "AT+QLWSTATUS?\r\n");
	delay_ms(200);
	REGStatus = 3;
	NBtimer = 600;
	PwrStopCnt = 60;
	cStatus = CONNECT_QLWSTATUS;
}


void NB_LogSend(void)
{
  char ascbuf[240];
	memset(ascbuf,0,sizeof(ascbuf));
	HexsToAscs(NbLogBuf,ascbuf,NbLogLength);
	Clear_Buffer();
	NBAT_SEND("AT+QLWNOTIFY=19,0,0,1,%d,%s,0,1\r\n",NbLogLength,ascbuf);
}
#else

U8 LogREGindex;

void NB_LogREG1(U8 i)
{
  U8 checksum;
	NbLogBuf[ 0 ] = 0x5A;
	NbLogLen = 5 + 10; 
	NbLogBuf[ 1 ]	= NbLogLen;
	//NbLogBuf[ 1 ] = 0x69;
	NbLogBuf[ 2 ] = 0xB1;
	NbLogBuf[ 3 ] = 0x88;
	NbLogBuf[ 4 ] = Access_BatteryData;
	NbLogBuf[ 5 ] = 1;

	NBLog_GetRecord( i ,&NbLogBuf[ 6 ]);
	
	checksum = Sys_GenFunChecksum(&NbLogBuf[ 1 ]);
	NbLogBuf[NbLogBuf[ 1 ] + 1] = checksum;
	NbLogLength = NbLogBuf[ 1 ]+2;


	NBAT_SEND("AT\r\n")
	delay_ms(100);
	Clear_Buffer();
	NBAT_SEND( "AT+QLWSTATUS?\r\n");
	delay_ms(200);
	REGStatus = 3;
	NBtimer = 120;
	cStatus = CONNECT_QLWSTATUS;
}


void NB_LogSend(void)
{
  char ascbuf[30];
	
	HexsToAscs(NbLogBuf,ascbuf,NbLogLength);
	Clear_Buffer();
	NBAT_SEND("AT+QLWNOTIFY=19,0,0,1,%d,%s,0,1\r\n",NbLogLength,ascbuf);
}

void NB_LogSend1(U8 i)
{
  U8 checksum;
  char ascbuf[30];
	
	NbLogBuf[ 0 ] = 0x5A;
	NbLogLen = 5 + 10; 
	NbLogBuf[ 1 ]	= NbLogLen;
	//NbLogBuf[ 1 ] = 0x69;
	NbLogBuf[ 2 ] = 0xB1;
	NbLogBuf[ 3 ] = 0x88;
	NbLogBuf[ 4 ] = Access_BatteryData;
	NbLogBuf[ 5 ] = 1;

	NBtimer = 120;

	
	NBLog_GetRecord( i ,&NbLogBuf[ 6 ]);
	checksum = Sys_GenFunChecksum(&NbLogBuf[ 1 ]);
	NbLogBuf[NbLogBuf[ 1 ] + 1] = checksum;
	NbLogLength = NbLogBuf[ 1 ]+2;
	NBLog_GetRecord( i ,&NbLogBuf[ 6 ]);
	
	checksum = Sys_GenFunChecksum(&NbLogBuf[ 1 ]);
	NbLogBuf[NbLogBuf[ 1 ] + 1] = checksum;
	NbLogLength = NbLogBuf[ 1 ]+2;

	HexsToAscs(NbLogBuf,ascbuf,NbLogLength);
	Clear_Buffer();
	NBAT_SEND("AT+QLWNOTIFY=19,0,0,1,%d,%s,0,1\r\n",NbLogLength,ascbuf);
}
#endif


/****************************************************************************************
LOG
****************************************************************************************/

void NB_msgSend(void)
{
  char ascbuf[32];
	
	memset(ascbuf,0,sizeof(ascbuf));
	HexsToAscs(MsgBuf,ascbuf,MsgLength);
	Clear_Buffer();
	NBAT_SEND("AT+QLWNOTIFY=19,0,0,1,%d,%s,0,1\r\n",MsgLength,ascbuf);
	delay_ms(200);
}


void NB_InitRegMsg(void)
{ 
	U8   checksum ,RegMsgLength;
	char RegMsgBuf[50],ascbuf[100];
	
  RegMsgBuf[ 0 ] = 0x5A;
	RegMsgBuf[ 1 ] = 44;
	RegMsgBuf[ 2 ] = 0xA5;
	RegMsgBuf[ 3 ] = 0x00;
	memcpy (&RegMsgBuf[ 4 ], Sys_PataCfg.MacBuf,6 );
	memcpy (&RegMsgBuf[ 10 ], IMEI_code,15 );
	memcpy (&RegMsgBuf[ 25 ], ICCID_code,20 );

	checksum = Sys_GenFunChecksum((U8*)&RegMsgBuf[ 1 ]);
	RegMsgBuf[RegMsgBuf[ 1 ] + 1] = checksum;    
	RegMsgLength = RegMsgBuf[ 1 ]+2;
  memset(ascbuf,0,sizeof(ascbuf));
	HexsToAscs((U8*)RegMsgBuf,ascbuf,RegMsgLength);
	Clear_Buffer();
	NBAT_SEND("AT+QLWNOTIFY=19,0,0,1,%d,%s,0,1\r\n",RegMsgLength,ascbuf);
	delay_ms(200);
}

/****************************************************************************************************



****************************************************************************************************/


static void ProtoAnaly_PowerOnIn(uint8 *pInData ,uint8 *pOutData)
 {
	 pOutData[0] = 2; /* 长度*/
	 pOutData += 2;

	 //CodeType = BLEUnLockType;
	 Access_Unlock();

	 pOutData[1]= E_OK;
 }
 
 u8 tttbuf[20];
 
static void ProtoAnaly_PowerOffIn(uint8 *pInData ,uint8 *pOutData)
 {
	 pOutData[0] = 2; /* 长度*/
	 pOutData += 2;
	 pOutData[1]= E_OK ;

	 //CodeType = BLELockType;
	 Access_Lock();
 }

static void ProtoAnaly_SetTimeIn(uint8_t *pInData ,uint8_t *pOutData)
 {
	 Rtc_Type Rtc_data;
	 pOutData[0] = 2; 					 /* 长度*/
	 pOutData += 2;
	 Rtc_data.Year = pInData[1];
	 Rtc_data.Month= pInData[2];
	 Rtc_data.Day= pInData[3];
	 Rtc_data.Hour= pInData[4];
	 Rtc_data.Minute= pInData[5];
	 Rtc_data.Second= pInData[6];
	 AccRcord.AppTimerBk = 0;
	 Rtc_Ioctl(RTC_CLOCKSET,&Rtc_data);
	 
	 pOutData[1] = E_OK;
 }

static void ProtoAnaly_GetTimeIn(uint8_t *pInData ,uint8_t *pOutData)
 {
	 pOutData[0] = 7; /* 长度*/ 		// 8
	 pOutData += 2;
	 ProtoAnaly_UpdateTime();

	 pOutData[1]= ProtoAnaly_Rtcinfo.Year;
	 pOutData[2] = ProtoAnaly_Rtcinfo.Month;
	 pOutData[3] = ProtoAnaly_Rtcinfo.Day;
	 pOutData[4]= ProtoAnaly_Rtcinfo.Hour;
	 pOutData[5] =ProtoAnaly_Rtcinfo.Minute;
	 pOutData[6]= ProtoAnaly_Rtcinfo.Second;
 }

static void ProtoAnaly_LoadRecordIn(uint8_t *pInData ,uint8_t *pOutData)
{
 uint16 index;
 
 pOutData[0] = 7+ ACCESS_RECORD_ONCE_MAX; 		 /* 长度*/
 pOutData += 2;
 pOutData[1] = E_OK;
 index = BUILD_UINT16(pInData[1], pInData[1]);
 
 if((AccRcord.recordnum == 0) || (AccRcord.Srecordindex >= AccRcord.recordnum))
 {/* 没有开门记录*/
	 pOutData[2] = 0;
	 pOutData[3] = 0;
	 pOutData[4] = pOutData[2];
	 pOutData[5] = pOutData[3];
	 memset(&pOutData[6], 0xff, 14);
	 Access_Record_Null = 1;
	 return;
 }
 if(AccRcord.Srecordindex >= ACCESS_RECORD_MAX)
 {
	 AccRcord.Srecordindex = 0;
 }
 
 if(index != 0)
 {
	 if(index == (AccRcord.Srecordindex+1))
	 {
		 AccRcord.RecordList[AccRcord.Srecordindex].Id = 0xffffffff;
		 AccRcord.Srecordindex++;
		 if(AccRcord.Srecordindex >= ACCESS_RECORD_MAX)
		 {
			 AccRcord.Srecordindex = 0;
		 }
		 RecordList_StorePara(0);
	 }
 }
 while(1)
 {
	 if(AccRcord.RecordList[AccRcord.Srecordindex].Id != 0xffffffff)
	 {
		 RecordList_StorePara(0);
		 break;
	 }
	 if((AccRcord.Srecordindex<AccRcord.Wrecordindex) || (AccRcord.Srecordindex>AccRcord.Wrecordindex))
	 {
		 if(++AccRcord.Srecordindex>=ACCESS_RECORD_MAX)
		 {
			 AccRcord.Srecordindex = 0;
		 }
		 RecordList_StorePara(0);
	 }
	 else
	 {/* 没有开门记录*/
		 pOutData[2] = 0;
		 pOutData[3] = 0;
		 pOutData[4] = pOutData[2];
		 pOutData[5] = pOutData[3];
		 memset(&pOutData[6], 0xff, 14);
		 Access_Record_Null = 1;
		 return;
	 }
 }
 index = AccRcord.Srecordindex;
 Access_GetRecord(AccRcord.Srecordindex,&pOutData[6]);
 pOutData[2] = (index>>8);
 pOutData[3] = (index&0xff);
 pOutData[4] = (AccRcord.recordnum>>8);
 pOutData[5] = (AccRcord.recordnum&0xff);
}


static void ProtoAnaly_SetListIn(uint8 *pInData ,uint8 *pOutData)
 { 
	 pOutData[0] = 2; 		 /* 长度*/
	 pOutData +=2;

	if (pInData[1] == KEY_TYPE_PASSWORD)	
	 {
	 		pOutData[1] = ProtoAnaly_AddId(pInData[1],&pInData[3]);
	 }
	else if ( pInData[1] == KEY_TYPE_CARD  )
	 {
	 		pOutData[1] = ProtoAnaly_AddId(pInData[1],&pInData[2]);
	 }
 }



static void ProtoAnaly_RomveIdin(uint8 *pInData ,uint8 *pOutData)
{
	pOutData[0] = 4;			/* 长度*/
	pOutData += 2;	

	pOutData[1] = ProtoAnaly_RemoveId(pInData[1],&pInData[2]);

	pOutData[2]=(pInData[2+(1<<3)]);
	pOutData[3]=(pInData[3+(1<<3)]);
}

static void ProtoAnaly_ReadDeviceInfoExtIn(uint8_t *pInData ,uint8_t *pOutData)
{
	pOutData[0] = 2+2; /* 长度*/
	pOutData += 2;
	pOutData[1]= E_OK;
	pOutData[2] = pInData[1];
	pOutData[3] = 0;

	if(pInData[1] == CONFIG_KEY_AUTOLOCK)
	{
		if(Sys_PataCfg.CfgFlag&AUTOLOCK)
		{
			pOutData[3] = 1;
			pOutData[4] = HI_UINT16(Sys_PataCfg.OpenLockTime);
			pOutData[5] = LO_UINT16(Sys_PataCfg.OpenLockTime);
			pOutData -= 2; 
 			pOutData[0] += 2;
		}
	}
	else if(pInData[1] == CONFIG_KEY_BTSAVEMODE)
	{
		if(Sys_PataCfg.CfgFlag&BTSAVEMODE)
		{
			pOutData[3] = 1;
		}
	}
	else if(pInData[1] == CONFIG_KEY_DETCARDSLEEP)
	{
		if(Sys_PataCfg.CfgFlag&DETCARDSLEEP)
		{
			pOutData[3] = 1;
		}
	}
	else if(pInData[1] == CONFIG_KEY_DISIBLEDOORLOCK)
	{
		if(Sys_PataCfg.CfgFlag&DISIBLEDOORLOCK)
		{
			pOutData[3] = 1;
		}
	}
}


static void ProtoAnaly_ConfigDeviceExtIn(uint8_t *pInData, uint8_t *pOutData)
{
 pOutData[0] = 2; 		 /* 长度*/
 pOutData += 2;
 pOutData[1] = E_OK;

 if(pInData[1] == CONFIG_KEY_AUTOLOCK)
 {
	 if(pInData[2] == 1)
	 {
		 Sys_PataCfg.CfgFlag |= AUTOLOCK;
		 Sys_PataCfg.OpenLockTime = BUILD_UINT16(pInData[4], pInData[3]);
	 }
	 else
	 {
		 Sys_PataCfg.CfgFlag &= ~AUTOLOCK;
	 }
 }
 else if(pInData[1] == CONFIG_KEY_BTSAVEMODE)
 {
	 if(pInData[2] == 1)
	 {
		 Sys_PataCfg.CfgFlag |= BTSAVEMODE;
	 }
	 else
	 {
		 Sys_PataCfg.CfgFlag &= ~BTSAVEMODE;
	 }
 }
 else if(pInData[1] == CONFIG_KEY_DETCARDSLEEP)
 {
	 if(pInData[2] == 1)
	 {
		 Sys_PataCfg.CfgFlag |= DETCARDSLEEP;
	 }
	 else
	 {
		 Sys_PataCfg.CfgFlag &= ~DETCARDSLEEP;
	 }
 }
 else if(pInData[1] == CONFIG_KEY_DISIBLEDOORLOCK)
 {
	 if(pInData[2] == 1)
	 {
		 Sys_PataCfg.CfgFlag |= DISIBLEDOORLOCK;
	 }
	 else
	 {
		 Sys_PataCfg.CfgFlag &= ~DISIBLEDOORLOCK;
		 if(DoorIsOpen())
		 {
			 Motor_DoorLockSt = MOTOR_DOORLOCK_OPEN;
		 }
		 else
		 {
			 Motor_DoorLockSt = MOTOR_DOORLOCK_OPENLOCK;
		 }
	 }
 }
 
 Sys_StoreFlag = STD_TRUE;
 //Sys_McuRestFlag = STD_TRUE;
}


static void ProtoAnaly_SetParaDefIn(uint8_t *pInData ,uint8_t *pOutData)
{	
	pOutData[0] = 2;	/* 长度*/
	pOutData += 2;
	pOutData[1] = E_OK;

	Sys_Parainit();
	Sys_StorePara();
	ResetMsgSendBle();
	Line_1A_WT588S( 0x2d );
}

static void ProtoAnaly_Modeset(uint8_t *pInData ,uint8_t *pOutData)
{
	pOutData[0] = 2; /* 长度*/
	pOutData += 2;
	pOutData[1]= E_OK ;

	if(pInData[1] == CONFIG_VOICE)
	{
		if(pInData[2] == 1)
		{
			Sys_PataCfg.CfgMode |= VOICEMODE;
			WT588POW_OPEN();
		}
		else if(pInData[2] == 2)
		{
			Sys_PataCfg.CfgMode &= ~VOICEMODE;
			WT588POW_CLOSE();
		}
	}
	else if(pInData[1] == CONFIG_PASS)
	{
		if(pInData[2] == 1)
		{
			Sys_PataCfg.CfgMode |= PASSMODE;
		}
		else if(pInData[2] == 2)
		{
			Sys_PataCfg.CfgMode &= ~PASSMODE;
		}
	}
	else if(pInData[1] == CONFIG_NBMODE)
	{
		if(pInData[2] == 1)
		{
			Sys_PataCfg.CfgMode |= NBOPENMODE;
		}
		else if(pInData[2] == 2)
		{
			Sys_PataCfg.CfgMode &= ~NBOPENMODE;
		}	
	}
	else if(pInData[1] == CONFIG_REFACTORY)
	{
		if(pInData[2] == 1)
		{
			Sys_PataCfg.CfgMode |= REFACTORYMODE;
		}
		else if(pInData[2] == 2)
		{
			Sys_PataCfg.CfgMode &= ~REFACTORYMODE;
		}		
	}		

	Sys_StoreFlag = STD_TRUE;
}

static void ProtoAnaly_Modequery(uint8_t *pInData ,uint8_t *pOutData)
{
	pOutData[0] = 2+1; /* 长度*/
	pOutData += 2;
	pOutData[1] = pInData[1];


	if(pInData[1] == CONFIG_VOICE)
	{
		if(Sys_PataCfg.CfgMode&VOICEMODE)
		{
			pOutData[2] = 1;
		}
		else
		{
		  pOutData[2] = 2;
		}
	}
	else if(pInData[1] == CONFIG_PASS)
	{
		if(Sys_PataCfg.CfgMode&PASSMODE)
		{
			pOutData[2] = 1;
		}
		else
		{
		  pOutData[2] = 2;
		}		
	}
	else if(pInData[1] == CONFIG_NBMODE)
	{
		if(Sys_PataCfg.CfgMode&NBOPENMODE)
		{
			pOutData[2] = 1;
		}
		else
		{
		  pOutData[2] = 2;
		}	
	}
	else if(pInData[1] == CONFIG_REFACTORY)
	{
		if(Sys_PataCfg.CfgMode&REFACTORYMODE)
		{
			pOutData[2] = 1;
		}
		else
		{
		  pOutData[2] = 2;
		}		
	}	
}

static void ProtoAnaly_Logset(uint8_t *pInData ,uint8_t *pOutData)
{
	pOutData[0] = 2; /* 长度*/
	pOutData += 2;
	pOutData[1]= E_OK ;

  Sys_PataCfg.CfgLog[0] = pInData[2];
  Sys_PataCfg.CfgLog[1] = pInData[1];

	Sys_StoreFlag = STD_TRUE;
}

static void ProtoAnaly_Logquery(uint8_t *pInData ,uint8_t *pOutData)
{
	pOutData[0] = 2+1; /* 长度*/
	pOutData += 2;


  pOutData[1] = Sys_PataCfg.CfgLog[1];
  pOutData[2] = Sys_PataCfg.CfgLog[0];
}


const ProtoAnaly_ComdUpType ProtoAnalyAppTableNB[]=
	{
		COMD_LOCK_UP,ProtoAnaly_PowerOnIn,                        //   0x1B
		COMD_LOCK_DOWN,ProtoAnaly_PowerOffIn,                     //   0x1C
		COMD_SET_TIME_APP,ProtoAnaly_SetTimeIn,                   //   0x7D
		COMD_GET_TIME_APP,ProtoAnaly_GetTimeIn,                   //   0x7C 
		COMD_LOAD_RECORD,ProtoAnaly_LoadRecordIn,                 //   0x46 
		//CMD_O2O_ADD_OTHER_KEY, ProtoAnaly_PowerOnIn,            //   0x23  
		COMD_SET_LIST,ProtoAnaly_SetListIn,                       //   0x27 
		COMD_REMOVE_ID,ProtoAnaly_RomveIdin,                      //   0x28
		COMD_READ_CONFIG_NEW, ProtoAnaly_ReadDeviceInfoExtIn,     //   0x5C
		COMD_SET_CONFIG_NEW, ProtoAnaly_ConfigDeviceExtIn,        //   0x5B
		COMD_SET_PARA_DEF,ProtoAnaly_SetParaDefIn,                //   0x79
		COMD_MODE_SET,ProtoAnaly_Modeset, 												//	 0xA6
		COMD_MODE_QUERY,ProtoAnaly_Modequery, 										//	 0xA7
		COMD_LOG_SET,ProtoAnaly_Logset,                           //	 0xB2  
		COMD_LOG_QUERY,ProtoAnaly_Logquery,                       //	 0xB3    			
	};
#define PROTOANALY_APPCOMMAND_NUM (sizeof(ProtoAnalyAppTableNB) / sizeof(ProtoAnalyAppTableNB[0]))



void Proto_CommNBPack(uint8_t* pdatin) 
	{
		uint8_t checksum;
		uint8_t lenth;
		char ascbuf[100];
		if(pdatin[0] == 0)
		{
			return;
		}

	  Com_PutNbBuf[0] = HEADEDATA;
	  Com_PutNbBuf[3] = 0x55;                 //  Reserved

		pdatin[0] += 2;

		//增加checksum 
		checksum = Sys_GenFunChecksum(pdatin);
		Com_PutNbBuf[pdatin[0] + 1] = checksum;
		lenth = pdatin[0]+2;
		
		memset(ascbuf,0,sizeof(ascbuf));
		HexsToAscs(Com_PutNbBuf,ascbuf,lenth);
	  Clear_Buffer();
	  NBAT_SEND("AT+QLWNOTIFY=19,0,0,1,%d,%s,0,1\r\n",lenth,ascbuf);
		delay_ms(200);
}

/****************************************************************************************************
 **Function:
	  Std_ReturnType ProtoAnaly_CmdAppProc(uint8 *pindat,uint8 *poutdat)
 **Description:  
 **Input: 
 **Output: 
 ****************************************************************************************************/
Std_ReturnType ProtoAnaly_CmdAppProcnNB(uint8 *pindat,uint8 *poutdat)
{
	uint8 i;
	for(i= 0;i< PROTOANALY_APPCOMMAND_NUM;i++)
	{
		if(ProtoAnalyAppTableNB[i].ComdId == pindat[1])
		{
			pindat += 1;	 
			if(&(ProtoAnalyAppTableNB[i].comdinmanage)== NULL)
			{
				break;
			}
			ProtoAnalyAppTableNB[i].comdinmanage(&pindat[1],poutdat);
			poutdat[1] = ProtoAnalyAppTableNB[i].ComdId;
			return E_OK;
		}
	}
	return E_NOT_OK;
}


void parseServerAckPackage(void)
{
	uint8_t  checksum,result;

	if ( HEADEDATA != Com_getNbBuf[ 0 ])
		{
			//CLR_UART4BUF();
			return; 
		}
	
	checksum = Com_getNbBuf[ Com_getNbBuf[PROTO_COMM_COMD_LENTHADD] -1 ];

	/*
	if ( checksum != Sys_GenFunChecksum(&com_getbuff[PROTO_COMM_COMD_LENTHADD]))
			{
				CLR_UART4BUF();
				return; 
			}
	*/

		memset(Com_PutNbBuf,0,sizeof ( Com_PutNbBuf ));

		result = ProtoAnaly_CmdAppProcnNB(&Com_getNbBuf[PROTO_COMM_COMD_OFFSET], &Com_PutNbBuf[PROTO_COMM_COMD_LENTHADD]);
		if(result == E_OK)
		{
			Proto_CommNBPack(&Com_PutNbBuf[PROTO_COMM_COMD_LENTHADD]);
		}
}

