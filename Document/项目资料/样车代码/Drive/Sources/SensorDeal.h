#ifndef	_sensortable_h
#define		_sensortable_h

#include 		"hidef.h" 		/* for EnableInterrupts macro */
#include 		"derivative.h" /* include peripheral declarations */
#include		"typedef.h"
#include		"common.h"
#include		"DriveKeyDeal.h"

#ifndef	_NO_SensorDeal_EXTERN
#define	_SensorDeal_EXTERN	extern
#else
#define	_SensorDeal_EXTERN
#endif
/******************funtion macros********************/
void	SensorDataDeal(void);
/**************ram macros*********************************/
///车速采集相关的变量
_SensorDeal_EXTERN	unsigned	char	Time_Capture_Zero;  
_SensorDeal_EXTERN	unsigned 	char	SendSpeedData[1];
_SensorDeal_EXTERN	unsigned 	char	SpeedDataNum;
_SensorDeal_EXTERN	unsigned 	char	SpeedData[16];
_SensorDeal_EXTERN	unsigned 	int	Capture_Timer_Num[2];
_SensorDeal_EXTERN	unsigned 	char	SendSpeedDataLast;
_SensorDeal_EXTERN	unsigned 	char	SendSpeedDataUpdata1Num;
_SensorDeal_EXTERN	unsigned 	char	SendSpeedDataUpdataNum;
_SensorDeal_EXTERN unsigned	int    Speed_Parameter;			
_SensorDeal_EXTERN unsigned 	char Speed_Pulse_Number;		

_SensorDeal_EXTERN	unsigned 	char 	SensorData[7];
_SensorDeal_EXTERN	unsigned	char	SendSensorData[6];
_SensorDeal_EXTERN	unsigned 	char	FuelLevelNum,FuelLevel_Num[2],Count_FuelLevel_Num;
_SensorDeal_EXTERN	unsigned	char	OilPressureNum,OilPressure_Num[2],Count_OilPressure_Num;
_SensorDeal_EXTERN	unsigned	char	GasPress1Num,GasPress1_Num[2],Count_GasPress1_Num;
_SensorDeal_EXTERN	unsigned	char	GasPress2Num,GasPress2_Num[2],Count_GasPress2_Num;
_SensorDeal_EXTERN	unsigned	char	TemperatureNum,Temperature_Num[2],Count_Temperature_Num;
_SensorDeal_EXTERN	unsigned	char	EngineRoomTempNum,EngineRoomTemp_Num[2],Count_EngineRoomTemp_Num;
_SensorDeal_EXTERN	unsigned	char	EngineWaterTempNum,EngineWaterTemp_Num[2],Count_EngineWaterTemp_Num;
///转速采集相关的变量
_SensorDeal_EXTERN	unsigned 	int		Rotation_Capture_Timer_Num[2];
_SensorDeal_EXTERN	unsigned	char	Time_Rotation_Zero;
_SensorDeal_EXTERN	unsigned 	char	RotationDataNum;
_SensorDeal_EXTERN	unsigned 	int	CaptureRotationData[16];
_SensorDeal_EXTERN	unsigned 	int	SendRotationData[1];
///传感器对应的AD值表
#ifdef	_NO_SensorDeal_EXTERN
const	unsigned	char	FuelLevel_Table[101]=	{
																			0,17,20,23,26,29,32,35,38,41,44,
																			47,50,53,56,59,61,63,65,67,69,
																			71,73,75,77,79,81,83,85,88,91,
																			94,94,95,96,96,97,98,98,99,100,
																			100,100,100,102,102,102,104,104,104,106,
																			106,108,110,111,113,115,117,119,120,120,
																			122,122,123,124,124,125,126,126,126,127,
																			127,127,129,129,131,131,133,133,135,135,
																			136,136,136,138,138,138,140,140,140,142,
																			142,142,144,144,144,145,145,145,147,160																		
															 				};
const	unsigned	char	OilPressure_Table2[46]={
																			118,113,107,103, 98, 94, 88, 82, 75, 72,
 																			70, 69, 67, 66, 64, 63, 61, 60, 58, 57,                                  
 																			55, 54, 51, 50, 49, 48, 47, 46, 45, 44,
 																			43, 42, 41, 40, 39, 38, 37, 36, 35, 33,
 																			32, 29, 25, 19, 10, 0
																			};	
