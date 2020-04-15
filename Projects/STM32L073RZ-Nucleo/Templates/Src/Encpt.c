
/****************************************************************************/ 
/* Include file                                                             */                                                 
/****************************************************************************/
#include <string.h>

#include "main.h"


const int32 Encpt_KeyTableExt32[4] =
{
  0xa5143f2c, 0x297625d8,
  0x3841e327, 0x95d8482d,
};

void tea_encrypt(int32 *v, int32 *k) {
    int32 y=v[0], z=v[1], sum=0, i;         /* set up */
    int32 delta=0x9e3779b9;                 /* a key schedule constant */
    int32 a=k[0], b=k[1], c=k[2], d=k[3];   /* cache key */
    for (i=0; i < 32; i++) {                        /* basic cycle start */
        sum += delta;
        y += ((z<<4) + a) ^ (z + sum) ^ ((z>>5) + b);
        z += ((y<<4) + c) ^ (y + sum) ^ ((y>>5) + d);/* end cycle */
    }
    v[0]=y;
    v[1]=z;
}

void tea_decrypt(int32 *v, int32 *k) {
    int32 y=v[0], z=v[1], sum=0xC6EF3720, i; /* set up */
    int32 delta=0x9e3779b9;                  /* a key schedule constant */
    int32 a=k[0], b=k[1], c=k[2], d=k[3];    /* cache key */
    for(i=0; i<32; i++) {                            /* basic cycle start */
        z -= ((y<<4) + c) ^ (y + sum) ^ ((y>>5) + d);
        y -= ((z<<4) + a) ^ (z + sum) ^ ((z>>5) + b);
        sum -= delta;                                /* end cycle */
    }
    v[0]=y;
    v[1]=z;
}

/************************************************************************/
/*                                                                      */
/*      Module title:           TEA_Encrypt                             */
/*      Module type:                                      */
/*                                                                      */
/*      Functional Description: The chipher function                    */
/*                                                                      */
/*      input parameter 1:      pointer to a 32 bit input/out string    */
/*                      2:      key       */
/************************************************************************/
void TEA_Encrypt( uint8* buf, uint8* key )
{
	uint8  n=BLOCK_SIZE/4;
	uint32 *v=(unsigned long *)buf;
	uint32 *k=(unsigned long *)key;
	uint32 z = v[n - 1],y = v[0],sum = 0,e ;
	uint8 p,q ;
	// Coding Part 
	q = S_LOOPTIME + 52 / n ;
	while ( q-- > 0 )
	{
		sum += DELTA ;
		e = sum >> 2 & 3 ;
		for ( p = 0 ; p < n - 1 ; p++ )
			y = v[p + 1],
			z = v[p] += MX;
		y = v[0] ;
		z = v[n - 1] += MX;
	}
}
/************************************************************************/
/*                                                                      */
/*      Module title:           TEA_Decrpyt                             */
/*      Module type:                                      */
/*                                                                      */
/*      Functional Description: The chipher function                    */
/*                                                                      */
/*      input parameter 1:      pointer to a 32 bit input/out string    */
/*                      2:      key       */
/************************************************************************/
void TEA_Decrpyt( uint8* buf, uint8* key )
{
	uint8 n=BLOCK_SIZE/4;
	uint32 *v=(unsigned long *)buf;
	uint32 *k=(unsigned long *)key;
	uint32 z = v[n - 1],y = v[0],sum = 0,e ;
	uint8 p,q ;
	
	//Decoding Part...
	q = S_LOOPTIME + 52 / n ;
	sum = q * DELTA ;
	while ( sum != 0 )
	{
		e = sum >> 2 & 3 ;
		for ( p = n - 1 ; p > 0 ; p-- )
			z = v[p - 1],
			y = v[p] -= MX;
		z = v[n - 1] ;
		y = v[0] -= MX;
		sum -= DELTA ;
	}
}

void Encpt_EncryptExt(uint8* InBuf)
{
	//tea_encrypt((int32 *)InBuf,(int32 *)&Encpt_KeyTableExt[0]);
	tea_encrypt((int32 *)InBuf,(int32 *)Encpt_KeyTableExt32);	
}

void Encpt_DecrpytExt( uint8* InBuf)
{
	//tea_decrypt((int32 *)InBuf,(int32 *)&Encpt_KeyTableExt[0]);
	tea_decrypt((int32 *)InBuf,(int32 *)Encpt_KeyTableExt32);
}

extern uint32 ProtoAnaly_RtcLocalTime;
extern Rtc_Type ProtoAnaly_Rtcinfo;

#define B     pBuff[8]
#define C     pBuff[9]
#define D     pBuff[10]
#define E     pBuff[11]
#define F     pBuff[12]
#define G     pBuff[13]
#define ACTIVE     pBuff[14]

