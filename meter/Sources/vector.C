#include	"config.h"
#pragma MESSAGE DISABLE C4200///屏蔽无必要的编译提示 
/************************funtion macros**************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */
static void  _EntryPoint(void); 
extern	interrupt	void   rti_int(void);
extern	interrupt	void   CAN0error_int(void);
extern	interrupt	void   CAN0rxd_int(void);
extern	interrupt	void   Can0WakeUpInterrupt(void);
extern	interrupt	void   WatchDogIsr(void);
///extern	interrupt void   tchanel7_Handler(void); 
extern	interrupt void   tchanel7_Handler(void);
extern	interrupt void   MSCAN1Rec(void);
extern	interrupt void   CAN1error_int(void);
#pragma CODE_SEG USER_ROM
/*********************no used interrupt***************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void UnimplementedISR(void) 
{
   for(;;);
}     
////////////////////////////////////////////////
typedef void (*near tIsrFunc)(void);
const tIsrFunc _vect[] @0xDF10 = {     /* Interrupt table */ 	
		  UnimplementedISR,			  //119
	    UnimplementedISR,	
	    UnimplementedISR,
	    UnimplementedISR,	
	    UnimplementedISR,
	    UnimplementedISR,
	    UnimplementedISR,
	    UnimplementedISR,	
	    UnimplementedISR,
	    UnimplementedISR,				//110	
	    UnimplementedISR,
	    UnimplementedISR,	
			UnimplementedISR,
			UnimplementedISR,	
			UnimplementedISR,
			UnimplementedISR,	
			UnimplementedISR,
			UnimplementedISR,
	    UnimplementedISR,
	    UnimplementedISR,				//100	    
	    UnimplementedISR,	    
	    UnimplementedISR,
	    UnimplementedISR,	    
	    UnimplementedISR,	    
	    UnimplementedISR,
	    UnimplementedISR,	    
	    UnimplementedISR,	    
	    UnimplementedISR,
	    UnimplementedISR,	    
	    UnimplementedISR,	    
	    UnimplementedISR,					//90
	    UnimplementedISR,	    
	    UnimplementedISR,	    
	    UnimplementedISR,
	    UnimplementedISR,	    
	    UnimplementedISR,	    
	    UnimplementedISR,
	    UnimplementedISR,	    
	    UnimplementedISR,	    
	    UnimplementedISR,	    		  //80
	    UnimplementedISR,
        UnimplementedISR,                 /* vector 78 */
        UnimplementedISR,                 /* vector 77 */
        UnimplementedISR,                 /* vector 76 */
        UnimplementedISR,                 /* vector 75 */
        UnimplementedISR,                 /* vector 74 */
        UnimplementedISR,                 /* vector 73 */
        UnimplementedISR,                 /* vector 72 */
        UnimplementedISR,                 /* vector 71 */
        UnimplementedISR,                 /* vector 70 */
        UnimplementedISR,                 /* vector 69 */
        UnimplementedISR,                 /* vector 68 */
        UnimplementedISR,                 /* vector 67 */
        UnimplementedISR,                 /* vector 66 */
        UnimplementedISR,                 /* vector 65 */
        UnimplementedISR,                 /* vector 64 */
        UnimplementedISR,                 /* vector 63 */
        UnimplementedISR,                 /* vector 62 */
        UnimplementedISR,                 /* vector 61 */
        UnimplementedISR,                 /* vector 60 */
        UnimplementedISR,                 /* vector 59 */
        UnimplementedISR,                 /* vector 58 */
        UnimplementedISR,                 /* vector 57 */
        UnimplementedISR,                 /* vector 56 */
        UnimplementedISR,                 /* vector 55 */
        UnimplementedISR,                 /* vector 54 */
        UnimplementedISR,                 /* vector 53 */
        UnimplementedISR,                 /* vector 52 */
        UnimplementedISR,                 /* vector 51 */
        UnimplementedISR,                 /* vector 50 */
        UnimplementedISR,                 /* vector 49 */
        UnimplementedISR,                 /* vector 48 */
        UnimplementedISR,                 /* vector 47 */
        UnimplementedISR,                 /* vector 46 */
        UnimplementedISR,                 /* vector 45 */
        UnimplementedISR,                 /* vector 44 */
        UnimplementedISR,                 /* vector 43 */
        MSCAN1Rec,                 				/* vector 42 */
        CAN1error_int,                 		/* vector 41 */
        UnimplementedISR,                 /* vector 40 */
        UnimplementedISR,                 /* vector 39 */
        CAN0rxd_int,                 	    /* vector 38 */
        CAN0error_int,                 	  /* vector 37 */
        Can0WakeUpInterrupt,              /* vector 36 */
        UnimplementedISR,                 /* vector 35 */
        UnimplementedISR,                 /* vector 34 */
        UnimplementedISR,                 /* vector 33 */
        UnimplementedISR,                 /* vector 32 */
        UnimplementedISR,                 /* vector 31 */
        UnimplementedISR,                 /* vector 30 */
        UnimplementedISR,                 /* vector 29 */
        UnimplementedISR,                 /* vector 28 */
        UnimplementedISR,                 /* vector 27 */
        UnimplementedISR,                 /* vector 26 */
        UnimplementedISR,                 /* vector 25 */
        UnimplementedISR,                 /* vector 24 */
        UnimplementedISR,                 /* vector 23 */
        UnimplementedISR,                 /* vector 22 */
        UnimplementedISR,                 /* vector 21 */
        UnimplementedISR,       					/* vector 20 */
        UnimplementedISR,                 /* vector 19 */
        UnimplementedISR,                 /* vector 18 */
        UnimplementedISR,                 /* vector 17 */
        UnimplementedISR,                 /* vector 16 */
        tchanel7_Handler,                 /* vector 15 */
        UnimplementedISR,                 /* vector 14 */
        UnimplementedISR,                 /* vector 13 */
        UnimplementedISR,                 /* vector 12 */
        UnimplementedISR,                 /* vector 11 */
        UnimplementedISR,                 /* vector 10 */
        UnimplementedISR,                 /* vector 09 */
        UnimplementedISR,                 /* vector 08 */
        rti_int,                 		  		/* vector 07 */
        UnimplementedISR,                 /* vector 06 */
        UnimplementedISR,                 /* vector 05 */
        UnimplementedISR,                 /* vector 04 */
        UnimplementedISR,                 /* vector 03 */
        UnimplementedISR,                 /* vector 02 */
				UnimplementedISR,		
	 			_EntryPoint,
};
/*************************************************
**************************************************/
#pragma CODE_SEG ROM_USER_START
static void _EntryPoint(void)
{
  __asm jmp _Startup;                   /* Jump to C startup code */
}  
#pragma CODE_SEG  USER_ROM


