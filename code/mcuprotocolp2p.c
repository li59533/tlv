/**
 **************************************************************************************************
 * @file        mcuprotocolp2p.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "mcuprotocolp2p.h"

/**
 * @addtogroup    XXX 
 * @{  
 */

/**
 * @addtogroup    mcuprotocolp2p_Modules 
 * @{  
 */

/**
 * @defgroup      mcuprotocolp2p_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      mcuprotocolp2p_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      mcuprotocolp2p_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      mcuprotocolp2p_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      mcuprotocolp2p_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      mcuprotocolp2p_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      mcuprotocolp2p_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static void (* mcu_p2p_checksum_err)(void);
static void (* mcu_p2p_checkseq_err)(void);
static void (* mcu_p2p_frametype_data)(uint8_t *buf , uint16_t len);

static int8_t mcu_p2p_checksum(uint8_t *buf , uint16_t len );

/**
 * @}
 */

/**
 * @defgroup      mcuprotocolp2p_Functions 
 * @brief         
 * @{  
 */



void MCU_P2P_ChecksumSet_Callback( void(* callback)(void))
{
	mcu_p2p_checksum_err = callback;
}

void MCU_P2P_CheckseqSet_Callback( void(* callback)(void))
{
	mcu_p2p_checkseq_err = callback;
}

void MCU_P2P_FrameTypeDataSet_Callback(void (*callback)(uint8_t * buf ,uint16_t len))
{
	mcu_p2p_frametype_data = callback;
}

void MCU_P2P_Decode(uint8_t * buf , uint16_t len )
{
	MCUprotocolp2p_t * MCUprotocolp2p = (MCUprotocolp2p_t * ) buf;
	static uint8_t seq = 0 ; 
	// -------- Check sum ------------
	if(mcu_p2p_checksum(buf , len) != 1)
	{
		if(mcu_p2p_checkseq_err != NULL)
		{
			mcu_p2p_checkseq_err();
		}
		return;
	}
	// -------------------------------
	// -------- Check Seq ------------
	if(seq == MCUprotocolp2p->Seq)
	{
		if(mcu_p2p_checksum_err != NULL)
		{
			mcu_p2p_checksum_err();
		}
		return;
	}
	// -------------------------------
	// -------- Check FCF ------------
			// -- ConnType --
	if(MCUprotocolp2p->FCF.ConnType != MCUPROTOCOL_CONNTYPE_P2P)
	{
		return  ;
	}
			// --------------
			// -- Getway --
	if(MCUprotocolp2p->FCF.Gateway == 1)
	{
		
	}
			// --------------
			// -- FrameType --
	if(MCUprotocolp2p->FCF.FrameType == MCUPROTOCOL_FRAMETYPE_ACK)
	{
		
	}
	else
	{
		if(mcu_p2p_frametype_data != NULL)
		{
			mcu_p2p_frametype_data(&MCUprotocolp2p->Cmd , MCUprotocolp2p->Length - 7);
		}

	}
			// --------------
	
	// -------------------------------
	
	
}

static int8_t mcu_p2p_checksum(uint8_t *buf , uint16_t len )
{
	uint8_t sum_value = 0 ;
	
	for(uint16_t i = 3 ; i < len - 2 ; i ++)
	{
		sum_value += buf[i];
	}
	
	if(buf[len - 2] == sum_value)
	{
		return 1;
	}
	else
	{
		return -1;
	}
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

