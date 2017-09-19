#ifndef __XGATE_H_
#define __XGATE_H_

#include "S32K144.h"
#include "Typedef.h"
#include "StepDeal.h"
#include <stdlib.h>
#include "ap_S32K1xx_FTM.h"

//extern uint32_t g_tachoMeterTimer,g_carSpeedTimer;
EXT UINT16 PulseCnt[2];
/*******step parameter macros******/
#define		S0_DTC				0x80
#define		S1_DTC				0x80
#define CLOCKWISE       0
#define CNTCLOCKWISE    1  

#define     STEP01              4
#define     STEP1               8
#define     STEP2               12
#define     STEP23              16
#define     STEP3               20
#define     STEP34              24

#define     STEP0_4              4 // 2
#define     STEP5_8              8//6
#define     STEP9_18             18//16
#define     STEP19               19//17
#define     STEP20_23            23//21

#define		CAR_SPEED_1KM_STEP_NUM	20

void init_motor(void);
void motor_update(uchar channel, uchar dir);
void motor_micro_update(uchar channel, uchar dir);
void motorWorking(void);
#endif