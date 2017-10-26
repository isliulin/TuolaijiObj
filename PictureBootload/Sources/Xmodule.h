#ifndef __XMODULE_H_
#define __XMODULE_H_

#define XMODEM_NUL 0x00
#define XMODEM_SOH 0x01
#define XMODEM_STX 0x02
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_CAN 0x18
#define XMODEM_EOF 0x1A
#define XMODEM_RECIEVING_WAIT_CHAR 'C'

typedef unsigned char uchar;
typedef unsigned int uint;   

typedef unsigned char BOOL;
typedef signed char S8,*PS8;
typedef unsigned char BYTE,U8,*PU8;
typedef signed short int S16,*PS16;
typedef unsigned short int WORD,U16,*PU16;
typedef signed long LONG,S32,*PS32;
typedef unsigned long DWORD,U32,*PU32;
typedef unsigned char	Register;

 typedef union UnionData{
   U8   data8[2];
   U16  data16;
 }UnionData;
 
 typedef union UnionData32{
    U8   data8[4];
    U32  data32;
 }UnionData32;

// |                     data format                      |
// | start | type | data len |  data          |  CRC  | end  |
// | 1bytes|1bytes|  2bytes  |     ....       | 1bytes|1bytes|
// | 0xaa  | 0x01 |  Lx   Hx |  xx ... xx  xx |  xx   | 0xbb |
//                               |   |   |        |
// | ----- | ---- | -------- |  xx + ... +xx  =  CRC  | ---- |
typedef struct XmodemData{
  U8 Start;   //起始位
  U8 Type;
  UnionData Datalen;
  U8  Data[512];
  U8  Crc;
  U8 End;
  
  U8 isFinish; // 是否接受完成
}XmodemData;


typedef struct _Packet{
  U32 FileDownLoadAddress;
  U32 FileSize;
  U32 HasDownLoadSize;
}TPacket;



#define XMODEM_OK       0x43
#define XMODEM_TIMEOUT  0x44
#define XMODEM_CRCERR   0x45
#define XMODEM_RESEND   0x46
#define XMODEM_ERROR    0x47

void Xmodule_init(void);
void XmodemWaitStart(void);
void XmodemAck(U8 msg);

U32 getFileDownloadAddress(XmodemData* Packet);
U32 getFileDownloadSize( XmodemData* Packet) ;

char XmodemRecivePacket(XmodemData* Packet);


#endif //_XMODULE_H_