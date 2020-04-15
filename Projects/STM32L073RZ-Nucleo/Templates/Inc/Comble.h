/****************************************************************************************
文件:   Comble.h
说明:
****************************************************************************************/
#ifndef _COMBLE_H__   //防重包含.
#define _COMBLE_H__
/***************************************************************************************/
#if 1
/******************************************************************************
**********************Macro definition*******************************************
******************************************************************************/
#define HEADEDATA	       (U8)(0x5A)    

#define HEADEDATASEND	   (U8)(0xA5)    

#define MAX_COMBUF	 250   

#define  PROTO_COMM_COMD_LENTHADD	1
#define  PROTO_COMM_COMD_OFFSET		1
#define  PROTO_COMM_COMD_CMDADD		2

#define  PROTO_COMM_BRG_LENTH			50//255


/******************************************************************************
**********************Type statement*******************************************
******************************************************************************/
typedef enum 
{
	COMD_GET_HEARTBEAT = 0x01,
	COMD_SET_IP = 0x02,
	COMD_GET_IP = 0x03,
	COMD_SET_REST = 0x04,
	COMD_SET_TIME = 0x05,
	COMD_GET_TIME = 0x06,
	COMD_CONFIG_HOST = 0x07, /* 配置通讯主控制器的参数 */
	COMD_READER_UNLOAD_CARD_ID = 0x08, /* 读卡片器上传ID	*/
	COMD_READER_WRITE = 0x09, /* 写卡*/
	COMD_READER_READ = 0x0A, /* 读卡*/
	COMD_SET_TIME_APP = 0x7D,       //
	COMD_GET_TIME_APP = 0x7C,       //

	COMD_GET_CARLOCK_FIFO = 0x0A,
	COMD_SET_CARLOCK_FIFO = 0x0b,

	CMD_O2O_READ_ID = 0x15,
	COMD_MODIFY_CONFIG = 0x16,	// 下发配置信息16
	COMD_READ_CONFIG,  // 17
	COMD_MODIFY_PWSD, 	//修改设备密码18
	COMD_MODIFY_DEFAULT_PASWD = 0x59, /* 更改默认密码*/

	CMD_O2O_READ_OTHER_KEY = 0x22,	// 读取锁体中22
	CMD_O2O_ADD_OTHER_KEY,	// 增加密码/卡片23
	CMD_O2O_DELETE_OTHER_KEY,  // 删除密码/卡片24
		 
	COMD_GET_ID_TOTAL = 0x25,
	COMD_GET_LIST = 0x26,
	COMD_SET_LIST = 0x27,                  //
	COMD_REMOVE_ID = 0x28,                 //
	COMD_LARNCARD_OK_ID = 0x2C,

	COMD_LOCK_UP = 0x1B,                   //
	COMD_POWER_ON = COMD_LOCK_UP,          //  
	COMD_LOCK_DOWN = 0x1C,                 //
	COMD_LOAD_RECORD = 0x46, /* 读取开锁记录*/          //
	COMD_REMOVE_RECORD = 0x47, /*移除记录*/

	CMD_O2O_OPEN_DEVICE_EXT = 0x1E,  // 开锁指令带IE

	CMD_ADD_OTHER_KEY,	// 增加密码/卡片23 没有邦定用户

	COMD_UPLOAD_RECORD = 0x40, /* 上传开锁记录*/
	COMD_SET_PROPERTY_BUILD_ID = 0x42,	/* 配置小区编号和楼栋编号*/
	COMD_GET_PROPERTY_BUILD_ID = 0x41,	/* 读取小区编号和楼栋编号*/
	COMD_GET_PUBLIC_BUILD_ID = 0x43,			/*	读取公共门楼栋编号*/
	COMD_SET_PUBLIC_BUILD_ID = 0x44,	/*	配置公共门楼栋编号*/
	COMD_DELETE_PUBLIC_BUILD_ID = 0x45,  /*  删除公共门楼栋编号*/
	
	COMD_SET_ID= 0x50,
	COMD_GET_ID= 0x51,

	COMD_BEEP= 0x52,
	COMD_GET_APN= 0x53,
	COMD_SET_APN= 0x54,

	COMD_ALARM= 0x55,
	COMD_ALWAYS_STATE= 0x56,
	COMD_SET_CONFIG_NEW = 0x5B,       //
	COMD_READ_CONFIG_NEW = 0x5C,      //
	COMD_SEND_WGID= 0x61,
	CMD_GET_SYS_INFO = 0x62, /* 获取设备系统信息 控制主机从蓝牙设备获取SYS_ID, MAC, PASSWORD */
	COMD_SET_RF_PARA = 0x6D,	
	COMD_GET_RF_PARA = 0x6E, 
	COMD_MODIFY_CUSTOM_INFO = 0x70, /* 更改客户代码*/
	CMD_DEFAULT_DEVICE = 0x79, /* 恢复出厂设置*/
	COMD_SET_PARA_DEF = CMD_DEFAULT_DEVICE,
	
	COMD_O2O_SET_TOUCH_SEN = 0x71,
	CMD_O2O_READ_VERSION = 0x7A,
	COMD_SET_DECRP_WORD = 0x7e,
	COMD_RESET_DEVICE = 0x7F,
	COMD_REQ_OPEN = 0x57,
	COMD_FACTORY_TEST = 0x85, /* 工厂测试*/
	COMD_AESKEY_SET=0xA1,     //AESKEY COMM

	COMD_INPUTPCODE_SET=0xA2,    
  COMD_INPUTPCARD_SET=0xA3, 		
  COMD_LOCK_SET=0xA4, 	
	COMD_PSK_SET=0xA5, 	
	COMD_MODE_SET=0xA6,	
	COMD_MODE_QUERY=0xA7, 
	COMD_LOG_SET=0xB2, 
	COMD_LOG_QUERY=0xB3, 
	COMD_GET_DAT = 0xFE
}ProtoCommDef_Type;

