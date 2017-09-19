#define 	CPUINIT_C
#include	"config.h"

#pragma CODE_SEG  USER_ROM

/******************************************************************
funtion operation:initialization of cpu
funtion call:void pim_init(void);void RamInit(void);
		void CRGModuleInitial(void);void timer0_init(void);
		void CAN1_Init(void);void AD_Init(void);
input parameter:void
return:void	
 ******************************************************************/
void CpuInit(void)
{
  Vector_Init();
  CRGModuleInitial();
	Clear_256_WatchDog();
  pim_init();	
  IrqClose();
  PowerOffSaveRead();
	RamInit();
	InputCaptureParameter_Initial();
  Meterswitchconfig();
  init_motor();
  SetupXGATE();
  init_timer();	
	AD_Init();
	Clear_256_WatchDog();
	J1939_SignalCheck();
  EnableInterrupts;
	back0_motor(1);
  CAN0_Init();
  CAN1Init();
	pwm_Init();
	Init_Capture_Funtion();
}

/****************************************
funtion operation:the mcu pin initialization
input parameter:void
return:void
 ****************************************/
static void pim_init(void)
{
	ECLKCTL|=192;
 
	WOMT = 0X00;			  //enable pp0 and pp2 for rxd1 txd1,1 for ps2 and ps3

  RDRIV = 0x00;			  //for pk,pe,pd,pc,pb,pa full drive
  PPST = 0X00;

  PORTA = 0X00;	 
  DDRA  = 0xff;			  
    
	PORTB = 0X00;
	DDRB  = 0XFF;			  //pb0-pb3 for hc4016 drive led
	  
	PTL   = 0x00;  
	PERL  = 0x00;
	DDRL  = 0b00001001;	
		
	PORTK = 0x00;			  //pk6 -PK7 for none
	DDRK  = 0xff;			  //pk0-3 for none; PK4 for GPIO1 ,PK5 for GPIO2,
 
	PORTC = 0X00;			  
	DDRC  = 0xff;			  //pc0-pc3 for key,pc4 for none,pc5-pc7 for mc33975_1 spi

	ECLKCTL|=0x80;             
	PORTE = 0X00;			  //pe4 -pe6 for none
	DDRE  = 0X70;			  //pe0 for xirq;pe1 for irq;pe2 -pe3 for key;

  PTU   = 0X00;	
	DDRU  = 0XFF;			  //motor1-2 		 

	PTV   = 0X00;			  //motor3-motor4  
	DDRV  = 0XFF;

	PTW   = 0X00;
	DDRW  = 0XFF;

  PTT   = 0X00;			  //pt4 for sda pt5 for sck,pt6 for an_pow,pt7 for led3
	DDRT  = 0b10111111;			  //PT0 for mc339751 so,pt1 for int1_mcz1,pt2 for c_rpm,pt3 for c_speed

  PTP   = 0b00000000;	///pp0 for tx pp2 for rx ,pp1 for led4,pp3 for pwm2 set 1 to close power,pp4 for pwm2,
	DDRP  = 0Xff;			  //pp5 for pwm1,pp6 for sda ,pp7 for sck

  PTS   = 0X00;			  //ps4-ps7 for SPI 
	DDRS  = 0XFf;			  //ps0 for in1;ps1 for in2;ps2 for led5;ps3 for none

  PTM   = 0x00; 
	DDRM  = 0Xff;			  //PM1 for CAN0 RS;pm2 canrx0;pm3 cantx0;pm4 canrx1;pm5 cantx1;

	PORTD = 0x00;
  DDRD  = 0XFF;			  //pd0-pd3 for mc339752 spi,pd4 for led8,others none  

	DDRAD = 0X00;	         
	PTAD  = 0X00;     //pad5 for INT_MCZ2;pad4 for ADC_MCZ2;pad3 for panel temp
								
  Cat4016_Data[0].Data=0x0000;			///close all led 
	Cat4016_Drive(&Cat4016_Data[0].Data);  
}

