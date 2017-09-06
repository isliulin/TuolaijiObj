#ifndef __STEPDEAL_H_
#define __STEPDEAL_H_
#include	"Typedef.h"
#include 	"configFile.h"
#include	"BitMacros.h"
#include 	"load_deal.h"
#include    "sensor.h"
#include 	"J1939.h"
#define	MOTOR_NUM		6

extern UINT16 InsideData[160];
extern /*volatile*/  uint  	MotorMaxStep[MOTOR_NUM],MotorMinStep[MOTOR_NUM];

/*
extern ByteTo1bit meter;
#define TACHO           meter.bits.bit5         //转速
#define bCARSPEED        meter.bits.bit4         //车速
*/
/***************parameter macros********************/
#define NO_1_MOTOR      0
#define NO_2_MOTOR      1
#define NO_3_MOTOR      2
#define NO_4_MOTOR      3
#define NO_5_MOTOR      4
#define NO_6_MOTOR      5

/*******step number macros******/
#define PRES            		1//0      			 
#define TEMP            		2 /*1 */      		
#define Oilplant        		3///2      			
#define TACHOMETER      		5///3       
#define Carspeed			4///4
#define PRES2            		0///5   





#define OK							1
#define STEPHIG					2
#define STEPLOW 				3
#define STEPNOT					4

#define TACHOMAXSTEP    2550//11520  ///rotation step is 0~245  (0-35krpm/min)
#define PRESMAXSTEP     10//4400   ///air press 1 step is 0~90  (0-1MPA) 
#define OILMAXSTEP      8//4400   ///oil step is 0~90   (0-1)
#define TEMPMAXSTEP     8//4400   ///water temperature step is 0~90   (40-120°))
#define CARSPEEDMAXSTEP 2550//11760  ///speed step is 0~245  (0-140km/min)
#define PRES2MAXSTEP    10//4400   ///air press 2 step is 0~90 (0-1MPA) 
#define	MotorBackDelayNum	600
#define	VOLTAGEMAXSTEP	3000

#define MAX_NUM         10
#define  EFFECT_START  3
#define  EFFECT_CNT    4
#define  GEAR_0        0
#define  GEAR_1        1
#define  GEAR_2        2
#define  GEAR_3        3
#define  STEP_INSIDE_16BIT_TYPE    0x01
#define  STEP_J1939_TYPE           0x02
/******************funtion macros*************************/
//#pragma CODE_SEG  USER_ROM
//EXT void collectMotorsPhysicalPrm(void);
UINT16 physicalToStep(UINT8 motorNo,UINT32 physical,UINT32 expend);
EXT void doMotor(UINT8 motorNo);
EXT void	StepDeal(void);
EXT unsigned char check_pos(unsigned char channel,unsigned int position);
EXT void	motor_move(unsigned char channel,unsigned int position);
EXT void back0_motor(void) ;
EXT UINT16 filterGear3(UINT8 motorNo,UINT16 newData,UINT16 oldData);
EXT uint adjustNewData(uint oldData,uint newData);
EXT uint average(uint *pData, uchar cnt);
EXT void sort(uint *pData, uchar cnt);
EXT void moveBackAData(UINT16 *pData, UINT8 cnt);
EXT void saveAData(UINT16 *pData,UINT8 cnt,UINT16 data);
EXT void dataCopyUint(UINT16 *pDst, UINT16 *pScr,UINT8 cnt);
EXT void motorWarming(UINT8 type,UINT8 index,UINT16 data,UINT8 motorNo);
EXT UINT32 getDisParameter(UINT8 type,UINT16 index, UINT8 *rDot);
EXT void back0_motor_off(void);
EXT void motorPost(void);
/*****************Ram macros******************/
EXT	volatile	uchar		carspeed;
EXT uint   	delay1s;
EXT	volatile 	uchar 	resetback0;            
EXT	/*volatile*/  uint  	MotorFinalStep[MOTOR_NUM],MotorCurStep[MOTOR_NUM];
EXT	/*volatile*/  uint  	MotorMaxStep[MOTOR_NUM],MotorMinStep[MOTOR_NUM];
EXT volatile  uchar   pres_value;
EXT volatile  uchar  	gaspress2;
//EXT uint StepDataBuf[6][MAX_NUM];
EXT StructFilter StepDataBuf[6];
//EXT uint StepData[6];
#endif



