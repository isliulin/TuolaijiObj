#ifndef	_typedef_h
#define	_typedef_h

#include "Eeprom.h"

/****************************************/
typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;

#define ON					1
#define OFF					0

#define NOP();				asm nop;
#define	BYTE		unsigned char	
#define	WORD		unsigned int	
#define	DWORD		unsigned long	
#define	bit 		unsigned char
#define	CODE 		const
#define _between(x, a, b)	(a<=x && x<=b)
#define _swap(a, b)			{ a^=b; b^=a; a^=b; }
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
  typedef union
  {
    byte Byte;
    struct
    {
      unsigned bit0: 1;
      unsigned bit1: 1;
      unsigned bit2: 1;
      unsigned bit3: 1;
      unsigned bit4: 1;
      unsigned bit5: 1;
      unsigned bit6: 1;
      unsigned bit7: 1;
    } Bits;
  }
  BitCommon;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef	struct
{
	BYTE	second;
	BYTE	minute;
	BYTE	hour;
	BYTE	week;
	BYTE	date;
	BYTE	month;
	BYTE	year;		
}M4100_TIME;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef	union
{
	struct
	{
		unsigned		bit0:1;
		unsigned		bit1:1;
		unsigned		bit2:1;
		unsigned		bit3:1;
		unsigned		bit4:1;
		unsigned		bit5:1;
		unsigned		bit6:1;
		unsigned		bit7:1;	
		unsigned		bit8:1;
		unsigned		bit9:1;
		unsigned		bit10:1;
		unsigned		bit11:1;
		unsigned		bit12:1;
		unsigned		bit13:1;
		unsigned		bit14:1;
		unsigned		bit15:1;	
	}bits;
	unsigned	int	Data;	
}LoadStruct;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef struct
{
   uint   PGN;
   uchar  data[8];
   uint   byte_count;
   uchar  source_addr;  
   uchar  dest_addr;    	   
} J1939_PDU_T;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef struct
{
   ulong  identifier;
   uchar  data[8];
   uchar  byte_count;
}CAN_PACKET_T;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef	union
{
    struct
    {
    	unsigned        DtH:1;	
    	unsigned        Dp:2;
    	unsigned        S0:5;             
    	unsigned        Sa:4;
    	unsigned        DtL:4;      
    }b;
    unsigned int   ID;
}CanIDStruct;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef	struct  
{
	unsigned int  CanID;
	unsigned char Priority;
	unsigned char Data[8];
	unsigned char NoOfBytes;	
	unsigned Ext:1;
	unsigned Remote:1;
}CanMessageStruct;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef	union
{
  struct
  {
    unsigned         Key0:1;           
    unsigned         Key1:1;
    unsigned         Key2:1;           
    unsigned         Key3:1;
    unsigned         Key4:1;           
    unsigned         Key5:1;
    unsigned         Key6:1;           
    unsigned         Key7:1;
  }b;
  unsigned char      Data;
}SelfKeyStruct;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef	union
{
	struct
	{
		unsigned	Load0:1;
		unsigned	Load1:1;
		unsigned	Load2:1;
		unsigned	Load3:1;
		unsigned 	Load4:1;
		unsigned 	Load5:1;
		unsigned 	Load6:1;
		unsigned 	Load7:1;	
	}Loads;
	unsigned	char	Load;	
}PutLoadStruct;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef	union
 {
    struct
    {
    unsigned         S0:2;             
    unsigned         S1:2;             
    unsigned         S2:2;             
    unsigned         S3:2;             
    }St;
       
    unsigned char    Data;
}LoadStStruct;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef	union 
{
    struct
    {
    unsigned         ST0:1;             
    unsigned         ST1:1;             
    unsigned         ST2:1;                
    unsigned         ST3:1;             
    unsigned         ST4:1;             
    unsigned         ST5:1;             
    unsigned         ST6:1;             
    unsigned         ST7:1;                                  
    }b;
    unsigned     char    Data;
}StStruct;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef	union
{
	struct
	{
		unsigned	LedLoad0:1;
		unsigned	LedLoad1:1;
		unsigned	LedLoad2:1;
		unsigned	LedLoad3:1;
		unsigned 	LedLoad4:1;
		unsigned 	LedLoad5:1;
		unsigned 	LedLoad6:1;
		unsigned 	LedLoad7:1;	
	}LedLoads;
	unsigned	char	LedLoad;	
}MeterLedStruct;

