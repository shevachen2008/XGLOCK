
#include "main.h"

char *strx;
char  RxBuffer[512];

char  IMEI_code[16]={0};
char  IMSI_code[16]={0};
char  ICCID_code[21]={0};
//char  Psk_code [32] = {0};


//char  aaa_code[16]={0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};

//char  Psk_code [32]={"a64fe9a25f08b971a5819da241f5d905"};
//char  Psk_code [32]={"a8ae0b9df3c307b05070836126e32689"};
//char  Psk_code [32]={"07dcee885b7e0259ac4fede34004b3fb"};  // 117  
//char  Psk_code [32]={"2b4db4b4fc714f99d6061e3ea39dbc06"};  // FCC
//char  Psk_code [32]={"4274556c1f387319ff05247e34d6963c"};  // FCC1
//char  Psk_code [32]={"325c3700afe6ae39e20b6099d3849f8c"};   // TEST616
//char  Psk_code [32]={"0b950dce5fcaa5a316b90f30d0fd760b"};   // TEST366
//char  Psk_code [32]={"c72e2b23ee5083f08c0cd38867e1d79b"};   // TEST673
//char	Psk_code [32]={"1e8a6e9391c5fbaccaf820814bc84ad8"}; 	// TEST358
//char	Psk_code [32]={"342874ccaba44b95f85b3e03f6ccbb9b"}; 	// TEST067
//char	Psk_code [32]={"4274556c1f387319ff05247e34d6963c"}; 		// TEST923

char	Psk_code [32]={"82b86b55a03354e3072fba16248c5f49"}; 		

typedef struct
 {
    const char IMSI_TestBuf[15]; 
 }IMSIType;

const IMSIType  IMSI_TestTable[ ] = 
 {
	 "460042624209326",
	 "460042624209346", //  
 };

 

U8    NBconectFlag;  
U16   NBstatus;   
U16   NBtimer;   
U8    NBInitFlag;
U8    ReadIMEIFlag;

U8    NBdelayCnt;


U16   RxCounter;
BC26  BC26_Status;
U8    socketnum = 0;//当前的socket号码

U8  Bcdbuf[7]; 
U8  Hexbuf[7]; 
U64 MacData;
U8  QENGFlag;


Connect_StateType  cStatus;    //  连接状态
NB_RECV_PACK       nb_recv_pack;

#define NOBOUND			       0X000A
#define NETOK              0X00AA
#define AGTESTMODE		     0X0AAA
#define AUTHOK			       0XAAAA


/**
* 获取AT命令响应后每一个参数值，适用格式： 参数，参数，参数，...
*
* @author qhq
*
* @param srcStr 指向AT命令响应的内容
* @param word  保存每一个参数值
* @param size  word的个数
*
* @return int  返回实际word数
*/
int at_get_para(char chop,char *srcStr, char **word, int size)
{
  int index = 0;
  int i = 0;
  char *str = srcStr;
  
  while (*(str + i) != '\0')
  {
    if (*(str + i) == chop)
    { 
      word[index] = str;
      word[index++][i] = '\0';
      str = (str + i + 1);
      i = -1;
    }
    
    else if((*(str + i) == '\r')||(*(str + i) == '\n'))
    {
      str=(str+i);
      i=0;
    }
    if (index >= size)
    {
      return index;
    }
    i++;
  }
  
  if (strlen(str) > 0)
  {
    word[index++] = str;
  }
  return index;
}



void  hexToAscs(uint8_t hex,char *ascs)
{
  
  uint8_t h,l;
  h=(hex>>4)&0x0f;
  l=(hex&0x0f);
  
  
  if(h<=9)   //lgf0503  ((h>=0)&&(h<=9))
    ascs[0]=h+0x30;
  else if((h>=10)&&(h<=15)){
    ascs[0]=h+0x41-10;
  }else{
    ascs[0]=0xff;
  }
  
  if(l<=9)   //lgf0503   ((l>=0)&&(l<=9))
    ascs[1]=l+0x30;
  else if((l>=10)&&(l<=15)){
    ascs[1]=l+0x41-10;
  }else{
    ascs[1]=0xff;
  }  
}

