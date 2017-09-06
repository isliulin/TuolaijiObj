#define		LOADDEAL_C
#include	"config.h"
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
#pragma CODE_SEG  __NEAR_SEG	NON_BANKED
interrupt  void  tchanel7_Handler(void) //14
{
  if((bootloader_wait_flag==0)&&(boot_reset_flag==0)&&((menu_id!=LogoMenuNum)||(OnKeyBit!=KeyOn)))
  {
  	FlashLampDeal();        //check flash status and make the dobule led wink
		
  	if((AccKeyBit==KeyOn)||(OnKeyBit==KeyOn)||(DangerWarnKeyBit==KeyOn/*(LeftTurnLampKeyBit==KeyOn)&&(RightTurnLampKeyBit==KeyOn)*/))
  	{
    	buzzer_cancel();
    	buzzer_control();

  		delayxms++;
  		if(delayxms>=dirdelay) 				////flash delay 
  		{		   
	  		delayxms_flag=1;
  		}

   		buzzer_ms++;
   		if(buzzer_ms>=buzzer_delay)		////buzzer delay control
   		{       
   	 		buzzer_ms=0;
    		buzzer_ms_flag=1;
   		}
  	}
  	else 
  	{
    	buzzer_flag=0;
			buzzer_ms_flag=0;
			BuzzerOff;
			delayxms_flag=0;
			buzzer_ms=0;
  	}
 	}
	TFLG1 = 0x80; 
	TC7 += 12500/16;             
}
#pragma CODE_SEG USER_ROM
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void Load_Deal(void)
{
	ErrorReset();
	
	StTest();
	
	if(PowerDeviceChangeData()==1);
		PowerDeviceDrive(1);

	LedDisplay();
	///SendMeterLedProcess();

}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void FlashLampDeal(void)
{
	unsigned char LeftFrontTurnLamp=0,RightFrontTurnLamp=0;
	unsigned char LeftBackTurnLamp=0,RightBackTurnLamp=0;
	unsigned char LeftTopTurnLamp=0,RightTopTurnLamp=0;
	/*change recieve load state*/
	LeftFrontTurnLamp		=	FrontDriveLoad[0].St.S3;///Load 4
	RightFrontTurnLamp	=	FrontDriveLoad[1].St.S0;///Load 5
		
	LeftBackTurnLamp		=	BackDriveLoad[1].St.S0;///Load 5
	RightBackTurnLamp		=	BackDriveLoad[1].St.S1;///Load 6
			
//	LeftTopTurnLamp			=	TopDriveLoad[1].St.S2;///Load 7
//	RightTopTurnLamp		=	TopDriveLoad[1].St.S3;///Load 8

  switch(FlashIndex)
  {
    case 0:
      if(DangerWarnKeyBit==KeyOn/*(LeftTurnLampKeyBit==KeyOn)&&(RightTurnLampKeyBit==KeyOn)*/)
      {      
        if((LeftFrontTurnLamp==Load_Short||RightFrontTurnLamp==Load_Short)
        ||(LeftBackTurnLamp==Load_Short||RightBackTurnLamp==Load_Short)
        ||(LeftTopTurnLamp==Load_Short||RightTopTurnLamp==Load_Short)
        ||(FrontDriveErrorFlag==1)||(BackDriveErrorFlag==1)||(PowerErrorFlag==1)||(PowerContrlErrorFlag==1)/*||(TopDriveErrorFlag==1)*/)
          dirdelay=HighSpeedFlashDelay; ///if error state will high speed flash                                       
        else
          dirdelay=LowSpeedFlashDelay;	///if normal state will low speed flash 
				SleepCodeBuf=WakeCode;
        FlashCodeBuf=AllFlashOpenCode;   
        SendCan0Message(SendSleepFlash); 
        FlashIndex=1;       
      }
      else if(LeftTurnLampKeyBit==KeyOn)
      {		       																					 
        if((LeftFrontTurnLamp==Load_Short)||(LeftBackTurnLamp==Load_Short)||(LeftTopTurnLamp==Load_Short)     
        ||(FrontDriveErrorFlag==1)||(BackDriveErrorFlag==1)||(PowerErrorFlag==1)||(PowerContrlErrorFlag==1)/*||(TopDriveErrorFlag==1)*/)
          dirdelay=HighSpeedFlashDelay;
        else
          dirdelay=LowSpeedFlashDelay;
				SleepCodeBuf=WakeCode;
        FlashCodeBuf=LeftFlashOpenCode;  
        SendCan0Message(SendSleepFlash); 
        FlashIndex=1;      
      }
      else if(RightTurnLampKeyBit==KeyOn)
      {
        if((RightFrontTurnLamp==Load_Short)||(RightBackTurnLamp==Load_Short)||(RightTopTurnLamp==Load_Short)
        ||(FrontDriveErrorFlag==1)||(BackDriveErrorFlag==1)||(PowerErrorFlag==1)||(PowerContrlErrorFlag==1)/*||(TopDriveErrorFlag==1)*/)
          dirdelay=HighSpeedFlashDelay;
        else                                        										
          dirdelay=LowSpeedFlashDelay;	
        SleepCodeBuf=WakeCode;
        FlashCodeBuf=RightFlashOpenCode;  
        SendCan0Message(SendSleepFlash);    
        FlashIndex=1;    
      }  
      delayxms=0;
      delayxms_flag=0;   
      break;
    case 1:
      if(delayxms_flag)
      {
      	SleepCodeBuf=WakeCode;
        FlashCodeBuf=AllFlashCloseCode;  
        SendCan0Message(SendSleepFlash);
        delayxms=0;
        delayxms_flag=0;
        FlashIndex=2;  
      }
      break;
    case 2:      
     	if(delayxms_flag)
			{
      	FlashIndex=0;  
     	}
      break;
    default:
      FlashIndex=0;
      break;
  	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void buzzer_control(void)
{
  switch(buzzer_index)
  {
    case 0:
			if(DangerWarnKeyBit==KeyOn/*(LeftTurnLampKeyBit==KeyOn)&&(RightTurnLampKeyBit==KeyOn)*/)  
      {
      	if(FlashCodeBuf==AllFlashOpenCode)							/*control one of side to light will be flash*/
          buzzer_flag=1;
         else if(FlashCodeBuf==AllFlashCloseCode)
          buzzer_flag=0;
      }	        
      else if((SecondKindFlag==1)&&(OnKeyBit==KeyOn))    		//second kind error will sound 100%
        buzzer_flag=1;
      else if((LeftTurnLampKeyBit==KeyOn)&&(OnKeyBit==KeyOn))
      { 
        if(FlashCodeBuf==LeftFlashOpenCode||FlashCodeBuf==AllFlashOpenCode)
          buzzer_flag=1;
        else if(FlashCodeBuf==AllFlashCloseCode) 
          buzzer_flag=0;
      }
      else if((RightTurnLampKeyBit==KeyOn)&&(OnKeyBit==KeyOn))//turning work will sound 50% in turning lamp
      {       
        if(FlashCodeBuf==RightFlashOpenCode||FlashCodeBuf==AllFlashOpenCode)
          buzzer_flag=1;
        else if(FlashCodeBuf==AllFlashCloseCode)
          buzzer_flag=0;
      }
	  	else if(((threeKindFlag==1)||(FirstKindFlag==1))&&(OnKeyBit==KeyOn))  //over speed buzzer
	  	{	    		
        buzzer_flag=1;
        buzzer_delay=ShortNoiseDelay;
        buzzer_index=1;
     	}
     	else      
     	{
      	buzzer_flag=0;
     	}
	    
      buzzer_ms=0;			     		
      buzzer_ms_flag=0;
      break;
    case 1:
      if(buzzer_ms_flag)
      {     
				buzzer_delay=ShortNoiseDelay;
        buzzer_flag=0;
        buzzer_ms=0;
        buzzer_ms_flag=0;
        buzzer_index=2;  
      }
      break;
    case 2:
      if(buzzer_ms_flag)
	  	{
        buzzer_index=0;
      }
      break;
    default:
      buzzer_index=0;
      break;
  }
  if((AccKeyBit==KeyOn)||(OnKeyBit==KeyOn))
  {   
  	if(buzzer_flag==1||BuzzerLevelSetFlag==1)             
  		SetBuzzer;
  	else
  		BuzzerOff;
  }
  else
  {		
   	if(DangerWarnKeyBit==KeyOn/*(LeftTurnLampKeyBit==KeyOn)&&(RightTurnLampKeyBit==KeyOn)*/)
   	{
   		if(buzzer_flag==1)            
  		 	SetBuzzer;
  		else
  		 	BuzzerOff;
   	}
   	else
   		BuzzerOff;
  }
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void buzzer_cancel(void)
{
	unsigned char i;
	
  if(BuzzerCancelFlag==1)
  {
    BuzzerCancelFlag=0;
    for(i=0;i<SECOND_ERROR_NUM;i++)
			Second_Error[i]=0x00;
		for(i=0;i<sizeof(SpecialSecond_Error);i++)
			SpecialSecond_Error[i]=0x00;
		for(i=0;i<MODULE_CONNECT_NUM;i++)
			First_Error[i]=0x00;
		for(i=0;i<sizeof(SpecialThird_Error);i++)
			SpecialThird_Error[i]=0x00;
  }
	
  FirstKindFlag=0;
	for(i=0;i<MODULE_CONNECT_NUM;i++)
	{
		if(First_Error[i]!=0x00)
			FirstKindFlag=1;///module error warning
	}  	
	
  SecondKindFlag=0;
  for(i=0;i<SECOND_ERROR_NUM;i++)
	{
		if(Second_Error[i]!=0x00)
			SecondKindFlag=1;
	}  	
  for(i=0;i<sizeof(SpecialSecond_Error);i++)
  {
		if(SpecialSecond_Error[i]!=0x00)
			SecondKindFlag=1;
  }
	
  threeKindFlag=0;
  for(i=0;i<sizeof(SpecialThird_Error);i++)
  {
		if(SpecialThird_Error[i]!=0x00)
			threeKindFlag=1;
  }
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void	Cat4016_Drive(unsigned	int	*Cat4016_Data)
{
	unsigned char	i;
	unsigned int LedTempData=0;
	Cat4016_Latch_IO=1;
	Cat4016_Sin_IO=1;
	Cat4016_Clk_IO=1;	
	Cat4016_Blank_IO=1;
	
	Cat4016_Blank=0;						///enable CAT4016

	LedTempData=*Cat4016_Data;
	Cat4016_Latch=0;
  for(i=0;i<16;i++)
	{
		if(LedTempData&0x0001)	
			Cat4016_Sin=1;
		else
			Cat4016_Sin=0;
		
		Cat4016_Clk=0;    		///load the data in rising edge 
  	asm nop;                                           
  	asm nop;
   	Cat4016_Clk=1;
  	asm nop;                                           
  	asm nop;
		LedTempData>>=1;
	}
  Cat4016_Latch=1;    		///lock the data in falling edge 
  asm nop;                                           
  asm nop;
  Cat4016_Latch=0;
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void LedDisplay(void)
{ 
	uchar i,TempDataA=0;
	unsigned char Temp=0;

	OvercarspeedLEDIO  =1;
	OverTachospeedLEDIO=1;

	FrontDoorLedIO=1;
	MiddleDoorLedIO=1;
	
	Cat4016_Data[0].Data=0x0000;				 ///first close all led,then open what I need
	if(DangerWarnKeyBit==KeyOn/*(LeftTurnLampKeyBit==KeyOn)&&(RightTurnLampKeyBit==KeyOn)*/)	
	{
		if(FlashCodeBuf==AllFlashOpenCode)
		{
			MLed7=1;						///turn left
			MLed10=1;						///turn right
		}
	}	      
	if(AccKeyBit==KeyOn)///power on led
		MLed15=1;
	if(BackRoomKeyBit==KeyOff)
		MLed2=1;
	if(FrontDoorOpenSignalKeyBit==KeyOn)
		FrontDoorLed=1;
	else
		FrontDoorLed=0;
	if(MiddleDoorOpenSignalKeyBit==KeyOn)
		MiddleDoorLed=1;
	else
		MiddleDoorLed=0;
	if(OnKeyBit==KeyOn)
	{
		if(LeftTurnLampKeyBit==KeyOn)
		{
			if(FlashCodeBuf==LeftFlashOpenCode||FlashCodeBuf==AllFlashOpenCode)
				MLed7=1;                    
		}
		if(RightTurnLampKeyBit==KeyOn)
		{
			if(FlashCodeBuf==RightFlashOpenCode||FlashCodeBuf==AllFlashOpenCode)
				MLed10=1;                   
		}
	}
	if(OnKeyBit==KeyOff)
	{
		OverTachospeedLED = 0;				
		OvercarspeedLED = 0;					
		MotorLedLeftOff;
		MotorLedRightOff;
		goto _LedDrive;					 ///except turning,other are shelves
	}		      
	if(HandStopKeyBit==KeyOn)
		MLed3=1;     
	if((LocationLampKeyBit==KeyOn)&&(FrontFogKeyBit==KeyOn))
		MLed14=1;
	if((LocationLampKeyBit==KeyOn)&&(FrontFogKeyBit==KeyOn)&&(BackFogKeyBit==KeyOn))
		MLed12=1;
	if(ABSMulfunctionIndicateKeyBit==KeyOn)
		MLed6=1;
	
	if(OilNumberData<OilLowNum)
		MLed8=1;
	

	///黄色报警图标
	Temp=0;
	for(i=0;i<FIRST_ERROR_NUM;i++)
	{
		if(SmallError[i]!=0)
		{
			Temp=1;
			break;
		}			
	}
	if(Temp==1)
		MLed13=1;
	///红色报警图标
	Temp=0;
	for(i=0;i<sizeof(First_ErrorOld);i++)
	{
		if(First_ErrorOld[i]!=0)
		{
			Temp=1;
			break;
		}
	}
	for(i=0;i<sizeof(Second_ErrorOld);i++)
	{
		if(Second_ErrorOld[i]!=0)
		{
			Temp=1;
			break;
		}
	}
	if(Temp!=0)
		MLed16=1;
	
	if(LocationLampKeyBit==KeyOn||LightLevelSetFlag==1)		     
	{
		MotorLedLeftOn;
		MotorLedRightOn;
	}
	else
	{
		MotorLedLeftOff;
		MotorLedRightOff;
	}	
			
	if(FarLampKeyBit==KeyOn&&NearLampKeyBit==KeyOn)
	{
		MLed11=1;   
	}
	else if(FarLampKeyBit==KeyOn)
	{
		MLed9=1;   
	}
	
	if(OvercarspeedFlag==1)			       
		OvercarspeedLED = 1;				
	else
		OvercarspeedLED = 0;	
	if(OvertachospeedFlag==1)		
		OverTachospeedLED = 1;				
	else
		OverTachospeedLED = 0;

	if(ENGINE_ECU_CANErrorFlag==0)
	{
		if(WaterTemperatureFlag==1)
			MLed5=1;
	}
	else					
	{
		if(WaterTempFlahTime>=WaterTempFlahNum)
		{
			WaterTempFlahTime=0;
			WaterTempFlahFlag=~WaterTempFlahFlag;
		}
		if(WaterTempFlahFlag==0||EngineWaterWarnKeyBit==KeyOn)
			MLed5=1;	
	}
	

_LedDrive:
	TempDataA=0;	
	if(SleepFlag==0)						
	{				
		for(i=0;i<NUM_CAT4016;i++)			//send meter led process
		{
			if(Cat4016_Data[i].Data!=Old_Cat4016_Data[i].Data)
			{
				SendMeterLedFlag=1;
				Old_Cat4016_Data[i].Data=Cat4016_Data[i].Data;
				TempDataA=1;
			}
		}
		if(TempDataA==1)
		{
			Cat4016_Drive(&Cat4016_Data[0].Data);
		}
	}
}		
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void SendMeterLedProcess(void)
{
	if(OnKeyBit==KeyOff)
		return;
	if(MLed14 == 1)
		SendMeterLedData[0].LedLoads.LedLoad0 = 1;
	else
		SendMeterLedData[0].LedLoads.LedLoad0 = 0;
	
	if(MLed12 == 1)
		SendMeterLedData[0].LedLoads.LedLoad1= 1;
	else
		SendMeterLedData[0].LedLoads.LedLoad1= 0;	
	
	if(MLed11== 1)
		SendMeterLedData[0].LedLoads.LedLoad2 = 1;
	else
		SendMeterLedData[0].LedLoads.LedLoad2 = 0;	

//if(MLed14 == 1)				///发动机故障指示灯
//SendMeterLedData[0].LedLoads.LedLoad3 = 1;
//else
		SendMeterLedData[0].LedLoads.LedLoad3 = 0;	

	if(FrontAirPressWarnKeyBit==KeyOn)
		SendMeterLedData[0].LedLoads.LedLoad4 = 1;
	else
		SendMeterLedData[0].LedLoads.LedLoad4 = 0;

	if(BackAirPressWarnKeyBit==KeyOn)	
		SendMeterLedData[0].LedLoads.LedLoad5 = 1;
	else
		SendMeterLedData[0].LedLoads.LedLoad5 = 0;	
	
	if(MLed6 == 1)
		SendMeterLedData[0].LedLoads.LedLoad6 = 1;
	else
		SendMeterLedData[0].LedLoads.LedLoad6 = 0;	

	if(MLed3 == 1)
		SendMeterLedData[0].LedLoads.LedLoad7 = 1;
	else
		SendMeterLedData[0].LedLoads.LedLoad7 = 0;		

	if(MLed9 == 1)
		SendMeterLedData[1].LedLoads.LedLoad0 = 1;
	else
		SendMeterLedData[1].LedLoads.LedLoad0 = 0;		

	if(MLed7 == 1)
		SendMeterLedData[1].LedLoads.LedLoad1 = 1;
	else
		SendMeterLedData[1].LedLoads.LedLoad1 = 0;			

	if(MLed10 == 1)
		SendMeterLedData[1].LedLoads.LedLoad2 = 1;
	else
		SendMeterLedData[1].LedLoads.LedLoad2 = 0;		

	if(MLed8 == 1)
		SendMeterLedData[1].LedLoads.LedLoad3 = 1;
	else
		SendMeterLedData[1].LedLoads.LedLoad3 = 0;		
	
	if(MLed5 == 1)
		SendMeterLedData[1].LedLoads.LedLoad4 = 1;
	else
		SendMeterLedData[1].LedLoads.LedLoad4 = 0;		
	
  if(oilPres < FualLowNum)
		SendMeterLedData[1].LedLoads.LedLoad5 = 1;
	else
		SendMeterLedData[1].LedLoads.LedLoad5 = 0;		

	if(OvercarspeedLED == 1)
		SendMeterLedData[1].LedLoads.LedLoad6 = 1;
	else
		SendMeterLedData[1].LedLoads.LedLoad6 = 0;		

	if(OverTachospeedLED == 1)
		SendMeterLedData[1].LedLoads.LedLoad7 = 1;
	else
		SendMeterLedData[1].LedLoads.LedLoad7 = 0;		

	if(EngineLowerWaterKeyBit==KeyOn)
		SendMeterLedData[2].LedLoads.LedLoad0 = 1;
	else
		SendMeterLedData[2].LedLoads.LedLoad0 = 0;	

		SendMeterLedData[2].LedLoads.LedLoad1 = 0;		

	///if(DelaySpeedIndicateKeyBit==KeyOn)
		SendMeterLedData[2].LedLoads.LedLoad2 = 1;
	///else
		///SendMeterLedData[2].LedLoads.LedLoad2 = 0;	

	if(BackRoomKeyBit==KeyOff)
		SendMeterLedData[2].LedLoads.LedLoad3 = 1;
	else
		SendMeterLedData[2].LedLoads.LedLoad3 = 0;	
	
	if((NShieldSignalKeyBit==KeyOff)&&(RShieldSignalKeyBit==KeyOff))///D shelves
		SendMeterLedData[2].LedLoads.LedLoad4 = 1;
	else
		SendMeterLedData[2].LedLoads.LedLoad4 = 0;		

	if(NShieldSignalKeyBit==KeyOn)
		SendMeterLedData[2].LedLoads.LedLoad5 = 1;
	else
		SendMeterLedData[2].LedLoads.LedLoad5 = 0;		

	if(RShieldSignalKeyBit==KeyOn)
		SendMeterLedData[2].LedLoads.LedLoad6 = 1;
	else
		SendMeterLedData[2].LedLoads.LedLoad6 = 0;		
	
	if(FrontDoorOpenSignalKeyBit==KeyOn)
		SendMeterLedData[2].LedLoads.LedLoad7 = 1;
	else
		SendMeterLedData[2].LedLoads.LedLoad7 = 0;		
	
	if(MiddleDoorOpenSignalKeyBit==KeyOn)
		SendMeterLedData[3].LedLoads.LedLoad0= 1;
	else
		SendMeterLedData[3].LedLoads.LedLoad0= 0;	

	if(FootStopKeyBit ==KeyOn)
		SendMeterLedData[3].LedLoads.LedLoad1= 1;
	else
		SendMeterLedData[3].LedLoads.LedLoad1= 0;	
	
//	if(1)
		SendMeterLedData[3].LedLoads.LedLoad2= 1;
//	else
//		SendMeterLedData[3].LedLoads.LedLoad2= 0;	

		SendMeterLedData[3].LedLoads.LedLoad3= 0;	
		SendMeterLedData[3].LedLoads.LedLoad4= 0;	
		SendMeterLedData[3].LedLoads.LedLoad5= 0;	
		SendMeterLedData[3].LedLoads.LedLoad6= 0;	
		SendMeterLedData[3].LedLoads.LedLoad7= 0;	

	if(AirFiltrateWarnKeyBit==KeyOn)
		SendMeterLedData[4].LedLoads.LedLoad0= 1;
	else
		SendMeterLedData[4].LedLoads.LedLoad0= 0;	

	if(AccKeyBit==KeyOn)
		SendMeterLedData[4].LedLoads.LedLoad1= 1;
	else
		SendMeterLedData[4].LedLoads.LedLoad1= 0;		

	if(EngineRoomTemperatureKeyBit==KeyOn)
		SendMeterLedData[4].LedLoads.LedLoad2= 1;
	else
		SendMeterLedData[4].LedLoads.LedLoad2= 0;	
	
//	if(0)
	//	SendMeterLedData[4].LedLoads.LedLoad3= 1;
//	else
		SendMeterLedData[4].LedLoads.LedLoad3= 0;	

	if(MILErrorFlag ==1)
		SendMeterLedData[4].LedLoads.LedLoad4= 1;
	else
		SendMeterLedData[4].LedLoads.LedLoad4= 0;	

	if(CatalystTankLevelFlag ==1)
		SendMeterLedData[4].LedLoads.LedLoad5= 1;
	else
		SendMeterLedData[4].LedLoads.LedLoad5= 0;	

	if(SCRCANModuleErrorFlag ==1)
		SendMeterLedData[4].LedLoads.LedLoad6= 1;
	else
		SendMeterLedData[4].LedLoads.LedLoad6= 0;

	if((carspeed>5)&&(FrontDoorOpenSignalKeyBit==KeyOn))
		SendMeterLedData[4].LedLoads.LedLoad7= 1;
	else
		SendMeterLedData[4].LedLoads.LedLoad7= 0;	
	

	if((carspeed>5)&&(MiddleDoorOpenSignalKeyBit==KeyOn))
		SendMeterLedData[5].LedLoads.LedLoad0= 1;
	else
		SendMeterLedData[5].LedLoads.LedLoad0= 0;		

		SendMeterLedData[5].LedLoads.LedLoad1= 0;		
		SendMeterLedData[5].LedLoads.LedLoad2= 0;		
		SendMeterLedData[5].LedLoads.LedLoad3= 0;		
		SendMeterLedData[5].LedLoads.LedLoad4= 0;		
		SendMeterLedData[5].LedLoads.LedLoad5= 0;		
		SendMeterLedData[5].LedLoads.LedLoad6= 0;		
		SendMeterLedData[5].LedLoads.LedLoad7= 0;		
	
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void StTest(void)
{
	unsigned	char	Sti,Stj,Stx,Sty,FData,PData,SData;		
	
	GetSTProcess();
		
	Stx=0;
	Sty=0;	
	for(Sti=0;Sti<Num_PowerDevice;Sti++)
	{
		for(Stj=0;Stj<8;Stj++)
		{			
			FData=FlagsADError[Sti];      
			FData=(FData>>Stj)&0x01;						
			if(FData==0||(FData==1&&FlagsADShort[Sti][Stj]<AD_SHORT_N))		
			{
				PData=PutData[Sti].Load;
				PData=(PData>>Stj)&0x01;				
				SData=StData[Sti].Data;
				SData=(SData>>Stj)&0x01;
				if((SData==0&&PData==1))///switch close but drive load will detect open or short							
				{
        	FlagsADShort[Sti][Stj]++;
        	if(FlagsADShort[Sti][Stj]>=AD_SHORT_N)
        	{ 
          	ReState[Stx].Data&=~(0x03<<Sty);
          	ReState[Stx].Data|=0x01<<Sty;          			///short is "01"
          	FlagsADError[Sti]|=0x01<<Stj;
          	FlagsADShort[Sti][Stj]=AD_SHORT_N;
						FlagsADNormalOpen[Sti][Stj]=0;							///if be judged to error,clear normal state counter
						FlagsADNormalClose[Sti][Stj]=0;
           }
         }
/*         else if(SData==0&&PData==0) 												///switch close and not drive load will detect open
         {
						FlagsADOpen[Sti][Stj]++;
						if(FlagsADOpen[Sti][Stj]>=AD_OPEN_N)
						{ 
							ReState[Stx].Data&=~(0x03<<Sty);
							ReState[Stx].Data|=0x02<<Sty;            			///load Open is 10
							FlagsADError[Sti]|=0x01<<Stj;
								FlagsADOpen[Sti][Stj]=AD_OPEN_N;
							FlagsADNormalOpen[Sti][Stj]=0;								///if be judged to error,clear normal state counter
							FlagsADNormalClose[Sti][Stj]=0;
						}
          }*/
          else	if(SData==1&&PData==1)														
         	{
         		FlagsADNormalOpen[Sti][Stj]++;
						if(FlagsADNormalOpen[Sti][Stj]>=AD_NORMAL_OPEN)
						{
							FlagsADNormalOpen[Sti][Stj]=AD_NORMAL_OPEN;
         			ReState[Stx].Data&=~(0x03<<Sty);									///normal open is 00
            	FlagsADError[Sti]&=~(0x01<<Stj);								///error(short or open) flag clear              		        	
            	FlagsADOpen[Sti][Stj]=0;      										///when switch not open and load open counter clear             		         	           	     		
            	FlagsADShort[Sti][Stj]=0;             					///short counter clear  
            	FlagsADNormalClose[Sti][Stj]=0;
						}
         	} 	
         	else	if((SData==1&&PData==0)||(SData==0&&PData==0))
         	{	 
         		FlagsADNormalClose[Sti][Stj]++;
         		if(FlagsADNormalClose[Sti][Stj]>=AD_NORMAL_CLOSE)
         		{
         			FlagsADNormalClose[Sti][Stj]=AD_NORMAL_CLOSE;
				 			ReState[Stx].Data&=~(0x03<<Sty);					
				 			ReState[Stx].Data|=0x03<<Sty;									///normal close is 11
           		FlagsADError[Sti]&=~(0x01<<Stj);							///error(short or open) flag clear             		        	
           		FlagsADOpen[Sti][Stj]=0;      									///when switch not open and load open counter clear            		         	
           		FlagsADShort[Sti][Stj]=0;             				///short counter clear
           		FlagsADNormalOpen[Sti][Stj]=0;
         		}
          }        	
			}
			FData=FlagsADError[Sti];
      FData=(FData>>Stj)&0x01;
      if(FData==1)
      {  
        if(FlagsADShort[Sti][Stj]>=AD_SHORT_N)      				///if load is short,close it 	
         	PutData[Sti].Load&=~(0x01<<Stj);		                  
      }
			Sty+=2;      																					///2bit show a load state which is open,short or normal
      if(Sty>=8)
      {
        Stx++; 
        Sty=0;
      }
		}		
	} 	
	///mask Load state
	if(ReState[0].St.S0==Load_Open)
	{
		if(MLoad1)
			ReState[0].St.S0=Load_On;
		else
			ReState[0].St.S0=Load_Off;
	}
	if(ReState[0].St.S1==Load_Open)
	{
		if(MLoad2)
			ReState[0].St.S1=Load_On;
		else
			ReState[0].St.S1=Load_Off;
	}
	if(ReState[0].St.S2==Load_Open)
	{
		if(MLoad3)
			ReState[0].St.S2=Load_On;
		else
			ReState[0].St.S2=Load_Off;
	}
	ReState[0].Data=ReState[0].Data|0b11000000;
} 	
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void	GetSTProcess(void)
{	
	if(Bts724St[0]>PowerDeviceStNorm)
		StData[0].b.ST0=1;
	else 
		StData[0].b.ST0=0;

	if(Bts724St[1]>PowerDeviceStNorm)
		StData[0].b.ST1=1;
	else 
		StData[0].b.ST1=0;		
	#if		J13_OUT==0
		if(MLoad3==1)
		{
			if(PWM_Out_AD[1]>ADCCompareHighData)//125
				StData[0].b.ST2=1;		
			else
				StData[0].b.ST2=0;		
		}
	#endif
	
	StData[0].b.ST3=1;///mask no used state
	StData[0].b.ST4=1;
	StData[0].b.ST5=1;
	StData[0].b.ST6=1;
	StData[0].b.ST7=1;
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static unsigned char PowerDeviceChangeData(void)
{
	unsigned	char	TempDataB,TempDataC;
	unsigned	char	Mi,Mj;	
  ///load out realize 		 
  for(Mi=0;Mi<Num_PowerDevice;Mi++)
		PutData[Mi].Load=0x00;						///first close all,then open what you need	

	if(SleepFlag==1)
		goto	_LoadOut;

	if((OnKeyBit==KeyOn)&&(engSpeed>450))
		MLoad1=1;
	if((OnKeyBit==KeyOn)&&(LocationLampKeyBit==KeyOn))
		MLoad2=1;

/*	
	#if		J13_OUT==0
		if((OnKeyBit==KeyOn)&&(carspeed<=20))
			MLoad3=1;
	#endif
*/		
  for(Mi=0;Mi<Num_PowerDevice;Mi++)
  {
  	for(Mj=0;Mj<8;Mj++)
  	{  
  		TempDataB=FlagsADError[Mi];
   		TempDataB=(TempDataB>>Mj)&0x01;    			
   		if(TempDataB==0x01)   
   		{
   			if(FlagsADShort[Mi][Mj]>=AD_SHORT_N)////if short,must close load
   				PutData[Mi].Load&=~(0x01<<Mj); 
   		}
   	}
  }
	_LoadOut:  	
  ////judge if load drive will be changed	
  TempDataC=0;
  for(Mi=0;Mi<Num_PowerDevice;Mi++)
  {  
    if(PutData[Mi].Load!=OldPutDtata[Mi].Load)
    {
      TempDataC=1;                            
      OldPutDtata[Mi].Load=PutData[Mi].Load;
     }     
   } 
  return(TempDataC); 	
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void	PowerDeviceDrive(unsigned char	LoadState)
{
	PANNAL_LOAD1_IO = 1;
  PANNAL_LOAD2_IO = 1;
	#if		J13_OUT==0
		PANNAL_LOAD3_IO	= 1;
	#endif
	
	if(LoadState==0)
	{
		PANNAL_LOAD1=0;
		PANNAL_LOAD2=0;
		#if		J13_OUT==0
			PANNAL_LOAD3=0;
		#endif
	}
	else if(LoadState==1)
	{
		if(MLoad1)
			PANNAL_LOAD1=1;
		else
			PANNAL_LOAD1=0;
		if(MLoad2)
			PANNAL_LOAD2=1;
		else
			PANNAL_LOAD2=0;
		#if		J13_OUT==0
			if(MLoad3)
				PANNAL_LOAD3=1;
			else
				PANNAL_LOAD3=0;
		#endif
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void	ErrorReset(void)
{
	unsigned	char	Mi,Mj;
  if((AccKeyBit==KeyOff)&&(OnKeyBit==KeyOff)&&(DangerWarnKeyBit==KeyOff/*(LeftTurnLampKeyBit==KeyOff)&&(RightTurnLampKeyBit==KeyOff)*/))
	{
		if(ClearErrorFlag==1)
  	{
   		for(Mi=0;Mi<Num_PowerDevice;Mi++)
   		{   	
   			FlagsADError[Mi]=0x00;

				V_POWE4_ErrorFlag=0;
				VideoPower1ErrorFlag=0;
				VideoPower2ErrorFlag=0;

				Video_Power_Detect_Time[0]=0;
				Video_Power_Detect_Time[1]=0;
				Speed_PWM_Detect_Time=0;
				
   			for(Mj=0;Mj<8;Mj++)
   			{
   				FlagsADShort[Mi][Mj]=0x00;     		
   				FlagsADOpen[Mi][Mj]=0x00; 
   				FlagsADNormalOpen[Mi][Mj]=0x00;	
					FlagsADNormalClose[Mi][Mj]=0x00;
   			}
   		}		
   		ClearErrorFlag=0;    		
  	}  
	}
	else
	{
		if(ClearErrorFlag==0)
			ClearErrorFlag=1;
	}   		
}
#pragma CODE_SEG DEFAULT
