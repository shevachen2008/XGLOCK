
#ifndef STD_TYPES_H
#define STD_TYPES_H
#include"main.h"

/***************************************************************************/
/* HEADER-FILES (Only those that are needed in this file)                  */
/***************************************************************************/


/***************************************************************************/
/* Include files                                                           */
/***************************************************************************/
#include <stdio.h>
#include <string.h>
#include<math.h>

/***************************************************************************/
/* Version                  (abcd: Main version ab Sub Version )        */
/***************************************************************************/



/***************************************************************************/
/* macros                                                                  */
/***************************************************************************/
#define E_OK  0x00
#define E_NOT_OK  0x01
#define E_INPAR_ERR  0x02
#define E_REVDATA_ERR  0x03
#define E_HAR_ERR  0x04
#define E_HAR_WAIT  0x05
#define E_NOT_CONNT  0x06
#define E_END_LIGHT  0x7E  /* 只需要灯光提示*/
#define E_END  0x7F
#define E_GET_ACK 0x80

//#define E_TIME_OUT   0x06

enum
{
	E_SYS_PASWD_WRONG = 2, /* 系统码错误*/
	E_LOCK_ID_WRONG, /* 锁ID 错误*/
	E_USER_PASWD_WRONG,  /*用户密码错误*/
	E_TIME_OUT, /* 时间无效*/
	E_NO_LOGIN, /* 未登录*/
	E_KEY_EXIST, /* 钥匙，密码，卡片已经存在*/
	E_KEY_FULL, /* 钥匙，密码，卡片已经满了*/
	E_KEY_EMPTY, /* 钥匙不存在*/
	E_KEY_NO_SUPPORT, /* 命令不支持*/
	E_NO_LICENSE, /* 未授权*/
	E_PASSWORD_DEFAULT, /* 设备密码为默认密码*/
};

/* 0x02 - 0x3F : Available to user specifica errors */
//typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

#define STD_HIGH  0x01 /* Physical state 5V or 3.3V */
#define STD_LOW  0x00 /* Physical state 0V */

#define STD_ACTIVE  0x01 /* Logical state active */
#define STD_IDLE  0x00 /* Logical state idle */

#define STD_ON  0x01
#define STD_OFF  0x00

#define STD_TRUE  0x01
#define STD_FALSE  0x00

#define STD_WRITE  0x01
#define STD_READ  0x00

#define EVE_OK  0x01
#define EVE_NO  0x00

#define STD_RIGHT  0x01
#define STD_LEFT  0x00 

enum{
	CASE_IN,
  	CASE_01,
       CASE_02,
       CASE_03,
       CASE_04,
       CASE_05,
       CASE_06,
       CASE_07,
       CASE_EEROR,
       CASE_OK,
       CASE_END
};
/***************************************************************************/
/* Default switches                                                        */
/***************************************************************************/


/***************************************************************************/
/* Memory qualifier                                                        */
/***************************************************************************/


/***************************************************************************/
/* return values                                                           */
/***************************************************************************/


/***************************************************************************/
/* error codes                                                             */
/***************************************************************************/


/***************************************************************************/
/* compatibility defines                                                   */
/***************************************************************************/       


/***************************************************************************/
/* function prototypes internal                                            */
/***************************************************************************/
#define SIGN(x)  (((x) >= 0) ? (1) : (-1))
//#define MAX(x, y)  (((x) < (y)) ? (y) : (x))
//#define MIN(x, y)  (((x) > (y)) ? (y) : (x))

#define TWOHI_UINT32(a) (((a) >> 24) & 0xFF)
#define TWOLO_UINT32(a) (((a) >> 16) & 0xFF)
#define ONEHI_UINT32(a) (((a) >> 8) & 0xFF)
#define ONELO_UINT32(a) ((a) & 0xFF)

#define BUILD_FLAT_TIME(YEAR, MONTH, DAY) \
	   ((uint16)YEAR << 9) | ((uint16)MONTH << 5) | (uint16)DAY

//#define BUILD_HOTEL_TIME(YEAR, MONTH, DAY, HOUR) \
//	   ((uint32)YEAR << 17) | ((uint32)MONTH << 13) | ((uint32)DAY << 8) | HOUR

