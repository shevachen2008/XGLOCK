

/**************************************************************************************************/ 
/* Include file                                                                                   */                                                 
/**************************************************************************************************/
#include "main.h"


extern uint8  Access_AddId_NewFlag;
extern uint8  Access_FlashProcFlag;
extern uint32 Access_UserId_Temp;
extern uint32 Access_ScanInex_Temp;
extern uint8 Access_AttTIme_Temp[8];
extern uint32 ProtoAnaly_RtcLocalTime;
	

LockKey_StatisticsType LockKeyStatistics;
LockKey_BlackListType LockKeyBlackList;
LockKey_ServeKeyListType LockKeyServeKeyList;
//LockKey_AppKeyListType LockKeyAppKeyList;
uint8 Access_Record_Null = 0;


uint8 ServeKeyFlashOffset;
uint8 num_weishu;

/****************************************************************************************************
**Function:
	uint8 LockKey_RemoveKey(uint8 type, uint8 index)
**Author: rory
**Description: 
**Input: 
**Output: 
****************************************************************************************************/
void LockKey_RemoveKey(uint8 idtype, uint8 index)
{	
	uint8 year, month, day;
	uint32 tempTime;
	
	if(index == 0)
	{
		return;
	}
	index--;
	if(idtype == 1)
	{
	    ServeKeyList_Readflash(ServeKeyFlashOffset);
		tempTime = LockKeyServeKeyList.KeyList[index].StartTime;
		year = BUILD_YEAR(tempTime);
		month = BUILD_MONTH(tempTime);
		day = BUILD_DAY(tempTime);
		if((ProtoAnaly_Rtcinfo.Year == year)&&(ProtoAnaly_Rtcinfo.Month == month)
		&&(ProtoAnaly_Rtcinfo.Day == day))
		{ /* 加入黑名单*/
			BlackList_Add(LockKeyServeKeyList.KeyList[index].Type, LockKeyServeKeyList.KeyList[index].KeyId,
			BUILD_TIME(ProtoAnaly_Rtcinfo.Year,ProtoAnaly_Rtcinfo.Month,ProtoAnaly_Rtcinfo.Day,
			23,59,59));
			BlackList_StorePara();
		}
		memset(&LockKeyServeKeyList.KeyList[index], 0xff, sizeof(LockKey_ServeKeyType));
		if(LockKeyStatistics.ServKeyCount > 0)
		{
			LockKeyStatistics.ServKeyCount--;
		}
	    ServeKeyList_Writeflash(ServeKeyFlashOffset);
		//ServeKeyList_StorePara1();  
	}
}

uint8_t numcmp(uint8_t* str, uint8_t *substr,uint8_t len)
{
  uint8_t* s = str; 
  uint8_t i = 0 , rflag =1;
  
  while (rflag) 
    { 
    if (memcmp(s, substr, len) == 0) 
      { 
        rflag = 0;
        return 1;
      } 
    else 
      { 
	      if ( ++i <= (num_weishu - len))
	        {
	          s++;
	        }
	      else
	        {
	          rflag = 0;
	          return 0;
	        }
      } 
    }
  return 0;
}


uint8 keyHexLen;
uint8 keyStoreLen;

uint8 keyInaAscBuf[32];
uint8 keyStoreHexBuf[4];
uint8 keyStoreAscBuf[8];

uint8 jishuFLAG;