/****************************************************************************************************
**Function:
	uint8 sys_CheckGetDynamicFlat(uint8* pDeviceId, uint8* pDevicePassword, uint8* pTouchKey)
**Author: rory
**Description: >
**Input: 
**Output: 
****************************************************************************************************/
uint8 sys_CheckGetDynamicFlat(uint32 DeviceId, uint32 DevicePassword, uint8* pTouchKey, uint8  length)
{
	uint32 inputPassword;
	uint32 startTime;
	//uint32 currentTime;
	uint8 temp, l;

	uint32 int07f;
	uint32 int06f;
	uint32 int05f;
	uint32 int04f;
	uint16 int03f;
	uint16 int02f;
	uint8 int01f;
	uint8 int00f;
	uint8 ret;
	
	l = length;
	if(length&0x01)
	{
		l--;
		int00f = pTouchKey[(l>>1)] &0xf0;
		int00f >>= 4;
		l=l-2;
		temp = pTouchKey[(l>>1)];
		int01f = temp&0x0f;
		int02f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)];
		int03f = temp&0x0f;
		int04f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)];
		int05f = temp&0x0f;
		int06f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)];
		int07f = temp&0x0f;
	}
	else
	{
		temp = pTouchKey[(l>>1)-1];
		int00f = temp&0x0f;
		int01f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)-1];
		int02f = temp&0x0f;
		int03f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)-1];
		int04f = temp&0x0f;
		int05f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)-1];
		int06f = temp&0x0f;
		int07f = temp>>4;
	}
	int07f = int07f*10000000;
	int06f = int06f*1000000;
	int05f = int05f*100000;
	int04f = int04f*10000;
	int03f = int03f*1000;
	int02f = int02f*100;
	int01f = int01f*10;
	int00f = int00f;

	inputPassword =int07f+int06f+int05f+int04f+int03f+int02f+int01f+int00f;
	
  //uint8* pBuff  = (uint8*)malloc(20);
  uint8 pBuff[20];
  #define A     pBuff[14]
  #define Type pBuff[15]
  #define Index pBuff[16]
  #define T       pBuff[17]
	A = (inputPassword & 0x0f000000) >> 24;
	B = (inputPassword & 0x00f00000) >> 20;
	C = (inputPassword & 0x000f0000) >> 16;
	D = (inputPassword & 0x0000f000) >> 12;
	E = (inputPassword & 0x00000f00) >> 8;
	F = (inputPassword & 0x000000f0) >> 4;
	G = inputPassword & 0x0000000f;
	T = G&0x03;
	G = (G&0xfc)|(A&0x03);
	A = T;
	Type = (A<<1)|(B>>3);

	if(Type == 4)
	{/* 单次*/
		temp = D;
		D = C;
		C = temp;
	}
	else if(Type == 5)
	{/* 永久*/
		temp = E;
		E = C;
		C = temp;
	}
	startTime = BUILD_UINT32(((E<<4)|F), ((C<<4)|D), (B&0x07), 0);
	for(uint8 i=0; i<G; i++)
	{// << 
		if(startTime&0x40000)
		{
			T = 1;
		}
		else
		{
			T = 0;
		}
		startTime <<= 1;
		startTime &= 0x7FFFE;
		startTime |= T;
	}
	B = ((startTime>>16)&0x07) | (B&0x8);
	C = (startTime&0x0F000)>>12;
	D = (startTime&0x00F00)>>8;
	E = (startTime&0x000F0)>>4;
	F = (startTime&0x0000F);
	temp = G;
	G = (C&0x03);
	G |= (E&0x0C);
	E = (E&0x03)|(temp&0x0C);
	C = (C&0x0C)|(temp&0x03);
	if(Type != 4)
	{
		pBuff[0] = ONEHI_UINT32(DeviceId);
		pBuff[1] = ONELO_UINT32(DeviceId);
		pBuff[2] = TWOHI_UINT32(DevicePassword);
		pBuff[3] = TWOLO_UINT32(DevicePassword);
		pBuff[4] = ONEHI_UINT32(DevicePassword);
		pBuff[5] = ProtoAnaly_Rtcinfo.Year;
		pBuff[6] = ProtoAnaly_Rtcinfo.Month;
		pBuff[7] = ProtoAnaly_Rtcinfo.Day;
		Encpt_EncryptExt(pBuff);
	}
	
	uint8 temp_buf[12];
	uint32 keytemp;
	if(length&0x01)
	{
		keytemp = BUILD_UINT32(pTouchKey[(length>>1)-1],pTouchKey[(length>>1)-2],pTouchKey[(length>>1)-3],pTouchKey[(length>>1)-4]);
		keytemp<<=4;
		keytemp|=(pTouchKey[(length>>1)]>>4);
	}
	else 
	{
		keytemp = BUILD_UINT32(pTouchKey[(length>>1)-1],pTouchKey[(length>>1)-2],pTouchKey[(length>>1)-3],pTouchKey[(length>>1)-4]);
	}

	if(Type == 3)
	{// 清空密码
		if((pBuff[0] == ((F<<4)|G)) && (pBuff[1] == ((D<<4)|E)) && ((pBuff[2]&0x7f) == (((B&0x7)<<4)|C)))
		{
			LockKey_Remove_PasswordKey();
			Sys_PataCfg.StartTime = BUILD_TIME(ProtoAnaly_Rtcinfo.Year, ProtoAnaly_Rtcinfo.Month, ProtoAnaly_Rtcinfo.Day,
									ProtoAnaly_Rtcinfo.Hour, 0, 0);
			Sys_GenFun32To8(keytemp, temp_buf);
		#if (defined(SUPPORT_RECORD_LOC_STORE)&&(SUPPORT_RECORD_LOC_STORE == STD_TRUE))
			Access_WriteRecordFlash(temp_buf,ProtoAnaly_RtcLocalTime,KEY_TYPE_CLEAR,ACCESS_OPEN_LOCK_TPYE);
		#endif
		#if (SUPPORT_RECORD_RTIME_SEND == STD_TRUE)
			ProtoAnaly_AddRecordCmd(temp_buf,ProtoAnaly_RtcLocalTime,KEY_TYPE_CLEAR,ACCESS_OPEN_LOCK_TPYE);
		#endif
		    Sys_StorePara();
			Access_FlashArrang();
			return E_END;
		}
		return E_NOT_OK;
	}
	else if(Type == 4)
	{// 单次密码
		uint8 s, e;
		pBuff[0] = ONEHI_UINT32(DeviceId);
		pBuff[1] = ONELO_UINT32(DeviceId);
		pBuff[2] = TWOHI_UINT32(DevicePassword);
		pBuff[3] = TWOLO_UINT32(DevicePassword);
		pBuff[4] = ProtoAnaly_Rtcinfo.Year;
		pBuff[5] = ProtoAnaly_Rtcinfo.Month;
		pBuff[6] = ProtoAnaly_Rtcinfo.Day;
		if(ProtoAnaly_Rtcinfo.Hour<22)
		{
			if(ProtoAnaly_Rtcinfo.Hour>=2)
			{
				s = ProtoAnaly_Rtcinfo.Hour-2;
			}
			else
			{
				s = 0;
			}
			e = ProtoAnaly_Rtcinfo.Hour+2;
		}
		else
		{
			s = ProtoAnaly_Rtcinfo.Hour-2;
			e = 23;
		}
		for(; s<(e+1); s++)
		{
			memcpy(temp_buf, pBuff, 7);
			temp_buf[7] = s;
			Encpt_EncryptExt(temp_buf);
			if((temp_buf[0] == ((F<<4)|G)) && (temp_buf[1] == ((D<<4)|E)))
			{
				break;
			}
		}
		if(s>e)
		{
		return E_NOT_OK;
	}
//		uint8 keyId = ((B&0x07)<<4)|C;
		uint8 retType;
		if(ProtoAnaly_Rtcinfo.Hour>s)   
		{
		    retType = ACCESS_NG_TPYE|ACCESS_OPEN_LOCK_TPYE;
			ret = E_END;
		}
		else
		{
			BlackList_Add(KEY_TYPE_ONE_TIME, keytemp,
					BUILD_TIME(ProtoAnaly_Rtcinfo.Year,ProtoAnaly_Rtcinfo.Month,ProtoAnaly_Rtcinfo.Day,
					(e+1),59,59));
			BlackList_StorePara();
			retType = ACCESS_OPEN_LOCK_TPYE;
			ret = E_OK;
		}
		Sys_GenFun32To8(keytemp, temp_buf);
		Access_WriteRecordFlash(temp_buf,ProtoAnaly_RtcLocalTime,KEY_TYPE_ONE_TIME,retType);
		return ret;
	}
	if((pBuff[0] == ((F<<4)|G)) && (pBuff[1] == ((D<<4)|E)))
	{
		
	}
	else
	{
		return E_NOT_OK;
	}

	if(Type == 5)
	{// 永义密码
		startTime = BUILD_TIME(ProtoAnaly_Rtcinfo.Year,ProtoAnaly_Rtcinfo.Month,ProtoAnaly_Rtcinfo.Day,
		ProtoAnaly_Rtcinfo.Hour,ProtoAnaly_Rtcinfo.Minute,ProtoAnaly_Rtcinfo.Second); // 开始时间
		LockKey_Add_VisitCodeKey(KEY_TYPE_FOREVER, keytemp, startTime, 0xffffffff);
		Sys_GenFun32To8(keytemp, temp_buf);
	#if (defined(SUPPORT_RECORD_LOC_STORE)&&(SUPPORT_RECORD_LOC_STORE == STD_TRUE))
		Access_WriteRecordFlash(temp_buf,ProtoAnaly_RtcLocalTime,KEY_TYPE_FOREVER,ACCESS_OPEN_LOCK_TPYE);
	#endif
	#if (SUPPORT_RECORD_RTIME_SEND == STD_TRUE)
		ProtoAnaly_AddRecordCmd(temp_buf,ProtoAnaly_RtcLocalTime,KEY_TYPE_FOREVER,ACCESS_OPEN_LOCK_TPYE);
	#endif
		return E_OK;
	}
	return E_NOT_OK;
}

