#ifndef __MX25L12845D
#define __MX25L12845D
//#include <mc9s12xhz256.h>
#include "types.h"
//#include "Flash.h"
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"

/**************condition macros***************/
#ifndef	_NO_MX25L12845D_EXTERN
#define	_MX25L12845D_EXTERN	extern
#else
#define	_MX25L12845D_EXTERN 
#endif



//
#define err_flash_unkown   				0xF0
#define err_flash_checksum 				0xF1
#define err_flash_lenth   	 					0xF2
#define err_flash_clear    						0xF3
#define	 err_flash_Write_checksum		0xF4
#define flash_OK					 				0x01



/*********************IO macros****************************/	
#define	ISD_MISO							PTS_PTS4
#define	ISD_MISO_IO	  			DDRS_DDRS4

#define	ISD_MOSI		  				PTS_PTS5
#define	ISD_MOSI_IO	  			DDRS_DDRS5 

#define	ISD_SCLK							PTS_PTS6
#define	ISD_SCLK_IO	  			DDRS_DDRS6

#define	ISD_CS		    				PTS_PTS7
#define	ISD_CS_IO	  				DDRS_DDRS7 

#define   HNS206
#ifdef HNS206
#define	V_5V_CTRL						PORTD_PD3
#define	V_5V_CTRL_IO	   			DDRD_DDRD3 

#define	DOWNLOAD_CTRL				PORTD_PD1
#define	DOWNLOAD_CTRL_IO		DDRD_DDRD1

#define	MODE_CTRL						PORTD_PD0
#define	MODE_CTRL_IO					DDRD_DDRD4

#else
 #if 0
#define	V_5V_CTRL						PORTD_PD2
#define	V_5V_CTRL_IO	   			DDRD_DDRD2 

#define	DOWNLOAD_CTRL				PORTD_PD1
#define	DOWNLOAD_CTRL_IO		DDRD_DDRD1

#define	MODE_CTRL						PORTD_PD0
#define	MODE_CTRL_IO					DDRD_DDRD0
  #endif
#endif


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
#define FLASH_CMD_READ        0x03
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