/****************************************************************************************************
**Function:
	uint16 LockKey_CommPswdId(uint8 idtpye, uint8 *pid, uint32 *pkeyId,uint8 lenth)
**Author: rory
**Description: 返回id所在色索引 
**Input: 0:黑名单  1:服务下发 2:APP下发
**Output:  0 表示没找到
****************************************************************************************************/
uint16 LockKey_CommPswdId(uint8 idtpye, uint8 *pid, uint32 *pkeyId, uint8 lenth)
{	
	uint32 keytemp,rdkeytemp, keyData;
	uint8 lenthhfbyte, currentMax,  keyType;
  uint8 match = FALSE;
	
	if(idtpye == 0)
	{
		currentMax = LockKeyStatistics.BlackListCount;
		//totalMax = BLACK_LIST_MAX;
	}
	else if(idtpye == 1)
	{
		currentMax = LockKeyStatistics.ServKeyCount;
		if ( lenth &0x01 )
			{
				keyHexLen = (lenth>>1)+1; 			// 奇数长度
				jishuFLAG = 1;
			}
		else
			{
				keyHexLen = lenth>>1; 					// 偶数长度
			}
			HexsToAscs(pid,keyInaAscBuf,keyHexLen);
		
		//totalMax = SERVE_KEY_MAX;
	}
	else
	{
		return 0;
	}
	if(currentMax == 0)
	{
		return 0;
	}
	lenthhfbyte = lenth;
	for(;;)
	{	
		LOCKKEY_COMMAGAIN:
		keytemp = 0;
		if(lenthhfbyte >= 8)
		{
			if(lenthhfbyte&0x01)
			{
				keytemp = BUILD_UINT32(pid[(lenthhfbyte>>1)-1],pid[(lenthhfbyte>>1)-2],pid[(lenthhfbyte>>1)-3],pid[(lenthhfbyte>>1)-4]);
				keytemp<<=4;
				keytemp|=(pid[(lenthhfbyte>>1)]>>4);
			}
			else 
			{
				keytemp = BUILD_UINT32(pid[(lenthhfbyte>>1)-1],pid[(lenthhfbyte>>1)-2],pid[(lenthhfbyte>>1)-3],pid[(lenthhfbyte>>1)-4]);
			}
			lenthhfbyte--;
		}
		else 
		{
			keytemp = 0;
			if(lenthhfbyte&0x01)
			{
				for(uint8 i =0;i <(lenthhfbyte>>1)+1;i++)
				{
					keytemp<<=8;
					keytemp|= pid[i];
				}
				keytemp>>=4;
			}
			else 
			{
				for(uint8 i =0;i <(lenthhfbyte>>1);i++)
				{
					keytemp<<=8;
					keytemp|= pid[i];
				}
			}
			lenthhfbyte--;
		}
#if 1		
		for(uint16 j= 0;j< currentMax;)
		{	
		  if(idtpye == 0)
	      {
	  			for(uint8 i =0;i <BLACK_LIST_MAX;i++)  
	  	    {
	  				keyType = LockKeyBlackList.BlackList[i].Type;
	  				if( (keyType == KEY_TYPE_PASSWORD) || (keyType == KEY_TYPE_TIME_SLOT_7) || (keyType == KEY_TYPE_TIME_SLOT_8)
	  					|| (keyType == KEY_TYPE_ONE_TIME) || (keyType == KEY_TYPE_FOREVER)	|| (keyType == KEY_TYPE_CLEAR))
	  				{
	  					keyData = LockKeyBlackList.BlackList[i].KeyId;
	  					if(keyData != 0xffffffff)
	  					{
	  						if(BlackList_Remove(i) == E_OK)
	  						{// 成功删除黑名单
	  							continue;
	  						}
	  					}
	  				}
	  				else
	  				{
	  					continue;
	  				}
	  				if(keyData == 0xffffffff)
	  				{
	  					continue;
	  				}
	  				j += 1;
	  				rdkeytemp = keyData;
	  				if(( rdkeytemp&0xf0000000)!= 0xf0000000)
	  				{
	  					if(keytemp == rdkeytemp)
	  					{
	  						*pkeyId = rdkeytemp;
	  						return (i+1);
	  					}
	  				}
	  				else if((rdkeytemp &0xff000000)!= 0xff000000)
	  				{
	  					if((keytemp&0x0fffffff) == (rdkeytemp&0x0fffffff))
	  					{
	  						*pkeyId = (rdkeytemp&0x0fffffff)|0xf0000000;
	  						return (i+1);
	  					}
	  				}
	  				else if((rdkeytemp &0xfff00000)!= 0xfff00000)
	  				{
	  					if((keytemp&0x00ffffff) == (rdkeytemp&0x00ffffff))
	  					{
	  							*pkeyId = (rdkeytemp&0x00ffffff)|0xff000000;
	  							return (i+1);
	  					}
	  				}
	  				else if((rdkeytemp &0xffff0000)!= 0xffff0000)
	  				{
	  					if((keytemp&0x000fffff) == (rdkeytemp&0x000fffff))
	  					{
	  						*pkeyId = (rdkeytemp&0x000fffff)|0xfff00000;
	  						return (i+1);
	  					}
	  				}
	  				else if((rdkeytemp &0xffff0000)== 0xffff0000)
	  				{
	  					if((keytemp&0x0000ffff) == (rdkeytemp&0x0000ffff))
	  					{
	  						*pkeyId = (rdkeytemp&0x0000ffff);
	  						return (i+1);
	  					}
	  				}
	  			}
				if (lenthhfbyte < 4)
					{
						return 0;
					}
					else 
					{
						goto LOCKKEY_COMMAGAIN;
					}
	      }
			else if ( idtpye == 1 )
			{
			  for(uint8 k=0;k<SERVE_KEY_PAGEMAX;k++ )
	       {
	        ServeKeyList_Readflash( k );
					
					//HAL_UART_Transmit(&UART4_Handle, (uint8_t*)444, 4,2);
					for(uint8 i =0;i <SERVE_KEY_MAX;i++)  
					{
						//HAL_UART_Transmit(&UART4_Handle, "2#", 2,2);
						keyType = LockKeyServeKeyList.KeyList[i].Type;
						if((keyType == KEY_TYPE_PASSWORD) || (keyType == KEY_TYPE_TIME_SLOT_7)
							|| (keyType == KEY_TYPE_TIME_SLOT_8) || (keyType == KEY_TYPE_ONE_TIME) || (keyType == KEY_TYPE_FOREVER))
						{
						  ServeKeyFlashOffset = k;
							keyData = LockKeyServeKeyList.KeyList[i].KeyId;
							
							if(ServeKeyList_Remove(i) == E_OK)
							{// 成功删除
								continue;
							}
						}
						else
						{
							continue;
						}

						if(keyData == 0xffffffff)
						{
							continue;
						}
						j += 1;

            C32HEX2BYTESMSB (keyStoreHexBuf, keyData ); 
						HexsToAscs(keyStoreHexBuf,keyStoreAscBuf,4);

						if ( 'F' == keyStoreAscBuf[0] && 'F' == keyStoreAscBuf[1] 
							 && 'F' == keyStoreAscBuf[2] && 'F' == keyStoreAscBuf[3] )
							{
							  keyStoreLen = 4;
								memcpy(keyStoreAscBuf, &keyStoreAscBuf[4], 4 );
							}
						else if ( 'F' == keyStoreAscBuf[0] && 'F' == keyStoreAscBuf[1] 
							     && 'F' == keyStoreAscBuf[2] )
							{
							  keyStoreLen = 5;
								memcpy(keyStoreAscBuf, &keyStoreAscBuf[3], 5 );							
							}
						else if  ( 'F' == keyStoreAscBuf[0] && 'F' == keyStoreAscBuf[1] )
							{
							  keyStoreLen = 6;
								memcpy(keyStoreAscBuf, &keyStoreAscBuf[2], 6 );								
							}
						else if ('F' == keyStoreAscBuf[0] )
							{
							  keyStoreLen = 7;
								memcpy(keyStoreAscBuf, &keyStoreAscBuf[1], 7 );									
							}
					  else
					  	{
							  keyStoreLen = 8;
					  	}

					  if ( 1 ==  numcmp(keyInaAscBuf,keyStoreAscBuf,keyStoreLen))
					  	{
					  		*pkeyId = keyData;
	  						//return (k*200+i+1);
	  						return (i+1);
					  	}
					 }
			    }  
				 return 0;
			  }
		}
#endif		
	}
}

