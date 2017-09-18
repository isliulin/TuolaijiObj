#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <stdlib.h>
#include "sci.h"     //uart function
#include "typedef.h"
#include "Xmodule.h"
#include "MX25L12845D.h"
                                 



typedef struct DownloadPicture{
    XmodemData Packet;
    U32 PictureBaseAddress;   //spi address
    U32 PictureCurrentAddress;   //spi address
    U32 PictureBytes;     //picture size
    U32 HasDownBytes;     //has Down Byte 
    U8  isFinish;   
}DownloadPicture;

DownloadPicture PictureMsg;



void spiflash_init() {
  
  
}

void main(void) {
  /* put your own code here */
  
  dword  timercount = 0;
  unsigned char flashID = 0;
  
  asm sei;///πÿ±’÷–∂œ
  
  Xmodule_init();
  
  
  spiflash_init();
  Bootloader_Mode_Change();
  flashID = Flash_Read_ID();
  for(;;) {
   // XmodemWaitStart();
  // char ret = XmodemRecivePacket(&PictureMsg.Packet);
    
   // RecviceXmodemData(&PictureMsg.Packet);  //  have finish a packet
   // AnalysisXmodemData(&PictureMsg);   
  }
   
  


	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
