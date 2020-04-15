/*********************************************************************************
*********************************************************************************/
#ifndef _STM32_PORT_H_
#define _STM32_PORT_H_

//#include "stm32l0xx_hal_gpio.h"
#if 1
#define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< Pin 0 selected */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /*!< Pin 1 selected */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /*!< Pin 7 selected */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< Pin 10 selected */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< Pin 11 selected */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< Pin 12 selected */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< Pin 14 selected */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< Pin 15 selected */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< All pins selected */
#endif

#define GPIO_SET_A0(x)        	  GPIOA->ODR=(GPIOA->ODR&~GPIO_Pin_0)  	| (x ? GPIO_Pin_0 : 0)
#define GPIO_SET_A1(x)        	  GPIOA->ODR=(GPIOA->ODR&~GPIO_Pin_1) 	| (x ? GPIO_Pin_1 : 0)
#define GPIO_SET_A2(x)        	  GPIOA->ODR=(GPIOA->ODR&~GPIO_Pin_2)  	| (x ? GPIO_Pin_2 : 0)
#define GPIO_SET_A3(x)        	  GPIOA->ODR=(GPIOA->ODR&~GPIO_Pin_3)  	| (x ? GPIO_Pin_3 : 0)
#define GPIO_SET_A4(x)        	  GPIOA->ODR=(GPIOA->ODR&~GPIO_Pin_4)  	| (x ? GPIO_Pin_4 : 0)
#define GPIO_SET_A5(x)        	  GPIOA->ODR=(GPIOA->ODR&~GPIO_Pin_5)  	| (x ? GPIO_Pin_5 : 0)
#define GPIO_SET_A6(x)        	  GPIOA->ODR=(GPIOA->ODR&~GPIO_Pin_6)  	| (x ? GPIO_Pin_6 : 0)
#define GPIO_SET_A7(x)        	  GPIOA->ODR=(GPIOA->ODR&~GPIO_Pin_7)  	| (x ? GPIO_Pin_7 : 0)
#define GPIO_SET_A8(x)        	  GPIOA->ODR=(GPIOA->ODR&~GPIO_Pin_8)  	| (x ? GPIO_Pin_8 : 0)
#define GPIO_SET_A9(x)        	  GPIOA->ODR=(GPIOA->ODR&~GPIO_Pin_9)  	| (x ? GPIO_Pin_9 : 0)
#define GPIO_SET_A10(x)        	  GPIOA->ODR=(GPIOA->ODR&~GPIO_Pin_10)  | (x ? GPIO_Pin_10 : 0)
#define GPIO_SET_A11(x)        	  GPIOA->ODR=(GPIOA->ODR&~GPIO_Pin_11)  | (x ? GPIO_Pin_11 : 0)
#define GPIO_SET_A12(x)        	  GPIOA->ODR=(GPIOA->ODR&~GPIO_Pin_12)  | (x ? GPIO_Pin_12 : 0)
#define GPIO_SET_A13(x)        	  GPIOA->ODR=(GPIOA->ODR&~GPIO_Pin_13)  | (x ? GPIO_Pin_13 : 0)
#define GPIO_SET_A14(x)        	  GPIOA->ODR=(GPIOA->ODR&~GPIO_Pin_14)  | (x ? GPIO_Pin_14 : 0)
#define GPIO_SET_A15(x)        	  GPIOA->ODR=(GPIOA->ODR&~GPIO_Pin_15)  | (x ? GPIO_Pin_15 : 0)

#define GPIO_SET_B0(x)        	  GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_0)  	| (x ? GPIO_Pin_0 : 0)
#define GPIO_SET_B1(x)        	  GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_1) 	| (x ? GPIO_Pin_1 : 0)
#define GPIO_SET_B2(x)        	  GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_2)  	| (x ? GPIO_Pin_2 : 0)
#define GPIO_SET_B3(x)        	  GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_3)  	| (x ? GPIO_Pin_3 : 0)
#define GPIO_SET_B4(x)        	  GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_4)  	| (x ? GPIO_Pin_4 : 0)
#define GPIO_SET_B5(x)        	  GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_5)  	| (x ? GPIO_Pin_5 : 0)
#define GPIO_SET_B6(x)        	  GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_6)  	| (x ? GPIO_Pin_6 : 0)
#define GPIO_SET_B7(x)        	  GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_7)  	| (x ? GPIO_Pin_7 : 0)
#define GPIO_SET_B8(x)        	  GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_8)  	| (x ? GPIO_Pin_8 : 0)
#define GPIO_SET_B9(x)        	  GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_9)  	| (x ? GPIO_Pin_9 : 0)
#define GPIO_SET_B10(x)        	  GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_10)  | (x ? GPIO_Pin_10 : 0)
#define GPIO_SET_B11(x)        	  GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_11)  | (x ? GPIO_Pin_11 : 0)
#define GPIO_SET_B12(x)        	  GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_12)  | (x ? GPIO_Pin_12 : 0)
#define GPIO_SET_B13(x)        	  GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_13)  | (x ? GPIO_Pin_13 : 0)
#define GPIO_SET_B14(x)        	  GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_14)  | (x ? GPIO_Pin_14 : 0)
#define GPIO_SET_B15(x)        	  GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_15)  | (x ? GPIO_Pin_15 : 0)

