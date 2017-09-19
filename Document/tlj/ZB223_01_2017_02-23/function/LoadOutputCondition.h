#ifndef __LOAD_OUTPUT_CONDITION_H__
#define __LOAD_OUTPUT_CONDITION_H__

#include "typedef.h"
#include "load_deal.h"
#include "configFile.h"
#include "common.h"
#include "KeyDeal.h"
#include "WaitTimer.h"
#include "NamedPhysicalKeyValue.h"
#include "keyProcess.h"
//#include "TW8816.H"

#define NUMBER_OF_OUTPUT_CHANNELS				160//(Num_PowerDevice+NUM_CAT4016*2)*8+NUM_MAIN_MANU_LED_OUTPUT_CHANNELS+NUM_BUZZER_CHANNELS+ 30
#define CONDITION_MAX_CONDITION_UNITS			23//是一个逻辑表达式中最小项个数的最大值,用两个字节存储一个最小项

/**
NOTE: 
 
1.Driver Module:
  in the orginal app: (see PostProcess) 
   the hold on after key off is applied on
    DLoad15(when FrontDriveFlag==1)(5000ms)-FrontDoorOpen
    DLoad2 (BackDriveFlag==1)(10000ms)- 
2. 
 
 
*/
//#pragma CONST_SEG __GPAGE_SEG CONFIG_DATA_SDO
//extern  UINT16 ROMCONST  LoadOutputConditions[NUMBER_OF_OUTPUT_CHANNELS][CONDITION_MAX_CONDITION_UNITS+1+2];
void loadTovirtualKey(void);
//#pragma CONST_SEG    DEFAULT
//extern PutLoadStruct PutDataa[NUMBER_OF_OUTPUT_CHANNELS/8];//when NUMBER_OF_OUTPUT_CHANNELS%8!=0;we should +1
//#define OutPutChannelData  ((UINT8 *)OutPutData)
BOOLEAN IsConditionTrue( UINT16  ROMCONST GENPTR*GENPTR_B pCondition,UINT8 startIndex, UINT8 maxCount);
void ProcessLoadOutput(UINT8 *conditionData,UINT8 *outPutData,UINT8 outPutConditionArrayIndex_FirstDimension,UINT8 channelsCount );


#endif
