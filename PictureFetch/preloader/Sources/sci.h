#ifndef	_SCI_H_
#define	_SCI_H_

#include "config.h"

#define SCI0_NUM    128
#define SCI1_NUM    12

#define SCI_FRAME_NUM   5
#define TIME_2S    30 // 200ms*30串口接收数据间隔大于此时间则认为是另一包数据帧

extern U8 SCI_data;
//串口数据结构
typedef struct
{
    U8  type;           //串口类型1，2，3，4
    U8  flag;           //是否是新数据，是：1，否：0
    U16 len;            //buf的数据长度
    U8  ack;            //1：需应答，0：不必应答
    U8  fup;            //1：上传，0：不上传
    U8  frametype;      //1：应答帧，0：数据帧
    U8  cmdtype;        //0x01:can数据，02：io指令，03：路牌数据，04：调度屏数据，05：POS机，06：刷卡机
    U8  buf[SCI0_NUM];  //数据
}SCI_DAT;

typedef struct
{
    U8  type;           //串口类型1，2，3，4
    U8  flag;           //是否是新数据，是：1，否：0
    U16 len;            //buf的数据长度
    U8  ack;            //1：需应答，0：不必应答
    U8  fup;            //1：上传，0：不上传
    U8  frametype;      //1：应答帧，0：数据帧
    U8  cmdtype;        //0x01:can数据，02：io指令，03：路牌数据，04：调度屏数据，05：POS机，06：刷卡机
    U8  buf[SCI1_NUM];  //串口1比较小另定义一个比较小的
}SCI1_DAT;


typedef union
{
    U8 dat[SCI0_NUM+8];
    SCI_DAT sci;
}SCIDAT;




typedef union
{
    U8 dat[SCI1_NUM+8];
    SCI1_DAT sci;
}SCI1DAT;

//串口帧队列结构
typedef struct
{
    U8  in;
    U8  out;
    SCI_DAT  frame[SCI_FRAME_NUM];    
}SCI_QUE;


/****************全局变量定义******************/




/****************函数声明**********************/
void SCI0_Init(void);
void SCI0_Send(U8 dat);
void SCI0_Sendbytes(U8 *pdat,U16 len);
U8 SCI0_Getbyte(void);
void SCI1_Init(void);
void SCI1_Send(U8 dat);
void SCI1_Sendbytes(U8 *pdat,U16 len);

void SCI2_Init(void);
void SCI2_Send(U8 dat);
void SCI2_Sendbytes(U8 *pdat,U16 len);

void SCI4_Init(void);
void SCI4_Send(U8 dat);
void SCI4_Sendbytes(U8 *pdat,U16 len);
U8 uart_waitchar(void);

/***********************************************
*函数名：Write_SCIFrameToQue
*功  能：写SCI数据队列
*参  数：*pFrame 要写入的数据
*返  回：void
************************************************/
void Write_SCIFrameToQue(SCI_DAT* pFrame);
/***********************************************
*函数名：Read_SCIFrameFromQue
*功  能：读SCI数据队列
*参  数：*pFrame 保存读出的数据
*返  回：0：成功，255：失败
************************************************/
U8 Read_SCIFrameFromQue(SCI_DAT* pFrame);
/***********************************************
*函数名：Send_LedDat
*功  能：发送路牌数据
*参  数：len 数据长度
*返  回：void
************************************************/
void Send_LedDat();

#endif