/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef	union 
{
    struct
    {
	    unsigned         Protect_Lamp:2;             
	    unsigned         Amber_Lamp:2;             
	    unsigned         Red_Lamp:2;                
	    unsigned         MIL_Lamp:2;                                           
    }b;
    unsigned     char    Data;
}Diagnose_Lamp;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef struct image_info_s {
	BYTE lut_type;	///	//1:luts(byte pointer method.default on TW8835MENU), 0:lut(address method)
	BYTE rle;				//upper:bpp bottom:rle_counter.
	WORD dx,dy;			//width, height
} image_info_t;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef struct image_item_info_s {
	BYTE type;	//0:LUT+image, 1:header+lut+image 2:image_only 3: lut_only 4:header_only	
	DWORD loc;  //absolute location on SpiFlash
	image_info_t *info;
	BYTE alpha;		//alpha index. 0xFF:ignore.
} image_item_info_t;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef struct image_header_info_s {
	DWORD lut_loc;
	DWORD image_loc;

	BYTE lut_type;///	//1:luts(byte pointer method.default on TW8835MENU), 0:lut(address method)
	BYTE bpp;
	BYTE rle;		//upper:bpp bottom:rle_counter.
	WORD dx,dy;		//width, height
	WORD lut_size;	//we don't need it. we can assume LUT size & image size.
} image_header_info_t;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef struct menu_item_info_s {
	BYTE osd_type;				//0:SpiOSD 1:FontOSD..failed. not using

	void (*p)(void);			//linked function

	WORD x,y;					//start x, start y
	BYTE align;					//align type

	//osd_type_meun_item_t osd;				//BYTE up,left,down,right;	//navi_key
	//struct sosd_menu_item_s	osd_s;
	struct image_item_info_s *image;
	struct image_item_info_s *image1;
	BYTE image_type;			//0:Background, icon, ....., fSOD info

	BYTE win;					//upper:fosd, bottom:sosd
	WORD lut;					//lut start offset	   1110207 become 9bit
	BYTE up,left,down,right;	//navi_key
} menu_item_info_t;

/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
struct RLE2_HEADER {
    unsigned char op_bmpbits;
    unsigned char rledata_cnt;

    unsigned short w;
    unsigned short h;

    unsigned long  size;
    
    unsigned char LUT_format; // bit0- 0:RGBa, 1:BGRa, bit1- 0:LUTS, 1:LUT;
    unsigned char LUT_colors; // 0xff:256 colors,  0x7F:128 colors,...
    unsigned char dummy0;
    unsigned char dummy1;
};
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
 typedef struct {    
     DWORD start;    
     DWORD length;   
 } SLIDEIMAGE;       
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef struct menu_item_info{
		unsigned int	Menu_X;
		unsigned int	Menu_Y;
		unsigned int	Menu_Lut;
};
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef struct FONT_SPI_INFO_s {
	DWORD loc;
	WORD size;				//curr max 10*1024 = 2800h
	BYTE width,height;
	WORD max;
	WORD bpp2;
	WORD bpp3;
	WORD bpp4;
} FONT_SPI_INFO_t;


/*******************************/
typedef	union
{
  struct
  {
    unsigned         LioQiFlag:1;          
    unsigned         DiYaFlag:1;
    unsigned         GaowenFlag:1;           
    unsigned         DiDianYaFlag:1;
    unsigned         HuWenDuFlag:1;           
    unsigned         ChunanGanQiFlag:1;
    unsigned         TPMSZhuJiFlag:1;           
    unsigned         TPMSCANFlag:1;
  }b;
  unsigned char      Data;
}TPMSStateStruct;

typedef	struct  
{
//	unsigned  char Tyre;
	unsigned  int  TyrePressure;
	unsigned  char TyreTemperature;
	unsigned  char  TyreVoltage;	
	TPMSStateStruct      TyreStateFlag;


}TPMSMessageStruct;

typedef	union  
{
  struct
  {
    unsigned         BMSSystemStatusFlag:1;          
    unsigned         BMUStatusFlag:1;
    unsigned         LECUStatusFlag:2;           
    unsigned         BatterFualFlag:2;
    unsigned         BatterWorkFlag:2;  
  }b;
  unsigned char      Data;
}BMSStatus1Struct;

typedef	union  
{
  struct
  {
    unsigned         SingleBatHigherVoltFlag_2:1;          
    unsigned         SingleBatHigherVoltFlag_1:1;
    unsigned         SingleBatLowerVoltFlag_2:1;           
    unsigned         SingleBatLowerVoltFlag_1:1;
    unsigned         BatterHigherTempFlag_2:1;  
    unsigned         BatterHigherTempFlag_1:1; 
    unsigned         SingleBatDiffentVoltFlag:1;  
    unsigned         SingleBatDiffentTempFlag_1:1; 		
  }b;
  unsigned char      Data;
}BMSStatus2Struct;

typedef	union  
{
  struct
  {
    unsigned         IntranetCanErrFlag:1;          
    unsigned         OuternetCanErrFlag:1;
    unsigned         HighVoltLeakageFlag:1;           
    unsigned         Reserve:1;
    unsigned         DischargeRelayAdhereFlag:1;  
    unsigned         DischargeCurrentBreakFlag:1; 
    unsigned         DischargeOverCurrentFlag:1;  
    unsigned         ChargeOverCurrentFlag:1; 		
  }b;
  unsigned char      Data;
}BMSStatus3Struct;

#endif


