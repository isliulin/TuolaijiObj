#include "ap_lpit.h"

void lpit0Init()
{
	PCC_LPIT0 = PCC_LPIT0_CGC(1)|PCC_LPIT0_PCS(6);
	//Enable the protocol clock by setting the M_CEN bit in the MCR register.
	LPIT0_MCR = 0x00000001u;//Enables the peripheral clock to the module timers.
	//Wait for 4 protocol clock cycles
	//wait4ProtocolClock();
	__asm("nop");
	LPIT0_CLRTEN = LPIT_CLRTEN_CLR_T_EN_0(1)
	  			 | LPIT_CLRTEN_CLR_T_EN_1(1)
				 | LPIT_CLRTEN_CLR_T_EN_2(1)
				 | LPIT_CLRTEN_CLR_T_EN_3(1);
    
	LPIT0_TCTRL0 = 0x0000000000u; 	//MODE(b3-2):			0 32-bit Periodic Counter
									//TSOT(b16)			0 Timer starts to decrement immediately based on restart
									//TSOI(b17)			0 Timer does not stop after timeout
									//TROT	(B18)			0 Timer will not reload on selected trigger
									//TRG_SRC(b23):		1 Trigger source selected is the internal trigger
									//TRG_SEL(b27-24)	0 Time channel '0' trigger source is selected
	LPIT0_TCTRL1 = 0x0000000000u;
	LPIT0_TCTRL2 = 0x0000000000u;
	LPIT0_TCTRL3 = 0x0000000000u;
	
	//set hte timer timeout value
	LPIT0_TVAL0 = CH0TIMER - 1 ;
	LPIT0_TVAL1 = CH1TIMER - 1;
	LPIT0_TVAL2 = CH2TIMER - 1;
	LPIT0_TVAL3 = CH3TIMER - 1;
	//Configure TIEn bits in MIER
	LPIT0_MIER = LPIT_MIER_TIE0_MASK	//Channel 0 Timer Interrupt Enable
			   | LPIT_MIER_TIE1_MASK	//Channel 1 Timer Interrupt Enable
			   | LPIT_MIER_TIE2_MASK	//Channel 2 Timer Interrupt Enable
			   | LPIT_MIER_TIE3_MASK	//Channel 3 Timer Interrupt Enable
	  			;
	//LPIT0_MCR |= 0x0000000C;	//DBG_EN(b3):		1 continue to run in debug mode
								//DOZE_EN(b2)	1 continue to run in doze mode
	LPIT0_TCTRL0 = 0x00000001u;	//T_EN(b1)		1 Timer Channel is enabled
	LPIT0_TCTRL1 |= LPIT_TCTRL1_T_EN_MASK;
	LPIT0_TCTRL2 |= LPIT_TCTRL2_T_EN_MASK;
	LPIT0_TCTRL3 |= LPIT_TCTRL3_T_EN_MASK;

	LPIT0_CLRTEN = LPIT_CLRTEN_CLR_T_EN_0_MASK
				|LPIT_CLRTEN_CLR_T_EN_1_MASK
				|LPIT_CLRTEN_CLR_T_EN_2_MASK
				|LPIT_CLRTEN_CLR_T_EN_3_MASK
				;  
	LPIT0_SETTEN = LPIT_SETTEN_SET_T_EN_0_MASK	//该位写0无效
	 // 			 | LPIT_SETTEN_SET_T_EN_1_MASK
	//			 | LPIT_SETTEN_SET_T_EN_2_MASK
				 | LPIT_SETTEN_SET_T_EN_3_MASK
	  			;

	//for channels configured in Capture Mode

	//any time can read
	//currentValue = LPIT0_CVAL0;
	//clear flag TIF
	LPIT0_MSR = 0x0000000fu;// clear all 4 channels flags

	enable_irq (48);//ch0
	set_irq_priority (48, 3);
//	enable_irq (49);//ch1
//	enable_irq (50);//ch2
//	enable_irq (51);//ch3
}	