uint16_t HexsToAscs(uint8_t *hexs,char * ascs,uint16_t length)
{
  uint16_t i,j=0;
  for( i=0;i<length;i++){
    hexToAscs(hexs[i],ascs+j);
    j+=2;
  } 
  return j;
}

uint8_t charsToHex(char *asc)
{
  
  uint8_t hex=0;
  if((asc[0]>='0')&&(asc[0]<='9')){
    hex=asc[0]-0x30;
  }
  else if((asc[0]>='a')&&(asc[0]<='f')){
    hex=asc[0]-'a'+0xa;
  }
  else if((asc[0]>='A')&&(asc[0]<='F')){
    hex=asc[0]-'A'+0xa;
  }
  
  hex=hex<<4;
  
  if((asc[1]>='0')&&(asc[1]<='9')){
    hex+=(asc[1]-0x30);
  }
  else if((asc[1]>='a')&&(asc[1]<='f')){
    hex+=(asc[1]-'a'+0xa);
  }
  else if((asc[1]>='A')&&(asc[1]<='F')){
    hex+=(asc[1]-'A'+0xa);
  } 
  
  return hex;
}
uint16_t  ascsToHexs(char *ascs,uint8_t * hexs,uint16_t length)
{
  uint16_t i,j=0;
 for( i=0;i<length;i+=2){
    hexs[j++]=charsToHex(ascs+i);
  } 
  return j;
}

//基于查表实现BCD与Ascii之间的转换
static uint8 ascii2bcd1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
static uint8 ascii2bcd2[6]  = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

uint32  ASC2BCD(uint8 *bcd,  char *asc, uint32 len)  
{  
    uint8 c = 0;  
    uint8 index = 0;  
    uint8 i = 0;    
    len >>= 1;      
  
    for(; i < len; i++) {  
        //first BCD  
        if(*asc >= 'A' && *asc <= 'F') {  
            index = *asc - 'A';   
            c  = ascii2bcd2[index] << 4;  
        } else if(*asc >= '0' && *asc <= '9') {  
            index = *asc - '0';  
            c  = ascii2bcd1[index] << 4;  
        }  
        asc++;  
  
        //second BCD  
        if(*asc >= 'A' && *asc <= 'F') {  
            index = *asc - 'A';   
            c  |= ascii2bcd2[index];  
        } else if(*asc >= '0' && *asc <= '9') {  
            index = *asc - '0';  
            c  |= ascii2bcd1[index];  
        }  
        asc++;  
  
        *bcd++ = c;  
    }     
    return 0;  
}  
void BcdsTOhex(void)
{
	U8  MacTempBuf[8];
	U32 MacTemp;
	U64 MacData;
	Hexbuf[0] = (Bcdbuf[6]>>4)*10 +(0xf & Bcdbuf[6]);
	Hexbuf[1] = (Bcdbuf[5]>>4)*10 +(0xf & Bcdbuf[5]);
	Hexbuf[2] = (Bcdbuf[4]>>4)*10 +(0xf & Bcdbuf[4]);
	Hexbuf[3] = (Bcdbuf[3]>>4)*10 +(0xf & Bcdbuf[3]);
	Hexbuf[4] = (Bcdbuf[2]>>4)*10 +(0xf & Bcdbuf[2]);
	Hexbuf[5] = (Bcdbuf[1]>>4)*10 +(0xf & Bcdbuf[1]);
	Hexbuf[6] = (Bcdbuf[0]>>4)*10 +(0xf & Bcdbuf[0]);
	
	MacData = ((U64)Hexbuf[6]*1000000000000) 
	+((U64)Hexbuf[5]*10000000000) 		
	+((U64)Hexbuf[4]*100000000) 		
	+((U64)Hexbuf[3]*1000000)
	+((U64)Hexbuf[2]*10000)
	+((U64)Hexbuf[1]*100) 
	+ Hexbuf[0];

  C64HEX2BYTESMSB(MacTempBuf,MacData);
	MacTempBuf[2] = MacTempBuf[2]|0xC0;
	memcpy(Sys_PataCfg.MacBuf,&MacTempBuf[2],6 ); 
	MacTemp = BUILD_UINT32(MacTempBuf[7], MacTempBuf[6], MacTempBuf[5], MacTempBuf[4]);
	if ( MacTemp != Sys_PataCfg.Mac )
		{
			Sys_PataCfg.Mac = MacTemp;
			Sys_StorePara();
		}
}