/****************************************************************************************************
**Function:
	uint8 LockKey_Check_PaswdKey(uint8 *pInData)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint8 LockKey_Check_PaswdKey(uint8* pTouchKey,uint8 lenth )
{
	uint16 index;
	uint8  temp_buf[8];
	uint32 keyId, startTime, endTime;
	uint8  ret, idtype, keyType,cycledat;

	index =  LockKey_CommPswdId(0, pTouchKey, &keyId, lenth);
	if(index != 0)
	{
		return E_TIME_OUT;
	}
	index =  LockKey_CommPswdId(1, pTouchKey, &keyId, lenth);
	if(index != 0)
	{
	  KeyIdSend = keyId;
		startTime = LockKeyServeKeyList.KeyList[index-1].StartTime;
		endTime = LockKeyServeKeyList.KeyList[index-1].EndTime;
		keyType = LockKeyServeKeyList.KeyList[index-1].Type;
		cycledat =  LockKeyServeKeyList.KeyList[index-1].Cycle;
		idtype = 1;
	}
	else
	{ 
		return E_NOT_OK;
	}

	ret = Access_ComTime(startTime, endTime);
	if(ret == E_TIME_OUT)
	{
		LockKey_RemoveKey(idtype, index);
	}
	if((cycledat&0x80) ==0)
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
		if((cycledat&(1<<t)) ==0)
		{
			return E_TIME_OUT;
		}
		if(Access_ComTimeLoop(startTime, endTime) != E_OK)
		{
			return E_TIME_OUT;  
		}
	}
	
	Sys_GenFun32To8(keyId, temp_buf);

	Access_WriteRecordFlash(temp_buf,ProtoAnaly_RtcLocalTime,keyType,ACCESS_OPEN_LOCK_TPYE);

	return ret; 
}


/****************************************************************************************************
**Function:
	uint8 LockKey_Check_CardKey(uint8 idtpye,uint8 *pid)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint8 LockKey_Check_CardKey(uint8 idtpye,uint8 *pid)
{
	uint32 keyId;

	if(idtpye == KEY_TYPE_IDENTITY_CARD) 
	{ 
		keyId = BUILD_UINT32(pid[7],pid[6],pid[5],pid[4]);  //
	}
	else
	{
	//keyId = BUILD_UINT32(pid[0], pid[1], pid[2], pid[3]);
	keyId = BUILD_UINT32(pid[3], pid[2], pid[1], pid[0]);
	}
	/*  检测黑名单*/
	for(uint8 i=0; i<BLACK_LIST_MAX; i++)
	{
		if((LockKeyBlackList.BlackList[i].Type == KEY_TYPE_CARD) || (LockKeyBlackList.BlackList[i].Type == KEY_TYPE_IDENTITY_CARD))
		{
			if(LockKeyBlackList.BlackList[i].KeyId != 0xffffffff)
			{
				if(BlackList_Remove(i) == E_OK)
				{// 成功删除黑名单
					continue;
				}
			}
		}
		else
		{
			continue;
		}
		if(idtpye == LockKeyBlackList.BlackList[i].Type)
		{
			if(LockKeyBlackList.BlackList[i].KeyId == keyId)
			{
				return E_TIME_OUT;
			}
		}
	}
	if((LockKeyStatistics.ServKeyCount == 0) && (LockKeyStatistics.AppKeyCount == 0))
	{
		return E_NOT_OK;
	}
	for (uint8 j=0;j<SERVE_KEY_PAGEMAX;j++ )
	{
	    ServeKeyList_Readflash( j );	
		for(uint8 i=0; i<SERVE_KEY_MAX; i++)
		{
			if(idtpye == LockKeyServeKeyList.KeyList[i].Type)
			{
				if(LockKeyServeKeyList.KeyList[i].KeyId == keyId)
				{
				  CardIdSend = keyId;   
				#if (defined RTC_EN) && (RTC_EN == STD_TRUE)
					uint8 t = 0;

					if(ProtoAnaly_Rtcinfo.Week == 7)
					{
						t = 0;
					}
					else
					{
						t = ProtoAnaly_Rtcinfo.Week;
					}
					ret = Access_ComTime(LockKeyServeKeyList.KeyList[i].StartTime, LockKeyServeKeyList.KeyList[i].EndTime); 
					if(ret != E_OK) 
					{
						return E_NOT_OK;
					}
					else if(LockKeyServeKeyList.KeyList[i].Cycle&0x80)
					{
						return E_OK;
					}
					else if(LockKeyServeKeyList.KeyList[i].Cycle & (1<<t))
					{
						if(Access_ComTimeLoop(LockKeyServeKeyList.KeyList[i].StartTime, LockKeyServeKeyList.KeyList[i].EndTime) == E_OK)
						{
							return E_OK;
						}
						else
						{
							return E_NOT_OK;
						}
					}
				#else
					return E_OK;
				#endif
				}
			}
		}
	}
	return E_NOT_OK;
}

