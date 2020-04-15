

#include "main.h"

#define MOTOR_DRIVE_TIME250           250

uint8 Ver;   // °æ±¾

Sys_StateType Sys_State;
uint16 Sys_TimeOutX16mS;
uint8  Sys_CheckFlag;

Sys_PataCfgType	Sys_PataCfg;

uint8 Sys_CheckUpFlag = 0;
uint8  Sys_McuRestFlag = 0;
uint8  Sys_StoreFlag = 0;
uint16 Sys_RstTimsoutx64ms = 0;
uint16 Sys_StoreTimsoutx64ms = 0;
uint16 Sys_SleepErrTimsoutx64ms = 0;
uint32 Sys_RestTimex64ms;


uint32 Sys_WkTick = 0;
uint32 NwxtSys_Tick = 0;
uint8  Sys_WkupType;
uint8  Sys_ResetDevice;
uint8  Sys_frstPoweron;
uint8  Sys_SpiOnFlag;

#define RELOAD_COUNT (32768*60-1)     // 6s  

const uint8 Aes_KeyTable[16];

/****************************************************************************************************
**Function:
	void Ekey_Parainit(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Sys_ParainitFirst(void)
{
	memset(&Sys_PataCfg,0,sizeof(Sys_PataCfg));
//	Sys_PataCfg.Mac = BUILD_UINT32(m_mac_add[5], m_mac_add[4], m_mac_add[3], m_mac_add[2]);
	Sys_PataCfg.Pswd = 0x12345678;
	//Sys_PataCfg.SysId = CUSTOM_ID;
	Sys_PataCfg.CustomId = 0x6868;
	Sys_PataCfg.OpenLockTime = 9000;
	Sys_PataCfg.MotorTime = MOTOR_DRIVE_TIME250;
	Sys_PataCfg.HeartTime = 30;
	//Sys_PataCfg.CfgFlag = (SYSBIGFLG);
	Sys_PataCfg.BuildId = 0xff;
	Sys_PataCfg.PropertyId = 0;
	Sys_PataCfg.HotelPswd = 0;
	Sys_PataCfg.bleCfg.advInt = 1000;
	Sys_PataCfg.bleCfg.connectOff = 5000;

	Sys_PataCfg.bleCfg.txPower = 4;
	
	Sys_PataCfg.usedpage = 0x03;


	Sys_PataCfg.StartTime = 0;
	Sys_PataCfg.touchSensitive = 3;
//	Sys_PataCfg.SyncIndex = 0;
	Sys_PataCfg.Headadd = 0;
	Sys_PataCfg.HeadMac = 0;
	Sys_PataCfg.UploadRecord=1;
	Sys_PataCfg.SyncIndex = 1;

	Sys_PataCfg.CfgMode = 0x0B;    // 1011  
	memset (&Sys_PataCfg.CfgLog,0,sizeof(Sys_PataCfg.CfgLog)); 

	Access_EraseAllKey(KEY_TYPE_ALL);
	Access_EraseRecordData();
	memset(&LockKeyStatistics,0,sizeof(LockKeyStatistics));

	Sys_PataCfg.State =WORK_NORMAL;
	//Access_DefaultPaswd();
}


/****************************************************************************************************
**Function:
	void Ekey_Parainit(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Sys_Parainit(void)
{
	Sys_PataCfg.Pswd = 0x12345678;
	Sys_PataCfg.OpenLockTime = 9000;
	Sys_PataCfg.HeartTime = 30;
	//Sys_PataCfg.CfgFlag = (SYSBIGFLG);
	Sys_PataCfg.BuildId = 0xff;
	Sys_PataCfg.PropertyId = 0;
	Sys_PataCfg.HotelPswd = 0;
	Sys_PataCfg.bleCfg.advInt = 1000;
	Sys_PataCfg.bleCfg.connectOff = 5000;

	Sys_PataCfg.bleCfg.txPower = 4;
	
	Sys_PataCfg.usedpage = 0x03;

	Sys_PataCfg.StartTime = 0;
	Sys_PataCfg.touchSensitive = 3;
	Sys_PataCfg.Headadd = 0;
	Sys_PataCfg.HeadMac = 0;
	Sys_PataCfg.UploadRecord=1;

	Sys_PataCfg.CfgMode = 0x0B;     // 1011

	memset (&Sys_PataCfg.CfgLog,0,sizeof(Sys_PataCfg.CfgLog)); 
	
	Access_EraseAllKey(KEY_TYPE_ALL);
	Access_EraseRecordData();
	memset(&LockKeyStatistics,0,sizeof(LockKeyStatistics));
	Sys_PataCfg.State =WORK_NORMAL;
}

/****************************************************************************************************
**Function:
	void Ekey_StorePara(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Sys_StorePara(void)
{
	Flash_ComType eepdata;

	Sys_PataCfg.KeyWord = 0x12345678;
	eepdata.pData = (uint8_t *)&Sys_PataCfg;
	eepdata.Lenth = sizeof(Sys_PataCfg);
	eepdata.Add = ACCESS_SYS_PARA_OFFSET;
	Flash_IoCtl(FLASH_ERASE_SECT,&eepdata.Add);
	Flash_Write(&eepdata);
}

/****************************************************************************************************
**Function:
	void Sys_LoadPara(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Sys_LoadPara(void)
{
		Flash_ComType eepdata;
		uint8 i;
		
		eepdata.pData = (uint8_t *)&Sys_PataCfg;
		eepdata.Lenth = sizeof(Sys_PataCfg);
		eepdata.Add = ACCESS_SYS_PARA_OFFSET;
		
		Flash_Read(&eepdata);
	
		for(i=0; i<3; i++)
		{
			Flash_Read(&eepdata);
			if(Sys_PataCfg.KeyWord!= 0x12345678)
			{
				Sys_GenFunDelayMs(500);
				continue;
			}
			else
			{
				break;
			}
		}
		if(i>=3)
		{
			Sys_ParainitFirst();
			Sys_StorePara();
			Sys_McuRestFlag=STD_TRUE;
		}

		//Sys_PataCfg.SysId = CUSTOM_ID;
		//memcpy(&Sys_PataCfg.Aeskey, Aes_KeyTable, 16);
}


/****************************************************************************************************
**Function:
void Sys_RestProc(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Sys_RestProc(void)           //  100ms
{
	if(Sys_StoreTimsoutx64ms)
	{
		Sys_StoreTimsoutx64ms--;
		if(Sys_StoreTimsoutx64ms == 0)
		{
			Sys_StorePara();
		}
	}
	if(Sys_RstTimsoutx64ms)
	{
		Sys_RstTimsoutx64ms--;
		if(Sys_RstTimsoutx64ms == 0)
		{
			//NVIC_SystemReset();
		}
	}
	if(Sys_StoreFlag != STD_FALSE)
	{
		Sys_StoreFlag=STD_FALSE;
		Sys_StoreTimsoutx64ms = 2;
	}
	if(Sys_McuRestFlag != STD_FALSE)
	{
		Sys_McuRestFlag=STD_FALSE;
		Sys_RstTimsoutx64ms = 400;
	}

	if ( SendDelayFlag )
		{
		  SendDelayFlag = 0;
			LockPadMsgSendBle();
		}

	if ( 1 == NbLogStatus )
		{
		  NbLogStatus = 2;
		  NBheartbeatFlag = 0;
			NB_LogREG( 10 );
		}

	if ( NBheartbeatFlag )
		{
		  NBheartbeatFlag = 0;
			if ( AccRcord.Wrecordindex >= 9 ) 	 
				{
					NB_LogREG( 10 );
				}
      else
      	{
      	  NB_LogREG( AccRcord.Wrecordindex );
      	}
		}

	if ( HALL_Unlogflag )
		{
		  HALL_UnlogCnt--;
		  if ( HALL_UnlogCnt == 0 )
		  	{
					HALL_Unlogflag = 0;
		  	}
		}
}

/****************************************************************************************************
**Function:
	void Sys_StartUpCheck(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
#define SYS_AUTO_IDENCRY STD_TRUE

const uint16 Batt_VolPerTable[BATT_VOLPER_MAX][2]=
{
//8
{6100,100},
{4700,30},
{4100,5},
{4000,0},
};
/****************************************************************************************************
**Function:
	void Sys_GetBat(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
U8 adcFlag;
/****************************************************************************************************
**Function:
void Sys_Init(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Sys_Init(void)
{	
	Rtc_Init();
	Access_Init();
	Sys_LoadPara();
	NBstatus_LoadPara();
	//Access_EraseRecordData();
	
  //	Sys_Parainit();
  //  NBstatus = 0;
	//  Factory_Sub = CASE_IN;
	// Sys_PataCfg.CheckLR = 0xA5;
  // Ver = 11;
	adcFlag = 0;
  PwrStopStatus = 0;
  ReadIMEIFlag = 1;
	NBconectFlag = 0;
	CodeOtherType = LockRebootType;
	NB_otherMsgLoad( );
	GPIO_SET_A3(0);

	
}


U8 	CutFlag;
void CutPinProc(void)
{
  if ( 0x01 == CutPin() && 0 == CutFlag )
  	{
      CutFlag = 1;
		  CodeOtherType = CutType;
			NB_otherMsgLoad( );
  	  Line_1A_WT588S( 0x0d ); 
  	}
}

uint8_t  keyScan;
uint8_t  keyData;
uint8_t  keyCopy;

uint16_t keyTime;
uint8_t  keyLast;

uint8_t  keyCode;
uint8_t  keyScan;

#define INCEX(X)     if(++X==0)X--//(X=((X)+1>(X))?(X)+1:(X))

#define  C_KS_SHORT  0X00
#define  C_KS_1SEC   0X20

#define  C_KC_UNDEF  0X1F
#define  C_KC_NONE   0X00

#define  C_KC_01     1
#define  C_KC_02     2

#define  C_SC_UNDEF  0XFF
#define  C_SC_NONE   0X00
#define  C_SC_01     0x01  
#define  C_SC_02     0x02

#define  C_KT_REPEAT  20
#define  C_KT_SHORT  ( C_KT_REPEAT + 6 )
#define  C_KT_5SEC   ( C_KT_REPEAT + 250 )

#define C_KS_SHORT   0X00
#define C_KS_1SEC    0X20
#define C_KS_2SEC    0X40
#define C_KS_3SEC    0X60
#define C_KS_5SEC    0X80
#define C_KS_DEALED  0XE0


void KeyScanning( void )
  {
     static uint8_t  keyScanBuf[ 3 ];
     
     keyScan = 0;

     if (0x00 == ReFactoryKey()) 
       {
         keyScan |= C_SC_01;
       }

     if (0x00 ==  DoorSensorKey())  
			 {
			 		keyScan |= C_SC_02;
			 }
     /* Following code for Filtering scanning data. */
     keyScanBuf[ 2 ] = keyScanBuf[ 1 ];
     keyScanBuf[ 1 ] = keyScanBuf[ 0 ];
     keyScanBuf[ 0 ] = keyScan;
     keyScan = ( keyScanBuf[ 0 ] & keyScanBuf[ 1 ] )
                  | ( keyScanBuf[ 1 ] & keyScanBuf[ 2 ] )
                  | ( keyScanBuf[ 2 ] & keyScanBuf[ 0 ] );
  }


