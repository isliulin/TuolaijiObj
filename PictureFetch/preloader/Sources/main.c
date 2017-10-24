#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <stdlib.h>
#include "sci.h"     //uart function
#include "typedef.h"
#include "Xmodule.h"
#include "MX25L12845D.h"

#include "TW_control.h"
                                 



typedef struct DownloadPicture{
    XmodemData Packet;
    U32 PictureBaseAddress;   //spi address
    U32 PictureCurrentAddress;   //spi address
    U32 PictureBytes;     //picture size
    U32 HasDownBytes;     //has Down Byte 
    U8  isFinish;   
}DownloadPicture;

DownloadPicture PictureMsg;


void pwm_init() {
 
PWMCTL |= 0b01000000;	///6bit->->??1??PWM??4?5???16bit pwm;
 // #if		J13_OUT==1
  	PWMCTL |= 0b00100000;	///5bit->??1??PWM??2?3???16bit pwm;
 // #endif
	PWMPRCLK = 0x07; /*6~4bit->B = clock prescale is 8;2~0bit->A =clock prescale */
	PWMCLK=0b00000000;	/*Clock SB is the clock source for PWM channel*/
	///channel 4/5
  PWMPOL_PPOL5 = 1; /*PWM POL ENALBE,outputs are high at the beginning of the period*/
	PWMCAE_CAE5 = 0; /* left aligned Edge Output Mode on channel 6 */
	PWMPER45 = 0xfffe; /*peoriods of the channel pwm,first set to high all*/
	PWMDTY45	= 0xffff;/*PWM Channel 6 Duty*/
	PWME_PWME5 = 1; /*PWM CHANNEL ENABLE*/
//	#if		J13_OUT==1
		///channel 2/3
	  PWMPOL_PPOL3 = 1; /*PWM POL ENALBE,outputs are high at the beginning of the period*/
		PWMCAE_CAE3 = 0; /* left aligned Edge Output Mode on channel 6 */
		PWMPER23 = 0xfffe; /*peoriods of the channel pwm,first set to high all*/
		PWMDTY23	= 0xffff;/*PWM Channel 6 Duty*/
		PWME_PWME3 = 1; /*PWM CHANNEL ENABLE*/
//	#endif
	
	PWMPER1 = 0x35; ///?????PWM??
	PWMDTY1 = 0x35; ///PWM???
	PWME_PWME1 = 1;	///????

	PWMPER6 = 0xd0; ///???????PWM??
	PWMDTY6 = 0xd0; ///PWM???
	PWME_PWME6 = 1;	///????

	PWMPER5 = 0xd0; ///???????PWM??
	PWMDTY5 = 0xd0; ///PWM???
	PWME_PWME5 = 1;	///????
}

void spiflash_init() {
  
  
}

void memset(uchar* src, uchar value, U32 len) {
    U32 i;
    for(i = 0; i < len; i++) {
      src[i] = value; 
  }    
}

#define	LED_LEFT						PORTK_PK0
#define	LED_LEFT_IO	   			DDRK_DDRK0

#define	LED_RIGHT					  PORTB_PB0
#define	LED_RIGHT_IO	   		DDRB_DDRB0




#define	MODE_CTRL1						PORTE_PE4
#define	MODE_CTRL_IO1					DDRE_DDRE4

void init_io(void)
{
  LED_LEFT_IO = 1;
  LED_LEFT  = 0  ;
  
  
  LED_RIGHT_IO = 1;
  LED_RIGHT  = 0  ;
  
  LCDCR0_LCDEN=0;
  
//  MODE_CTRL_IO1 = 1;
 // MODE_CTRL1 = 1;
  
  
  
}

static void CRGModuleInitial(void)
{
	CLKSEL_PLLSEL = 0; 
	CLKSEL_RTIWAI = 1; ///rtc stop in wait mode
	CLKSEL_COPWAI = 1; ///cop stop in wait mode
	RTICTL = 0x49; ///16M OSC 5.12ms enter RTI;10x(1024*8)/16M
	COPCTL = 0x00; ///1s will reset COP  funtion
	CRGINT_RTIE = 1; ///RTI interrupt enable
}




void main(void) {
  /* put your own code here */
  
  dword  timercount = 0;
  unsigned char flashID = 0;
  
  ulong addr = 0x200;
  uchar buff[512];
  int i;
  uchar ret = 0;
  BYTE tw8835ID = 0;

  
  asm sei;///¹Ø±ÕÖÐ¶Ï
 //  CRGModuleInitial();
 //  init_io();
  
  
 // spiflash_init();
//  pwm_init();
Bootloader_Mode_Change();
 
  
  // Xmodule_init();
  
  
  flashID = Flash_Read_ID();

//  tw8835ID = ReadTW88(0x00);	

//  flashID=1;
              
 
// Erase_Flash();
 
 
 
//  XmodemWaitStart();

  //XmodemRecivePacket(&PictureMsg.Packet);
  
  //  Tw_Pannal_Parameter_Init();
   // BackLightCmd(1);
  
    i =1;
   // ret = Flash_Page_Write(addr, sizeof(buff), buff);
     i = 0; 

    for(i=0;i<512;i++) {
      buff[i] = 0;  
    }
     //  Bootloader_Mode_Change();
   // ret = Flash_PageRead(addr, sizeof(buff), buff);
    i=4;
   // XmodemWaitStart();
    // char ret = XmodemRecivePacket(&PictureMsg.Packet);
    
   // RecviceXmodemData(&PictureMsg.Packet);  //  have finish a packet
   // AnalysisXmodemData(&PictureMsg);   
  
   
  


	EnableInterrupts;


  for(;;) {
  LED_LEFT = 0;
  LED_RIGHT =0;
  
  for(i = 0; i < 20000; i++) {
       asm nop;
  }
    
  
  LED_LEFT = 1;
  LED_RIGHT =1;
  
    for(i = 0; i < 20000; i++) {
       asm nop;
  }
    
  
  
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
