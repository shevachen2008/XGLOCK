/***************************************************************
 * Copyright (C), 2019-2029, Rently
 * File name    : Hardware.c
 * Author       : sheva
 * Version      : V1.0.0
 * Date         : 2019-07-24
 * Description  : 
 * Others       : 
***************************************************************/

#include "main.h"


/**
 The following definitions relate to time.  System Clock, 
 Time base timer ,Delay timer
***************************************************************/

static void SystemClock_Config(void)
#if 1
  {
  	RCC_OscInitTypeDef RCC_OscInitStruct;
  	RCC_ClkInitTypeDef RCC_ClkInitStruct;
  
  	/* Enable Power Control clock */
  	__HAL_RCC_PWR_CLK_ENABLE();
  
  	/* The voltage scaling allows optimizing the power consumption when the device is 
  	clocked below the maximum system frequency, to update the voltage scaling value 
  	regarding system frequency refer to product datasheet.  */
  	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  
  	/* Disable Power Control clock */
  	__HAL_RCC_PWR_CLK_DISABLE();	  
  
  	/**Initializes the CPU, AHB and APB busses clocks 
  	*/
  	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  // 外部晶振，8MHz
  	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;	       // 4倍频，得到16MHz主时钟
  	RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
  	HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  	/**Initializes the CPU, AHB and APB busses clocks 
  	*/
  	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
  															 |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  	RCC_ClkInitStruct.SYSCLKSource =	RCC_SYSCLKSOURCE_PLLCLK;
  	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

		/*
		HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
		HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
		*/
		
  }
#endif
void delay_us(u16 nus)     
  {
	u32 temp;
	SysTick->LOAD = 20*nus;
	SysTick->VAL = 0X00;//清空计数器
	SysTick->CTRL = 0X01;//使能，减到零是无动作，采用外部时钟源
	do
	{
		temp=SysTick->CTRL;//读取当前倒计数值
	}while((temp&0x01)&&(!(temp&(1<<16))));//等待时间到达
	SysTick->CTRL=0x00; //关闭计数器
	SysTick->VAL =0X00; //清空计数器
  }

  
void delay_100us(u16 nus)     
  {
	u32 temp;
	SysTick->LOAD = 196*nus;
	SysTick->VAL = 0X00; 
	SysTick->CTRL = 0X01; 
	do
	{
		temp=SysTick->CTRL; 
	}while((temp&0x01)&&(!(temp&(1<<16)))); 
	SysTick->CTRL=0x00;  
	SysTick->VAL =0X00;  
  }
  
void delay_ms(u16 nms)
 {
	u32 temp;
	SysTick->LOAD = 2000*nms;
	SysTick->VAL = 0X00;	 
	SysTick->CTRL = 0X01;  
	do
	{
		temp = SysTick->CTRL;	 
	}while((temp&0x01)&&(!(temp&(1<<16))));   
	SysTick->CTRL = 0x00;   
	SysTick->VAL = 0X00;	  
 }
  
TIM_HandleTypeDef htim2;
void HAL_TIM_Base_MspInit ( TIM_HandleTypeDef* htim_base )
  {
	if ( htim_base->Instance == TIM2 )
	{
		__HAL_RCC_TIM2_CLK_ENABLE();

		HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ ( TIM2_IRQn );
	}
  }
  
/* TIM2 init function */
static void  TIM2_Init(void)				// 100US ONCE
  {
	//  TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 15;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 1000;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim2);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

	HAL_TIM_Base_Start_IT(&htim2);
  }




/*******************************************************************************
* Function Name  : HARDWARE_SetupGPIO
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void HARDWARE_SetupGPIO(void)	
  {
	GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();	
    __HAL_RCC_GPIOB_CLK_ENABLE();	
    __HAL_RCC_GPIOC_CLK_ENABLE();	


	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_8|GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10|GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull 	= GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  }




/*******************************************************************************
串口驱动 与 数据打印的相关配置

*******************************************************************************/




UART_HandleTypeDef UART1_Handle;  
U8    UART1_RXBuffer[512];     
U8    UART1_RXcnt; 
U8    UART1_RXFlag ; 

