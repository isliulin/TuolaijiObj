/*
 * File:        sysinit.c
 * Purpose:     Kinetis Configuration
 *              Initializes processor to a default state
 *
 * Notes:
 *
 */

#include "common.h"
#include "sysinit.h"
#include "uart.h"
#include "includes.h"
#include "BacklightControl.h"
#include "hwMacroPatch.h"
#include "S32K144.h"
#include "TJA1145.h"
#include "systick.h"


uint32_t bus_clk_khz = 0;


void RamInit(void)
{
    UINT8 i;
    RainScratchChangeFlag = 1;
    USR_s_OV.SleepCodeBuf = WakeCode;
    ID_require_flag=1;
    SetMileageFlag = 0;
    PwmLoadOutType = SysConfigData.pwm_load_out;
    if(SysConfigData.post==0x01)Sys_Post_Flag = 1; 
    J1939IdNum =  testJ1939IdNum();
    for(i=0;i<J1939IdNum;i++)  J1939DetectCnt[i].dat = 0x80;
    
    
   J1939EngSpeedIndex =  J1939_OFFSET_BYTE+J1939_STATUS_NUMBER+J1939_CHAR_NUMBER;
   SpeedRateTemp = SysConfigData.SpeedRate;//L:仪表没有输入捕捉 CaptureSpeed();	
   CycleCaptureNumTemp = SysConfigData.CycleCaptureNum;//仪表没有输入捕捉 CaptureSpeed();


   clearData((UINT8 *)(&USR_s_OV),sizeof(USR_s_OV));   
   clearData((UINT8 *)(&USR_LOAD_OV),sizeof(USR_LOAD_OV));   

   
    clearData(&USR_s_OV.FrontDriveKey[0].Data,sizeof(USR_s_OV.FrontDriveKey));
    clearData(&USR_s_OV.FrontDriverVirtualKey[0].Data,sizeof(USR_s_OV.FrontDriverVirtualKey));
    clearData(&USR_s_OV.FrontDriveLoad[0].Data,sizeof(USR_s_OV.FrontDriveLoad));
    clearData(&USR_LOAD_OV.FrontDriveLoad[0].Data,sizeof(USR_LOAD_OV.FrontDriveLoad)); 
    clearIntData(&InsideData[FRONT_INSIDE_OFFSET],(BACK_INSIDE_OFFSET-FRONT_INSIDE_OFFSET));
    
    clearData(&USR_s_OV.BackDriveKey[0].Data,sizeof(USR_s_OV.BackDriveKey));
    clearData(&USR_s_OV.BackDriverVirtualKey[0].Data,sizeof(USR_s_OV.BackDriverVirtualKey));
    clearData(&USR_s_OV.BackDriveLoad[0].Data,sizeof(USR_s_OV.BackDriveLoad));
    clearData(&USR_LOAD_OV.BackDriveLoad[0].Data,sizeof(USR_LOAD_OV.BackDriveLoad)); 
    clearIntData(&InsideData[BACK_INSIDE_OFFSET],(MIDDLE_INSIDE_OFFSET-BACK_INSIDE_OFFSET));
    
    clearData(&USR_s_OV.MiddleDriveKey[0].Data,sizeof(USR_s_OV.MiddleDriveKey));
    clearData(&USR_s_OV.MiddleDriverVirtualKey[0].Data,sizeof(USR_s_OV.MiddleDriverVirtualKey));
    clearData(&USR_s_OV.MiddleDriveLoad[0].Data,sizeof(USR_s_OV.MiddleDriveLoad));
    clearData(&USR_LOAD_OV.MiddleDriveLoad[0].Data,sizeof(USR_LOAD_OV.MiddleDriveLoad));
    clearIntData(&InsideData[MIDDLE_INSIDE_OFFSET],(TOP_INSIDE_OFFSET-MIDDLE_INSIDE_OFFSET));
    
    clearData(&USR_s_OV.TopDriveKey[0].Data,sizeof(USR_s_OV.TopDriveKey));
    clearData(&USR_s_OV.TopDriverVirtualKey[0].Data,sizeof(USR_s_OV.TopDriverVirtualKey));
    clearData(&USR_s_OV.TopDriveLoad[0].Data,sizeof(USR_s_OV.TopDriveLoad));
    clearData(&USR_LOAD_OV.TopDriveLoad[0].Data,sizeof(USR_LOAD_OV.TopDriveLoad));
    clearIntData(&InsideData[TOP_INSIDE_OFFSET],(POWER_INSIDE_OFFSET-TOP_INSIDE_OFFSET));
    
     clearData(&USR_s_OV.PowerLoadState[0].Data,sizeof(USR_s_OV.PowerLoadState));
     clearData(&USR_LOAD_OV.PowerLoadState[0].Data,sizeof(USR_LOAD_OV.PowerLoadState));
     clearData(&USR_s_OV.PowerFuse[0].Data,sizeof(USR_s_OV.PowerFuse));
     
     LeftTurnSignalWarn  = (IcoWarnConfig.DashboardIcoWarn[28] >>4)&0x08;
     RightTurnSignalWarn = (IcoWarnConfig.DashboardIcoWarn[36] >>4)&0x08;
     
     wheel_key[0] = 0;   wheel_key[1] = 0;

}