#define GPIO_SET_C0(x)        	  GPIOC->ODR=(GPIOC->ODR&~GPIO_Pin_0)  	| (x ? GPIO_Pin_0 : 0)
#define GPIO_SET_C1(x)        	  GPIOC->ODR=(GPIOC->ODR&~GPIO_Pin_1) 	| (x ? GPIO_Pin_1 : 0)
#define GPIO_SET_C2(x)        	  GPIOC->ODR=(GPIOC->ODR&~GPIO_Pin_2)  	| (x ? GPIO_Pin_2 : 0)
#define GPIO_SET_C3(x)        	  GPIOC->ODR=(GPIOC->ODR&~GPIO_Pin_3)  	| (x ? GPIO_Pin_3 : 0)
#define GPIO_SET_C4(x)        	  GPIOC->ODR=(GPIOC->ODR&~GPIO_Pin_4)  	| (x ? GPIO_Pin_4 : 0)
#define GPIO_SET_C5(x)        	  GPIOC->ODR=(GPIOC->ODR&~GPIO_Pin_5)  	| (x ? GPIO_Pin_5 : 0)
#define GPIO_SET_C6(x)        	  GPIOC->ODR=(GPIOC->ODR&~GPIO_Pin_6)  	| (x ? GPIO_Pin_6 : 0)
#define GPIO_SET_C7(x)        	  GPIOC->ODR=(GPIOC->ODR&~GPIO_Pin_7)  	| (x ? GPIO_Pin_7 : 0)
#define GPIO_SET_C8(x)        	  GPIOC->ODR=(GPIOC->ODR&~GPIO_Pin_8)  	| (x ? GPIO_Pin_8 : 0)
#define GPIO_SET_C9(x)        	  GPIOC->ODR=(GPIOC->ODR&~GPIO_Pin_9)  	| (x ? GPIO_Pin_9 : 0)
#define GPIO_SET_C10(x)        	  GPIOC->ODR=(GPIOC->ODR&~GPIO_Pin_10)  | (x ? GPIO_Pin_10 : 0)
#define GPIO_SET_C11(x)        	  GPIOC->ODR=(GPIOC->ODR&~GPIO_Pin_11)  | (x ? GPIO_Pin_11 : 0)
#define GPIO_SET_C12(x)        	  GPIOC->ODR=(GPIOC->ODR&~GPIO_Pin_12)  | (x ? GPIO_Pin_12 : 0)
#define GPIO_SET_C13(x)        	  GPIOC->ODR=(GPIOC->ODR&~GPIO_Pin_13)  | (x ? GPIO_Pin_13 : 0)
#define GPIO_SET_C14(x)        	  GPIOC->ODR=(GPIOC->ODR&~GPIO_Pin_14)  | (x ? GPIO_Pin_14 : 0)
#define GPIO_SET_C15(x)        	  GPIOC->ODR=(GPIOC->ODR&~GPIO_Pin_15)  | (x ? GPIO_Pin_15 : 0)

#define GPIO_SET_D0(x)        	  GPIOD->ODR=(GPIOD->ODR&~GPIO_Pin_0)  	| (x ? GPIO_Pin_0 : 0)
#define GPIO_SET_D1(x)        	  GPIOD->ODR=(GPIOD->ODR&~GPIO_Pin_1) 	| (x ? GPIO_Pin_1 : 0)
#define GPIO_SET_D2(x)        	  GPIOD->ODR=(GPIOD->ODR&~GPIO_Pin_2)  	| (x ? GPIO_Pin_2 : 0)
#define GPIO_SET_D3(x)        	  GPIOD->ODR=(GPIOD->ODR&~GPIO_Pin_3)  	| (x ? GPIO_Pin_3 : 0)
#define GPIO_SET_D4(x)        	  GPIOD->ODR=(GPIOD->ODR&~GPIO_Pin_4)  	| (x ? GPIO_Pin_4 : 0)
#define GPIO_SET_D5(x)        	  GPIOD->ODR=(GPIOD->ODR&~GPIO_Pin_5)  	| (x ? GPIO_Pin_5 : 0)
#define GPIO_SET_D6(x)        	  GPIOD->ODR=(GPIOD->ODR&~GPIO_Pin_6)  	| (x ? GPIO_Pin_6 : 0)
#define GPIO_SET_D7(x)        	  GPIOD->ODR=(GPIOD->ODR&~GPIO_Pin_7)  	| (x ? GPIO_Pin_7 : 0)
#define GPIO_SET_D8(x)        	  GPIOD->ODR=(GPIOD->ODR&~GPIO_Pin_8)  	| (x ? GPIO_Pin_8 : 0)
#define GPIO_SET_D9(x)        	  GPIOD->ODR=(GPIOD->ODR&~GPIO_Pin_9)  	| (x ? GPIO_Pin_9 : 0)
#define GPIO_SET_D10(x)        	  GPIOD->ODR=(GPIOD->ODR&~GPIO_Pin_10)  | (x ? GPIO_Pin_10 : 0)
#define GPIO_SET_D11(x)        	  GPIOD->ODR=(GPIOD->ODR&~GPIO_Pin_11)  | (x ? GPIO_Pin_11 : 0)
#define GPIO_SET_D12(x)        	  GPIOD->ODR=(GPIOD->ODR&~GPIO_Pin_12)  | (x ? GPIO_Pin_12 : 0)
#define GPIO_SET_D13(x)        	  GPIOD->ODR=(GPIOD->ODR&~GPIO_Pin_13)  | (x ? GPIO_Pin_13 : 0)
#define GPIO_SET_D14(x)        	  GPIOD->ODR=(GPIOD->ODR&~GPIO_Pin_14)  | (x ? GPIO_Pin_14 : 0)
#define GPIO_SET_D15(x)        	  GPIOD->ODR=(GPIOD->ODR&~GPIO_Pin_15)  | (x ? GPIO_Pin_15 : 0)


