#undef	EXT
#ifdef	STEPDEAL_C
#define	EXT
#else
#define	EXT extern
#endif
/* common defines and macros */
#include <hidef.h>      	
#include <mc9s12xhz512.h>
#include	"typedef.h"
/***************parameter macros********************/
#define	StepMotorNum		6

#define OK							1
#define STEPHIG					2
#define STEPLOW 				3
#define STEPNOT					4

#define TACHOMAXSTEP    3000//11520  ///rotation step is 0~245  (0-35krpm/min)
#define PRESMAXSTEP     1100//4400   ///air press 1 step is 0~90  (0-1MPA) 
#define OILMAXSTEP      1100//4400   ///oil step is 0~90   (0-1)
#define TEMPMAXSTEP     1100//4400   ///water temperature step is 0~90   (40-120бу))
#define CARSPEEDMAXSTEP 3000//11760  ///speed step is 0~245  (0-140km/min)
#define PRES2MAXSTEP    1100//4400   ///air press 2 step is 0~90 (0-1MPA) 
#define	MotorBackDelayNum	800
#define	VOLTAGEMAXSTEP		3000
#define	CAR_SPEED_1KM_STEP_NUM	21
/******************funtion macros*************************/
#pragma CODE_SEG  USER_ROM
void	StepDeal(void);
void 	CARSPEED(void);
void  back0_motor(uchar BigBack );
void 	EngRotation(void) ;
static void 	OilPlant(void) ;
static void 	WatTemp(void) ; 
static uchar 	check_pos(unsigned char channel,unsigned int position);
static void	motor_move(unsigned char channel,unsigned int position);
static void GasPres1(void);
static void GasPres2(void);
static void VoltageStep(void);
static void SOCStep(void);

#pragma CODE_SEG DEFAULT
/*****************Ram macros******************/
EXT	volatile	uchar		carspeed;
EXT uint   	delay1s;
EXT uchar   	delay1s_1;
EXT	volatile 	uchar 	resetback0;            
EXT	volatile  uint  	MotorFinalStep[StepMotorNum],MotorCurStep[StepMotorNum];
EXT	volatile  uint  	MotorMaxStep[StepMotorNum],MotorMinStep[StepMotorNum];
EXT volatile  uchar   pres_value;
EXT volatile  uchar  	gaspress2;


