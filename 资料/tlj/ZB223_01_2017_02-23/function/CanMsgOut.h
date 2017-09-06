#ifndef __CANMSGOUTNUM_H_
#define __CANMSGOUTNUM_H_

#include	"typedef.h"
#include "configFile.h"
#include "common.h"
#include "J1939.h"


#define CanMsgOutNum 20


extern void CanMsgOutInit(void);
void doJ1939Translate(void);


extern Time_Info Time[CanMsgOutNum];

#endif

