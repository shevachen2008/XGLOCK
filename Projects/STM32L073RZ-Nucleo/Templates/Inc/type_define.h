/************************************************************
************************************************************/
#ifndef _TYPE_DEFINE_H_
#define _TYPE_DEFINE_H_

#define EI()            do{__enable_irq();}while(0)
#define DI()            do{__disable_irq();}while(0)
#define nop()           do{__no_operation();}while(0)

#define TRUE     1
#define FALSE    0

#define STD_TRUE  0x01
#define STD_FALSE  0x00

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


typedef signed char BOOL;
typedef volatile signed char VBOOL;


typedef signed long  s32;
typedef signed short s16;
typedef signed char  s8;

typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef unsigned char  INT8U;
typedef unsigned short INT16U;
typedef unsigned long  INT32U;

typedef long  int32;		


typedef signed long  const sc32;  /* Read Only */
typedef signed short const sc16;  /* Read Only */
typedef signed char  const sc8;   /* Read Only */

typedef volatile signed long  vs32;
typedef volatile signed short vs16;
typedef volatile signed char  vs8;

typedef volatile signed long  const vsc32;  /* Read Only */
typedef volatile signed short const vsc16;  /* Read Only */
typedef volatile signed char  const vsc8;   /* Read Only */

typedef unsigned char       uint8;
typedef unsigned int      	uint16;
typedef unsigned long       uint32;
typedef long long           uint64;


typedef unsigned char  	    uCHAR;
typedef unsigned char  	    BYTE;
typedef signed   char  		sCHAR;

typedef unsigned int 		WORD;
typedef signed   short 		sWORD;

typedef unsigned long  	    uDWORD;
typedef signed   long  		sDWORD;
typedef unsigned long		DWORD;

typedef void 				VOID;
typedef unsigned long  	    LONG;


typedef unsigned long  const uc32;  /* Read Only */
typedef unsigned short const uc16;  /* Read Only */
typedef unsigned char  const uc8;   /* Read Only */

typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile unsigned long  const vuc32;  /* Read Only */
typedef volatile unsigned short const vuc16;  /* Read Only */
typedef volatile unsigned char  const vuc8;   /* Read Only */

typedef unsigned char U8;
typedef unsigned short  U16;
typedef unsigned long U32;
typedef long long     U64;

typedef signed char   S8;
typedef signed short    S16;
typedef signed long   S32;

#define BMSK(N)      (1U<<(N))
#define BSET(X,N)    (X)|=BMSK(N)
#define BCLR(X,N)    (X)&=(BMSK(N)^-1)
#define BCPL(X,N)    (X)^=BMSK(N)
#define BTST(X,N)    ((X)&BMSK(N))

#define isNULL(this) (NULL == this)
#define tdSUCCESS    (1)
#define tdFAILTURE    (-1)

#define C64HEX2BYTESMSB(pBuf, datas) \
				*(pBuf + 0) = (U8)((datas) >> 56); \
				*(pBuf + 1) = (U8)((datas) >> 48); \
				*(pBuf + 2) = (U8)((datas) >> 40); \
				*(pBuf + 3) = (U8)((datas) >> 32); \
	      *(pBuf + 4) = (U8)((datas) >> 24); \
        *(pBuf + 5) = (U8)((datas) >> 16); \
        *(pBuf + 6) = (U8)((datas) >> 8); \
        *(pBuf + 7) = (U8)((datas) >> 0)


#define C32HEX2BYTESMSB(pBuf, datas) \
	       *(pBuf + 0) = (U8)((datas) >> 24); \
        *(pBuf + 1) = (U8)((datas) >> 16); \
        *(pBuf + 2) = (U8)((datas) >> 8); \
        *(pBuf + 3) = (U8)((datas) >> 0)
        
#define C32HEX2BYTESLSB(pBuf, datas) \
	       *(pBuf + 0) = (U8)((datas) >> 0); \
        *(pBuf + 1) = (U8)((datas) >> 8); \
        *(pBuf + 2) = (U8)((datas) >> 16); \
        *(pBuf + 3) = (U8)((datas) >> 24)
        

#define C16HEX2BYTESLSB(pBuf, datas) \
	       *(pBuf + 0) = (U8)((datas) >> 0); \
        *(pBuf + 1) = (U8)((datas) >> 8)

#define C16HEX2BYTESMSB(pBuf, datas) \
	       *(pBuf + 0) = (U8)((datas) >> 8); \
        *(pBuf + 1) = (U8)((datas) >> 0)
          
#define C2BYTES2HEX(pBuf) \
	       (  ((U16)(*((pBuf) + 1)) << 8) \
	        | (U16)(*((pBuf) + 0)))

#define C4BYTES2HEX(pBuf) \
	       (  ((U32)(*((pBuf) + 0)) << 0) \
	        | ((U32)(*((pBuf) + 1)) << 8) \
	        | ((U32)(*((pBuf) + 2)) << 16) \
	        | ((U32)(*((pBuf) + 3)) << 24))

#define BUILD_YEAR(a) ((a&0xfc000000) >> 26)
#define BUILD_MONTH(a) ((a&0x3c00000) >> 22)
#define BUILD_DAY(a) ((a&0x3e0000) >> 17)
#define BUILD_HOUR(a) ((a&0x1f000) >> 12)
#define BUILD_MINUTE(a) ((a&0xfc0) >> 6)
#define BUILD_SECOND(a) (a&0x3f)

	        
#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)
	        
/***************************************************************************/
/* data types                                                              */
/***************************************************************************/
typedef uint8 Std_ReturnType;
typedef uint8 Std_ReWrType;
typedef uint8 Std_LeveType;
typedef uint8 Std_JudgeType;
typedef uint8 Eve_ReultType;
typedef uint8 Std_Pulse_CountType;



typedef struct {
	void (*open)(void *pData);
	void (*release)(void *pData);
	Std_ReturnType (*read)(void *pData);
	Std_ReturnType (*write)(void *pData);
	Std_ReturnType (*ioctl)(uint8 Cmd,void *pData);
}Dr_OpertonType;

//常用的枚举类型.

typedef enum
{
    ANSWER_YES=0,
    ANSWER_NO,
}ANSWER;

typedef enum
{
    SWITCH_ON=0,
    SWITCH_OFF,
}SWITCH;

typedef enum    //操作类.
{
    OP_SUCCESS=0,
    OP_FAIL,
}OP;

typedef enum    //执行类.
{
    EXE_SUCCESS=0,
    EXE_FAIL,
    EXE_PROGRESS,
}EXE;


#define	MAKE_WORD(v1,v2)	(((T16U)(v1)<<8) + (T16U)(v2))

#define	HIBYTE(v1)	((U8)((v1)>>8))   
#define	LOBYTE(v1)	((U8)((v1)&0xff)) 
#define NUM_ELEMENTS(array) (sizeof(array) / sizeof(array[0]))


#endif      /*_TYPE_DEFINE_H_*/


