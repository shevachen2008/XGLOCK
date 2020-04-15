
#ifndef __ACCESS_H
#define __ACCESS_H
#include "main.h"

typedef struct
{
	uint8	IdType;
	uint8	*pid;
	uint8	*pdat;
} Card_ParmType;

#define ACCESS_CARDBUILDID_MAX    			4

#define ACCESS_RECORD_ONCE_MAX				14
#define ACCESS_RECORD_MAX				      80    //(512-1)
#define ACCESS_RECORD_RDMAX				    256

#define ACCESS_LEARN_RECORD_MAX				100
#define ACCESS_RECORD_SEND_ONCE_MAX		16


#define ACCESS_OPEN_LOCK_TPYE	           1
#define ACCESS_CLOSE_LOCK_TPYE	           0
#define ACCESS_NG_TPYE	                   0x02

#define FLASH_ONCEBLOCK_SIZE			     128*1024
#define ACCESS_SYNC_PASWD_MAX          10


#define ACCESS_SYS_PARA_OFFSET			    0x10000                // BLOCK 1 		
#define ACCESS_BLACK_LIST_OFFSET        0x20000                // BLOCK 2   USE 2 BLOCK   
#define ACCESS_SERVE_KEY_OFFSET         0x40000                // BLOCK 4   USE 2 BLOCK   

#define ACCESS_RECORD_OFFSET            0x80000 
#define NBSTATUS_OFFSET                 0xA0000 


/******************************************************************************
**********************Macro definition*******************************************
******************************************************************************/
enum
{
	WORK_NORMAL, /* ��*/
	WORK_NORMALLY_OPEN,   /* ����*/
	WORK_LEARN_CARD,
	WORK_TEST,
};

enum
{
	TYPE_LOCK_HOME = 0, /* ��������*/
	TYPE_LOCK_FLAT, /* ��Ԣ��*/ 
	TYPE_LOCK_CAR, /* ��λ��*/
	TYPE_KEY, /* ��������Կ��*/
	TYPE_LOCK_HANDUP, /* ���ӹ���*/
	TYPE_LOCK_ACCESS , /* �Ž�*/
	TYPE_LOCK_ACCESS_WIG , /* �Ž�Τ�����*/
	TYPE_LOCK_ACCESS_REMOTE, /* Զ�̿����Ž�*/
	TYPE_LOCK_VIDEO, /* ���ӶԽ��Ž�*/
	TYPE_LOCK_BIKE, /* Զ�̿����Ž�*/
	TYPE_LOCK_BIKE_REMOTE, /* Զ�̿����Ž�*/
	TYPE_LOCK_HOTEL_REMOTE = 11,  /* Զ�̿��ƾƵ깫Ԣ����*/
	TYPE_LOCK_HOTEL = 12,  /* �Ƶ깫Ԣ����*/
	TYPE_LOCK_FIGNERPRINT , /* ָ����*/
	TyPE_LOCK_LORA_GATEWAY = 16, /* Lora����*/
	TyPE_LOCK_SEND_CARD = 31, /* ������*/
	TYPE_LAB = 0x20, /* �������ӱ�ǩ*/
};


enum
{
	KEY_TYPE_ALL = 0,
	KEY_TYPE_APP,
	KEY_TYPE_PASSWORD,
	KEY_TYPE_CARD,
	KEY_TYPE_57CARD = KEY_TYPE_CARD,
	KEY_TYPE_FINGERPRINT,
	KEY_TYPE_IDENTITY_CARD,
	KEY_TYPE_QRCODE , // ��ά��͸��
	KEY_TYPE_USERID,
	KEY_TYPE_MAX,
	KEY_TYPE_CLEAR_ALL=KEY_TYPE_MAX,//= 10, ������е�Կ��
	
	KEY_TYPE_TIME_ONE, /*6λ�����ÿ�����*/
	KEY_TYPE_TIME_INDEX, /*6λ�������ÿ�����*/
	KEY_TYPE_TIME_SLOT, /*6λʱ��ηÿ�����*/
	KEY_TYPE_TIME_DATE,  /*8λ��������*/
	KEY_TYPE_MECHANICAL_KEY = 13,  /* ��еԿ�׿���*/
	