/****************************************************************************************************
**Function:
	uint8 LockKey_Remove_PasswordKey(BlueKey_AllType BlueKey)
**Author: rory
**Description:
**Input: type: 0(All)  1(one)
             limitTime:限制时间
**Output: 
****************************************************************************************************/
uint8 LockKey_Remove_PasswordKey(void)
{
	uint8 offset;
	uint8 year,month,day;
	uint32 tempTime;
	for (uint8 j=0;j<SERVE_KEY_PAGEMAX;j++ )
	{
	  ServeKeyList_Readflash( j );	

		for(uint8 i=0; i<SERVE_KEY_MAX; i++)
		{
			if(LockKeyServeKeyList.KeyList[i].KeyId != 0xffffffff)
			{
				if((LockKeyServeKeyList.KeyList[i].Type == KEY_TYPE_TIME_SLOT_8)
					|| (LockKeyServeKeyList.KeyList[i].Type == KEY_TYPE_FOREVER)
					|| (LockKeyServeKeyList.KeyList[i].Type == KEY_TYPE_ONE_TIME)
					|| (LockKeyServeKeyList.KeyList[i].Type == KEY_TYPE_TIME_SLOT_7))
				{
				    offset = j;
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
					}
				}
				memset(&LockKeyServeKeyList.KeyList[i], 0xff, sizeof(LockKey_ServeKeyType));
			}
		}
	}
	BlackList_StorePara();
	ServeKeyList_Writeflash( offset );	
	//ServeKeyList_StorePara1();//
	return E_OK;
}

