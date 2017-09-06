#ifndef __AP_S32K1XX_FTM_H
#define __AP_S32K1XX_FTM_H

#include "S32K144.h"
#include "Sysinit.h"
#define ABS(a)         (((a) < 0) ? (-(a)) : (a))

//Clock Source Selection
#define FTM_CS_NC		0	//No clock selected. This in effect disables the FTM counter
#define FTM_CS_Sys		1	//
#define FTM_CS_Fix		2	//
#define FTM_CS_Ext		3	//#define FTM_CS_SYS		1	//

//Prescale Factor Selection
#define DIV1			0	//Divide by 1
#define DIV2			2	//Divide by 2
#define DIV4			4	//Divide by 4
#define DIV8			8	//Divide by 8
#define DIV16			16	//Divide by 16
#define DIV32			32	//Divide by 32
#define DIV64			64	//Divide by 64
#define DIV128			128	//Divide by 128

#define ALIGN_LEFT  (FTM_C0SC_MSB_MASK|FTM_C0SC_ELSB_MASK)    //PWM左对齐
#define ALIGN_RIGHT (FTM_C0SC_MSB_MASK|FTM_C0SC_ELSA_MASK)    //PWM左对齐


typedef enum FTMn
{
    ftm0,
    ftm1,
    ftm2,
    ftm3
} FTMn;

typedef enum CHn
{
    //   --FTM0--  --FTM1--  --FTM2--
    CH0,   //     PTC1      PTA8      PTA10
    CH1,   //     PTC2      PTA9      PTA11
    CH2,   //     PTC3       ×         ×
    CH3,   //     PTC4       ×         ×
    CH4,   //     PTD4       ×         ×
    CH5,   //     PTD5       ×         ×
    CH6,   //     PTD6       ×         ×
    CH7    //     PTD7       ×         ×
    // ×表示不存在
} CHn;


#define FTM_PRECISON 100u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/FTM_PRECISON

void FTM_PWM_init(FTMn ftmn, CHn ch, uint32_t freq, uint32_t duty);
uint32_t FTM_PWM_Duty(uint32_t freq,uint32_t duty);
void FTM_PWM_freq(FTMn ftmn, uint32_t freq) ; 


uint8_t LPLD_FTM_IC_Init(FTM_Type *ftmx ,uint8_t ps);
uint8_t LPLD_FTM_IC_Enable(FTM_Type *ftmx, uint8_t chn, uint8_t capture_edge);



#endif