/***************************************
funtion operation:CRG module initialization
 ***************************************/
static void CRGModuleInitial(void)
{
	CLKSEL_PLLSEL = 0; 
	CLKSEL_RTIWAI = 1; ///rtc stop in wait mode
	CLKSEL_COPWAI = 1; ///cop stop in wait mode
	RTICTL = 0x49; ///16M OSC 5.12ms enter RTI;10x(1024*8)/16M
	COPCTL = 0x00; ///1s will reset COP  funtion
	CRGINT_RTIE = 1; ///RTI interrupt enable
}
/********************************************
funtion operation: initialization ram of the mcu
 ********************************************/
static void RamInit(void)
{
  unsigned char i;
	unsigned char DataTemp[5];
	
	///新增加需要初始化的参数
	///LightLevel=2;
  Eeprom_Read_Fact(RainScratch_Addr, DataTemp, 5);
	rain_scr=DataTemp[0];
  if ((rain_scr < RainScratchHighSpeedNum) || (rain_scr > RainScratchLowSpeedNum))
    rain_scr = RainScratchInitSpeedNum;

	Eeprom_Read_Fact(TPMSIDSAVEStateAddr, (uchar*)&TPMS_ID, 18);

	LightLevel=DataTemp[1];
	if(LightLevel>3)
		LightLevel=2;

	BuzzerLevel=DataTemp[2];
	if(BuzzerLevel>3)
		BuzzerLevel=2;
	
	VehicleSmallDistanceLow=read_data[1];
	VehicleSmallDistanceLow<<=8;
	VehicleSmallDistanceLow+=read_data[0];
	
	VehicleSmallDistanceHigh=read_data[3];
	VehicleSmallDistanceHigh<<=8;
	VehicleSmallDistanceHigh+=read_data[2];
  if (VehicleSmallDistanceLow == 0xffff && VehicleSmallDistanceHigh == 0xffff)
  ///if it is the first initial,must be reset to 0
  {
    VehicleSmallDistanceLow = 0;
    VehicleSmallDistanceHigh = 0;
  }
  OldVehicleSmallDistance = VehicleSmallDistanceHigh;
  OldVehicleSmallDistance <<= 16;
  OldVehicleSmallDistance += VehicleSmallDistanceLow;
  VehicleSmallDistance = OldVehicleSmallDistance;
    ///give the value to the first display
	VehicleOverallDistanceLow=read_data[5];
	VehicleOverallDistanceLow<<=8;
	VehicleOverallDistanceLow+=read_data[4];
	
	VehicleOverallDistanceHigh=read_data[7];
	VehicleOverallDistanceHigh<<=8;
	VehicleOverallDistanceHigh+=read_data[6];
  if (VehicleOverallDistanceLow == 0xffff && VehicleOverallDistanceHigh ==0xffff)
  ///if it is the first initial,must be reset to 0
  {
    VehicleOverallDistanceLow = 0;
    VehicleOverallDistanceHigh = 0;
  }
  OldVehicleOverallDistance = VehicleOverallDistanceHigh;
  OldVehicleOverallDistance <<= 16;
  OldVehicleOverallDistance += VehicleOverallDistanceLow;
  VehicleOverallDistance = OldVehicleOverallDistance;
  ///give the value to the first display
  OldVehicleOverallDistance -= OldVehicleSmallDistance / 100;
		
  ///first must reduce the VehicleSmallDistance,because it is repeat in last display deal

  VehicleSmallDistanceNum[0] = (uchar)((VehicleSmallDistance / 100) >> 8);
  VehicleSmallDistanceNum[1] = (uchar)(VehicleSmallDistance / 100);

  VehicleOverallDistanceNum[0] = (uchar)(VehicleOverallDistance >> 16);
  VehicleOverallDistanceNum[1] = (uchar)(VehicleOverallDistance >> 8);
  VehicleOverallDistanceNum[2] = (uchar)VehicleOverallDistance;

	//////////////////////////////////////////////////
	for(i=0;i<Num_PowerDevice;i++)
  {
		ReState[2*i].Data=0xff;
		ReState[2*i+1].Data=0xff;
			
  	TempReState[2*i].Data=0xff;
		TempReState[2*i+1].Data=0xff;
	}
#ifdef _INIT_TEST
	for(i=0;i<Num_PowerDevice;i++)
	{
		ReState[2*i].Data=0x55;
		ReState[2*i+1].Data=0x55;
	}
	for(i=0;i<DRIVELOADNUM;i++)
		FrontDriveLoad[i].Data=0x55;	
	for(i=0;i<DRIVELOADNUM;i++)
		BackDriveLoad[i].Data=0x55;	
	for(i=0;i<POWER_FUSE_NUM;i++)
		PowerFuse[i].Data=0x55;	
	for(i=0;i<POWERCONTRL_LOAD_NUM;i++)
		PowerContrlLoad[i].Data=0x55;	
#endif
	for(i=0;i<DRIVE_KEY_NUM;i++)
	 	FrontDriveKey[i].Data=0x00;	
	for(i=0;i<DRIVE_KEY_NUM;i++)
	 	BackDriveKey[i].Data=0x00;		
	for(i=0;i<DRIVE_KEY_NUM;i++)
	 	TopDriveKey[i].Data=0x00;		
	for(i=0;i<POWER_LOAD_NUM;i++)
		PowerLoad[i].Data=0xff;
	for(i=0;i<POWER_FUSE_NUM;i++)
		PowerFuse[i].Data=0xff;		
	for(i=0;i<POWERCONTRL_LOAD_NUM;i++)
		PowerContrlLoad[i].Data=0xff;

	st_status		=EngineIdleNum;	//first start shelves status is off
	st_statusOld=EngineIdleNum;
  SleepCodeBuf = WakeCode;
  FlashCodeBuf = AllFlashCloseCode;

  menu_id = LogoMenuNum;
//	Display_MainMenu_Flag=1;
	ID_require_flag=1;
	RainScratchChangeFlag = 1;
	SendConfigDataFlag=1;
	ConfigDataResult=0x01;	// 发送配置值

	BackRoomKeyBit=KeyOn;
//	LeftFrontStopHoofKeyBit =KeyOn;
//	RightFrontStopHoofKeyBit=KeyOn;	
//	LeftBackStopHoofKeyBit =KeyOn;
//	RightBackStopHoofKeyBit=KeyOn;	

	#ifdef	_INIT_TEST
	AccKeyBit=KeyOn;
	OnKeyBit=KeyOn;
	///LocationLampKeyBit=KeyOn;

	/*ReState[0].Data=0xf5;
	for(i=0;i<DRIVELOADNUM-1;i++)
		BackDriveLoad[i].Data=0xaa;//0x55;
	BackDriveLoad[i].Data=0xFa;
	for(i=0;i<DRIVELOADNUM-1;i++)
		FrontDriveLoad[i].Data=0xaa;//0x55;
	FrontDriveLoad[i].Data=0xFa;
	for(i=0;i<DRIVELOADNUM-1;i++)
		TopDriveLoad[i].Data=0xaa;//0x55;
	TopDriveLoad[i].Data=0xFa;
	for(i=0;i<POWER_LOAD_NUM-1;i++)
		PowerLoad[i].Data=0xaa;//0x55;
		PowerLoad[i].Data=0xFa;
	for(i=0;i<POWER_FUSE_NUM-1;i++)
		PowerFuse[i].Data=0xaa;*/
	#endif
}

