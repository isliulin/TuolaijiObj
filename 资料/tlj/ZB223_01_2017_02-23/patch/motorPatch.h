#ifndef __MOTORPATCH_
#define __MOTORPATCH_
#include "Typedef.h"

#define CARSPEED_PERGRID		8 //����ÿС��8��
#define CARSPEED_PERGRID_STEP   CARSPEED_PERGRID * 12 //ÿ���ߵĲ�����ÿ��1/12��

//UINT16 carSpeedFilter(UINT16 step);
unsigned int getCarSpeedTimes(unsigned int newTarget);

#endif
