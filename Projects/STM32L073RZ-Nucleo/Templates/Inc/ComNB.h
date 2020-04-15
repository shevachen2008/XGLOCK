/****************************************************************************************
文件:   ComNB.h
说明:
****************************************************************************************/
#ifndef _COMNB_H__   //防重包含.
#define _COMNB_H__
/***************************************************************************************/
#if 1

#define ContactSensorClosedType   16  //门磁关着状态
#define ContactSensorOpenedType   17  //门磁开着状态

#define ContactSensorEnableType     18   //门磁使能
#define ContactSensorDisabledType   19   //门磁失能

#define SecurityEnableType     20      // 安全按钮使能
#define SecurityDisabledType   25      //安全按钮失能

#define LockRebootType  26            //重启        
#define LowBatteryType  27            //低电量

#define LockHallSwitch_CheckON   28    //后面板旋钮,扭动,锁舌处于缩进去   UNLOCK
#define LockHallSwitch_CheckOF   29    //后面板旋钮,扭动,锁舌处于伸出来   LOCK


#define KeyPadLockType  10        //  键盘按键,锁门键锁门
#define AutoLockType    12        //  自动锁门 
#define BLELockType     6        //  蓝牙锁门 
#define NBLockType      7        //  NB锁门 

#define PSWUnLockType   2        //  密码开门
#define FOBUnLockType   3        //  卡开门
#define BLEUnLockType   4        //  蓝牙开门
#define NBUnLockType    5        //  NB开门

#define CutType         11       //  防撬     


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
文件结束
****************************************************************************************/