uint8 sys_CheckGetDynamicHotel(uint32 DeviceId, uint32 DevicePassword, uint8* pTouchKey, uint8  length)
{
	uint32 inputPassword;
	uint32 startTime;
	//uint32 currentTime;
	uint32 endTime;
	uint8 temp, l;

	uint32 int06f;
	uint32 int05f;
	uint32 int04f;
	uint16 int03f;
	uint16 int02f;
	uint8 int01f;
	uint8 int00f;

	l = length;
	if(length&0x01)
	{
		l--;
		int00f = pTouchKey[(l>>1)] &0xf0;
		int00f >>= 4;
		l=l-2;
		temp = pTouchKey[(l>>1)];
		int01f = temp&0x0f;
		int02f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)];
		int03f = temp&0x0f;
		int04f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)];
		int05f = temp&0x0f;
		int06f = temp>>4;
	}
	else
	{
		temp = pTouchKey[(l>>1)-1];
		int00f = temp&0x0f;
		int01f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)-1];
		int02f = temp&0x0f;
		int03f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)-1];
		int04f = temp&0x0f;
		int05f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)-1];
		int06f = temp&0x0f;
	}
	int06f = int06f*1000000;
	int05f = int05f*100000;
	int04f = int04f*10000;
	int03f = int03f*1000;
	int02f = int02f*100;
	int01f = int01f*10;
	int00f = int00f;

	inputPassword = int06f+int05f+int04f+int03f+int02f+int01f+int00f;
	
#if 1
       //uint8* pBuff  = (uint8*)malloc(20);
       uint8 pBuff[20];
	//uint8 A = (inputPassword & 0x0f000000) >> 24;
	B = (inputPassword & 0x00f00000) >> 20;
	C = (inputPassword & 0x000f0000) >> 16;
	D = (inputPassword & 0x0000f000) >> 12;
	E = (inputPassword & 0x00000f00) >> 8;
	F = (inputPassword & 0x000000f0) >> 4;
	G = inputPassword & 0x0000000f;

	#define START   pBuff[14]
	#define YEARMONTH  pBuff[15]
	#define DAY       pBuff[16]
	#define END       pBuff[17]
	#define DAY_FLAG      pBuff[18]
	#define REMOVEKEY      pBuff[19]
	
	startTime = BUILD_UINT32(((E<<4)|F), ((C<<4)|D), B, 0);
	for(uint8 i=0; i<G; i++)
	{// << 
		if(startTime&0x40000)
		{
			B = 1;
		}
		else
		{
			B = 0;
		}
		startTime <<= 1;
		startTime &= 0x7FFFE;
		startTime |= B;
	}
	B = startTime>>16;
	C = (startTime&0x0F000)>>12;
	D = (startTime&0x00F00)>>8;
	E = (startTime&0x000F0)>>4;
	F = (startTime&0x0000F);
	START = G;
	G = (C&0x03);
	G |= (E&0x0C);
	E = (E&0x03)|(START&0x0C);
	C = (C&0x0C)|(START&0x03);
	
	YEARMONTH = (B&0x03);
	//DAY_FLAG = B&0x01;
	DAY = ((C<<1)|(D>>3));
	REMOVEKEY = (B&0x04)>>2;
	START = (D&0x04)>>2;
	END = (((D&0x03)<<3)|(E>>1)); 
