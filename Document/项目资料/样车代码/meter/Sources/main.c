/***************************************************/
#include	"config.h"
/* this variable definition is to demonstrate how to share data between XGATE and S12X */
#pragma DATA_SEG SHARED_DATA
volatile int shared_counter; /* volatile because both cores are accessing it. */
#pragma DATA_SEG DEFAULT
/********************************************************/
/*****************main program*********************/
#pragma CODE_SEG  USER_ROM

void main(void) 
{ 
  CpuInit();	
 	for(;;)
 	{   	 
		clrwatchdog();

	  if((bootloader_wait_flag==0)&&(boot_reset_flag==0))  
	  {
			KeyDeal();				
			Load_Deal();
			MesseageDeal(); 
			SensorDealFunction();
			StepDeal();  
			Distance_Counter();
			updataRealTime(); 
			Tft_Display();
	  }	  
  } 
}
#pragma CODE_SEG DEFAULT