#define GPIO_SET_E0(x)        	  GPIOF->ODR=(GPIOF->ODR&~GPIO_Pin_0)  	| (x ? GPIO_Pin_0 : 0)
#define GPIO_SET_E1(x)        	  GPIOF->ODR=(GPIOF->ODR&~GPIO_Pin_1) 	| (x ? GPIO_Pin_1 : 0)
#define GPIO_SET_E2(x)        	  GPIOF->ODR=(GPIOF->ODR&~GPIO_Pin_2)  	| (x ? GPIO_Pin_2 : 0)
#define GPIO_SET_E3(x)        	  GPIOF->ODR=(GPIOF->ODR&~GPIO_Pin_3)  	| (x ? GPIO_Pin_3 : 0)
#define GPIO_SET_E4(x)        	  GPIOF->ODR=(GPIOF->ODR&~GPIO_Pin_4)  	| (x ? GPIO_Pin_4 : 0)
#define GPIO_SET_E5(x)        	  GPIOF->ODR=(GPIOF->ODR&~GPIO_Pin_5)  	| (x ? GPIO_Pin_5 : 0)
#define GPIO_SET_E6(x)        	  GPIOF->ODR=(GPIOF->ODR&~GPIO_Pin_6)  	| (x ? GPIO_Pin_6 : 0)
#define GPIO_SET_E7(x)        	  GPIOF->ODR=(GPIOF->ODR&~GPIO_Pin_7)  	| (x ? GPIO_Pin_7 : 0)
#define GPIO_SET_E8(x)        	  GPIOF->ODR=(GPIOF->ODR&~GPIO_Pin_8)  	| (x ? GPIO_Pin_8 : 0)
#define GPIO_SET_E9(x)        	  GPIOF->ODR=(GPIOF->ODR&~GPIO_Pin_9)  	| (x ? GPIO_Pin_9 : 0)
#define GPIO_SET_E10(x)        	  GPIOF->ODR=(GPIOF->ODR&~GPIO_Pin_10)  | (x ? GPIO_Pin_10 : 0)
#define GPIO_SET_E11(x)        	  GPIOF->ODR=(GPIOF->ODR&~GPIO_Pin_11)  | (x ? GPIO_Pin_11 : 0)
#define GPIO_SET_E12(x)        	  GPIOF->ODR=(GPIOF->ODR&~GPIO_Pin_12)  | (x ? GPIO_Pin_12 : 0)
#define GPIO_SET_E13(x)        	  GPIOF->ODR=(GPIOF->ODR&~GPIO_Pin_13)  | (x ? GPIO_Pin_13 : 0)
#define GPIO_SET_E14(x)        	  GPIOF->ODR=(GPIOF->ODR&~GPIO_Pin_14)  | (x ? GPIO_Pin_14 : 0)
#define GPIO_SET_E15(x)        	  GPIOF->ODR=(GPIOF->ODR&~GPIO_Pin_15)  | (x ? GPIO_Pin_15 : 0)
//==============================================================================================
#define GPIO_READ_A0()        	 (GPIOA->IDR&GPIO_Pin_0)
#define GPIO_READ_A1()        	 (GPIOA->IDR&GPIO_Pin_1)
#define GPIO_READ_A2()        	 (GPIOA->IDR&GPIO_Pin_2)
#define GPIO_READ_A3()        	 (GPIOA->IDR&GPIO_Pin_3)
#define GPIO_READ_A4()        	 (GPIOA->IDR&GPIO_Pin_4)
#define GPIO_READ_A5()        	 (GPIOA->IDR&GPIO_Pin_5)
#define GPIO_READ_A6()        	 (GPIOA->IDR&GPIO_Pin_6)
#define GPIO_READ_A7()        	 (GPIOA->IDR&GPIO_Pin_7)
#define GPIO_READ_A8()        	 (GPIOA->IDR&GPIO_Pin_8)
#define GPIO_READ_A9()        	 (GPIOA->IDR&GPIO_Pin_9)
#define GPIO_READ_A10()        	 (GPIOA->IDR&GPIO_Pin_10)
#define GPIO_READ_A11()        	 (GPIOA->IDR&GPIO_Pin_11)
#define GPIO_READ_A12()        	 (GPIOA->IDR&GPIO_Pin_12)
#define GPIO_READ_A13()        	 (GPIOA->IDR&GPIO_Pin_13)
#define GPIO_READ_A14()        	 (GPIOA->IDR&GPIO_Pin_14)
#define GPIO_READ_A15()        	 (GPIOA->IDR&GPIO_Pin_15)

#define GPIO_READ_B0()        	 (GPIOB->IDR&GPIO_Pin_0)
#define GPIO_READ_B1()        	 (GPIOB->IDR&GPIO_Pin_1)
#define GPIO_READ_B2()        	 (GPIOB->IDR&GPIO_Pin_2)
#define GPIO_READ_B3()        	 (GPIOB->IDR&GPIO_Pin_3)
#define GPIO_READ_B4()        	 (GPIOB->IDR&GPIO_Pin_4)
#define GPIO_READ_B5()        	 (GPIOB->IDR&GPIO_Pin_5)
#define GPIO_READ_B6()        	 (GPIOB->IDR&GPIO_Pin_6)
#define GPIO_READ_B7()        	 (GPIOB->IDR&GPIO_Pin_7)
#define GPIO_READ_B8()        	 (GPIOB->IDR&GPIO_Pin_8)
#define GPIO_READ_B9()        	 (GPIOB->IDR&GPIO_Pin_9)
#define GPIO_READ_B10()        	 (GPIOB->IDR&GPIO_Pin_10)
#define GPIO_READ_B11()        	 (GPIOB->IDR&GPIO_Pin_11)
#define GPIO_READ_B12()        	 (GPIOB->IDR&GPIO_Pin_12)
#define GPIO_READ_B13()        	 (GPIOB->IDR&GPIO_Pin_13)
#define GPIO_READ_B14()        	 (GPIOB->IDR&GPIO_Pin_14)
#define GPIO_READ_B15()        	 (GPIOB->IDR&GPIO_Pin_15)

