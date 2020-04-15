
/***************************************************************
 * Copyright (C), 2019-2029, Rently
 * File name    : Main.c
 * Author       : sheva
 * Version      : V1.0.0
 * Date         : 2019-07-24
 * Description  : 
 * Others       : 
***************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "main.h"

U8  ticks2ms;
U8  ticks10ms;
U8  ticks20ms;
U8  ticks50ms;
U8  ticks100ms;
U8  ticks1s;
U8  ticks2s;

Rtc_Type Rtc_rdata;

uint8    step;
uint8 Factory_Sub = 0;
uint8 Factory_Time64ms;

void Factory_Motor_Test(void)
{
	switch(Factory_Sub)
	{
		case CASE_IN:
		{
				MotorDrive.ioctl(MOTOR_CMD_CLOSE, NULL);
				Factory_Sub = CASE_01;
				Factory_Time64ms = 50;
			break;
		}
		case CASE_01:
		{ /* 关*/
			if((Motor_State == MOTOR_STAT_IDLE) || (Factory_Time64ms == 0))
			{
				Factory_Sub = CASE_02;   //
				Factory_Time64ms = 50;
			}
			break;
		}
		case CASE_02:
		{/* 等待开锁*/
			if(Factory_Time64ms == 0)
			{
				MotorDrive.ioctl(MOTOR_CMD_OPEN, NULL);
				Factory_Sub = CASE_03;
				Factory_Time64ms = 50 ;
			}
			break;
		}
		case CASE_03:
		{ /* 开锁*/
			if((Motor_State == MOTOR_STAT_IDLE) || (Factory_Time64ms == 0))
			{
				Factory_Sub = CASE_04;
				Factory_Time64ms = 50 ;
			}
			break;
		}
		case CASE_04:
		{ /* 等待关锁*/
			if (Factory_Time64ms == 0)
			{	/* 关锁*/
				MotorDrive.ioctl(MOTOR_CMD_CLOSE, NULL);
				Factory_Sub = CASE_01;
				Factory_Time64ms =50;
			}
			break;
		}
		default:
		{
			Factory_Sub = CASE_END;
			break;
		}
	}
}
void Factory_Proc(void)
{
/*
  if ( AutoCheckStatus == 0 || AutoCheckStatus == 1 )
  	{
  	   Factory_Time64ms = 30;
  	   return;
  	}
	*/

	if(Factory_Time64ms != 0)
	{
		Factory_Time64ms--;
		return;
	}
	Factory_Motor_Test();
}

uint8  hall1;  
uint8  hall2;
uint8  hall3;

uint16  KK;


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

int main(void) 
{
	HAL_Init();
	System_HardwareInit();

	Sys_Init();

	if ((__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET))     // 掉电复位 
		{
		  Sys_PataCfg.CheckLR = 0x0;
		 	Sys_StorePara();
	 }
	while (1)
		{
		 	if ( PwrStopStatus == 1)
		    {
		      PwrGpio_Scan();
		    } 
		if ( ticks2ms )
				{	
					ticks2ms = 0;
					if ( ++ticks10ms >= 5 )                 //  10ms
						{
							ticks10ms = 0;
							KeyPress();
							hall1 = ComCol1GetData();
							hall2 = ComCol2GetData(); 		 
							hall3 = ComCol3GetData(); 	
							//HAL_IWDG_Refresh(&hiwdg);     // 喂狗
//#if 1
							Motor_Proc();
							MotorDrive.ioctl(MOTOR_CMD_SCAN, NULL);
							if ( 1 == AutoCheckStatus )
								{
							  	Motor_AutoCheckLR( );
								}
							if ( ++ticks20ms >= 2 )             //  20ms
								{
									ticks20ms = 0;
							  	GetPowervalue();
									if ( UART1_RXFlag ) 
										{
										  UART1_RXFlag = 0;
									   // HAL_UART_Transmit(&UART4_Handle, (uint8_t*)RxBuffer, RxCounter,2);
										  BC26_RECData();
										}
									if ( UART4_RXFlag ) 
										{
										  UART4_RXFlag = 0;
											if ( AgtestFlag )
												{
												  AGPRDATA();
												}
											else
												{
												  PR_UARTDATA();
												}
										}
								}
#if 1
							if ( ++ticks50ms >= 5 )              //  50ms
								{
									ticks50ms = 0;
								}	
							if ( ++ticks100ms >= 10 )             //  100ms
								{
									ticks100ms = 0;
									NB_Timer( );
									Sys_RestProc( );
							  	GetPowerPercent( );
									MotorDrive.ioctl(MOTOR_CMD_SCAN_DOOR, NULL);   // 门磁
								  //Factory_Proc();
									if ( ++ticks1s >= 10 )			 //  1s       
										{
											 ticks1s = 0;  
									  //   PwrStopTimer( );
									     CutPinProc( );
									     Read_IMEI( );
								       NB_Init( );
									  // Agtest( );			
									  // NB_SendCsq( );
									  
										}
									if ( ++ticks2s >= 20 )      // 2s
										{
										  ticks2s = 0;
										  NB_HeartBeat();
										//TestAddCode();
										}
								}		
#endif
						}
				}	
		}
}


