#ifndef __LED_H_
#define __LED_H_
#include "S32K144.h"
#include "Common.h"
#include "Typedef.h"
EXT UINT8 LeftTurnSignalWarn;
EXT UINT8 RightTurnSignalWarn;
extern BitCommon ledShow[4];
//LED灯定义


#define LED15_595				ledShow[0].Bits.bit0	//32 - 32  总电源指示,
#define LED1_595				       ledShow[0].Bits.bit1	//33 - 50  前门开
#define LED3_595				       ledShow[0].Bits.bit2	//34 - 39 水位报警指示(少掉发动机预热)
#define LED5_595				       ledShow[0].Bits.bit3	//35 - 38 驻车符号指示
#define LED7_595				       ledShow[0].Bits.bit4	//36 - 37 左前蹄片报警指示灯
#define LED9_595				       ledShow[0].Bits.bit5 	//37 - 35 远光灯
#define LED11_595				ledShow[0].Bits.bit6	//38 - 34 近光灯
#define LED13_595				ledShow[0].Bits.bit7	//39 - 33 轻微故障
//---------------------------------------
#define LED16_595				ledShow[1].Bits.bit0	//40 - 45  后雾灯符号片
#define LED14_595				ledShow[1].Bits.bit1 	//41 - 46前雾灯符号片
#define LED12_595				ledShow[1].Bits.bit2	//42 - 47二级故障指示灯
#define LED10_595				ledShow[1].Bits.bit3	//43 - 42右前蹄片报警指示灯
#define LED8_595				      ledShow[1].Bits.bit4	//44 - 41气压报警指示灯
#define LED6_595				      ledShow[1].Bits.bit5	//45 - 40发动机仓门开指示
#define LED4_595				      ledShow[1].Bits.bit6	//46 -  54位置灯指示灯
#define LED2_595				      ledShow[1].Bits.bit7	//47 - 51 中门开
#define LED38_595                        ledShow[2].Bits.bit2
#define LED67_595			      ledShow[2].Bits.bit4	//47 - 51 中门开
#define LED68_595                        ledShow[2].Bits.bit5
#define TURN_LEFT_LED 		       LED38_595
#define TURN_RIGHT_LED		       LED68_595


#define LED21_IS				ledShow[3].Bits.bit0	//40 - 45  后雾灯符号片
#define LED34_IS				ledShow[3].Bits.bit1 	//41 - 46前雾灯符号片
#define LED32_IS				ledShow[3].Bits.bit2	//42 - 47二级故障指示灯
#define LED30_IS				ledShow[3].Bits.bit3	//43 - 42右前蹄片报警指示灯
#define LED25_IS			      ledShow[3].Bits.bit4	//44 - 41气压报警指示灯
#define LED31_IS			      ledShow[3].Bits.bit5	//45 - 40发动机仓门开指示
#define LED35_IS			      ledShow[3].Bits.bit6	//46 -  54位置灯指示灯
#define LED33_IS			      ledShow[3].Bits.bit7	//47 - 51 中门开



//---------------------------------------
//------------------------------------------------------------
//#define LED17_595				ledShow[2].Bits.bit6	//转速报警
//#define LED37_595				ledShow[2].Bits.bit3	//车速报警
//---------------------------------------
//------------------------------------------------------------


#define SLIGHT_FAULT		LED13_595//ledShow[0].Bits.bit7	//轻微故障黄色报警灯
#define SERIOUS_FAULT		LED12_595//ledShow[1].Bits.bit2	//严重故障红色报警灯

void ledInit(void);
void ledInterface(void);
void	Out595(unsigned char	*buf);
#endif  