//系统时钟初始化
void SCG_Init(void)
{
	SCG_SOSCCFG = SCG_SOSCCFG_SC16P(1)|SCG_SOSCCFG_RANGE(3)|SCG_SOSCCFG_HGO(0)|SCG_SOSCCFG_EREFS(1);
	SCG_SOSCDIV = SCG_SOSCDIV_SOSCDIV3(0)|SCG_SOSCDIV_SOSCDIV2(0)|SCG_SOSCDIV_SOSCDIV1(0);
	SCG_SOSCCSR |= SCG_SOSCCSR_SOSCEN_MASK|SCG_SOSCCSR_SOSCERCLKEN_MASK;
	SCG_SPLLCSR =0;
	SCG_SPLLCFG = SCG_SPLLCFG_MULT(0x04)|SCG_SPLLCFG_PREDIV(2)|SCG_SPLLCFG_SOURCE(0);
	//SCG_SPLLCFG = SCG_SPLLCFG_MULT(0x00)|SCG_SPLLCFG_PREDIV(7)|SCG_SPLLCFG_SOURCE(0);
	//SCG_SPLLCFG = SCG_SPLLCFG_MULT(0x04)|SCG_SPLLCFG_PREDIV(0)|SCG_SPLLCFG_SOURCE(0);//8*40/1	from OSC
    //SCG_SPLLCFG = SCG_SPLLCFG_MULT(0x1f)|SCG_SPLLCFG_PREDIV(0x07)|SCG_SPLLCFG_SOURCE(0);// 27M晶振         8*40/1	from OSC
	SCG_SPLLCFG = SCG_SPLLCFG_MULT(0x04)|SCG_SPLLCFG_PREDIV(0x05)|SCG_SPLLCFG_SOURCE(1);//内部时钟
	SCG_SPLLDIV = SCG_SPLLDIV_SPLLDIV3(1)|SCG_SPLLDIV_SPLLDIV2(1)|SCG_SPLLDIV_SPLLDIV1(1);
	SCG_SPLLCSR |= SCG_SPLLCSR_SPLLEN_MASK;//Enble PLL
	
	//SCG_CLKOUTCNFG = SCG_CLKOUTCNFG_CLKOUTSEL(6);//select Clkout
	
	SCG_FIRCDIV = SCG_FIRCDIV_FIRCDIV3(1)|SCG_FIRCDIV_FIRCDIV2(1)|SCG_FIRCDIV_FIRCDIV1(1);
	SCG_RCCR = SCG_RCCR_SCS(6)|SCG_RCCR_DIVCORE(0)|SCG_RCCR_DIVBUS(1)|SCG_RCCR_DIVSLOW(3);//select PLL,BUS/2,FLSAH/4
	//SCG_RCCR = SCG_RCCR_SCS(3)|SCG_RCCR_DIVCORE(0)|SCG_RCCR_DIVBUS(0)|SCG_RCCR_DIVSLOW(1);
	//SCG_HCCR = SCG_RCCR_SCS(6)|SCG_RCCR_DIVCORE(1)|SCG_RCCR_DIVBUS(1)|SCG_RCCR_DIVSLOW(1);
}

void SCG_Init8M(void)
{
	SCG_RCCR = SCG_RCCR_SCS(3)|SCG_RCCR_DIVCORE(0)|SCG_RCCR_DIVBUS(0)|SCG_RCCR_DIVSLOW(1);
	SCG_SPLLCSR =0;//|= SCG_SPLLCSR_SPLLEN_MASK;//Enble PLL
	SCG_SPLLCFG = SCG_SPLLCFG_MULT(0x00)|SCG_SPLLCFG_PREDIV(7)|SCG_SPLLCFG_SOURCE(0);
	SCG_SPLLDIV = SCG_SPLLDIV_SPLLDIV3(1)|SCG_SPLLDIV_SPLLDIV2(1)|SCG_SPLLDIV_SPLLDIV1(1);
	SCG_SPLLCSR |= SCG_SPLLCSR_SPLLEN_MASK;//Enble PLL
	
	SCG_RCCR = SCG_RCCR_SCS(6)|SCG_RCCR_DIVCORE(2)|SCG_RCCR_DIVBUS(2)|SCG_RCCR_DIVSLOW(2);//select PLL,BUS/2,FLSAH/4
	
}

