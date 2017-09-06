#ifndef __LCMDISP_H_
#define __LCMDISP_H_

#include "typedef.h"
#include "ISR.H"
#include "xgate.h"

#define  BRAKE_D_VALUE           200 //10km/h    按加速度为5计算。
#define  SPEED_UP_D_VALUE        50  //2.5km/h按加速度为1.4计算。
#define  ENGBRAKE_D_VALUE           100 
#define  ENGSPEED_UP_D_VALUE        25 

/******funtion macros***********************************/
EXT void doDistance(void);
EXT void	SaveOverallDistance(void);
EXT UINT32 calcDistance(UINT32 distance,UINT16 speed);
EXT UINT16 calcSpeed(UINT16 frequency);
EXT void initSpeedBuf(void);
//EXT UINT16 filter(SPEED_DATA_BUF_STRUCT *pSpeedBuf,UINT16 newData);
EXT UINT16 filter(UINT16 newData);
//EXT UINT16 calcRealSpeed(UINT16 *pSpeedPulseCnt);
EXT UINT16 calcRealSpeed(UINT8 channel);
EXT UINT16 calcRealEngSpeed(UINT8 channel);
//EXT void doSpeed(void);
EXT void initV12AndVideoPowerPort(void);
EXT void CountHundredKilometerFuelConsume(void); 

/****************ram macros*****************************/
EXT UINT16 LowSpeed[5];
EXT UINT8 LowSpeedPoint;
EXT UINT8 LowSpeedFlag;
EXT SPEED_DATA_BUF_STRUCT SpeedBuf;

EXT UINT16 LowEngSpeed[5];
EXT UINT8 LowEngSpeedPoint;
EXT UINT8 LowEngSpeedFlag;
EXT SPEED_DATA_BUF_STRUCT EngSpeedBuf;


extern UINT8   speed_count;
extern unsigned int   Capture6TimeBuf[2];
extern UINT8 SpeedDataNum,SpeedData[16];
extern unsigned char	SendSpeedData[1];
extern unsigned 	char	SendSpeedDataLast;
extern unsigned 	char	SendSpeedDataUpdata1Num;
extern unsigned 	char	SendSpeedDataUpdataNum;
EXT unsigned 	char  count_speed;
EXT unsigned  int   TempSpeedData;


//EXT UINT16 CarSpeed;
EXT UINT16 CarSpeedFromDashboard;
EXT UINT16 CarEngSpeedFromDashboard;
//EXT UINT32  TotalDistance;
//EXT UINT32  RelativeDistance;
EXT  unsigned long	CountDistanceSave;
//EXT const UINT8 DashboardSpeedChannel;	

EXT UINT8 DashBoardSpeedChannelTemp;
EXT UINT8 DashBoardEngSpeedChannelTemp;
EXT UINT16 SpeedRateTemp;
EXT UINT8 CycleCaptureNumTemp;

#endif

