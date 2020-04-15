
/**************************************************************************************************/ 
/* Include file                                                                                   */                                                 
/**************************************************************************************************/
#include "main.h"


Access_ParaType Access_Globle;

uint8 Access_MotorTimer64 = 0;
uint16 Access_LockTimer64 = 0;
uint16 Access_DefaultTimer64 = 0;
uint16 Access_CardLearnTimer64 = 0;
uint16 Access_ResetDeviceTimer64 = 0;

uint8  Access_AddId_NewFlag= 0;
uint8  Access_FlashProcFlag = 0;
uint32 Access_UserId_Temp = 0;
uint32 Access_ScanInex_Temp = 0;
uint8 Access_AttTIme_Temp[9] = {0};


uint16 Access_OpenRedIndex = 0;
uint16 Access_OpenWriteaIndex = 0;

uint16 Access_Store_Recordx64ms = 0;
uint8 Access_KeyDatTimex64ms=0 ;
uint8 Access_BatteryData;
uint16 Access_LockDeviceTimer64ms=0;  /* 三次错误锁住*/

uint8 Access_LockStatus=0; /* 门锁状态*/

AccRcordParaType AccRcord;
//Access_SyncPaswdType Access_SyncPaswd;

void Access_CardProcess(uint8 idtpye, uint8* pUid,Access_CardDatType *CardDat);
Std_ReturnType Access_ComTimeLoop(uint32 startTime, uint32 endTime);

extern void Tsm_Init(void);
extern void Touch_Init( void );