#endif
	//inputPassword = ProtoAnaly_Rtcinfo.Year;
	//inputPassword = BUILD_HOTEL_TIME(ProtoAnaly_Rtcinfo.Year, ProtoAnaly_Rtcinfo.Month, ProtoAnaly_Rtcinfo.Day, ((ProtoAnaly_Rtcinfo.Hour*60 + ProtoAnaly_Rtcinfo.Minute)/15));
	/*uint8* pRtcBuf  = osal_mem_alloc(6);
	Lock_Rtc_Read(pRtcBuf);*/
	uint8 i;
	uint8 temp_buf[8];

	for(i=0; i<3; i++)
	{
		pBuff[0] = ONEHI_UINT32(DeviceId);
		pBuff[1] = ONELO_UINT32(DeviceId);
		pBuff[2] = TWOHI_UINT32(DevicePassword);
		pBuff[3] = TWOLO_UINT32(DevicePassword);

		if(i == 0)
		{
			pBuff[4] = ProtoAnaly_Rtcinfo.Year;
			pBuff[5] = ProtoAnaly_Rtcinfo.Month;
			pBuff[6] = ProtoAnaly_Rtcinfo.Day;
		}
		else if(i == 1)
		{
			if((ProtoAnaly_Rtcinfo.Month ==1)&&(ProtoAnaly_Rtcinfo.Day == 1))
			{// 1月1日
				pBuff[4] = ProtoAnaly_Rtcinfo.Year-1;
				pBuff[5] = 12;
				pBuff[6] = 31;
			}
			else if(((ProtoAnaly_Rtcinfo.Month == 2) || (ProtoAnaly_Rtcinfo.Month == 4) || (ProtoAnaly_Rtcinfo.Month == 6) || (ProtoAnaly_Rtcinfo.Month == 9)
				|| (ProtoAnaly_Rtcinfo.Month == 11))&&(ProtoAnaly_Rtcinfo.Day == 1))
			{
				pBuff[4] = ProtoAnaly_Rtcinfo.Year;
				pBuff[5] = ProtoAnaly_Rtcinfo.Month-1;
				pBuff[6] = 31;
			}
			else if ((ProtoAnaly_Rtcinfo.Month == 3) && (ProtoAnaly_Rtcinfo.Day == 1))
			{
				pBuff[4] = ProtoAnaly_Rtcinfo.Year;
				pBuff[5] = 2;
				pBuff[6] = 28;
			}
			else if(((ProtoAnaly_Rtcinfo.Month ==5) || (ProtoAnaly_Rtcinfo.Month ==7) || (ProtoAnaly_Rtcinfo.Month ==8) || (ProtoAnaly_Rtcinfo.Month ==10)
				|| (ProtoAnaly_Rtcinfo.Month ==12))&&(ProtoAnaly_Rtcinfo.Day == 1))
			{
				pBuff[4] = ProtoAnaly_Rtcinfo.Year;
				pBuff[5] = ProtoAnaly_Rtcinfo.Month-1;
				pBuff[6] = 30;
			}
			else
			{
				pBuff[4] = ProtoAnaly_Rtcinfo.Year;
				pBuff[5] = ProtoAnaly_Rtcinfo.Month;
				pBuff[6] = ProtoAnaly_Rtcinfo.Day-1;
			}
		}
		else if(i == 2)
		{
			if ((ProtoAnaly_Rtcinfo.Month == 3) && (ProtoAnaly_Rtcinfo.Day == 1))
			{
				pBuff[4] = ProtoAnaly_Rtcinfo.Year;
				pBuff[5] = 2;
				pBuff[6] = 29;
			}
			else
			{
				//free(pBuff);
				//osal_mem_free(pRtcBuf);
				return E_NOT_OK;
			}
		}
		
		uint8 s;
		if(START == 0)
		{
			for(s=0; s<13; s++)
			{
				memcpy(temp_buf, pBuff, 7);
				temp_buf[7] = s;
				Encpt_EncryptExt(temp_buf);
				if((temp_buf[0] == ((F<<4)|G)) && ((temp_buf[1]&0x01) == (E&0x1)))
				{
					break;
				}
			}
			if(s > 12)
			{
				continue;
			}
		}
		else
		{
			for(s=13; s<24; s++)
			{
				memcpy(temp_buf, pBuff, 7);
				temp_buf[7] = s;
				Encpt_EncryptExt(temp_buf);
				if((temp_buf[0] == ((F<<4)|G)) && ((temp_buf[1]&0x01) == (E&0x1)))
				{
					break;
				}
			}
			if(s > 23)
			{
				continue;
			}
		}
		startTime = BUILD_TIME(pBuff[4], pBuff[5], pBuff[6], s, 0, 0);

		
		if(YEARMONTH == 0)
		{
			endTime = BUILD_TIME(pBuff[4], pBuff[5], DAY, END, 0, 0);
		}
		else if(YEARMONTH == 1)
		{
			endTime = BUILD_TIME(pBuff[4], (pBuff[5]+1), DAY, END, 0, 0);
		}
		else if(YEARMONTH == 2)
		{
			endTime = BUILD_TIME((pBuff[4]+1), 1, DAY, END, 0, 0);
		}
		uint16 timeLimit = 25;
		if(ProtoAnaly_Rtcinfo.Day == pBuff[6])
		{
			if(ProtoAnaly_Rtcinfo.Hour >= s)
			{
				timeLimit = ProtoAnaly_Rtcinfo.Hour -s;
			}
		}
		else if(ProtoAnaly_Rtcinfo.Month == pBuff[5])
		{
			if(ProtoAnaly_Rtcinfo.Day > pBuff[6])
			{
				if((ProtoAnaly_Rtcinfo.Hour <= s) && ((ProtoAnaly_Rtcinfo.Day - pBuff[6]) == 1))
				{
					timeLimit = 24 - (s-ProtoAnaly_Rtcinfo.Hour);
				}
			}
		}
		else  if(ProtoAnaly_Rtcinfo.Month >= pBuff[5])
		{
			if((ProtoAnaly_Rtcinfo.Month - pBuff[5]) == 1)
			{
				if(ProtoAnaly_Rtcinfo.Day == 1)
				{
					timeLimit = (24-pBuff[6]) + s;
				}
			}
		}
		else if(ProtoAnaly_Rtcinfo.Year != pBuff[4])
		{
			if((ProtoAnaly_Rtcinfo.Month == 1) && (pBuff[5] == 12))
			{
				if((ProtoAnaly_Rtcinfo.Day == 1) && (pBuff[6] == 31))
				{
					timeLimit = (24-pBuff[6]) + s;
				}
			}
		}
		if(timeLimit>24)
		{
			//free(pBuff);
			return E_NOT_OK;
		}
		else
		{
			break;
		}
	}
	//free(pBuff);
	if(i >= 3)
	{
		//osal_mem_free(pRtcBuf);
		return E_NOT_OK;
	}
	
	if(Sys_PataCfg.StartTime > startTime)
	{
		//osal_mem_free(pRtcBuf);
		return E_TIME_OUT;
	}

	if(REMOVEKEY == 1)
	{
		if(startTime > Sys_PataCfg.StartTime) 
		{
			REMOVEKEY = 2; 
		}
	}
	
	if((ProtoAnaly_RtcLocalTime > endTime) || (ProtoAnaly_RtcLocalTime < startTime))
	{
		//osal_mem_free(pRtcBuf);
		return E_NOT_OK;
	}
	else
	{
		//osal_mem_free(pRtcBuf);
		uint32 keytemp;
		if(length&0x01)
		{
			keytemp = BUILD_UINT32(pTouchKey[(length>>1)-1],pTouchKey[(length>>1)-2],pTouchKey[(length>>1)-3],0);
			keytemp<<=4;
			keytemp|=(pTouchKey[(length>>1)]>>4);
		}
		else 
		{
			keytemp = BUILD_UINT32(pTouchKey[(length>>1)-1],pTouchKey[(length>>1)-2],pTouchKey[(length>>1)-3],pTouchKey[(length>>1)-4]);
			keytemp &= 0x0fffffff;
		}
		keytemp |= 0xf0000000;
		if(REMOVEKEY == 2)
		{
			//Flat_RemoveKey(1, 0); 
			// remove all key
			LockKey_Remove_VisitCodeKey();
			Sys_PataCfg.StartTime = BUILD_TIME(ProtoAnaly_Rtcinfo.Year, ProtoAnaly_Rtcinfo.Month, ProtoAnaly_Rtcinfo.Day,
									ProtoAnaly_Rtcinfo.Hour, 0, 0);
			Sys_StorePara();
		}
		startTime = BUILD_TIME(ProtoAnaly_Rtcinfo.Year, ProtoAnaly_Rtcinfo.Month, ProtoAnaly_Rtcinfo.Day,
									ProtoAnaly_Rtcinfo.Hour, ProtoAnaly_Rtcinfo.Minute, ProtoAnaly_Rtcinfo.Second);
		LockKey_Add_VisitCodeKey(KEY_TYPE_TIME_SLOT_7, keytemp, startTime, endTime);
		uint8 temp_buf[8]={0};
		Sys_GenFun32To8(keytemp, temp_buf);
	#if (defined(SUPPORT_RECORD_LOC_STORE)&&(SUPPORT_RECORD_LOC_STORE == STD_TRUE))
		Access_WriteRecordFlash(temp_buf,ProtoAnaly_RtcLocalTime,KEY_TYPE_TIME_SLOT_7,ACCESS_OPEN_LOCK_TPYE);
	#endif
	#if (SUPPORT_RECORD_RTIME_SEND == STD_TRUE)
		ProtoAnaly_AddRecordCmd(temp_buf,ProtoAnaly_RtcLocalTime,KEY_TYPE_TIME_SLOT_7,ACCESS_OPEN_LOCK_TPYE);
	#endif
		return E_OK;
	}
}