#define BUILD_TIME(YEAR, MONTH, DAY, HOUR, MINUTE, SECOND) \
	  (((uint32)YEAR << 26) & 0xfc000000) + (((uint32)MONTH << 22) & 0x3c00000) \
	+ (((uint32)DAY << 17) & 0x3e0000) + (((uint32)HOUR << 12) & 0x1f000) \
	+ (((uint32)MINUTE << 6) & 0xfc0) + (SECOND & 0x3f)

#define BUILD_HOTEL_TIME(YEAR, MONTH, DAY, MINUTE) \
	  (((uint32)(YEAR+16) << 26) & 0xfc000000) + (((uint32)MONTH << 22) & 0x3c00000) \
	+ (((uint32)DAY << 17) & 0x3e0000) + (((uint32)((MINUTE*15)/60) << 12) & 0x1f000) \
	+ ((uint32)(((MINUTE*15)%60) << 6) & 0xfc0)

#define BUILD_LOOP_TIME( HOUR, MINUTE, SECOND) \
	  (((uint32)HOUR << 12) & 0x1f000) \
	+ (((uint32)MINUTE << 6) & 0xfc0) + (SECOND & 0x3f)
#define BUILD_YEAR(a) ((a&0xfc000000) >> 26)
#define BUILD_MONTH(a) ((a&0x3c00000) >> 22)
#define BUILD_DAY(a) ((a&0x3e0000) >> 17)
#define BUILD_HOUR(a) ((a&0x1f000) >> 12)
#define BUILD_MINUTE(a) ((a&0xfc0) >> 6)
#define BUILD_SECOND(a) (a&0x3f)

#define BUILD_ACTIONTYPE(type, action) \
          ((uint8)(((action) & 0x07)|(((type) & 0x1F) << 3)))

#define BUILD_TYPE(x)      (x>>3)
#define BUILD_ACTION(x)  (x&0x07)

/***************************************************************************/
/* data types                                                              */
/***************************************************************************/
typedef uint8 Std_ReturnType;
typedef uint8 Std_ReWrType;
typedef uint8 Std_LeveType;
typedef uint8 Std_JudgeType;
typedef uint8 Eve_ReultType;
typedef uint8 Std_Pulse_CountType;



typedef struct
{
	uint16 vendorID;
	uint16 moduleID;
	uint8  SW_major_version;
	uint8  SW_minor_version;
	uint8  SW_patch_version;
}Std_VersionInfoType;

typedef struct
{
    unsigned char bit0:1; 
    unsigned char bit1:1; 
    unsigned char bit2:1; 
    unsigned char bit3:1; 
    unsigned char bit4:1; 
    unsigned char bit5:1; 
    unsigned char bit6:1; 
    unsigned char bit7:1; 
}Std_Bit8Flag;

typedef struct
{
    unsigned char bit0:1; 
    unsigned char bit1:1; 
    unsigned char bit2:1; 
    unsigned char bit3:1; 
    unsigned char bit4:1; 
    unsigned char bit5:1; 
    unsigned char bit6:1; 
    unsigned char bit7:1;
    unsigned char bit8:1; 
    unsigned char bit9:1; 
    unsigned char bit10:1; 
    unsigned char bit11:1; 
    unsigned char bit12:1; 
    unsigned char bit13:1; 
    unsigned char bit14:1; 
    unsigned char bit15:1;  
}Std_Bit16Flag;


typedef union
{
	Std_Bit8Flag  bits ;
	unsigned char byte;
}Std_Bit8Byte;

typedef union
{
	Std_Bit16Flag  bits ;
	unsigned int word;
}Std_Bit16Word;

/*
typedef struct {
	void (*open)(void *pData);
	void (*release)(void *pData);
	Std_ReturnType (*read)(void *pData);
	Std_ReturnType (*write)(void *pData);
	Std_ReturnType (*ioctl)(uint8 Cmd,void *pData);
}Dr_OpertonType;
*/

/***************************************************************************/
/* External Declarations                                                   */
/***************************************************************************/


/***************************************************************************/
/* Callback functions                                                      */
/***************************************************************************/

#endif	