//extern Rtc_Type ProtoAnaly_Rtcinfo;
extern uint8  Cardetflag;
extern uint8  Lora_Timeout;
extern uint8  ProtoAnaly_LoraBrgSize;
extern uint8 SysSleepFlag;
//extern uint8  Sys_frstPoweron;
extern uint8 BattdetTimer;
/****************************************************************************************************
**Function:
	void Access_Init(void)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Access_Init(void)
{
	Access_LockTimer64 = 0;
	Access_DefaultTimer64 = 0;
	Access_CardLearnTimer64 = 0;
	Access_ResetDeviceTimer64 = 0;

	//Access_Globle.State = Sys_PataCfg.State;
	if(Sys_PataCfg.State  == WORK_LEARN_CARD)
	{
		Sys_PataCfg.State = WORK_NORMAL;
	}
	Access_Globle.DefaultTimes = 0;
	//LockCommu_Init();
	MotorDrive.open(NULL);
	BlackList_LoadPara();
	RecordList_LoadPara();
	Access_FlashArrang();
	//Factory_State = COMD_FATORY_IDLE;
}

/****************************************************************************************************
**Function:
	void Access_DetCardProc(Card_ParmType *pdat, Access_CardDatType *pAccess_CardDat)
**Author: rory
**Description:  64ms  once
**Input: 
**Output: 
****************************************************************************************************/
uint8 Access_CardDatProc(Card_ParmType *pdat, Access_CardDatType *pAccess_CardDat)
{
	Encpt_DecrpytExt(&pdat->pdat[0]);
	Encpt_DecrpytExt(&pdat->pdat[8]);

	if((pdat->pdat[0] == 0)&&(pdat->pdat[1] == 0))
	{	
		return E_OK;
	}
	else 
	{   
		uint8 year, month, day, hour, minute;
		
		uint8 cardType = pdat->pdat[0] >> 5;
		pAccess_CardDat->hotelId = BUILD_UINT16(pdat->pdat[1], pdat->pdat[0])&0x1fff;
		/* start Time */
		year = (pdat->pdat[10]>>1);
		year += 16;
		month = ((pdat->pdat[10]&0x01)<<3)|(pdat->pdat[11]>>5);
		day = pdat->pdat[11]&0x1f;
		hour = pdat->pdat[12]/4;
		minute = (pdat->pdat[12]%4)*15;
		pAccess_CardDat->Timest = BUILD_TIME(year, month, day, hour, minute, 0);

		/* end Time */
		year = (pdat->pdat[13]>>1);
		year += 16;
		month = ((pdat->pdat[13]&0x01)<<3)|(pdat->pdat[14]>>5);
		day = pdat->pdat[14]&0x1f;
		hour = pdat->pdat[15]/4;
		minute = (pdat->pdat[15]%4)*15;
		
		pAccess_CardDat->Timeend = BUILD_TIME(year, month, day, hour, minute, 0);
		pAccess_CardDat->cardtype = CARD_TYPE_NULL;
		pAccess_CardDat->HotelPaswd = BUILD_UINT16(pdat->pdat[3], pdat->pdat[2])&0x0fff;
		if(cardType == 2)
		{  /* 客人卡*/
			if((pAccess_CardDat->hotelId != Sys_PataCfg.PropertyId) || (Sys_PataCfg.PropertyId == 0))
			{
			return E_OK;
			}
			if((Access_ComTime(pAccess_CardDat->Timest, pAccess_CardDat->Timeend) != E_OK)
				|| (pAccess_CardDat->Timest < AccRcord.CustomTimerBk))
			{/*卡片失效*/
				return E_TIME_OUT;
			}
			pAccess_CardDat->deviceId = BUILD_UINT32(pdat->pdat[5], pdat->pdat[4], pdat->pdat[3], pdat->pdat[2]);
			pAccess_CardDat->devicePaswd = BUILD_UINT32(pdat->pdat[9], pdat->pdat[8], pdat->pdat[7], pdat->pdat[6]);
			pAccess_CardDat->cardtype = CARD_TYPE_GUEST;
			if(pAccess_CardDat->Timest > AccRcord.CustomTimerBk)
			{
				AccRcord.CustomTimerBk = pAccess_CardDat->Timest;
				RecordList_StorePara(0);
			}
			return E_OK;
		}
		else if(cardType == 4)
		{/* 员工卡*/
			uint8 type = (pdat->pdat[2]&0xf0)>>4;
			uint8 configFlag  = pdat->pdat[8];
			
			if((pAccess_CardDat->hotelId != Sys_PataCfg.PropertyId) || (Sys_PataCfg.PropertyId == 0)
			|| (pAccess_CardDat->HotelPaswd != Sys_PataCfg.HotelPswd))
			{
				return E_OK;
			}
			if(Access_ComTime(pAccess_CardDat->Timest, pAccess_CardDat->Timeend) != E_OK)
			{/*卡片失效*/
				if((type != CARD_TYPE_STAFF_LOOP) && (type != CARD_TYPE_LOSS))
				{
					return E_TIME_OUT;
				}
			}
			
			if(type == CARD_TYPE_INSPECTION)
			{/* 巡检测卡*/
			#if (defined(SUPPORT_RECORD_LOC_STORE)&&(SUPPORT_RECORD_LOC_STORE == STD_TRUE))
				Access_WriteRecordFlash(pdat->pid,ProtoAnaly_RtcLocalTime,RECORD_INSPECTION_CARD,ACCESS_OPEN_LOCK_TPYE);
			#endif
			#if (SUPPORT_RECORD_RTIME_SEND == STD_TRUE)
	           ProtoAnaly_AddRecordCmd(pdat->pid,ProtoAnaly_RtcLocalTime,RECORD_INSPECTION_CARD,ACCESS_OPEN_LOCK_TPYE);
			#endif
				return E_END_LIGHT;
			}
			if(type == CARD_TYPE_LOSS)
			{/* 挂失卡*/
				pAccess_CardDat->deviceId = BUILD_UINT32(pdat->pdat[9], pdat->pdat[8], pdat->pdat[7], pdat->pdat[6]);
				BlackList_Add(KEY_TYPE_CARD, pAccess_CardDat->deviceId, BUILD_TIME(year, month, day, hour, 59, 59));
				BlackList_StorePara();
				return E_END;
			}
			if(type == CARD_TYPE_EMERGENCY)
			{/* 应急卡*/
				if(pAccess_CardDat->Timest < AccRcord.EmergencyTimerBk)
				{
					return E_TIME_OUT;
				}
				pAccess_CardDat->cardtype = CARD_TYPE_EMERGENCY;
				if(configFlag & LOSS_OLD_CARD)
				{
					if(pAccess_CardDat->Timest > AccRcord.EmergencyTimerBk)
					{
						AccRcord.EmergencyTimerBk = pAccess_CardDat->Timest;
						RecordList_StorePara(0);
					}
				}
				return E_OK;
			}
			if(type == CARD_TYPE_ALL)
			{/* 总卡*/
				if(pAccess_CardDat->Timest < AccRcord.AllTimerBk)
				{
					return E_TIME_OUT;
				}
				pAccess_CardDat->cardtype = CARD_TYPE_ALL;
				if(configFlag & LOSS_OLD_CARD)
				{
					if(pAccess_CardDat->Timest > AccRcord.AllTimerBk)
					{
						AccRcord.AllTimerBk = pAccess_CardDat->Timest;
						RecordList_StorePara(0);
					}
				}
				return E_OK;
			}
			if(Sys_PataCfg.BuildId == 0)
			{
				pAccess_CardDat->cardtype = type;
				return E_OK;
			}
			else
			{
				uint8 i;
					
				/*if((pdat->pdat[4] == 0) && (pdat->pdat[5] == 0) && (pdat->pdat[6] == 0)
					&& (pdat->pdat[7] == 0))
				{
					pAccess_CardDat->cardtype = type;
				}
				else*/
				{
					if(pAccess_CardDat->Timest < AccRcord.StaffTimerBk)
					{
						return E_TIME_OUT;
					}
					for(i=0; i<ACCESS_CARDBUILDID_MAX; i++)
					{
						if(Sys_PataCfg.BuildId == pdat->pdat[4+i])
						{
								//pAccess_CardDat->cardtype = type;
							break;
						}
					}
					if(i >= ACCESS_CARDBUILDID_MAX)
					{
						return E_NOT_OK;
					}
					if(configFlag & LOSS_OLD_CARD)
					{
						if(pAccess_CardDat->Timest > AccRcord.StaffTimerBk)
						{
							AccRcord.StaffTimerBk = pAccess_CardDat->Timest;
							RecordList_StorePara(0);
						}
					}
				}
				if(type == CARD_TYPE_STAFF)
				{/* 时间限制*/
					pAccess_CardDat->cardtype = type;
					return E_OK;
				}
				else if(type == CARD_TYPE_STAFF_LOOP)
				{
					uint8 t = 0;

					if(ProtoAnaly_Rtcinfo.Week == 7)
					{
						t = 0;
					}
					else
					{
						t = ProtoAnaly_Rtcinfo.Week;
					}
					
					//for(i=0; i<8; i++)
					{
						if(pdat->pdat[9] & (1<<t))
						{
							//if(ProtoAnaly_Rtcinfo.Week == i)
							{
								if(Access_ComTimeLoop(pAccess_CardDat->Timest, pAccess_CardDat->Timeend) == E_OK)
								{
									pAccess_CardDat->cardtype = type;
									return E_OK;
								}
								else
								{
									return E_NOT_OK;
								}
							}
						}
					}
				}
				pAccess_CardDat->cardtype = CARD_TYPE_NULL;
				return E_NOT_OK;
			}
		}
		else if(cardType == 6)
		{/* 门锁配置卡*/
			if((pdat->pdat[2]&0xf0) == 0x00)
			{/* 门锁配置*/
				if((BUILD_UINT32(pdat->pdat[9], pdat->pdat[8], pdat->pdat[7], pdat->pdat[6])&0x0000FFFF) != 0x00003344)
			{/* 验证卡片合法性*/
					return E_NOT_OK;
				}
			}
			else
			{
				if((BUILD_UINT32(pdat->pdat[9], pdat->pdat[8], pdat->pdat[7], pdat->pdat[6])&0xFFFFFFFF) != 0x11223344)
				{/* 验证卡片合法性*/
					return E_NOT_OK;
				}
			}
			if(Access_ComTime(pAccess_CardDat->Timest, pAccess_CardDat->Timeend) != E_OK)
			{/*卡片失效*/
				return E_TIME_OUT;
			}
			if((pdat->pdat[2]&0xf0) == 0x00)
			{/* 门锁配置*/
				uint16 newPaswd;

				if((Sys_PataCfg.PropertyId == 0)&&(Sys_PataCfg.Pswd != 0x12345678))
				{
					return E_OK;
				}
				if(Sys_PataCfg.PropertyId == 0)
				{/* 重未配置过*/
					Sys_PataCfg.PropertyId = pAccess_CardDat->hotelId;
					Sys_PataCfg.HotelPswd = pAccess_CardDat->HotelPaswd;
					if(pdat->pdat[6] != 0xff)
					{
						Sys_PataCfg.BuildId = pdat->pdat[6];
					}
					if(pdat->pdat[7] < 0x10)
					{
						Sys_PataCfg.usedpage = pdat->pdat[7];
					}
					Sys_StorePara();
					Sys_McuRestFlag=STD_TRUE;
					return E_END;
				}
				else
				{
					if(/*(pAccess_CardDat->HotelPaswd != Sys_PataCfg.HotelPswd) ||*/(pAccess_CardDat->hotelId != Sys_PataCfg.PropertyId))
					{
						return E_OK;
					}
				}
				newPaswd = BUILD_UINT16(pdat->pdat[5], pdat->pdat[4])&0x0fff;
					Sys_PataCfg.HotelPswd = newPaswd;
				if(pdat->pdat[6] != 0xff)
				{
					Sys_PataCfg.BuildId = pdat->pdat[6];
				}
				if(pdat->pdat[7] < 16)
				{
					Sys_PataCfg.usedpage = pdat->pdat[7];
				}
					Sys_StorePara();
					Sys_McuRestFlag=STD_TRUE;
				return E_END;
			}
		}
		else if(cardType == 5)
		{/* 配置卡*/
			if(BUILD_UINT32(pdat->pdat[9], pdat->pdat[8], pdat->pdat[7], pdat->pdat[6]) != 0x11223344)
			{
				return E_OK;
			}
			if((pdat->pdat[2]&0xf0) == 0x10)
			{/* 功能配置*/
				if(pdat->pdat[3] == 1)
				{/* 开蓝牙*/
					Sys_PataCfg.CfgFlag &= ~DISIBLEBLUETOOTH;
				}
				else if(pdat->pdat[3] == 0)
				{/* 关蓝牙*/
					Sys_PataCfg.CfgFlag |= DISIBLEBLUETOOTH;
				}
				else if(pdat->pdat[3] == 2)
				{/* 唤醒蓝牙*/
					 if(Sys_EnableBlueTooth())
				 	{
						//ble_dahao_start_advert(0);
					}
					return E_END;
				}
				Sys_StorePara();
				Sys_McuRestFlag=STD_TRUE;
				return E_END;
			}
			else if((pdat->pdat[2]&0xf0) == 0x20)
			{/* 客户代码*/
				if(Access_ComTime(pAccess_CardDat->Timest, pAccess_CardDat->Timeend) != E_OK)
				{/*卡片失效*/
					return E_TIME_OUT;
				}
				return E_END;
			}
			else if((pdat->pdat[2]&0xf0) == 0x30)
			{/* 工厂代码*/
				if(Access_ComTime(pAccess_CardDat->Timest, pAccess_CardDat->Timeend) != E_OK)
				{/*卡片失效*/
					return E_TIME_OUT;
				}
				Sys_PataCfg.CustomId = BUILD_UINT16(pdat->pdat[5], pdat->pdat[4]);
				Sys_StorePara();
				Sys_McuRestFlag=STD_TRUE;
				return E_END;
			}
			else  if((pdat->pdat[2]&0xf0) == 0x40)
			{/* 工厂测试卡*/
				if(Access_ComTime(pAccess_CardDat->Timest, pAccess_CardDat->Timeend) != E_OK)
				{/*卡片失效*/
					return E_TIME_OUT;
				}
				if(Sys_PataCfg.Pswd != 0x12345678)
				{
					return E_END;
				}
				pAccess_CardDat->cardtype = CARD_TYPE_TESK;
				return E_OK;
			}
		}
		else
		{
			return E_OK;
		}

	}
	return E_OK;
}