/***********************************
funtion operation:initialization of CAN1 bus
 ************************************/
void CAN0_Init(void)
{
	uint CanInitCount=0;
	
  CAN0CTL0_CSWAI = 1; /* CAN Stops in Wait Mode */
  CAN0CTL0_INITRQ = 1; /* request into initial mode */
  while (!CAN0CTL1_INITAK)
  {
    CanInitCount++;
		if(CanInitCount>1000)
			break;
  }
   /* wait into intial mode */

  CAN0CTL1 = 0x80; /*open normal  mode,OSC source*/
  CAN0IDAC_IDAM0 = 1; /*two 16 bit acceptance filter*/
  CAN0IDMR0 = 0xff; /*not set filter*/
  CAN0IDMR1 = 0xff;
  CAN0IDMR2 = 0xff;
  CAN0IDMR3 = 0xff;
  CAN0IDMR4 = 0xff;
  CAN0IDMR5 = 0xff;
  CAN0IDMR6 = 0xff;
  CAN0IDMR7 = 0xff;
  CAN0BTR0 = 0x87; /*16M OSC,SJW is 3 Tq cycle,prescaler value is 8 bit rate is about 125k*/
  CAN0BTR1 = 0x32; /*one sample per bit,time segment 1 is 2 Tq and time segment 2 is 3 Tq*/

  CAN0CTL1_CLKSRC = 0; /* Select the clock source from crystal */
  CAN0CTL0_INITRQ = 0; /* Start device */
	CanInitCount=0;
  while (CAN0CTL1_INITAK)
	{
		CanInitCount++;
		if(CanInitCount>1000)
			break;
	}
   /* Wait for device initialization acknowledge */
	CanInitCount=0;
  while (!CAN0CTL0_SYNCH)
	{
		CanInitCount++;
		if(CanInitCount>1000)
			break;
	}
   /* Wait for synchronization */
  CAN0RFLG |= 254; /* Reset error flags */
  CAN0RIER = 125; /*only open interrupt,other error dialogic will be deal with query*/
}
/****************************************************************************************
 *****************************************************************************************/
