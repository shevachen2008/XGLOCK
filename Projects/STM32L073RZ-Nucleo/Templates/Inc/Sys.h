

#ifndef SYS_H_
#define SYS_H_

#include "main.h"

/******************************************************************************
**********************Macro definitiont*******************************************
******************************************************************************/
#define      CUSTOM_RENTLY                                0xff8a  /*  Rently*/
#define      CUSTOM_ID   CUSTOM_RENTLY


#define      BATT_VOLPER_MAX	4

#define				SYSID						0xffff
#define				CARLOCKID					10

#define             PUBLIC_BUID_MAX                   64  
#define             BUILD_MAX                               10
#define 		   UPDATE_CHECKSUM				0x55667788


#define				SYSBIGFLG						  0x01
#define				SYSCARD3BYTE					0x02
#define				DETCARDSLEEP					0x04   /* 卡片休眠检测*/
#define				AUTOLOCK					    0x08   /* 自动上锁*/
#define				BTSAVEMODE				    0x10   /* 蓝牙触摸广播*/
#define       DISIBLEDOORLOCK       0x20   /* 门磁检测*/
#define       DISIBLEBLUETOOTH      0x40


#define      VOICEMODE       0x01
#define      PASSMODE        0x02
#define      NBOPENMODE      0x04
#define      REFACTORYMODE   0x08


#define WKUP_BLE  			   	0                                         
#define WKUP_KEY_INT               	1                                         
#define WKUP_LORA_INT            	2                                         
#define WKUP_LORA_TIME            	3                                         
#define SYS_MAXID					48
#define SYS_MINID					20

#define Sys_CheckCardSleepDet()      (Sys_PataCfg.CfgFlag&DETCARDSLEEP)
#define Sys_CheckBTSleepMode()      (Sys_PataCfg.CfgFlag&BTSAVEMODE)
#define Sys_CheckAutoLock()             (Sys_PataCfg.CfgFlag&AUTOLOCK)
#define Sys_CheckDisableAutoLock() ((Sys_PataCfg.CfgFlag&AUTOLOCK) == 0)
#define Sys_CheckDisableDoorLock() (Sys_PataCfg.CfgFlag&DISIBLEDOORLOCK)
#define Sys_CheckEnableDoorLock()  ((Sys_PataCfg.CfgFlag&DISIBLEDOORLOCK) == 0)
#define Sys_EnableBlueTooth()      ((Sys_PataCfg.CfgFlag&(uint8)DISIBLEBLUETOOTH) == 0)
#define Sys_DisableBlueTooth()      ((Sys_PataCfg.CfgFlag&(uint8)DISIBLEBLUETOOTH))

#define Sys_EnableVoice()         ((Sys_PataCfg.CfgMode&(uint8)VOICEMODE))
#define Sys_DisableVoice()        ((Sys_PataCfg.CfgMode&(uint8)VOICEMODE)== 0)

#define Sys_DisblePass()          ((Sys_PataCfg.CfgMode&(uint8)PASSMODE))
#define Sys_EnblePass()          ((Sys_PataCfg.CfgMode&(uint8)PASSMODE)== 0)

#define Sys_EnableNB()            ((Sys_PataCfg.CfgMode&(uint8)NBOPENMODE))
#define Sys_DisbleNB()            ((Sys_PataCfg.CfgMode&(uint8)NBOPENMODE)== 0)

#define Sys_EnableReFactory()     ((Sys_PataCfg.CfgMode&(uint8)REFACTORYMODE))
#define Sys_DisbleReFactory()     ((Sys_PataCfg.CfgMode&(uint8)REFACTORYMODE)== 0)


#define      LOGBLE          0x01
#define      LOGNB           0x02
#define      LOGPSW          0x04
#define      LOGAUTO         0x08
#define      LOGKEYPAD       0x10
#define      LOGCONSENSOR    0x20
#define      LOGHALL         0x40
#define      LOGLOWBAT       0x80

#define   Log_EnableBLE()           ((Sys_PataCfg.CfgLog[0]&(uint8)LOGBLE))
#define   Log_EnableNB()            ((Sys_PataCfg.CfgLog[0]&(uint8)LOGNB))
#define   Log_EnablePSW()           ((Sys_PataCfg.CfgLog[0]&(uint8)LOGPSW))
#define   Log_EnableAUTO()          ((Sys_PataCfg.CfgLog[0]&(uint8)LOGAUTO))
#define   Log_EnableKEYPAD()        ((Sys_PataCfg.CfgLog[0]&(uint8)LOGKEYPAD))
#define   Log_EnableCONSENSOR()     ((Sys_PataCfg.CfgLog[0]&(uint8)LOGCONSENSOR))
#define   Log_EnableHALL()     			((Sys_PataCfg.CfgLog[0]&(uint8)LOGHALL))
#define   Log_EnableLOWBAT()     		((Sys_PataCfg.CfgLog[0]&(uint8)LOGLOWBAT))

