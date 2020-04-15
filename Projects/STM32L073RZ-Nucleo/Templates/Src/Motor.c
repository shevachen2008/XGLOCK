#include "main.h"


Motor_StateType Motor_State = MOTOR_STAT_IDLE;
Motor_PositType Motor_PositStatus;
Motor_DoorLockType Motor_DoorLockSt;

uint8 Motor_SubState;
uint8 Motor_DetectSw=0xff;
uint16 Motor_Timer64ms;
uint16 Motor_AutoLockTimer64ms;   /* 自动关锁*/
uint8 Motor_SwLR;      /* 左右检测*/

uint8 AutoCheckStatus;      /* 自动左右检测*/

uint8 Motor_DoorSw;    /* 门磁检测开关*/

uint8 DoorSw_Status;    /* 门磁状态*/
uint8 HALL_Status;      /* HALL状态*/
uint8 HALL_Unlogflag;
uint8 HALL_UnlogCnt;
uint8 DoorSw_Unlogflag;


uint8 MotorLevelLeftSw;      /* 左开门水平检测开关*/
uint8 MotorLevelRightSw;     /* 右开门水平检测开关*/

uint8 MotorVerticalSw; /* 垂直检测开关*/
uint8 MotorBackSw;      /* 回位检测开关*/

#define CheckLevelLeftSwOn()   (MotorLevelLeftSw > MOTOR_STATE_ON)
#define CheckLevelRightSwOn()  (MotorLevelRightSw > MOTOR_STATE_ON)

#define ChecVerticalSwOn()     (MotorVerticalSw > MOTOR_STATE_ON)
#define ChecBackSwOn()         (MotorBackSw < MOTOR_STATE_OFF)

#define MOTOR_WORK_TIME         400 
#define MOTOR_AUTOCHECK_TIME    400 
unsigned char TravelSwitchLast=0;
unsigned char TravelSwitchNow=0;

 
/****************************************************************************************************
**Function:
   uint8 Motor_AutoCheckLR(void)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Motor_AutoCheckLR(void)
{
  
//	static uint8 CheckFlag;
	switch(Motor_SubState)
	{
		case CASE_IN:
		{
			if ( CheckLevelLeftSwOn() || ComCol1GetData()==0 )
				{
				  Sys_PataCfg.CheckLR = 0x5A;         //  左开门 
					Access_MotorWorkOn(1);	
					delay_ms(510);
					Access_MotorStop();				  
					Motor_SubState = CASE_OK;
				}
			else if( CheckLevelRightSwOn()||ComCol3GetData()==0)
				{
 				  Sys_PataCfg.CheckLR = 0xA5;          // 右开门 
					Access_MotorWorkOn(0);	
					delay_ms(510);
					Access_MotorStop();
					Motor_SubState = CASE_OK;
				}
			else 
				{
					 Access_MotorWorkOn(0);               // 电机逆时针 转盘顺时针  向右转 
					 Motor_SubState = CASE_01;
					 Motor_Timer64ms = MOTOR_AUTOCHECK_TIME;				
				}
			break;
		}
		case CASE_01:
		{
			if(Motor_Timer64ms == 0)
			{
				Access_MotorStop();
				delay_ms(10);
				Access_MotorWorkOn(1);                // 向左转
				Motor_SubState = CASE_02;
				Motor_Timer64ms = MOTOR_AUTOCHECK_TIME;  
				break;
			}

			if(CheckLevelRightSwOn()||ComCol3GetData()==0)
			{
				Access_MotorStop();
				Sys_PataCfg.CheckLR = 0xA5; 				 //  Right
				Motor_SubState = CASE_OK;
			}
			break;
		}
		case CASE_02:
			{
				if(Motor_Timer64ms == 0)
				{
					Access_MotorStop();
					Motor_SubState = CASE_EEROR;
					break;
				}

				if(CheckLevelLeftSwOn()||ComCol1GetData()==0)
				{
					Access_MotorStop();
					Sys_PataCfg.CheckLR = 0x5A; 			 // 		Left
					Motor_SubState = CASE_OK;
				}				
			 break;
			}

		case CASE_OK:
		{
      if ( 0xA5 == Sys_PataCfg.CheckLR )
      	{
      	  Access_MotorWorkOn(1);  
      	}
			else if ( 0x5A == Sys_PataCfg.CheckLR )
				{
					Access_MotorWorkOn(0);	
				}

      if( Access_BatteryData <= 55 )
      	{
					Motor_Timer64ms = 60;
      	}
			else
				{
				   	Motor_Timer64ms = 40;
				}
	
			Motor_SubState = CASE_END;
			break;
		}   
		case CASE_END:
		{	
			if(Motor_Timer64ms == 0)
			{
				Access_MotorStop();  
				
				Sys_StorePara();
				AutoCheckStatus = 2;
				if ( checkLockFlag )
					{
					  checkLockFlag = 0;
						Motor_AutoLockTimer64ms = 0;
						Motor_DoorLockSt = MOTOR_DOORLOCK_IDLE;
						MotorDrive.ioctl(MOTOR_CMD_CLOSE, NULL);
					}
				if ( checkUnLockFlag )
					{
					  checkUnLockFlag = 0;
						MotorDrive.ioctl(MOTOR_CMD_OPEN, NULL);
					}
			}			
		  break;
		}
    case CASE_EEROR:		
		{
			if(Motor_Timer64ms == 0) 
			{
				Access_MotorStop();  
				Line_1A_WT588S( 0x1E );      //fail 
        //delay_ms(500);
			  
				Motor_SubState = CASE_IN;
				AutoCheckStatus = 2;
			}	
		  break;
		}
			
		default:
			break;
	}	
}



/****************************************************************************************************
**Function:
   uint8 Motor_CheckLR(void)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint8 Motor_CheckLR(void)
{
	uint8 ret;
  if (0x5A == Sys_PataCfg.CheckLR )
  	{
  	  ret = 1;      /* Left */
  	}
	else if ( 0xA5 == Sys_PataCfg.CheckLR )
		{
		 ret = 0;       /* Right */
		}
	
