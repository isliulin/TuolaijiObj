#undef	EXT
#ifdef	EEPROM_C
#define	EXT
#else
#define	EXT extern
#endif
	/* common defines and macros */
	//#include <hidef.h>      	
	#include <mc9s12xhz256.h>
	#include	"typedef.h"
  /****************fution macros************************/
	#pragma CODE_SEG  USER_ROM
  static void Eeprom_IO_MACROS(void);
  static void Eeprom_SCL_delay(void);
  static void ReWrite_Delay(void);
  static void Eeprom_IICstart(void);
  static void Eeprom_IICstop(void);
  static unsigned char Eeprom_IICwaitack(void);
  static void Eeprom_IICsendack(void);
  static void Eeprom_IICsendnotack(void);
  static void Eeprom_IICsendbyte(unsigned char csendbyte);
  static uchar Eeprom_IICreceivebyte(void);
  static uchar Eeprom_register_write(unsigned char internal_addr, unsigned char *sp,unsigned char data_len);
  static uchar Eeprom_register_read(unsigned char internal_addr, unsigned char *dp,unsigned char data_len);
  static uchar Eeprom_Page_Write(unsigned char internal_addr, unsigned char *sp,unsigned char data_len);
  void Eeprom_Write_Fact(unsigned char internal_addr, unsigned char *sp, unsigned char data_len);
  void Eeprom_Read_Fact(unsigned char internal_addr, unsigned char *dp, unsigned char data_len);
  void PowerOffSave(void);
  void PowerOffSaveRead(void);
	#pragma CODE_SEG DEFAULT
  /*******************ram macros*******************/
  EXT unsigned char write_data[9];
  EXT unsigned char read_data[9];
  /*********parameter macros**********/
  #define Eeprom_OK					0x80
  #define Eeprom_ERROR				0x00
  #define Eeprom_ADDR				0xa0
  /*********IO macros**********/
  #define Eeprom_SCL_IO	 		DDRC_DDRC5
  #define Eeprom_SCL		      PORTC_PC5
  #define Eeprom_SDA_IO	    DDRC_DDRC4
  #define Eeprom_SDA		      PORTC_PC4		
  /***********eeprom macros****************/
#define EEP_24CXX_PAGE_SIZE  8
 #define SmallDistance_Addr 	 0x00
 #define OverallDistance_Addr  SmallDistance_Addr+4 		
 #define RainScratch_Addr 		 OverallDistance_Addr+4+1 	//新增超速报警+蜂鸣器音量+车门未关报警
 #define LcdShine_Addr				 RainScratch_Addr+3 	 
 #define FrontDoorVideoChannelAddr				 LcdShine_Addr+4 
 #define MiddleDoorVideoChannelAddr 			 FrontDoorVideoChannelAddr+1
 #define RShieldVideoChannel_Addr 				 MiddleDoorVideoChannelAddr+1  
 #define TopVideoChannel_Addr 						 RShieldVideoChannel_Addr+1  
	
 #define FrontDoorVideoSignalStateAddr		 TopVideoChannel_Addr+1  
 #define MiddleDoorVideoSignalStateAddr 	 FrontDoorVideoSignalStateAddr+1
 #define RShieldVideoSignalStateAddr			 MiddleDoorVideoSignalStateAddr+1
 #define TopVideoSignalStateAddr					 RShieldVideoSignalStateAddr+1
	
 #define TPMSIDSAVEStateAddr							 (TopVideoSignalStateAddr+1)