/****************************************************************************************************
**Function:
	uint8 LockKey_Add_VisitCodeKey(uint8 *pData)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint8 LockKey_Add_VisitCodeKey(uint8 idtype,uint32 keyId,  uint32 startTime, uint32 endTime)
{
	uint8 buftemp[13];
	uint8 err = E_NOT_OK;

	Sys_GenFun32To8(keyId, &buftemp[0]);
	Sys_GenFun32To8(endTime, &buftemp[4]);
	//buftemp[8] = 0;
	Sys_GenFun32To8(startTime, &buftemp[8]);
	buftemp[12] = 0xff;
	err = ProtoAnaly_AddId(idtype, buftemp);
	return err;
}

/****************************************************************************************************
**Function:
	uint8 LockKey_Add_BlueKey(BlueKey_AllType BlueKey)
**Author: rory
**Description:
**Input: type: 0(All)  1(one)
             limitTime:限制时间
**Output: 
****************************************************************************************************/
uint8 LockKey_Remove_VisitCodeKey(void)
{
	uint8 offset;
	uint8 year,month,day;
	uint32 tempTime;
	for (uint8 j=0;j<SERVE_KEY_PAGEMAX;j++ )
	{
	    ServeKeyList_Readflash( j );	
		for(uint8 i=0; i<SERVE_KEY_MAX; i++)
		{
			if(LockKeyServeKeyList.KeyList[i].Type == KEY_TYPE_TIME_SLOT_7)
			{
			    offset = j;
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
				}
				memset(&LockKeyServeKeyList.KeyList[i], 0xff, sizeof(LockKey_ServeKeyType));
			}
		}
	}
	BlackList_StorePara();
	
	ServeKeyList_Writeflash( offset );	
	//ServeKeyList_StorePara1();//
	return E_OK;
}