enum
{
	CONFIG_KEY_AUTOLOCK = 1, /* 自动闭锁+ 闭锁时间*/
	CONFIG_KEY_BTSAVEMODE,  /* 蓝牙省电模式 触摸后广播:1*/
	CONFIG_KEY_DETCARDSLEEP, /* 卡片一直工作:1*/
	CONFIG_KEY_DISIBLEDOORLOCK, /* 关闭门磁:1*/
};

enum
{
	CONFIG_VOICE = 1,  
	CONFIG_PASS,   
	CONFIG_NBMODE,  
	CONFIG_REFACTORY,	
};



typedef struct
{
	uint8 ComdId;
	void (*comdinmanage)(uint8_t *pindata,   uint8_t *poutdata);
}ProtoAnaly_ComdUpType;

/******************************************************************************
**********************Variable definition*******************************************
******************************************************************************/
extern U8 initpswFlag;
extern U8 SendDelayFlag;

extern U8 initpsw[5];
extern U8  Psk_BUF[16];

extern Std_ReturnType ProtoAnaly_AddId(uint8 idtpye,uint8 *idbuf);
extern void PR_UARTDATA(void);
extern void ProtoAnaly_UpdateTime(void);
extern void ProtoAnaly_UpdateTime1(void);

extern Std_ReturnType ProtoAnaly_AddId(uint8 idtpye,uint8 *idbuf);
extern Std_ReturnType ProtoAnaly_RemoveId(uint8 idtpye,uint8* id);

extern void LowBatteryMsgSendBle(void);
extern void NormalBatteryMsgSendBle(void);
extern void ResponseLockData(uint8_t cmd,uint8_t result);

extern void HallSwitchMsgSendBle(uint8_t mode,uint8_t stataus);
extern void ResetMsgSendBle(void);
extern void LockPadMsgSendBle(void);
extern void TestAddCode (void);

/******************************************************************************
**********************Function declared*******************************************
******************************************************************************/


#endif

/***************************************************************************************/
#endif
/****************************************************************************************
文件结束
****************************************************************************************/

