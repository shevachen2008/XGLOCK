
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

#define DoorSwGetData()     HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)     // �Ŵ�

#define ReversalSwGetData() HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9)     // �г̿���

//#define ComCol1GetData()     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)      // ����  ��ʱ���յ�  ����
//#define ComCol2GetData()     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)     /* ��ֱ��⿪��*/
//#define ComCol3GetData()     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)     // ���� ˳ʱ���յ�   �ҿ���

#define ComCol1GetData()     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)      // ����  ��ʱ���յ�  ����
#define ComCol2GetData()     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)      /* ��ֱ��⿪��*/
#define ComCol3GetData()     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)       // ���� ˳ʱ���յ�   �ҿ���

/******************************************************************************
**********************Type statement*******************************************
******************************************************************************/	
typedef enum
{
	MOTOR_CMD_SCAN = 0,
	MOTOR_CMD_OPEN,
	MOTOR_CMD_CLOSE,
	MOTOR_CMD_AUTO_CLOSE,/* �Ŵż�����*/
	MOTOR_CMD_SCAN_DOOR, /* ɨ���Ŵ�*/
	MOTOR_CMD_MAX
}Motor_CmdType;

typedef enum
{
	MOTOR_STAT_IDLE = 0,
	MOTOR_STAT_LOCK,
	MOTOR_STAT_UNLOCK,
       MOTOR_STAT_WAITOPENDOOR, /* �ȴ��Ŵ�*/
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


