#define		_NO_CPUINIT_EXTERN
#include	"CpuInit.h"
///#define		_Test
/*========================================================
 *brief:    Cpu initialization.
 *author:   yao qing rui
 *input:    void
 *output:   void
 *call:		
========================================================*/
void	CpuInit(void)
{  
	unsigned char i;
	
	///MCG_Init();     							/* Clock Generator initialization ,if use bootloader,not need to initialize*/

	///Init_Cop();

	///Init_LVD();
	
	GPIO_Init();   				 				/* GPIO configuration */ 
	
	for(i=0;i<3;i++)
		Out595(i,0);								/*if start,close all load first*/

	 ADC_Init();  									/*ADC initialization*/
	 
	 __RESET_WATCHDOG();				/*feed watch dog*/
	 External_WatchDog();
		
    TestSelfID();									/*test self id*/

	 RamInit();   									/*Initialize ram state*/

    SwitchLogicConfigure();				/*after Test self ID and then configure switch logic*/ 

	LoadOpenDetect();
   
    Rti_Init();     									/* Real time counter initialization */

	#if 1
	 if(BackDriveFlag==1)						/*because it is in back drive*/
	 {
 		Capture_Ja59_Init();  			/*capture initialization*/
//		Capture_Ja48_Init();
	 }
	 #endif
	 
   MSCAN_Init(NormalCanMode);	/*MSCAN cofiguration*/    

	EnableInterrupts;  						/* Enable interrupts */
}
/*========================================================
 *brief:    Multipurpose Clock Generator initialization.
 *author:   yao qing rui
 *input:    void
 *output:   void
========================================================*/
static void MCG_Init(void) 
{
 	/*  System clock initialization */
	MCGTRM = *(unsigned char*)0xFFAF;    /* Initialize MCGTRM register from a non volatile memory */
	MCGSC = *(unsigned char*)0xFFAE;     /* Initialize MCGSC register from a non volatile memory */
	MCGC2=0x36;/* MCGC2: BDIV=0,RANGE=1,HGO=1,LP=0,EREFS=1,ERCLKEN=1,EREFSTEN=0 */
	MCGC1=0xBA;/* MCGC1: CLKS=2,RDIV=7,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
	MCGC3=0x01;/* MCGC1: CLKS=2,RDIV=7,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
	while(!MCGSC_OSCINIT) {              /* Wait until external reference is stable */
	}
	while(MCGSC_IREFST) {                /* Wait until external reference is selected */
	}
	while((MCGSC & 0x0C) != 0x08) {      /* Wait until external clock is selected as a bus clock reference */
	}
	MCGC2=0x3E;/* MCGC2: BDIV=0,RANGE=1,HGO=1,LP=1,EREFS=1,ERCLKEN=1,EREFSTEN=0 */
	MCGC1=0x92;/* MCGC1: CLKS=2,RDIV=2,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
	MCGC3=0x41;/* MCGC3: LOLIE=0,PLLS=1,CME=0,??=0,VDIV=1 */
}
/*********************************************************
**********************************************************/
static void Init_Cop(void)
{
	SOPT2_COPCLKS=0;					
	SOPT2_COPW=0;   
	SOPT1_COPT = 3;         /* Watchdog enabled 1.024s*/
}
/*********************************************************
**********************************************************/
static void Init_LVD(void)
{
	SPMSC1_LVDE=1;					/* Low-Voltage Detect Enable */
	SPMSC1_LVDRE=1;					/* Low-Voltage Detect Reset Enable */
	SPMSC2_LVDV =1;					/*LVW Trip Point 4.6V*/
	SPMSC2_LVWV =1;	
}
/*========================================================
 *brief:    IO initialization.
 *author:   yao qing rui
 *input:    void
 *output:   void
========================================================*/
static void GPIO_Init(void)
{       
    PTADD=0b00001001;/*bit7->irq interrupt pin;bit6->f01 sensor pin;bit5->f03 sensor pin;bit4->f06 sensor pin;
    									bit3->4051 dc3 output pin;bit2->adc2 input pin;bit1->adc0 input pin;bit0->165 clock output pin*/
    PTAD=0b00000000;
    
    PTBDD=0b10001011;/*bit7->an_power drive pin;bit6->st17 input pin;bit5->f02 sensor pin;bit4->f05 sensor pin;
											bit3->4051 db3 drive pin;bit2->adc1 input pin;bit1->v-power drive pin;bit0->595 send data drive pin*/
    PTBD=0b00000000;		
    
    PTCDD=0b00001011;/*bit7->temperature sensor input pin;bit6->165 data input pin;bit5->st18 input pin;bit4->f04 sensor input pin;
											bit3->4051 da3 drive pin;bit2->165 data input pin;bit1->165 lock drive pin;bit0->595 clock drive pin*/
    PTCD=0b00000000;	
    
    PTDDD=0b00111100;/*bit7->Nc1 be set to input pin;bit6->Nc2 be set to input pin;bit5->in8 drive pin;bit4->in7 drive pin;
    									bit3>in6 drive pin;bit2->in5 drive pin;bit1->rotation capture input pin;bit0->speed capture input pin*/
    PTDD=0b00000000;
    
    PTEDD=0b01111100;/*bit7->can rx;bit6->can tx;bit5->595 clock drive pin;bit4->595 lock drive pin;
    									bit3->595 oe drive pin;bit2->595 send data drive pin;bit1->no used pin;bit0->no used pin*/
    PTED=0b01001000;		/*first must set can tx to 1;set oe pin to 1 to close 595*/
    
    PTFDD=0b11111111;/*bit7->CAN bus rs drive pin;bit6->at24c02 sda drive pin;bit5->at24c02 scl dive pin;bit4->external wathcdog drive pin;
    									bit3->4051 da2 drive pin;bit2->4051 dc1 drive pin;bit1->4051 db1 drive pin;bit0->4051 da1 drive pin*/
    PTFD=0b00000000;
    
    PTGDD=0b00111100;/*bit5->dc2 drive pin;bit4->db2 drive pin;bit3->mosfet drive pin;bit2->mosfet drive pin*/
    PTGD=0b00000000;	/*irst close Mosfet1 and Mosfet2*/
}
/*========================================================
 *brief:    Rti initialization
 *author:   yao qing rui
 *input:    void
 *output:   void
========================================================*/
static void Rti_Init(void)
{       
    RTCSC = 0x33;  /*bit7->Real-Time Interrupt occur Flag;bit6.5->Clock Source Select:the source clock is external;
    								bit4->enable interrupt;bit3~0->Clock Prescaler Select:2^12*/
    RTCMOD=0x13;   /*base data*/
}
/*========================================================
 *brief:    capture initialization.
 *author:   yao qing rui
 *input:    void
 *output:   void
========================================================*/ 
static void	Capture_Ja59_Init(void)
{	  
    TPM2SC=0X0f;       /*bit7->Timer Overflow Flag;bit6->Timer Overflow Interrupt Enable;bit5->Center-Aligned PWM Select;
    										bit4.3->Clock Source Select;bi2~0->Prescale Divisor Select;128*(1/8)=16us计算一次*/
    
    TPM2C0SC=0X48;     /*bit7->Channel 0 Occur Flag;bit6->enable interrupt;bit5.4->Mode Select for TPM Channel 0;
    										bit3.2->Edge/Level Select,Down edge*/  
}
/*========================================================
 *brief:    capture initialization.
 *author:   yao qing rui
 *input:    void
 *output:   void
========================================================*/ 
static void	Capture_Ja48_Init(void)
{	  
    TPM2SC=0X0f;       /*bit7->Timer Overflow Flag;bit6->Timer Overflow Interrupt Enable;bit5->Center-Aligned PWM Select;
    										bit4.3->Clock Source Select;bi2~0->Prescale Divisor Select;128*(1/8)=16us计算一次*/
    
    TPM2C1SC=0X48;     /*bit7->Channel 0 Occur Flag;bit6->enable interrupt;bit5.4->Mode Select for TPM Channel 1;
    										bit3.2->Edge/Level Select,Down edge*/  
}
/*========================================================
 *brief:    ADC initialization.
 *author:   yao qing rui
 *input:    void
 *output:   void
========================================================*/ 
static void	ADC_Init(void)
{
	ADCCFG = 0x91;			/*bit7->Low Power Configuration;bit6.5->Clock Divide Select;bit4->Long Sample Time Configuration;
												bit3.2->Conversion Mode Selection:10bit adc;bit1.0->Input Clock Select:bus clock/2*/							
	APCTL1=0b01110110;	/*bit1.2.4.5.6 is select to adc pin;other is common io*/
  	APCTL2=0b01110100;	/*bit1.2.4.5 is select to adc pin;other is common io*/
 	APCTL3=0b10110000;	/*bit4.7 is select to adc pin;other is common io*/
}
/***************************************************
* \brief    MSCAN configuration, baud rate = 125kbps, 16 bit acceptance filters
* \author   karry
* \param    void
* \return   void
*****************************************************/
void MSCAN_Init(unsigned char	ConfigureCanMode) 
{   	

	CANCTL0_INITRQ = 1;   						/* MSCAN in initialization mode */

    while (!(CANCTL1_INITAK));				/* Wait for initialization mode acknowledge */
       
    CANCTL1_CANE = 1;      					/* Enable MSCAN module */        
    
    CANCTL1_CLKSRC = CAN_CLK;    		/* Clock source selection */    
    CANCTL1_LOOPB = 0;          				/* Set to 1 for LoopBack Mode, 0 otherwise */    
    CANCTL1_LISTEN = 0;         				/* Not listen only mode */        

	if(ConfigureCanMode==NormalCanMode)
  		asm nop;  
  	else if(ConfigureCanMode==ListenCanMode)   
  		CANCTL1_LISTEN = 1;         			/* listen only mode */    
                                                                              
    CANBTR1_TSEG_10 = TIME_SEGM1;    /* Time Segment 1 */
    CANBTR1_TSEG_20 = TIME_SEGM2;    /* Time Segment 2 */                             
                                    
    CANBTR0_BRP = CAN_PRESC;   			/* Baud rate prescaler */  
    CANBTR0_SJW = CAN_SYNJW;    			/* Sinchronization jump width */    
    CANBTR1_SAMP = 0;      						/* One sample per bit */	      
             
    CANIDAC_IDAM = 0;         		 				///1->  所接收的数据为11位的标准帧；0->所接收的数据为29位标准帧
		
    CANIDAR0 = 0xFF;				
    CANIDAR1 = 0xFF;				
    CANIDAR2 = 0xFF;				
    CANIDAR3 = 0xFF;
    CANIDAR4 = 0xFF;
    CANIDAR5 = 0xFF;
    CANIDAR6 = 0xFF;
    CANIDAR7 = 0xFF;
    CANIDMR0 = 0xff;               					/*不设置接受过滤*/
    CANIDMR1 = 0xff;
    CANIDMR2 = 0xff;
    CANIDMR3 = 0xff;
    CANIDMR4 = 0xff;
    CANIDMR5 = 0xff;
    CANIDMR6 = 0xff;
    CANIDMR7 = 0xff;
	
	/*
	///Can Id Mask	
	if(FrontDriveFlag==1)
		BootCanMask=FrontDriveID;
	else if(BackDriveFlag==1)
		BootCanMask=BackDriveID;
	else if(TopDriveFlag==1)
		BootCanMask=TopDriveID;
	CANIDAR0 = (BootCanMask>>21) & 0xFF;	///PGN change to 32bit	 
	CANIDAR1 = (BootCanMask>>18) & 0x07; 
	CANIDAR1 = CANIDAR1 << 5;
	CANIDAR1 = CANIDAR1 | ((BootCanMask>>15) & 0x07);
	CANIDAR2 = (BootCanMask>>7) & 0xFF; 
	CANIDAR3 = (BootCanMask<<1) & 0xFF; 
	
	CANIDAR4 =CANIDAR0;
	CANIDAR5 =CANIDAR1;
	CANIDAR6 =CANIDAR2;
	CANIDAR7 =CANIDAR3;
	
	CANIDMR0 = 0x00;               	///设置过滤位
	CANIDMR1 = 0x18;					///SRR及IDE位不过滤
	CANIDMR2 = 0x00;
	CANIDMR3 = 0x01;					///RTR不过滤
	CANIDMR4 = 0x00;
	CANIDMR5 = 0x18;
	CANIDMR6 = 0x00;
	CANIDMR7 = 0x01;*/

	CANCTL0_INITRQ = 0;            			/* Exit initialization mode request */

    while (CANCTL1_INITAK);     				/* Wait for normal mode */   

    while(!(CANCTL0_SYNCH));  			/* Wait for CAN synchronization */

	CANRFLG = 0xC3;          /* Reset Receiver Flags
                             *  
                             *  0b11000011
                             *      ||||||||__ Receive Buffer Full Flag
                             *      |||||||___ Overrun Interrupt Flag
                             *      ||||||____ 
                             *      |||||_____>- Transmitter Status Bits
                             *      ||||______ 
                             *      |||_______>- Receiver Status Bits
                             *      ||________ CAN Status Change Interrupt Flag
                             *      |_________ Wake-Up Interrupt Flag
                             */                                 
	CANRIER_RXFIE = 1;     			/* Enable Full Receive Buffer interrupt */   
	CANRIER_CSCIE=1;					/*OPEN can error interrupt*/
	CANRIER_TSTATE0=1;			/*RxERR: 127 < receive error counter*/
	CANRIER_TSTATE1=1;   
	CANRIER_RSTATE0=1;			/*TxERR: 127 < transmit error counter ≤ 255*/
	CANRIER_RSTATE1=1;    
}
/**********************************
***********************************/
static void	RamInit(void)
{
	unsigned char	i;

	for(i=0;i<2;i++)
		OldKeyData[i].Data=0xff;
	ClearErrorFlag=1;											///if power on,clear error
	if(FrontDriveFlag==1)
	{
		ReRainScratchIntermission=6;			///initilize the rain scratch intermission time	
		RunToOffFlag=1;
		RainScratchRealyOffFlag=1;
	}
	else if(BackDriveFlag)
	{
		OldKeyData[0].b.Key6=0;						///engine room open key is key 7
		BackRoomKeyBit=KeyOn;						///when power on at first,let engine room at close state
	}
}

