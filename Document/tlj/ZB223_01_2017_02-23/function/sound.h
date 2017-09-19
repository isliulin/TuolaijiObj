#ifndef __SOUND_H_
#define __SOUND_H_


#include	"typedef.h"
#include	"load_deal.h"

//#pragma CODE_SEG DEFAULT
//#pragma CODE_SEG  USER_ROM





//#pragma CODE_SEG DEFAULT
EXT UINT8 DashboardLoadsSound[2];
EXT UINT8 OldDashboardLoadsSound;
EXT UINT8 FrontDriveLoadsSound[3];
EXT UINT8 OldFrontDriveLoadsSound[3];
EXT UINT8 BackDriveLoadsSound[3];
EXT UINT8 OldBackDriveLoadsSound[3];
EXT UINT8 MiddleDriveLoadsSound[3];
EXT UINT8 OldMiddleDriveLoadsSound[3];
EXT UINT8 TopDriveLoadsSound[3];
EXT UINT8 OldTopDriveLoadsSound[3];
EXT UINT8 PowerLoadsSound[3];
EXT UINT8 PowerContorolLoadSound[3];
EXT UINT8 OldPowerLoadsSound[3];
EXT UINT8 PowerLoadsFuse[3];
EXT UINT8 J1939FaultSoundWarn;

EXT UINT16 Cat4016DataOld;
EXT UINT8  Gear1Warn[20];
EXT UINT8  OldGear1Warn[20];
EXT UINT8  ComGear1Warn[20];
EXT UINT8  Gear2Warn[20];
EXT UINT8  OldGear2Warn[20];
EXT UINT8  ComGear2Warn[20];
EXT UINT8  Gear3Warn[20];
EXT UINT8  OldGear3Warn[20];
EXT UINT8  ComGear3Warn[20];
EXT UINT8  Gear4Warn[20];
EXT UINT8  OldGear4Warn[20];
EXT UINT8  ComGear4Warn[20];

EXT UINT8  SensorFaultSound[6];
EXT UINT8  OldSensorFaultSound[6];
EXT UINT8  ComSensorFaultSound[6];

EXT UINT8 IndicatorLed[20];
EXT UINT8 IndicatorOldLed[20];
EXT UINT8  ComModuleErrorFlagWarn;
EXT UINT8  OldModuleErrorFlagWarn;

EXT UINT8 Gear2SondCnt;
EXT UINT8 Gear3SondCnt;
//EXT UINT8 LcdWarm;
//EXT UINT8 OldLcdWarm;

void getIcoWarn(void);
void sound(void);
void loadOpenOrShortSoundWarn(UINT8 *pLoadStatus,UINT8 *pSoundWarn,UINT8 len);
void sensorFaultSound(void);
UINT8 sound_byte_compare(UINT8 *newSt,UINT8 *oldSt,UINT8 len);
void  CopyDataSound(UINT8 *pScr,UINT8 *pDst,UINT8 cnt) ;
UINT8 do1GearSound(void);
UINT8 do2GearSound(void);
UINT8 do3GearSound(void);
UINT8 do4GearSound(void);
void renewSound(void);
void BuzzerOut(UINT8 mode,UINT8 on_flag);


#endif

