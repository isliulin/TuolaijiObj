#ifndef	_Macros_H
#define		_Macros_H

#include 	"hidef.h" 		/* for EnableInterrupts macro */
#include 	"derivative.h" /* include peripheral declarations */
/**********************************/
typedef	union
{
	struct
	{
		unsigned     b0:1;
		unsigned     b1:1;
		unsigned     b2:1;		
	   unsigned     S:5;
	}b;
	unsigned  char Data;
}SelfIDStruct;
/*****************************************************/
typedef union 
{
  byte Byte;
  struct 
  {
    unsigned	 bit0      :1; 
    unsigned	 bit1      :1; 
    unsigned	 bit2      :1; 
    unsigned	 bit3      :1;
    unsigned	 bit4      :1; 
    unsigned	 bit5      :1;
    unsigned	 bit6      :1;
    unsigned	 bit7      :1;
  } Bits;
} BitCommon;
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
   unsigned long	identifier;
   unsigned char  data[8];
   unsigned char  byte_count;
}CAN_PACKET_T;
//////////////////////////////////////////////////////////////////////////////
#define	S19_DATA_NUMBER		35						///一行S19数据的大小->byte格式

typedef struct
{
	unsigned char  len;
   unsigned char  S19data[S19_DATA_NUMBER+6];	///数据长度+最大4个地址+最大个数据+1校验和
   unsigned char  function;
}_S19_BUF;
//////////////////////////////////////////////////////////////////////////////
typedef union
{
	unsigned char data[S19_DATA_NUMBER+8];
	_S19_BUF S19Buf;
}_S19_RX_BUF;
/*********************parameter macros***************************/
/* External crystal oscillator = 8MHz */
/* FOR PEE mode:Bus_Clock = (1/2)*[(Osc_clock/Ref_div)*VCO_div]/(Bus_div) = 8MHz */   
#define Bootloader_BUS_DIV 				0       /* Bus divider = 0, divide by 1 */
/* Clock source options */
#define BUS_CLK     								1 
/* MSCAN module configuration constants */
/* Baud rate = CANCLK/(Prescaler * time quantas) */
/* TSEG1 + TSEG2 + SYNCH_SEG = 4 time quantas */
#define Bootloader_CAN_CLK  	  		0 /* Clock source: 0 for external oscillator, 1 for bus clock */
#define Bootloader_TIME_SEGM1  		4   /* Time Segment 1 = 5 */
#define Bootloader_TIME_SEGM2  		1   /* Time Segment 2 = 2 */
/* Prescaler = CANCLK/(Baud rate * time quantas) = 8MHz/(250kHz * 4) = 8 */                                               
#define Bootloader_CAN_PRESC   		3   /* Baud rate prescaler = 8 */ 
#define Bootloader_CAN_SYNJW   		0   /* Sinchronization jump width = 1 clock cycles */ 
/***can initialize macros***/
#define	MaskMode								1
#define	NormalMode							2
/*Can Id Definitions*/
#define	 FrontDriveID						0x10FF0121
#define	 BackDriveID						0x10FF0221
#define	 TopDriveID						0x10FF0321
#define	 PowerModuleID				0x10FF0421
#define	 PowerControlModuleID	0x10FF0621

#define	 MainTestBordID				0x10FF0721///协议未定义
#define	 SlaveTestBordID				0x10FF0821///协议未定义
///mcu area macros
#define	_Mcu_Type 60

#if	_Mcu_Type==32
	#define	Total_Sector_Num				44				///dz32 is 44
	#define	Flash_First_Address				0x7c00		///dz32 is 0x7c00
	#define	UserCodeFirstAddr				0x7c00		///dz32 is 0x7c00
	#define	Last_Ram_Num						0x087f		//dz32 is 0x087f
#elif	_Mcu_Type==60
	#define	Total_Sector_Num				77				///dz60 is 77
	#define	Flash_First_Address				0x1900		///dz60 is 0x1900
	#define	UserCodeFirstAddr				0x1900		///dz60 is 0x1900
	#define	Last_Ram_Num						0x107f		//dz60 is 0x107f
#endif
#define	First_Ram_Num							0x0080
#define	Flash_Sector_Area					0x0300
#define	Flash_Sector_Num					Total_Sector_Num-6	///6 is flash protect area;44 is MCU sector number	
#define	Flash_Last_Address					0xf3ff	
#define	Config_Sector_Num					1

#define	Num_Sec2										0x3c00
#define	BootloaderDelay_Num					0xb400			///1.4-1.5s						
///communicate error reason
#define err_flash_unkown   	0xF0
#define err_flash_checksum 	0xF1
#define err_flash_lenth    			0xF2
#define err_flash_clear    			0xF3
#define flash_OK					 	0x01
#endif
