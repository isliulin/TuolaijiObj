#include "deepSleep.h"

void dodeepSleep(unsigned char dodeepSleepflag){
	static int cnt = 0;
	if(dodeepSleepflag){
		cnt++;

		if(cnt == 2){
			DO_DEEPSLEEP();
		}
	}
	
}