// +QLWRD: 0,"write",31908,19,1,0,2,4,"32313231",0
// "write",31908,19,1,0,2,4,"32313231",0
U8 convert_rec_data(char *buf)
{
   char *words[8] = { NULL };
   int i;
   i=at_get_para(',',buf,words,8);
   if(i>7)
   {
     nb_recv_pack.msg_id = atoi(words[1]);
     nb_recv_pack.obj_id = atoi(words[2]);
     nb_recv_pack.ins_id = atoi(words[3]); 
     nb_recv_pack.res_id = atoi(words[4]);
     nb_recv_pack.value_type = atoi(words[5]);
     nb_recv_pack.len = atoi(words[6]);

	 //memcpy ( Tbuf1,words[7]+1,nb_recv_pack.len<<2);
     ascsToHexs(words[7]+1,(U8*)nb_recv_pack.data,nb_recv_pack.len<<2); 
     ascsToHexs(nb_recv_pack.data,(U8*)Com_getNbBuf,nb_recv_pack.len); 
     return 1;
   }
   else
   {
      return 0;
   } 
}


void NBstatus_StorePara(void)
{
	Flash_ComType eepdata;
	
	eepdata.pData = (uint8_t *)&NBstatus;
	eepdata.Lenth = sizeof(NBstatus);
	eepdata.Add = NBSTATUS_OFFSET;
	Flash_IoCtl(FLASH_ERASE_SECT,&eepdata.Add);
	Flash_Write(&eepdata);
}


void NBstatus_LoadPara(void)
{
	Flash_ComType eepdata;
	eepdata.pData = (uint8_t *)&NBstatus;
	eepdata.Lenth = sizeof(NBstatus);
	eepdata.Add = NBSTATUS_OFFSET;
	Flash_Read(&eepdata);
}


void Clear_Buffer(void)//清空缓存
{
		u16 i;
		for(i=0;i<512;i++)
		RxBuffer[i]=0;//缓存
		RxCounter=0;
}

void ReadIMEI(void)
{
    static U8 cntAT;
    PWRKEYON();       //  拉低 
    delay_ms(1000);
    PWRKEYOFF();       //  拉高正常开机
    NBAT_SEND("AT\r\n"); 
    delay_ms(200);

    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        NBAT_SEND("AT\r\n"); 
        delay_ms(200);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
        if ( ++ cntAT >= 20 )
        	{
        	  cntAT = 0;
						Line_1A_WT588S( 0x02 ); 		 //one
						delay_ms(500);
						Line_1A_WT588S( 0x1f ); 		 //check
						
						HARDWARE_SetupUSART4();
						return;
        	}
    }

	//	Line_1A_WT588S( 0x56 ); 			      //Connecting, please wait

    Clear_Buffer(); 
		NBAT_SEND("ATE0\r\n");  
    delay_ms(300); 
		

		Clear_Buffer(); 
    NBAT_SEND("AT+CGSN=1\r\n"); //获取IMEI号
    delay_ms(500);
		
    strx=strstr((const char*)RxBuffer,(const char*)"+CGSN:"); 

		if ( strx )
			{
				memcpy ( IMEI_code , strx+7 , 15 );
			}

		 ASC2BCD(Bcdbuf, &IMEI_code[1], 14); 
		 BcdsTOhex();
}


