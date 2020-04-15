/****************************************************************************************
�ļ�:   ADC.c
˵��:    
****************************************************************************************/
//ͷ�ļ�.
#include "main.h"

#define  POWERSIZE   9 
U16 value_AdPower_buf	[ POWERSIZE ];
 
/******************************** 
*��������swap 
*���ã�������������ֵ 
*������������������ 
*����ֵ���� 
********************************/  

void swap(U16 *a, U16 *b)    
  {  
      U16 temp;  
    
      temp = *a;  
      *a = *b;  
      *b = temp;  
      return ;  
  }  

/************************************ 
 *��������quicksort 
 *���ã����������㷨 
 *������ 
 *����ֵ���� 
 ************************************/  
void quicksort(U16 array[], U16 maxlen, U16 begin, U16 end)  
  {  
      int i, j;  
    
      if(begin < end)  
      {  
          i = begin + 1;  // ��array[begin]��Ϊ��׼������˴�array[begin+1]��ʼ���׼���Ƚϣ�  
          j = end;        // array[end]����������һλ  
              
          while(i < j)  
          {  
              if(array[i] > array[begin])  // ����Ƚϵ�����Ԫ�ش��ڻ�׼�����򽻻�λ�á�  
              {  
                  swap(&array[i], &array[j]);  // ����������  
                  j--;  
              }  
              else  
              {  
                  i++;  // �����������һλ���������׼���Ƚϡ�  
              }  
          }  
    
          /* ����whileѭ����i = j�� 
           * ��ʱ���鱻�ָ����������  -->  array[begin+1] ~ array[i-1] < array[begin] 
           *                           -->  array[i+1] ~ array[end] > array[begin] 
           * ���ʱ������array�ֳ��������֣��ٽ�array[i]��array[begin]���бȽϣ�����array[i]��λ�á� 
           * ���array[i]��array[begin]���������������ָ�ֵ������Դ����ƣ�ֱ�����i = j�������������˳��� 
           */  
    
          if(array[i] >= array[begin])  // �������Ҫȡ�ȡ�>=������������Ԫ������ͬ��ֵʱ������ִ���  
          {  
              i--;  
          }  
    
          swap(&array[begin], &array[i]);  // ����array[i]��array[begin]  
          quicksort(array, maxlen, begin, i);  
          quicksort(array, maxlen, j, end);  
      }  
  }  


U16 filter( U16 *pBUF, U16 dat, U8 size )
  {
    static U8 count; 
    pBUF[count++] = dat;
    if ( count < size )
     {
      return 0;
     }
    else
     {
       count = 0;
       quicksort(pBUF, size, 0, size-1);  // ��������  
       return pBUF[(size-1)/2]; 
     }
  }

//#define NUM_ELEMENTS(array) (sizeof(array) / sizeof(array[0]))
const U16 powerToPercent[][2]={{0xE9B, 100},
                               {0xE62, 95 },
                               {0xE2C, 90 },
                               {0xDF5, 85 },
                               {0xDBA, 80 },
                               {0xD84, 75 },
                               {0xD49, 70 },
                               {0xD13, 65 },
                               {0xCD9, 60 },
                               {0xCA4, 55 },
                               {0xC6B, 50 },
                               {0xC34, 45 },
                               {0xBFC, 40 },
                               {0xBAF, 35 },
                               {0xB70, 30 },
                               {0xB03, 25 },
                               {0xA99, 20 },
                               {0xA23, 15 },
                               {0x9B8, 10 },
                               {0x8C3, 5  },
                               {0x806, 0  },
                              };

/*
const U16 powerToPercent[][2]={{600, 100},
                               {591, 95 },
                               {582, 90 },
                               {573, 85 },
                               {564, 80 },
                               {555, 75 },
                               {546, 70 },
                               {537, 65 },
                               {528, 60 },
                               {519, 55 },
                               {510, 50 },
                               {501, 45 },
                               {492, 40 },
                               {480, 35 },
                               {470, 30 },
                               {452, 25 },
                               {435, 20 },
                               {417, 15 },
                               {400, 10 },
                               {360, 5  },
                               {320, 0  },
                              };
*/


U16  powerAD;
U16  powervalue;
U8   adccnt;

void GetPowervalue(void)
{
  static U8 lowPowercnt,Powercnt1 ,lowPowerFlag;
  U16 powervaluetemp;

  HAL_ADC_Start(&Power_adc);

  if (HAL_ADC_PollForConversion(&Power_adc, 10) != HAL_TIMEOUT)
  {
     powerAD = HAL_ADC_GetValue(&Power_adc);
  }
	
	powervaluetemp = filter( value_AdPower_buf , powerAD, sizeof(value_AdPower_buf));
	
	if ( 0 !=  powervaluetemp )		 powervalue = powervaluetemp;

  if ( 0 == adcFlag )
  	{
  	   if ( ++adccnt >= 50 )
  	   	{
  	   	  adcFlag = 1;
  	   	}
  	}

  if ( 0 == adcFlag )  return;
	if (( powervalue <= 0xB45 )&&(0 == lowPowerFlag))    //     4.6V
		{
			if ( ++lowPowercnt>= 10 )
				{
				  lowPowercnt = 0;
					lowPowerFlag = 1;
					LowBatteryMsgSendBle();
					CodeOtherType = LowBatteryType;
				  NB_otherMsgLoad( );
				}
		}

   if (( 1 == lowPowerFlag ) && ( powervalue >= 0xBA5))
   	 {
   	   if (++Powercnt1 >= 10 )
   	   	 {
   	   	   Powercnt1 = 0;
					 lowPowerFlag = 0;
					 NormalBatteryMsgSendBle();
   	   	 }
   	 }
}

void GetPowerPercent(void)
{
  U8 index;
	for ( index = 0;index < NUM_ELEMENTS(powerToPercent); index++)
		{
		  if ( powervalue >= powerToPercent[index][0] )
		  	{
		  	    Access_BatteryData =  powerToPercent[index][1];
					  index = NUM_ELEMENTS(powerToPercent);
		  	}
		}
}

