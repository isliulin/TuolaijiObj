#ifndef __MOTORPATCH_
#define __MOTORPATCH_
#include "Typedef.h"

#define CARSPEED_PERGRID		8 //车速每小格8°
#define CARSPEED_PERGRID_STEP   CARSPEED_PERGRID * 12 //每格走的步数，每步1/12°

//UINT16 carSpeedFilter(UINT16 step);
unsigned int getCarSpeedTimes(unsigned int newTarget);

#endif
