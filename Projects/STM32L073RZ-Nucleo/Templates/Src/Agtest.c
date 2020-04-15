/****************************************************************************************
文件:   Agtest.c
说明:    
****************************************************************************************/
//头文件.
#include "main.h"

U8 AgtestFlag;
U8 AgCmdSendFlag;
U8 AgCmdSendcnt;
U8 AgDateSendFlag;
U8 AgDateSendcnt;

U8 AgtestCnt2s;
U8 AgtestCnt8s;
U8 AgtestCnt20s;
U8 AgtestCnt120s;

U8  AgtestFlashError;
U8  AgtestRtcError;
U8  AgtestADCError;
U8  AgtestHallError;
U8  AgtestNbError;
U8  AgtestNbUartError;
U8  AgtestBLEUartError;

U8  AgtestPutBuf[ 10 ];

U32 	RTCtestTime1;
U32 	RTCtestTime2;

void Agtest( void )
{
   if ( 0 == AgtestFlag )   return;
	 ProtoAnaly_UpdateTime();
   if ( 1 == AgCmdSendFlag )
   	{
		  SendAgtestCMD();
   	  if ( ++AgCmdSendcnt >=5)
				{
				  AgCmdSendFlag = 0;
					AgtestBLEUartError = 1;
			  }
   	}

   if ( 1 == AgDateSendFlag )
   	{
		  SendAgtestDate();
   	  if ( ++AgDateSendcnt >=5)
				{
				  AgDateSendFlag = 0;
					AgtestBLEUartError = 1;
			  }
   	}	 

	 if( 0 == AgtestCnt2s )  RTCtestTime1 = ProtoAnaly_RtcLocalTime ;
   if ( ++AgtestCnt2s >= 2)
   	 {
   	    AgtestCnt2s =0;
				if ( 0xEF15 == SPI_FLASH_ReadID())
					{
					  AgtestFlashError = 0;
					}
				else
					{
					  AgtestFlashError = 1;
					}

				if (0x100 >= powervalue)
					{
						AgtestADCError = 0;
					}
				else
					{
						AgtestADCError = 1;
					}

			 if ( RTCtestTime1 > RTCtestTime2 )
				 {
				   AgtestRtcError = 0;
				 }
			 else
				 {
				   AgtestRtcError = 1;
				 }
       RTCtestTime2 = RTCtestTime1; 
			 
			 if ((0 == ComCol1GetData()) 
			 	   || (0 == ComCol2GetData())
			 	   ||( 0 == ComCol3GetData()))
			 	 {
			 	   AgtestHallError = 1;
			 	 }
			 else
			 	 {
					 AgtestHallError = 0;
			 	 }
   	}

	 if ( ++AgtestCnt8s >= 8)
	 	 {
	 	    AgtestCnt8s = 0;
				
			  Clear_Buffer(); 
			  NBAT_SEND("AT\r\n"); 
        delay_ms(200);

				strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
			 
			 if ( strx )
			 	{
			 	  AgtestNbUartError = 0;
			 	}
			 else
			 	{
			 	   AgtestNbUartError = 1;
			 	}

				if ( AgtestBLEUartError )
					{
					  Line_1A_WT588S( 0x0d );
					}
				else
					{
						Line_1A_WT588S( 0x0E );
					}
	 	 }

		if ( ++AgtestCnt20s >= 20)
	 	 {
	 	    AgtestCnt20s = 0;
				AgDateSendFlag = 1;
				AgDateSendcnt = 0;
				SendAgtestDate();
				
	 	 }
		 
	 if ( ++AgtestCnt120s >= 120)
	 	 {
	 	    AgtestCnt120s = 0;
	 	    NBInitFlag = 1;
	 	 }
}


void SendAgtestCMD( void )
{
	 U8 checksum;
	 GPIO_SET_A3(1);
   delay_ms(100);
	 GPIO_SET_A3(0);

   delay_ms(500);
	 
	 memset(UART4_RXBuffer,0,sizeof( UART4_RXBuffer ));
	 UART4_RXcnt = 0;
   AgtestPutBuf[0] = 0xB4;
   AgtestPutBuf[1] = 0x03;
   AgtestPutBuf[2] = 0x88;
   AgtestPutBuf[3] = 0x00;
	 
	 checksum = Sys_GenFunChecksum(&AgtestPutBuf[1]);
	 AgtestPutBuf[4] = checksum;
	 HAL_UART_Transmit(&UART4_Handle, (uint8_t*)AgtestPutBuf, 5,2);
}


void SendAgtestDate( void )
{
	 U8 checksum;
	 GPIO_SET_A3(1);
   delay_ms(200);
	 GPIO_SET_A3(0);

   delay_ms(500);

	 memset(UART4_RXBuffer,0,sizeof( UART4_RXBuffer ));
	 UART4_RXcnt = 0;
	
   AgtestPutBuf[0] = 0xB4;
   AgtestPutBuf[1] = 0x08;
   AgtestPutBuf[2] = 0x28;
   AgtestPutBuf[3] = AgtestRtcError;
   AgtestPutBuf[4] = AgtestNbError;
   AgtestPutBuf[5] = AgtestNbUartError;
   AgtestPutBuf[6] = AgtestHallError;
   AgtestPutBuf[7] = AgtestADCError;
   AgtestPutBuf[8] = AgtestFlashError;

	 checksum = Sys_GenFunChecksum(&AgtestPutBuf[1]);
	 AgtestPutBuf[9] = checksum;
	 HAL_UART_Transmit(&UART4_Handle, (uint8_t*)AgtestPutBuf, 10,2);
}

void AGPRDATA(void)
{
	if ( 0x4B ==  UART4_RXBuffer[0] )
		{
		  AgCmdSendFlag = 0;
			AgDateSendFlag = 0;
			AgtestBLEUartError = 0;
		}
}