#define GPIO_READ_C0()        	 (GPIOC->IDR&GPIO_Pin_0)
#define GPIO_READ_C1()        	 (GPIOC->IDR&GPIO_Pin_1)
#define GPIO_READ_C2()        	 (GPIOC->IDR&GPIO_Pin_2)
#define GPIO_READ_C3()        	 (GPIOC->IDR&GPIO_Pin_3)
#define GPIO_READ_C4()        	 (GPIOC->IDR&GPIO_Pin_4)
#define GPIO_READ_C5()        	 (GPIOC->IDR&GPIO_Pin_5)
#define GPIO_READ_C6()        	 (GPIOC->IDR&GPIO_Pin_6)
#define GPIO_READ_C7()        	 (GPIOC->IDR&GPIO_Pin_7)
#define GPIO_READ_C8()        	 (GPIOC->IDR&GPIO_Pin_8)
#define GPIO_READ_C9()        	 (GPIOC->IDR&GPIO_Pin_9)
#define GPIO_READ_C10()        	 (GPIOC->IDR&GPIO_Pin_10)
#define GPIO_READ_C11()        	 (GPIOC->IDR&GPIO_Pin_11)
#define GPIO_READ_C12()        	 (GPIOC->IDR&GPIO_Pin_12)
#define GPIO_READ_C13()        	 (GPIOC->IDR&GPIO_Pin_13)
#define GPIO_READ_C14()        	 (GPIOC->IDR&GPIO_Pin_14)
#define GPIO_READ_C15()        	 (GPIOC->IDR&GPIO_Pin_15)

#define GPIO_READ_D0()        	 (GPIOD->IDR&GPIO_Pin_0)
#define GPIO_READ_D1()        	 (GPIOD->IDR&GPIO_Pin_1)
#define GPIO_READ_D2()        	 (GPIOD->IDR&GPIO_Pin_2)
#define GPIO_READ_D3()        	 (GPIOD->IDR&GPIO_Pin_3)
#define GPIO_READ_D4()        	 (GPIOD->IDR&GPIO_Pin_4)
#define GPIO_READ_D5()        	 (GPIOD->IDR&GPIO_Pin_5)
#define GPIO_READ_D6()        	 (GPIOD->IDR&GPIO_Pin_6)
#define GPIO_READ_D7()        	 (GPIOD->IDR&GPIO_Pin_7)
#define GPIO_READ_D8()        	 (GPIOD->IDR&GPIO_Pin_8)
#define GPIO_READ_D9()        	 (GPIOD->IDR&GPIO_Pin_9)
#define GPIO_READ_D10()        	 (GPIOD->IDR&GPIO_Pin_10)
#define GPIO_READ_D11()        	 (GPIOD->IDR&GPIO_Pin_11)
#define GPIO_READ_D12()        	 (GPIOD->IDR&GPIO_Pin_12)
#define GPIO_READ_D13()        	 (GPIOD->IDR&GPIO_Pin_13)
#define GPIO_READ_D14()        	 (GPIOD->IDR&GPIO_Pin_14)
#define GPIO_READ_D15()        	 (GPIOD->IDR&GPIO_Pin_15)


#define GPIO_READ_E0()        	 (GPIOF->IDR&GPIO_Pin_0)
#define GPIO_READ_E1()        	 (GPIOF->IDR&GPIO_Pin_1)
#define GPIO_READ_E2()        	 (GPIOF->IDR&GPIO_Pin_2)
#define GPIO_READ_E3()        	 (GPIOF->IDR&GPIO_Pin_3)
#define GPIO_READ_E4()        	 (GPIOF->IDR&GPIO_Pin_4)
#define GPIO_READ_E5()        	 (GPIOF->IDR&GPIO_Pin_5)
#define GPIO_READ_E6()        	 (GPIOF->IDR&GPIO_Pin_6)
#define GPIO_READ_E7()        	 (GPIOF->IDR&GPIO_Pin_7)
#define GPIO_READ_E8()        	 (GPIOF->IDR&GPIO_Pin_8)
#define GPIO_READ_E9()        	 (GPIOF->IDR&GPIO_Pin_9)
#define GPIO_READ_E10()        	 (GPIOF->IDR&GPIO_Pin_10)
#define GPIO_READ_E11()        	 (GPIOF->IDR&GPIO_Pin_11)
#define GPIO_READ_E12()        	 (GPIOF->IDR&GPIO_Pin_12)
#define GPIO_READ_E13()        	 (GPIOF->IDR&GPIO_Pin_13)
#define GPIO_READ_E14()        	 (GPIOF->IDR&GPIO_Pin_14)
#define GPIO_READ_E15()        	 (GPIOF->IDR&GPIO_Pin_15)

/*Setting  pins to General purpose output push-pull mode max speed at 50MHz  (CNFx=00,MODEx=11)                                         */
 //GPIOC
#define GPIO_C0_DIR_OUT        GPIOC->CRL = (GPIOC->CRL&  0xFFFFFFF0)  | 0x00000003;
#define GPIO_C1_DIR_OUT        GPIOC->CRL = (GPIOC->CRL&  0xFFFFFF0F)  | 0x00000030;
#define GPIO_C2_DIR_OUT        GPIOC->CRL = (GPIOC->CRL&  0xFFFFF0FF)  | 0x00000300;
#define GPIO_C3_DIR_OUT        GPIOC->CRL = (GPIOC->CRL&  0xFFFF0FFF)  | 0x00003000;
#define GPIO_C4_DIR_OUT        GPIOC->CRL = (GPIOC->CRL&  0xFFF0FFFF)  | 0x00030000;
#define GPIO_C5_DIR_OUT        GPIOC->CRL = (GPIOC->CRL&  0xFF0FFFFF)  | 0x00300000;
#define GPIO_C6_DIR_OUT        GPIOC->CRL = (GPIOC->CRL&  0xF0FFFFFF)  | 0x03000000;
#define GPIO_C7_DIR_OUT        GPIOC->CRL = (GPIOC->CRL&  0x0FFFFFFF)  | 0x30000000;
#define GPIO_C8_DIR_OUT        GPIOC->CRH = (GPIOC->CRH&  0xFFFFFFF0)  | 0x00000003;
#define GPIO_C9_DIR_OUT        GPIOC->CRH =	(GPIOC->CRH&  0xFFFFFF0F)  | 0x00000030;
#define GPIO_C10_DIR_OUT       GPIOC->CRH = (GPIOC->CRH&  0xFFFFF0FF)  | 0x00000300;
#define GPIO_C11_DIR_OUT       GPIOC->CRH = (GPIOC->CRH&  0xFFFF0FFF)  | 0x00003000;
#define GPIO_C12_DIR_OUT       GPIOC->CRH = (GPIOC->CRH&  0xFFF0FFFF)  | 0x00030000;
#define GPIO_C13_DIR_OUT       GPIOC->CRH = (GPIOC->CRH&  0xFF0FFFFF)  | 0x00300000;
#define GPIO_C14_DIR_OUT       GPIOC->CRH = (GPIOC->CRH&  0xF0FFFFFF)  | 0x03000000;
#define GPIO_C15_DIR_OUT       GPIOC->CRH = (GPIOC->CRH&  0x0FFFFFFF)  | 0x30000000; 
 //GPI0B