	RECORD_EMERGENCY_CARD = 15, /* Ӧ����*/
	RECORD_TOTAL_CARD = 16, /* �ܿ�*/
	RECORD_STAFF_CARD = 17, /* Ա������ʱ*/
	RECORD_STAFF_LOOP_CARD = 18, /* Ա��ѭ��*/
	RECORD_CUSTOM_CARD = 19, /* ���˿�*/
    RECORD_INSPECTION_CARD = 20, /* Ѳ�쿨*/

	KEY_TYPE_ONE_TIME = 21, // һ��������
	KEY_TYPE_FOREVER = 22, // ��������
	KEY_TYPE_TIME_SLOT_7 = 23,   //7λ ʱ���
	KEY_TYPE_CLEAR = 24, // �������
	KEY_TYPE_TIME_SLOT_8 = 25,   //8λ ʱ���

	ACCESS_INFO_LOCK_STATUS = 28,  /*  ��״̬��Ϣ����:  act:   bit0:��״̬(1:�� 0:��) bit1:����(1:����)*/
	ACCESS_FALSE_LOCK_ALARM = 29,  /*  ��������*/
	ACCESS_LEARN_IDENTITY_CARD_TPYE = 30, /* ���֤*/
	ACCESS_LEARN_CARD_TPYE = 31,
	
	KEY_TYPE_FAST_APP_NOID_OPEN = 0xf1,  //APP���ٿ���
	KEY_TYPE_FAST_PSWD_OPEN,//�ÿ�����ٿ���
	KEY_TYPE_FAST_CARD_OPEN,//��Ƭ���ٿ���
	KEY_TYPE_FAST_QRCODE_OPEN,//��ά����ٿ���
};

enum
{
	ACCESS_DET_WAIT = 0,
	ACCESS_DET_M1,
	ACCESS_DET_T5557ST,
	ACCESS_DET_T5557STOP,
};

enum
{
	CARD_TYPE_STAFF_LOOP = 0,  /* Ա��������ʱ���:ÿ��*/
	CARD_TYPE_STAFF,  /* Ա���� ��Ч��*/
	CARD_TYPE_LOSS, /* ��ʧ��*/
	CARD_TYPE_EMERGENCY, /* Ӧ���� */
	CARD_TYPE_ALL, /* �ܿ�*/
	CARD_TYPE_INSPECTION, /* Ѳ�쿨*/
	CARD_TYPE_TESK, /* ���Կ� */
	CARD_TYPE_GUEST, /* ���˿�*/
	CARD_TYPE_NULL, /* ��Ч��*/
};


enum
{
	ACCESS_AUTO_TEST_IDLE= 0,
	ACCESS_AUTO_TEST_CLOCK,
	ACCESS_AUTO_TEST_BLE,
	ACCESS_AUTO_TEST_IC_ID_IDENTITY,	
	ACCESS_AUTO_TEST_KEYTABLE,
	ACCESS_AUTO_TEST_WIFI,
	ACCESS_AUTO_TEST_GPRS,
};

#define LOCK_HOME   0
#define LOCK_FLAT    1
#define LOCK_HOTEL_FLAT    2
#define LOCK_HOTEL_FLAT_REMOTE    3

#if (defined LORA_ENABLE) && (LORA_ENABLE == STD_TRUE)	
#define LOCK_TYPE   LOCK_HOTEL_FLAT_REMOTE
#else
#define LOCK_TYPE   LOCK_HOTEL_FLAT
#endif


#define LOCK_TOUCH_REMOTE_OPEN_TIME    20000/64

//#define ACCESS_FASTCMPMAX	1024
//#define ACCESS_FASTCMPMIN	512
#define OPEN_REVERSE_DOOR			0x01   /* ���Կ�������*/
#define OPEN_COMMOM_DOOR			0x02   /* ���Թ�����*/
#define LOSS_OLD_CARD			    0x03   /* ��ʧ��ǰ��*/

/******************************************************************************
**********************Type statement*******************************************
******************************************************************************/
typedef struct {
	uint8 WegeMode; /* 0: 34; 1:26 */
	uint8 ErrorTimes;
	//uint8 State;
	uint8 DefaultTimes;
	uint8 UserIndex;
}Access_ParaType;	

typedef struct {
	uint16 UserIndex; 
	uint16 FlashIndex;
}Access_IdIndxType;	

