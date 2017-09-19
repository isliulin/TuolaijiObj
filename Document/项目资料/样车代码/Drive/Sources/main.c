/**********************************************************
**			YuTong Drive 
***********************************************************
**Copyright: (C) 2011 Xmhns Company
**File:       		Main.c
**Mcu:	     			MC9S08DZ60
**Client:					YuTong-KeChe
**Author:     		Karry
**Version:    		V1.0
**Date:       		2011-05-28
***********************************************************/
/*******************************************************/
#include    "main.h"
/*******************main program*************************/

void main(void)
{   	
   CpuInit();
    
  for(;;)
	{    
		__RESET_WATCHDOG();/*feed watch dog*/
		External_WatchDog();
		
		KeyDeal();
				
		LoadDeal();					

		if(SleepStartFlags==0)		
		{
			SensorDataDeal();		
		
     		MessageDeal();      
		}
	}	
}

