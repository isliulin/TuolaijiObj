#ifndef __can0
#define __can0
#include <mc9s12xhz512.h>
#include "type.h"
#include "Flash.h"

/**************condition macros***************/
#ifndef	_NO_can0_EXTERN
#define	_can0_EXTERN	extern
#else
#define	_can0_EXTERN 
#endif
///////////////////////////////////////////////////////////////
#pragma CODE_SEG IFsh1_CODE
void err_send(uchar data);
void CAN0Init(void);
void CAN0_ReadFrame(void);
void CAN0_SendFrame(void);
void address_or_checksum(void);
void rev_data(void);
void ack_send(void);
void data_add(void);
void out_wdt_reset(void);
static void	Pic_Buffer_Intial(void);
//////////////////////////////////////////////////////////
_can0_EXTERN  ulong count_delay;///计算当前是否有CAN数据
_can0_EXTERN  ulong   FirstS19Address;
_can0_EXTERN  uint  Sector_Number;///总共要擦除的扇区个数
_can0_EXTERN  uchar	 BootLoader_Content;///下载内容识别  
_can0_EXTERN  CAN_PACKET_T  send_can_packet;
_can0_EXTERN  uchar ResponseData;
_can0_EXTERN  CAN_PACKET_T 	rev_can_packet;
///主芯片内部FLASH 的相关变量
_can0_EXTERN  _S19_RX_BUF  S19DataBuf;///接收主芯片数据帧的环形缓存
_can0_EXTERN uchar s19RxCount;///接收的S19数据长度
_can0_EXTERN uchar s19LineLen;	///s19数据长度
///外挂FLASH的相关变量
_can0_EXTERN  uint	Sector_Start_Adr;///扇区的开始位置
_can0_EXTERN  uchar	Pic_S19_RX_Index;///图片接收数据的环形缓存的接收变量
_can0_EXTERN  uchar	Pic_S19_Write_Index;///图片写入数据的环形缓存的接收变量
_can0_EXTERN uchar Pic_Flash_Write_Len[S19_RX_MAX_SIZE];///写入图片数据的长度
_can0_EXTERN  _S19_RX_BUF  PicS19DataBuf[S19_RX_MAX_SIZE];///接收图片数据帧的环形缓存
_can0_EXTERN uchar RxPicS19LineFlag;///接收的图片数据整行是否完整标志
_can0_EXTERN uchar PicS19RxCount[S19_RX_MAX_SIZE];///接收的图片S19数据长度
_can0_EXTERN uchar PicS19LineLen[S19_RX_MAX_SIZE];	///图片s19数据长度

//////////////////////////////////////////////////////
#define GetMeterFunction()		rev_can_packet.data[0]
#define GetMeterS19Len()		rev_can_packet.data[1]

#define err_flash_unkown   				0xF0
#define err_flash_checksum 				0xF1
#define err_flash_lenth   	 					0xF2
#define err_flash_clear    						0xF3
#define	 err_flash_Write_checksum		0xF4

#define flash_OK					 				0x01

#define RX_Checksum_Error_ACK		0x0E

#define METER_ID 				0x10ff0117

#define	EXTERNAL_WATCHDOG_IO		DDRK_DDRK6
#define	EXTERNAL_WATCHDOG				PORTK_PK6

#pragma CODE_SEG DEFAULT               /* Change code section to DEFAULT. */


#endif




