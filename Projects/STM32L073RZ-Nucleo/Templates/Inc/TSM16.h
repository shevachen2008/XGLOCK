#ifndef __TSM16_H__
#define __TSM16_H__

/* Private typedef -----------------------------------------------------------*/

typedef signed long  TSM_S32_t;
typedef signed short TSM_S16_t;
typedef signed char  TSM_S8_t;
typedef unsigned long  TSM_U32_t;
typedef unsigned short TSM_U16_t;
typedef unsigned char  TSM_U8_t;

typedef enum
{
    TSM_SUCCESS = 0x0,
    TSM_FAILURE = 1,
}TSM_RetVal_e;

typedef enum
{
     TSM_TRUE=0,
     TSM16_FALSE=1,
}TSM_BOOL;

#define TSM16_ADDR		    0xD0
#define TSM16_RST_PIN		GPIO_PIN_6
#define TSM16_IRQ_PIN       GPIO_PIN_8
#define HALL_IRQ_PIN        GPIO_PIN_12              // 与串口唤醒中断复用  串口中断只唤醒 
#define IRQ_GPIO_PORT       GPIOC

extern uint8_t  keymsg;
extern uint8_t  leddisflag;
extern uint8_t  keyflag;

extern uint8_t  LowPowerCnt;
extern uint8_t  LowPowerCntFlag;

extern void TSM16_Init(void);
extern void TSM16_Check(void);
extern void TSM16_GPIO_EXTI_Callback(void);
extern void TSM16_INTO_SLEEP(void);

#endif
