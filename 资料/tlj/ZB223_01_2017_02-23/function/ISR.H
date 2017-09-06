#ifndef __ISR_H_
#define __ISR_H_

#include	"typedef.h"
#include "xgate.h"
#include "Led.h"
#include	"TW_FontOsd.h"
#include	"TW_SpiOsd.h"
#include	"common.h"
#include "WaitTimer.h"
#include <stdlib.h>
#include "display.h" 
#include "CanMsgOut.h"

#define		PrescaleNum		80



#define  CAN0_PC_SOURCE_ID                0x01
#define  CAN0_METER_SOURCE_ID             0x02
#define  CAN0_FRONT_SOURCE_ID             0x03               
#define  CAN0_BACK_SOURCE_ID              0x04
#define  CAN0_TOP_SOURCE_ID               0x05
#define  CAN0_POWER_SOURCE_ID             0x07
#define  CAN0_POWER_CONTOROL_ID           0X0A
#define  CAN0_360_video_ID                0x80

#define  CAN0_LISTEN_MODE                 0x00                    
#define  CAN0_EXIT_LISTEN_MODE            0x08

#define  CAN0_BT_TYPE                     0x1// 下载
#define  CAN0_SW_TYPE                     0x2//（20物理开关）
#define  CAN0_VIRTUAL_SW_TYPE             0x3//（30虚拟开关）
#define  CAN0_LOAD_TYPE                   0x4// (40负载状态)
#define  CAN0_SENSOR_TYPE                 0x5// (50 51 52 传感器+电流)                  
#define  CAN0_SYS_CONFIG_TYPE             0X7// 系统配置（70雨刮间歇等，71休眠 72）    
#define  CAN0_MODE_VERSION_TYPE           0x8// 80版本号
#define  CAN0_CAR_PARAMETER_TYPE          0x9// 总里程油耗  里程小计  
#define  CAN0_360_video                   0x10 //360摄像机
#define  CAN0_SET_SYS_TYPE                0xa

/*****Functions Prototypes**********/
//#pragma CODE_SEG  __NEAR_SEG	NON_BANKED
//EXT interrupt void rti_int(void);
//EXT interrupt void irq_int(void);
//EXT interrupt void timer1IntHander(void);
//EXT interrupt void WatchDogIsr(void);
EXT void initPIT1(void);
EXT void initTimeParameter(void);
EXT void clearTimeFlag(void);
//#pragma CODE_SEG DEFAULT
	/****************ram macros********************/
EXT word TestStepTime;
EXT UINT16 TimeCnt;
EXT UINT16 RtiCnt;
EXT  BitCommon TimeFlag;
#define  _2MS_F                                    TimeFlag.Bits.bit0
#define  KEY_SCANF_10MS_F                          TimeFlag.Bits.bit1
#define  _50MS_F                                   TimeFlag.Bits.bit2
#define  _100MS_F                                  TimeFlag.Bits.bit3
#define  CAN0_200MS_F                              TimeFlag.Bits.bit4
#define  _300MS_F                                  TimeFlag.Bits.bit5
#define  _500MS_F                                  TimeFlag.Bits.bit6
#define  _1S_F                                     TimeFlag.Bits.bit7

EXT  BitCommon DisTimeFlag;
#define  _1S_DIS_F                                 DisTimeFlag.Bits.bit0
#define  DIS_Blink1_F                              DisTimeFlag.Bits.bit1
#define  DIS_Blink2_F                              DisTimeFlag.Bits.bit2
#define  FUAL_REQUIRE_F                            DisTimeFlag.Bits.bit3
#define  UPDOWN_F                                  DisTimeFlag.Bits.bit4
#define  D_1S_F                                    DisTimeFlag.Bits.bit5
#define  TPMS_1S_F                                 DisTimeFlag.Bits.bit6
#define  SENSOR_1S_F                               DisTimeFlag.Bits.bit7
//EXT UINT8 _2msCnt;
EXT UINT8 _10msCnt;
EXT UINT8 _50msCnt;
EXT UINT8 _100msCnt;


typedef struct
{
 uint16 highTime;      //十六位定时器不够用扩展
 uint32 preCaptureTime;
 uint32 nextCaptureTime; 
 uint8  upflag;        //  更新标志
 uint8 captureCount; 
 uint16 captureToFre; 
}St_CaptureTime;


/*******************parameter macros**************************/

#define CAN_STATUS_TX1_MASK      		0X04 /* Bus-Off state mask in register */
#define CAN_STATUS_RX1_MASK     		0X10 /* Bus-Off state mask in error flag */
#define CAN_STATUS_TX_MASK      		0X0C /* Bus-Off state mask in register */
#define CAN_STATUS_RX_MASK     	 		0X30 /* Bus-Off state mask in error flag */
/****************IO macros********************/
#define Xirq_Io	    	     PORTE_PE0
#define Irq_Io				     PORTE_PE1
#define	Tw8816_Power	     PORTD_PD1
extern UINT8  TEST_DATA[6];
extern UINT8  Power_Current_Valve[16];        //
extern UINT8 listen_mode_flag;
extern UINT32 PcFromMileage;



void lpit0_ch0_isr(void);
void lpit0_ch1_isr(void);
void lpit0_ch2_isr(void);
void lpit0_ch3_isr(void);
void lpit0_isr(void);
void can1Recieve_isr(void);
void CAN0rxd_int(void); 
extern St_CaptureTime   CapturePwm0,CapturePwm1;
void  CaptureSpeed(St_CaptureTime *stCaptureTime);
#endif