static void CAN1Init(void)
{
	uint CanInitCount;
  CAN1CTL0_INITRQ = 1; /* request into initial mode */
	CanInitCount=0;
  while (!CAN1CTL1_INITAK)/* wait into intial mode */
  {
		CanInitCount++;
		if(CanInitCount>1000)
			break;
  }
  CAN1CTL1 = 0x80; /*open normal  mode,OSC source*/
  CAN1IDAC_IDAM = 1; /*two 16 bit acceptance filter*/
  CAN1IDMR0 = 0xff; /*not set filter*/
  CAN1IDMR1 = 0xff;
  CAN1IDMR2 = 0xff;
  CAN1IDMR3 = 0xff;
  CAN1IDMR4 = 0xff;
  CAN1IDMR5 = 0xff;
  CAN1IDMR6 = 0xff;
  CAN1IDMR7 = 0xff;
  CAN1BTR0 = 0x87; /*16M OSC,SJW is 3 Tq cycle,prescaler value is 8 bit rate is about 250k*/
  CAN1BTR1 = 0x32; /*one sample per bit,time segment 1 is 3 Tq and time segment 2 is 3 Tq*/

  CAN1CTL1_CLKSRC = 0; 				/* Select the clock source from crystal */

  CAN1CTL0_INITRQ = 0;       	/* Start device */
	CanInitCount=0;
  while(CAN1CTL1_INITAK)     /* Wait for device initialization acknowledge */
  {
		CanInitCount++;
		if(CanInitCount>1000)
			break;
  }		
	CanInitCount=0;
  while(!CAN1CTL0_SYNCH) 
	{
		CanInitCount++;
		if(CanInitCount>1000)
			break;
	}

  CAN1RFLG |= 254; /* Reset Receiver Flags*/
  CAN1RIER = 125; /* Enable Receive Buffer Full Interrupt*/
}

/**********************************
 ***********************************/
void sleep_mode(void)
{
  uint sleep_wait_time = 0;

  CAN0CTL0_WUPE = 1; /* Wake-Up Enable;must be set in front of entering sleep mode*/
  CAN0CTL0_SLPRQ = 1; /*request enter sleep mode*/
  while (!CAN0CTL1_SLPAK)/*wait sleep mode complete*/
  {
    sleep_wait_time++;
    if (sleep_wait_time >= 1000)
      break;
  }
  CAN0RIER_WUPIE = 1; /* Wake-up Interrupt Enable */
  CAN0RFLG_WUPIF = 1; /* Enable Wake-up Interrupt Flag */
}

