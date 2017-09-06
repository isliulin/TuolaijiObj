#ifndef	__MESSAGE_H_
#define __MESSAGE_H_

#include	"typedef.h"
#include "BitMacros.h"
#include "configFile.h"
#include "J1939.h"
#include "sensor.h"
#include	"sound.h"

/***********parameter macros****************/

/******funtion macros****************************/
//#pragma CODE_SEG  USER_ROM
extern const  UINT8 Version[12];


EXT void ClearInternalWatchDog(void);
EXT void ClearExternalWatchDog(void);
//EXT void FeedWatchDog(void);

EXT void SpeedPwmOut(void);
EXT void setData(UINT8 *pData,UINT8 cnt);

EXT void initModuleDetectParameter(void);



EXT void doLoadStatusWarnToStack(UINT16 base,UINT8 *pOldStatus,UINT8 *pNewStatus,UINT8 cnt);













EXT void sensorFault(UINT8 base,UINT16 faultNum,UINT8 cnt);


EXT void initSensorData(void);



//#pragma CODE_SEG DEFAULT
//------------------------------------------------------------------------
#define HAVENO_AIR_PRESSURE          0x40 
#define AIR_PRESSURE_FROM_J1939      0x80
#define HAVENO_OIL_MASS              0x40
#define OIL_MASS_FROM_J1939          0x80
#define HAVENO_OIL_PRESSURE          0x40
#define OIL_PRESSURE_FROM_J1939      0x80
#define HAVENO_ROTATION_RATE         0x40
#define ROTATION_RATE_FROM_J1939     0x80
#define CAR_SPEED_FROM_J1939         0x80

#define FRONT_AIR_PRESSURE           0x00
#define BACK_AIR_PRESSURE            0x01


#define MODULE_DETECT_TIME_OV_NUM    15 //50ms检测一次，因此如果模块检测单元超过4表示2秒内还没有被清除，为模块丢失。
#define DMI_DETECT_TIME_OV_NUM       100

#define WatchDogResetParameter1				    	0x55
#define WatchDogResetParameter2				    	0xaa

#define FAULT_CODE_BASE_AVAID               0xff
#define SLEEP_DELAY_TIME                    3000
#define SLEEP_CODE                          0x55
#define WAKE_UP_CODE                        0xaa
/*******************ram macros*****************************/
EXT const UINT8*  ModuleTemperaturePoint[5];
EXT UINT8 FrontModuleTemperature;
EXT UINT8 BackModuleTemperature;
EXT UINT8 MiddleModuleTemperature;
EXT UINT8 TopModuleTemperature;
EXT UINT8 PowerModuleTemperature;
EXT UINT8 FrontAirPressureJ1939;
EXT UINT8 BackAirPressureJ1939;
EXT UINT8 OilMassJ1939;
EXT UINT8 OilPressureJ1939;
EXT UINT8 PwmLoadOutType;

EXT UINT16 RotationRateJ1939;
EXT UINT8 WaterTempJ1939;
EXT UINT32 TotalConsume;
EXT UINT16 FuelRate;

EXT UINT16 Carbamide;
EXT UINT16 SleepDelayCnt;

EXT	ulong 	VehicleSpeedData;

EXT UINT8 OldDashboardLoadStatusData[2];
EXT UINT8 OldFrontLoadStatusData[5];
EXT UINT8 OldBackLoadStatusData[5];
EXT UINT8 OldMiddleLoadStatusData[5];
EXT UINT8 OldTopLoadStatusData[5];
EXT UINT8 OldPowerStatusData[5];
EXT UINT8 OldPowerFuseStatusData[5];
EXT UINT8 OldPowerContorolLoadState[5];

EXT UINT16 ModuleConfigTemp;
//EXT UINT8 OldModuleErrorFlag;
EXT UINT8 ClrSleepCnt;
EXT UINT16 LoadWarnCnt;


EXT	uchar 	FirstErrorOld[FIRST_ERROR_NUM];
EXT	uchar 	FirstError[FIRST_ERROR_NUM];


EXT	UINT16 	dis_count;//故障总个数
#define	MulfunctionTotalNum								143
EXT	UINT16 	dis_content[MulfunctionTotalNum];
extern UINT16 SleepDelayCnt;


/*******************parameter macros**************/
#define	PwmPeriodNum	1
/*******************send kind macros**************/

/*********************IO macros****************************/	



//函数声明
void doFaultWarning(void);
void doModuleErrorToStack(void);
void push_dis(UINT16 tmp);
void pop_dis(UINT16 tmp);
void moduleLost(void);
void frontDriveLost(void);

void backDriveLost(void);
void middleDriveLost(void);
void topDriveLost(void);

void clearIntData(UINT16 *pData,UINT8 cnt);
void clearData(UINT8 *pData,UINT8 cnt);
void powerDriveLost(void);
void powerContorolDriveLost(void);
void j1939Lost(void);
void ModuleFault(void);
void ExLoadStatusWarnToStack(UINT8 *pOldStatus,UINT8 *pNewStatus,const UINT8 * pWarnByte,UINT8 cnt);
void ExSensorFault(UINT8 base,const UINT8 * pWarnByte,UINT8 cnt);
void ModuleDetect(void);
void  MesseageDeal(void);
void PowerDeal(void);
void WakeUpInit(void);
void CanSendDeal(void);
void sleep_mode(void);
void LoadStatusCopy(void) ;
void DisLoadWarnShow(void);
void SendCan0Message(void);


#endif
