/****************************************************************************************
�ļ�:   I2c.c
˵��:   I2C�ӿ�.ʹ��ģ�ⷽʽʵ��.
****************************************************************************************/
//ͷ�ļ�.
#include "main.h"
#if 0
void I2C_SOFT_BIT_DELAY(void)
{
    uint8 i;
    
    i = 8; 
    while(i--);    
}



void I2c_SoftClockSetOutput(void)
  {
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }


void I2c_SoftClockSetInput(void)
  {
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }



void I2c_SoftDatSetOutput(void)
  {
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }

void I2c_SoftDatSetInput(void)
  {
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }


/****************************************************************************************/
void I2C_SOFT_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
  GPIO_InitStruct.Pull = GPIO_PULLUP;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);
}



////////////////////////////////////////////////////////////////////////////////
I2c_Soft_ResultType I2C_SOFT_Start(void)
////////////////////////////////////////////////////////////////////////////////
{
	uint16 TimeOutCtr;
	//I2c_SoftClockLow();
	//I2C_SOFT_BIT_DELAY();
	//I2c_SoftDathigh();

    I2c_SoftClockhigh();
    I2C_SOFT_BIT_DELAY();
    I2c_SoftDathigh();
    I2C_SOFT_BIT_DELAY();
    I2c_SoftDatLow();
   I2C_SOFT_BIT_DELAY();
    I2c_SoftClockLow();

	
	I2c_SoftDatSetInput();
	TimeOutCtr = I2C_SOFT_TIMEOUT_CNT;
	do
	{
	} while ((0 == I2c_SoftDatGetData()) && (0 < --TimeOutCtr));
	I2c_SoftDatSetOutput();
	I2c_SoftDathigh();
	if (0 < TimeOutCtr)
	{
		I2c_SoftClockhigh();
		I2c_SoftClockSetInput();
		TimeOutCtr = I2C_SOFT_TIMEOUT_CNT;
		do
		{
		} while ((0 == I2c_SoftClockGetData()) && (0 < --TimeOutCtr));
		I2c_SoftClockSetOutput();
		I2c_SoftClockhigh();
		if (0 < TimeOutCtr)
		{
			I2c_SoftDatLow();
			I2C_SOFT_BIT_DELAY();
			return I2C_SOFT_SUCCESS;
		}
	}
	return I2C_SOFT_FAILED;
}

////////////////////////////////////////////////////////////////////////////////
I2c_Soft_ResultType I2C_SOFT_Stop(void)
////////////////////////////////////////////////////////////////////////////////
{
	uint16 TimeOutCtr;

	I2c_SoftClockLow();
	I2C_SOFT_BIT_DELAY();
	I2c_SoftDatLow();
	I2C_SOFT_BIT_DELAY();
	I2c_SoftClockhigh();
	I2c_SoftClockSetInput();
	TimeOutCtr = I2C_SOFT_TIMEOUT_CNT;
	do
	{
	} while ((0 == I2c_SoftClockGetData()) && (0 < --TimeOutCtr));
	I2c_SoftClockSetOutput();
	I2c_SoftClockhigh();
	if (0 < TimeOutCtr)
	{
		I2c_SoftDathigh();
		I2C_SOFT_BIT_DELAY();
		I2c_SoftDatSetInput();
		TimeOutCtr = I2C_SOFT_TIMEOUT_CNT;
		do
		{
		} while ((0 == I2c_SoftDatGetData()) && (0 < --TimeOutCtr));
		I2c_SoftDatSetOutput();
		if (0 < TimeOutCtr)
		{
			return I2C_SOFT_SUCCESS;
		}
	}

	return I2C_SOFT_FAILED;
}

////////////////////////////////////////////////////////////////////////////////
I2c_Soft_ResultType I2C_SOFT_Byte_Rx(BYTE *pRxData, BYTE LastByte)
////////////////////////////////////////////////////////////////////////////////
{
	uint8 BitNum;
	BYTE RxDataLocal;
	uint16 TimeOutCtr;

	I2c_SoftClockhigh();
	I2c_SoftClockSetInput();
	TimeOutCtr = I2C_SOFT_TIMEOUT_CNT;
	do
	{
	} while ((0 == I2c_SoftClockGetData()) && (0 < --TimeOutCtr));
	I2c_SoftClockSetOutput();
	if (0 < TimeOutCtr)
	{
		I2c_SoftClockLow();
		I2C_SOFT_BIT_DELAY();
		I2c_SoftDathigh();
		I2c_SoftDatSetInput();

		RxDataLocal = (BYTE)0;
		BitNum = 7;
		do
		{
			I2c_SoftClockhigh();
			I2C_SOFT_BIT_DELAY();
			I2C_SOFT_BIT_DELAY();
			RxDataLocal |= ((I2c_SoftDatGetData()) << BitNum);
			I2c_SoftClockLow();
			I2C_SOFT_BIT_DELAY();
			I2C_SOFT_BIT_DELAY();
		} while (0 < BitNum--);
		I2c_SoftDatSetOutput();
		//(I2C_SOFT_RX_NOT_LAST_BYTE == LastByte) ? I2c_SoftDatLow():I2c_SoftDathigh();
		if ( I2C_SOFT_RX_NOT_LAST_BYTE == LastByte )
		{
		  I2c_SoftDatLow();
		}
		else
		{
		  I2c_SoftDathigh();
		}
			
		I2C_SOFT_BIT_DELAY();
		I2c_SoftClockhigh();

		*pRxData = RxDataLocal;

		return I2C_SOFT_SUCCESS;
	}

	return I2C_SOFT_FAILED;
}

