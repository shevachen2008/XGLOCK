/****************************************************************************************
文件:   Comble.c
说明:   STM32 与 52832 通信业务协议 
****************************************************************************************/
//头文件.
#include "main.h"


//uint8_t	  com_sendbuff[PROTO_COMM_BRG_LENTH];
static uint8_t	  com_getbuff [PROTO_COMM_BRG_LENTH];
static uint8_t    Proto_PackOutbuf[PROTO_COMM_BRG_LENTH];
static uint8_t    SendDataBuf[PROTO_COMM_BRG_LENTH];
static uint8_t    SendLockDataBuf[PROTO_COMM_BRG_LENTH];

extern U8 HALL_Status;

static void ProtoAnaly_PowerOnIn(uint8 *pInData ,uint8 *pOutData)
	{
		pOutData[0] = 2; /* 长度*/
		pOutData += 14;

		CodeType = BLEUnLockType;
		Access_Unlock();

		pOutData[1]= E_OK;
	}

static void ProtoAnaly_PowerOffIn(uint8 *pInData ,uint8 *pOutData)
	{
		pOutData[0] = 2; /* 长度*/
		pOutData += 14;
		pOutData[1]= E_OK ;

		CodeType = BLELockType;
		Access_Lock();
	}

static void ProtoAnaly_SetTimeIn(uint8_t *pInData ,uint8_t *pOutData)
	{
		Rtc_Type Rtc_data;

		pOutData[0] = 2; 			      /* 长度*/
		pOutData += 14;
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

void ProtoAnaly_UpdateTime(void)
	{
		Rtc_Type Rtc_data;
		if(Rtc_Read(&Rtc_data)!= E_OK)
		{
		  return;
		}
		if((Rtc_data.Year > 99 )||(Rtc_data.Month > 12)||(Rtc_data.Day> 31)
		||(Rtc_data.Hour> 23)||(Rtc_data.Minute> 60)||(Rtc_data.Second> 60))
		{
		  return;
		}
		memcpy(&ProtoAnaly_Rtcinfo,&Rtc_data,sizeof(ProtoAnaly_Rtcinfo));
		ProtoAnaly_RtcLocalTime = (((uint32)ProtoAnaly_Rtcinfo.Year << 26) & 0xfc000000) + (((uint32)ProtoAnaly_Rtcinfo.Month << 22) & 0x3c00000)
		+ (((uint32)ProtoAnaly_Rtcinfo.Day << 17) & 0x3e0000) + (((uint32)ProtoAnaly_Rtcinfo.Hour << 12) & 0x1f000)
		+ (((uint32)ProtoAnaly_Rtcinfo.Minute << 6) & 0xfc0) + (ProtoAnaly_Rtcinfo.Second & 0x3f);
		AccRcord.TimerBk = ProtoAnaly_RtcLocalTime;
		ProtoAnaly_Rtcinfo.Week = Main_GetWeekFromDay(ProtoAnaly_Rtcinfo.Year,ProtoAnaly_Rtcinfo.Month,ProtoAnaly_Rtcinfo.Day);
	}


void ProtoAnaly_LoraSetTime(uint32 rtime)
{
	Rtc_Type Rtc_data;

	Rtc_data.Year = ((rtime& 0xfc000000) >> 26) ;
	Rtc_data.Month = ((rtime& 0x3c00000) >> 22) ;
	Rtc_data.Day = ((rtime& 0x3e0000) >> 17) ;
	Rtc_data.Hour = ((rtime& 0x1f000) >> 12) ;
	Rtc_data.Minute = ((rtime& 0xfc0) >> 6) ;
	Rtc_data.Second = (rtime& 0x3f) ;
	Rtc_Ioctl(RTC_CLOCKSET,&Rtc_data);
 }


static void ProtoAnaly_GetTimeIn(uint8_t *pInData ,uint8_t *pOutData)
	{
	  pOutData[0] = 8; /* 长度*/     // 8
		pOutData += 14;
		
		ProtoAnaly_UpdateTime();
		pOutData[1]= ProtoAnaly_Rtcinfo.Year;
		pOutData[2] = ProtoAnaly_Rtcinfo.Month;
		pOutData[3] = ProtoAnaly_Rtcinfo.Day;
		pOutData[4]= ProtoAnaly_Rtcinfo.Hour;
		pOutData[5] =ProtoAnaly_Rtcinfo.Minute;
		pOutData[6] = ProtoAnaly_Rtcinfo.Second;
		pOutData[7] = Ver;    
	}
uint16 intemp;
static void ProtoAnaly_LoadRecordIn(uint8_t *pInData ,uint8_t *pOutData)
{
	uint16 index;
	
	pOutData[0] = 7+ ACCESS_RECORD_ONCE_MAX; 			/* 长度*/
	pOutData += 14;
	pOutData[1] = E_OK;
	index = BUILD_UINT16(pInData[2], pInData[1]);
	intemp = index;
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

Std_ReturnType ProtoAnaly_AddId(uint8 idtpye,uint8 *idbuf)
{
	uint16 index = 0xff;
	uint8  flashoffset;
	
	uint8 type;
	uint8 err = E_NOT_OK;
	uint32 keyId;

	keyId = BUILD_UINT32(idbuf[3], idbuf[2], idbuf[1], idbuf[0]);

	BlackList_RemoveKey(idtpye, keyId);
	for (uint8 j=0;j<SERVE_KEY_PAGEMAX;j++ )
	{
	  ServeKeyList_Readflash( j );	
		for(uint8 i=0; i<SERVE_KEY_MAX; i++)
		{
			if(keyId == LockKeyServeKeyList.KeyList[i].KeyId)
			{//强制修改有效时间
				LockKeyServeKeyList.KeyList[i].EndTime = BUILD_UINT32(idbuf[7], idbuf[6], idbuf[5], idbuf[4]);
				LockKeyServeKeyList.KeyList[i].StartTime = BUILD_UINT32(idbuf[11], idbuf[10], idbuf[9], idbuf[8]);
				LockKeyServeKeyList.KeyList[i].Cycle = idbuf[12];
				
				ServeKeyList_Writeflash( j );
				return E_OK;
			}
			if(index == 0xff)
			{
				if(LockKeyServeKeyList.KeyList[i].KeyId == 0xffffffff)
				{
				    flashoffset = j;
					  index = i;
				}
			}
		}
	}
	if(index != 0xff)
	{//增新
	  ServeKeyList_Readflash( flashoffset );
		LockKeyServeKeyList.KeyList[index].Type = idtpye;
		LockKeyServeKeyList.KeyList[index].KeyId = keyId;
		LockKeyServeKeyList.KeyList[index].EndTime = BUILD_UINT32(idbuf[7], idbuf[6], idbuf[5], idbuf[4]);
		LockKeyServeKeyList.KeyList[index].StartTime = BUILD_UINT32(idbuf[11], idbuf[10], idbuf[9], idbuf[8]);
		LockKeyServeKeyList.KeyList[index].Cycle = idbuf[12];
		LockKeyStatistics.ServKeyCount++;
		ServeKeyList_Writeflash( flashoffset );
		return E_OK;
	}

	for (uint8 j=0;j<SERVE_KEY_PAGEMAX;j++ )
	{
	  ServeKeyList_Readflash( j );	
		for(uint8 i=0; i<SERVE_KEY_MAX; i++)
		{
			type = LockKeyServeKeyList.KeyList[i].Type;
			if(((type == KEY_TYPE_ONE_TIME) || (type == KEY_TYPE_TIME_SLOT_7) 
				|| (type == KEY_TYPE_TIME_SLOT_8)) && (LockKeyServeKeyList.KeyList[i].KeyId != 0xffffffff))
			{
				LockKeyServeKeyList.KeyList[i].Type = idtpye;
				LockKeyServeKeyList.KeyList[i].KeyId = keyId;
				LockKeyServeKeyList.KeyList[i].EndTime = BUILD_UINT32(idbuf[7], idbuf[6], idbuf[5], idbuf[4]);
				LockKeyServeKeyList.KeyList[i].StartTime = BUILD_UINT32(idbuf[11], idbuf[10], idbuf[9], idbuf[8]);
				LockKeyServeKeyList.KeyList[i].Cycle = idbuf[12];
				
				ServeKeyList_Writeflash( j );
				return E_OK;
			}
		}
	}
	return err;
}


#if 1
U32 addId;
U16 addId1;
U16 addId2;
U8 TestAddFlag;
uint16_t bcdbuf[10],hexbuf[10];
uint16_t bcddata;

static uint8_t HexstoBcd( uint8_t dat ) 
{
	return (dat/10)*16 + dat%10;
}


static uint16_t HexstoBcds( uint16_t dat )      //  读取时候用
  {
		hexbuf[1] = dat%100;
		hexbuf[2] = dat/100;

		bcdbuf[1] = HexstoBcd( hexbuf[1] );
		bcdbuf[2] = HexstoBcd( hexbuf[2] );
    bcddata = C2BYTES2HEX(&bcdbuf[1]);
    return bcddata;
  }

void TestAddCode (void)
{
  
  if ( 1 == TestAddFlag )  return;
	TestAddFlag = 1;
  addId1 = 0x3e7;
	
	for (uint8 j=0;j<SERVE_KEY_PAGEMAX;j++ )
	{
	  ServeKeyList_Readflash( j );	
		for(uint8 i=0; i<SERVE_KEY_MAX; i++)
		{   addId1++; 
		    addId2 = HexstoBcds ( addId1 ); 
		    addId = 0xFFFF0000 + addId2;
				LockKeyServeKeyList.KeyList[i].Type = 0x02;
				LockKeyServeKeyList.KeyList[i].KeyId = addId;
				LockKeyServeKeyList.KeyList[i].EndTime = 0xffffffff;
				LockKeyServeKeyList.KeyList[i].StartTime = 0x50000000;
				LockKeyServeKeyList.KeyList[i].Cycle = 0x80;
				
				//ServeKeyList_Writeflash( j );
		}
		ServeKeyList_Writeflash( j );
	}
	
	 Line_1A_WT588S( 0x08 ); 	 
}
#endif


static void ProtoAnaly_SetListIn(uint8 *pInData ,uint8 *pOutData)
	{
		pOutData[0] = 4;			/* 长度*/
		pOutData += 14;

	 if ((pInData[1] == KEY_TYPE_PASSWORD)|| (pInData[1] == KEY_TYPE_CARD))	 
	 	{
			pOutData[1] = ProtoAnaly_AddId(pInData[1],&pInData[2]);

			pOutData[2]=(pInData[3+(pInData[2]<<3)]);
			pOutData[3]=(pInData[4+(pInData[2]<<3)]);
	 	}
	}


Std_ReturnType ProtoAnaly_RemoveId(uint8 idtpye,uint8* id)
{	
	uint8 year, month, day, i;
	uint32 tempTime;
	
	uint32 keyId;
	
	if(idtpye == KEY_TYPE_IDENTITY_CARD)
	{
		keyId = BUILD_UINT32(id[7],id[6],id[5],id[4]);
	}
	else
	{
		keyId = BUILD_UINT32(id[3],id[2],id[1],id[0]);
	}
	
	ProtoAnaly_UpdateTime();
	
	if((keyId == 0xffffffff) || (keyId == 0))
	{
		return E_OK;
	}
	else
	{
		for (uint8 j=0;j<SERVE_KEY_PAGEMAX;j++ )
		{
	    	ServeKeyList_Readflash( j );	
			for(i=0; i<SERVE_KEY_MAX; i++)
			{
				if(idtpye == LockKeyServeKeyList.KeyList[i].Type)
				{
					if(LockKeyServeKeyList.KeyList[i].KeyId == keyId)
					{
						if((idtpye == KEY_TYPE_ONE_TIME) || (idtpye == KEY_TYPE_FOREVER)
						|| (idtpye == KEY_TYPE_TIME_SLOT_7) || (idtpye == KEY_TYPE_CLEAR)
						|| (idtpye == KEY_TYPE_TIME_SLOT_8))
						{
							tempTime = LockKeyServeKeyList.KeyList[i].StartTime;
							year = BUILD_YEAR(tempTime);
							month = BUILD_MONTH(tempTime);
							day = BUILD_DAY(tempTime);
							if((ProtoAnaly_Rtcinfo.Year == year)&&(ProtoAnaly_Rtcinfo.Month == month)
							&&(ProtoAnaly_Rtcinfo.Day == day))
							{ /* 加入黑名单*/
								BlackList_Add(LockKeyServeKeyList.KeyList[i].Type, LockKeyServeKeyList.KeyList[i].KeyId,
								BUILD_TIME(ProtoAnaly_Rtcinfo.Year,ProtoAnaly_Rtcinfo.Month,ProtoAnaly_Rtcinfo.Day,
								23,59,59));
								BlackList_StorePara();
							}
						}
						memset(&LockKeyServeKeyList.KeyList[i], 0xff, sizeof(LockKey_ServeKeyType));
						if(LockKeyStatistics.ServKeyCount>0)
						{
							LockKeyStatistics.ServKeyCount--;
						}
						
						ServeKeyList_Writeflash( j );
						//ServeKeyList_StorePara1();//
						return E_OK;
			   		}
		    	}	
			}
		}
		if((idtpye == KEY_TYPE_ONE_TIME) || (idtpye == KEY_TYPE_FOREVER)
		||(idtpye == KEY_TYPE_CLEAR) || (idtpye == KEY_TYPE_TIME_SLOT_8))
						{
			 /* 加入黑名单*/
			BlackList_Add(idtpye, keyId,
			BUILD_TIME(ProtoAnaly_Rtcinfo.Year,ProtoAnaly_Rtcinfo.Month,ProtoAnaly_Rtcinfo.Day,
				23,59,59));
			BlackList_StorePara();
			}
		else if(idtpye == KEY_TYPE_TIME_SLOT_7)
			{
		/* 加入黑名单*/
			BlackList_Add(idtpye, keyId,
			BUILD_TIME(ProtoAnaly_Rtcinfo.Year,ProtoAnaly_Rtcinfo.Month,(ProtoAnaly_Rtcinfo.Day+1),
		23,59,59));
		BlackList_StorePara();
		}
	}
	return E_OK;
}


static void ProtoAnaly_RomveIdin(uint8 *pInData ,uint8 *pOutData)
{
	pOutData[0] = 4;			/* 长度*/
	pOutData += 14;	

	pOutData[1] = ProtoAnaly_RemoveId(pInData[1],&pInData[2]);

	pOutData[2]=(pInData[2+(1<<3)]);
	pOutData[3]=(pInData[3+(1<<3)]);
}

static void ProtoAnaly_ReadDeviceInfoExtIn(uint8_t *pInData ,uint8_t *pOutData)
{
	pOutData[0] = 2+2; /* 长度*/
	pOutData += 14;
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
			pOutData -= 14; 
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
	//uint16 temp;
	//uint8 ret;
	
	pOutData[0] = 2; 			/* 长度*/
	pOutData += 14;
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
}

static void ProtoAnaly_FactoryTest(uint8_t *pInData ,uint8_t *pOutData)
{
#if 0
		pOutData[0] = 2; /* 长度*/
		pOutData += 2;
		
		pOutData[1] = E_OK;

		switch(pInData[1])
		{
			case COMD_FATORY_ENTERN_SLEEP:
			{
				/*BLE_UPDATA_DISCONNECT_TIMER(2000/64);
				Access_CardLearnTimer64 = 0;
				Access_BeepStart(BEEP_FAST,1);*/
				Factory_State = COMD_FATORY_ENTERN_SLEEP;
				Factory_Sub = CASE_IN;
				Access_BeepStart(BEEP_FAST,1);
				//BLE_UPDATA_DISCONNECT_TIMER(2000);
				break;
			}
			case COMD_FATORY_PARA_INIT:
			{
				Sys_ParainitFirst();
				Sys_StorePara();
				Sys_McuRestFlag=STD_TRUE;
				break;
			}
			case COMD_FATORY_MOTOR_TEST:
			{
				Factory_State = COMD_FATORY_MOTOR_TEST;
				Factory_Sub = CASE_IN;
				Access_BeepStart(BEEP_FAST,1);
				break;
			}
			default: 
			{
				pOutData[1] = E_KEY_NO_SUPPORT;
				break;
			}
		}
#endif
}
U32 timeset;
U32 PswdTemp;
U8  Psk_BUF[16];
static void ProtoAnaly_Aeskeyset(uint8_t *pInData ,uint8_t *pOutData)
{
	//U32 timeset;
	pOutData[0] = 2; /* 长度*/
	pOutData += 14;
	pOutData[1]= E_OK;

	Sys_PataCfg.Pswd = PswdTemp;
	Sys_StoreFlag = STD_TRUE;

  memcpy( Psk_BUF, &pInData[1], 16 );
	NBInitFlag = 1;
	NBdelayCnt = 5;
	NBstatus = 0;
	timeset = BUILD_UINT32(pInData[20],pInData[19],pInData[18],pInData[17] );
  ProtoAnaly_LoraSetTime( timeset );
}

U8 initpsw[5] = {0x12,0x34,0x56,0x78,0x90};
U8 initpswFlag;
U8 SendDelayFlag;

static void ProtoAnaly_inputPcode(uint8_t *pInData ,uint8_t *pOutData)
{
	uint8 ret = E_NOT_OK;
	
	pOutData[0] = 2;						/* 长度*/
	pOutData += 14;

  if (0 == memcmp(initpsw,&pInData[2],5 )&& (Sys_PataCfg.Pswd == 0x12345678))
  {
		initpswFlag = 1;
		CodeType = PSWUnLockType;
    Access_Unlock();
    return;
  }
  initpswFlag = 0;
	num_weishu = pInData[1];
	Disable_USART4IRQ();
	ret =	Touch_KeyProc(&pInData[2],pInData[1]);    
	HARDWARE_SetupUSART4();
	pOutData[1] = ret; 
}

static void ProtoAnaly_inputPcard(uint8_t *pInData ,uint8_t *pOutData)
{
  	uint8 ret = E_NOT_OK;
		
		pOutData[0] = 2; 			      /* 长度*/
		pOutData += 14;
		Disable_USART4IRQ();
		ret = LockKey_Check_CardKey(KEY_TYPE_CARD,&pInData[2]);    //
		HARDWARE_SetupUSART4();
		if ( ret == E_OK )
			{
				CodeType = FOBUnLockType;
				Access_Unlock();
			}
		pOutData[1] = ret; 
}

static void ProtoAnaly_SetParaDefIn(uint8_t *pInData ,uint8_t *pOutData)
{	
	pOutData[0] = 2;	/* 长度*/
	pOutData += 14;
	pOutData[1] = E_OK;

	Sys_Parainit();
	Sys_StorePara();
	Line_1A_WT588S( 0x2d );
}

static void ProtoAnaly_LockFromKeyBoard(uint8_t *pInData ,uint8_t *pOutData)
{
	pOutData[0] = 2; /* 长度*/
	pOutData += 14;
	pOutData[1]= E_OK ;
	
	CodeType = KeyPadLockType;
  Access_Lock();
}

static void ProtoAnaly_Modeset(uint8_t *pInData ,uint8_t *pOutData)
{
	pOutData[0] = 2; /* 长度*/
	pOutData += 14;
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
	pOutData += 14;
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
		pOutData += 14;
		pOutData[1]= E_OK ;

    Sys_PataCfg.CfgLog[0] = pInData[2];
	  Sys_PataCfg.CfgLog[1] = pInData[1];

		Sys_StoreFlag = STD_TRUE;
	}

 static void ProtoAnaly_Logquery(uint8_t *pInData ,uint8_t *pOutData)
 {
	 pOutData[0] = 2+1; /* 长度*/
	 pOutData += 14;
 
	 pOutData[1] = Sys_PataCfg.CfgLog[1];
	 pOutData[2] = Sys_PataCfg.CfgLog[0];
 }


 const ProtoAnaly_ComdUpType ProtoAnalyAppTable[]=
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
		COMD_FACTORY_TEST,ProtoAnaly_FactoryTest,                 //   0x85 
		COMD_AESKEY_SET,ProtoAnaly_Aeskeyset,                     //   0xA1
		COMD_INPUTPCODE_SET,ProtoAnaly_inputPcode,                //   0xA2    
		COMD_INPUTPCARD_SET,ProtoAnaly_inputPcard,                //   0xA3  
		COMD_LOCK_SET,ProtoAnaly_LockFromKeyBoard,                //   0xA4      
		COMD_SET_PARA_DEF,ProtoAnaly_SetParaDefIn,                //   0x79
		COMD_MODE_SET,ProtoAnaly_Modeset, 											  //	 0xA6
		COMD_MODE_QUERY,ProtoAnaly_Modequery, 										//	 0xA7
		COMD_LOG_SET,ProtoAnaly_Logset,                           //	 0xB2    
		COMD_LOG_QUERY,ProtoAnaly_Logquery, 											//	 0xB3 			

 };
#define PROTOANALY_APPCOMMAND_NUM (sizeof(ProtoAnalyAppTable) / sizeof(ProtoAnalyAppTable[0]))

//mac层处理列表

// 唤醒码需要 25个以上 
uint8_t search( uint8_t a[], uint8_t n, uint8_t x )
  {
    uint8_t i,count = 0;
    for ( i = 0;i < n;i++ )
      {
        if ( a[i] == x )
          {
      	 	count += 1; 
      	 	return i;
      	  }
      }
    if ( count != 0 )   return 0xff;
	  else return i;
  }

 /****************************************************************************************************
 **Function:
	  Std_ReturnType ProtoAnaly_CmdAppProc(uint8 *pindat,uint8 *poutdat)
 **Description:  
 **Input: 
 **Output: 
 ****************************************************************************************************/
Std_ReturnType ProtoAnaly_CmdAppProc(uint8 *pindat,uint8 *poutdat)
{
	uint8 i;
	for(i= 0;i< PROTOANALY_APPCOMMAND_NUM;i++)
	{
		if(ProtoAnalyAppTable[i].ComdId == pindat[1])
		{
			pindat += 13;	 
			if(&(ProtoAnalyAppTable[i].comdinmanage)== NULL)
			{
				break;
			}
			ProtoAnalyAppTable[i].comdinmanage(&pindat[1],poutdat);
			poutdat[1] = ProtoAnalyAppTable[i].ComdId;
			return E_OK;
		}
	}
	return E_NOT_OK;
}

void Proto_CommHostAckPack(uint8_t* pdatin) 
	{
		uint8_t checksum;
		uint8_t lenth;
		if(pdatin[0] == 0)
		{
			return;
		}
    if ( 0x1B == pdatin[1] || 0x1C == pdatin[1] )
    {
			return;
    }
			
	  Proto_PackOutbuf[0] = HEADEDATASEND;
		
	  Proto_PackOutbuf[3] = 0x55;

    if ( MOTOR_POSIT_LOCK == Motor_PositStatus )   Proto_PackOutbuf[4] = 0x01;
    if ( MOTOR_POSIT_UNLOCK == Motor_PositStatus )   Proto_PackOutbuf[4] = 0x02;
    if ( MOTOR_POSIT_MIDDLE == Motor_PositStatus )   Proto_PackOutbuf[4] = 0x03;
		
	  Proto_PackOutbuf[5] = (U8)(Access_BatteryData);
    
		memcpy(&Proto_PackOutbuf[6],Sys_PataCfg.MacBuf,6 );
		Sys_GenFun32To8(Sys_PataCfg.Pswd,(&Proto_PackOutbuf[12]));

		pdatin[0] += 14;

		//增加checksum 
		checksum = Sys_GenFunChecksum(pdatin);
		Proto_PackOutbuf[pdatin[0] + 1] = checksum;
		lenth = pdatin[0]+2;

	  HAL_UART_Transmit(&UART4_Handle, (uint8_t*)Proto_PackOutbuf, lenth,2);
	}

void CLR_UART4BUF(void)
  {
		UART4_RXcnt = 0;
		memset(UART4_RXBuffer,0,sizeof( UART4_RXBuffer ));
  }

void PR_UARTDATA(void)
  {
    uint8_t  len,cnt,checksum,result;
		
		cnt = search( UART4_RXBuffer, MAX_COMBUF,HEADEDATA );
		if ( cnt != 0xff )
			{
				len = UART4_RXcnt - cnt;
				
				memset(com_getbuff,0,sizeof(com_getbuff));
				memcpy(com_getbuff,&UART4_RXBuffer[cnt],len );
				CLR_UART4BUF();
			}
	  else
		  {
		    CLR_UART4BUF();
			  return; 
		  }
     
		if ( HEADEDATA != com_getbuff[ 0 ])
			{
        CLR_UART4BUF();
				return; 
			}

		checksum = com_getbuff[ com_getbuff[PROTO_COMM_COMD_LENTHADD] -1 ];
		
		/*
		if ( checksum != Sys_GenFunChecksum(&com_getbuff[PROTO_COMM_COMD_LENTHADD]))
		    {
					CLR_UART4BUF();
					return; 
		    }
	  */
	  
		PwrStopCnt = 10;    //  10 
		PswdTemp = BUILD_UINT32(com_getbuff[15], com_getbuff[14], com_getbuff[13], com_getbuff[12]);

    memset(Proto_PackOutbuf,0,sizeof ( Proto_PackOutbuf ));

	  result = ProtoAnaly_CmdAppProc(&com_getbuff[PROTO_COMM_COMD_OFFSET], &Proto_PackOutbuf[PROTO_COMM_COMD_LENTHADD]);
		if(result == E_OK)
		{
			Proto_CommHostAckPack(&Proto_PackOutbuf[PROTO_COMM_COMD_LENTHADD]);
		}
}

void SendDataBle (uint8_t mode,uint8_t stataus)
{
	uint8_t checksum;
	uint8_t lenth;
	GPIO_SET_A3(1);
  delay_ms(100);
	GPIO_SET_A3(0);

  delay_ms(500);	
	SendDataBuf[0] = HEADEDATASEND;
	SendDataBuf[1] = 14+3;
  SendDataBuf[2]	= 0xA8; 
	SendDataBuf[3] = 0x55;
	
	if ( MOTOR_POSIT_LOCK == Motor_PositStatus )	   SendDataBuf[4] = 0x01;
	if ( MOTOR_POSIT_UNLOCK == Motor_PositStatus )	 SendDataBuf[4] = 0x02;
	if ( MOTOR_POSIT_MIDDLE == Motor_PositStatus )	 SendDataBuf[4] = 0x03;
	
	SendDataBuf[5] = (U8)(Access_BatteryData);
	
	memcpy(&SendDataBuf[6],Sys_PataCfg.MacBuf,6 );
	Sys_GenFun32To8(Sys_PataCfg.Pswd,(&SendDataBuf[12]));
	
	SendDataBuf[16] = mode;
	SendDataBuf[17] = stataus;
	//增加checksum 
	checksum = Sys_GenFunChecksum(&SendDataBuf[1]);
	SendDataBuf[18] = checksum;
	lenth = 19;
	
	HAL_UART_Transmit(&UART4_Handle, (uint8_t*)SendDataBuf, lenth,2);
}


void LowBatteryMsgSendBle(void)
{
	SendDataBle(0x01,0x01);
}

void NormalBatteryMsgSendBle(void)
{
	SendDataBle(0x01,0x02);
}


void HallSwitchMsgSendBle(uint8_t mode,uint8_t stataus)
{
 // if ( StopSendHallSwitchMsgflag )  return;
    SendDataBle(mode,stataus);    // DEBUG 
}

void ResetMsgSendBle(void)
{
  SendDataBle(0x03,0x01);
}

void LockPadMsgSendBle(void)
{
  SendDataBle(0x04,0x01);
}


void ResponseLockData(uint8_t cmd,uint8_t result)
{
  uint8_t checksum;
	uint8_t lenth;
	SendLockDataBuf[0] = HEADEDATASEND;
	SendLockDataBuf[1] = 0x10;
  SendLockDataBuf[2] = cmd; 
	SendLockDataBuf[3] = 0x55;

	if ( MOTOR_POSIT_LOCK == Motor_PositStatus )	   SendLockDataBuf[4] = 0x01;
	if ( MOTOR_POSIT_UNLOCK == Motor_PositStatus )	 SendLockDataBuf[4] = 0x02;
	if ( MOTOR_POSIT_MIDDLE == Motor_PositStatus )	 SendLockDataBuf[4] = 0x03;

	SendLockDataBuf[5] = (U8)(Access_BatteryData);

	memcpy(&SendLockDataBuf[6],Sys_PataCfg.MacBuf,6 );
	Sys_GenFun32To8(Sys_PataCfg.Pswd,(&SendLockDataBuf[12]));

  SendLockDataBuf[ 16 ] =  result;

	//增加checksum 
	checksum = Sys_GenFunChecksum(&SendLockDataBuf[1]);
	SendLockDataBuf[17] = checksum;
	lenth = 18;
	
	HAL_UART_Transmit(&UART4_Handle, (uint8_t*)SendLockDataBuf, lenth,2);
}


