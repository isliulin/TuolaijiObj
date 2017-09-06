#include 	<hidef.h> /* for EnableInterrupts macro */
#include	"derivative.h" /* include peripheral declarations */
#include	"S08_Flash.h"/*include flash decarations*/
#include	"Bootloader.h"
#include	"Bootloader_Init.h"
/**************funtion macros******************/

/*************************************/
void main(void)
{
	__RESET_WATCHDOG(); /* 	feeds the dog */
	Bootloader_External_WatchDog();
	ADC_Init();
  TestSelfID();
	Bootloader_MSCAN_Init(MaskMode);	///initialize can mode to mask id if want to recieve flash data
	StartStateDetect();
	FlashInit();
	CopyInRAM();
	Erase_Flash_Zero();
  for(;;) 
 	{
    	__RESET_WATCHDOG(); /* feeds the dog */
		Bootloader_External_WatchDog();

		Bootloader_Deal();
  } 
}
