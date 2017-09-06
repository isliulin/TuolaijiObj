#ifndef	__KEYDEAL_H_
#define __KEYDEAL_H_

#include "typedef.h"
#include "S32K144.h"
#include "common.h"
#include "load_deal.h"
#include "ap_S32K1xx_adc.h"
#include "isr.h"
#include "EngineDiagnose_DM1.h" 
#include "load_deal.h"
#include "LoadOutputCondition.h"
#include "message.h"
#include "sound.h"
#include "BitMacros.h"
/*******************parameter macros**********************/
#define MeterKeyNum						6
#define	VolatageCountNum			10
#define	AdcNum								5
/******funtion macros****************************/
//#pragma CODE_SEG  USER_ROM   
void middleDoorKey(void);
void middleDoorOpenKey(void);
void frontDoorOpenKey(void);
void rGearKey(void); 
void scanKey1(void);
void scanKey2(void);
void scanKey3(void);
void scanKey4(void);
void scanKey5(void);
void scanKey6(void);
void InterfaceSwitchKey(void);


EXT void doOpenShortKey(UINT8 *pStatus,UINT8 len,UINT8 *pShortOpenKey);
EXT void shortOpenKey(void);
EXT void Hc4051_ChannelSelect(unsigned char Num_Hc4051,unsigned char Channel4051);
EXT void AdcValueExtract(void);
EXT void tidyPsychicKeys(SelfKeyStruct *pKey);
void AdcValueChangeToSwitchState(void);



EXT void lowOnlyKey(UINT8 *pAdcValue,UINT8 *pBuf,UINT8 index);
EXT void highonlykey(UINT8 *pAdcValue,UINT8 *pBuf,UINT8 index);
EXT void lowHighKey(UINT8 *pAdcValue,UINT8 *pConfig,UINT8 *pBuf,UINT8 index);
void	AdcResultChangetoRealy(void);
void adcValueToSwitch(STRUCT_KEY_CONFIG  keyConfog ,UINT8 *pAdcValue, UINT8 *pBuf,UINT8 len);
EXT void scanLoadsKey(UINT8 *pKey,UINT8 *pLoadsStatus,UINT8 style,UINT8 cnt);
EXT void doVirtualKey(void);



//#pragma CODE_SEG DEFAULT
  /****************ram macros**************************/
EXT SelfKeyStruct MeterKeyNew[7];
EXT SelfKeyStruct MeterKeyOld[7];
EXT UINT8 CheckVoltageValue[4];
EXT UINT8 CheckVoltageCnt[4];

EXT SelfKeyStruct PwmShortStNew;
EXT SelfKeyStruct PwmShortStOld;
EXT SelfKeyStruct PwmShortSt;
  

EXT UINT8 AdcValue[50];
EXT UINT8 KeyADValueTemp[40];	//开关adc值，排序后
EXT SelfKeyStruct MeterVirtualKey[8];
EXT UINT8 MeterLoadsOpenKey;
EXT UINT8 MeterLoadsOpenOldKey;
EXT UINT8 CarRuning;
EXT UINT16 MotorRuning;
EXT UINT8  BoardTemperatureAdValue;
EXT UINT8  Video_Power_Detect_Time[2];
EXT UINT8  VideoPower1ErrorFlag ,VideoPower2ErrorFlag;

extern UINT8 OddKey;
extern UINT8 OldOddKey;
extern UINT8 OddKeyValue;
extern UINT8 EvenKey;
extern UINT8 OldEvenKey;
extern UINT8 EvenKeyValue;
// extern UINT8 Test_wain;

/*******************************************************/
#define  HIGH_LOW        0x02 // 0b10 
#define  HIGH_ONLY       0x03 // 0b11
#define  LOW_ONLY        0x00 // 0b00
#define  LOAD_STATUS_MASK 0x03

//#define  LOAD_SHORT       0x01
//#define  LOAD_OPEN        0x02
#define  METER_LOAD_LEN   2      //仪表负载2个字节，有8个负载
#define  DRIVER_LOAD_LEN   5

EXT LCD_MSG LcdMsg;
EXT UINT8 Indicator2Status;
#define CLEAR_ALL_KEY             0x0000
#define EXIT_KEY                  0x0001
#define UP_KEY                    0x0002
#define DOWN_KEY                  0x0004
#define ENTER_KEY                 0x0008
#define SILENCE_KEY               0x0010
#define VIDEO_KEY                 0x0020


#define MIDDLE_DOOR_OPEN_KEY      0x0080
#define R_GEAR_KEY                0x0100
#define R_GEAR_OVER_KEY           0x0200
#define MIDDLE_DOOR_OPEN_OVER_KEY 0x0400


#define MIDDLE_DOOR_NOT_CHOSE_KEY 0x0800
#define MIDDLE_DOOR_NOR_KEY       0x1000
#define FRONT_DOOR_NOT_CHOSE_KEY  0x2000
#define FRONT_DOOR_NOR_KEY        0x4000
#define INTERFACE_SWITCH_KEY      0x0040
#define INTERFACE_SWITCH_NOT_KEY  0x8000



#define STATUS_INDEX_MASK         0x1f
	/********************************************************/
	EXT byte	ChannelSetCursor;
/***************按键 macros*************************/


#define	WAITBOTTON 	1

extern unsigned char freeButtonvoltage;


#define MinSW1		0*freeButtonvoltage/255
#define MaxSW1		10*freeButtonvoltage/255
//----------------------
#define MinSW2		79*freeButtonvoltage/255
#define MaxSW2		93*freeButtonvoltage/255
//----------------------
#define MinSW3		121*freeButtonvoltage/255
#define MaxSW3		135*freeButtonvoltage/255
//----------------------
#define MinSW4		146*freeButtonvoltage/255
#define MaxSW4		160*freeButtonvoltage/255
//----------------------
#define MinSW5		173*freeButtonvoltage/255
#define MaxSW5		187*freeButtonvoltage/255
//----------------------
#define MinSW6		200*freeButtonvoltage/255
#define MaxSW6		210*freeButtonvoltage/255
//----------------------

#define SW1			1
#define SW2			2
#define SW3			3
#define SW4			4
#define SW5			5
#define SW6			6





	

	/*******************parameter macros**************/
  #define REFERENCE_VOLTAGE		AdcValue[30]	///when power is 12v,norm voltage is 1.1v                       
  #define ADCCompareLowData		REFERENCE_VOLTAGE/4///low state is 0.7v when pull up,availability is 0.2v         0.275
  #define ADCCompareLowData1	REFERENCE_VOLTAGE/8
  
  #define ADCCompareHighData	REFERENCE_VOLTAGE/6*5 ///high state is 0.7v when pull up,availability is 1.4v     0.916
  #define ADCCompareLow1Data	REFERENCE_VOLTAGE/5*4///high state is 1.4v when pull up,availability is 0.7v      0.88
  #define ADCCompareHigh1Data 	REFERENCE_VOLTAGE/2   //jc12(32) jc14(33) 两路电压输入85
  
  #define CheckShortData    185/3*2


	#define	VOLTAGE_REVISE_NUM			5
	
void keyDeal_dataInit(void);	
void initKeyVariable(void);	
void initHc4051(void);
void closeConsumePin(void);
void initConsumePin(void);
void KeyDeal(void);
void scanKey(void);
void initKeyPort(void);

void PwmShortCheck(void);
void gear1WarnKey(void);
void gear2WarnKey(void);
void V12_PowerDeal(void);
void init_motor_io();
void turnOnConsumePin(void);
void closeConsumePin(void);

#ifdef ZB206B
void doZB206Power(void);
#endif


#endif						


	
