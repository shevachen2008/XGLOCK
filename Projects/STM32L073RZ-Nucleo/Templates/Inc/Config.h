/*****************************************************************************
|  File Name: Usart_bsp.h
|
|  Description: Header File defined
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     -------------------------------------
| LXW          LXW               
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date        Ver     Author  Description
| ---------   ----    ------  ------------------------------------------------
| 2013-10      LXW     Initial version 
|****************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H


/******************************************************************************
**********************Macro definitiont*******************************************
******************************************************************************/
//#define IOP_EN			STD_TRUE


#define  KEY_LIGHT_ONE      0    /* ����ͳһ����*/
#define  KEY_LIGHT_12        1    /* ����Ϊ12����*/  
#define  KEY_LIGHT_13        2    /* ����Ϊ13����*/  
#define  KEY_LIGHT_16        3    /* ����Ϊ16����*/

#define WATCHDOG_EN		STD_TRUE
#define RTC_EN				STD_TRUE
#define BEEP_EN				STD_TRUE
#define LIGHT_EN			STD_TRUE
#define ADC_EN				STD_FALSE
#define PWM_EN				STD_TRUE
#define SYS_ENCRY			STD_FALSE

#define KEY_EN   				STD_TRUE

#define UART_EN					STD_TRUE
#define FLASH_EN				STD_TRUE
#define EEPROM_EN				STD_FALSE

#define RC522_EN				STD_TRUE
#define RC523_TYPEB				STD_FALSE  /* TRUE:���֤*/
#define RC522_RDDAT				STD_TRUE
#define RC522_CARD_ENCPT		STD_TRUE  /* ��ƬID����*/
#define T57CARD					STD_FALSE

#define COMMENCPT_ENABLE 		STD_TRUE   	/*TRUE: ͨѶ����  ��̩��ɽķ�Ĳ�����*/
#define DOORBELL_EN   			STD_FALSE 		/* ���尴ť��Ч*/
#define ENABLE_DYNC_PASWD   	STD_TRUE		/* ��̬����*/


#define SUPPORT_M1_CARD		STD_TRUE
#define SUPPORT_PWSD			STD_TRUE
#define SUPPORT_FINGERPRINT	STD_FALSE
#define SUPPORT_IDENTITY_CARD	STD_TRUE   	/* TRUE: ���֤����*/
#define SUPPORT_APP_ID			STD_TRUE

#define MOTOR_DRIVE_TIME450           450
#define MOTOR_DRIVE_TIME400           400
#define MOTOR_DRIVE_TIME250           250

#define SUPPORT_RECORD_LOC_STORE STD_TRUE   	/* TRUE:���ش洢��¼*/

#define SUPPORT_DETDOOR		      STD_TRUE			//�Ŵż��

#define USE_FLASH_NRF           STD_FALSE                    //�õ�Ƭ���ڲ�FLASH

#define TOUCH_WAKEUP_PANEL     STD_FALSE  /* ��������ʱ��Ҫ�Ȼ��Ѳ��ܲ�������*/

#define LORA_ENABLE                  STD_FALSE
#define USE_RFEX_MODE                STD_FALSE   /* ���õ�����ģ��*/

#define SUPPORT_RECORD_RTIME_SEND	 STD_FALSE   //��¼��ʵ�ϴ�

#define KEY_BOARD      KEY_LIGHT_13

#define  TOUCH_V11       0
#define  TOUCH_V13       1
#define TOUCH_BORAD   TOUCH_V13

#define TOUCH_ENABLE   STD_TRUE

#define DEBUG_ENABLE   STD_FALSE
#endif	

