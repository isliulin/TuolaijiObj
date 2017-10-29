#undef	EXT
#ifdef	TW_FontOsd_C
#define	EXT
#else
#define	EXT extern
#endif
/* common defines and macros */
#include <hidef.h>      	
#include <mc9s12xhz256.h>
#include	"typedef.h"
/******************IO macros**************************/
#define	I2C_SCL						PORTD_PD2
#define	I2C_SCL_IO					DDRD_DDRD2
#define	I2C_SDA						PORTD_PD3
#define	I2C_SDA_IO					DDRD_DDRD3  

#define	V_POWER_CTRL					PTL_PTL3
#define	V_POWER_CTRL_IO	   	DDRL_DDRL3 
#define	V_POWER3_CTRL				PTL_PTL0
#define	V_POWER3_CTRL_IO	  DDRL_DDRL0
#define	MODE_CTRL						PORTE_PE4
#define	MODE_CTRL_IO					DDRE_DDRE4
#define	DOWNLOAD_CTRL				PORTK_PK3
#define	DOWNLOAD_CTRL_IO		DDRK_DDRK3
#define	V_5V_CTRL						PORTD_PD1
#define	V_5V_CTRL_IO	   			DDRD_DDRD1
#define	TV_POWER1_CTRL			PORTK_PK1
#define	TV_POWER1_CTRL_IO	  DDRK_DDRK1
#define	TV_POWER2_CTRL			PORTK_PK0
#define	TV_POWER2_CTRL_IO	  DDRK_DDRK0
#define	V_POW4_CTRL					PORTB_PB0
#define	V_POW4_CTRL_IO			DDRB_DDRB0			
#define	V_12_CTRL						PORTB_PB1
#define	V_12_CTRL_IO					DDRB_DDRB1	
#define	AN_POW_IO						DDRT_DDRT1
#define	AN_POW								PTT_PTT1

#define	ISD_MISO							PTS_PTS4
#define	ISD_MISO_IO	  			DDRS_DDRS4

#define	ISD_MOSI		  				PTS_PTS5
#define	ISD_MOSI_IO	  			DDRS_DDRS5 

#define	ISD_SCLK							PTS_PTS6
#define	ISD_SCLK_IO	  			DDRS_DDRS6

#define	ISD_CS		    				PTS_PTS7
#define	ISD_CS_IO	  				DDRS_DDRS7 

#define TW88I2CAddress		0x8a	// TW88XX
//-----------------------------------------------------------------------------
//		Special Features
//-----------------------------------------------------------------------------
#define ON					1
#define OFF					0

#define	I2cDelay_Num			2
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
#define BG_COLOR_RED				0x40	
#define BG_COLOR_GREEN		0x20
#define BG_COLOR_YELLOW		0x60
#define BG_COLOR_BLUE			0x10
#define BG_COLOR_MAGENTA	0x50
#define BG_COLOR_CYAN			0x30
#define BG_COLOR_WHITE		0x70
#define BG_COLOR_TRANS		0x80	
#define BG_COLOR_MASK			0xf0	

#define CH_COLOR_BLACK 		0x00	//For color of char.
#define CH_COLOR_RED				0x04	
#define CH_COLOR_GREEN		0x02
#define CH_COLOR_YELLOW		0x06
#define CH_COLOR_BLUE			0x01
#define CH_COLOR_MAGENTA	0x05
#define CH_COLOR_CYAN			0x03
#define CH_COLOR_WHITE		0x07
#define CH_BLINK						0x08	
#define CH_COLOR_MASK			0x0f	
/******************funtion macros*************************/
#pragma CODE_SEG  USER_ROM
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

////////////////////////////////////////////////////////////////////////////
void DownloadFontByDMA(WORD dest_font_index, DWORD src_loc, WORD size, BYTE width, BYTE height);
void FontOsdSetOsdRamAddrAttr(BYTE Winno,WORD OsdRamAddrStart);
void WriteColor_StringToAddr
(WORD OsdRamAddr, BYTE Back_Front_Color, const BYTE *str,BYTE Cnt, BYTE mode);
void FontOsd_Interface_CharacterHeight_Pixels(BYTE Character_Height,BYTE Character_Pixels);
void InitFontRam(WORD dest_font_index, const FONT_SPI_INFO_t *font);
void FontOsdWinDisplay(BYTE winno,BYTE en);
void FontOsdWinEnable(BYTE winno, BYTE en);
void FontOsdWinScreen(BYTE winno, WORD x, WORD y, BYTE w, BYTE h);
void FontOsdAplha(BYTE winno,BYTE alpha_color, BYTE alpha_value);
void FontOsdWinZoom(BYTE winno, BYTE zoomH, BYTE zoomV);
void FOsdOsdRamFifoBypass(BYTE fOn);
/////////////////////////////////////////////////////////////////////////////
void I2CStart(void);
void I2CWriteData(BYTE value);
BYTE I2CReadData(void);
void I2CStop(void);
void I2Cdelay(void);
BYTE ReadI2C		(BYTE addr, BYTE index);
void WriteI2C(BYTE addr, BYTE index, BYTE val);
void WriteI2CnD(BYTE addr, BYTE index, BYTE val, BYTE cnt);

#define WriteTW88(a,b)				WriteI2C(TW88I2CAddress, a, b)
#define ReadTW88(a)					ReadI2C(TW88I2CAddress, a)
#define WriteTW88Page(b)		WriteTW88(0xff,b)

#define dd()  I2Cdelay()


#pragma CODE_SEG DEFAULT
  