////////////////////////////////////////////////////////////////////////////////
I2c_Soft_ResultType I2C_SOFT_Byte_Tx(BYTE TxData)
////////////////////////////////////////////////////////////////////////////////
#if 1
{
	uint8 BitNum;
	uint16 TimeOutCtr;

	I2c_SoftClockLow();
	I2c_SoftDathigh();
	I2c_SoftDatSetInput();
	TimeOutCtr = I2C_SOFT_TIMEOUT_CNT;
	do
	{
	} while ((0 == I2c_SoftDatGetData()) && (0 < --TimeOutCtr));
	I2c_SoftDatSetOutput();
	if (0 < TimeOutCtr)
	{
		BitNum = 7;
		do
		{
			I2c_SoftClockLow();
			//SOFT_IIC_DELAY(1);
			if(0 == ((1 << BitNum) & TxData)) 
			{
				I2c_SoftDatLow();
			}
			else
			{
				I2c_SoftDathigh();
			}
			//SOFT_IIC_DELAY(1);
			I2C_SOFT_BIT_DELAY();
			I2c_SoftClockhigh();
//			I2C_SOFT_BIT_DELAY();
			I2C_SOFT_BIT_DELAY();
		} while (0 < BitNum--);
		I2c_SoftClockLow();
		I2c_SoftDathigh();
		I2c_SoftDatSetInput();
		I2C_SOFT_BIT_DELAY();
		I2c_SoftClockhigh();
		TimeOutCtr = 10;
              do
              {
                  I2C_SOFT_BIT_DELAY();
              } while ((1 == I2c_SoftDatGetData()) && (0 < --TimeOutCtr));
		if (0 == I2c_SoftDatGetData())
		{
			I2c_SoftDatSetOutput();
                     I2c_SoftDatLow();
			return I2C_SOFT_SUCCESS;
		}
		I2c_SoftDatSetOutput();
		I2c_SoftDatLow();
	}
	return I2C_SOFT_FAILED;
}
#else
{
	uint8 BitNum;
	uint16 TimeOutCtr;
	GPIO_InitTypeDef GPIO_InitStructure;

	I2c_SoftClockLow();
	I2C_SOFT_BIT_DELAY();
	I2c_SoftDathigh();
	I2c_SoftDatSetInput();
	TimeOutCtr = I2C_SOFT_TIMEOUT_CNT;
	do
	{
	} while ((0 == I2c_SoftDatGetData()) && (0 < --TimeOutCtr));
	I2c_SoftDatSetOutput();
	if (0 < TimeOutCtr)
	{
		BitNum = 7;
		do
		{
			I2c_SoftClockLow();
			I2C_SOFT_BIT_DELAY();
			(0 == (((BYTE)1 << BitNum) & TxData)) ?  I2c_SoftDatLow() :I2c_SoftDathigh();
			I2C_SOFT_BIT_DELAY();
			I2c_SoftClockhigh();
			I2C_SOFT_BIT_DELAY();
		} while (0 < BitNum--);
		I2c_SoftClockLow();
		I2C_SOFT_BIT_DELAY();
		I2c_SoftDathigh();
		I2c_SoftDatSetInput();
		I2c_SoftClockhigh();
              TimeOutCtr = 5;
              do
              {
                  I2C_SOFT_BIT_DELAY();
              } while ((1 == I2c_SoftDatGetData()) && (0 < --TimeOutCtr));
		if (0 == I2c_SoftDatGetData())
		{
			I2c_SoftDatSetOutput();
                     I2c_SoftDatLow();
			return I2C_SOFT_SUCCESS;
		}
		I2c_SoftDatSetOutput();
		I2c_SoftDatLow();
	}
	return I2C_SOFT_FAILED;
}

#endif

#else

#define SCL_CLK_DLY		5

/***********************************************************************
* SCL--PB6
***********************************************************************/
#define I2cSCL_OUT_HIGH()	   GPIO_SET_B6(1)
#define I2cSCL_OUT_LOW()	   GPIO_SET_B6(0)



