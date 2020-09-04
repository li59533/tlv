/**
 **************************************************************************************************
 * @file        lnprotocolp2p.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "lnprotocolp2p.h"
#include "lnprotocol.h"
/**
 * @addtogroup    XXX 
 * @{  
 */

/**
 * @addtogroup    lnprotocolp2p_Modules 
 * @{  
 */

/**
 * @defgroup      lnprotocolp2p_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      lnprotocolp2p_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      lnprotocolp2p_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      lnprotocolp2p_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      lnprotocolp2p_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      lnprotocolp2p_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      lnprotocolp2p_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
void (*rev_process)(uint8_t * cmd , uint16_t len);
/**
 * @}
 */

/**
 * @defgroup      lnprotocolp2p_Functions 
 * @brief         
 * @{  
 */

void LNprotocol_Rev_CallbackRegister(void (*callback)(uint8_t * cmd , uint16_t len))
{
    rev_process = callback;
}

void LNprotocol_Rev_Analysis_Flow(uint8_t *buf , uint16_t len)
{
    


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