void SCG_Init80M(void)
{
	SCG_RCCR = SCG_RCCR_SCS(3)|SCG_RCCR_DIVCORE(0)|SCG_RCCR_DIVBUS(0)|SCG_RCCR_DIVSLOW(1);
	SCG_SPLLCSR =0;//|= SCG_SPLLCSR_SPLLEN_MASK;//Enble PLL
	SCG_SPLLCFG = SCG_SPLLCFG_MULT(0x04)|SCG_SPLLCFG_PREDIV(2)|SCG_SPLLCFG_SOURCE(0);
	SCG_SPLLDIV = SCG_SPLLDIV_SPLLDIV3(1)|SCG_SPLLDIV_SPLLDIV2(1)|SCG_SPLLDIV_SPLLDIV1(1);
	SCG_SPLLCSR |= SCG_SPLLCSR_SPLLEN_MASK;//Enble PLL
	
	SCG_RCCR = SCG_RCCR_SCS(6)|SCG_RCCR_DIVCORE(0)|SCG_RCCR_DIVBUS(1)|SCG_RCCR_DIVSLOW(3);//select PLL,BUS/2,FLSAH/4
	
}


void freePort(void)
{
      	PCC_PORTA |= PCC_PORTA_CGC_MASK;
	PCC_PORTB |= PCC_PORTB_CGC_MASK;
	PCC_PORTC |= PCC_PORTC_CGC_MASK;
	PCC_PORTD |= PCC_PORTD_CGC_MASK;
        PCC_PORTE |= PCC_PORTE_CGC_MASK;

//MCU电源 深度休眠
	MCU_5VEN_MUX();MCU_5VEN_PDDR_O;//5V电源
	MCU_5VEN_H;
//看门狗初始化        
        WDT_MUX();WDT_PDDR();
 //can         
        CAN0_RS_MUX();CAN0_RS_PDDR_OUT;CAN0_RS_L;
	CAN1_RS_MUX();CAN1_RS_PDDR_OUT;CAN1_RS_L;
	CAN2_RS_MUX();CAN2_RS_PDDR_OUT;CAN2_RS_L;
//电源控制     还有其他的
	V_POWER_V3_MUX();V_POWER_V3_PDDR_O;
	V_POWER_V1_MUX();V_POWER_V1_PDDR_O;
	
  	POWER_12V_MUX();POWER_12V_OUT; 	POWER_12V_OFF ;
	POWER_12VV_MUX();POWER_12VV_OUT ;POWER_12VV_L;
	TV_POWER_MUX(); TV_POWER_OUT;TV_POWER_L; //摄像头电源
	
        AN_POW_MUX();AN_POW_PDDR_O;AN_POW_L;   
        AN1_POW_MUX();AN1_POW_PDDR_O;AN1_POW_L;   
 //蜂鸣器声音大小控制       
          LEVEL1_MUX(); LEVEL1_PDDR_O; LEVEL1_L;  
	  LEVEL2_MUX(); LEVEL2_PDDR_O; LEVEL2_L;    
	  LEVEL3_MUX(); LEVEL3_PDDR_O; LEVEL3_L;  

	  PWMOUT1_PORT_MUX();
         PWMOUT1_PDDR_O;
	  PWMOUT1_L;
 //捕获通道
         PWMIN_CPSR_PORT_MUX();
         PWMIN_CPRPM_PORT_MUX();
	 
       

}
void sysInit(void)
{	
  	DisableInterrupts;
	//系统时钟
	SCG_Init();
	bus_clk_khz = 40000;
        
	freePort();
	FeedWatchDog();
	lpit0Init();//lpit定时中断
	canInit(0,250,0,0) ;//can0
	CANEnableRXBuff(0,CAN0_REV_IMB);//使能接收缓冲区
	EnableCANInterrupt(0,CAN0_REV_IMB);
	canInit(1,250,0,0) ;//can1
	CANEnableRXBuff(1,CAN1_REV_IMB);//使能接收缓冲区
	EnableCANInterrupt(1,CAN1_REV_IMB);
	canInit(2,250,0,0) ;
	CANEnableRXBuff(2,CAN2_REV_IMB);//使能接收缓冲区
	EnableCANInterrupt(2,CAN2_REV_IMB);
	CanMsgOutInit();
	hw_adc_init(0);
	hw_adc_init(1);
	initHc4051();
	ledInit();
	initKeyPort();//按键IO口初始化
	TW88XX_Power(0);
	FeedWatchDog();
	init_motor();//指针初始化
	back0_motor();
	
	SOUND_POW_PORT_MUX();
	FTM_PWM_init(SOUND_POW_MODULE, SOUND_POW_CHANNL, SOUND_FRQ, 50);//music-Pow

	initModuleDetectParameter();
        initDashboardLoadOutputPin();
	keyDeal_dataInit();
	initSpeedBuf();
	initLoadsParameter();
	initSensorData();
	RamInit();

	//PowerOffSave();
        IS31FL_IO_MACROS();//测试
	InitDS1302();
	initEepromData();
	initDisParameter();
	ModuleConfigTemp = SysConfigData.ModuleConfig;
	SetDashboardVersion();
	FeedWatchDog();

         LPLD_FTM_IC_Init(FTM3 ,7);
         LPLD_FTM_IC_Enable(FTM3, CH5, ALIGN_LEFT);
         LPLD_FTM_IC_Enable(FTM3, CH6, ALIGN_LEFT);
         enable_irq (FTM3_IRQn);
	  PWMOUT2_PORT_MUX();
	  PWMOUT1_PORT_MUX();
        lptmr_init();
	SysTick_Init();
	EnableInterrupts;
}