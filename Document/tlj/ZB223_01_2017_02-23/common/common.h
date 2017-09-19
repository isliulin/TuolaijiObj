/*
 * File:        common.h
 * Purpose:     File to be included by all project files
 *
 * Notes:
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/*****************sleep flash macros***************/
#define SleepCode										0xaa
#define WakeCode										0x55
#define LeftFlashOpenCode						0x01
#define RightFlashOpenCode					0x02
#define AllFlashOpenCode						0x03
#define AllFlashCloseCode						0x00
#define CloseSendCode								0x04   

#include "HwMacroPatch.h"




  

/*
 * Debug prints ON (#define) or OFF (#undef)
 */
#define DEBUG
#define DEBUG_PRINT

/* 
 * Include the generic CPU header file 
 */
#include "evb_s32k144.h"
#include "S32K144.h"


/* 
 * Include common utilities
 */

#include "assert.h"
#include "io.h"
#include "startup.h"
#include "stdlib.h"
#include "arm_cm4.h"
#include "Typedef.h"
#include "configFile.h"
#include "LcmDisp.h"

#if defined(__GNUC__)
#include "io.h"
#include "stdlib.h"
#endif


#if (defined(IAR))
	#include "intrinsics.h"
#endif

/********************************************************************/
#include "BitMacros.h"
/******************parameter macros*******************/
#define	OSC_Number		16
#define KeyOn					1//0
#define KeyOff				0//1

#define Parameters_EEp_Addr 0
/*********parameter macros**********/
#define Eeprom_OK				0x80
#define Eeprom_ERROR			0x00
#define Eeprom_ADDR			0xa8
#define EEP_24CXX_PAGE_SIZE 	8 
/*********IO macros**********/
#define Eeprom_SCL_IO	 			DDRC_DDRC5
#define Eeprom_SCL		      PORTC_PC5
#define Eeprom_SDA_IO	      DDRC_DDRC4
#define Eeprom_SDA		      PORTC_PC4	







extern UINT8 write_data[21],read_data[21];

void DelayMs(unsigned int Delay);  
void DelAsciiInvalidZero(UINT8 *pData,UINT8 cnt);
void DispHexToAscii(UINT8 *pBuf,UINT32 data);
void Disp2ToAscii(UINT8 *pBuf,UINT8 data) ;
void bit16HexToAscii(UINT8 *pBuf,UINT16 data);
void bit8HexToAscii(UINT8 *pBuf,UINT8 data);
void bit32HexToAscii(UINT8 *pBuf,UINT32 data);
void insertDot(UINT8 *pData,UINT8 dot,UINT8 cnt);
UINT8 arrangeDispBuffer(UINT8 *pData,UINT8 *pDispData,UINT8 lenght, UINT8 disDotLenght,UINT8 disIntegerLenght); 
void DelAsciiInvalidZeroEx(UINT8 *pData,UINT8 cnt);
void binToDec(uchar bin, uchar *pDec);
void dataCopy(UINT8 *pDst,UINT8 *pScr,UINT8 cnt);
void asciiToGb2312(UINT8 *pGb2312,UINT8 *pAscii,UINT8 cnt);
UINT8 sign16Bit(UINT16 *pData,UINT16 offset);
UINT8 sign32Bit(UINT32 *pData,UINT32 offset);













void CarMaintain(void); 

void PowerOffSave(void);
void PowerOffSaveRead(void);
void delay_us(word delay);
void Eeprom_Read_Fact_Ex(unsigned char internal_addr, byte *dp, unsigned char data_len);
void Eeprom_Write_Fact_Ex(unsigned char internal_addr, byte *sp, unsigned char  data_len);

void Eeprom_IO_MACROS(void);
unsigned char Eeprom_register_read(unsigned char internal_addr, byte *dp,unsigned char data_len);
unsigned char Eeprom_register_write(unsigned char internal_addr, byte  *sp, unsigned char data_len);
unsigned char Eeprom_Page_Write(unsigned char internal_addr, byte *sp,unsigned char data_len);
//I2C
void Eeprom_IICstart(void);
void Eeprom_IICsendbyte(unsigned char csendbyte);
byte Eeprom_IICwaitack(void);
void Eeprom_IICstop(void);
unsigned char Eeprom_IICreceivebyte(void);
void Eeprom_IICsendack(void);
void Eeprom_IICsendnotack(void);
void Eeprom_SCL_delay(void);
void ReWrite_Delay(void);
void initEepromData(void);

#endif /* _COMMON_H_ */
