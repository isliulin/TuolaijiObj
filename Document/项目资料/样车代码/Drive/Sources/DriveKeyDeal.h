#ifndef	_DriveKeyDeal_h
#define		_DriveKeyDeal_h

#include 	"hidef.h" 		/* for EnableInterrupts macro */
#include 	"derivative.h" /* include peripheral declarations */
#include	"typedef.h"
#include	"common.h"
#include	"LoadDeal.h"
#include	"SensorDeal.h"
#include	"BitMacros.h"

#ifndef	_NO_DriveKeyDeal_EXTERN
#define		_DriveKeyDeal_EXTERN	extern
#else
#define		_DriveKeyDeal_EXTERN
#endif
/************funtion macros****************/
void	Hc4051_ChannelSelect(unsigned char Num_Hc4051,unsigned char Channel4051);

void	ADC_ChannelSelect(unsigned	char	AdcChannel);

static void KeyChangeProcess(void);

static void  ScanKeySensor(void);

void	SwitchLogicConfigure(void);

static void  KeyProcess(void);

void	KeyDeal(void);
/****************ram	macros***************************/
_DriveKeyDeal_EXTERN	ADStruct	ADresult;

_DriveKeyDeal_EXTERN	unsigned char	NormAdcData1;

_DriveKeyDeal_EXTERN	SelfKeyStruct	KeyData[2],OldKeyData[2],FrontKeyData[2],BackKeyData[2],TopKeyData[2],MeterKeyData[5],PowerControlKeyData[1],MultiFunctionKeyData[1],MeterDummyKeyData[1];

_DriveKeyDeal_EXTERN	unsigned char	FirstKeyData[2],SecondKeyData[2];

_DriveKeyDeal_EXTERN	unsigned char	AdcNumber;
                                                                             
_DriveKeyDeal_EXTERN	unsigned	char	Hc4051ChannelAdcData[20],LastHc4051ChannelAdcData[15];

_DriveKeyDeal_EXTERN	SwLogicConfig  SwitchConfigure[2];                                                    

_DriveKeyDeal_EXTERN	unsigned	char	KeyAdcTime;	         

_DriveKeyDeal_EXTERN	unsigned	char	StAdcData[2];
/***************parameter macros*********************/
#define	LowKey				NormAdcData1/10*3		///when power is 26v,norm adc data is 117 and norm power is 10v;when low,the data is 54 and norm power is 5v;	
#define	HighKey 			NormAdcData1/6*5	///when high,the data is 176 and norm power is 15v
///#define	LowKey3			NormAdcData3/10*13	///norm adc data is 117;when low,the data is 152 and norm power is 13v	
///#define	HighKey3 			NormAdcData3/10*12	///when high,the data is 176 and norm power is 15v
#define	LowKey1			NormAdcData1/5*4		///norm adc data is 67;when low,the data is 152 and norm power is 13v
#define	StNormData		155

#define	Mcu_U24				0
#define	Mcu_U26				1
#define	Mcu_U13				2
#define	ADC_WAIT_TIME	250
/******************sensor	pin macros**********************/
#define		Sensor_Power_IO		PTBDD_PTBDD7
#define		Sensor_Power			PTBD_PTBD7
#define		V_Power_IO				PTBDD_PTBDD1
#define		V_Power						PTBD_PTBD1
/*******************ad sampling pin macros*************************/
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

#endif