#define GPIO_B0_DIR_OUT        GPIOB->CRL = (GPIOB->CRL&  0xFFFFFFF0)  | 0x00000003;
#define GPIO_B1_DIR_OUT        GPIOB->CRL = (GPIOB->CRL&  0xFFFFFF0F)  | 0x00000030;
#define GPIO_B2_DIR_OUT        GPIOB->CRL = (GPIOB->CRL&  0xFFFFF0FF)  | 0x00000300;
#define GPIO_B3_DIR_OUT        GPIOB->CRL = (GPIOB->CRL&  0xFFFF0FFF)  | 0x00003000;
#define GPIO_B4_DIR_OUT        GPIOB->CRL = (GPIOB->CRL&  0xFFF0FFFF)  | 0x00030000;
#define GPIO_B5_DIR_OUT        GPIOB->CRL = (GPIOB->CRL&  0xFF0FFFFF)  | 0x00300000;
#define GPIO_B6_DIR_OUT        GPIOB->CRL = (GPIOB->CRL&  0xF0FFFFFF)  | 0x03000000;
#define GPIO_B7_DIR_OUT        GPIOB->CRL = (GPIOB->CRL&  0x0FFFFFFF)  | 0x30000000;
#define GPIO_B8_DIR_OUT        GPIOB->CRH = (GPIOB->CRH&  0xFFFFFFF0)  | 0x00000003;
#define GPIO_B9_DIR_OUT        GPIOB->CRH =	(GPIOB->CRH&  0xFFFFFF0F)  | 0x00000030;
#define GPIO_B10_DIR_OUT       GPIOB->CRH = (GPIOB->CRH&  0xFFFFF0FF)  | 0x00000300;
#define GPIO_B11_DIR_OUT       GPIOB->CRH = (GPIOB->CRH&  0xFFFF0FFF)  | 0x00003000;
#define GPIO_B12_DIR_OUT       GPIOB->CRH = (GPIOB->CRH&  0xFFF0FFFF)  | 0x00030000;
#define GPIO_B13_DIR_OUT       GPIOB->CRH = (GPIOB->CRH&  0xFF0FFFFF)  | 0x00300000;
#define GPIO_B14_DIR_OUT       GPIOB->CRH = (GPIOB->CRH&  0xF0FFFFFF)  | 0x03000000;
#define GPIO_B15_DIR_OUT       GPIOB->CRH = (GPIOB->CRH&  0x0FFFFFFF)  | 0x30000000;
//GPIOA

#define GPIO_A0_DIR_OUT        GPIOA->CRL = (GPIOA->CRL&  0xFFFFFFF0)  | 0x00000003;
#define GPIO_A1_DIR_OUT        GPIOA->CRL = (GPIOA->CRL&  0xFFFFFF0F)  | 0x00000030;
#define GPIO_A2_DIR_OUT        GPIOA->CRL = (GPIOA->CRL&  0xFFFFF0FF)  | 0x00000300;
#define GPIO_A3_DIR_OUT        GPIOA->CRL = (GPIOA->CRL&  0xFFFF0FFF)  | 0x00003000;
#define GPIO_A4_DIR_OUT        GPIOA->CRL = (GPIOA->CRL&  0xFFF0FFFF)  | 0x00030000;
#define GPIO_A5_DIR_OUT        GPIOA->CRL = (GPIOA->CRL&  0xFF0FFFFF)  | 0x00300000;
#define GPIO_A6_DIR_OUT        GPIOA->CRL = (GPIOA->CRL&  0xF0FFFFFF)  | 0x03000000;
#define GPIO_A7_DIR_OUT        GPIOA->CRL = (GPIOA->CRL&  0x0FFFFFFF)  | 0x30000000;
#define GPIO_A8_DIR_OUT        GPIOA->CRH = (GPIOA->CRH&  0xFFFFFFF0)  | 0x00000003;
#define GPIO_A9_DIR_OUT        GPIOA->CRH =	(GPIOA->CRH&  0xFFFFFF0F)  | 0x00000030;
#define GPIO_A10_DIR_OUT       GPIOA->CRH = (GPIOA->CRH&  0xFFFFF0FF)  | 0x00000300;
#define GPIO_A11_DIR_OUT       GPIOA->CRH = (GPIOA->CRH&  0xFFFF0FFF)  | 0x00003000;
#define GPIO_A12_DIR_OUT       GPIOA->CRH = (GPIOA->CRH&  0xFFF0FFFF)  | 0x00030000;
#define GPIO_A13_DIR_OUT       GPIOA->CRH = (GPIOA->CRH&  0xFF0FFFFF)  | 0x00300000;
#define GPIO_A14_DIR_OUT       GPIOA->CRH = (GPIOA->CRH&  0xF0FFFFFF)  | 0x03000000;
#define GPIO_A15_DIR_OUT       GPIOA->CRH = (GPIOA->CRH&  0x0FFFFFFF)  | 0x30000000;