#define __HAL_RCC_USART1_CLK_DISABLE()   CLEAR_BIT(RCC->APB2ENR,  (RCC_APB2ENR_USART1EN))


/*******************************************************************************
* Function Name  : SetupUSUART1
* Description    : to NB-IOT
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HARDWARE_SetupUSART1( void )
  {
    GPIO_InitTypeDef GPIO_InitStruct;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
		
		__HAL_RCC_USART1_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
      PA10     ------> USART1_TX
      PA9    ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_Pin_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
   /* USART1 configuration --------------------*/  
  
    UART1_Handle.Instance = USART1;                      //  指定串口
    UART1_Handle.Init.BaudRate = 115200;                  // 波特率
    UART1_Handle.Init.WordLength = UART_WORDLENGTH_8B;   // 字长为8位数据格式
    UART1_Handle.Init.StopBits = UART_STOPBITS_1;        //  1个停止位
    UART1_Handle.Init.Parity = UART_PARITY_NONE;         // 无奇偶校验位
    UART1_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;   // 无硬件流控
    UART1_Handle.Init.Mode = UART_MODE_TX_RX;            // 收发模式

    HAL_UART_DeInit(&UART1_Handle);
	  HAL_UART_Init(&UART1_Handle);
  
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  
    __HAL_UART_ENABLE_IT(&UART1_Handle, UART_IT_RXNE);
	  __HAL_UART_ENABLE_IT(&UART1_Handle,UART_IT_IDLE);
  }


void USART1_IRQHandler(void)
  {
    if ((__HAL_UART_GET_FLAG(&UART1_Handle,UART_FLAG_RXNE)!=RESET))                                                     
      {                     
        RxBuffer[ RxCounter++ ] = (uint8_t)(UART1_Handle.Instance->RDR&(uint8_t)0x00FF);                                                              
	    } 
    if ((__HAL_UART_GET_FLAG(&UART1_Handle,UART_FLAG_IDLE)!=RESET))  
      {
         USART1->ICR = USART_ICR_IDLECF;
		 		 UART1_RXFlag = 1;
      }
  }

#if 0
UART_HandleTypeDef UART2_Handle;  
U8    UART2_RXBuffer[512];     
U8    UART2_RXcnt; 
U8    UART2_RXFlag ; 


/*******************************************************************************
* Function Name  : SetupUSUART2
* Description    : to NB-IOT
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HARDWARE_SetupUSART2( void )
  {
    GPIO_InitTypeDef GPIO_InitStruct;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
		
		__HAL_RCC_USART2_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
      PA2     ------> USART1_TX
      PA3    ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_Pin_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
   /* USART1 configuration --------------------*/  
  
    UART2_Handle.Instance = USART2;                      //  指定串口
    UART2_Handle.Init.BaudRate = 115200;                  // 波特率
    UART2_Handle.Init.WordLength = UART_WORDLENGTH_8B;   // 字长为8位数据格式
    UART2_Handle.Init.StopBits = UART_STOPBITS_1;        //  1个停止位
    UART2_Handle.Init.Parity = UART_PARITY_NONE;         // 无奇偶校验位
    UART2_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;   // 无硬件流控
    UART2_Handle.Init.Mode = UART_MODE_TX_RX;            // 收发模式

    HAL_UART_DeInit(&UART2_Handle);
	  HAL_UART_Init(&UART2_Handle);
  
    HAL_NVIC_SetPriority(USART2_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  
    __HAL_UART_ENABLE_IT(&UART2_Handle, UART_IT_RXNE);
	  __HAL_UART_ENABLE_IT(&UART2_Handle,UART_IT_IDLE);
  }



void USART2_IRQHandler(void)
  {
    if ((__HAL_UART_GET_FLAG(&UART2_Handle,UART_FLAG_RXNE)!=RESET))                                                     
      {                     
        UART2_RXBuffer[ UART2_RXcnt++ ] = (uint8_t)(UART2_Handle.Instance->RDR&(uint8_t)0x00FF);                                                              
	    } 
    if ((__HAL_UART_GET_FLAG(&UART2_Handle,UART_FLAG_IDLE)!=RESET))  
      {
         USART2->ICR = USART_ICR_IDLECF;
		 		 UART2_RXFlag = 1;
      }
  }
