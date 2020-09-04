/**
 **************************************************************************************************
 * @file        lnprotocol.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "lnprotocol.h"

/**
 * @addtogroup    XXX 
 * @{  
 */

/**
 * @addtogroup    lnprotocol_Modules 
 * @{  
 */

/**
 * @defgroup      lnprotocol_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      lnprotocol_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      lnprotocol_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      lnprotocol_Private_Types
 * @brief         
 * @{  
 */
typedef struct
{
    uint8_t buf[LNPROTOCOL_BUFFER_SIZE_MAX + 50];
    uint16_t index;
    void (*recv)(uint8_t *pBuf,uint16_t length);
}LNprotocolFlowCache_t;

#if LNPROTOCOL_PACKETANALYSIS_ENABLE == 1
typedef struct
{
    void (*recv)(uint8_t *pBuf,uint16_t length);
}LNprotocolPacketCache_t;
#endif

/**
 * @}
 */
 
/**
 * @defgroup      lnprotocol_Private_Variables 
 * @brief         
 * @{  
 */
static LNprotocolFlowCache_t s_LNprotocolFlowCacheList[LNPROTOCOL_FLOWCHANNEL_COUNT];

#if LNPROTOCOL_PACKETANALYSIS_ENABLE == 1
static LNprotocolPacketCache_t s_LNprotocolPacketCacheList[LNPROTOCOL_PACKETCHANNEL_COUNT] = 
{
    NULL,
};
#endif
/**
 * @}
 */
 
/**
 * @defgroup      lnprotocol_Public_Variables 
 * @brief         
 * @{  
 */
uint32_t g_LNprotocol_Seq = 0;

LNprotocolPacket_t g_LNprotocolMakeCache;
/**
 * @}
 */

/**
 * @defgroup      lnprotocol_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static uint16_t lnprotocol_flow_decode(uint8_t channel)
/**
 * @}
 */

/**
 * @defgroup      lnprotocol_Functions 
 * @brief         
 * @{  
 */



// ---------------- Flow Analgysis ------------------
static uint16_t lnprotocol_flow_decode(uint8_t channel)
{
    uint16_t offset = 0;
    uint16_t remove_count = 0;
    uint16_t remain_len = s_LNprotocolFlowCacheList[channel].index;
    uint8_t *pBuf = s_LNprotocolFlowCacheList[channel].buf;

    while (remain_len >= sizeof(LNprotocolAHR_t) + sizeof(LNprotocolAFR_t))
    {
        if (pBuf[LNPROTOCOL_AHR_SIGN_OFFSET + offset] == LNPROTOCOL_AHR_SIGN)
        {
            uint16_t len;
            len = pBuf[LNPROTOCOL_AHR_LENGTH_OFFSET + offset]+pBuf[LNPROTOCOL_AHR_LENGTH_OFFSET + offset+1]*256;
            if (len <= LNPROTOCOL_BUFFER_SIZE_MAX)
            {
                if (remain_len >= len)
                {
                    if (pBuf[LNPROTOCOL_AHR_SIGN_OFFSET + offset + len - 1] == LNPROTOCOL_AFR_SIGN)
                    {   
                        if (s_LNprotocolFlowCacheList[channel].recv != NULL)
                        {   
                            s_LNprotocolFlowCacheList[channel].recv(&pBuf[LNPROTOCOL_AHR_SIGN_OFFSET+offset], len);
                            
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

void LNprotocol_FlowAnalysis(uint8_t channel,uint8_t *pBuf,uint16_t length)
{
    uint16_t empty_count = 0,read_count = 0;

    if (length > 0)
    {
        empty_count = sizeof(s_LNprotocolFlowCacheList[channel].buf) - s_LNprotocolFlowCacheList[channel].index;  

        if (length > empty_count)    
        {
            read_count = empty_count;
        }
        else
        {
            read_count = length;
        }

        memcpy(&s_LNprotocolFlowCacheList[channel].buf[s_LNprotocolFlowCacheList[channel].index],pBuf,read_count); 
        s_LNprotocolFlowCacheList[channel].index += read_count;   
        s_LNprotocolFlowCacheList[channel].index -= lnprotocol_flow_decode(channel);
    }
}

void LNprotocol_FlowSetCallback(uint8_t channel,void (*recv)(uint8_t *pBuf,uint16_t length))
{
    s_LNprotocolFlowCacheList[channel].recv = recv;
}

uint16_t LNprotocol_FlowGetCacheIdleLength(uint8_t channel)
{   
    return (sizeof(s_LNprotocolFlowCacheList[channel].buf) - s_LNprotocolFlowCacheList[channel].index);
}

// --------------------------------------------


// -------------- encode ----------------------
uint8_t LNprotocol_AddTlv(uint8_t * disaddr,LN_Tlv_t * tlv_value)
{
	disaddr[0] = tlv_value->Tag;
	disaddr[1] = tlv_value->Len;
	memcpy(&disaddr[2],tlv_value->Value.Array,tlv_value->Len);
	return tlv_value->Len + 2;
}

uint8_t LNprotocol_GetChecksum(uint8_t *buf , uint16_t len)
{
	uint8_t checksum = 0;
	uint8_t * check_ptr = 0;
	uint16_t check_len = 0;
	
	check_ptr = buf + 1 ;
	check_len = len - 3 ;
	
	while(check_len --)
	{
		checksum += *(check_ptr ++);
	}	
	return checksum;
}


uint16_t LNprotocol_GetChecksum16bits(uint8_t *buf , uint16_t len)
{
	uint16_t checksum = 0;
	uint8_t * check_ptr = 0;
	uint16_t check_len = 0;
	
	check_ptr = buf + 1 ;
	check_len = len - 3 ;
	
	while(check_len --)
	{
		checksum += *(check_ptr ++);
	}	
	return checksum;	
}



int8_t LNprotocol_Checksum(uint8_t * buf,uint16_t len)  // enter a complete buf
{
	uint8_t checksum = 0;
	uint8_t * check_ptr = 0;
	uint16_t check_len = 0;
	
	check_ptr = buf + 1 ;
	check_len = len - 3 ;
	
	while(check_len --)
	{
		checksum += *(check_ptr ++);
	}
	
	if(checksum == *check_ptr)
	{
		return 1;
	}else
	{
		return -1;
	}
}



uint8_t * LNprotocol_AddPayload(uint8_t * buf_ptr , uint8_t * payload , uint16_t len)
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