//GPIOD

#define GPIO_D0_DIR_OUT        GPIOD->CRL = (GPIOD->CRL&  0xFFFFFFF0)  | 0x00000003;
#define GPIO_D1_DIR_OUT        GPIOD->CRL = (GPIOD->CRL&  0xFFFFFF0F)  | 0x00000030;
#define GPIO_D2_DIR_OUT        GPIOD->CRL = (GPIOD->CRL&  0xFFFFF0FF)  | 0x00000300;
#define GPIO_D3_DIR_OUT        GPIOD->CRL = (GPIOD->CRL&  0xFFFF0FFF)  | 0x00003000;
#define GPIO_D4_DIR_OUT        GPIOD->CRL = (GPIOD->CRL&  0xFFF0FFFF)  | 0x00030000;
#define GPIO_D5_DIR_OUT        GPIOD->CRL = (GPIOD->CRL&  0xFF0FFFFF)  | 0x00300000;
#define GPIO_D6_DIR_OUT        GPIOD->CRL = (GPIOD->CRL&  0xF0FFFFFF)  | 0x03000000;
#define GPIO_D7_DIR_OUT        GPIOD->CRL = (GPIOD->CRL&  0x0FFFFFFF)  | 0x30000000;
#define GPIO_D8_DIR_OUT        GPIOD->CRH = (GPIOD->CRH&  0xFFFFFFF0)  | 0x00000003;
#define GPIO_D9_DIR_OUT        GPIOD->CRH =	(GPIOD->CRH&  0xFFFFFF0F)  | 0x00000030;
#define GPIO_D10_DIR_OUT       GPIOD->CRH = (GPIOD->CRH&  0xFFFFF0FF)  | 0x00000300;
#define GPIO_D11_DIR_OUT       GPIOD->CRH = (GPIOD->CRH&  0xFFFF0FFF)  | 0x00003000;
#define GPIO_D12_DIR_OUT       GPIOD->CRH = (GPIOD->CRH&  0xFFF0FFFF)  | 0x00030000;
#define GPIO_D13_DIR_OUT       GPIOD->CRH = (GPIOD->CRH&  0xFF0FFFFF)  | 0x00300000;
#define GPIO_D14_DIR_OUT       GPIOD->CRH = (GPIOD->CRH&  0xF0FFFFFF)  | 0x03000000;
#define GPIO_D15_DIR_OUT       GPIOD->CRH = (GPIOD->CRH&  0x0FFFFFFF)  | 0x30000000;

//GPIOE

#define GPIO_E0_DIR_OUT        GPIOF->CRL = (GPIOF->CRL&  0xFFFFFFF0)  | 0x00000003;
#define GPIO_E1_DIR_OUT        GPIOF->CRL = (GPIOF->CRL&  0xFFFFFF0F)  | 0x00000030;
#define GPIO_E2_DIR_OUT        GPIOF->CRL = (GPIOF->CRL&  0xFFFFF0FF)  | 0x00000300;
#define GPIO_E3_DIR_OUT        GPIOF->CRL = (GPIOF->CRL&  0xFFFF0FFF)  | 0x00003000;
#define GPIO_E4_DIR_OUT        GPIOF->CRL = (GPIOF->CRL&  0xFFF0FFFF)  | 0x00030000;
#define GPIO_E5_DIR_OUT        GPIOF->CRL = (GPIOF->CRL&  0xFF0FFFFF)  | 0x00300000;
#define GPIO_E6_DIR_OUT        GPIOF->CRL = (GPIOF->CRL&  0xF0FFFFFF)  | 0x03000000;
#define GPIO_E7_DIR_OUT        GPIOF->CRL = (GPIOF->CRL&  0x0FFFFFFF)  | 0x30000000;
#define GPIO_E8_DIR_OUT        GPIOF->CRH = (GPIOF->CRH&  0xFFFFFFF0)  | 0x00000003;
#define GPIO_E9_DIR_OUT        GPIOF->CRH =	(GPIOF->CRH&  0xFFFFFF0F)  | 0x00000030;
#define GPIO_E10_DIR_OUT       GPIOF->CRH = (GPIOF->CRH&  0xFFFFF0FF)  | 0x00000300;
#define GPIO_E11_DIR_OUT       GPIOF->CRH = (GPIOF->CRH&  0xFFFF0FFF)  | 0x00003000;
#define GPIO_E12_DIR_OUT       GPIOF->CRH = (GPIOF->CRH&  0xFFF0FFFF)  | 0x00030000;
#define GPIO_E13_DIR_OUT       GPIOF->CRH = (GPIOF->CRH&  0xFF0FFFFF)  | 0x00300000;
#define GPIO_E14_DIR_OUT       GPIOF->CRH = (GPIOF->CRH&  0xF0FFFFFF)  | 0x03000000;
#define GPIO_E15_DIR_OUT       GPIOF->CRH = (GPIOF->CRH&  0x0FFFFFFF)  | 0x30000000;

 /*SET THE GPIP PORT TO FLOATING INPUT MODE ,CNFx=01B,MODEx=00B;		  */

