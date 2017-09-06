
#ifndef __TW_FONTOSD_H_
#define __TW_FONTOSD_H_
/* common defines and macros */
#include	"Typedef.h"
#include        "common.h"
#include        "TW_SpiOsd.h"
#include "keyDeal.h"
#include "ap_S32K1xx_lptmr.h"



#define TW88I2CAddress		0x8a	// TW88XX
//-----------------------------------------------------------------------------
//		Special Features
//-----------------------------------------------------------------------------
//#define ON					1
//#define OFF					0

#define	I2cDelay_Num			40
///osd control
#define	OsdOpen						1
#define	OsdClose						0
#define	OSDRAM0						0
#define	OSDRAM1						1

#define	Osd_State					0x55
#define	Video_State				0xaa

#define	BackLightOpen			1
#define	BackLightOff				0
#define	PanalGammaVccOn		1
#define	PanalGammaVccOff	0
///video signal
#define	SignalExist				1
#define	SignalNo						0

#define	MIDDLE_DOOR_SIGNAL	0
#define	R_SHIELD_SIGNAL			1

#define	SWITCH_VIDEO_OSD						0
#define	SWITCH_VIDEO_MIDDLE_DOOR		2
#define	SWITCH_VIDEO_R_SHIELD      1
#define	SWITCH_VIDEO_UP_VIDEO			3
#define	SWITCH_VIDEO_FRONT_DOOR		4

#define	VIDEO_CHANNEL_FRONT_DOOR		0
#define	VIDEO_CHANNEL_MIDDLE_DOOR		1
#define	VIDEO_CHANNEL_R_SHIELD      2
#define	VIDEO_CHANNEL_UP_VIDEO			3


///window number
#define	FONT_OSD_WIN_1				0
#define	FONT_OSD_WIN_2				1
#define	FONT_OSD_WIN_3 				2
#define	FONT_OSD_WIN_4 				3
#define	FONT_OSD_ALL_WIN			0xff

#define EXTENDED_COLOR		0x80
#define BG_COLOR_BLACK 		0x00	//For background color of window, border, and char. 
#define BG_COLOR_RED			0x40	
#define BG_COLOR_GREEN		0x20
#define BG_COLOR_YELLOW		0x60
#define BG_COLOR_BLUE			0x10
#define BG_COLOR_MAGENTA	0x50
#define BG_COLOR_CYAN			0x30
#define BG_COLOR_WHITE		0x70
#define BG_COLOR_TRANS		0x80	
#define BG_COLOR_MASK			0xf0	
//
#define CH_COLOR_BLACK 		0x00	//For color of char.  黑色
#define CH_COLOR_CYAN			0x03                      //青色

#define CH_COLOR_BLUE			0x09                      //蓝色
#define CH_COLOR_GREEN		0x0a                      //绿色
#define CH_COLOR_BLUE1    0x0b                      //浅蓝色
#define CH_COLOR_RED			0x0c                     	//红色
#define CH_COLOR_MAGENTA	0x0d                      //紫色
#define CH_COLOR_YELLOW		0x0e                      //黄色
#define CH_COLOR_WHITE		0x0f                      //白色


//#define CH_BLINK					0x08	                    //闪烁
#define CH_COLOR_MASK			0x0f	
/******************funtion macros*************************/
//#pragma CODE_SEG  USER_ROM
void	TW88XX_Power(unsigned char	State);
void	Tw_Pannal_Parameter_Init(void);
void	OsdCmd(unsigned char Osd_Cmd);
void	Osd_Video_Change(BYTE Cmd);
EXT void BackLightCmd(BYTE Cmd);
EXT void ContrastCmd(unsigned char Data);
EXT void BrightnessCmd(unsigned char Data);
EXT void HueCmd(unsigned char Data);
void	SharpnessCmd(unsigned char Data);
void	VideoChannelSelect(unsigned char Channel);
unsigned char	VideoSignalDetect(void);
void	Init_PictureState(void);
void	WaitTW8825SystemStability(void);

////////////////////////////////////////////////////////////////////////////
void DownloadFontByDMA(WORD dest_font_index, DWORD src_loc, WORD size, BYTE width, BYTE height);
void FontOsdSetOsdRamAddrAttr(BYTE Winno,WORD OsdRamAddrStart);
void WriteColor_StringToAddr
(WORD OsdRamAddr, BYTE Back_Front_Color, BYTE * str,BYTE Cnt, BYTE mode);
void FontOsd_Interface_CharacterHeight_Pixels(BYTE Character_Height,BYTE Character_Pixels);
void InitFontRam(WORD dest_font_index, const FONT_SPI_INFO_t *font);
void FontOsdWinDisplay(BYTE winno,BYTE en);
void FontOsdWinEnable(BYTE winno, BYTE en);
void FontOsdWinScreen(BYTE winno, WORD x, WORD y, BYTE w, BYTE h);
void FontOsdAplha(BYTE winno,BYTE alpha_color, BYTE alpha_value);
void FontOsdWinZoom(BYTE winno, BYTE zoomH, BYTE zoomV);
void FOsdOsdRamFifoBypass(BYTE fOn);
void	Clear_External_WatchDog(void);
/////////////////////////////////////////////////////////////////////////////
void I2CStart(void);
void I2CWriteData(BYTE value);
BYTE I2CReadData(void);
void I2CStop(void);
void I2Cdelay(void);
void I2Cdelay2(void);

BYTE ReadI2C		(BYTE addr, BYTE index);
void WriteI2C(BYTE addr, BYTE index, BYTE val);
void WriteI2CnD(BYTE addr, BYTE index, BYTE val, BYTE cnt);

#define WriteTW88(a,b)				WriteI2C(TW88I2CAddress, a, b)
#define ReadTW88(a)					ReadI2C(TW88I2CAddress, a)
#define WriteTW88Page(b)		WriteTW88(0xff,b)

//extern void (*TW8835_I2C_delay)();
#define dd()  (*TW8835_I2C_delay)()//lptmr_waitting_us(3*2)//I2Cdelay()
#define dd2() (*TW8835_I2C_delay)()	//lptmr_waitting_us(1)//I2Cdelay2()	


#endif
//#pragma CODE_SEG DEFAULT
  

