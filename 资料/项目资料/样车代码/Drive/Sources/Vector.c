extern	void _Startup(void);
extern	interrupt void  rtc_isr(void);
extern	interrupt void  CANRx_ISR(void);
extern	interrupt void CAN0error_int(void);
extern	interrupt void tpm2ch1_isr(void);
extern	interrupt void tpm2ch0_isr(void);
static void	_Entry_Point(void);
/**************************************************
Description:Disable the warning information of editor
**************************************************/
#pragma MESSAGE DISABLE C4000 
#pragma MESSAGE DISABLE C4200 
/**************************************************/
static interrupt void Cpu_Interrupt(void)											    
{	    																			    
	while(1); 																		    
}  
/**********************entry address****************/
#pragma	CODE_SEG	ENTRYPOINT_ROM
static void	_Entry_Point(void)
{
	asm nop;
	asm nop;
	_asm jmp	_Startup;
}
#pragma	CODE_SEG	DEFAULT
/***************************************************
****************************************************/
/*if bootloader the first vector number is 0xEDC0,else is 0xffc0*/
void (* near const _vect[])() @0xEDC0 = { /* Interrupt vector table */
         Cpu_Interrupt,                /* Int.no. 31 Vacmp2 (at F3C0)                Unassigned */
         Cpu_Interrupt,                /* Int.no. 30 Vacmp1 (at F3C2)                Unassigned */
         Cpu_Interrupt,                /* Int.no. 29 Vcantx (at F3C4)                Unassigned */
         CANRx_ISR,                		 /* Int.no. 28 Vcanrx (at F3C6)                Used */
         CAN0error_int,                /* Int.no. 27 Vcanerr (at F3C8)               Used */
         Cpu_Interrupt,                /* Int.no. 26 Vcanwu (at F3CA)                Unassigned */
         rtc_isr,            					 /* Int.no. 25 Vrtc (at F3CC)                  Used */
         Cpu_Interrupt,                /* Int.no. 24 Viic (at F3CE)                  Unassigned */
         Cpu_Interrupt,                /* Int.no. 23 Vadc (at F3D0)                  Unassigned */
         Cpu_Interrupt,                /* Int.no. 22 Vport (at F3D2)                 Unassigned */
         Cpu_Interrupt,                /* Int.no. 21 Vsci2tx (at F3D4)               Unassigned */
         Cpu_Interrupt,                /* Int.no. 20 Vsci2rx (at F3D6)               Unassigned */
         Cpu_Interrupt,                /* Int.no. 19 Vsci2err (at F3D8)              Unassigned */
         Cpu_Interrupt,                /* Int.no. 18 Vsci1tx (at F3DA)               Unassigned */
         Cpu_Interrupt,                /* Int.no. 17 Vsci1rx (at F3DC)               Unassigned */
         Cpu_Interrupt,                /* Int.no. 16 Vsci1err (at F3DE)              Unassigned */
         Cpu_Interrupt,                /* Int.no. 15 Vspi (at F3E0)                  Unassigned */
         Cpu_Interrupt,                /* Int.no. 14 Vtpm2ovf (at F3E2)              Unassigned */
         tpm2ch1_isr, 						    		/* Int.no. 13 Vtpm2ch1 (at F3E4)              Used */
         tpm2ch0_isr,					     			/* Int.no. 12 Vtpm2ch0 (at F3E6)              Unassigned */
         Cpu_Interrupt,                /* Int.no. 11 Vtpm1ovf (at F3E8)              Unassigned */
         Cpu_Interrupt,                /* Int.no. 10 Vtpm1ch5 (at F3EA)              Unassigned */
         Cpu_Interrupt,                /* Int.no.  9 Vtpm1ch4 (at F3EC)              Unassigned */
         Cpu_Interrupt,                /* Int.no.  8 Vtpm1ch3 (at F3EE)              Unassigned */
         Cpu_Interrupt,                /* Int.no.  7 Vtpm1ch2 (at F3F0)              Unassigned */
         Cpu_Interrupt,                /* Int.no.  6 Vtpm1ch1 (at F3F2)              Unassigned */
         Cpu_Interrupt,                /* Int.no.  5 Vtpm1ch0 (at F3F4)              Unassigned */
         Cpu_Interrupt,                /* Int.no.  4 Vlol (at F3F6)                  Unassigned */
         Cpu_Interrupt,                /* Int.no.  3 Vlvd (at F3F8)                  Unassigned */
         Cpu_Interrupt,                /* Int.no.  2 Virq (at F3FA)                  Unassigned */
         Cpu_Interrupt,             		 /* Int.no.  1 Vswi (at F3FC)                  Unassigned */
         _Entry_Point,									/* Int.no.  0 VReset(at F3FE) 	 							 Used*/
 };