//	ret = 0; //

	return ret;
}

/****************************************************************************************************
**Function:
	void Access_MotorOpen(void)
**Author: rory
**Description:
**Input: direction:1 顺时针  0:逆时针
**Output: 
****************************************************************************************************/
void Access_MotorWorkOn(uint8 direction)
{
//	Vdd_PowerOn();
	if(direction == 0)
	{
	//	LOCK_M_ADD_PIN_CLEAR();
	//  LOCK_M_MINUS_PIN_SET();

		LOCK_M_ADD_PIN_SET();
    LOCK_M_MINUS_PIN_CLEAR();
	}
	else
	{
	//	LOCK_M_ADD_PIN_SET();
  //  LOCK_M_MINUS_PIN_CLEAR();

			LOCK_M_ADD_PIN_CLEAR();
			LOCK_M_MINUS_PIN_SET();
	}
}

/****************************************************************************************************
**Function:
	void Access_MotorStop(void)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Access_MotorStop(void)
{
	LOCK_M_ADD_PIN_CLEAR();
	LOCK_M_MINUS_PIN_CLEAR();
}
/****************************************************************************************************
**Function:
   int8 Motor_Open(void *pData)
**Author:rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Motor_Open(void *pData)
{
	Motor_State = MOTOR_STAT_IDLE;
	Motor_DoorLockSt = MOTOR_DOORLOCK_IDLE;
	//COM_COLHight();
	Motor_SwLR = Motor_CheckLR();
}
	

/****************************************************************************************************
**Function:
    Std_ReturnType Motor_Write(uint8 Cmd,void *pData)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
Std_ReturnType Motor_Write(void *pData)
{
	
	return E_OK;
}

/****************************************************************************************************
**Function:
    Std_ReturnType Motor_Write(uint8 Cmd,void *pData)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
Std_ReturnType Motor_Read(void *pData)
{
	if(Motor_State == MOTOR_STAT_IDLE)
	{
		return E_OK;
	}
	else
	{
		return E_NOT_OK;
	}
}

/****************************************************************************************************
**Function:
    Std_ReturnType Motor_Write(uint8 Cmd,void *pData)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Motor_DetectPosit(void)
{
	if(ComCol1GetData() == 0)
	{
		 if(MotorLevelLeftSw < MOTOR_STATE_MAX)
		 {
			   MotorLevelLeftSw++;
		 }
	}
	else
	{
		 if(MotorLevelLeftSw > MOTOR_STATE_MIN)
		 {
			  MotorLevelLeftSw--;
		 }
	}
	
	if(ComCol2GetData() == 0)
	{
		  if(MotorVerticalSw<MOTOR_STATE_MAX)
		  {
			   MotorVerticalSw++;
		  }
	}
	else
	{
		  if(MotorVerticalSw>MOTOR_STATE_MIN)
		  {
			   MotorVerticalSw--;
		  }
	}

	if(ComCol3GetData() == 0)
	{
		 if(MotorLevelRightSw < MOTOR_STATE_MAX)
		 {
			   MotorLevelRightSw++;
		 }
	}
	else
	{
		 if(MotorLevelRightSw > MOTOR_STATE_MIN)
		 {
			  MotorLevelRightSw--;
		 }
	}	
	
	if(ReversalSwGetData() == 0)
	{
		if(MotorBackSw<MOTOR_STATE_MAX)
		{
			MotorBackSw++;
		}
	}
	else
	{
		if(MotorBackSw>MOTOR_STATE_MIN)
		{
			MotorBackSw--;
		}
	}
	if(Motor_SwLR == 1)
	{
		if(CheckLevelLeftSwOn())
		{
			 Motor_PositStatus = MOTOR_POSIT_LOCK;
		}
		else if(ChecVerticalSwOn())
		{
			Motor_PositStatus = MOTOR_POSIT_UNLOCK;
			if(Sys_CheckEnableDoorLock())
			{
				 if(Motor_DoorLockSt == MOTOR_DOORLOCK_IDLE)
				 {
						 Motor_DoorLockSt = MOTOR_DOORLOCK_OPENLOCK;
				 }
			}
		}
		else
		{
			Motor_PositStatus = MOTOR_POSIT_MIDDLE;
		}
	}
	else
	{
		if(CheckLevelRightSwOn())
		{
			Motor_PositStatus = MOTOR_POSIT_LOCK;
		}
		else if(ChecVerticalSwOn())
		{
			Motor_PositStatus = MOTOR_POSIT_UNLOCK;
			if(Sys_CheckEnableDoorLock())
			{
				 if(Motor_DoorLockSt == MOTOR_DOORLOCK_IDLE)
				 {
						 Motor_DoorLockSt = MOTOR_DOORLOCK_OPENLOCK;
				 }
			}		
		}
		else
		{
			Motor_PositStatus = MOTOR_POSIT_MIDDLE;
		}
	}


	if ( HALL_Unlogflag )   return;
	
	if (Motor_SwLR == 1)
		{
		  if ( CheckLevelLeftSwOn())
		    {
			  	if ( 0 == HALL_Status )   HALL_Status = 1;    // lock 
					if ( 2 == HALL_Status )
						{
						  HALL_Status = 1;
					    HallSwitchMsgSendBle(0x02,0x01); 
							CodeOtherType = LockHallSwitch_CheckOF;
							NB_otherMsgLoad( );
						}
		  	}
			else if ( ChecVerticalSwOn())
				{
				    if ( 0 == HALL_Status ) 	HALL_Status = 2;    // unlock
						if ( 1 == HALL_Status ) 
							{
								HALL_Status = 2;
					      HallSwitchMsgSendBle(0x02,0x02); 
								CodeOtherType = LockHallSwitch_CheckON;
								NB_otherMsgLoad( );
							}
				}			
		}
	else
		{
			if ( CheckLevelRightSwOn())
				{
			  	if ( 0 == HALL_Status )   HALL_Status = 1;
					if ( 2 == HALL_Status )
						{
						  HALL_Status = 1;
						  HallSwitchMsgSendBle(0x02,0x01); 
							CodeOtherType = LockHallSwitch_CheckOF;
							NB_otherMsgLoad( );
						}				
				  
				}
			else if ( ChecVerticalSwOn())
				{
				    if ( 0 == HALL_Status ) 	HALL_Status = 2;
						if ( 1 == HALL_Status )
							{
								HALL_Status = 2;
					    	HallSwitchMsgSendBle(0x02,0x02); 
								CodeOtherType = LockHallSwitch_CheckON;
								NB_otherMsgLoad( );
							}				
				  
				}
		}
}

U8 LockErrorFlag;
void LockError(void)
{
	if ( LockErrorFlag )
		{
		  if ( CheckLevelRightSwOn())
		   	{
					LockErrorFlag = 0;
					Access_MotorWorkOn(1);
					delay_ms(500);
					Access_MotorStop();
		   	}
			else if ( CheckLevelLeftSwOn())
				{
					LockErrorFlag = 0;
					Access_MotorWorkOn(0);
					delay_ms(500);
					Access_MotorStop();				
				}
		}
}
U8 MotorDelayTime;
/****************************************************************************************************
**Function:
	 void Motor_LockProc(uint8 LR)
**Author: rory
**Description:
**Input: LR = 1: Left   0:Right
**Output: 
****************************************************************************************************/
void Motor_LockProc(uint8 LR)
{
	switch(Motor_SubState)
	{
		case CASE_IN:
		{
			/* 判断方向*/
			if(LR == 1)
			{/* Left */
				if(CheckLevelLeftSwOn())
				{
				  if ( BLELockType == CodeType )
					  {
							ResponseLockData( 0x1C,0);
					  }
					Motor_SubState = CASE_OK;
					PwrStopCnt = 2;
					Line_1A_WT588S( 0x1A ); 
					HALL_Status = 1;
					break;
				}
				else
				{
					Access_MotorWorkOn(1);         // 电机顺时钟  转盘逆时钟 向左转  左开门  Left
					Motor_SubState = CASE_01;
					Motor_Timer64ms = MOTOR_WORK_TIME;
				}
			}
			else
			{/* Right */
				if(CheckLevelRightSwOn())
				{
			  	if ( BLELockType == CodeType )
				  	{
							ResponseLockData( 0x1C,0);
				  	}
					Motor_SubState = CASE_OK;
					PwrStopCnt = 2;
					Line_1A_WT588S( 0x1A ); 		 
					NB_LockMsgLoad( );
					HALL_Status = 1;
					break;
				}
				else
				{
					Access_MotorWorkOn(0);
					Motor_SubState = CASE_01;
					Motor_Timer64ms = MOTOR_WORK_TIME;
				}
			}
			break;
		}
		case CASE_01:
		{/* 2S 等待*/
			if(Motor_Timer64ms == 0)
			{
			  LockErrorFlag = 1;
				Access_MotorStop();
				Motor_SubState = CASE_EEROR;
				break;
			}
			if(LR == 1)
			{/* Left */
				if(CheckLevelLeftSwOn()||ComCol1GetData()==0)
				{
					HALL_Status = 1;
					delay_ms(100);
					Access_MotorStop();
					Motor_Timer64ms = 50;
					Motor_SubState = CASE_03;
					break;
				}
			}
			else
			{/* Right */
				 if(CheckLevelRightSwOn()||ComCol3GetData()==0)
				 {
					 HALL_Status = 1;
					 delay_ms(150);
					 Access_MotorStop();
					 Motor_Timer64ms = 50;
					 Motor_SubState = CASE_03;
					 break;
				 }
			}
		break;
	}

#if 0			
			if(LR == 1)
			{/* Left */
				if(CheckLevelLeftSwOn()||ComCol1GetData()==0)
				{
					Motor_SubState = CASE_02;
					Motor_Timer64ms = 1;
				}
			}
			else
			{/* Right */
				 if(CheckLevelRightSwOn()||ComCol3GetData()==0)
				 {
					   Motor_SubState = CASE_02;
					   Motor_Timer64ms = 1;
						 
						 Access_MotorStop();
				 }
			}
			break;
		}
		case CASE_02:
		{
			if(LR == 1)
			{
				 if(Motor_Timer64ms==0||ComCol1GetData()==0)
				 {
					  	 Access_MotorStop();
						   Motor_Timer64ms = 10;
						   Motor_SubState = CASE_03;
						   break;
				 }
			}
			else
			{
					if(Motor_Timer64ms==0||ComCol2GetData()==0)
					{
					  	Access_MotorStop();
						  Motor_Timer64ms = 10;
						  Motor_SubState = CASE_03;
						  break;
					}
		  }
			break;
		}
		#endif	
		case CASE_03:
		{
			if(Motor_Timer64ms != 0)
			{
				 break;
			}
			TravelSwitchLast = ReversalSwGetData();
			if(LR == 1)
			{/* Left */
				 Access_MotorWorkOn(0);
			}
			else
			{/* Right */
				 Access_MotorWorkOn(1);
			}

			Motor_Timer64ms = 50;
			Motor_SubState = CASE_04;
			break;
		}
		case CASE_04:
		{

			if(TravelSwitchLast != ReversalSwGetData())
			{
				 Motor_Timer64ms=0;
			}

			if(Motor_Timer64ms != 0)
			{
				 break;
			}

			Motor_Timer64ms = 50;
			Motor_SubState = CASE_05;
			
			break;
		}
		
		case CASE_05:
		{
			if(Motor_Timer64ms == 0)
			{
				Access_MotorStop();
				Motor_SubState = CASE_EEROR;
				break;
			}
			
			if(ChecBackSwOn() || TravelSwitchLast != ReversalSwGetData())
			{
				Motor_Timer64ms = 0;
				Motor_SubState = CASE_06;
			}
			break;
		}
		case CASE_06:
		{
			if(Motor_Timer64ms == 0)
			{
				if(LR == 1)
				{/* Left */
					
					Access_MotorWorkOn(0);
				}
				else
				{/* Right */
					Access_MotorWorkOn(1);
				}
				Motor_SubState = CASE_07;
				/*
				if (LR == 1)                             //   回转   
					{
					  Motor_Timer64ms = 20;           // ?????   
				  }  
				else
					{
            if ( Access_BatteryData >= 90  )
            	{
            	  Motor_Timer64ms = 11;
            	}
						else if ((Access_BatteryData >= 50) && (Access_BatteryData < 90))
							{
							  Motor_Timer64ms = 17;
							}
						else
							{
							  Motor_Timer64ms = 21;
							}
					}
					*/
					Motor_Timer64ms = 10;
				break;
			}
			break;
		}
		case CASE_07:
		{
			if(Motor_Timer64ms == 0)
			{
				Access_MotorStop();   

				if ( BLELockType == CodeType )
					{
						ResponseLockData( 0x1C,0);
					}
				Motor_SubState = CASE_END;
				PwrStopCnt = 4;
				Line_1A_WT588S( 0x1A ); 		 
				NB_LockMsgLoad( );
				break;
			}
			break;
		}
		case CASE_EEROR:
			if ( BLELockType == CodeType )
				{
					ResponseLockData( 0x1C,1);
				}		
			HALL_Unlogflag = 0;
		case CASE_END:
		{
			Access_MotorStop();
			break;
		}
	}
}