#define  GPIO_C0_DIR_IN       GPIOC->CRL = (GPIOC->CRL&   0xFFFFFFF0)  | 0x00000004;
#define  GPIO_C1_DIR_IN       GPIOC->CRL = (GPIOC->CRL&   0xFFFFFF0F)  | 0x00000040;
#define  GPIO_C2_DIR_IN       GPIOC->CRL = (GPIOC->CRL&   0xFFFFF0FF)  | 0x00000400;
#define  GPIO_C3_DIR_IN       GPIOC->CRL = (GPIOC->CRL&   0xFFFF0FFF)  | 0x00004000;
#define  GPIO_C4_DIR_IN       GPIOC->CRL = (GPIOC->CRL&   0xFFF0FFFF)  | 0x00040000;
#define  GPIO_C5_DIR_IN       GPIOC->CRL = (GPIOC->CRL&   0xFF0FFFFF)  | 0x00400000;
#define  GPIO_C6_DIR_IN       GPIOC->CRL = (GPIOC->CRL&   0xF0FFFFFF)  | 0x04000000;
#define  GPIO_C7_DIR_IN       GPIOC->CRL = (GPIOC->CRL&   0x0FFFFFFF)  | 0x40000000;
#define  GPIO_C8_DIR_IN       GPIOC->CRH = (GPIOC->CRH&   0xFFFFFFF0)  | 0x00000004;
#define  GPIO_C9_DIR_IN       GPIOC->CRH = (GPIOC->CRH&   0xFFFFFF0F)  | 0x00000040;
#define  GPIO_C10_DIR_IN      GPIOC->CRH = (GPIOC->CRH&   0xFFFFF0FF)  | 0x00000400;
#define  GPIO_C11_DIR_IN      GPIOC->CRH = (GPIOC->CRH&   0xFFFF0FFF)  | 0x00004000;
#define  GPIO_C12_DIR_IN      GPIOC->CRH = (GPIOC->CRH&   0xFFF0FFFF)  | 0x00040000;
#define  GPIO_C13_DIR_IN      GPIOC->CRH = (GPIOC->CRH&   0xFF0FFFFF)  | 0x00400000;
#define  GPIO_C14_DIR_IN      GPIOC->CRH = (GPIOC->CRH&   0xF0FFFFFF)  | 0x04000000;
#define  GPIO_C15_DIR_IN      GPIOC->CRH = (GPIOC->CRH&   0x0FFFFFFF)  | 0x40000000;

#define  GPIO_A0_DIR_IN       GPIOA->CRL = (GPIOA->CRL&   0xFFFFFFF0)  | 0x00000004;
#define  GPIO_A1_DIR_IN       GPIOA->CRL = (GPIOA->CRL&   0xFFFFFF0F)  | 0x00000040;
#define  GPIO_A2_DIR_IN       GPIOA->CRL = (GPIOA->CRL&   0xFFFFF0FF)  | 0x00000400;
#define  GPIO_A3_DIR_IN       GPIOA->CRL = (GPIOA->CRL&   0xFFFF0FFF)  | 0x00004000;
#define  GPIO_A4_DIR_IN       GPIOA->CRL = (GPIOA->CRL&   0xFFF0FFFF)  | 0x00040000;
#define  GPIO_A5_DIR_IN       GPIOA->CRL = (GPIOA->CRL&   0xFF0FFFFF)  | 0x00400000;
#define  GPIO_A6_DIR_IN       GPIOA->CRL = (GPIOA->CRL&   0xF0FFFFFF)  | 0x04000000;
#define  GPIO_A7_DIR_IN       GPIOA->CRL = (GPIOA->CRL&   0x0FFFFFFF)  | 0x40000000;
#define  GPIO_A8_DIR_IN       GPIOA->CRH = (GPIOA->CRH&   0xFFFFFFF0)  | 0x00000004;
#define  GPIO_A9_DIR_IN       GPIOA->CRH = (GPIOA->CRH&   0xFFFFFF0F)  | 0x00000040;
#define  GPIO_A10_DIR_IN      GPIOA->CRH = (GPIOA->CRH&   0xFFFFF0FF)  | 0x00000400;
#define  GPIO_A11_DIR_IN      GPIOA->CRH = (GPIOA->CRH&   0xFFFF0FFF)  | 0x00004000;
#define  GPIO_A12_DIR_IN      GPIOA->CRH = (GPIOA->CRH&   0xFFF0FFFF)  | 0x00040000;
#define  GPIO_A13_DIR_IN      GPIOA->CRH = (GPIOA->CRH&   0xFF0FFFFF)  | 0x00400000;
#define  GPIO_A14_DIR_IN      GPIOA->CRH = (GPIOA->CRH&   0xF0FFFFFF)  | 0x04000000;
#define  GPIO_A15_DIR_IN      GPIOA->CRH = (GPIOA->CRH&   0x0FFFFFFF)  | 0x40000000;

#define  GPIO_B0_DIR_IN       GPIOB->CRL = (GPIOB->CRL&   0xFFFFFFF0)  | 0x00000004;
#define  GPIO_B1_DIR_IN       GPIOB->CRL = (GPIOB->CRL&   0xFFFFFF0F)  | 0x00000040;
#define  GPIO_B2_DIR_IN       GPIOB->CRL = (GPIOB->CRL&   0xFFFFF0FF)  | 0x00000400;
#define  GPIO_B3_DIR_IN       GPIOB->CRL = (GPIOB->CRL&   0xFFFF0FFF)  | 0x00004000;
#define  GPIO_B4_DIR_IN       GPIOB->CRL = (GPIOB->CRL&   0xFFF0FFFF)  | 0x00040000;
#define  GPIO_B5_DIR_IN       GPIOB->CRL = (GPIOB->CRL&   0xFF0FFFFF)  | 0x00400000;
#define  GPIO_B6_DIR_IN       GPIOB->CRL = (GPIOB->CRL&   0xF0FFFFFF)  | 0x04000000;
#define  GPIO_B7_DIR_IN       GPIOB->CRL = (GPIOB->CRL&   0x0FFFFFFF)  | 0x40000000;
#define  GPIO_B8_DIR_IN       GPIOB->CRH = (GPIOB->CRH&   0xFFFFFFF0)  | 0x00000004;
#define  GPIO_B9_DIR_IN       GPIOB->CRH = (GPIOB->CRH&   0xFFFFFF0F)  | 0x00000040;
#define  GPIO_B10_DIR_IN      GPIOB->CRH = (GPIOB->CRH&   0xFFFFF0FF)  | 0x00000400;
#define  GPIO_B11_DIR_IN      GPIOB->CRH = (GPIOB->CRH&   0xFFFF0FFF)  | 0x00004000;
#define  GPIO_B12_DIR_IN      GPIOB->CRH = (GPIOB->CRH&   0xFFF0FFFF)  | 0x00040000;
#define  GPIO_B13_DIR_IN      GPIOB->CRH = (GPIOB->CRH&   0xFF0FFFFF)  | 0x00400000;
#define  GPIO_B14_DIR_IN      GPIOB->CRH = (GPIOB->CRH&   0xF0FFFFFF)  | 0x04000000;
#define  GPIO_B15_DIR_IN      GPIOB->CRH = (GPIOB->CRH&   0x0FFFFFFF)  | 0x40000000;

