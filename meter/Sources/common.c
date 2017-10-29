#define 	COMMON_C
#include	"config.h"
#pragma CODE_SEG  USER_ROM
/*******************************************
funtion operation:delay time 1ms
funtion call:void
input parameter:delay,the number of delay time
return:void
 ********************************************/
void DelayMs(unsigned int Delay)
{
  unsigned int i, j;

  for (i = 0; i < Delay; i++)
  {
    for (j = 0; j < 1000; j++)
    {
      asm nop;
      asm nop;
    }
  }
}
/***********************************************

************************************************/

void Clear_256_WatchDog(void)
{
	WDT_IO=1;
		  
	if(WDT==1)
		WDT=0; 	
	else 
		WDT=1; 
}
/****************************************

*****************************************/
void	ClearWatchDog(void)
{
	ARMCOP=WatchDogResetParameter1;		 //reset COP					
  ARMCOP=WatchDogResetParameter2;	
}
void	Clear_External_WatchDog(void)
{

	WriteTW88Page(0x00);
	WriteTW88(0x86,0x80);
	WriteTW88(0x8e,0x80);

	if(TW88XX_WatchDog_Flag==0)
	{
		WriteTW88(0x96,0x80);
		WriteTW88(0x9e,0x80);
		asm nop;
		WriteTW88(0x96,0x80);
		WriteTW88(0x9e,0x80);
		asm nop;
		TW88XX_WatchDog_Flag = 1;
		
	}
	else
	{
		WriteTW88(0x96,0x00);
		WriteTW88(0x9e,0x00);
		asm nop;
		WriteTW88(0x96,0x00);
		WriteTW88(0x9e,0x00);
		asm nop;
		TW88XX_WatchDog_Flag = 0;
	}
}
void clrwatchdog(void)
{
	Clear_256_WatchDog();
	///if(TVPowerOff_Flag==1)
	if(OnKeyBit)
		Clear_External_WatchDog();
}
#pragma CODE_SEG DEFAULT

