#ifndef __DEEPSLEEP_H_
#define __DEEPSLEEP_H_
#include "S32K144.h"
#include "HwMacroPatch.h"
#include "Typedef.h"

#define DO_DEEPSLEEP()	MCU_5VEN_L


extern PutLoadStruct LogicOutPut[20];
#define DEEP_SLEEP_LOAD		LogicOutPut[6].Loads.Load7

void dodeepSleep(unsigned char dodeepSleepflag);

#endif
