#ifndef __SENSOR_H_
#define __SENSOR_H_

#include "typedef.h"
#include "configFile.h"
#include "ISR.H"
#include "J1939.h"
#include	"physicalq.h"



extern  UINT16 InsideData[END_INSIDE_OFFSET1]; //仪表5个传感器 + 前驱6个传感器 + 后驱6个传感器 + 中驱6个传感器 + 顶驱6个传感器 + 电池电压 + 6个板温+车速




//#pragma CODE_SEG DEFAULT

#define  CARSPEED          RealCarSpeed//InsideData[7]   


#define  SENSOR_VIRTUAL_KEY                  0x80     
#define  SENSOR_WARM_MASK                    0x70
#define  SENSOR_LOWER_WARM                   0x00
#define  SENSOR_MIDDLE_WARM                  0x40
#define  SENSOR_BILATERAL_WARM               0x20
#define  SENSOR_BIGGER_WARM                  0x60
#define  SCHMITT_TRIGGER_LOW                 0x30
#define  SCHMITT_TRIGGER_HIGH                0x10

#define  SENSOR_VALID                        0x80
#define  SENSOR_BIT_VALUE_MASK               0x0FFF 
#define  SENSOR_DOT_VALUE_MASK               0xF000 
#define  SENSOR_DOT_GRESSION                 12



#define BATTERY_DATA_NUM   6
#define INSIDE_SENSOR_NUM  6
EXT UINT16 BatteryDataRoundBuf[BATTERY_DATA_NUM];
EXT UINT8  BatteryDataIndex;
EXT UINT16 BatteryValtage;

EXT UINT8   SensorAdcValue[10];
EXT UINT8   PwmAdcValue[2];
EXT UINT16  RealCarSpeed;
#define DASHBOARD_TEMP_DATA_NUM   6

EXT UINT16 DashBoardTempRoundBuf[BATTERY_DATA_NUM];
EXT UINT8  DashBoardTempIndex;
EXT UINT16 DashBoardTemp;

EXT UINT16    OldSensorData[INSIDE_16BIT_DATA_NUM];

void doSensor(void);
UINT16 adTophysical(UINT8 adValue,const INSIDE_16BIT_ATTR_STRUCT * pSegment);
UINT16 doDashBoardTemp(UINT16 dashTempAdValue);
UINT16 temperatureScaleChange(UINT16 temperatureAdValue);
UINT8 saveUINT16DataToRoundBuf(UINT16 *pRoundBuf,UINT16 data,UINT8 index,UINT8 size);
UINT16 averageUINT16DataWithoutMaxAndMin(UINT16 *pRoundBuf,UINT8 size);
UINT16 calcVolatage(UINT16 volatageAdValue);
UINT16 adToRes(UINT16 res,UINT16 ad);
UINT16 adToVoltage(UINT16 Ratio,UINT16 ad);
UINT16 calcPhysicalDirectRatioRes(UINT16 r1,UINT16 y1,UINT16 r2,UINT16 y2,UINT16 ad);
void getInsideVirtualKey(SelfKeyStruct *virtualKey);

#endif