#define ReFactoryKey()      HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)
#define DoorSensorKey()  HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)
#define CutPin()            HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8)



/******************************************************************************
**********************Type statement*******************************************
******************************************************************************/
typedef enum 
{
	SYS_STARTUP 		= 0,
	SYS_NORMAL,
	SYS_SLEEP,
}Sys_StateType ;

typedef struct {
	uint16 	SerSocketPort;		// 远程端口
	uint32  remoteCommIp;		//服务器  IP
	char 	TargetDnsName[40];	// 域名
	char 	ApnCh[20];
}TcpIpPataType;//运行模式

typedef struct {
	uint32	remoteCommIp;		//服务器  IP
	char 	TargetDnsName[40];
	uint32	CheckSum;
}FtpInfoType;

typedef struct {
	uint16	connectOff;		//断开连接
	uint8       txPower;             // 发射功率
	uint16     advInt;                // 广播间隔
}BLEInfoType;

typedef struct {
	BLEInfoType  bleCfg;
	uint16   CustomId; /* 用于发卡器加密密码*/
	uint8 	AppSleepBread;
	uint8 	Headadd;
	uint8 	SyncIndex;
	uint8   usedpage;
	uint8 	EncpyPsd[8];
	uint8 	AarmEnable; /* 报警提示1:开启;  0:关闭*/
	uint16  randindex;
	uint16 	SysId;
	uint8 	Aeskey[16];
	
	uint32	HeadMac;
	uint32 	Pswd;
	uint32 	Mac;
	uint8 	MacBuf[6];
	
	uint16 	OpenLockTime; /* 开锁保持时间*/
	uint16     MotorTime; /* 电机驱动时间*/
	uint16 	HeartTime; /* 心跳帧时间* 16ms */
	uint8 	BuildId; /* 组ID  1 ~ 255 */    // 0 :公共门
	uint16 	PropertyId; /*酒店ID :0 ~ 65535 */
	uint16  HotelPswd;
	uint8 	CfgFlag; /*配置信息*/
	uint32  StartTime; // 动态密码卡片开始时间
	uint8   touchSensitive; // 触摸板灵敏度调节
	uint8   UploadRecord;
	uint8   State;
	uint8   CheckLR;
	uint8   AGTest;
	uint8   CfgMode;
	uint8   CfgLog[2];
	uint32  KeyWord;
}Sys_PataCfgType;

/******************************************************************************
**********************Variable definition******************************************
******************************************************************************/
extern Sys_PataCfgType Sys_PataCfg;
extern uint8 Sys_CheckUpFlag ;
extern uint8  Sys_McuRestFlag;
extern uint8  Sys_StoreFlag;
extern uint16 Sys_RstTimsoutx64ms;
extern uint16 Sys_StoreTimsoutx64ms;
extern uint32 Sys_Tick ;
extern uint8  Sys_WkupType;
extern uint8  Sys_ResetDevice;
extern Sys_StateType Sys_State;
extern uint8  Sys_frstPoweron;
extern const uint16 Batt_VolPerTable[BATT_VOLPER_MAX][2];
extern uint16 Sys_SleepErrTimsoutx64ms ;
extern uint8  adcFlag;
extern uint8  Ver;

/******************************************************************************
**********************Function declared*******************************************
******************************************************************************/
extern void Sys_StorePara(void);
extern void Sys_LoadPara(void);
extern void Sys_Init(void);
extern void Sys_StateProc(void);
extern void Sys_Parainit(void);
extern void Sys_ParainitFirst(void);
extern void Sys_RestProc(void);
extern uint8 Sys_GetBat(void);
extern void Sys_wdt_feed(void);  
extern void Sys_wdt_start(void);  
#if (defined DEBUG_ENABLE) && (DEBUG_ENABLE == STD_TRUE)
extern void  Sys_Debug(uint8 mode);
#endif

extern void Sys_SpiOpen(void);
extern void Sys_SpiClose(void);
extern void CutPinProc(void);
extern void KeyPress( void );        

#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

