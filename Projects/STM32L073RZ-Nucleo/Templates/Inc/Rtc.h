

#ifndef RTC_H_
#define RTC_H_

#include "main.h"

/******************************************************************************
**********************Macro definitiont*******************************************
******************************************************************************/
#define 	RTC_ADD					0x60
#define 	RTC_COMM_ALL_REALTIME	0x04
#define 	RTC_COMM_TIME_REALTIME	0x06
#define 	RTC_COMM_STATUS_REG1	0
#define 	RTC_COMM_STATUS_REG2	0x02
#define 	RTC_COMM_INT1			0x08
#define 	RTC_COMM_INT2			0x0A


//#define  	RTC_CHECK				STD_TRUE

/******************************************************************************
**********************Type statement*******************************************
******************************************************************************/
#if 1

typedef uint8 Rtc_CommType;
#define RTC_FAILED					0
#define RTC_SUCCESS					1

typedef enum 
{
	RTC_CLOCKSET 		= 0,	
	RTC_WLARMSET ,	
	SETRTCCONTROLMAX	
} Rtc_CmdType;

typedef struct {
	uint8	Year;
	uint8	Month;
	uint8	Day;
	uint8	Hour;
	uint8	Minute;
	uint8	Second;
	uint8 	Week;
}Rtc_Type;

typedef struct {
	uint8	Year;
	uint8	Month;
	uint8	Day;
}Rtc_CaledarType;

typedef struct {
	uint8	Hour;
	uint8	Minute;
	uint8	Second;
}Rtc_TimeType;
typedef struct {
	uint8	Flag;
	uint8	Hour;
	uint8	Minute;
	uint8	Second;
}Rtc_WlarmType;

/******************************************************************************
**********************Variable definition***************************************
******************************************************************************/
extern uint32 ProtoAnaly_RtcLocalTime;
extern uint32 ProtoAnaly_RtcLocalTimeBak;

extern uint8 Rtc_Wlarm_Enable;
extern Rtc_Type ProtoAnaly_Rtcinfo;

extern Std_ReturnType Rtc_Ioctl(uint8 Cmd,void *pData);
extern Std_ReturnType Rtc_Read(void *pData);
extern Std_ReturnType Rtc_Read1(void *pData);

extern void Rtc_Open(void *pData);
extern uint8 Main_GetWeekFromDay(unsigned char year,unsigned char month,unsigned char day) ;
extern void RTC_Check(void);
extern void Rtc_Init(void);
#endif
/******************************************************************************
**********************Function declared****************************************
******************************************************************************/
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


