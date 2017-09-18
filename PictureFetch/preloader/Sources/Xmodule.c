#include "Xmodule.h"
#include "sci.h"




void Xmodule_init(void) {
 SCI0_Init();  
}


void XmodemWaitStart(void) {
  unsigned long  timercount = 0; 
  while(SCI0_Getbyte()!= XMODEM_SOH) {//wait start single
    if(++timercount > 10000)//wait about 1 second
    {
      SCI0_Send(XMODEM_RECIEVING_WAIT_CHAR);//send a "C"
      timercount=0;
     }            
   }
}

void clearPacketData(XmodemData *pdata) {
    pdata->Start  = 0x00;
    pdata->Type   = 0x00;
    pdata->Datalen.data16 = 0x00;	
      
    pdata->Crc    = 0x00;
    
    pdata->End    = 0x00;                      
    pdata->isFinish = 0x00;	
}

U8 CheckSum(U8* buff, U32 bufflen) {
   long i;
   U8 temp = 0;
   for(i =0 ; i< bufflen; i++) {
      temp += buff[i];  
   }
   return temp;
}


void RecviceData(XmodemData* Packet) {
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
     Packet->Crc = uart_waitchar();	
     Packet->End          = uart_waitchar();
     
     //crc 校验
     if( CheckSum(Packet->Data, Packet->Datalen.data16) == Packet->Crc) 
     {      
      Packet->isFinish = 0x01;
     } else {
      Packet->isFinish = 0x00;
     }
}


U32 getFileDownloadAddress(XmodemData* Packet) {
    UnionData32 data;
    data.data8[0] =  Packet->Data[0];
    data.data8[1] =  Packet->Data[1];
    data.data8[2] =  Packet->Data[2];
    data.data8[3] =  Packet->Data[4];
    return data.data32;
    
  //  U32 address = 0;
  //  address = (Packet->Data[0]) +  (Packet->Data[1] << 8) + (Packet->Data[2] << 16) + (Packet->Data[3] << 24);
  //   
  //  return address;
}

U32 getFileDownloadSize( XmodemData* Packet) {
    UnionData32 data;
    data.data8[0] =  Packet->Data[4];
    data.data8[1] =  Packet->Data[51];
    data.data8[2] =  Packet->Data[6];
    data.data8[3] =  Packet->Data[7];
    return data.data32;
  // return (Packet->Data[4]) +  (Packet->Data[5] << 8) + (Packet->Data[6] << 16) + (Packet->Data[7] << 24);
}


char XmodemRecivePacket(XmodemData* Packet) 
{
  RecviceData(Packet);
  if(Packet->isFinish == 0x01)
    return 0;
  else 
    return -1;
   
}
  

//void reciveMsgPacket(DownloadPicture *P) 
//{
//  P->PictureBaseAddress = (P->Packet.Data[0]) +  (P->Packet.Data[1] << 8) + (P->Packet.Data[2] << 16) + (P->Packet.Data[3] << 24);
//  P->PictureCurrentAddress = P->PictureBaseAddress;
//  P->PictureBytes   = (P->Packet.Data[4]) +  (P->Packet.Data[5] << 8) + (P->Packet.Data[6] << 16) + (P->Packet.Data[7] << 24);
//  P->HasDownBytes   = 0;
//  P->isFinish = 0;
  //擦除数据SPI FLASH 数据
//}


//void recivePicturePacket(DownloadPicture *P) 
//{
 //  if(P->PictureBytes == 0){
 //     return;
 //  }
   
   // set SPI Flash Address 和P->PictureAddress
   //烧写SPI
   	
//   P->PictureCurrentAddress = P->PictureCurrentAddress +  P->Packet.Datalen.data16;
//   P->HasDownBytes += P->Packet.Datalen.data16;
//   if (P->HasDownBytes == P->PictureBytes) {
//    P->isFinish = 1;
 //  }
//}