/****************************************************************************************************
**Function:
		void Motor_UnLockProc(uint8 LR)
**Author: rory
**Description:
**Input: LR = 1: Left   0:Right
**Output: 
****************************************************************************************************/
void Motor_UnLockProc(uint8 LR)
{
	switch(Motor_SubState)
	{
		case CASE_IN: /* 判断方向*/
		{			
			if( ChecVerticalSwOn())
			//if( ComCol2GetData()==0)
			{
			  if ( BLEUnLockType == CodeType )
				  {
						ResponseLockData( 0x1B,0);
				  }			
				 HALL_Status = 2;
				 Motor_SubState = CASE_OK;
				 PwrStopCnt = 2;
				 Line_1A_WT588S( 0x19 );	
				 break;
			}
			else
			{  
			   if (LR == 1)/* Left */
			   { 
					 Access_MotorWorkOn(0);
			   }
				 else   /* Right */
				 {
					 Access_MotorWorkOn(1);
				 }
				 Motor_SubState = CASE_01;
				 Motor_Timer64ms = MOTOR_WORK_TIME;
			}
			
			break;
		}
		case CASE_01:
		{/* 2S 等待*/
								
			if(Motor_Timer64ms == 0)
			{
				Access_MotorStop();
				Motor_SubState = CASE_EEROR;
				break;
			}

		 if(ChecVerticalSwOn() || ComCol2GetData()==0)
		 {
			 HALL_Status = 2;
			 /*
			if ( Access_BatteryData >= 85  )
				{
				  delay_ms(500);
				}
			else if ((Access_BatteryData >= 45) && (Access_BatteryData < 85))
				{
					delay_ms(600);
				}
			else
				{
					delay_ms(700);
				}
       */
       delay_ms(200);
			 Access_MotorStop();
			 Motor_Timer64ms = 100;
			 Motor_SubState = CASE_03;
			 break;
		 }
			break;
		}
	
		case CASE_03:
		{
			if(Motor_Timer64ms != 0)
			{
				 break;
			}
			
			TravelSwitchLast = ReversalSwGetData(); //行程开关
			if(LR == 1)/* Left */
			{
				  Access_MotorWorkOn(1);
			}
			else/* Right */
			{
				 Access_MotorWorkOn(0);
			}
			Motor_Timer64ms = 30;                 //  
			Motor_SubState = CASE_04;
			break;
		}
		case CASE_04:
		{
			 if(TravelSwitchLast != ReversalSwGetData())
			 Motor_Timer64ms=0;
			 
			 if(Motor_Timer64ms != 0)
			 {
				  break;
			 }
			 Motor_Timer64ms = 30;
			 Motor_SubState = CASE_06;
			 
			 break;
		}
		/*
		case CASE_05:
		{

			if(Motor_Timer64ms == 0)
			{
				Access_MotorStop();
				Motor_SubState = CASE_EEROR;
				break;
			}
			if( ChecBackSwOn()||(TravelSwitchLast != ReversalSwGetData()) )
			{
				Motor_SubState = CASE_06;
				
				Motor_Timer64ms = 0;
			}
			break;
		}
		*/
		case CASE_06:
		{
			if(Motor_Timer64ms == 0)
			{
				if(LR == 1)
				{/* Left */

					Access_MotorWorkOn(1);
				}
				else
				{/* Right */
					Access_MotorWorkOn(0);
				}
				Motor_SubState = CASE_07;
        /*  
				if ( Access_BatteryData >= 85  )
					{
					  Motor_Timer64ms = 40;
					}
				else if ((Access_BatteryData >= 35) && (Access_BatteryData < 85))
					{
					  Motor_Timer64ms = 50;
					}
				else
					{
					  Motor_Timer64ms = 70;
					}
        */
         Motor_Timer64ms = 10;
				break;
			}
			break;
		}
		case CASE_07:
		{
			if(Motor_Timer64ms == 0)
			{
				Access_MotorStop();
				if ( BLEUnLockType == CodeType )
					{
						ResponseLockData( 0x1B,0);
					}
				Motor_SubState = CASE_END;
				PwrStopCnt = 4;
				
				Line_1A_WT588S( 0x19 ); 		 
			  NB_UnLockMsgLoad( );				
				break;
			}
			break;
		}
		case CASE_EEROR:
			if ( BLEUnLockType == CodeType )
				{
					ResponseLockData( 0x1B,1);
				}			
			HALL_Unlogflag = 0;
		case CASE_END:
		{
			break;
		}   
	}
}