/**********************************************************************
* SDA---PB7
***********************************************************************/
#define I2cSDA_OUT_HIGH()	GPIO_SET_B7(1)
#define I2cSDA_OUT_LOW()	GPIO_SET_B7(0)

#define  READ_I2cSDA()		 GPIO_READ_B7()

#define  I2cSDA_OUTPUT()	 GPIOB->MODER|=0x00004000
#define  I2cSDA_INPUT()		 GPIOB->MODER&=0xffff3fff

/****************************************************************************************/
void I2C_SOFT_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);
}



/****************************************************************************************/
void I2cDelay(u16 dat)
{
		delay_us(dat);
}



/****************************************************************************************
����:   ��������.
����:   ��.
����:   ��.
˵��:   ������������,���ظ���������.��SCLΪ��ʱ,SDA�ɸߵ���.
        ����״̬��,SCL,SDA��̬Ϊ�ߵ�ƽ;����ͨѶ״̬��,SCL,SDA��̬Ϊ�͵�ƽ.
****************************************************************************************/
void I2cStart(void)
{
    I2cSCL_OUT_HIGH();
    I2cDelay(SCL_CLK_DLY);
    I2cSDA_OUT_HIGH();
    I2cDelay(SCL_CLK_DLY);
    I2cSDA_OUT_LOW();
    I2cDelay(SCL_CLK_DLY);
    I2cSCL_OUT_LOW();
}

/****************************************************************************************
����:   ֹͣ����.
����:   ��.
����:   ��.
˵��:   ����ֹͣ����.��SCLΪ��ʱ,SDA�ɵ͵���.
        ����״̬��,SCL,SDA��̬Ϊ�ߵ�ƽ;����ͨѶ״̬��,SCL,SDA��̬Ϊ�͵�ƽ.
****************************************************************************************/
void I2cStop(void)
{
    I2cSCL_OUT_LOW();
    I2cDelay(SCL_CLK_DLY);
    I2cSDA_OUT_LOW();
    I2cDelay(SCL_CLK_DLY);
    I2cSCL_OUT_HIGH();
    I2cDelay(SCL_CLK_DLY);
    I2cSDA_OUT_HIGH();
}

/****************************************************************************************
����:   CLOCK
����:   ��.
����:   �������.
˵��:   
****************************************************************************************/

void I2cClock(void)
{
    I2cDelay(SCL_CLK_DLY);
    I2cSCL_OUT_HIGH();
    I2cDelay(SCL_CLK_DLY);
    I2cSCL_OUT_LOW();
}

/****************************************************************************************
����:   ����ACKλ.
����:   ��.
����:   �������.
˵��:   ���������ڴ��豸��ACKλ.
****************************************************************************************/

BOOL I2cReadAck(void)
{
    u16 timeout=20;

    I2cSDA_INPUT();
    I2cDelay(5);
    I2cSCL_OUT_HIGH();
    
    while (--timeout)
    {
        I2cDelay(SCL_CLK_DLY);
        if(READ_I2cSDA()==0) break;
    }
    
    I2cSCL_OUT_LOW();
    I2cSDA_OUT_HIGH();
    I2cSDA_OUTPUT();
    
    if (timeout)
        return 1;          // get Ack
    else 
        return 0;         // time out,not ack

}

/****************************************************************************************
����:   ����ACK/NACK.
����:   ��.
����:   ��.
****************************************************************************************/
#define ACK			0
#define NACK			1

void Send_Ack(u8 Ack)
{
    if(Ack==0)
    {
        I2cSDA_OUT_LOW();
    }
    else
    {
        I2cSDA_OUT_HIGH();
    }
    
    I2cClock();
}

/****************************************************************************************
����:   ����һ�ֽ�����.
����:   ��.
����:   ��.
˵��:   ����һ���ֽ�(8λ)����.
        ����״̬��,SCL,SDA��̬Ϊ�ߵ�ƽ;����ͨѶ״̬��,SCL,SDA��̬Ϊ�͵�ƽ.
****************************************************************************************/
void I2cWriteByte(INT8U dat)
{
    register u8 i;
    
    I2cSCL_OUT_LOW();
    
    for (i=0;i<8;i++)
    {
        if(dat&0x80)
            I2cSDA_OUT_HIGH();
        else
            I2cSDA_OUT_LOW();
        
        dat<<=1;
        
        I2cClock();
    }
}