uint8 sys_CheckGetDynamicHotel_V8(uint32 DeviceId, uint32 DevicePassword, uint8* pTouchKey, uint8  length)
{
	uint32 inputPassword;
	uint32 startTime;
	uint32 endTime;
	uint8 temp, l;

	uint32 int07f;
	uint32 int06f;
	uint32 int05f;
	uint32 int04f;
	uint16 int03f;
	uint16 int02f;
	uint8 int01f;
	uint8 int00f;

	l = length;
	if(length&0x01)
	{
		l--;
		int00f = pTouchKey[(l>>1)] &0xf0;
		int00f >>= 4;
		l=l-2;
		temp = pTouchKey[(l>>1)];
		int01f = temp&0x0f;
		int02f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)];
		int03f = temp&0x0f;
		int04f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)];
		int05f = temp&0x0f;
		int06f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)];
		int07f = temp&0x0f;
	}
	else
	{
		temp = pTouchKey[(l>>1)-1];
		int00f = temp&0x0f;
		int01f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)-1];
		int02f = temp&0x0f;
		int03f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)-1];
		int04f = temp&0x0f;
		int05f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)-1];
		int06f = temp&0x0f;
		int07f = temp>>4;
	}
	int07f = int07f*10000000;
	int06f = int06f*1000000;
	int05f = int05f*100000;
	int04f = int04f*10000;
	int03f = int03f*1000;
	int02f = int02f*100;
	int01f = int01f*10;
	int00f = int00f;

	inputPassword = int07f+int06f+int05f+int04f+int03f+int02f+int01f+int00f;
	