/****************************************************************************************************
**Function:
	uint8 LockKey_Add_BlueKey(BlueKey_AllType BlueKey)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint8 LockKey_Remove_Key(uint8 *pData, uint8 type)
{
	uint8 i;
	uint32 userId = BUILD_UINT32(pData[3], pData[2], pData[1], pData[0]);
 
		if(LockKeyStatistics.ServKeyCount == 0)
		{
			return E_OK;
		}
	    for (uint8 j=0;j<SERVE_KEY_PAGEMAX;j++ )
	    {
		    ServeKeyList_Readflash( j );			
			for(i=0; i<SERVE_KEY_MAX; i++)
			{
				if(LockKeyServeKeyList.KeyList[i].KeyId == userId)
				{
					uint8 year, month, day;
					uint32 tempTime;
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
					memset(&LockKeyServeKeyList.KeyList[i], 0xff, sizeof(LockKey_ServeKeyType));
					if(LockKeyStatistics.ServKeyCount > 0)
					{
						LockKeyStatistics.ServKeyCount--;
					}
					
					ServeKeyList_Writeflash( j );
					//ServeKeyList_StorePara1();
					return E_OK;
				}
			}
	    }
		/* 密码为未使用*/
		BlackList_Add(type, userId,
					BUILD_TIME(ProtoAnaly_Rtcinfo.Year,ProtoAnaly_Rtcinfo.Month,ProtoAnaly_Rtcinfo.Day,
					23,59,59));
		BlackList_StorePara();
	return E_OK;
}

/****************************************************************************************************
**Function:
	void BlackList_LoadPara(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void BlackList_Add(uint8 Type, uint32 KeyId, uint32 InvalidTime)
{
	uint8 index = 0xff;

	for(uint8 i=0; i<BLACK_LIST_MAX; i++)
	{
		if(LockKeyBlackList.BlackList[i].KeyId == KeyId)
		{
			LockKeyBlackList.BlackList[i].Type = Type;
			LockKeyBlackList.BlackList[i].KeyId = KeyId;
			LockKeyBlackList.BlackList[i].InvalidTime = InvalidTime;
			return;
		}
		else if(index == 0xff)
		{
			if(LockKeyBlackList.BlackList[i].KeyId == 0xffffffff)
			{
				index = i;
			}
		}
	}
	if(index != 0xff)
	{
		LockKeyBlackList.BlackList[index].Type = Type;
		LockKeyBlackList.BlackList[index].KeyId = KeyId;
		LockKeyBlackList.BlackList[index].InvalidTime = InvalidTime;
		LockKeyStatistics.BlackListCount++;
	}
}


/****************************************************************************************************
**Function:
	void LockKey_CheckRemoveBlackList(uint8 index)
**Author: rory
**Description: 返回id所在色索引 
**Input: 
**Output: 
****************************************************************************************************/
uint8 BlackList_Remove(uint8 index)
{	
	if(LockKeyBlackList.BlackList[index].InvalidTime < ProtoAnaly_RtcLocalTime)
	{
		memset(&LockKeyBlackList.BlackList[index], 0xff, sizeof(LockKey_BlackType));
		if(LockKeyStatistics.BlackListCount > 0)
		{
			LockKeyStatistics.BlackListCount--;
		}
		BlackList_StorePara();
		return E_OK;
	}
	return E_NOT_OK;
}


