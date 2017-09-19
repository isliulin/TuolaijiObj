#ifndef	_Bootloader_Init
#define		_Bootloader_Init

#include 	"hidef.h" 		/* for EnableInterrupts macro */
#include 	"derivative.h" /* include peripheral declarations */
#include	"Macros.h"
#include	"Bootloader.h"
#include	"SelfID.h"

#ifndef	_No_Bootloader_Init_EXTERN
#define		_Bootloadr_Init_EXTERN	extern
#else
#define		_Bootloadr_Init_EXTERN
#endif
/**********funtion macros************/
static void Init_MCG(void);
static void Init_Cop(void);
static void Init_LVR(void);
void Bootloader_MSCAN_Init(unsigned char	CanConfig);
void StartStateDetect(void);
/**********global variable macros**********/
_Bootloadr_Init_EXTERN	volatile unsigned char Last_Position @0xEDFE;
#if _Mcu_Type==32
  _Bootloadr_Init_EXTERN	volatile unsigned char First_Position @0x7c00; 
#elif _Mcu_Type==60
  _Bootloadr_Init_EXTERN	volatile unsigned char First_Position @0x1900; 
#endif

#endif