#define  GPIO_D0_DIR_IN       GPIOD->CRL = (GPIOD->CRL&   0xFFFFFFF0)  | 0x00000004;
#define  GPIO_D1_DIR_IN       GPIOD->CRL = (GPIOD->CRL&   0xFFFFFF0F)  | 0x00000040;
#define  GPIO_D2_DIR_IN       GPIOD->CRL = (GPIOD->CRL&   0xFFFFF0FF)  | 0x00000400;
#define  GPIO_D3_DIR_IN       GPIOD->CRL = (GPIOD->CRL&   0xFFFF0FFF)  | 0x00004000;
#define  GPIO_D4_DIR_IN       GPIOD->CRL = (GPIOD->CRL&   0xFFF0FFFF)  | 0x00040000;
#define  GPIO_D5_DIR_IN       GPIOD->CRL = (GPIOD->CRL&   0xFF0FFFFF)  | 0x00400000;
#define  GPIO_D6_DIR_IN       GPIOD->CRL = (GPIOD->CRL&   0xF0FFFFFF)  | 0x04000000;
#define  GPIO_D7_DIR_IN       GPIOD->CRL = (GPIOD->CRL&   0x0FFFFFFF)  | 0x40000000;
#define  GPIO_D8_DIR_IN       GPIOD->CRH = (GPIOD->CRH&   0xFFFFFFF0)  | 0x00000004;
#define  GPIO_D9_DIR_IN       GPIOD->CRH = (GPIOD->CRH&   0xFFFFFF0F)  | 0x00000040;
#define  GPIO_D10_DIR_IN      GPIOD->CRH = (GPIOD->CRH&   0xFFFFF0FF)  | 0x00000400;
#define  GPIO_D11_DIR_IN      GPIOD->CRH = (GPIOD->CRH&   0xFFFF0FFF)  | 0x00004000;
#define  GPIO_D12_DIR_IN      GPIOD->CRH = (GPIOD->CRH&   0xFFF0FFFF)  | 0x00040000;
#define  GPIO_D13_DIR_IN      GPIOD->CRH = (GPIOD->CRH&   0xFF0FFFFF)  | 0x00400000;
#define  GPIO_D14_DIR_IN      GPIOD->CRH = (GPIOD->CRH&   0xF0FFFFFF)  | 0x04000000;
#define  GPIO_D15_DIR_IN      GPIOD->CRH = (GPIOD->CRH&   0x0FFFFFFF)  | 0x40000000;

#define  GPIO_E0_DIR_IN       GPIOF->CRL = (GPIOF->CRL&   0xFFFFFFF0)  | 0x00000004;
#define  GPIO_E1_DIR_IN       GPIOF->CRL = (GPIOF->CRL&   0xFFFFFF0F)  | 0x00000040;
#define  GPIO_E2_DIR_IN       GPIOF->CRL = (GPIOF->CRL&   0xFFFFF0FF)  | 0x00000400;
#define  GPIO_E3_DIR_IN       GPIOF->CRL = (GPIOF->CRL&   0xFFFF0FFF)  | 0x00004000;
#define  GPIO_E4_DIR_IN       GPIOF->CRL = (GPIOF->CRL&   0xFFF0FFFF)  | 0x00040000;
#define  GPIO_E5_DIR_IN       GPIOF->CRL = (GPIOF->CRL&   0xFF0FFFFF)  | 0x00400000;
#define  GPIO_E6_DIR_IN       GPIOF->CRL = (GPIOF->CRL&   0xF0FFFFFF)  | 0x04000000;
#define  GPIO_E7_DIR_IN       GPIOF->CRL = (GPIOF->CRL&   0x0FFFFFFF)  | 0x40000000;
#define  GPIO_E8_DIR_IN       GPIOF->CRH = (GPIOF->CRH&   0xFFFFFFF0)  | 0x00000004;
#define  GPIO_E9_DIR_IN       GPIOF->CRH = (GPIOF->CRH&   0xFFFFFF0F)  | 0x00000040;
#define  GPIO_E10_DIR_IN      GPIOF->CRH = (GPIOF->CRH&   0xFFFFF0FF)  | 0x00000400;
#define  GPIO_E11_DIR_IN      GPIOF->CRH = (GPIOF->CRH&   0xFFFF0FFF)  | 0x00004000;
#define  GPIO_E12_DIR_IN      GPIOF->CRH = (GPIOF->CRH&   0xFFF0FFFF)  | 0x00040000;
#define  GPIO_E13_DIR_IN      GPIOF->CRH = (GPIOF->CRH&   0xFF0FFFFF)  | 0x00400000;
#define  GPIO_E14_DIR_IN      GPIOF->CRH = (GPIOF->CRH&   0xF0FFFFFF)  | 0x04000000;
#define  GPIO_E15_DIR_IN      GPIOF->CRH = (GPIOF->CRH&   0x0FFFFFFF)  | 0x40000000;
#endif

