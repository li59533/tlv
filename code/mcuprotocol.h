/**
 **************************************************************************************************
 * @file        mcuprotocol.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _MCUPROTOCOL_H_
#define _MCUPROTOCOL_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"





/**
 * @addtogroup    mcuprotocol_Modules 
 * @{  
 */




/**
 * @defgroup      mcuprotocol_Exported_Macros 
 * @{  
 */
/************************* system define **************/
#define MCUPROTOCOL_AHR_SIGN                0x7e //, mcuproto header
#define MCUPROTOCOL_AFR_SIGN                0x7e //, mcuproto footer

#define MCUPROTOCOL_BUFFER_SIZE_MAX         500

#define MCUPROTOCOL_AHR_SIGN_OFFSET         0   
#define MCUPROTOCOL_AHR_LENGTH_OFFSET       1    
#define MCUPROTOCOL_APD_DATA_OFFSET         3
// ------ConnType -----
#define MCUPROTOCOL_CONNTYPE_P2P            0
#define MCUPROTOCOL_CONNTYPE_TCPIP          1
#define MCUPROTOCOL_CONNTYPE_802            2
#define MCUPROTOCOL_CONNTYPE_BLE            3
#define MCUPROTOCOL_CONNTYPE_UART           4
// --------------------
// ------FrameType ----
#define MCUPROTOCOL_FRAMETYPE_DATA          0
#define MCUPROTOCOL_FRAMETYPE_ACK           1
// --------------------
#define MCUPROTOCOL_MODEL_BROADCAST         0xFFFF
#define MCUPROTOCOL_MODEL_NOTCASE           0x0000

#define MCUPROTOCOL_ADDR_BROADCAST          0xFFFF
#define MCUPROTOCOL_ADDR_NOTCARE            0x0000
/************************** user define *****************/
#define MCUPROTOCOL_FLOWANALYSIS_ENABLE     1
#define MCUPROTOCOL_FLOWCHANNEL_COUNT       1
#define MCUPROTOCOL_FLOWCHANNEL_CONF	    0

#define MCUPROTOCOL_PACKETANALYSIS_ENABLE   0
#define MCUPROTOCOL_PACKETCHANNEL_COUNT     1

/**
 * @}
 */

/**
 * @defgroup      mcuprotocol_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      mcuprotocol_Exported_Types 
 * @{  
 */
// ----- Std Cmd --------
typedef enum
{
    /* 所有ConnType均遵循的标准命令 */
	MCUCmd_NotSupport            = 0,    // 不支持的命令应答
	
	MCUCmd_StdGetCmdListReq      = 1,    // 获取支持的命令列表请求
	MCUCmd_StdGetCmdListResp     = 2,    // 获取支持的命令列表应答
	
	
	MCUCmd_StdBeatNotify         = 10,   // 心跳
    
    MCUCmd_StdFWVersionGetReq    = 11,   // 获取版本号请求
    MCUCmd_StdFWVersionGetResp   = 12,   // 获取版本号应答
    
    MCUCmd_StdTimeSetReq         = 13,   // 时间设置请求
    MCUCmd_StdTimeSetResp        = 14,   // 时间设置应答
    MCUCmd_StdTimeGetReq         = 15,   // 时间获取请求
    MCUCmd_StdTimeGetResp        = 16,   // 时间获取应答
    
    MCUCmd_StdDownloadNotify     = 20,	// 下载通知
    MCUCmd_StdDownloadNameReq    = 21,   // 文件名请求
    MCUCmd_StdDownloadNameResp   = 22,   // 文件名应答
    MCUCmd_StdDownloadInfoReq    = 23,	// 文件信息请求
    MCUCmd_StdDownloadInfoResp   = 24,	// 文件信息应答
    MCUCmd_StdDownloadDataReq    = 25,	// 文件数据请求
    MCUCmd_StdDownloadDataResp   = 26,	// 文件数据应答
    
    /* 按ConnType类型遵循的标准命令 */
    MCUCmd_StdConnModelGetReq    = 50,    // 获取设备的传输类型和设备型号请求
	MCUCmd_StdConnModelGetResp   = 51,    // 获取设备的传输类型和设备型号应答
	
    MCUCmd_StdConfigGetReq       = 60,	// 配置获取请求
    MCUCmd_StdConfigGetResp      = 61,	// 配置获取应答
    
    MCUCmd_StdConfigSetReq       = 62,	// 配置设置请求
    MCUCmd_StdConfigSetResp      = 63,	// 配置设备应答
    
    MCUCmd_StdStatusReq          = 64,	// 状态获取请求
    MCUCmd_StdStatusResp         = 65,	// 状态获取应答
    
    MCUCmd_StdScanReq            = 70,	// 扫描请求
    MCUCmd_StdScanResp           = 71,	// 扫描应答
    
    MCUCmd_StdAssociateReq,	    = 72,   // 关联请求
    MCUCmd_StdAssociateResp      = 73,   // 关联应答，传感->网关组网使用
    
    MCUCmd_StdRegisterReq        = 74,	// 注册请求，到服务器用
    MCUCmd_StdRegisterResp       = 75,	// 注册应答
}MCUCmd_e;
// ----------------------
// ----- Self Cmd Tag-------
typedef enum
{
	MCUCmd_SelfUPData = 128 , // �ϱ�����
}MCUCmd_Self_e;