/****************************************************************************************************
**Function:
	void BlackList_LoadPara(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void BlackList_LoadPara(void)
{
  //FlashDrive.open(NULL); 
	Flash_ComType eepdata;
	eepdata.pData = (uint8_t *)&LockKeyBlackList;
	eepdata.Lenth = sizeof(LockKeyBlackList);
	eepdata.Add = ACCESS_BLACK_LIST_OFFSET;
	FlashDrive.read(&eepdata);
	if(LockKeyBlackList.KeyWord!= 0x12345678)
	{
		memset(&LockKeyBlackList,0xff,sizeof(LockKeyBlackList));
		BlackList_StorePara();
	}
}

/*
void BlackList_Readflash( uint8 offset )
{
	FlashDrive.open(NULL); 

	Flash_ComType eepdata;
	eepdata.pData = (uint8_t *)&LockKeyBlackList;
	eepdata.Lenth = sizeof(LockKeyBlackList);
	eepdata.Add = ACCESS_BLACK_LIST_OFFSET + (offset<<12);    
	FlashDrive.read(&eepdata);
}
*/


/****************************************************************************************************
**Function:
	void BlackList_StorePara(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void BlackList_StorePara(void)
{
	Flash_ComType eepdata;

	LockKeyBlackList.KeyWord = 0x12345678;
	eepdata.pData = (uint8_t *)&LockKeyBlackList;
	eepdata.Lenth = sizeof(LockKeyBlackList);
	eepdata.Add = ACCESS_BLACK_LIST_OFFSET;
	FlashDrive.ioctl(FLASH_ERASE_SECT,&eepdata.Add);
	FlashDrive.write(&eepdata);
}


/****************************************************************************************************
**Function:
	void BlackList_RemoveKey(uint8 idtpye, uint8 pData)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void BlackList_RemoveKey(uint8 idtpye, uint32 keyId)
{
	uint8 modify = 0;
	
	for(uint8 i=0; i<BLACK_LIST_MAX; i++)
	{
		if(idtpye == LockKeyBlackList.BlackList[i].Type)
		{
			if(keyId == LockKeyBlackList.BlackList[i].KeyId)
			{
				modify = 1;
				memset(&LockKeyBlackList.BlackList[i], 0xff, sizeof(LockKey_BlackType));
				if(LockKeyStatistics.BlackListCount > 0)
				{
					LockKeyStatistics.BlackListCount--;
				}
			}
		}
	}
	if(modify == 1)
	{
		BlackList_StorePara();
	}
}



void Del_Servekeylist(void)
{
		Flash_ComType eepdata;
		
		eepdata.Add = ACCESS_SERVE_KEY_OFFSET;
		FlashDrive.ioctl(FLASH_ERASE_BLOCK,&eepdata.Add);
}



void ServeKeyList_Readflash( uint8 offset )
{
	Flash_ComType eepdata;
	eepdata.pData = (uint8_t *)&LockKeyServeKeyList;
	eepdata.Lenth = sizeof(LockKeyServeKeyList);
	eepdata.Add = ACCESS_SERVE_KEY_OFFSET + (offset<<12);    // 
	FlashDrive.read(&eepdata);
}

/****************************************************************************************************
**Function:
	void ServeKeyList_StorePara(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void ServeKeyList_StorePara(void)
{


}
    
void ServeKeyList_Writeflash(uint8 offset)
{
	Flash_ComType eepdata;
	
	eepdata.pData = (uint8_t *)&LockKeyServeKeyList;
	eepdata.Lenth = sizeof(LockKeyServeKeyList);
	eepdata.Add = ACCESS_SERVE_KEY_OFFSET +(offset<<12);
	FlashDrive.ioctl(FLASH_ERASE_SECT,&eepdata.Add);
	FlashDrive.write(&eepdata);
}



/****************************************************************************************************
**Function:
	void ServeKeyList_Remove(uint8 index)
**Author: rory
**Description: 
**Input: 
**Output: 
****************************************************************************************************/
uint8 ServeKeyList_Remove(uint8 index)
{	
	if(LockKeyServeKeyList.KeyList[index].EndTime < ProtoAnaly_RtcLocalTime)
	{
		memset(&LockKeyServeKeyList.KeyList[index], 0xff, sizeof(LockKey_ServeKeyType));
		if(LockKeyStatistics.ServKeyCount > 0)
		{
			LockKeyStatistics.ServKeyCount--;
		}
		ServeKeyList_Writeflash(ServeKeyFlashOffset);
		
		return E_OK;
		//return E_NOT_OK;
	}
	
	return E_NOT_OK;
	//return E_OK;
}


