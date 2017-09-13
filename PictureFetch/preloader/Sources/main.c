#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <stdlib.h>
#include "sci.h"     //uart function
#include "typedef.h"
                                 
#define XMODEM_NUL 0x00
#define XMODEM_SOH 0x01
#define XMODEM_STX 0x02
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_CAN 0x18
#define XMODEM_EOF 0x1A
#define XMODEM_RECIEVING_WAIT_CHAR 'C'


void uart_init(void) {
 SCI0_Init();  
}


void WaitXmodemStart(void) {
  dword  timercount = 0; 
  while(SCI0_Getbyte()!= XMODEM_SOH) {//wait start single
    if(++timercount > 10000)//wait about 1 second
    {
      SCI0_Send(XMODEM_RECIEVING_WAIT_CHAR);//send a "C"
      timercount=0;
     }            
   }
}

 typedef union UnionData{
   U8   data8[2];
   U16  data16;
 }UnionData;

// |                     data format                      |
// | start | type | data len |  data          |  CRC  | end  |
// | 0xaa  | 0x01 |  Lx   Hx |  xx ... xx  xx | Lx Hx | 0xbb |
//                               |   |   |        |
// | ----- | ---- | -------- |  xx + ... +xx  =  CRC  | ---- |
typedef struct XmodemData{
  U8 Start;   //起始位
  U8 Type;
  UnionData Datalen;
  U8  Data[512];
  UnionData Crc;
  U8 End;
  
  U8 isFinish; // 是否接受完成
}XmodemData;


void clearPacketData(XmodemData *pdata) {
    pdata->Start  = 0x00;
    pdata->Type   = 0x00;
    pdata->Datalen.data16 = 0x00;	
      
    pdata->Crc.data16    = 0x00;
    
    pdata->End    = 0x00;                      
    pdata->isFinish = 0x00;	
}


typedef struct DownloadPicture{
    XmodemData Packet;
    U32 PictureBaseAddress;   //spi address
    U32 PictureCurrentAddress;   //spi address
    U32 PictureBytes;     //picture size
    U32 HasDownBytes;     //has Down Byte 
    U8  isFinish;   
}DownloadPicture;

DownloadPicture PictureMsg;

union
{
   U8 CharData[2];
  int Int16Data;
}Int2Char1;

#define crc Int2Char1.Int16Data
#define crcLow Int2Char1.CharData[0]
#define crcHigh Int2Char1.CharData[1]


void RecviceXmodemData(XmodemData* Packet) {
     U16 Datalen;
     U16 i;
      
     clearPacketData(Packet);
     Packet->Start = uart_waitchar();
     Packet->Type  = uart_waitchar();
     Packet->Datalen.data8[0] = uart_waitchar();	
     Packet->Datalen.data8[1] = uart_waitchar();
     
     Datalen = Packet->Datalen.data16;
     for(i = 0; i < Datalen; i++) 
     {
       Packet->Data[i] = uart_waitchar();
     }
     Packet->Crc.data8[0] = uart_waitchar();	
     Packet->Crc.data8[1] = uart_waitchar();
     Packet->End          = uart_waitchar();
     
     //crc 校验
     
     Packet->isFinish = 0x01;          
}

void reciveMsgPacket(DownloadPicture *P) 
{
  P->PictureBaseAddress = (P->Packet.Data[0]) +  (P->Packet.Data[1] << 8) + (P->Packet.Data[2] << 16) + (P->Packet.Data[3] << 24);
  P->PictureCurrentAddress = P->PictureBaseAddress;
  P->PictureBytes   = (P->Packet.Data[4]) +  (P->Packet.Data[5] << 8) + (P->Packet.Data[6] << 16) + (P->Packet.Data[7] << 24);
  P->HasDownBytes   = 0;
  P->isFinish = 0;
  //擦除数据SPI FLASH 数据
}


void recivePicturePacket(DownloadPicture *P) 
{
   if(P->PictureBytes == 0){
      return;
   }
   
   // set SPI Flash Address 和P->PictureAddress
   //烧写SPI
   	
   P->PictureCurrentAddress = P->PictureCurrentAddress +  P->Packet.Datalen.data16;
   P->HasDownBytes += P->Packet.Datalen.data16;
   if (P->HasDownBytes == P->PictureBytes) {
    P->isFinish = 1;
   }
}


void AnalysisXmodemData(DownloadPicture *P) {
    if(P->Packet.isFinish == 0x01) {
       switch(P->Packet.Type) {
        case 0x01:// 发送烧写地址
          reciveMsgPacket(P);
          break;
        case 0x02:// 发送图片数据
          recivePicturePacket(P);
          break;
        default:
          break;
       }      
    }else{
      //数据错误，重新发送 
    }
}


void spiflash_init() {
  
  
}

void main(void) {
  /* put your own code here */
  
  dword  timercount = 0;
  
  asm sei;///关闭中断
  
  uart_init();
  
  spiflash_init();
   
  for(;;) {
   // WaitXmodemStart();
    
    RecviceXmodemData(&PictureMsg.Packet);  //  have finish a packet
    AnalysisXmodemData(&PictureMsg);   
  }
   
  


	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
