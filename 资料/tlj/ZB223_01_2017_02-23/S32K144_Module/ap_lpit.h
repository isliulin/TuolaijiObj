#ifndef __AP_LPIT_H_
#define __AP_LPIT_H_
#include "S32K144.h"
#include "arm_cm4.h"

// ���ֵ4294967295
#define CH0TIMER	(1000)*80		//ͨ��0��ʱ 1000us  
#define CH1TIMER	1000*80
#define CH2TIMER	1000*80
#define CH3TIMER	5120*80

void lpit0Init(void);
#endif