/***********************************************************
funtion operation:initialization of adc
 **********************************************************/
static void AD_Init(void)
{
  ATDCTL2=0xc0;				 ///open AD power and close edge trige
	ATDCTL3=0x28;				 ///a queue change 5 times,no fifo mode 
	ATDCTL4=0xc1;				 ///10 bits precision，speed 770k(8m bus)
	ATDCTL5=0X00;

	ATDDIEN0 =0b11110110; ///AN8/AN11不是AD口，其他均是     
	ATDDIEN1 =0b11111111;	///AN0~AN7的AD口
}
/***********************************************************

 ************************************************************/
static void	Init_Capture_Funtion(void)
{
	MCFLG = 0b00000000;		///0~3bit->The first input capture has been caused by a falling edge.
	TCTL3 |= 0b00100000;		///first enable capture 6 in rising edge mode
	TC6 = 0;								///clear the capture holding ram	
	TIE |= 0b01000000;			///;;1-> enable interrupt,0-> disable interrupt
	TFLG1 |= 0b01000000; 	///clear timer interrupt flag
}
/*********************************************************

 *********************************************************/
static void init_timer(void)
{
	OCPD  = 255;
	///TFLG1 = 255;				
	
  TSCR2 = 0b00000111;				///enable timer overflow interrupt,128 Prescale Factor
  TIOS  = 0b10111111;				//channel 0,1,2,3 act as o/c;else act as i/c

  TC0   = 0x100;
  TC1   = 0x100;
  TC2   = 0x100;
  TC3   = 0x100;    
  TC4   = 0X100;           
	TC5   = 0X100;
	///TC6   = 41650;
	TC7   = 0x100;

	TIE   = 0b10000000;	///enable 7 channel
  TSCR1 = 0x80;				///Enable timer freeze mode 
}
/***********************************************************

 ************************************************************/
static void init_motor(void)
{ 
	unsigned char i;

  MCCTL0 = 0x0;               ///fbus(Eclock)=0, FAST=0 (slow mode)
  MCCTL1 = 0x80;             	///RECIRC =1 (low side), disable interrupt for PWM motor
  MCPER = 0x80;             	///0x80=128 resoultion frequency = fbus    
 
  for(i=0;i<12;i++)          ///Dual Full H-bridge mode,left aligned
  	*(&MCCC0+i)=0xd0;
  for(i=0;i<12;i++)						///Duty cycle channel 0 for micro-stepping initialization
  	*(&MCDC0+i)=0x00;
  for(i=0;i<6;i++)            
	{
  	MotorFinalStep[i]=0;
  	MotorCurStep[i]=0;
  	if(i==1)
  		MotorMaxStep[1] = PRESMAXSTEP;  //motor end limit
  	else if(i==2)
  		MotorMaxStep[2]= TEMPMAXSTEP;
  	else if(i==3)
  		MotorMaxStep[3]= OILMAXSTEP;
  	else if(i==5)
  		MotorMaxStep[5]= TACHOMAXSTEP;
		else if(i==4)
  		MotorMaxStep[4]= CARSPEEDMAXSTEP;  
		else if(i==0)
  		MotorMaxStep[0]= PRES2MAXSTEP;
  	MotorMinStep[i] = 0;              //motor head limit
  }    
}

/******************************************************************************************
 *******************************************************************************************/
static void IrqOpen(void)
{
  IRQCR = 0xc0; //Enable extern interrupt
}

/******************************************************************************************
 *******************************************************************************************/
static void IrqClose(void)
{
  IRQCR = 0x00; //Disable extern interrupt
}

/******************************************************************************************
 *******************************************************************************************/
static void XirqOpen(void)
{
  asm ANDCC# 0xBF;	///Enable Xirq
}