/****************************************************************************************************
**Function:
    Std_ReturnType Motor_Write(uint8 Cmd,void *pData)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Motor_Proc(void)
{
	if((Motor_DetectSw>0)&&(Motor_DetectSw<4))
	{
		//COM_COLHight();
		Motor_DetectPosit();
	}
  LockError();
	switch(Motor_State)
	{
		case MOTOR_STAT_IDLE:
		{
			break;
		}
		case MOTOR_STAT_LOCK:
		{
				 Motor_LockProc(Motor_SwLR);
				 if(Motor_SubState == CASE_OK)
				 {
						 Motor_State = MOTOR_STAT_IDLE;
						 break;
				 }
				 else if((Motor_SubState == CASE_END)||(Motor_SubState == CASE_EEROR))
				 {
					  Motor_State = MOTOR_STAT_IDLE;
					  break;
				 }
				 break;
		}
		case MOTOR_STAT_UNLOCK:
		{
			Motor_UnLockProc(Motor_SwLR);

			if(Motor_SubState == CASE_OK)
			{
				Motor_State = MOTOR_STAT_IDLE;
				if(Sys_CheckAutoLock())
				{
					Motor_AutoLockTimer64ms = Sys_PataCfg.OpenLockTime/10;
				}
				break;
			}
			else if(Motor_SubState == CASE_EEROR)
			{
				Motor_State = MOTOR_STAT_IDLE;
				break;
			}
			else if(Motor_SubState == CASE_END)
			{
				//if(Factory_State != COMD_FATORY_MOTOR_TEST)
				if( 1 )
				{
					if(Sys_CheckAutoLock())
					{
						Motor_AutoLockTimer64ms = Sys_PataCfg.OpenLockTime/10;
					}
					if(Sys_CheckEnableDoorLock())
					{
						Motor_Timer64ms = 50;
						Motor_State = MOTOR_STAT_WAITOPENDOOR;	
					}
					else
					{
						Motor_State = MOTOR_STAT_IDLE;
					}
				}
				else
				{
					Motor_State = MOTOR_STAT_IDLE;
				}
			}
			break;
		}
		case MOTOR_STAT_WAITOPENDOOR:
		{
			if(DoorIsOpen())
			{
				Motor_DoorLockSt = MOTOR_DOORLOCK_OPEN;
				Motor_State = MOTOR_STAT_IDLE;	
				break;
			}
			else if(Motor_Timer64ms == 0)
			{
				Motor_State = MOTOR_STAT_IDLE;
				Motor_DoorLockSt = MOTOR_DOORLOCK_OPENLOCK;
				break;
			}
			break;
		}
		default:
		{	
			Motor_State = MOTOR_STAT_IDLE;
		}
	}
}

/****************************************************************************************************
**Function:
    Std_ReturnType Motor_IoCtl(uint8 Cmd,void *pData)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
 Std_ReturnType Motor_IoCtl(uint8 Cmd,void *pData)
 {
 	switch(Cmd)
 	{
		case MOTOR_CMD_SCAN:
		{
			if(Motor_Timer64ms != 0)
			{
				Motor_Timer64ms--;
			}
			
			if(Motor_AutoLockTimer64ms != 0)
			{
				Motor_AutoLockTimer64ms--;
				if(Motor_AutoLockTimer64ms == 0)
				{
				  if ( Sys_DisblePass())
				  	{
				  		CodeType = AutoLockType;
					    Access_Lock();
				  	}
				}
			}
			if(Motor_DetectSw == 0xff)
			{
				//COM_COLHight();
			}
			else if(Motor_State !=MOTOR_STAT_IDLE)
			{
				//COM_COLHight();
				Motor_DetectSw = 2;
				break;
			}
			else if(Motor_DetectSw == 4)
			{
			//	COM_COLLow();
			}
			else if(Motor_DetectSw > 8)
			{
				Motor_DetectSw = 0xfe;
			}
			Motor_DetectSw++;
			break;
		}
		case MOTOR_CMD_OPEN:
		{
			if(Motor_State == MOTOR_STAT_IDLE)
			{
				Motor_State = MOTOR_STAT_UNLOCK;	
				Motor_SubState = CASE_IN;	
				Motor_SwLR = Motor_CheckLR();
			}
			break;
		}
		case MOTOR_CMD_CLOSE:
		{
			if((Motor_State == MOTOR_STAT_IDLE) || (Motor_State == MOTOR_STAT_WAITOPENDOOR))
			{
				Motor_State = MOTOR_STAT_LOCK;	
				Motor_DoorLockSt = MOTOR_DOORLOCK_IDLE;
				Motor_SubState = CASE_IN;
				Motor_SwLR = Motor_CheckLR();
			}
			break;
		}
		case MOTOR_CMD_SCAN_DOOR:
		{
			if(DoorSwGetData() == 0)
			{
				if(Motor_DoorSw < 3)
				{
					Motor_DoorSw++;
					if(Motor_DoorSw == 3)
					{
						if((Motor_DoorLockSt == MOTOR_DOORLOCK_OPEN) && (Sys_CheckEnableDoorLock())
							&& (Motor_PositStatus != MOTOR_POSIT_LOCK) && (Sys_DisblePass()))
						{
									CodeType = AutoLockType;
									Access_Lock();
						}
					}
				}
			}
			else
			{
				if(Motor_DoorSw > 0)
				{
					Motor_DoorSw--;
					if(Motor_DoorSw == 0)
					{
						if(Motor_DoorLockSt == MOTOR_DOORLOCK_OPENLOCK)
						{/* 门被打开*/
							Motor_DoorLockSt = MOTOR_DOORLOCK_OPEN;
						}
					}
				}
			}
			
	 if ( DoorSw_Unlogflag ) 	return E_OK;

   	if ( DoorSwGetData() == 0)
			{
			  if ( 0 == DoorSw_Status )   DoorSw_Status = 1;
        if ( 2 == DoorSw_Status )
					{
					   DoorSw_Status = 1; 
			  		 CodeOtherType = ContactSensorClosedType;
					   NB_otherMsgLoad( );
					}
   		}
		else
			{
			  if ( 0 == DoorSw_Status )   DoorSw_Status = 2;
        if ( 1 == DoorSw_Status )
					{
					  DoorSw_Status = 2; 
						CodeOtherType = ContactSensorOpenedType;
					  NB_otherMsgLoad( );
					}
			}
			break;
		}
		default:break;
	}
	return E_OK;
 }

/****************************************************************************************************
**Function:
   Std_ReturnType Motor_Close(void *pData)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Motor_Close(void *pData)
{

}

const Dr_OpertonType MotorDrive =
{
	Motor_Open, Motor_Close, Motor_Read, Motor_Write, Motor_IoCtl
};