/****************************************************************************************
����:   ����һ���ֽ�����.
����:   ��.
����:   �������.
˵��:   ���������ڴ��豸��һ���ֽ�����.
****************************************************************************************/
INT8U I2cReadByte(void)
{
    u8 value=0;
    
    u8 i ;
    
    I2cDelay(SCL_CLK_DLY);
    I2cSDA_INPUT();
    
    for(i=0;i<8;i++) 
    {
        I2cDelay(SCL_CLK_DLY);
        I2cSCL_OUT_HIGH();
        
        value<<=1;
        if(READ_I2cSDA()) value++;
        
        I2cDelay(SCL_CLK_DLY);
        I2cSCL_OUT_LOW();
    }
    
    I2cSDA_OUTPUT();
    I2cSDA_OUT_HIGH();
    
    return (value);
}

/********************************************************************************
* ����: д.
* ����: addr    ������ַ.I2C������ַ.
*      	reg     �Ĵ�����ַ.��ʼ��ַ.
*      	ptr     Դ����.
*      	len     Ҫд����ֽ���.����.
* ����: �������.0:ʧ��. 1:�ɹ�.
********************************************************************************/

//OP I2cWriteBytes(unsigned char uDevice, unsigned char uAddr, unsigned char * ptr, unsigned char len)
Std_ReturnType I2cWriteBytes(unsigned char uDevice, unsigned char * ptr, unsigned char len)
{
     I2cStart();

	 
     I2cWriteByte(uDevice&0xfe);
	 
     if (I2cReadAck() == FALSE)
     {
          I2cStop(); 
          return 0;
     }   
     
     //I2cWriteByte(uAddr);
     //if(I2cReadAck()==FALSE){I2cStop();return OP_FAIL; }
     
     while (len--)		//����.
     {        
         I2cWriteByte(*ptr);
         if(I2cReadAck()==FALSE){I2cStop();return 0; }
         
         ptr++;
     }
     
     I2cStop();
     
     return 1;
}

/***************************************************************************************************
����:   ��.
����:   uDeviceAddr �豸��ַ.������ַ.
               uCellAddr   ��Ԫ��ַ.Ƭ�ڵ�ַ,�洢����ַ,�Ĵ�����ַ.
               puBuf       ������.�������ض�������.
               uNum        �ֽ���.
����:   �������.
***************************************************************************************************/
//OP I2cReadBytes(unsigned char uDevice, unsigned char uAddr, unsigned char * puBuf, unsigned char uNum)
Std_ReturnType I2cReadBytes(unsigned char uDevice, unsigned char * puBuf, unsigned char uNum)
{  
     I2cStart();

	 
     I2cWriteByte(uDevice&0xfe);
     if(I2cReadAck() == FALSE)
     {
          I2cStop();
          return 0;
     }
     
     /*
     I2cWriteByte(uAddr);
     if(I2cReadAck() == FALSE)
     {
          I2cStop();
          return OP_FAIL;
     }
     */  
	 
     I2cStart(); 
     I2cWriteByte(uDevice|0x01);


     if(I2cReadAck() == FALSE)
     {
          I2cStop();
          return 0;
     }
     

     while (uNum--)
     {
          *puBuf++ =I2cReadByte();

          if(uNum)
          	{
               Send_Ack(ACK);
          	}
          else  
          	{
               Send_Ack(NACK);
          	}
     }
     
     I2cStop();
     
     return 1;
}

#endif


/*--------------------------------------
;ģ������:Line_1A_WT588S(UC8 ddata)
;�� ��:ʵ��һ�ߴ���ͨ�ź���
;�� ��:ddata
;�� ��:
; one_line_DATA //������
;-------------------------------------*/

void Line_1A_WT588S (U8 ddata)
{
	U8 i;
	U8 B_DATA;
	U8  num_temp = 0;
	U16 ddata_temp, pdata_temp;
	
	P_DATA_OUT_HIGH();

	
	ddata_temp = ddata;
	pdata_temp = ddata_temp& 0x00FF;
	ddata_temp >>= 8;
	pdata_temp <<= 8;
	ddata_temp |= pdata_temp;//�û����ݸ�ֵ
	num_temp = 16;
	P_DATA_OUT_LOW();
	delay_ms( 5 );  
	B_DATA = (U8)( ddata_temp & 0x0001 );
	for( i=0; i<num_temp; i++ )
	{
		if ( i == 8 )
		{
			P_DATA_OUT_HIGH();
			delay_ms( 2 ); 
			P_DATA_OUT_LOW();
			delay_ms( 5 ); 
		} 
		P_DATA_OUT_HIGH();
		if ( B_DATA == 0 )
		{  
			delay_100us(2);  
			P_DATA_OUT_LOW();
			delay_100us(6);  
		} 
		else
		{ 
			delay_100us(6);  
			P_DATA_OUT_LOW();
			delay_100us(2);  
		} 
		ddata_temp = ddata_temp >> 1;
		B_DATA = (U8)( ddata_temp & 0x0001 );
	}
	P_DATA_OUT_HIGH();
}


