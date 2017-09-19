#ifndef	_SelfID_H
#define		_SelfID_H

#include 	"hidef.h" 		/* for EnableInterrupts macro */
#include 	"derivative.h" /* include peripheral declarations */
#include	"Typedef.h"
#include	"BitMacros.h"
#include	"DriveKeyDeal.h"
#include	"Common.h"

#ifndef	_NO_SelfID_EXTERN
#define		_SelfID_EXTERN	extern
#else
#define		_SelfID_EXTERN
#endif
/*************funtion macros**************/
void TestSelfID(void);
/****************global variable macros*********************/
_SelfID_EXTERN	SelfIDStruct	SelfID,TempSelfID;
_SelfID_EXTERN	unsigned char	NormAdcData;
/***************parameter macros*********************/
#define	ModuleCheckLowKey			NormAdcData/10*3		///when power is 26v,norm adc data is 117 and norm power is 10v;when low,the data is 54 and norm power is 5v;	
#define	ModuleCheckHighKey 		NormAdcData/6*5	///when high,the data is 176 and norm power is 15v

#define	AD_ENABLE_DELAY_NUM		4000
#endif

