#ifndef __PHYSICAL_H_
#define __PHYSICAL_H_

#include "typedef.h"
#include "common.h"
#include "configFile.h"
#include "message.h"
#include "LcmDisp.h"
#include "configFile.h"




//#pragma CODE_SEG  USER_ROM
extern void insidePhys32Bit(void);
EXT void insidePhysStatus(void);
EXT void insidePhys(void);

EXT UINT16 getCarEngSpeedChannel(UINT8 eng);
EXT void getVersion(void);
EXT void ClearVersion(void);
EXT UINT8 CompareData(UINT8 *src,UINT8 *dst,UINT8 lenght);
EXT void SetDashboardVersion(void); 


extern UINT8 InsidePhysStatus[8];
extern  INSIDE_PHYS_32BIT_STRUCT InsidePhys32BitAttr[4];
extern  UINT32 InsidePhys32Bit[4];
extern  UINT8 InsideAscii[10][11];
UINT8  getCarSpeedChannel(UINT8 src);
#endif