/****************************************************************************************************
**Function:
	void Access_Proc(void)
**Author: rory
**Description:  64ms
**Input: 
**Output: 
****************************************************************************************************/
void Access_TimerProc(void)
{	
	/*if(Access_MotorTimer64 != 0)
	{
		Access_MotorTimer64--;
		if(Access_MotorTimer64 == 0)
		{
			Access_MotorStop();
		}
	}*/
	if(Access_ResetDeviceTimer64 != 0)
	{
		Access_ResetDeviceTimer64--;
		if(Access_ResetDeviceTimer64 == 0)
		{
			NVIC_SystemReset();
		}
	}
	if(Access_LockDeviceTimer64ms != 0)
	{
		Access_LockDeviceTimer64ms--;
		if(Access_LockDeviceTimer64ms <= 1)
		{
			Access_Globle.ErrorTimes =0;
			Access_LockDeviceTimer64ms = 0;
		}
	}
	
	if(Access_CardLearnTimer64 != 0)
	{
		--Access_CardLearnTimer64;
		if(Access_CardLearnTimer64 == 0)
		{
			Sys_PataCfg.State = WORK_NORMAL;
		 //	Access_BeepStart(BEEP_NORMAL,1);
		}
	}
	if(++Sys_SleepErrTimsoutx64ms >= (60000/64) *10)
	{
		// 超时未休眠
		Sys_SleepErrTimsoutx64ms = 0;
		NVIC_SystemReset();
	}
}
U8 checkLockFlag;
U8 checkUnLockFlag;
/****************************************************************************************************
**Function:
	void Access_Unlock(void)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Access_Unlock(void)
{
	PwrStopCnt = 10;
  HALL_Unlogflag = 1;
	HALL_UnlogCnt = 80;
  if ((Sys_PataCfg.CheckLR == 0x00) || (Sys_PataCfg.CheckLR == 0xFF))
  	{
			PwrStopCnt = 20;
  	  AutoCheckStatus = 1;
			checkLockFlag = 0;
			checkUnLockFlag = 1;
			Motor_SubState = CASE_IN;
			return;
  	}
	
	MotorDrive.ioctl(MOTOR_CMD_OPEN, NULL);
}
/****************************************************************************************************
**Function:
	void Access_Lock(void)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Access_Lock(void)
{
	PwrStopCnt = 10;
  HALL_Unlogflag = 1;
	HALL_UnlogCnt = 80;
  if ((Sys_PataCfg.CheckLR == 0x00) || (Sys_PataCfg.CheckLR == 0xFF))
  	{
			PwrStopCnt = 20;
  	  AutoCheckStatus = 1;
			checkLockFlag = 1;
			checkUnLockFlag = 0;
			Motor_SubState = CASE_IN;
			return;
  	}
	Motor_AutoLockTimer64ms = 0;
	Motor_DoorLockSt = MOTOR_DOORLOCK_IDLE;
	MotorDrive.ioctl(MOTOR_CMD_CLOSE, NULL);
}
/****************************************************************************************************
**Function:
	void Access_OpenError(void)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Access_OpenError(void)
{


}


/****************************************************************************************************
**Function:
	void Access_EraseAllKey(void)
**Author: rory
**Description: 
**Input: 
**Output: 
****************************************************************************************************/
void Access_EraseAllKey(uint8 idtpye)
{
	memset(&LockKeyBlackList,0xff,sizeof(LockKeyBlackList));
	BlackList_StorePara();
	
	for (uint8 j=0;j<SERVE_KEY_PAGEMAX;j++ )
	{
			memset(&LockKeyServeKeyList,0xff,sizeof(LockKeyServeKeyList));
			ServeKeyList_Writeflash(j);
	}
}