void KeyGenerate( void )
  {  	 
    if ( keyData != keyScan )
      {
        keyData = keyScan;
        keyTime = C_KT_REPEAT;
      }
    else
      {
        INCEX( keyTime );
        switch ( keyTime )
          {
            case C_KT_SHORT:
                keyLast = keyCopy;
                switch( keyData )
                  {
                    case C_SC_NONE:     keyCode = C_KC_NONE; break;
                    case C_SC_01:       keyCode = C_KC_01;   break;      
                    case C_SC_02:       keyCode = C_KC_02;   break;
                    default:  keyCode = C_KC_UNDEF; break;
                  }
                keyCopy = keyCode;
                break;  

            case C_KT_5SEC:
                keyCode = ( keyCode & 0x1f ) | C_KS_5SEC;
                keyCopy = keyCode;
                break;
            default:
                break;
          }
      } 
    }  

void KeyHandler( void ) 
  {
    U8  key;

    if ( C_KS_DEALED != ( keyCode & 0xe0 ) ) 
      {
        key = keyCode;
        keyCode |= C_KS_DEALED;  

	 	switch ( key )
	      {
					case C_KC_01|C_KS_5SEC:     //   C_KC_01   TEST 
						if ( Sys_EnableReFactory())
							{
								PwrStopCnt = 5;
								Line_1A_WT588S( 0x2d ); 	 //  factroy reset
								Sys_Parainit();
								Sys_StorePara();
								ResetMsgSendBle();
							}
						else
							{
								Line_1A_WT588S( 0x0E ); 	 // ding dong
							}
					break;

				
	       case C_KC_02|C_KS_5SEC:      //    C_KC_02   TEST
						if ( Sys_PataCfg.CfgFlag&DISIBLEDOORLOCK )
							{
								Sys_PataCfg.CfgFlag &= ~DISIBLEDOORLOCK;
							}
						else
							{
								Sys_PataCfg.CfgFlag |= DISIBLEDOORLOCK;
							}
						Sys_StoreFlag = STD_TRUE;
						Line_1A_WT588S( 0x0E ); 	 // ding dong
	 	     break;
				 
	      case C_KC_NONE: 

	 	   	 break;
	 	  default:
	 	   	 break;
	 	 }
    }
  }

void KeyPress( void )        
  {
    KeyScanning( );
    KeyGenerate( );
    KeyHandler( );
  }

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

