#ifndef __can0
#define __can0
#include <MC9S12XET256.h>
#include "type.h"
#include "Flash.h"

/**************condition macros***************/
#ifndef	_NO_can0_EXTERN
#define	_can0_EXTERN	extern
#else
#define	_can0_EXTERN 
#endif
//ragma CODE_SEG __NEAR_SEG NON_BANKED
#pragma CODE_SEG DEFAULT               /* Change code section to DEFAULT. */

_can0_EXTERN void ready_bootloader(void);
_can0_EXTERN void err_send(void);
_can0_EXTERN void CAN0Init(void);
_can0_EXTERN void CAN0_ReadFrame(void);
_can0_EXTERN void CAN0_SendFrame(dword MessageID_send,byte length_send,byte *data_send);
_can0_EXTERN void address_or_checksum(void);
_can0_EXTERN void rev_data(void);
_can0_EXTERN void ack_send(void);
_can0_EXTERN void data_add(void);
_can0_EXTERN void chip_erase(void);
_can0_EXTERN void out_wdt_reset(void);
_can0_EXTERN void reset_stop(void);

_can0_EXTERN void pflash_chip_erase(void)	;


_can0_EXTERN uchar flag;
_can0_EXTERN uchar flag_add;

//ram macro
_can0_EXTERN uchar data_s[8];
_can0_EXTERN dword MessageID_s;
_can0_EXTERN uchar length_s;

_can0_EXTERN uchar data_value[8];
_can0_EXTERN uchar data_flash[100];
_can0_EXTERN uchar data_count;
_can0_EXTERN dword ID_value;
_can0_EXTERN uchar length_value;
_can0_EXTERN uchar err_command;
_can0_EXTERN uchar err;

_can0_EXTERN dword address_value;
_can0_EXTERN dword address_value_buf;
_can0_EXTERN dword data_address_in;
_can0_EXTERN dword data_address_out;
_can0_EXTERN byte data_s_buf;
_can0_EXTERN dword lenth_data;
_can0_EXTERN dword lenth_data_buf;
_can0_EXTERN byte status_commmd;
_can0_EXTERN uchar send_form;
_can0_EXTERN dword count_delay;
_can0_EXTERN dword clr_address_first;
_can0_EXTERN dword clr_address;

#define err_flash_unkown   00
#define err_flash_checksum 01
#define err_flash_form	   02
#define err_flash_lenth    03
#define err_flash_clear    04

#define status_address	   00
#define status_code        02
#define status_checksum    03
#define clr_add_first 	   05
#define clr_add_last 	     06

#pragma CODE_SEG DEFAULT               /* Change code section to DEFAULT. */


#endif