typedef struct{
	uint8 keyId;//[ACCESS_SYNC_PASWD_MAX];
	uint8 KeyIndex;
	//uint8 KeyMax;
	//uint8 Year;
	//uint8 Month;
	//uint8 Day;
}Access_SyncPaswdType;

typedef struct {
	uint8 	FlashInitFlag;
}Access_flahInfoType;

typedef struct {
	uint8  cardtype;
	uint16 hotelId;
	uint32 deviceId;
	uint32 devicePaswd;
	uint16 HotelPaswd;
	uint32 Timest;
	uint32 Timeend;
	//uint8 build[ACCESS_CARDBUILDID_MAX];
}Access_CardDatType;

typedef struct {
	uint8 TypeResult;//��������| �������
	uint32 Id;//Կ��ID
	uint32 Time;//����ʱ��
}AccRcordDataType;

typedef struct {
	uint16 Wrecordindex;//д��¼����0~
	uint16 SNBrecordindex;//NB���ͼ�¼���� 0~
	uint16 Srecordindex;//���ͼ�¼���� 0~
	uint16 recordnum;//�ܼ�¼����
	uint32 TimerBk;//ʱ��
	uint32 CustomTimerBk; /* ���˿��Ĺ�ʧʱ��*/
	uint32 StaffTimerBk; /* Ա�����Ĺ�ʧʱ��*/
	uint32 AllTimerBk; /* �ܿ��Ĺ�ʧʱ��*/
	uint32 EmergencyTimerBk; /* Ӧ�����Ĺ�ʧʱ��*/
	uint32 AppTimerBk; /* APP����ʱ��*/
	AccRcordDataType RecordList[ACCESS_RECORD_MAX+1];
	uint32 KeyWord;
}AccRcordParaType;

extern U8 checkLockFlag;
extern U8 checkUnLockFlag;

/******************************************************************************
**********************Variable definition*******************************************
******************************************************************************/
extern Access_ParaType Access_Globle;
extern uint16 Access_ResetDeviceTimer64;
extern uint16 Access_OpenRedIndex ;
extern uint16 Access_OpenWriteaIndex ;
extern uint16 Access_Store_Recordx64ms;
extern uint8 protoAnaly_netCommType;

extern uint8 Access_MotorTimer64;
extern uint16 Access_LockTimer64;
extern uint16 Access_DefaultTimer64;
extern uint16 Access_CardLearnTimer64;
extern uint8 Access_BatteryData;
extern AccRcordParaType AccRcord;
extern uint32 Access_UserId_Temp;
extern uint16 Access_LockDeviceTimer64ms;
extern uint8 Access_LockStatus;
/****************************************************************************/
/* Function                                                                 */
/****************************************************************************/
extern void Access_Init(void);
extern void Access_Unlock(void);
extern void Access_Lock(void);
extern void Access_OpenError(void);

extern Std_ReturnType Access_ComTime(uint32 startTime, uint32 endTime);
//extern void Access_LearnCardOk(uint8 idtpye,uint8*cardid);
extern void Access_CardProcess(uint8 idtpye, uint8* pUid,Access_CardDatType *CardDat);

extern void Access_FlashArrang(void);
extern void Access_TimerProc(void);

extern void Access_EraseAllKey(uint8 idtpye);
extern void Access_DefaultPaswd(void);
extern void Access_SendRecordProc(void);
extern void Access_WriteRecordFlash(uint8* pKeyId,uint32 time ,uint8 type, uint8 action);
//extern void Access_MotorOpen(void);
//extern void Access_MotorClose(void);
//extern void Access_MotorStop(void);
extern void Vdd_PowerOff(void);

extern uint8 Access_CardDatProc(Card_ParmType *pdat, Access_CardDatType *pAccess_CardDat);
extern uint8  Access_GetRecord(uint16 index,uint8 *poutdat);
#define Access_DelayReset(x)   Access_ResetDeviceTimer64 = x/64
extern void Access_EraseRecordData(void);

extern void Access_DetCardProc(void);

extern Std_ReturnType Access_ComTimeLoop(uint32 startTime, uint32 endTime);

#endif	
/************************ (C) COPYRIGHT DaHao electronics *****END OF FILE****/

