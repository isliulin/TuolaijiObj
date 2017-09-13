#ifndef	_SCI_H_
#define	_SCI_H_

#include "config.h"

#define SCI0_NUM    128
#define SCI1_NUM    12

#define SCI_FRAME_NUM   5
#define TIME_2S    30 // 200ms*30���ڽ������ݼ�����ڴ�ʱ������Ϊ����һ������֡

extern U8 SCI_data;
//�������ݽṹ
typedef struct
{
    U8  type;           //��������1��2��3��4
    U8  flag;           //�Ƿ��������ݣ��ǣ�1����0
    U16 len;            //buf�����ݳ���
    U8  ack;            //1����Ӧ��0������Ӧ��
    U8  fup;            //1���ϴ���0�����ϴ�
    U8  frametype;      //1��Ӧ��֡��0������֡
    U8  cmdtype;        //0x01:can���ݣ�02��ioָ�03��·�����ݣ�04�����������ݣ�05��POS����06��ˢ����
    U8  buf[SCI0_NUM];  //����
}SCI_DAT;

typedef struct
{
    U8  type;           //��������1��2��3��4
    U8  flag;           //�Ƿ��������ݣ��ǣ�1����0
    U16 len;            //buf�����ݳ���
    U8  ack;            //1����Ӧ��0������Ӧ��
    U8  fup;            //1���ϴ���0�����ϴ�
    U8  frametype;      //1��Ӧ��֡��0������֡
    U8  cmdtype;        //0x01:can���ݣ�02��ioָ�03��·�����ݣ�04�����������ݣ�05��POS����06��ˢ����
    U8  buf[SCI1_NUM];  //����1�Ƚ�С����һ���Ƚ�С��
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

//����֡���нṹ
typedef struct
{
    U8  in;
    U8  out;
    SCI_DAT  frame[SCI_FRAME_NUM];    
}SCI_QUE;


/****************ȫ�ֱ�������******************/




/****************��������**********************/
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
*��������Write_SCIFrameToQue
*��  �ܣ�дSCI���ݶ���
*��  ����*pFrame Ҫд�������
*��  �أ�void
************************************************/
void Write_SCIFrameToQue(SCI_DAT* pFrame);
/***********************************************
*��������Read_SCIFrameFromQue
*��  �ܣ���SCI���ݶ���
*��  ����*pFrame �������������
*��  �أ�0���ɹ���255��ʧ��
************************************************/
U8 Read_SCIFrameFromQue(SCI_DAT* pFrame);
/***********************************************
*��������Send_LedDat
*��  �ܣ�����·������
*��  ����len ���ݳ���
*��  �أ�void
************************************************/
void Send_LedDat();

#endif