#endif


UART_HandleTypeDef  UART4_Handle;  
U8    UART4_RXBuffer[100];     
U8    UART4_RXcnt; 
U8    UART4_RXFlag ; 



void Disable_USART4IRQ(void)
{
  __HAL_RCC_USART4_CLK_DISABLE();

	HAL_NVIC_DisableIRQ(USART4_5_IRQn);

	__HAL_UART_DISABLE_IT(&UART4_Handle, UART_IT_RXNE);
	__HAL_UART_DISABLE_IT(&UART4_Handle,UART_IT_IDLE);
}



/*******************************************************************************
* Function Name  : SetupUSUART4
* Description    : to 52832
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HARDWARE_SetupUSART4( void )
  {
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Enable GPIO clock */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* Enable USART clock */
	__HAL_RCC_USART4_CLK_ENABLE();

	/**USART1 GPIO Configuration	
	PA0	   ------> USART4_TX
	PA1    ------> USART4_RX 
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_Pin_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF6_USART4;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* USART4 configuration --------------------*/	

	UART4_Handle.Instance = USART4; 					 //  指定串口
	UART4_Handle.Init.BaudRate = 115200;				  // 波特率
	UART4_Handle.Init.WordLength = UART_WORDLENGTH_8B;	 // 字长为8位数据格式
	UART4_Handle.Init.StopBits = UART_STOPBITS_1;		 //  1个停止位
	UART4_Handle.Init.Parity = UART_PARITY_NONE;		 // 无奇偶校验位
	UART4_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;	 // 无硬件流控
	UART4_Handle.Init.Mode = UART_MODE_TX_RX;			 // 收发模式


	HAL_UART_DeInit(&UART4_Handle);
	HAL_UART_Init(&UART4_Handle);

	HAL_NVIC_SetPriority(USART4_5_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(USART4_5_IRQn);

	__HAL_UART_ENABLE_IT(&UART4_Handle, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&UART4_Handle,UART_IT_IDLE);
 }



void USART4_5_IRQHandler(void)
  {
    if ((__HAL_UART_GET_FLAG(&UART4_Handle,UART_FLAG_RXNE)!=RESET))                                                     
      {                     
        UART4_RXBuffer[ UART4_RXcnt++ ] = (uint8_t)(UART4_Handle.Instance->RDR&(uint8_t)0x00FF);                                                              
	    } 
    if ((__HAL_UART_GET_FLAG(&UART4_Handle,UART_FLAG_IDLE)!=RESET))  
      {
         USART4->ICR = USART_ICR_IDLECF;
		 		 UART4_RXFlag = 1;
      }
  }


/*******************************************************************************
* Function Name  : SetupUSUART5
* Description    : to printf
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
UART_HandleTypeDef  UART5_Handle;  


int fputc(int ch, FILE *f)
{      
	uint8_t temp[1]={ch};
	HAL_UART_Transmit(&UART5_Handle,temp,1,2);
	return ch;
}

#if 0
static void HARDWARE_SetupUSART5( void )
  {
		GPIO_InitTypeDef GPIO_InitStruct;

		/* Enable GPIO clock */
		__HAL_RCC_GPIOC_CLK_ENABLE();

		/* Enable USART clock */
		__HAL_RCC_USART5_CLK_ENABLE();

		/**USART1 GPIO Configuration	
		PC12	   ------> USART5_TX
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		
    GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF6_USART5;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		/* USART5 configuration --------------------*/	

		UART5_Handle.Instance = USART5; 					 //  指定串口
		UART5_Handle.Init.BaudRate = 115200;				  // 波特率
		UART5_Handle.Init.WordLength = UART_WORDLENGTH_8B;	 // 字长为8位数据格式
		UART5_Handle.Init.StopBits = UART_STOPBITS_1;		 //  1个停止位
		UART5_Handle.Init.Parity = UART_PARITY_NONE;		 // 无奇偶校验位
		UART5_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;	 // 无硬件流控
		UART5_Handle.Init.Mode = UART_MODE_TX;			 // 收发模式


		HAL_UART_DeInit(&UART5_Handle);
		HAL_UART_Init(&UART5_Handle);
 }
