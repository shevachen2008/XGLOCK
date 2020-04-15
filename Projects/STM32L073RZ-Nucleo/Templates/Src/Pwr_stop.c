/****************************************************************************************
文件:   Pwr_stop.c
说明:   低功耗 STOP模式，RTC，外部中断都能唤醒
****************************************************************************************/
//头文件.

/*	RTC Wakeup Interrupt Generation:
	Wakeup Time Base = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE or LSI))
	Wakeup Time = Wakeup Time Base * WakeUpCounter 
	= (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE or LSI)) * WakeUpCounter
		==> WakeUpCounter = Wakeup Time / Wakeup Time Base

	To configure the wake up timer to 4s the WakeUpCounter is set to 0x1FFF:
	RTC_WAKEUPCLOCK_RTCCLK_DIV = RTCCLK_Div16 = 16 
	Wakeup Time Base = 16 /(~39.000KHz) = ~0,410 ms
	Wakeup Time = ~4s = 0,410ms  * WakeUpCounter
		==> WakeUpCounter = ~4s/0,410ms = 9750 = 0x2616 */
		

#include "main.h"

RTC_HandleTypeDef RTCHandle;

#define RTC_ASYNCH_PREDIV    0x7C
#define RTC_SYNCH_PREDIV     0x0127


#define HALL_LOCK     1;
#define HALL_UNLOCK   2;
#define HALL_MIDDLE   3;

U8  CheckRight; 
U8  CheckLeft; 
U8  CheckVertical; 


U8  PwrStopCnt;
U8  PwrStopStatus;
U8  PwrDoorSwStatus; 
U8  PwrAutoLockTimeRead; 
U8  PwrAutoLockTimeCnt; 


void PwrStopTimer(void)
{
  if ( PwrStopCnt > 0 )
  	{
  	   if ( --PwrStopCnt == 0 )
  	   	{
  	   	  DoorSw_Unlogflag = 0;
					enter_stop_mode();
  	   	}
  	}
}


void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*##-1- Configue the RTC clock soucre ######################################*/
  /* -a- Enable LSI Oscillator */
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1);
  }

  /* -b- Select LSI as RTC clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
    while(1);
  }

  /*##-2- Enable the RTC peripheral Clock ####################################*/
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();
  
  /*##-3- Configure the NVIC for RTC Alarm ###################################*/
  HAL_NVIC_SetPriority(RTC_IRQn, 0x0, 0);
  HAL_NVIC_EnableIRQ(RTC_IRQn);
}


void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_RTC_DISABLE();
}

void SystemPower_Config(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
    __HAL_RCC_PWR_CLK_ENABLE();  
		HAL_PWREx_EnableUltraLowPower();
		HAL_PWREx_EnableFastWakeUp();
		
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();
		__HAL_RCC_GPIOH_CLK_ENABLE();
		__HAL_RCC_GPIOE_CLK_ENABLE();

		GPIO_InitStructure.Pin = GPIO_PIN_All;
		GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStructure.Pull = GPIO_NOPULL;

		HAL_GPIO_Init(GPIOA, &GPIO_InitStructure); 
		HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
		HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
		HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
		HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
		HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

		__HAL_RCC_GPIOA_CLK_DISABLE();
		__HAL_RCC_GPIOB_CLK_DISABLE();
		__HAL_RCC_GPIOC_CLK_DISABLE();
		__HAL_RCC_GPIOD_CLK_DISABLE();
		__HAL_RCC_GPIOH_CLK_DISABLE();
		__HAL_RCC_GPIOE_CLK_DISABLE();

  /* Configure RTC */
  RTCHandle.Instance = RTC;
  RTCHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RTCHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RTCHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RTCHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RTCHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  HAL_RTC_Init(&RTCHandle);
}	

	
void sysClock_StopMode_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* Enable MSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.MSICalibrationValue=0x00;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
  
  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  
  /* Disable Power Control clock */
  __HAL_RCC_PWR_CLK_DISABLE();    
}

//  PA1 RX_UART4BLE
void Gpio_StopMode_Config(void)
  {
		GPIO_InitTypeDef GPIO_InitStruct;
		
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_1;   
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;  
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
	  HAL_NVIC_SetPriority( EXTI0_1_IRQn, 1, 1 );
    HAL_NVIC_EnableIRQ( EXTI0_1_IRQn );	
	}	


static void PwrHallGpio_Config(void)
  {
		GPIO_InitTypeDef GPIO_InitStruct;
		__HAL_RCC_GPIOB_CLK_ENABLE();

		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		HallPower_Open();
		
		GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_10|GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  
		
		HAL_Delay(5);
  }
static void PwrDoorSwGetGpio_Config(void)
  {
    GPIO_InitTypeDef GPIO_InitStruct;
		__HAL_RCC_GPIOA_CLK_ENABLE();

		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }

static void PwrGpio_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;
		__HAL_RCC_GPIOC_CLK_ENABLE();

		GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 
}