#if 0
//BC26_msgSend();
//HAL_GPIO_TogglePin( GPIOC, GPIO_PIN_7 );
//	flag =	ReversalSwGetData();
//	hall1 = ComCol1GetData();
//	hall2 = ComCol2GetData();			 
//	hall3 = ComCol3GetData();	


if ( ++aaa > 0x70 )
   {
	  aaa = 0;
   }
 Line_1A_WT588S( aaa );
 Rtc_Read(&Rtc_rdata);
 //HAL_UART_Transmit(&UART4_Handle, (uint8_t*)BBBB, 7,2);
  
KK = SPI_FLASH_ReadID();
//BC26_Senddata("16","420112345678B4706174680473756231");
//HAL_UART_Transmit(&UART4_Handle, (uint8_t*)RxBuffer, RxCounter,2);
//	GPIO_SET_C7(1);  
//	delay_ms(10);
//	GPIO_SET_C7(0);  

/*
strx=strstr((const char*)UART2_RXBuffer,(const char*)"+AT+READ");
if ( strx )
	{
		 
		 Clear_Buffer();
		 NBAT_SEND("AT+QLWNOTIFY=19,0,0,1,8,\"OAKSLOCK\",0,1\r\n"); 
		//MotorDrive.ioctl(MOTOR_CMD_OPEN, NULL);
		//openFlag = 1;
	} 


strx=strstr((const char*)UART2_RXBuffer,(const char*)"+AT+OPEN");
if ( strx )
	{
		 
		 Clear_Buffer();
		 NBAT_SEND("AT+QLWRD\r\n"); 
		//MotorDrive.ioctl(MOTOR_CMD_OPEN, NULL);
		//openFlag = 1;
	} 

strx=strstr((const char*)UART2_RXBuffer,(const char*)"AT+QLWDEREG");
if ( strx )
	{
		 
		 Clear_Buffer();
		 NBAT_SEND("AT+QLWDEREG\r\n"); 
	} 			

strx=strstr((const char*)UART2_RXBuffer,(const char*)"AT+QLWREG");
if ( strx )
	{
		 
		 Clear_Buffer();
		 NBAT_SEND("AT+QLWREG\r\n"); 
		 cStatus = CONNECT_BUFFER;
	} 														

strx=strstr((const char*)UART2_RXBuffer,(const char*)"+AT+CLOSE");
if ( strx )
	{
		MotorDrive.ioctl(MOTOR_CMD_CLOSE, NULL);
	} 
*/
NBAT_SEND("AT\r\n"); 
delay_ms(100);
HAL_UART_Transmit(&UART1_Handle,UART2_RXBuffer,sizeof(UART2_RXBuffer),2);
	UART2_RXcnt = 0;
	memset (UART2_RXBuffer,0,sizeof(UART2_RXBuffer));
	/*
											 ProtoAnaly_UpdateTime();
											if ( 0 == ComCol1GetData()	)
											 {
													HAL_UART_Transmit(&UART4_Handle, "00", 2,2);
											 }
											else
											 {
												HAL_UART_Transmit(&UART4_Handle, "11", 2,2);
											 }
											
											if ( 0 == ComCol2GetData()	)
											 {
												 HAL_UART_Transmit(&UART4_Handle, "00", 2,2);
											 }
											else
											 {
												 HAL_UART_Transmit(&UART4_Handle, "11", 2,2);
											 }
											*/

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
