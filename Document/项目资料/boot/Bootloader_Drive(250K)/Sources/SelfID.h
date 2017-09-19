#ifndef	_SelfID_H
#define		_SelfID_H

#include 	"hidef.h" 		/* for EnableInterrupts macro */
#include 	"derivative.h" /* include peripheral declarations */
#include	"Macros.h"

#ifndef	_NO_SelfID_EXTERN
#define		_SelfID_EXTERN	extern
#else
#define		_SelfID_EXTERN
#endif
/*************funtion macros**************/
void TestSelfID(void);
static void	Hc4051_ChannelSelect(unsigned char Num_Hc4051,unsigned char Channel4051);
static void	ADC_ChannelSelect(unsigned	char	AdcChannel);
void	ADC_Init(void);
/****************global variable macros*********************/
_SelfID_EXTERN	unsigned char	ADresult;
_SelfID_EXTERN	unsigned	char	Hc4051ChannelAdcData[12],NormAdcData;
_SelfID_EXTERN	SelfIDStruct	SelfID,TempSelfID;
/*******************ad sampling pin*************************/
////use hc4051 adc pin
#define	AN_MUX_A1				PTFD_PTFD0
#define	AN_MUX_B1				PTFD_PTFD1
#define	AN_MUX_C1				PTFD_PTFD2

#define	AN_MUX_A2				PTFD_PTFD3
#define	AN_MUX_B2				PTGD_PTGD4
#define	AN_MUX_C2				PTGD_PTGD5

#define	AN_MUX_A3				PTCD_PTCD3
#define	AN_MUX_B3				PTBD_PTBD3
#define	AN_MUX_C3				PTAD_PTAD3

#define	AN_MUX_A1_IO		PTFDD_PTFDD0
#define	AN_MUX_B1_IO		PTFDD_PTFDD1
#define	AN_MUX_C1_IO		PTFDD_PTFDD2

#define	AN_MUX_A2_IO		PTFDD_PTFDD3
#define	AN_MUX_B2_IO		PTGDD_PTGDD4
#define	AN_MUX_C2_IO		PTGDD_PTGDD5

#define	AN_MUX_A3_IO		PTCDD_PTCDD3
#define	AN_MUX_B3_IO		PTBDD_PTBDD3
#define	AN_MUX_C3_IO		PTADD_PTADD3

#define	Mcu_U24				0
#define	Mcu_U26				1
#define	Mcu_U13				2
/***********************io macros***************************/
#define		V_Power_IO			PTBDD_PTBDD1
#define		V_Power					PTBD_PTBD1
/***************parameter macros*********************/
#define	LowKey				NormAdcData/10*3		///when power is 26v,norm adc data is 117 and norm power is 10v;when low,the data is 54 and norm power is 5v;	
#define	HighKey 			NormAdcData/6*5	///when high,the data is 176 and norm power is 15v
#define	ADDR_AD_NUM		2
#define	NORM_AD_NUM		1
#endif


