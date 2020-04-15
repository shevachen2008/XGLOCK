#ifndef _HARDWARE_H_
#define _HARDWARE_H_


#define WT588POW_OPEN()     GPIO_SET_C4(0)      
#define WT588POW_CLOSE()    GPIO_SET_C4(1) 

#define  HallPower_Open()     GPIO_SET_B9(1)
#define  HallPower_Close()    GPIO_SET_B9(0)

#define P_DATA_OUT_LOW()	   GPIO_SET_A6(0)
#define P_DATA_OUT_HIGH()	   GPIO_SET_A6(1)


/**********NB IO ************************/
#define  PWRKEYON()       GPIO_SET_A11(1)   
#define  PWRKEYOFF()      GPIO_SET_A11(0)

#define  RESETON()        GPIO_SET_A12(1)
#define  RESETOFF()       GPIO_SET_A12(0)

#define  NBINTHIGH()      GPIO_SET_C7(1)
#define  NBINTLOW()       GPIO_SET_C7(0)




extern void HARDWARE_SetupUSART1( void );
extern void System_HardwareInit(void);
extern void delay_us (u16 nus);
extern void delay_100us (u16 nus);
extern void delay_ms(u16 nms);

extern void HARDWARE_SetupUSART4(void);
extern void Disable_USART4IRQ(void);


extern TIM_HandleTypeDef htim2;
extern SPI_HandleTypeDef  FLASH_SPI;

extern UART_HandleTypeDef  UART1_Handle;  
extern UART_HandleTypeDef  UART4_Handle;  
extern UART_HandleTypeDef  UART5_Handle;  

extern U8    UART1_RXBuffer[512];     
extern U8    UART1_RXcnt; 
extern U8    UART1_RXFlag ; 

extern U8    UART2_RXBuffer[512];     
extern U8    UART2_RXcnt; 
extern U8    UART2_RXFlag ; 

extern U8    UART4_RXBuffer[100];     
extern U8    UART4_RXcnt; 
extern U8    UART4_RXFlag ; 

extern IWDG_HandleTypeDef hiwdg;
extern ADC_HandleTypeDef  Power_adc;

#endif


