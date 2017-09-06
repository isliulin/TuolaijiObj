#ifndef __LED_H_
#define __LED_H_
#include "S32K144.h"
#include "Common.h"
#include "Typedef.h"
EXT UINT8 LeftTurnSignalWarn;
EXT UINT8 RightTurnSignalWarn;
extern BitCommon ledShow[4];
//LED�ƶ���


#define LED15_595				ledShow[0].Bits.bit0	//32 - 32  �ܵ�Դָʾ,
#define LED1_595				       ledShow[0].Bits.bit1	//33 - 50  ǰ�ſ�
#define LED3_595				       ledShow[0].Bits.bit2	//34 - 39 ˮλ����ָʾ(�ٵ�������Ԥ��)
#define LED5_595				       ledShow[0].Bits.bit3	//35 - 38 פ������ָʾ
#define LED7_595				       ledShow[0].Bits.bit4	//36 - 37 ��ǰ��Ƭ����ָʾ��
#define LED9_595				       ledShow[0].Bits.bit5 	//37 - 35 Զ���
#define LED11_595				ledShow[0].Bits.bit6	//38 - 34 �����
#define LED13_595				ledShow[0].Bits.bit7	//39 - 33 ��΢����
//---------------------------------------
#define LED16_595				ledShow[1].Bits.bit0	//40 - 45  ����Ʒ���Ƭ
#define LED14_595				ledShow[1].Bits.bit1 	//41 - 46ǰ��Ʒ���Ƭ
#define LED12_595				ledShow[1].Bits.bit2	//42 - 47��������ָʾ��
#define LED10_595				ledShow[1].Bits.bit3	//43 - 42��ǰ��Ƭ����ָʾ��
#define LED8_595				      ledShow[1].Bits.bit4	//44 - 41��ѹ����ָʾ��
#define LED6_595				      ledShow[1].Bits.bit5	//45 - 40���������ſ�ָʾ
#define LED4_595				      ledShow[1].Bits.bit6	//46 -  54λ�õ�ָʾ��
#define LED2_595				      ledShow[1].Bits.bit7	//47 - 51 ���ſ�
#define LED38_595                        ledShow[2].Bits.bit2
#define LED67_595			      ledShow[2].Bits.bit4	//47 - 51 ���ſ�
#define LED68_595                        ledShow[2].Bits.bit5
#define TURN_LEFT_LED 		       LED38_595
#define TURN_RIGHT_LED		       LED68_595


#define LED21_IS				ledShow[3].Bits.bit0	//40 - 45  ����Ʒ���Ƭ
#define LED34_IS				ledShow[3].Bits.bit1 	//41 - 46ǰ��Ʒ���Ƭ
#define LED32_IS				ledShow[3].Bits.bit2	//42 - 47��������ָʾ��
#define LED30_IS				ledShow[3].Bits.bit3	//43 - 42��ǰ��Ƭ����ָʾ��
#define LED25_IS			      ledShow[3].Bits.bit4	//44 - 41��ѹ����ָʾ��
#define LED31_IS			      ledShow[3].Bits.bit5	//45 - 40���������ſ�ָʾ
#define LED35_IS			      ledShow[3].Bits.bit6	//46 -  54λ�õ�ָʾ��
#define LED33_IS			      ledShow[3].Bits.bit7	//47 - 51 ���ſ�



//---------------------------------------
//------------------------------------------------------------
//#define LED17_595				ledShow[2].Bits.bit6	//ת�ٱ���
//#define LED37_595				ledShow[2].Bits.bit3	//���ٱ���
//---------------------------------------
//------------------------------------------------------------


#define SLIGHT_FAULT		LED13_595//ledShow[0].Bits.bit7	//��΢���ϻ�ɫ������
#define SERIOUS_FAULT		LED12_595//ledShow[1].Bits.bit2	//���ع��Ϻ�ɫ������

void ledInit(void);
void ledInterface(void);
void	Out595(unsigned char	*buf);
#endif  