#if 1
       //uint8* pBuff  = (uint8*)malloc(20);
       uint8 pBuff[20];
	A = (inputPassword & 0x0f000000) >> 24;
	B = (inputPassword & 0x00f00000) >> 20;
	C = (inputPassword & 0x000f0000) >> 16;
	D = (inputPassword & 0x0000f000) >> 12;
	E = (inputPassword & 0x00000f00) >> 8;
	F = (inputPassword & 0x000000f0) >> 4;
	G = inputPassword & 0x0000000f;

	uint8 ENCPT_HIGH;
	#define START   pBuff[14]
	#define YEARMONTH  pBuff[15]
	#define DAY       pBuff[16]
	#define END       pBuff[17]
	#define DAY_FLAG      pBuff[18]
	#define REMOVEKEY      pBuff[19]

	ENCPT_HIGH = ((A&0x03)<<2)|((B&0x08)>>2);
	startTime = BUILD_UINT32(((E<<4)|F), ((C<<4)|D), B, 0);
	for(uint8 i=0; i<G; i++)
	{// << 
		if(startTime&0x40000)
		{
			B = 1;
		}
		else
		{
			B = 0;
		}
		startTime <<= 1;
		startTime &= 0x7FFFE;
		startTime |= B;
	}
	B = startTime>>16;
	C = (startTime&0x0F000)>>12;
	D = (startTime&0x00F00)>>8;
	E = (startTime&0x000F0)>>4;
	F = (startTime&0x0000F);
	START = G;
	G = (C&0x03);
	G |= (E&0x0C);
	E = (E&0x03)|(START&0x0C);
	C = (C&0x0C)|(START&0x03);
	
	YEARMONTH = (B&0x03);
	//DAY_FLAG = B&0x01;
	DAY = ((C<<1)|(D>>3));
	REMOVEKEY = (B&0x04)>>2;
	START = (D&0x04)>>2;
	END = (((D&0x03)<<3)|(E>>1)); 
#endif
	uint8 i;
	uint8 temp_buf[8];

	for(i=0; i<3; i++)
	{
		pBuff[0] = ONEHI_UINT32(DeviceId);
		pBuff[1] = ONELO_UINT32(DeviceId);
		pBuff[2] = TWOHI_UINT32(DevicePassword);
		pBuff[3] = TWOLO_UINT32(DevicePassword);

		if(i == 0)
		{
			pBuff[4] = ProtoAnaly_Rtcinfo.Year;
			pBuff[5] = ProtoAnaly_Rtcinfo.Month;
			pBuff[6] = ProtoAnaly_Rtcinfo.Day;
		}
		else if(i == 1)
		{
			if((ProtoAnaly_Rtcinfo.Month ==1)&&(ProtoAnaly_Rtcinfo.Day == 1))
			{// 1月1日
				pBuff[4] = ProtoAnaly_Rtcinfo.Year-1;
				pBuff[5] = 12;
				pBuff[6] = 31;
			}
			else if(((ProtoAnaly_Rtcinfo.Month == 2) || (ProtoAnaly_Rtcinfo.Month == 4) || (ProtoAnaly_Rtcinfo.Month == 6) || (ProtoAnaly_Rtcinfo.Month == 9)
				|| (ProtoAnaly_Rtcinfo.Month == 11))&&(ProtoAnaly_Rtcinfo.Day == 1))
			{
				pBuff[4] = ProtoAnaly_Rtcinfo.Year;
				pBuff[5] = ProtoAnaly_Rtcinfo.Month-1;
				pBuff[6] = 31;
			}
			else if ((ProtoAnaly_Rtcinfo.Month == 3) && (ProtoAnaly_Rtcinfo.Day == 1))
			{
				pBuff[4] = ProtoAnaly_Rtcinfo.Year;
				pBuff[5] = 2;
				pBuff[6] = 28;
			}
			else if(((ProtoAnaly_Rtcinfo.Month ==5) || (ProtoAnaly_Rtcinfo.Month ==7) || (ProtoAnaly_Rtcinfo.Month ==8) || (ProtoAnaly_Rtcinfo.Month ==10)
				|| (ProtoAnaly_Rtcinfo.Month ==12))&&(ProtoAnaly_Rtcinfo.Day == 1))
			{
				pBuff[4] = ProtoAnaly_Rtcinfo.Year;
				pBuff[5] = ProtoAnaly_Rtcinfo.Month-1;
				pBuff[6] = 30;
			}
			else
			{
				pBuff[4] = ProtoAnaly_Rtcinfo.Year;
				pBuff[5] = ProtoAnaly_Rtcinfo.Month;
				pBuff[6] = ProtoAnaly_Rtcinfo.Day-1;
			}
		}
		else if(i == 2)
		{
			if ((ProtoAnaly_Rtcinfo.Month == 3) && (ProtoAnaly_Rtcinfo.Day == 1))
			{
				pBuff[4] = ProtoAnaly_Rtcinfo.Year;
				pBuff[5] = 2;
				pBuff[6] = 29;
			}
			else
			{
				//free(pBuff);
				//osal_mem_free(pRtcBuf);
				return E_NOT_OK;
			}
		}

		ENCPT_HIGH |= (E&0x1);
		uint8 s;
		if(START == 0)
		{
			for(s=0; s<13; s++)
			{
				memcpy(temp_buf, pBuff, 7);
				temp_buf[7] = s;
				Encpt_EncryptExt(temp_buf);
				if((temp_buf[0] == ((F<<4)|G)) && ((temp_buf[1]&0x0f) == (ENCPT_HIGH&0xf)))
				{
					break;
				}
			}
			if(s > 12)
			{
				continue;
			}
		}
		else
		{
			for(s=13; s<24; s++)
			{
				memcpy(temp_buf, pBuff, 7);
				temp_buf[7] = s;
				Encpt_EncryptExt(temp_buf);
				if((temp_buf[0] == ((F<<4)|G)) && ((temp_buf[1]&0x0f) == (ENCPT_HIGH&0xf)))
				{
					break;
				}
			}
			if(s > 23)
			{
				continue;
			}
		}
		startTime = BUILD_TIME(pBuff[4], pBuff[5], pBuff[6], s, 0, 0);

		
		if(YEARMONTH == 0)
		{
			endTime = BUILD_TIME(pBuff[4], pBuff[5], DAY, END, 0, 0);
		}
		else if(YEARMONTH == 1)
		{
			endTime = BUILD_TIME(pBuff[4], (pBuff[5]+1), DAY, END, 0, 0);
		}
		else if(YEARMONTH == 2)
		{
			endTime = BUILD_TIME((pBuff[4]+1), 1, DAY, END, 0, 0);
		}
		uint16 timeLimit = 25;
		if(ProtoAnaly_Rtcinfo.Day == pBuff[6])
		{
			if(ProtoAnaly_Rtcinfo.Hour >= s)
			{
				timeLimit = ProtoAnaly_Rtcinfo.Hour -s;
			}
		}
		else if(ProtoAnaly_Rtcinfo.Month == pBuff[5])
		{
			if(ProtoAnaly_Rtcinfo.Day > pBuff[6])
			{
				if((ProtoAnaly_Rtcinfo.Hour <= s) && ((ProtoAnaly_Rtcinfo.Day - pBuff[6]) == 1))
				{
					timeLimit = 24 - (s-ProtoAnaly_Rtcinfo.Hour);
				}
			}
		}
		else  if(ProtoAnaly_Rtcinfo.Month >= pBuff[5])
		{
			if((ProtoAnaly_Rtcinfo.Month - pBuff[5]) == 1)
			{
				if(ProtoAnaly_Rtcinfo.Day == 1)
				{
					timeLimit = (24-pBuff[6]) + s;
				}
			}
		}
		else if(ProtoAnaly_Rtcinfo.Year != pBuff[4])
		{
			if((ProtoAnaly_Rtcinfo.Month == 1) && (pBuff[5] == 12))
			{
				if((ProtoAnaly_Rtcinfo.Day == 1) && (pBuff[6] == 31))
				{
					timeLimit = (24-pBuff[6]) + s;
				}
			}
		}
		if(timeLimit>24)
		{
			//free(pBuff);
			return E_NOT_OK;
		}
		else
		{
			break;
		}
	}
	//free(pBuff);
	if(i >= 3)
	{
		//osal_mem_free(pRtcBuf);
		return E_NOT_OK;
	}
	
	if(Sys_PataCfg.StartTime > startTime)
	{
		//osal_mem_free(pRtcBuf);
		return E_TIME_OUT;
	}

	if(REMOVEKEY == 1)
	{
		if(startTime > Sys_PataCfg.StartTime) 
		{
			REMOVEKEY = 2; 
		}
	}
	
	if((ProtoAnaly_RtcLocalTime > endTime) || (ProtoAnaly_RtcLocalTime < startTime))
	{
		//osal_mem_free(pRtcBuf);
		return E_NOT_OK;
	}
	else
	{
		//osal_mem_free(pRtcBuf);
		uint32 keytemp;
		if(length&0x01)
		{
			keytemp = BUILD_UINT32(pTouchKey[(length>>1)-1],pTouchKey[(length>>1)-2],pTouchKey[(length>>1)-3],pTouchKey[(length>>1)-4]);
			keytemp<<=4;
			keytemp|=(pTouchKey[(length>>1)]>>4);
		}
		else 
		{
			keytemp = BUILD_UINT32(pTouchKey[(length>>1)-1],pTouchKey[(length>>1)-2],pTouchKey[(length>>1)-3],pTouchKey[(length>>1)-4]);
		}
		if(REMOVEKEY == 2)
		{
			//Flat_RemoveKey(1, 0); 
			// remove all key
			LockKey_Remove_VisitCodeKey();
			Sys_PataCfg.StartTime = BUILD_TIME(ProtoAnaly_Rtcinfo.Year, ProtoAnaly_Rtcinfo.Month, ProtoAnaly_Rtcinfo.Day,
									ProtoAnaly_Rtcinfo.Hour, 0, 0);
			Sys_StorePara();
		}
		startTime = BUILD_TIME(ProtoAnaly_Rtcinfo.Year, ProtoAnaly_Rtcinfo.Month, ProtoAnaly_Rtcinfo.Day,
									ProtoAnaly_Rtcinfo.Hour, ProtoAnaly_Rtcinfo.Minute, ProtoAnaly_Rtcinfo.Second);
		LockKey_Add_VisitCodeKey(KEY_TYPE_TIME_SLOT_7, keytemp, startTime, endTime);
		uint8 temp_buf[8]={0};
		Sys_GenFun32To8(keytemp, temp_buf);
	#if (defined(SUPPORT_RECORD_LOC_STORE)&&(SUPPORT_RECORD_LOC_STORE == STD_TRUE))
		Access_WriteRecordFlash(temp_buf,ProtoAnaly_RtcLocalTime,KEY_TYPE_TIME_SLOT_7,ACCESS_OPEN_LOCK_TPYE);
	#endif
	#if (SUPPORT_RECORD_RTIME_SEND == STD_TRUE)
		ProtoAnaly_AddRecordCmd(temp_buf,ProtoAnaly_RtcLocalTime,KEY_TYPE_TIME_SLOT_7,ACCESS_OPEN_LOCK_TPYE);
	#endif
		return E_OK;
	}
}

