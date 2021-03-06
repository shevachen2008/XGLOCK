/****************************************************************************************
文件:   I2c.h
说明:
****************************************************************************************/
#ifndef _I2C_H__   //防重包含.
#define _I2C_H__
/***************************************************************************************/
#if 1
/******************************************************************************
**********************Macro definition*******************************************
******************************************************************************/

/******************************************************************************
**********************Type statement*******************************************
******************************************************************************/

typedef uint8 Std_ReturnType;
typedef Std_ReturnType I2c_Soft_ResultType;

/******************************************************************************
**********************Variable definition*******************************************
******************************************************************************/



/******************************************************************************
**********************Function declared*******************************************
******************************************************************************/
extern void I2C_SOFT_Init(void);
extern Std_ReturnType I2cWriteBytes(unsigned char uDevice, unsigned char * puBuf, unsigned char uNum);
extern Std_ReturnType I2cReadBytes(unsigned char uDevice, unsigned char * puBuf, unsigned char uNum);

extern void Line_1A_WT588S (U8 ddata);


#endif

/***************************************************************************************/
#endif
/****************************************************************************************
文件结束
****************************************************************************************/

