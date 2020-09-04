/**
 **************************************************************************************************
 * @file        lnprotocolp2p.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _LNPROTOCOLP2P_H_
#define _LNPROTOCOLP2P_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    lnprotocolp2p_Modules 
 * @{  
 */

/**
 * @defgroup      lnprotocolp2p_Exported_Macros 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      lnprotocolp2p_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      lnprotocolp2p_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      lnprotocolp2p_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      lnprotocolp2p_Exported_Functions 
 * @{  
 */
void LNprotocol_Rev_CallbackRegister(void (*callback)(uint8_t * cmd , uint16_t len));
void LNprotocol_Rev_Analysis_Flow(uint8_t *buf , uint16_t len);
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