/******************************************************************************************
 *******************************************************************************************/
static void SetupXGATE(void)
{
   //initialize the XGATE vector block and set the XGVBR register to its start address 
  XGVBR= (unsigned int)(void*__far)(XGATE_VectorTable - XGATE_VECTOR_OFFSET);

  /* switch software trigger 0 interrupt to XGATE */
  //ROUTE_INTERRUPT(SOFTWARETRIGGER0_VEC, 0x81); /* RQST=1 and PRIO=1 */

  //switch software timer 0 interrupt to XGATE
  ROUTE_INTERRUPT(tchannel0, 0x86);            //RQST=1 and PRIO=1 
  ROUTE_INTERRUPT(tchannel1, 0x86);            //RQST=1 and PRIO=1 
  ROUTE_INTERRUPT(tchannel2, 0x86);            //RQST=1 and PRIO=1 
  ROUTE_INTERRUPT(tchannel3, 0x86);            //RQST=1 and PRIO=1 
  ROUTE_INTERRUPT(tchannel4, 0x87);            //RQST=1 and PRIO=1 
  ROUTE_INTERRUPT(tchannel5, 0x87);            //RQST=1 and PRIO=1 
  ROUTE_INTERRUPT(Capture6_Int_VEC, 0x87); 		 //RQST=1 and PRIO=1  
  XGMCTL= 0xFBC1; //enable XGATE mode and interrupts XGE | XGFRZ | XGIE  
  //XGSWT= 0x0101;
}

/******************************************************************************************
 *******************************************************************************************/
static void Vector_Init(void)
{
  IVBR = 0XDF;
}
/******************************************************************************************
funtion operation:initialization of pwm
*******************************************************************************************/
static void pwm_Init(void)
{
  PWMCTL |= 0b01000000;	///6bit->->设为1是将PWM端口4与5结合为16bit pwm；
  #if		J13_OUT==1
  	PWMCTL |= 0b00100000;	///5bit->设为1是将PWM端口2与3结合为16bit pwm;
  #endif
	PWMPRCLK = 0x07; /*6~4bit->B = clock prescale is 8;2~0bit->A =clock prescale */
	PWMCLK=0b00000000;	/*Clock SB is the clock source for PWM channel*/
	///channel 4/5
  PWMPOL_PPOL5 = 1; /*PWM POL ENALBE,outputs are high at the beginning of the period*/
	PWMCAE_CAE5 = 0; /* left aligned Edge Output Mode on channel 6 */
	PWMPER45 = 0xfffe; /*peoriods of the channel pwm,first set to high all*/
	PWMDTY45	= 0xffff;/*PWM Channel 6 Duty*/
	PWME_PWME5 = 1; /*PWM CHANNEL ENABLE*/
	#if		J13_OUT==1
		///channel 2/3
	  PWMPOL_PPOL3 = 1; /*PWM POL ENALBE,outputs are high at the beginning of the period*/
		PWMCAE_CAE3 = 0; /* left aligned Edge Output Mode on channel 6 */
		PWMPER23 = 0xfffe; /*peoriods of the channel pwm,first set to high all*/
		PWMDTY23	= 0xffff;/*PWM Channel 6 Duty*/
		PWME_PWME3 = 1; /*PWM CHANNEL ENABLE*/
	#endif
	
	PWMPER1 = 0x35; ///设置蜂鸣器PWM周期
	PWMDTY1 = 0x35; ///PWM占空比
	PWME_PWME1 = 1;	///使能通道

	PWMPER6 = 0xd0; ///设置右边背光的PWM周期
	PWMDTY6 = 0xd0; ///PWM占空比
	PWME_PWME6 = 1;	///使能通道

	PWMPER7 = 0xd0; ///设置左边背光的PWM周期
	PWMDTY7 = 0xd0; ///PWM占空比
	PWME_PWME7 = 1;	///使能通道
	
}
#pragma CODE_SEG DEFAULT