/****************************************************************************************************
**Function:
	void Access_DefaultPaswd(void)
**Author: rory
**Description: 
**Input: 
**Output: 
****************************************************************************************************/
void Access_DefaultPaswd(void)
{
	LockKeyServeKeyList.KeyList[0].Type = KEY_TYPE_PASSWORD;
	LockKeyServeKeyList.KeyList[0].KeyId = 0xff888888;
	LockKeyServeKeyList.KeyList[0].EndTime = BUILD_TIME(30, 12, 30, 23, 59, 59);
	LockKeyServeKeyList.KeyList[0].StartTime = BUILD_TIME(18, 1, 1, 1, 1, 1);	
	LockKeyStatistics.ServKeyCount++;
	//ServeKeyList_StorePara();
}

/****************************************************************************************************
**Function:
	Std_ReturnType Access_ComTime(uint32 startTime, uint32 endTime)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
Std_ReturnType Access_ComTime(uint32 startTime, uint32 endTime)
{
	uint32 LocalTime = ProtoAnaly_RtcLocalTime;//&0xffffffc0;
	
	if((startTime == 0xffffffff) || (LocalTime < startTime))
	{
		return E_END;
	}
	else if(endTime == 0xffffffff)
	{
		return E_OK;
	}
	if(startTime == 0)
	{
		if(endTime >= LocalTime)
		{
			return E_OK;
		}
	}
	else
	{
		if((startTime <= LocalTime)&&(LocalTime <= endTime))
		{
			return E_OK;

		}
		else if(LocalTime > endTime)
		{
			return E_TIME_OUT;
		}
	}
	return E_NOT_OK;
}

/****************************************************************************************************
**Function:
	Std_ReturnType Access_ComTimeLoop(uint32 startTime, uint32 endTime)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
Std_ReturnType Access_ComTimeLoop(uint32 startTime, uint32 endTime)
{
	uint32 currentTime;

	currentTime = BUILD_LOOP_TIME(ProtoAnaly_Rtcinfo.Hour, ProtoAnaly_Rtcinfo.Minute, ProtoAnaly_Rtcinfo.Second);
	startTime = startTime&0x1ffff;
	endTime = endTime&0x1ffff;
	if((startTime <= currentTime)&&(currentTime <= endTime))
	{
		return E_OK;

	}
	return E_NOT_OK;
}

#if 1//(SUPPORT_RECORD_LOC_STORE==STD_TRUE)
/****************************************************************************************************
**Function:
	void Access_WriteRecordFlash(uint8* pKeyId,uint32 time ,uint8 type, uint8 action)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint8  Access_GetRecord(uint16 index,uint8 *poutdat)
{
	poutdat[0] = BUILD_TYPE(AccRcord.RecordList[index].TypeResult);
	if((AccRcord.RecordList[index].TypeResult == ACCESS_LEARN_IDENTITY_CARD_TPYE)
		|| (AccRcord.RecordList[index].TypeResult == KEY_TYPE_IDENTITY_CARD))
	{
		poutdat[1] = 0xff;
		poutdat[2] = 0xff;
		poutdat[3] = 0xff;
		poutdat[4] = 0xff;
		poutdat[5] = TWOHI_UINT32(AccRcord.RecordList[index].Id);
		poutdat[6] = TWOLO_UINT32(AccRcord.RecordList[index].Id);
		poutdat[7] = ONEHI_UINT32(AccRcord.RecordList[index].Id);
		poutdat[8] = ONELO_UINT32(AccRcord.RecordList[index].Id);
	}
	else
	{
		poutdat[1] = TWOHI_UINT32(AccRcord.RecordList[index].Id);
		poutdat[2] = TWOLO_UINT32(AccRcord.RecordList[index].Id);
		poutdat[3] = ONEHI_UINT32(AccRcord.RecordList[index].Id);
		poutdat[4] = ONELO_UINT32(AccRcord.RecordList[index].Id);
		poutdat[5] = 0xff;
		poutdat[6] = 0xff;
		poutdat[7] = 0xff;
		poutdat[8] = 0xff;
	}
	poutdat[9] = BUILD_ACTION(AccRcord.RecordList[index].TypeResult);
	poutdat[10] = TWOHI_UINT32(AccRcord.RecordList[index].Time);
	poutdat[11] = TWOLO_UINT32(AccRcord.RecordList[index].Time);
	poutdat[12] = ONEHI_UINT32(AccRcord.RecordList[index].Time);
	poutdat[13] = ONELO_UINT32(AccRcord.RecordList[index].Time);
	return 1;
}
#endif
/****************************************************************************************************
**Function:
	void Access_WriteRecordFlash(uint8* pKeyId,uint32 time ,uint8 type, uint8 action)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Access_WriteRecordFlash(uint8* pKeyId,uint32 time ,uint8 type, uint8 action)
{
	if(AccRcord.Wrecordindex >= ACCESS_RECORD_MAX)
	{
		AccRcord.Wrecordindex = 0;
	}
	AccRcord.RecordList[AccRcord.Wrecordindex].Id = BUILD_UINT32(pKeyId[3], pKeyId[2], pKeyId[1], pKeyId[0]);
	AccRcord.RecordList[AccRcord.Wrecordindex].Time = time;
	AccRcord.RecordList[AccRcord.Wrecordindex].TypeResult = BUILD_ACTIONTYPE(type, action);
	AccRcord.Wrecordindex++;
	if(++AccRcord.recordnum >= 65535)
	{
		AccRcord.recordnum = 1;
	}
	RecordList_StorePara(0);
	Access_Record_Null = 0;
	if ( AccRcord.Wrecordindex >= 9 )    
		{
		  if( 0 == NbLogStatus && 1 == NBconectFlag ) NbLogStatus = 1; 
		}
}


/****************************************************************************************************
**Function:
	void Access_EraseRecordData(void)
**Author: lxw
**Description:
**Input: 
**Output: 根据索引强制修改数据
****************************************************************************************************/
void Access_EraseRecordData(void)
{
	memset((uint8_t *)&AccRcord, 0xff, sizeof(AccRcordParaType));
	AccRcord.Wrecordindex = 0;
	AccRcord.Srecordindex = 0;
	AccRcord.recordnum = 0;
	AccRcord.TimerBk = BUILD_TIME(18,1,1,1,1,1);
	AccRcord.CustomTimerBk = 0;
	AccRcord.StaffTimerBk = 0;
	AccRcord.AllTimerBk = 0;
	AccRcord.EmergencyTimerBk = 0;
	AccRcord.AppTimerBk = 0;
	RecordList_StorePara(1);
}