/****************************************************************************************************
**Function:
	uint8 sys_CheckGetDynamicDate(uint8* pDeviceId, uint8* pDevicePassword, uint8* pTouchKey)
**Author: rory
**Description: >
**Input: 
**Output: 
****************************************************************************************************/
uint8 sys_CheckGetDynamicDate(uint32 DeviceId, uint32 DevicePassword, uint8* pTouchKey, uint8  length)
{
	uint32 inputPassword;
	uint32 startTime;
	uint32 endTime;
	uint8 temp, l;

	uint32 int07f;
	uint32 int06f;
	uint32 int05f;
	uint32 int04f;
	uint16 int03f;
	uint16 int02f;
	uint8 int01f;
	uint8 int00f;
	
	l = length;
	if(length&0x01)
	{
		l--;
		int00f = pTouchKey[(l>>1)] &0xf0;
		int00f >>= 4;
		l=l-2;
		temp = pTouchKey[(l>>1)];
		int01f = temp&0x0f;
		int02f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)];
		int03f = temp&0x0f;
		int04f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)];
		int05f = temp&0x0f;
		int06f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)];
		int07f = temp&0x0f;
	}
	else
	{
		temp = pTouchKey[(l>>1)-1];
		int00f = temp&0x0f;
		int01f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)-1];
		int02f = temp&0x0f;
		int03f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)-1];
		int04f = temp&0x0f;
		int05f = temp>>4;
		l=l-2;
		temp = pTouchKey[(l>>1)-1];
		int06f = temp&0x0f;
		int07f = temp>>4;
	}
	int07f = int07f*10000000;
	int06f = int06f*1000000;
	int05f = int05f*100000;
	int04f = int04f*10000;
	int03f = int03f*1000;
	int02f = int02f*100;
	int01f = int01f*10;
	int00f = int00f;

	inputPassword =int07f+int06f+int05f+int04f+int03f+int02f+int01f+int00f;
	
  //uint8* pBuff  = (uint8*)malloc(20);
  uint8 pBuff[20];
  #define A     pBuff[14]
  #define Type pBuff[15]
  #define Index pBuff[16]
  #define T       pBuff[17]
	A = (inputPassword & 0x0f000000) >> 24;
	B = (inputPassword & 0x00f00000) >> 20;
	C = (inputPassword & 0x000f0000) >> 16;
	D = (inputPassword & 0x0000f000) >> 12;
	E = (inputPassword & 0x00000f00) >> 8;
	F = (inputPassword & 0x000000f0) >> 4;
	G = inputPassword & 0x0000000f;
	T = G&0x03;
	G = (G&0xfc)|(A&0x03);
	A = T;
	Type = (A>>1);
	
	startTime = BUILD_UINT32(((E<<4)|F), ((C<<4)|D), (B&0x07), 0);
	for(uint8 i=0; i<G; i++)
	{// << 
		if(startTime&0x40000)
		{
			T = 1;
		}
		else
		{
			T = 0;
		}
		startTime <<= 1;
		startTime &= 0x7FFFE;
		startTime |= T;
	}
	B = ((startTime>>16)&0x07) | (B&0x8);
	C = (startTime&0x0F000)>>12;
	D = (startTime&0x00F00)>>8;
	E = (startTime&0x000F0)>>4;
	F = (startTime&0x0000F);
	temp = G;
	G = (C&0x03);
	G |= (E&0x0C);
	E = (E&0x03)|(temp&0x0C);
	C = (C&0x0C)|(temp&0x03);

	pBuff[0] = ONEHI_UINT32(DeviceId);
	pBuff[1] = ONELO_UINT32(DeviceId);
	pBuff[2] = TWOHI_UINT32(DevicePassword);
	pBuff[3] = TWOLO_UINT32(DevicePassword);
	
	if(Type == 1)
	{
		pBuff[4] = ((A&0x01)<<5)|(B<<1)|((C&0x08)>>3);
	}
	else
	{
		pBuff[4] = ((A&0x01)<<7)|((B&0x0E)<<3)|C;
	}
	pBuff[5] = ProtoAnaly_Rtcinfo.Year;
	pBuff[6] = ProtoAnaly_Rtcinfo.Month;
	pBuff[7] = ProtoAnaly_Rtcinfo.Day;
	Encpt_EncryptExt(pBuff);
	
	if(Type == 1)
	{
		if((pBuff[0] == ((F<<4)|G)) && ((pBuff[1]&0x3f) == (((D&0x03)<<4)|E)))
		{
			startTime = BUILD_TIME(ProtoAnaly_Rtcinfo.Year, ProtoAnaly_Rtcinfo.Month, ProtoAnaly_Rtcinfo.Day, 
				ProtoAnaly_Rtcinfo.Hour,ProtoAnaly_Rtcinfo.Minute, ProtoAnaly_Rtcinfo.Second);
			endTime = BUILD_TIME((ProtoAnaly_Rtcinfo.Year+((A&0x01)<<1|(B>>3))), (((B&0x07)<<1)|(C>>3)), ProtoAnaly_Rtcinfo.Day, 
				23,59, 59);
		}
		else
		{
			return E_NOT_OK;
		}
	}
	else
	{
		if((pBuff[0] == ((F<<4)|G)) && ((pBuff[1]&0x07) == (E&0x07)))
		{
			startTime = BUILD_TIME(ProtoAnaly_Rtcinfo.Year, ProtoAnaly_Rtcinfo.Month, ProtoAnaly_Rtcinfo.Day, 
				ProtoAnaly_Rtcinfo.Hour,ProtoAnaly_Rtcinfo.Minute, ProtoAnaly_Rtcinfo.Second);
			endTime = BUILD_TIME(ProtoAnaly_Rtcinfo.Year, (((A&0x01)<<3)|(B>>1)), (((B&0x01)<<4)|C), 
				23,59, 59);
		}
		else
		{
			return E_NOT_OK;
		}
	}
	uint8 temp_buf[12];
	uint32 keytemp;
	if(length&0x01)
	{
		keytemp = BUILD_UINT32(pTouchKey[(length>>1)-1],pTouchKey[(length>>1)-2],pTouchKey[(length>>1)-3],pTouchKey[(length>>1)-4]);
		keytemp<<=4;
		keytemp|=(pTouchKey[(length>>1)]>>4);
	}
	else 
	{
		keytemp = BUILD_UINT32(pTouchKey[(length>>1)-1],pTouchKey[(length>>1)-2],pTouchKey[(length>>1)-3],pTouchKey[(length>>1)-4]);
	}
	LockKey_Add_VisitCodeKey(KEY_TYPE_TIME_SLOT_8, keytemp, startTime, endTime);
	Sys_GenFun32To8(keytemp, temp_buf);
#if (defined(SUPPORT_RECORD_LOC_STORE)&&(SUPPORT_RECORD_LOC_STORE == STD_TRUE))
	Access_WriteRecordFlash(temp_buf,ProtoAnaly_RtcLocalTime,KEY_TYPE_TIME_SLOT_8,ACCESS_OPEN_LOCK_TPYE);
#endif
#if (SUPPORT_RECORD_RTIME_SEND == STD_TRUE)
	ProtoAnaly_AddRecordCmd(temp_buf,ProtoAnaly_RtcLocalTime,KEY_TYPE_TIME_SLOT_8,ACCESS_OPEN_LOCK_TPYE);
#endif
	return E_OK;
}

/************************ (C) COPYRIGHT Dahao Microelectronics *****END OF FILE****/