static void WakeUp_stop_RTC(void)
{
	if ( 1 == PwrStopStatus )  PwrStopStatus = 2;
	__HAL_RCC_RTC_DISABLE();
	RTCHandle.State = HAL_RTC_STATE_RESET;
	System_HardwareInit();
}

void PwrGpio_Scan(void)
{
  if ( 1 == NBconectFlag )
  	{
			if ( ++NBHeartBeatCnt >= 1660 )   //1660  150  
				{
				  NBHeartBeatCnt = 0;
				 	NBheartbeatFlag = 1;
					DoorSw_Unlogflag = 1;
				  WakeUp_stop_RTC();
					return;
				}
  	}

	PwrGpio_Config();
	if ( 0 == ReFactoryKey() || 0 == DoorSensorKey() || 1 == CutPin() )
		{
			WakeUp_stop_RTC();
			return;
		}
  
	if ( LockErrorFlag )
	 	{
		 if ( 0 == ComCol1GetData() || 0 == ComCol3GetData())
			 {
			   WakeUp_stop_RTC();
				 return;
			 }
	 	}

  if ( 0 != Motor_AutoLockTimer64ms )
   	{
			if ( ++PwrAutoLockTimeCnt >= PwrAutoLockTimeRead )
				{
					Motor_AutoLockTimer64ms = 5;
				  WakeUp_stop_RTC();
					return;
				}
   	}

	if ((Sys_CheckEnableDoorLock())&& (Sys_DisblePass()))
  	{
			PwrDoorSwGetGpio_Config();
			if ( 2 == PwrDoorSwStatus )
				{
					if ( DoorSwGetData() == 0)
						{
						  WakeUp_stop_RTC();
							return;
						}				
				}
			else if ( 1 == PwrDoorSwStatus  )
				{
					if ( DoorSwGetData() == 1)
						{
							WakeUp_stop_RTC();
							return;
						}						
				}
		}

	if ( 0x5A == Sys_PataCfg.CheckLR )		 // left open 
		{
			if ((CheckLeft != ComCol1GetData()) && ( CheckVertical != ComCol2GetData()))
				{
				  WakeUp_stop_RTC();
					return;
				}
			else
				{
				  enter_stop_RTC();
				}
		}
	else if ( 0xA5 == Sys_PataCfg.CheckLR )		 // Right open 
		{
			if ((CheckRight!= ComCol3GetData()) && ( CheckVertical != ComCol2GetData()))
				{
				  WakeUp_stop_RTC();
					return;
				} 
			else
				{
				  enter_stop_RTC();
				}
	 }
	else
	 {
	   enter_stop_RTC();
	 }
}

static U8 extiCnt;
void enter_stop_RTC(void)
{
  extiCnt = 2;
	sysClock_StopMode_Config();
	SystemPower_Config();
  Gpio_StopMode_Config();
	
	HAL_RTCEx_DeactivateWakeUpTimer(&RTCHandle);
	HAL_RTCEx_SetWakeUpTimer_IT(&RTCHandle, 0x130B, RTC_WAKEUPCLOCK_RTCCLK_DIV16);   //  0x1F8e  0x130B
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
                           //  唤醒之后程序从此开始运行
  PwrHallGpio_Config();    // 在这里处理不会引起程序跑飞
}


void enter_stop_mode(void)
  {
		CheckLeft = ComCol1GetData(); 
		CheckVertical = ComCol2GetData(); 
		CheckRight = ComCol3GetData(); 
		if ((Sys_CheckEnableDoorLock())&& (Sys_DisblePass()))
			{
				if ( DoorSwGetData() == 0)
					{
					  PwrDoorSwStatus = 1;    // LOW
					}
				else
					{
					  PwrDoorSwStatus = 2;    // HIGH
					}
			}

	  if ( 0 != Motor_AutoLockTimer64ms )
	  	{
	  	  PwrAutoLockTimeCnt = 0;
	  	  PwrAutoLockTimeRead = (U8)(Motor_AutoLockTimer64ms/150);
	  	}

		PwrStopStatus = 1;
		WT588POW_CLOSE();       // wt588 power close
		HallPower_Close();
    enter_stop_RTC();
  }


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
  {
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
		if (GPIO_Pin == GPIO_PIN_1)
			{
				if ( extiCnt > 0 )
			   	{
			   		if ( --extiCnt == 0 )
			   	  	{
								if ( 1 == PwrStopStatus )  PwrStopStatus = 2;
								
		            HAL_GPIO_DeInit(GPIOA,GPIO_PIN_1);  //串口重新初始化时HAL_UART_Init()
		                                             // 函数会判断if(huart->State == HAL_UART_STATE_RESET),
		                                             // 此时才会执行HAL_UART_MspInit(huart),配置串口IO口，这句话必须有！

							 __HAL_RCC_RTC_DISABLE();
							 RTCHandle.State = HAL_RTC_STATE_RESET;
								System_HardwareInit();
			   	  	}
				 }
      }
  }

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
  /* Clear Wake Up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
}


