/**
 **************************************************************************************************
 * @file        lnprotocol.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _LNPROTOCOL_H_
#define _LNPROTOCOL_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    lnprotocol_Modules 
 * @{  
 */

/**
 * @defgroup      lnprotocol_Exported_Macros 
 * @{  
 */
/************************* system define **************/
#define LNPROTOCOL_AHR_SIGN                'L' //, lnproto header
#define LNPROTOCOL_AFR_SIGN                'N' //, lnproto footer

#define LNPROTOCOL_BUFFER_SIZE_MAX         500

#define LNPROTOCOL_AHR_SIGN_OFFSET         0   
#define LNPROTOCOL_AHR_LENGTH_OFFSET       1    
#define LNPROTOCOL_APD_DATA_OFFSET         3

#define LNPROTOCOL_CONNTYPE_P2P            0
#define LNPROTOCOL_CONNTYPE_TCPIP          1
#define LNPROTOCOL_CONNTYPE_802            2
#define LNPROTOCOL_CONNTYPE_BLE            3
#define LNPROTOCOL_CONNTYPE_UART           4

#define LNPROTOCOL_FRAMETYPE_DATA          0
#define LNPROTOCOL_FRAMETYPE_ACK           1

#define LNPROTOCOL_MODEL_BROADCAST         0xFFFF
#define LNPROTOCOL_MODEL_NOTCASE           0x0000

#define LNPROTOCOL_ADDR_BROADCAST          0xFFFF
#define LNPROTOCOL_ADDR_NOTCARE            0x0000
/************************** user define *****************/
#define LNPROTOCOL_FLOWANALYSIS_ENABLE     1
#define LNPROTOCOL_FLOWCHANNEL_COUNT       1
#define LNPROTOCOL_FLOWCHANNEL_MAINTAIN    0

#define LNPROTOCOL_PACKETANALYSIS_ENABLE   0
#define LNPROTOCOL_PACKETCHANNEL_COUNT     1

/**
 * @}
 */

/**
 * @defgroup      lnprotocol_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      lnprotocol_Exported_Types 
 * @{  
 */

#pragma pack(1)
typedef struct
{
    uint8_t Sign;
    uint16_t Length;
}LNprotocolAHR_t;

typedef struct
{
    uint8_t Sign;
}LNprotocolAFR_t;

typedef struct
{
    uint8_t Tag;
    uint8_t Len;
    union
    {
        uint8_t  Bit8;
        uint16_t Bit16;
        uint32_t Bit32;
        uint8_t  Array[100];
    }Value;
}LNprotocolTLV_t;

typedef struct
{
    uint8_t Buf[LNPROTOCOL_BUFFER_SIZE_MAX];
    uint16_t Length;
}LNprotocolPacket_t;

typedef struct
{
    LNprotocolPacket_t *pData;
    uint8_t In;
    uint8_t Out;
    uint8_t Size;
    uint8_t Count;
}LNprotocolPacketQueue_t;


#pragma pack()

/**
 * @}
 */

/**
 * @defgroup      lnprotocol_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      lnprotocol_Exported_Functions 
 * @{  
 */
// ----------------------decode -------------------------
void LNprotocol_FlowAnalysis(uint8_t channel,uint8_t *pBuf,uint16_t length);
void LNprotocol_FlowSetCallback(uint8_t channel,void (*recv)(uint8_t *pBuf,uint16_t length));
uint16_t LNprotocol_FlowGetCacheIdleLength(uint8_t channel);
// ---------------------- encode ------------------------
uint8_t LNprotocol_GetChecksum(uint8_t *buf , uint16_t len)
uint16_t LNprotocol_GetChecksum16bits(uint8_t *buf , uint16_t len)
uint8_t LNprotocol_AddTlv(uint8_t * disaddr,LN_Tlv_t * tlv_value);
uint8_t * LNprotocol_AddPayload(uint8_t * buf_ptr , uint8_t * payload , uint16_t len);
uint8_t LNprotocol_GetChecksum(uint8_t *buf , uint16_t len);
int8_t LNprotocol_Checksum(uint8_t * buf,uint16_t len);  // enter a complete buf



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
