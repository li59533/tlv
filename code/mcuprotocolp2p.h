/**
 **************************************************************************************************
 * @file        mcuprotocolp2p.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _MCUPROTOCOLP2P_H_
#define _MCUPROTOCOLP2P_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
#include "mcuprotocol.h"
/**
 * @addtogroup    mcuprotocolp2p_Modules 
 * @{  
 */

/**
 * @defgroup      mcuprotocolp2p_Exported_Macros 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      mcuprotocolp2p_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      mcuprotocolp2p_Exported_Types 
 * @{  
 */
 
 
 
 
 
 
 
#pragma pack(1)


typedef struct
{
    uint8_t 	Head; // 帧头
    uint16_t 	Length; // 数据包总长度

	MCUprotocolFCF_t FCF;	// 帧控制域
	uint8_t  Seq;	// 序列号

	uint8_t Cmd; // 命令类型
	uint8_t *CmdPayload; // 命令内容 


    uint8_t FCS; // 帧校验
    uint8_t Foot; // 帧尾        
}MCUprotocolp2p_t;

#pragma pack()

/**
 * @}
 */

/**
 * @defgroup      mcuprotocolp2p_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      mcuprotocolp2p_Exported_Functions 
 * @{  
 */
void MCU_P2P_Decode(uint8_t * buf , uint16_t len );

void MCU_P2P_ChecksumSet_Callback( void(* callback)(void));
void MCU_P2P_CheckseqSet_Callback( void(* callback)(void));
void MCU_P2P_FrameTypeDataSet_Callback(void (*callback)(uint8_t * buf ,uint16_t len));

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
