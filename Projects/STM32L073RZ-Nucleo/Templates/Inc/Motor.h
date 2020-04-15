
#ifndef MOTOR_H
#define MOTOR_H
#include "main.h"

/******************************************************************************
**********************Macro definition*******************************************
******************************************************************************/
#define MOTOR_CHECK		STD_FALSE


//#define Vdd_PowerOn()   GPIO_SET_C7(1)     

#define LOCK_M_ADD_PIN_SET()     GPIO_SET_C5(1)     // m+
#define LOCK_M_ADD_PIN_CLEAR()   GPIO_SET_C5(0)     

#define LOCK_M_MINUS_PIN_SET()    GPIO_SET_B0(1)    // M-  
#define LOCK_M_MINUS_PIN_CLEAR()  GPIO_SET_B0(0) 

#define COM_COLHight()		GPIO_SET_B9(1)          // hall vdd
#define COM_COLLow()		  GPIO_SET_B9(0)  

#define DoorSwGetData()     HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)     // 门磁

#define ReversalSwGetData() HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9)     // 行程开关

//#define ComCol1GetData()     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)      // 反右  逆时针终点  左开门
//#define ComCol2GetData()     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)     /* 垂直检测开关*/
//#define ComCol3GetData()     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)     // 反左 顺时针终点   右开门

#define ComCol1GetData()     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)      // 反右  逆时针终点  左开门
#define ComCol2GetData()     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)      /* 垂直检测开关*/
#define ComCol3GetData()     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)       // 反左 顺时针终点   右开门

/******************************************************************************
**********************Type statement*******************************************
******************************************************************************/	
typedef enum
{
	MOTOR_CMD_SCAN = 0,
	MOTOR_CMD_OPEN,
	MOTOR_CMD_CLOSE,
	MOTOR_CMD_AUTO_CLOSE,/* 门磁检测关门*/
	MOTOR_CMD_SCAN_DOOR, /* 扫描门磁*/
	MOTOR_CMD_MAX
}Motor_CmdType;

typedef enum
{
	MOTOR_STAT_IDLE = 0,
	MOTOR_STAT_LOCK,
	MOTOR_STAT_UNLOCK,
       MOTOR_STAT_WAITOPENDOOR, /* 等待门打开*/
}Motor_StateType;

typedef enum
{
	MOTOR_DOORLOCK_IDLE = 0,
	MOTOR_DOORLOCK_OPENLOCK,
	MOTOR_DOORLOCK_OPEN,
}Motor_DoorLockType;

typedef enum
{
	MOTOR_POSIT_LOCK,
	MOTOR_POSIT_MIDDLE,
	MOTOR_POSIT_UNLOCK,
}Motor_PositType;

#define MOTOR_STATE_ON          2
#define MOTOR_STATE_OFF         2
#define MOTOR_STATE_MAX         4
#define MOTOR_STATE_MIN         0

/******************************************************************************
**********************Variable definition*******************************************
******************************************************************************/
extern Motor_StateType Motor_State;
extern uint8 Motor_DoorSw;
extern Motor_PositType Motor_PositStatus;
extern Motor_DoorLockType Motor_DoorLockSt;
extern uint16 Motor_AutoLockTimer64ms;
extern uint8 AutoCheckStatus;      
extern uint8 HALL_Unlogflag;
extern uint8 HALL_UnlogCnt;
extern uint8 DoorSw_Unlogflag;

extern U8 LockErrorFlag;
extern uint8 Motor_SubState;

extern const Dr_OpertonType MotorDrive;

#define DoorIsClose()    (Motor_DoorSw > 2/*MOTOR_STATE_ON*/)
#define DoorIsOpen()     (Motor_DoorSw < 1/*MOTOR_STATE_ON*/)
/******************************************************************************
**********************Function declared*******************************************
******************************************************************************/
extern void Motor_Irq_Init(void);
extern void Motor_Proc(void);
extern void Access_MotorStop(void);

extern void Access_MotorWorkOn(uint8 direction);
extern void Motor_AutoCheckLR(void);

#endif	
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


