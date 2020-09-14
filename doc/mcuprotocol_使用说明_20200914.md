# mcuprotocol使用说明
版本：V0.0.1
## 1.文档说明
当前版本功能包括：
* P2P解包

## 2.使用方法
1.包含头文件“mcuprotocol.h”即可。
2.将函数```MCUprotocol_FlowAnalysis(uint8_t channel,uint8_t *pBuf,uint16_t length);```放置于数据接受的地方。用于接受数据。
3.编写一个Init()函数，用于注册回调函数。
```MCUprotocol_FlowSetCallback(uint8_t channel,void (*recv)(uint8_t *pBuf,uint16_t length));```
此注册函数用于注册**完成“头”、“长度”、“尾”**判断后所调用的回调函数。
4.目前对于P2P的设备，可以直接注册“mcuprotocolp2p.h”中的```void MCU_P2P_Decode(uint8_t * buf , uint16_t len );```
该函数判断FCS、Seq、FCF。

5.在init()函数中，继续注册。
```c
void MCU_P2P_ChecksumSet_Callback( void(* callback)(void));
void MCU_P2P_CheckseqSet_Callback( void(* callback)(void));
void MCU_P2P_FrameTypeDataSet_Callback(void (*callback)(uint8_t * buf ,uint16_t len));
```
```MCU_P2P_ChecksumSet_Callback```:注册如果**和校验**不通过的回调
```MCU_P2P_CheckseqSet_Callback```:注册**Seq重复**的回调
```MCU_P2P_FrameTypeDataSet_Callback```:**必须注册**的函数，用于回调解析payload的函数。当前的入参buf的指针指向CMD，len长度为payload长度。

6.对于payload的解析函数，由用户自行编写。
## 3.组包例子：
用户可参照一下例子进行P2P的协议组包
**P2P**：
```c
// -------------- Report Data -------------------
void APP_Dvalue_Report_data(void)
{
	uint8_t * buf_space ;
	uint8_t * payload_ptr ;
	MCUprotocolTLV_t MCUprotocolTLV;
	uint8_t len = 0;
	buf_space = pvPortMalloc(sizeof(uint8_t) * 100); //vPortFree()
	
	MCUprotocolp2p_t * MCUprotocolp2p = (MCUprotocolp2p_t * )buf_space;
	
	MCUprotocolp2p->Head = MCUPROTOCOL_AHR_SIGN;
	//MCUprotocolp2p->Length = ;
	MCUprotocolp2p->FCF.Ack = 0;
	MCUprotocolp2p->FCF.ConnType = MCUPROTOCOL_CONNTYPE_P2P;
	MCUprotocolp2p->FCF.FrameType = MCUPROTOCOL_FRAMETYPE_DATA;
	MCUprotocolp2p->FCF.Gateway = 0;
	MCUprotocolp2p->FCF.none_1 = 0;
	MCUprotocolp2p->FCF.none_2 = 0;
	MCUprotocolp2p->FCF.Pending = 0;
	MCUprotocolp2p->FCF.Sec = 0;
	MCUprotocolp2p->FCF.Trans = 0;
	MCUprotocolp2p->Seq = g_MCUprotocol_Seq ++;
	MCUprotocolp2p->Cmd = MCUCmd_SelfUPData;
	
	payload_ptr = &MCUprotocolp2p->Cmd + 1;
	
	MCUprotocolTLV.Tag = UPDATA_DVALUE_TAG;
	MCUprotocolTLV.Len = 4;
	memcpy(MCUprotocolTLV.Value.Array , (uint8_t *)&APP_Dvalue.D_value , 4);
	payload_ptr += MCUprotocol_AddTlv(payload_ptr ,&MCUprotocolTLV);
	len += 4;
	
	MCUprotocolp2p->Length = len + 9;
	MCUprotocolp2p->FCS = MCUprotocol_GetChecksum((uint8_t *)&MCUprotocolp2p->FCF , MCUprotocolp2p->Length - 5);
	MCUprotocolp2p->Foot = MCUPROTOCOL_AFR_SIGN;

	APP_Conf_Send_InQueue( &MCUprotocolp2p->Head , MCUprotocolp2p->Length);
	vPortFree(buf_space);
}

// ----------------------------------------------

```