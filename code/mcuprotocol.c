/**
 **************************************************************************************************
 * @file        mcuprotocol.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "mcuprotocol.h"

/**
 * @addtogroup    XXX 
 * @{  
 */

/**
 * @addtogroup    mcuprotocol_Modules 
 * @{  
 */

/**
 * @defgroup      mcuprotocol_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      mcuprotocol_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      mcuprotocol_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      mcuprotocol_Private_Types
 * @brief         
 * @{  
 */
 

 
 
typedef struct
{
    uint8_t buf[MCUPROTOCOL_BUFFER_SIZE_MAX + 50];
    uint16_t index;
    void (*recv)(uint8_t *pBuf,uint16_t length);
}MCUprotocolFlowCache_t;

#if MCUPROTOCOL_PACKETANALYSIS_ENABLE == 1
typedef struct
{
    void (*recv)(uint8_t *pBuf,uint16_t length);
}MCUprotocolPacketCache_t;
#endif

/**
 * @}
 */
 
/**
 * @defgroup      mcuprotocol_Private_Variables 
 * @brief         
 * @{  
 */
static MCUprotocolFlowCache_t s_MCUprotocolFlowCacheList[MCUPROTOCOL_FLOWCHANNEL_COUNT];

#if MCUPROTOCOL_PACKETANALYSIS_ENABLE == 1
static MCUprotocolPacketCache_t s_MCUprotocolPacketCacheList[MCUPROTOCOL_PACKETCHANNEL_COUNT] = 
{
    NULL,
};
#endif
/**
 * @}
 */
 
/**
 * @defgroup      mcuprotocol_Public_Variables 
 * @brief         
 * @{  
 */
uint8_t g_MCUprotocol_Seq = 0;

MCUprotocolPacket_t g_MCUprotocolMakeCache;
/**
 * @}
 */

/**
 * @defgroup      mcuprotocol_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static uint16_t mcuprotocol_flow_decode(uint8_t channel);
/**
 * @}
 */

/**
 * @defgroup      mcuprotocol_Functions 
 * @brief         
 * @{  
 */
 
// ---------------- Flow Analgysis ------------------
static uint16_t mcuprotocol_flow_decode(uint8_t channel)
{
    uint16_t offset = 0;
    uint16_t remove_count = 0;
    uint16_t remain_len = s_MCUprotocolFlowCacheList[channel].index;
    uint8_t *pBuf = s_MCUprotocolFlowCacheList[channel].buf;

    while (remain_len >= sizeof(MCUprotocolAHR_t) + sizeof(MCUprotocolAFR_t))
    {
        if (pBuf[MCUPROTOCOL_AHR_SIGN_OFFSET + offset] == MCUPROTOCOL_AHR_SIGN)
        {
            uint16_t len;
            len = pBuf[MCUPROTOCOL_AHR_LENGTH_OFFSET + offset]+pBuf[MCUPROTOCOL_AHR_LENGTH_OFFSET + offset+1]*256;
            if (len <= MCUPROTOCOL_BUFFER_SIZE_MAX)
            {
                if (remain_len >= len)
                {
                    if (pBuf[MCUPROTOCOL_AHR_SIGN_OFFSET + offset + len - 1] == MCUPROTOCOL_AFR_SIGN)
                    {   
                        if (s_MCUprotocolFlowCacheList[channel].recv != NULL)
                        {   
                            s_MCUprotocolFlowCacheList[channel].recv(&pBuf[MCUPROTOCOL_AHR_SIGN_OFFSET+offset], len);
                            
                        }
                        remain_len -= len;
                        remove_count += len;
                        offset += len;
                    }
                    else
                    {
                        offset++;
                        remain_len--;
                        remove_count++;
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                offset++;
                remain_len--;
                remove_count++;
            }
        }
        else
        {
            offset++;
            remain_len--;
            remove_count++;
        }
    }

    if (offset != 0)
    {
        for (int i=0; i < remain_len; i++)
        {
            pBuf[i] = pBuf[offset+i];
        }
    }

    return remove_count;
}

void MCUprotocol_FlowAnalysis(uint8_t channel,uint8_t *pBuf,uint16_t length)
{
    uint16_t empty_count = 0,read_count = 0;

    if (length > 0)
    {
        empty_count = sizeof(s_MCUprotocolFlowCacheList[channel].buf) - s_MCUprotocolFlowCacheList[channel].index;  

        if (length > empty_count)    
        {
            read_count = empty_count;
        }
        else
        {
            read_count = length;
        }
        memcpy(&s_MCUprotocolFlowCacheList[channel].buf[s_MCUprotocolFlowCacheList[channel].index],pBuf,read_count); 
        s_MCUprotocolFlowCacheList[channel].index += read_count;   
        s_MCUprotocolFlowCacheList[channel].index -= mcuprotocol_flow_decode(channel);
    }
} 

void MCUprotocol_FlowSetCallback(uint8_t channel,void (*recv)(uint8_t *pBuf,uint16_t length))
{
    s_MCUprotocolFlowCacheList[channel].recv = recv;
}

uint16_t MCUprotocol_FlowGetCacheIdleLength(uint8_t channel)
{   
    return (sizeof(s_MCUprotocolFlowCacheList[channel].buf) - s_MCUprotocolFlowCacheList[channel].index);
}

// --------------------------------------------


// -------------- encode ----------------------
uint8_t MCUprotocol_AddTlv(uint8_t * disaddr,MCUprotocolTLV_t * tlv_value)
{
	disaddr[0] = tlv_value->Tag;
	disaddr[1] = tlv_value->Len;
	memcpy(&disaddr[2],tlv_value->Value.Array,tlv_value->Len);
	return tlv_value->Len + 2;
}

uint8_t MCUprotocol_GetChecksum(uint8_t *buf , uint16_t len)
{
	uint8_t checksum = 0;

	while(len --)
	{
		checksum += *(buf ++);
	}	
	return checksum;
}

uint16_t MCUprotocol_GetChecksum16bits(uint8_t *buf , uint16_t len)
{
	uint16_t checksum = 0;

	while(len --)
	{
		checksum += *(buf ++);
	}	
	return checksum;	
}


uint8_t * MCUprotocol_AddPayload(uint8_t * buf_ptr , uint8_t * payload , uint16_t len)
{
	memcpy(buf_ptr , payload , len);
	return buf_ptr + len;

}


/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