#define UPDATA_DVALUE_TAG   0
#define UPDATA_CVALUE_TAG	1


// ----------------------

#pragma pack(1)
typedef struct
{
    uint8_t Sign;
    uint16_t Length;
}MCUprotocolAHR_t;

typedef struct
{
    uint8_t Sign;
}MCUprotocolAFR_t;


typedef	struct  
{
	unsigned ConnType:4; 	
	unsigned Gateway:1;	 	
	unsigned none_1:2;		
	unsigned FrameType:2;  
	unsigned none_2:2;  	
	unsigned Trans:1;
	unsigned Pending:1;
	unsigned Sec:1;
	unsigned Ack:1;
}MCUprotocolFCF_t;

typedef struct
{
    uint8_t Tag;
    uint8_t Len;
    union
    {
        uint8_t  Bit8;
        uint16_t Bit16;
        uint32_t Bit32;
        uint8_t  Array[50];
    }Value;
}MCUprotocolTLV_t;

typedef struct
{
    uint8_t Buf[MCUPROTOCOL_BUFFER_SIZE_MAX];
    uint16_t Length;
}MCUprotocolPacket_t;

typedef struct
{
    MCUprotocolPacket_t *pData;
    uint8_t In;
    uint8_t Out;
    uint8_t Size;
    uint8_t Count;
}MCUprotocolPacketQueue_t;


#pragma pack()

/**
 * @}
 */

/**
 * @defgroup      mcuprotocol_Exported_Variables 
 * @{  
 */
extern uint8_t g_MCUprotocol_Seq ;
/**
 * @}
 */

/**
 * @defgroup      mcuprotocol_Exported_Functions 
 * @{  
 */
 
// ----------------- include ----------------

#include "mcuprotocolp2p.h"
 
 
// ----------------------decode -------------------------
void MCUprotocol_FlowAnalysis(uint8_t channel,uint8_t *pBuf,uint16_t length);
void MCUprotocol_FlowSetCallback(uint8_t channel,void (*recv)(uint8_t *pBuf,uint16_t length));
uint16_t MCUprotocol_FlowGetCacheIdleLength(uint8_t channel);
// ---------------------- encode ------------------------
uint8_t MCUprotocol_GetChecksum(uint8_t *buf , uint16_t len);
uint16_t MCUprotocol_GetChecksum16bits(uint8_t *buf , uint16_t len);
uint8_t MCUprotocol_AddTlv(uint8_t * disaddr , MCUprotocolTLV_t * tlv_value);
uint8_t * MCUprotocol_AddPayload(uint8_t * buf_ptr , uint8_t * payload , uint16_t len);


/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
#endif