/****************************************************************************************************
**Function:
	void RecordList_LoadPara(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void RecordList_LoadPara(void)
{
		Flash_ComType eepdata;
		
		//FlashDrive.open(NULL); 
		eepdata.pData = (uint8_t *)&AccRcord;
		eepdata.Lenth = sizeof(AccRcord);
		eepdata.Add = ACCESS_RECORD_OFFSET;
		FlashDrive.read(&eepdata);
		
		if(AccRcord.TimerBk == 0xffffffff)
		{
			AccRcord.TimerBk = 0;
		}
		if(AccRcord.CustomTimerBk == 0xffffffff)
		{
			AccRcord.CustomTimerBk = 0;
		}
		if(AccRcord.StaffTimerBk == 0xffffffff)
		{
			AccRcord.StaffTimerBk = 0;
		}
		if(AccRcord.AllTimerBk == 0xffffffff)
		{
			AccRcord.AllTimerBk = 0;
		}
		if(AccRcord.EmergencyTimerBk == 0xffffffff)
		{
			AccRcord.EmergencyTimerBk = 0;
		}
		if(AccRcord.AppTimerBk == 0xffffffff)
		{
			AccRcord.AppTimerBk = 0;
		}
}
/****************************************************************************************************
**Function:
	void RecordList_StorePara(void)
**Author: lxw
**Description:
**Input: mode:1  直接存储  0:延时存储
**Output: 
****************************************************************************************************/
void RecordList_StorePara(uint8 mode)
{
	//FlashDrive.open(NULL); 

	Flash_ComType eepdata;

	Sys_PataCfg.KeyWord = 0x12345678;
	eepdata.pData = (uint8_t *)&AccRcord;
	eepdata.Lenth = sizeof(AccRcord);
	eepdata.Add = ACCESS_RECORD_OFFSET;
	FlashDrive.ioctl(FLASH_ERASE_SECT,&eepdata.Add);
	FlashDrive.write(&eepdata);
}

/****************************************************************************************************
**Function:
	void HomeLock_Unlock(void)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint8 Touch_KeyProc(uint8* pTouchKey,uint8 lenth)
{
	uint8 ret = E_NOT_OK;
	ProtoAnaly_UpdateTime();

	if(lenth>=4)
	{
		ret = LockKey_Check_PaswdKey(pTouchKey, lenth);
	}
	if(ret == E_NOT_OK)
	{
		if(lenth>=8)
		{
			ret = sys_CheckGetDynamicDate(Sys_PataCfg.Mac, Sys_PataCfg.Pswd, pTouchKey, lenth);
			if(ret == E_NOT_OK)
			{
				ret = sys_CheckGetDynamicFlat(Sys_PataCfg.Mac, Sys_PataCfg.Pswd, pTouchKey, lenth);
			}
			if(ret == E_NOT_OK)
			{
				//ret = sys_CheckGetDynamicHotel(Sys_PataCfg.Mac, Sys_PataCfg.Pswd, pTouchKey, lenth);
				ret = sys_CheckGetDynamicHotel_V8(Sys_PataCfg.Mac, Sys_PataCfg.Pswd, pTouchKey, lenth);
			}
		}
	}

	if(ret == E_OK)
	{
	  Access_Globle.ErrorTimes = 0;
	  CodeType = PSWUnLockType;
		Access_Unlock();
	}
	else
	{
	  ret = E_NOT_OK;
		Line_1A_WT588S( 0x42 );
		Access_Globle.ErrorTimes++;
		if(Access_Globle.ErrorTimes >= 5)
		{
		   Access_Globle.ErrorTimes = 0;
			 SendDelayFlag = 1;
			 Line_1A_WT588S( 0x0d );
		}		
	}
	return ret;
}

