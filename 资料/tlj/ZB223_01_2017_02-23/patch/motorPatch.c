/*
* L
*/
#include "motorPatch.h"
#include	"StepDeal.h"

int speedStepRefreshCnt = 1200;

#if 0
UINT16 carSpeedFilter(UINT16 step){
	
	UINT16 diff;
	UINT16 origin = MotorCurStep[Carspeed];
	UINT16 target;

	if(speedStepRefreshCnt > 0){
		if(step > origin) {
			diff = step - origin;
		}
		else{ 
			diff = origin - step;
		}

		if(diff < CARSPEED_PERGRID_STEP){
			//过滤6度 5公里
			target = MotorFinalStep[Carspeed];
		}
		else{
			target = step;
		}

	}else{
		speedStepRefreshCnt = 1200;
		target = step;
	}
	

	return target;
}

UINT16 carSpeedFilter(UINT16 step){
	static signed char lastDir = 0;
	static signed char currentDir = 0;
	static UINT16 lastStep = 0;
	UINT16 targetStep;
	
	if(step > lastStep){
		if(currentDir < 100)currentDir++;
	}else if(step < lastStep){
		if(currentDir > -100)currentDir--;
	} else return step;

	if(currentDir > lastDir){
		
	}
}


#endif
unsigned int getCarSpeedTimes(unsigned int newTarget){
	int proportion,derivative;
	int absV;
	static int lastProportion = 0;
	proportion = newTarget - MotorCurStep[Carspeed];
	derivative = proportion - lastProportion;
	lastProportion = proportion;

	absV = proportion/5 ; //10*derivative;
	if(absV < 0) absV = -absV;
	absV += derivative;
	if(absV < 0) absV = -absV;
	return (unsigned int)absV;
}