#endif



/*******************************************************************************
* Function Name  : SetupSPI1
* Description    : FLASH SPI.
*******************************************************************************/
SPI_HandleTypeDef  FLASH_SPI;

static void HARDWARE_SetupSPI1(void)
  {
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE(); 
	/**SPI1 GPIO Configuration	 
	PA15	------> SPI1_NSS   
	PB3	    ------> SPI1_SCK
	PB4	    ------> SPI1_MISO
	PB5	    ------> SPI1_MOSI 
	*/
	
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);	

	GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);//配置SPI的数据线和时钟线
	/* SPI Config */
	FLASH_SPI.Instance = SPI1;
	FLASH_SPI.Init.Mode = SPI_MODE_MASTER;//主模式
	FLASH_SPI.Init.Direction = SPI_DIRECTION_2LINES;//全双工
	FLASH_SPI.Init.DataSize = SPI_DATASIZE_8BIT;//数据位为8位
	FLASH_SPI.Init.CLKPolarity = SPI_POLARITY_LOW;//CPOL=0,low
	FLASH_SPI.Init.CLKPhase = SPI_PHASE_1EDGE;//CPHA为数据线的第一个变化沿
	FLASH_SPI.Init.NSS = SPI_NSS_SOFT;//软件控制NSS
	FLASH_SPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;//2		  分频，16M/4=4MHz
	FLASH_SPI.Init.FirstBit = SPI_FIRSTBIT_MSB;  //最高位先发送
	FLASH_SPI.Init.TIMode = SPI_TIMODE_DISABLE;  //TIMODE模式关闭
	FLASH_SPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;  //CRC关闭
	FLASH_SPI.Init.CRCPolynomial = 10;	//默认值，无效

	FLASH_SPI.State = HAL_SPI_STATE_RESET;
	HAL_SPI_Init(&FLASH_SPI);
  }

#if 0	
SPI_HandleTypeDef  VOICE_SPI;
/*******************************************************************************
* Function Name  : SetupSPI2
* Description    : VOICDCHIP  
*******************************************************************************/
static void HARDWARE_SetupSPI2(void)
  {
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE(); 
	/**SPI1 GPIO Configuration	 
	PB12	------> SPI2_NSS   
	PB13	------> SPI2_SCK
	PB14	------> SPI2_MISO
	PB15	------> SPI2_MOSI 
	*/
	
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);	

	GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF0_SPI2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);//配置SPI的数据线和时钟线
	/* SPI Config */
	VOICE_SPI.Instance = SPI2;
	VOICE_SPI.Init.Mode = SPI_MODE_MASTER;//主模式
	VOICE_SPI.Init.Direction = SPI_DIRECTION_2LINES;//全双工
	VOICE_SPI.Init.DataSize = SPI_DATASIZE_8BIT;//数据位为8位
	VOICE_SPI.Init.CLKPolarity = SPI_POLARITY_LOW;//CPOL=0,low
	VOICE_SPI.Init.CLKPhase = SPI_PHASE_1EDGE;//CPHA为数据线的第一个变化沿
	VOICE_SPI.Init.NSS = SPI_NSS_SOFT;//软件控制NSS
	VOICE_SPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;//2		  分频，16M/4=4MHz
	VOICE_SPI.Init.FirstBit = SPI_FIRSTBIT_MSB;  //最高位先发送
	VOICE_SPI.Init.TIMode = SPI_TIMODE_DISABLE;  //TIMODE模式关闭
	VOICE_SPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;  //CRC关闭
	VOICE_SPI.Init.CRCPolynomial = 10;	//默认值，无效

	VOICE_SPI.State = HAL_SPI_STATE_RESET;
	HAL_SPI_Init(&VOICE_SPI);
  }
#endif
/*******************************************************************************
* Function Name  : ADC
* Description    : ADC  
*******************************************************************************/
ADC_HandleTypeDef  Power_adc;

