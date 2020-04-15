#ifndef __BC26_H
#define __BC26_H	

typedef enum
{
	CONNECT_BUFFER = 0,
	CONNECT_QLWRD,
	CONNECT_OBSERVE, 
	CONNECT_READ, 
	CONNECT_REPORT, 
	CONNECT_WSTATUS,
	CONNECT_ADDOBJ,
	CONNECT_ADDOBJ1,
	CONNECT_QLWCFG,
	CONNECT_QLWREG,
	CONNECT_QLWDEREG,
	CONNECT_QLWSTATUS,
	CONNECT_RECOVERD,
	CONNECT_UPDATE,
}Connect_StateType;

#pragma  pack(push)  //保存对齐状态  
#pragma  pack(1) 
typedef struct
{
  U32  msg_id;
	U8   obj_id;
	U8   ins_id;
	U8   res_id;
	U8   value_type;
  U16  len;
  char data[512];
}NB_RECV_PACK;


typedef struct
{
    uint8_t CSQ;    
    uint8_t Socketnum;   //编号
    uint8_t reclen[10];   //获取到数据的长度
    uint8_t res;      
} BC26;

#pragma pack(pop)

extern U8    NBconectFlag;  
extern U16    NBtimer;   

extern char  RxBuffer[512]; 
extern uint16_t  RxCounter;
extern char *strx;
extern U8   TBUF[20];
extern Connect_StateType cStatus;
extern U16   NBstatus;   
extern char  IMEI_code[16];
extern char  IMSI_code[16];

extern NB_RECV_PACK  nb_recv_pack;
extern U8   NBInitFlag;
extern U8   ReadIMEIFlag;
extern char  ICCID_code[21];
extern U8    UpdateTimeFlag;
extern U8    NBdelayCnt;



void  hexToAscs(uint8_t hex,char *ascs);
uint16_t HexsToAscs(uint8_t *hexs,char * ascs,uint16_t length);
uint16_t  ascsToHexs(char *ascs,uint8_t * hexs,uint16_t length);

void OPEN_BC26(void);
void Clear_Buffer(void);//清空缓存	
void BC26_Init(void);
void BC26_PDPACT(void);
void BC26_ConUDP(void);
void BC26_RECData(void);
void BC26_Senddata(uint8_t *len,uint8_t *data);
void BC26_CreateSokcet(void);
void Clear_Buffer(void);
void NBstatus_LoadPara(void);
void BC26_ConLWM2M(void);
void NB_Timer(void);
void NB_Init(void);
void Read_IMEI( void );
void NB_SendCsq(void);
void NB_HeartBeat(void);
#endif    