uint8  cFlag;
U8 TBUF[20];

void BC26_Init(void)
{
		U8 index;  
		static U8 cntCIMI , cntCGATT;

    PWRKEYON();       //  拉低 
    delay_ms(1000);
    PWRKEYOFF();       //  拉高正常开机

    NBAT_SEND("AT\r\n"); 
    delay_ms(200);
    strx=strstr((const char*)RxBuffer,(const char*)"OK"); 
		
		// if(strx==NULL)//  
		 {
				RESETON(); 
				delay_ms(500);
				RESETOFF(); 						 
		 }

		if ( AgtestFlag )    AgtestNbError = 1;
    NBAT_SEND("AT\r\n"); 
    delay_ms(200);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        NBAT_SEND("AT\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    }

		Clear_Buffer(); 
		NBAT_SEND("AT+CFUN=0\r\n");     // 关射频 
    delay_ms(300); 
		
		Clear_Buffer(); 
		//NBAT_SEND("AT+CPSMS=0\r\n"); // 关 PSM 
		NBAT_SEND("AT+CPSMS=1,,,\"00000001\",\"00000000\"\r\n"); // 开 PSM 
    delay_ms(300); 

		Clear_Buffer(); 
		NBAT_SEND("ATE0\r\n");  
    delay_ms(200); 

		Clear_Buffer(); 
		NBAT_SEND("ATE0\r\n");  
    delay_ms(200); 

		Clear_Buffer(); 
		NBAT_SEND("AT+CFUN=1\r\n");     // 开射频 
    delay_ms(300); 


		Clear_Buffer(); 
    NBAT_SEND("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK"); 
    while(strx==NULL)
    {
        Clear_Buffer();	
        NBAT_SEND("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK,说明卡是存在的
        if ( ++ cntCIMI >= 60 )
        	{
        	  cntCIMI = 0;
						Line_1A_WT588S( 0x03 ); 		 //two
						delay_ms(500);
						Line_1A_WT588S( 0x1f ); 		 //check
						
						HARDWARE_SetupUSART4();
						return;
        	}
    }

		if ( strx )
			{
				memcpy ( IMSI_code , &RxBuffer[2], 15 );
			}

		Clear_Buffer(); 
    NBAT_SEND("AT+QCCID\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+QCCID:"); 

		if ( strx )
			{
				memcpy ( ICCID_code , strx+8, 20 );
			}

		Sys_PataCfg.AGTest = 0x0;

		if ( 0 == NBstatus )
			{
				Clear_Buffer(); 
				NBAT_SEND("AT+QLWCFG=\"auto_reg\",0\r\n");    //
				delay_ms(200);
			}
		else if ( AUTHOK == NBstatus )
			{
				Clear_Buffer(); 
				NBAT_SEND("AT+QLWCFG=\"auto_reg\",1\r\n");    //
				delay_ms(200);			
			}

		for ( index = 0;index < NUM_ELEMENTS(IMSI_TestTable); index++ )
	    {
        if ( memcmp(&IMSI_TestTable[index], IMSI_code, 15) == 0) 
         	{
         	  index = NUM_ELEMENTS(IMSI_TestTable);
						Sys_PataCfg.AGTest = 0xAA;
            if ( 0 == AgtestFlag )
            	{
            	  AgtestFlag = 1;
								AgCmdSendFlag = 1;
								AgCmdSendcnt = 0;
            	}
						 NBAT_SEND("AT+QLWCFG=\"auto_reg\",0\r\n");
						 delay_ms(200);	
         	}
			}
		Sys_StoreFlag = STD_TRUE;
		
		Clear_Buffer(); 
    NBAT_SEND("AT+CGATT=1\r\n"); //激活网络，PDP
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返OK
    Clear_Buffer();	
    NBAT_SEND("AT+CGATT?\r\n");  //查询激活状态
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//返1  "+CGATT: 1"
    Clear_Buffer();	
		while(strx==NULL)
		{
            Clear_Buffer();	
            NBAT_SEND("AT+CGATT?\r\n");//获取激活状态
            delay_ms(300);
            strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//返回1,表明注网成功
						if ( ++ cntCGATT >= 30 )
							{
								cntCGATT = 0;
								Line_1A_WT588S( 0x04 ); 		 //three
								delay_ms(500);
								Line_1A_WT588S( 0x1f ); 		 //check
								
								HARDWARE_SetupUSART4();
								return;
							}
		}
		
		Clear_Buffer();	
		NBAT_SEND("AT+CESQ\r\n");//查看获取CSQ值
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+CESQ");//返回CSQ
		if(strx)
		{
				BC26_Status.CSQ=(strx[7]-0x30)*10+(strx[8]-0x30);   //获取CSQ
				if((BC26_Status.CSQ==99)||((strx[7]-0x30)==0))      //说明扫网失败
				{
					while(1)
					{
                     //   BC26_Status.netstatus=0;
						printf("信号搜索失败，请查看原因!\r\n");
                        RESETON();//拉低
                        delay_ms(300);
                        delay_ms(300);	
                        RESETOFF();//复位模块
												delay_ms(300);//没有信号就复位
					}
				}
        else
        {
					if ( AgtestFlag ) 	 AgtestNbError = 0;
        }
     }
		HARDWARE_SetupUSART4();
		if ( AUTHOK == NBstatus )   cFlag = 1; 
		QENGFlag = 1;
		
}

void BC26_Senddata(uint8_t *len,uint8_t *data)
{
		NBAT_SEND("AT+QLWNOTIFY=19,0,0,%d,%d,%s,0,1\r\n",
	        nb_recv_pack.value_type,len,data); 
}

void NB_Timer(void)
{
   if ( NBtimer > 0 )
   	{
	   if ( --NBtimer == 0 )
	   	 {
				 PwrStopCnt = 1;
	   	 }
   	}
}

void BC26_RECData(void)
{
	U8 i;
	
	if ( AUTHOK != NBstatus )
		{
			switch ( cStatus )
				{
					case CONNECT_ADDOBJ:
					strx=strstr((const char*)RxBuffer,(const char*)"+QLWREG: 0");
					if ( strx )
						{
							delay_ms(1000);	
						//	Clear_Buffer();
						//	NBAT_SEND("AT+QLWDELOBJ=19\r\n");
							
							strx=strstr((const char*)RxBuffer,(const char*)"buffer");
							if ( strx )
								{  
									 Clear_Buffer();
									 NBAT_SEND("AT+QLWCFG=\"auto_reg\",1\r\n");
									 delay_ms(200);
									 Clear_Buffer();
									 NBAT_SEND("AT+QLWRD\r\n");
									 delay_ms(200);
									 cFlag = 1;
									 cStatus = CONNECT_QLWRD;
									 break;
								}
							else
								{
									Clear_Buffer(); 	
								  NBAT_SEND("AT+QLWADDOBJ=19,0,1,0\r\n");
								  //NBAT_SEND("AT+QLWDELOBJ=19\r\n");
									delay_ms(200);
							    cStatus = CONNECT_ADDOBJ1;
								}
						} 
					break;

				case CONNECT_ADDOBJ1:			
					strx=strstr((const char*)RxBuffer,(const char*)"+QLWADDOBJ:");
					if ( strx )
						{
							Clear_Buffer();
							NBAT_SEND("AT+QLWADDOBJ=19,1,1,0\r\n");
							delay_ms(200);
							cStatus = CONNECT_QLWCFG;
						}
					break;
					
					case CONNECT_QLWCFG:						
						strx=strstr((const char*)RxBuffer,(const char*)"+QLWADDOBJ:");
						if ( strx )
							{
								NBAT_SEND("AT+QLWDEREG\r\n");
								delay_ms(300);								
								cStatus = CONNECT_QLWDEREG;
							}
						break;

				case CONNECT_QLWDEREG:
					strx=strstr((const char*)RxBuffer,(const char*)"+QLWDEREG: 0");
					if ( strx )
						{
						  delay_ms(1000);	
							Clear_Buffer();	
							NBAT_SEND("AT+QLWREG\r\n");
							delay_ms(300);
							cStatus = CONNECT_QLWREG;
						}
					break;

				case CONNECT_QLWREG:
					strx=strstr((const char*)RxBuffer,(const char*)"+QLWREG: 0");
					if ( strx )
						{
							cFlag = 1;
							cStatus = CONNECT_BUFFER;
						}
					break;

				default:
					break;
				}
		}

	if ( 0 == cFlag )  return;
	switch ( cStatus )
		{
				case CONNECT_QLWSTATUS:
					strx=strstr((const char*)RxBuffer,(const char*)"+QLWSTATUS: ");
					if ( strx )
						{
							if ( 0x30 == RxBuffer[14] ) 	// +QLWSTATUS:0  Not Registered
								{
									Clear_Buffer();
									NBAT_SEND("AT+QLWREG\r\n");
									delay_ms(200);
									cStatus = CONNECT_QLWREG;
								}
							else if ( 0x32 == RxBuffer[14]) 	// +QLWSTATUS:2 Registered 
								{
									cStatus = CONNECT_REPORT;
									if ( 2 == REGStatus  )
							    	{
											NB_msgSend();
										}
									if ( 3 == REGStatus  )
										{
											NB_LogSend();
										}	
								}
							else if ( 0x36 == RxBuffer[14]) 
								{						
									Clear_Buffer();
									cStatus = CONNECT_RECOVERD;								
								}
						}
					break;
				case CONNECT_RECOVERD:                    //  \"
					strx=strstr((const char*)RxBuffer,(const char*)"\"recovered\",0");
					if ( strx )
						{
							Clear_Buffer();
							NBAT_SEND( "AT+QLWUPDATE=0,86400\r\n");
							delay_ms(200);
							cStatus = CONNECT_UPDATE;
						}
					break;
				case	CONNECT_UPDATE:
					strx=strstr((const char*)RxBuffer,(const char*)"+QLWUPDATE: 0");
					if ( strx )
						{
							Clear_Buffer();
							NBAT_SEND( "AT+QLWSTATUS?\r\n");
							delay_ms(200);
							cStatus = CONNECT_QLWSTATUS;
						}
					break;					
					
				case CONNECT_QLWREG:
					strx=strstr((const char*)RxBuffer,(const char*)"+QLWREG: 0");
					if ( strx )
						{
							cStatus = CONNECT_BUFFER;
						}
					break;
					
			 case CONNECT_BUFFER:
				 strx=strstr((const char*)RxBuffer,(const char*)"buffer");
				 if ( strx )
					 {
							cStatus = CONNECT_QLWRD;
							Clear_Buffer();
							NBAT_SEND("AT+QLWRD\r\n"); 
							delay_ms(200);
					 }
				 break;

			case CONNECT_QLWRD:
					strx=strstr((const char*)RxBuffer,(const char*)"observe");
					if ( strx )
						{
							 strx=strstr((const char*)strx,(const char*)",");//??è?μ?μúò????oo?
							 for(i=0;;i++)
								{ 
										if(strx[i+1]==',')
										break;
										TBUF[i]=strx[i+1]; 
								}
							 cStatus = CONNECT_OBSERVE;
							 Clear_Buffer();
							 
							 NBAT_SEND("AT+QLWOBSRSP=%s,1,19,0,0,1,5,hello,0\r\n",TBUF); 
						}
					
					strx=strstr((const char*)RxBuffer,(const char*)"write");
					if ( strx )
						{
							if ( 1 == convert_rec_data( strx ))
								{
										cStatus = CONNECT_REPORT;
										Clear_Buffer();
										NBAT_SEND("AT+QLWWRRSP=%d,2\r\n",nb_recv_pack.msg_id); 

										parseServerAckPackage( );
								}
						}
				break;

			case CONNECT_OBSERVE:
				strx=strstr((const char*)RxBuffer,(const char*)"+QLWOBSRSP: 0");
				if ( strx )
					{
						cStatus = CONNECT_REPORT;
						Clear_Buffer();
						NBtimer = 80; 
						NBconectFlag = 1;
						
						Line_1A_WT588S( 0x1d ); 		   //Sucess

						if ( AUTHOK !=NBstatus )
							{
								NBstatus = AUTHOK;
								NBstatus_StorePara();
								delay_ms(200);
							}
						NB_InitRegMsg();
					
					//NB_LogREG( 10 );
					}
				strx=strstr((const char*)RxBuffer,(const char*)"+QLWOBSRSP: 1");
				if ( strx )
					{
						Clear_Buffer();
						NBAT_SEND("AT+QLWDEREG\r\n");
						delay_ms(500);			
						
						Clear_Buffer();
						NBAT_SEND("AT+QLWREG\r\n");
						delay_ms(300);
						cStatus = CONNECT_QLWREG;
					}
				break;

			case CONNECT_REPORT:
				strx=strstr((const char*)RxBuffer,(const char*)"report_ack");
				if ( strx )
					{
					  if ( 3 == REGStatus )
					  	{
					  	  if ( 2 == NbLogStatus)  NbLogStatus = 0;
					  	  AccRcord.Wrecordindex = 0;
								
								Line_1A_WT588S( 0x0A ); 			 //Sucess
								//RecordList_StorePara(0);
					  	}
						cStatus = CONNECT_BUFFER;
					}
				NBtimer = 150; 
				break;
			default:
				break;
		}

	strx=strstr((const char*)RxBuffer,(const char*)"buffer");
		if ( strx )
			{
				 cStatus = CONNECT_QLWRD;
				 Clear_Buffer();
				 NBAT_SEND("AT+QLWRD\r\n"); 
				 delay_ms(200);
			}
}

 //   \"
void BC26_ConLWM2M(void)      
{
  char ascbuf[32];
  if ((AUTHOK == NBstatus) || (0xAA == Sys_PataCfg.AGTest))   return;
	
	memset(ascbuf,0,sizeof(ascbuf));
	HexsToAscs(Psk_BUF,ascbuf,16);
	
	NBAT_SEND("AT+QLWCFG=\"auto_reg\",0\r\n");
  delay_ms(200);
	
	Clear_Buffer();
  NBAT_SEND( "AT+QLWSTATUS?\r\n");
	delay_ms(200);
  strx=strstr((const char*)RxBuffer,(const char*)"+QLWSTATUS: ");	
	if ( strx )
		{
		  if ( 0x30 == RxBuffer[14] )   // +QLWSTATUS:0  Not Registered
		  	{
		  	  Clear_Buffer();
         // NBAT_SEND("AT+QLWCONFIG=0,southbound.quectel.com,5684,\"%s\",86400,0,%s,%s\r\n",IMEI_code,IMEI_code,ascbuf); //Psk_code
          NBAT_SEND("AT+QLWCONFIG=0,southbound.quectel.com,5684,\"%s\",86400,0,%s,%s\r\n",IMEI_code,IMEI_code,Psk_code); //Psk_code

					delay_ms(300);
			    strx=strstr((const char*)RxBuffer,(const char*)"OK"); 
			    while(strx==NULL)
			    {
			       Clear_Buffer();	
						 NBAT_SEND("AT+QLWCONFIG=0,southbound.quectel.com,5684,\"%s\",86400,0,%s,%s\r\n",IMEI_code,IMEI_code,ascbuf);//Psk_code
						 delay_ms(300);
			       strx=strstr((const char*)RxBuffer,(const char*)"OK"); 
			    }

					NBAT_SEND("AT+QLWCFG=\"access_mode\",1\r\n"); 	// buffer
					delay_ms(200);


					Clear_Buffer();
				  NBAT_SEND("AT+QLWREG\r\n");
				  delay_ms(200);

					cStatus = CONNECT_ADDOBJ;
		  	}
			else if ( 0x31 == RxBuffer[14] || 0x32 == RxBuffer[14])   // +QLWSTATUS:1 Registering 
				{
				  cFlag = 1;
					cStatus = CONNECT_BUFFER;
				}
		}
}

void NB_Init( void )
{
  if ( --NBdelayCnt > 0 )   return;
	if ( NBInitFlag )		
		{
			NBInitFlag = 0;
			PwrStopCnt = 60;
			Disable_USART4IRQ();
			
			Line_1A_WT588S( 0x56 ); 						//Connecting, please wait
			BC26_Init();
			BC26_ConLWM2M();
	  }
}

void Read_IMEI( void )
{
	if ( ReadIMEIFlag )		
		{
			ReadIMEIFlag = 0;
			PwrStopCnt = 4;
			ReadIMEI();
			NBInitFlag = 1;
			NBdelayCnt = 2;
	  }
}
#if 0	

uint8_t HextoBcd( uint8_t dat )      //  读取时候用
  {
	 return (dat/10)*16 + dat%10;
  }


u8 csqcnt;
void NB_SendCsq(void)
{  
 //  static u8 csqcnt;
	 char ascbuf[2];
	 
if ( 0 == NBconectFlag )	return;
  
	if ( ++ csqcnt >= 59 )    //  59 
		{
		   csqcnt = 0;
			 ProtoAnaly_UpdateTime1();
	
	  if ( ProtoAnaly_RtcLocalTime - ProtoAnaly_RtcLocalTimeBak >=	4000) //4000 ) 
	  	{
	  	  NBheartbeatFlag = 1;
	  	}			 
			 
			// Line_1A_WT588S( 0x0b );				//Sucess
#if 0		
			 Clear_Buffer();
			 NBAT_SEND("AT+CESQ\r\n");//查看获取CSQ值
			 delay_ms(300);
			 strx=strstr((const char*)RxBuffer,(const char*)"+CESQ");//返回CSQ
			 if(strx)
			 {
					 BC26_Status.CSQ=(strx[7]-0x30)*10+(strx[8]-0x30);	 //获取CSQ
			 }		
       
			 memset(ascbuf,0,sizeof(ascbuf));
			 hexToAscs(HextoBcd(BC26_Status.CSQ),ascbuf);
			 Clear_Buffer();
		   NBAT_SEND("AT+QLWNOTIFY=19,0,0,1,8,CESQ = %s,0,1\r\n",ascbuf);
			 cStatus = CONNECT_REPORT;
//#else
      Clear_Buffer();
			NBAT_SEND("AT+QENG=0\r\n"); 
			delay_ms(500);
		//	NBAT_SEND("AT+QLWNOTIFY=19,0,0,1,200,%s,0,1\r\n",RxBuffer);
		//	cStatus = CONNECT_REPORT;      
#endif
		}
}
#endif

void NB_HeartBeat(void)
{
	if ( 0 == NBconectFlag )	return;
//	NBHeartBeatCnt++;

	if ( ++NBHeartBeatCnt >= 1750 )   //1750   
		{
		  NBHeartBeatCnt = 0;
		 	NBheartbeatFlag = 1;
		}
}


