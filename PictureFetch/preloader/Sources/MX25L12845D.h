#ifndef __MX25L12845D
#define __MX25L12845D
#include <mc9s12xhz256.h>
#include "type.h"
#include "Flash.h"

/**************condition macros***************/
#ifndef	_NO_MX25L12845D_EXTERN
#define	_MX25L12845D_EXTERN	extern
#else
#define	_MX25L12845D_EXTERN 
#endif
/*********************IO macros****************************/	
#define	ISD_MISO							PTS_PTS4
#define	ISD_MISO_IO	  			DDRS_DDRS4

#define	ISD_MOSI		  				PTS_PTS5
#define	ISD_MOSI_IO	  			DDRS_DDRS5 

#define	ISD_SCLK							PTS_PTS6
#define	ISD_SCLK_IO	  			DDRS_DDRS6

#define	ISD_CS		    				PTS_PTS7
#define	ISD_CS_IO	  				DDRS_DDRS7 

#define	V_5V_CTRL						PORTD_PD1
#define	V_5V_CTRL_IO	   			DDRD_DDRD1 

#define	DOWNLOAD_CTRL				PORTK_PK3
#define	DOWNLOAD_CTRL_IO		DDRK_DDRK3

#define	MODE_CTRL						PORTE_PE4
#define	MODE_CTRL_IO					DDRE_DDRE4

#define SCK_L() 	ISD_SCLK=0
#define SCK_H() 	ISD_SCLK=1
#define SI_L() 	  ISD_MOSI=0
#define SI_H() 	  ISD_MOSI=1
#define CS_L() 		ISD_CS=0
#define CS_H() 		ISD_CS=1

#define	FLASH_PAGE_SIZE				256///BYTE
#define	FLASH_SECTOR_SIZE			4096///BYTE
///flash²Ù×÷Ö¸Áî
#define	FLASH_WRITE_ENABLE		0x06
#define	FLASH_WHOLE_ERASE		0X60	///60 or C7  to erase whole chip
#define	FLASH_SECTOR_ERASE		0X20
#define	FLASH_BLOCK_ERASE			0Xd8
#define	FLASH_PAGE_WRITE			0X02
#define	FLASH_FAST_READ				0X0b
#define	FLASH_ID_READ					0X9F
#define	FLASH_READ_STATAS			0X05
///////////////////////////////////////////////////////////////////////////////////////
uchar Flash_Read_ID(void) ; 
uchar Erase_Sector(uint Sector_Number)  ;
uchar Erase_Flash(void); 
uchar Erase_Block(uchar Block_Number)  ; 
uchar Flash_PageWrite(uint page,uint addr,uint len,uchar *pdata) ;
uchar Flash_PageRead(ulong addr,uint len,uchar *pdata) ; 
void	Bootloader_Mode_Change(void);
uchar Flash_Page_Write(ulong addr,uint Data_Len,uchar *pdata) ;
static uchar Flash_WaitBusy(void) ;
/////////////////////////////////////////////////////////////////////////////////////////  
_MX25L12845D_EXTERN unsigned char ISD_sendbuf[10] ;


#endif

