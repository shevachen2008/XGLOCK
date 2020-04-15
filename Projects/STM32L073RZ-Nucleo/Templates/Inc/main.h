/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_conf.h"
#include "stm32_port.h"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#include "type_define.h"
#include "I2c.h"

#include "type_define.h"
#include "Hardware.h"

#include "Rtc.h"  
#include "Flash.h"
#include "motor.h"
#include "Std_Types.h" 
#include "Sys_GenFun.h"
#include "sys.h"
#include "Encpt.h"
#include "Access.h"
#include "LockKey.h"
#include "bc26.h"
#include "Comble.h"
#include "ComNB.h"
#include "ADC.h"
#include "Pwr_stop.h"
#include "Agtest.h"

extern U8  ticks2ms;


#define NBAT_BUF_LEN   512
static char NBAT_BUFFER[NBAT_BUF_LEN];
#define NBAT_SEND(FORMAT,...) {\
    memset(NBAT_BUFFER, 0, NBAT_BUF_LEN);\
    sprintf(NBAT_BUFFER,FORMAT,##__VA_ARGS__); \
        HAL_UART_Transmit(&UART1_Handle, (uint8_t*)(NBAT_BUFFER), strlen((const char *)(NBAT_BUFFER)),2);\
}


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
