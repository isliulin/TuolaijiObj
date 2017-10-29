#undef	EXT
#ifdef	TW_SpiOsd_C
#define	EXT
#else
#define	EXT extern
#endif
/* common defines and macros */
#include <hidef.h>      	
#include <mc9s12xhz256.h>
#include	"typedef.h"
///////////////////////////////////////////////////////////////////////////////////////
//--------------------
// SPI FLASH Vendor
//--------------------
#define SFLASH_VENDOR_MX	0	//MX25L12845(C2 20 18) -- defaut
#define SFLASH_VENDOR_EON	1	//EN25Q128 ID(1C 30 18)
#define SFLASH_VENDOR_WB	2
#define SFLASH_VENDOR_NUM	3	//STM(20 BA 18)
///图片显示的模式
#define IMAGE_ITEM_TYPE_0		0	// LUT+IMAGE and use image_info_s
#define IMAGE_ITEM_TYPE_1		1	// RLE2_HEADER+LUT+IMAGE.
#define IMAGE_ITEM_TYPE_2		2	// RLE2_HEADER+LUT+IMAGE but, use image_info_s
#define IMAGE_ITEM_TYPE_3		3	// N/A	IMAGE only
#define IMAGE_ITEM_TYPE_4		4	// N/A  LUT only
#define IMAGE_ITEM_TYPE_5		5	// N/A
#define IMAGE_ITEM_TYPE_6		6	// N/A
#define MRLE_INFO_SIZE 			0x10;

#define TYPE_SOSD		0	//normal SOSD item
#define TYPE_FOSD		1	//normal FOSD item
#define TYPE_BG			2	//SOSD FIXED Background
#define TYPE_MBG			3	//SOSD MOVING Background
#define TYPE_SLIDE		4	//SOSD Slide area
#define TYPE_BOX			5	//SOSD BOX area

#define MENU_TYPE_NORMAL	0
#define MENU_TYPE_LIST		1
#define MENU_TYPE_SCRLDN	2
#define MENU_TYPE_DIALOG	3
#define MENU_TYPE_SLIDE		4
#define MENU_TYPE_SLIDE3	5
#define MENU_TYPE_NOTE		0x10	//need the selected note.
#define MENU_TYPE_TEST		0x20

#define PAGE1_DECODER		0x01	//0x101::0x142
#define PAGE1_VADC				0x01	//0x1C0::0x1E7	include LLPLL
#define PAGE1_LLPLL			0x01	//0x1C0::0x1E7	include LLPLL
#define PAGE2_SCALER			0x02	//0x201::0x21E
#define PAGE2_TCON				0x02	//0x240::0x24E
#define PAGE2_IMAGE			0x02	//0x280::0x2BF
#define PAGE2_GAMMA			0x02	//0x2E0::0x2E3 include LCDC
#define PAGE2_DITHER			0x02	//0x2E4::0x2E4
#define PAGE2_RGBLEVEL	0x02	//0x2F0::0x2F5
#define PAGE2_8BITPANEL	0x02	//0x2F8::0x2F9
#define PAGE3_FOSD				0x03	//0x300::0x354
#define PAGE4_SOSD				0x04	//0x400::0x3BE
#define PAGE4_SPI				0x04	//0x4C0::0x4DA include MCU
#define TWBASE_SPI				0xC0	//note:Not a page. it is a offset.
#define PAGE4_CLOCK			0x04	//0x4E0::0x4EB
#define PAGE5_MEAS				0x05	//0x500::0x545

#define PCLK_SEL_PCLK			0
#define PCLK_SEL_PLL				1
#define SPI_CK_SEL_27M		0
#define SPI_CK_SEL_32K		1
#define SPI_CK_SEL_PCLK		2
#define SPI_CK_DIV_1P0		0
#define SPI_CK_DIV_1P5		1
#define SPI_CK_DIV_2P0		2
#define SPI_CK_DIV_2P5		3

#define	SPI_OSD_WIN_ALL		0xff
#define	SPI_OSD_WIN_1			0
#define	SPI_OSD_WIN_2			1
#define	SPI_OSD_WIN_3			2
#define	SPI_OSD_WIN_4			3
#define	SPI_OSD_WIN_5			4
#define	SPI_OSD_WIN_6			5
#define	SPI_OSD_WIN_7			6
#define	SPI_OSD_WIN_8			7

//////////////////////////////////////////////////////////////
EXT BYTE DMAMODE_RW_FIX_REG;
EXT BYTE DMAMODE_RW_INC_REG;
EXT BYTE DMAMODE_R_XDATA;
EXT BYTE DMAMODE_W_XDATA;
EXT BYTE SPICMD_READ;
EXT BYTE SPICMD_PROGRAM;
EXT BYTE SPICMD_BYTES;
EXT image_header_info_t header_table;
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma CODE_SEG  USER_ROM
void InitCore(void);
void InitSystem(BYTE special);
BYTE check_cvbs(BYTE num);
void	Display_Layer_Graphics
(const SLIDEIMAGE *Image_Flash_Addr_Start,struct image_header_info_s *header,const struct menu_item_info *image_Disp,
	const struct RLE2_HEADER *rle_header,unsigned char Window);
void MenuPrepareImageHeader(const SLIDEIMAGE *Image_Flash_Addr_Start,struct image_header_info_s *header,const struct RLE2_HEADER *rle_header);
void SOsdWinBuffAllClean(void);
void InitCPU(void);
void SPI_SetModeByRegister( void );
void InitCore(void);
void FP_GpioDefault(void);
void FPPWC_OnOff(BYTE fOn);
void SSPLL_PowerUp(BYTE fOn);
void DCDC_On(BYTE step);
void FPBiasOnOff(BYTE fOn);
void DCDC_StartUP(void);
void McuSpiClkSet(BYTE PCLK_SEL,BYTE SPI_CK_SEL,BYTE SPI_CK_DIV) ;
void WaitVBlank(BYTE cnt);
void	SpiOsdWinAllOff(void);
void SpiOsdWinHWEnable(BYTE winno, BYTE en);
void	SpiOsdWinPixelAlpha( BYTE winno, BYTE alpha );
void SOsdHwBuffWrite2Hw(void);
void SOsdHwBuffWrite_Win(unsigned char winno,unsigned char WaitFlag);
void SpiOsdWinPixelWidth(BYTE winno, BYTE bpp);
void SpiOsdWinBuffEnable(BYTE winno, BYTE en);
void SOsdWinBuffClean(unsigned char winno);
void SpiOsdRLC(BYTE winno,BYTE dcnt, BYTE ccnt);
void SOsdWinBuffWrite2Hw(BYTE start, BYTE end);
void SOsdHwBuffSetAlpha(WORD alpha_index);
void	WaitTW8825SystemStability(void);

#pragma CODE_SEG DEFAULT

