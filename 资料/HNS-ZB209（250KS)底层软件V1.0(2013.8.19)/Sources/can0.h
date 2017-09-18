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
_can0_EXTERN  ulong count_delay;///���㵱ǰ�Ƿ���CAN����
_can0_EXTERN  ulong   FirstS19Address;
_can0_EXTERN  uint  Sector_Number;///�ܹ�Ҫ��������������
_can0_EXTERN  uchar	 BootLoader_Content;///��������ʶ��  
_can0_EXTERN  CAN_PACKET_T  send_can_packet;
_can0_EXTERN  uchar ResponseData;
_can0_EXTERN  CAN_PACKET_T 	rev_can_packet;
///��оƬ�ڲ�FLASH ����ر���
_can0_EXTERN  _S19_RX_BUF  S19DataBuf;///������оƬ����֡�Ļ��λ���
_can0_EXTERN uchar s19RxCount;///���յ�S19���ݳ���
_can0_EXTERN uchar s19LineLen;	///s19���ݳ���
///���FLASH����ر���
_can0_EXTERN  uint	Sector_Start_Adr;///�����Ŀ�ʼλ��
_can0_EXTERN  uchar	Pic_S19_RX_Index;///ͼƬ�������ݵĻ��λ���Ľ��ձ���
_can0_EXTERN  uchar	Pic_S19_Write_Index;///ͼƬд�����ݵĻ��λ���Ľ��ձ���
_can0_EXTERN uchar Pic_Flash_Write_Len[S19_RX_MAX_SIZE];///д��ͼƬ���ݵĳ���
_can0_EXTERN  _S19_RX_BUF  PicS19DataBuf[S19_RX_MAX_SIZE];///����ͼƬ����֡�Ļ��λ���
_can0_EXTERN uchar RxPicS19LineFlag;///���յ�ͼƬ���������Ƿ�������־
_can0_EXTERN uchar PicS19RxCount[S19_RX_MAX_SIZE];///���յ�ͼƬS19���ݳ���
_can0_EXTERN uchar PicS19LineLen[S19_RX_MAX_SIZE];	///ͼƬs19���ݳ���

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