/****************************************************************************************************
**Function:
	void Access_FlashArrang(void)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Access_FlashArrang(void)
{
	uint8 i,j;

	memset(&LockKeyStatistics,0,sizeof(LockKeyStatistics));
	for(i=0; i<BLACK_LIST_MAX; i++)
	{
		if(LockKeyBlackList.BlackList[i].KeyId != 0xffffffff)
		{
			LockKeyStatistics.BlackListCount++;
		} 
	}

	
	for ( j=0;j<SERVE_KEY_PAGEMAX;j++ )
	{
		ServeKeyList_Readflash( j );
			for ( i=0; i<SERVE_KEY_MAX; i++)
				{
			if (LockKeyServeKeyList.KeyList[i].KeyId != 0xffffffff)
				{
					LockKeyStatistics.ServKeyCount++;
				}
			}
	}
}
#if 0
/****************************************************************************************************
**Function:
	void Access_CardProcess(uint8 idtpye, uint8* pUid,Access_CardDatType *CardDat)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Access_CardProcess(uint8 idtpye, uint8* pUid,Access_CardDatType *CardDat)
{
	uint8 cardType;
	
	if(Sys_PataCfg.State == WORK_TEST)
	{
		Sys_PataCfg.State = WORK_NORMAL;
	}
	else if((Sys_PataCfg.State == WORK_NORMAL))
	{
		uint8 ret = E_NOT_OK;
		
		ret = LockKey_Check_CardKey(idtpye,pUid);
		if(ret == E_NOT_OK)
		{
#if (RC522_RDDAT == STD_TRUE)
				if(CardDat->cardtype == CARD_TYPE_GUEST)
				{
					if(CardDat->deviceId != Sys_PataCfg.Mac) 
					{
						ret = E_LOCK_ID_WRONG;
						goto ERR;
					}
					else if(CardDat->devicePaswd != Sys_PataCfg.Pswd) 
					{
						ret = E_USER_PASWD_WRONG;
						goto ERR;
					}
					else 
					{
					  idtpye = RECORD_CUSTOM_CARD;
						goto RIGHT;
					}
				}
				else if((CardDat->cardtype == CARD_TYPE_STAFF) || (CardDat->cardtype == CARD_TYPE_STAFF_LOOP))
				{
				       if(CardDat->cardtype == CARD_TYPE_STAFF)
					{
						idtpye = RECORD_STAFF_CARD;
					}
					else if(CardDat->cardtype == CARD_TYPE_STAFF_LOOP)
					{
						idtpye = RECORD_STAFF_LOOP_CARD;
					}
					goto RIGHT;
				}
				else if(CardDat->cardtype == CARD_TYPE_ALL)
				{
					idtpye = RECORD_TOTAL_CARD;	
					goto RIGHT;
				}
				else if(CardDat->cardtype == CARD_TYPE_EMERGENCY)
				{
					Sys_PataCfg.State = WORK_NORMALLY_OPEN;
					Access_Unlock();
					Access_LockTimer64 = 0;
					//Access_BeepStart(BEEP_VERY_SLOW,1);
					idtpye = RECORD_EMERGENCY_CARD;
				#if (defined(SUPPORT_RECORD_LOC_STORE)&&(SUPPORT_RECORD_LOC_STORE == STD_TRUE))
					Access_WriteRecordFlash(pUid,ProtoAnaly_RtcLocalTime, idtpye, ACCESS_OPEN_LOCK_TPYE);
				#endif
				#if (SUPPORT_RECORD_RTIME_SEND == STD_TRUE)
					ProtoAnaly_AddRecordCmd(pUid,ProtoAnaly_RtcLocalTime, idtpye, ACCESS_OPEN_LOCK_TPYE);
				#endif
					return;
				}
				else if(CardDat->cardtype == CARD_TYPE_TESK)
				{
					goto RIGHT;
				}
				ret = E_KEY_NO_SUPPORT;
				goto ERR;
#endif			
		}
		else if(ret == E_OK)
		{
			goto RIGHT;
		}   
	}
	else if(Sys_PataCfg.State == WORK_NORMALLY_OPEN)
	{
		uint8 ret = E_NOT_OK;
		
#if (RC522_RDDAT == STD_TRUE)
			if((CardDat->hotelId != Sys_PataCfg.PropertyId)||(CardDat->hotelId == 0))
			{
				ret = E_LOCK_ID_WRONG;
				goto ERR;
			}
			else 
			{
				if(CardDat->cardtype == CARD_TYPE_EMERGENCY)
				{
					ret = Access_ComTime(CardDat->Timest, CardDat->Timeend); 
					if(ret == E_OK)
					{
						//Access_Globle.State = WORK_NORMAL;
						Sys_PataCfg.State = WORK_NORMAL;
						goto RIGHT;
					}
				}
			}
#else
		Sys_PataCfg.State = WORK_NORMAL;
		//Access_Globle.State = WORK_NORMAL;
		goto RIGHT;	
#endif			
	}
	ERR:
	Access_OpenError();
	return ; 
	RIGHT:
	Access_Unlock();


	if(idtpye == KEY_TYPE_IDENTITY_CARD)
	{
		Access_WriteRecordFlash(&pUid[4],ProtoAnaly_RtcLocalTime, idtpye, ACCESS_OPEN_LOCK_TPYE);
	}
	else
	{
		Access_WriteRecordFlash(pUid,ProtoAnaly_RtcLocalTime,idtpye,ACCESS_OPEN_LOCK_TPYE);
	}
}
#endif 