const	unsigned	char	OilPressure_Table1[46]={0,15,19,23,27,31,35,39,43,46,48,				///realy oil press parameter
																			51,67,70,73,76,79,82,85,88,92,                          
																			96,98,100,102,104,106,108,110,112,114,
																			115,116,118,120,122,124,126,128,130,132,                 
																			134,136,138,140,142
																			};
const	unsigned	char	GasPressure_Table[101]={0,17,20,23,26,29,32,35,38,41,44,			///client gas press parameter
																				47,49,51,53,55,57,60,62,64,65,
																				67,69,71,73,75,78,81,84,87,90,
																				94,94,94,94,94,95,95,95,95,95,
																				96,97,98,99,100,101,103,104,105,107,
																				108,109,110,111,112,113,114,115,116,117,
																				118,119,120,121,122,123,123,124,125,126,
																				127,127,128,128,129,129,130,131,132,133,
																				134,134,135,135,136,136,137,138,139,140,
																				141,141,142,142,143,144,145,146,147,160
																				};	
const	unsigned	char	EngineRoom_Table[81]={227,227,225,225,223,223,221,221,219,219,			
																				218,218,218,216,216,216,216,214,214,214,
																				212,210,208,208,206,206,204,204,202,202,
																				199,199,196,196,194,194,192,190,188,188,
																				186,185,184,183,181,179,177,175,173,172,
																				170,169,168,167,165,164,163,162,161,159,
																				157,155,152,150,148,147,147,145,145,143,
																				141,139,137,135,133,131,129,127,126,125,0
																				};

#else
extern	const	unsigned	char	FuelLevel_Table[101];
extern	const	unsigned	char	OilPressure_Table1[46];
extern	const	unsigned	char	OilPressure_Table2[46];
extern	const	unsigned	char	GasPressure_Table[100];
#endif																											   
/*********************parameter macros*******************/			
///fuel level sensor macros
#define		SensorErrorNum			0xfe
#define		SensorOpenNum			204				///realy power is 4v
///vehicle speed macros
#define		Capture_Time				125				///100us count one

#define		SPEED_CAPTURE_SEC1		10
#define		SPEED_CAPTURE_SEC2		15
#define		SPEED_CAPTURE_SEC3		20
#define		SPEED_CAPTURE_SEC4		30
#define		SPEED_CAPTURE_SEC5		50
#define		SPEED_CAPTURE_SEC6		70
#define		SPEED_CAPTURE_SEC7		90
#define		SPEED_CAPTURE_SEC8		110
#define		SPEED_CAPTURE_SEC9		140

#define		SEC1_CAPTURE_NUM			1
#define		SEC2_CAPTURE_NUM			3
#define		SEC3_CAPTURE_NUM			4
#define		SEC4_CAPTURE_NUM			6
#define		SEC5_CAPTURE_NUM			8
#define		SEC6_CAPTURE_NUM			10
#define		SEC7_CAPTURE_NUM			12
#define		SEC8_CAPTURE_NUM			14
#define		SEC9_CAPTURE_NUM			16
///rotation macros
#define	 Rotation_Capture_Time		16*2		///分频后为16us进入一次捕获中断；因为是两次的中断差值，所以要*2

#define		ROTATION_CAPTURE_SEC1		200
#define		ROTATION_CAPTURE_SEC2		400
#define		ROTATION_CAPTURE_SEC3		600
#define		ROTATION_CAPTURE_SEC4		30
#define		ROTATION_CAPTURE_SEC5		50
#define		ROTATION_CAPTURE_SEC6		70
#define		ROTATION_CAPTURE_SEC7		90
#define		ROTATION_CAPTURE_SEC8		110
#define		ROTATION_CAPTURE_SEC9		140

#define		SEC1_ROTATION_NUM			1
#define		SEC2_ROTATION_NUM			3
#define		SEC3_ROTATION_NUM			4
#define		SEC4_ROTATION_NUM			6
#define		SEC5_ROTATION_NUM			8
#define		SEC6_ROTATION_NUM			10
#define		SEC7_ROTATION_NUM			12
#define		SEC8_ROTATION_NUM			14
#define		SEC9_ROTATION_NUM			16

#endif