static void ADCx_MspInit(ADC_HandleTypeDef *hadc)
  {
    GPIO_InitTypeDef  GPIO_Initure;
    
    /*** Configure the GPIOs ***/  
    /* Enable GPIO clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();            //开启GPIOA时钟
    
    /* Configure ADC1 Channel8 as analog input */
    GPIO_Initure.Pin = GPIO_PIN_4;
    GPIO_Initure.Mode = GPIO_MODE_ANALOG;
    GPIO_Initure.Pull   = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
  
    /*** Configure the ADC peripheral ***/ 
    /* Enable ADC clock */
    __HAL_RCC_ADC1_CLK_ENABLE();
  }

static HAL_StatusTypeDef ADCx_Init(void)
  {
	/* Set ADC instance */
	Power_adc.Instance = ADC1;

	if(HAL_ADC_GetState(&Power_adc) == HAL_ADC_STATE_RESET)
	  {
		/* ADC Config */
		Power_adc.Instance = ADC1;
		Power_adc.Init.OversamplingMode 	 = DISABLE;
		Power_adc.Init.ClockPrescaler		 = ADC_CLOCK_SYNC_PCLK_DIV2; /* (must not exceed 16MHz) */
		Power_adc.Init.LowPowerAutoPowerOff  = DISABLE;
		Power_adc.Init.LowPowerFrequencyMode = ENABLE;
		Power_adc.Init.LowPowerAutoWait 	 = ENABLE;
		Power_adc.Init.Resolution			 = ADC_RESOLUTION_12B;
		Power_adc.Init.SamplingTime 		 = ADC_SAMPLETIME_1CYCLE_5;
		Power_adc.Init.ScanConvMode 		 = ADC_SCAN_DIRECTION_FORWARD;
		Power_adc.Init.DataAlign			 = ADC_DATAALIGN_RIGHT;
		Power_adc.Init.ContinuousConvMode	 = DISABLE;
		Power_adc.Init.DiscontinuousConvMode = DISABLE;
		Power_adc.Init.ExternalTrigConv 	  = ADC_SOFTWARE_START; 		   /* Trig of conversion start done manually by software, without external event */
		Power_adc.Init.ExternalTrigConvEdge   = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because trig by software start */
		Power_adc.Init.EOCSelection 		  = ADC_EOC_SEQ_CONV;
		Power_adc.Init.DMAContinuousRequests  = DISABLE;

		/* Initialize MSP related to ADC */
		ADCx_MspInit(&Power_adc);

		/* Initialize ADC */
		if (HAL_ADC_Init(&Power_adc) != HAL_OK)
		{
		  return HAL_ERROR;
		}
		if (HAL_ADCEx_Calibration_Start(&Power_adc,ADC_SINGLE_ENDED) != HAL_OK)
		{
		  return HAL_ERROR;
		}
	  }
	return HAL_OK;
  }


static ADC_ChannelConfTypeDef sConfig;
uint8_t HARDWARE_SetupADC(void)
{
  if (ADCx_Init() != HAL_OK)
  {
    return (uint8_t) HAL_ERROR; 
  }
	
	
  /* Select Channel 8 to be converted */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;

  /* Return Joystick initialization status */
  return (uint8_t) HAL_ADC_ConfigChannel(&Power_adc, &sConfig);
}


IWDG_HandleTypeDef hiwdg;

void MX_IWDG_Init(uint8_t prv ,uint16_t rlv)
{
 
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = prv;
  hiwdg.Init.Reload = rlv;
  HAL_IWDG_Init(&hiwdg);
}

void System_HardwareInit(void)
{
	SystemClock_Config();
	
	TIM2_Init();
	I2C_SOFT_Init();
	HARDWARE_SetupGPIO();
	HARDWARE_SetupUSART1();    // nb
	// HARDWARE_SetupUSART2();
	HARDWARE_SetupUSART4();    // BLE 
  // HARDWARE_SetupUSART5();
	
	HARDWARE_SetupSPI1();
	//HARDWARE_SetupSPI2();
  HARDWARE_SetupADC();
	// MX_IWDG_Init(IWDG_PRESCALER_64,6250);

	WT588POW_OPEN();
	P_DATA_OUT_HIGH();
	PWRKEYOFF(); 
	RESETOFF();
	NBINTLOW(); 
	HallPower_Open();
	GPIO_SET_B8(1);     // RTC BAT
	PwrStopCnt = 5;
}


