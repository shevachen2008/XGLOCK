/****************************************************************************************
�ļ�:   ComNB.h
˵��:
****************************************************************************************/
#ifndef _COMNB_H__   //���ذ���.
#define _COMNB_H__
/***************************************************************************************/
#if 1

#define ContactSensorClosedType   16  //�ŴŹ���״̬
#define ContactSensorOpenedType   17  //�Ŵſ���״̬

#define ContactSensorEnableType     18   //�Ŵ�ʹ��
#define ContactSensorDisabledType   19   //�Ŵ�ʧ��

#define SecurityEnableType     20      // ��ȫ��ťʹ��
#define SecurityDisabledType   25      //��ȫ��ťʧ��

#define LockRebootType  26            //����        
#define LowBatteryType  27            //�͵���

#define LockHallSwitch_CheckON   28    //�������ť,Ť��,���ദ������ȥ   UNLOCK
#define LockHallSwitch_CheckOF   29    //�������ť,Ť��,���ദ�������   LOCK


#define KeyPadLockType  10        //  ���̰���,���ż�����
#define AutoLockType    12        //  �Զ����� 
#define BLELockType     6        //  �������� 
#define NBLockType      7        //  NB���� 

#define PSWUnLockType   2        //  ���뿪��
#define FOBUnLockType   3        //  ������
#define BLEUnLockType   4        //  ��������
#define NBUnLockType    5        //  NB����

#define CutType         11       //  ����     


extern uint8_t REGStatus; 
extern uint8_t CodeType;
extern uint8_t CodeOtherType;

extern uint8_t Com_getNbBuf[50];

extern uint32_t KeyIdSend;
extern uint32_t CardIdSend;

extern uint8_t NbLogStatus;
extern uint8_t NBheartbeatFlag;
extern uint16_t NBHeartBeatCnt;



extern U8 LogREGindex;

extern void NB_REG(U8 Type, U32 psw,U8 result);
//extern void NB_LockMsgSend(void);
//extern void NB_UnLockMsgSend(void);
//extern void NB_otherMsgSend(void);
extern void NB_InitRegMsg(void);
extern void NB_msgSend(void);
extern void NB_LogREG( U8 len );
extern void NB_LogSend(void);
extern void parseServerAckPackage(void);

extern void NB_LockMsgLoad(void);
extern void NB_UnLockMsgLoad(void);
extern void NB_otherMsgLoad(void);
//extern void NB_LogREG1(U8 i);
//extern void NB_LogSend1(U8 i);

#endif

/***************************************************************************************/
#endif
/****************************************************************************************
�ļ�����
****************************************************************************************/

