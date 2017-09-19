#ifndef __S08_FLASH__
  #define __S08_FLASH__
  #include "derivative.h"

  	#define FLASH_BLANK_CHECK_CMD 0x05
	#define FLASH_PROGRAM_CMD     		0x20
	#define FLASH_BURST_CMD       			0x25
  	#define FLASH_ERASE_CMD       			0x40	  
	#define FLASH_MASS_ERASE_CMD  	0x41
  	#define	FLASH_ERASE_ABORT_CMD	0x47
  
  	#define BUS_CLOCK    		8000000
	#define	FlahDelayNum		1000
  
  #if BUS_CLOCK > 12800000
    #define FLASH_CLOCK (unsigned char)((BUS_CLOCK/1600000) + 0x40)
  #else
    #define FLASH_CLOCK (unsigned char)(BUS_CLOCK/200000)
  #endif
  
  /* Macros to call the function using the different features */
  #define Flash_Erase(Address) \
          Flash_Cmd((unsigned int)Address, (unsigned int)1, (unsigned char*)0, FLASH_ERASE_CMD)  
   #define Flash_Blank_Check(Address) \
          Flash_Cmd((unsigned int)Address, (unsigned int)1, (unsigned char*)0, FLASH_BLANK_CHECK_CMD)
  #define Flash_Program(Address, Data) \
          Flash_Cmd((unsigned int)Address, (unsigned int)1, (unsigned char*)&Data, FLASH_PROGRAM_CMD)
  #define Flash_Burst(Address, Size, DataPtr) \
          Flash_Cmd((unsigned int)Address, (unsigned int)Size, (unsigned char*)DataPtr, FLASH_BURST_CMD)        

  #pragma CODE_SEG FLASH_ROUTINES
  unsigned char Flash_Cmd(unsigned int FlashAddress, unsigned int FlashDataCounter, unsigned char* pFlashDataPtr, unsigned char FlashCommand);
  unsigned char Flash_Read(unsigned long FlashAddress);
  #pragma CODE_SEG DEFAULT
  void FlashInit(void);
  void CopyInRAM(void);
#endif

