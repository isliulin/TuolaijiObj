
#ifndef __XGATE_H_
#define __XGATE_H_

/* XGATE vector table entry */
typedef void (*_NEAR XGATE_Function)(int);
typedef struct {
  XGATE_Function pc;        /* pointer to the handler */
  int dataptr;              /* pointer to the data of the handler */
} XGATE_TableEntry;

#pragma push								/* save current segment definitions */

#pragma CONST_SEG __GPAGE_SEG XGATE_VECTORS  /* for the HCS12X/XGATE shared setup, HCS12X needs global addressing to access the vector table. */

#define XGATE_VECTOR_OFFSET 9							   /* the first elements are unused and need not to be allocated (to save space) */

extern const XGATE_TableEntry XGATE_VectorTable[];

#pragma pop									/* restore previous segment definitions */


/* this code is to demonstrate how to share data between XGATE and S12X */
#pragma push								/* save current segment definitions */

#pragma DATA_SEG SHARED_DATA /* allocate the following variables in the segment SHARED_DATA */
volatile extern int shared_counter; /* volatile because both cores are accessing it. */
#pragma	pop
/*******step parameter macros******/
#define		S0_DTC				0x80
#define		S1_DTC				0x80
#define 	CLOCKWISE     0
#define 	CNTCLOCKWISE  1  			 
#define		PrescaleNum		16
/*******step number macros******/
#define PRES            		1//0      			 
#define TEMP            		2 ///1       		
#define Oilplant        		3///2      			
#define TACHOMETER      		5///3       
#define Carspeed						4///4
#define PRES2            		0///5   

#define 	STEP01						4
#define 	STEP1							8
#define 	STEP2							12
#define 	STEP23						16
#define 	STEP3							20
#define 	STEP34						24

#define		SPEED_CAPTURE_SEC1		10
#define		SPEED_CAPTURE_SEC2		15
#define		SPEED_CAPTURE_SEC3		20
#define		SPEED_CAPTURE_SEC4		30
#define		SPEED_CAPTURE_SEC5		50
#define		SPEED_CAPTURE_SEC6		70
#define		SPEED_CAPTURE_SEC7		90
#define		SPEED_CAPTURE_SEC8		110
#define		SPEED_CAPTURE_SEC9		140

#define		SEC1_CAPTURE_NUM			1
#define		SEC2_CAPTURE_NUM			3
#define		SEC3_CAPTURE_NUM			4
#define		SEC4_CAPTURE_NUM			6
#define		SEC5_CAPTURE_NUM			8
#define		SEC6_CAPTURE_NUM			10
#define		SEC7_CAPTURE_NUM			12
#define		SEC8_CAPTURE_NUM			14
#define		SEC9_CAPTURE_NUM			16

///vehicle speed macros
#define		SPEEDCAPTURETIMENUM			16				
/**************************Functions Prototypes**********************/
#pragma	CODE_SEG XGATE_CODE
static void motor_micro_update(uchar channe,unsigned char	Direction);
static void motor_update(uchar channel,unsigned char	Direction);
static	unsigned char Xgate_check_pos(unsigned char channel,unsigned int position);
static void	Xgate_motor_move(unsigned char channel,unsigned int position);
static void Xgate_CARSPEED(void);
//interrupt void calcRotationPulse(void);
#pragma	CODE_SEG	DEFAULT
#endif /* __XGATE_H_ */
