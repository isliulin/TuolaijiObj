#define		MESSAGE_C
#include	"config.h"
#pragma CODE_SEG  USER_ROM
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
void	MesseageDeal(void)
{
	SleepDeal();
	
	SystemPowerDeal();

	if(SleepFlag==1)
		return;

	module_check();
	ConnetDisplayDeal();	
	
	MeterErrorDisplayDeal();
	
	DriveErrorDisplayDeal();
	
	PowerErrorDisplayDeal();
	
//	PowerContrlCurrentDeal();

//	PowerContrlErrorDisplayDeal();	

	SecondErrorState_Logic();
	SecondErrorDisplayDeal();
	
	SpecialSecondErrorDeal();
	SpecialThirdErrorDeal();

	Sensor_rxd_process();
	Message_SpecialState_process();
	
	CanSendDeal();
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
static void SleepDeal(void)
{	
	uchar i;
	CAN0_RS_IO=1;
	CAN1_RS_IO=1;
	
	if(SleepFlag==1)
	{	
		WakeUpInitFlag=1;
	  SleepIndex=0;
		//休眠时关闭电机省电
	  for(i=0;i<12;i++)						///Duty cycle channel 0 for micro-stepping initialization
	  	*(&MCDC0+i)=0x00;		
		
		if ((AccKeyBit == KeyOn) || (OnKeyBit == KeyOn) || (DangerWarnKeyBit == KeyOn) 
		|| (FrontDoorOpenKeyBit== KeyOn)/*||(MiddleDoorOpenKeyBit== KeyOn)||*//*(BackRoomKeyBit== KeyOff)*/	
		||(FrontDoorCloseKeyBit== KeyOn)/*||(MiddleDoorCloseKeyBit== KeyOn)*/
		/*||(FrontDoorPreventNipKeyBit==KeyOn)||(MiddleDoorPreventNipKeyBit==KeyOn)*/)
			WakeUpInit();
	}
	else
	{
	 if(WakeUpInitFlag==1)
		WakeUpInit();	
	 switch(SleepIndex)
	 {
	   case 0:
	   if((AccKeyBit==KeyOff)&&(OnKeyBit==KeyOff)&&(DangerWarnKeyBit==KeyOff)
		 	&&(FrontDoorOpenKeyBit==KeyOff)/*&&(MiddleDoorOpenKeyBit==KeyOff)&&(BackRoomKeyBit==KeyOn)*/
		 	/*&&(MiddleDoorCloseKeyBit==KeyOff)*/&&(FrontDoorCloseKeyBit==KeyOff)
		 	/*&&(FrontDoorPreventNipKeyBit==KeyOff)&&(MiddleDoorPreventNipKeyBit==KeyOff)*/)  
	   {								
	       delay60s=0;
	       SleepIndex=1;
	 	  }
	 	  break;
	   case 1:
		   if(delay60s>CanEnterSleepDelayTimeNum)	/*1 minute will enter sleep*/
		   {		                		 
		     SleepCodeBuf=SleepCode;
	       SendCan0Message(SendSleepFlash); 		/*send sleep message,0xaa is sleep,0x55 is wake*/
//			 	 SleepFlag=1;					
			   sleep_mode();													/*CAN  enter sleep mode*/
	 	     SleepIndex=0;
				 CAN0_RS=1;
				 CAN1_RS=1;
				 SleepFlag=1;		
				 WakeUpInitFlag=1;
		   }
			 else
			 {
			   if ((AccKeyBit == KeyOn) || (OnKeyBit == KeyOn) || (DangerWarnKeyBit == KeyOn)
						|| (FrontDoorOpenKeyBit== KeyOn)/*||(MiddleDoorOpenKeyBit== KeyOn)||(BackRoomKeyBit== KeyOff)*/	
						/*||(MiddleDoorCloseKeyBit== KeyOn)*/||(FrontDoorCloseKeyBit== KeyOn)
						/*||(FrontDoorPreventNipKeyBit==KeyOn)||(MiddleDoorPreventNipKeyBit==KeyOn)*/)
			   {			    
				   SleepIndex=0;
				   SleepFlag=0;		 
			   }
			 }
			 break;
		 default:
	     SleepIndex=0;
	     break;
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
static void	CanSendDeal(void)
{
	uchar i;
	if(boot_identify_flag==1)
  { 
		if(SendSelfIdNum==0)
		{
	   SendCan0Message(SendSelfID);
		 SendSelfIdNum++;
		}
		else
		{
		 SendCan0Message(SendSelfID1);
		 boot_identify_flag=0;
		}
  }
	else  if(ID_require_flag==1)
	{
		ID_require_flag=0;
		SendCan0Message(sendrequireID);
	}
	else if(SendConfigDataFlag==1)
	{
		SendConfigDataFlag=0;
		if(ConfigDataResult== 0x01)
			SendConfigDataWaitFlag=1;
		SendCan0Message(SendConfigData);

	}
	else if(SendMeterKeyFlag==1)
	{
  	SendCan0Message(SendMeterSwich); 
		MeterKeySendTime=0;
		SendMeterKeyFlag=0;
	}
	else if(SendVirsualMeterKeyFlag==1)
	{
  	SendCan0Message(SendVirtualSwich); 
		VirsualMeterKeySendTime=0;
		SendVirsualMeterKeyFlag=0;
	}	
	else if(SendMeterLoadFlag==1)
	{
		SendCan0Message(Sendloadstatus);
		MeterSendLoadStTime=0;
		SendMeterLoadFlag=0;
	}
	else if(SendDistanceFlag==1)
	{
		SendCan0Message(SendDistance);
		MeterSendDistanceTime=0;
		SendDistanceFlag=0;
	}
	else if(RainScratchChangeFlag==1)
	{
		SendCan0Message(SendRainScratch);
		RainScratchWaitAckFlag=1;
		rains_count=0;
		RainScratchChangeFlag=0;
	} 
	else if(MeterSendFuelConsumeFlag==1)
	{	
		SendCan0Message(SendJ1939_oilwear);
		MeterSendFuelConsumeTime=0;
		MeterSendFuelConsumeFlag=0;		
	}
	else if(MeterSendFuelConsumeFlag1==1)
	{	
		SendCan0Message(SendJ1939_oilwear1);
		MeterSendFuelConsumeTime1=0;
		MeterSendFuelConsumeFlag1=0;		
	}	
	else if(SendMeterPanelTemperatureSensorFlag==1)
	{
		SendCan0Message(SendSensorstatus);
		MeterSendPanelTemperatureSensorTime=0;  
		SendMeterPanelTemperatureSensorFlag=0;
	}		
	/*
	else if(SendMeterLedFlag==1)
	{
		SendCan0Message(SendMeterLed);
		MeterLedSend250ms=0;  
		SendMeterLedFlag=0;
	}	*/	

	//胎压数据发送
	if(TPMS_TX1939_Time_Flag)
	{
		TPMS_TX1939_Time_Flag=0;
		if(TPMS_CAN_LearnEndFlag||TPMS_CAN_LearnEnd_CT)
		{
			TX_TPMS_LearnEnd();
			if(TPMS_CAN_LearnEnd_CT)
				TPMS_CAN_LearnEnd_CT--;
		}
		else
		{
			for(i=0;i<TYRE_COUNT;i++)
			{
				if((TyreState[i]==WAIT_REPLY)||(TyreState[i]==WAIT_SUCCEED))
				  TXWaitRelay();   
			}
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
static void SystemPowerDeal(void)
{
	uint Power_Change_Temp;
	
	///休眠时必须要关闭
	V_POWER_CTRL_IO = 1;
	///实现低功耗时休眠必须要关掉
	V_POWER3_CTRL_IO = 1;
	///检测到短路时要关闭
	TV_POWER1_CTRL_IO = 1;
	TV_POWER2_CTRL_IO = 1;
	///12V的控制总电源
	V_12_CTRL_IO=1;
	///传感器的电源
	AN_POW_IO	= 1;
	
	if(SleepFlag==1)
	{
		V_POWER_CTRL = 0;
//		V_POWER3_CTRL = 0;
		TV_POWER1_CTRL = 0;
		TV_POWER2_CTRL = 0;
		V_POW4_CTRL=0;
		AN_POW=0;
		V_12_CTRL=1;
	}
	else
	{
		V_12_CTRL=0;///控制12V的总电源
		V_POWER_CTRL = 1;///控制系统V1电源
		V_POWER3_CTRL = 1;///控制系统的V3电源
		AN_POW=1;///传感器电源
		
		Power_Change_Temp=((uint)V12_Power)*8/10;
		///控制视频供电的其中一个电源
		if(Video_Power[0]<(uchar)Power_Change_Temp)
		{
			Video_Power_Detect_Time[0]++;
			if(Video_Power_Detect_Time[0]>=3)///超过主循环三次的时间
			{
				Video_Power_Detect_Time[0]=3;
				TV_POWER1_CTRL = 0;
				VideoPower1ErrorFlag=1;
			}
			else
				TV_POWER1_CTRL = 1;
		}
		else
		{
			if(VideoPower1ErrorFlag==0)
			{
				Video_Power_Detect_Time[0]=0;
				TV_POWER1_CTRL = 1;
			}
			else
				TV_POWER1_CTRL = 0;
		}
		///控制视频供电的另一个电源
		if(Video_Power[1]<(uchar)Power_Change_Temp)
		{
			Video_Power_Detect_Time[1]++;
			if(Video_Power_Detect_Time[1]>=3)///超过主循环三次的时间
			{
				Video_Power_Detect_Time[1]=3;
				TV_POWER2_CTRL = 0;
				VideoPower2ErrorFlag=1;
			}
			else
				TV_POWER2_CTRL = 1;
		}
		else
		{
			if(VideoPower2ErrorFlag==0)
			{
				Video_Power_Detect_Time[1]=0;
				TV_POWER2_CTRL = 1;
			}
			else
				TV_POWER1_CTRL = 0;
		}
		
		Power_Change_Temp=((uint)V12_Power)*8/10;
		///车速电源
		if(SPEED_PWM_AD<(uchar)Power_Change_Temp)
		{
			Speed_PWM_Detect_Time++;
			if(Speed_PWM_Detect_Time>=3)///超过主循环三次的时间
			{
				Speed_PWM_Detect_Time=3;
				V_POW4_CTRL = 0;
				V_POWE4_ErrorFlag=1;
			}
			else
				V_POW4_CTRL = 1;
		}
		else
		{
			if(V_POWE4_ErrorFlag==0)
			{
				Speed_PWM_Detect_Time=0;
				V_POW4_CTRL = 1;
			}
			else
				V_POW4_CTRL = 0;
		}
	}
}
/****************************************************************************************************
		Funtion:change to speed pwm period
  	Arithmatics:			
																						1公里
		单个脉冲距离(公里）= --------------------------------------------------------------
													  车速速比（1公里走了几圈）*脉冲个数（1圈几个霍尔传感器）
													
													   单个脉冲距离(公里）
		单个脉冲时间（小时）=------------------------------
													   速度（公里/小时）		
		单个脉冲时间（us）= 单个脉冲时间（小时）*3600*1000*1000
																								3600*1000*1000
		单个脉冲时间（us）= -------------------------------------------------------------------------
												车速速比（1公里走了几圈）*脉冲个数（1圈几个霍尔传感器）* 速度（公里/小时）
									单个脉冲时间（us）
		PWM周期 = ----------------------------
								PWM 时钟周期(现用128us)
*****************************************************************************************************/
static void SpeedPwmOut(void)
{
	ulong	SpeedPwmTemp;
	ulong	HourChangeToUsNum;
	
	if(carspeed>=3)
	{
		SpeedPwmTemp=Speed_Pusle_Num*((ulong)Speed_Rate_Num);
		SpeedPwmTemp=SpeedPwmTemp*PwmPeriodNum;
		SpeedPwmTemp=SpeedPwmTemp*carspeed;
		HourChangeToUsNum=3600*((unsigned long)1000);///hour change to us 
		HourChangeToUsNum=HourChangeToUsNum*1000;
		PWMPER45=(uint)(HourChangeToUsNum/SpeedPwmTemp);
		PWMDTY45	= PWMPER45/2;	///duty is 1/2 to period
		#if		J13_OUT==1
			PWMPER23=(uint)(HourChangeToUsNum/SpeedPwmTemp);
			PWMDTY23	= PWMPER23/2;	///duty is 1/2 to period
		#endif
	}
	else
	{
		PWMPER45=0xfffe;					///all low electric
		PWMDTY45=0xffff;
		#if		J13_OUT==1
			PWMPER23=0xfffe;					///all low electric
			PWMDTY23=0xffff;
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
static void	Sensor_rxd_process(void)
{
	unsigned char	SensorTemp=0;
	///前气压传感器在具体模块及位置的处理
	#if		FRONT_AIR_SENSOR_IN==0
		SensorTemp=FrontSensorData[FRONT_AIR_POSITION_IN];    
	#elif		FRONT_AIR_SENSOR_IN==1
		SensorTemp=BackSensorData[FRONT_AIR_POSITION_IN];        
	#elif		FRONT_AIR_SENSOR_IN==2
		SensorTemp=TopSensorData[FRONT_AIR_POSITION_IN];      
	#elif		FRONT_AIR_SENSOR_IN==3
		SensorTemp=PannalSensorData[FRONT_AIR_POSITION_IN];      
	#endif
	if(SensorTemp==SensorErorrData)
	{
		pres_value=0x00;
		qy1cgerrflag=1;
	}
	else
	{
		qy1cgerrflag=0;
		if(SensorTemp>AirPressHighNum)
			pres_value=AirPressHighNum/2*5;///->/2*5代表将其转化为250个分辨率
		else
		{
			pres_value=SensorTemp/2*5;
		}
	}
	
	#if		FRONT_AIR_SENSOR_IN==0
		if(FrontDriveErrorFlag==1)
			pres_value=0;
	#elif		FRONT_AIR_SENSOR_IN==1	
		if(BackDriveErrorFlag==1)
		 	pres_value=0;
	#elif		FRONT_AIR_SENSOR_IN==2
		if(TopDriveErrorFlag==1)
			pres_value=0;
	#elif		FRONT_AIR_SENSOR_IN==3
		asm nop;
	#elif		FRONT_AIR_SENSOR_IN==0xff
		qy1cgerrflag=0;
	#endif
	///后气压传感器在具体模块及位置的处理
	#if		BACK_AIR_SENSOR_IN==0
		SensorTemp=FrontSensorData[BACK_AIR_POSITION_IN];    
	#elif		BACK_AIR_SENSOR_IN==1
		SensorTemp=BackSensorData[BACK_AIR_POSITION_IN];    
	#elif		BACK_AIR_SENSOR_IN==2
		SensorTemp=TopSensorData[BACK_AIR_POSITION_IN]; 
	#elif		BACK_AIR_SENSOR_IN==3
		SensorTemp=PannalSensorData[BACK_AIR_POSITION_IN]; 
	#endif
	if(SensorTemp==SensorErorrData)
	{
		gaspress2=0x00;
		qy2cgerrflag=1;
	}
	else
	{
		qy2cgerrflag=0;
		if(SensorTemp>AirPressHighNum)
			gaspress2=AirPressHighNum/2*5;///->/2*5代表将其转化为250个分辨率
		else
		{
			gaspress2=SensorTemp/2*5;
		}
	}

	#if		BACK_AIR_SENSOR_IN==0
		if(FrontDriveErrorFlag==1)
			gaspress2=0;
	#elif		BACK_AIR_SENSOR_IN==1
		if(BackDriveErrorFlag==1)
			gaspress2=0;   
	#elif		BACK_AIR_SENSOR_IN==2	
		if(TopDriveErrorFlag==1)
			gaspress2=0;
	#elif		BACK_AIR_SENSOR_IN==3	
		asm nop;
	#elif		BACK_AIR_SENSOR_IN==0xff
		qy2cgerrflag=0;
	#endif
	///油量传感器在具体模块及位置的处理
	#if		FUAL_LEVEL_SENSOR_IN==0
  	SensorTemp=FrontSensorData[FUAL_LEVEL_POSITION_IN];       
	#elif		FUAL_LEVEL_SENSOR_IN==1
		SensorTemp=BackSensorData[FUAL_LEVEL_POSITION_IN];  
	#elif		FUAL_LEVEL_SENSOR_IN==2
		SensorTemp=TopSensorData[FUAL_LEVEL_POSITION_IN];   
	#elif		FUAL_LEVEL_SENSOR_IN==3
		SensorTemp=PannalSensorData[FUAL_LEVEL_POSITION_IN];   
	#elif		FUAL_LEVEL_SENSOR_IN==0xff
		asm nop;
	#endif
	if(SensorTemp==SensorErorrData)
	{
		OilNumberData=0x00;
		ylcgerrflag=1;
	}
	else 
	{	
		ylcgerrflag=0;
		if(SensorTemp>OILHIGHNUM)
		{
			SensorTemp=OILHIGHNUM;
			OilNumberData=SensorTemp;
		}
		else 
		{
			if((SensorTemp<=OilNumberData-OILREVISENUM)||(SensorTemp>=OilNumberData+OILREVISENUM))
				OilNumberData=SensorTemp;	
		}
	}

	#if		FUAL_LEVEL_SENSOR_IN==0
    if(FrontDriveErrorFlag==1)
			OilNumberData=0;
	#elif		FUAL_LEVEL_SENSOR_IN==1
		if(BackDriveErrorFlag==1)
			OilNumberData=0;
	#elif		FUAL_LEVEL_SENSOR_IN==2
	  if(TopDriveErrorFlag==1)
			OilNumberData=0;      
	#elif		FUAL_LEVEL_SENSOR_IN==3
		asm nop;
	#elif		FUAL_LEVEL_SENSOR_IN==0xff
		ylcgerrflag=0;
	#endif
	///机油压力传感器在具体模块及位置的处理
	if(oil_j1939_flag==0)
	{
		#if		OIL_PRESS_SENSOR_IN==0
			SensorTemp=FrontSensorData[OIL_PRESS_POSITION_IN];     
		#elif		OIL_PRESS_SENSOR_IN==1
			SensorTemp=BackSensorData[OIL_PRESS_POSITION_IN];     
		#elif		OIL_PRESS_SENSOR_IN==2
			SensorTemp=TopSensorData[OIL_PRESS_POSITION_IN];    
		#elif		OIL_PRESS_SENSOR_IN==3
			SensorTemp=PannalSensorData[OIL_PRESS_POSITION_IN];    
		#endif
		if(SensorTemp==SensorErorrData)
		{
			oilPres=0x00;
			jycgerrflag=1;
		}
		else
		{
			jycgerrflag=0;
			if(SensorTemp>FualPressHighNum)
				oilPres=FualPressHighNum;
			else
			{
				oilPres=SensorTemp;
			}
		}

		#if		OIL_PRESS_SENSOR_IN==0
			if(FrontDriveErrorFlag==1)
				oilPres=0;  
		#elif		OIL_PRESS_SENSOR_IN==1
			if(BackDriveErrorFlag==1)
				oilPres=0;    
		#elif		OIL_PRESS_SENSOR_IN==2
			if(TopDriveErrorFlag==1)
				oilPres=0;
		#elif		OIL_PRESS_SENSOR_IN==0xff
			jycgerrflag=0;
		#endif
	}
	///水温传感器在具体模块及位置的处理
	#if		WATER_TEMPERATURE_SENSOR_IN==0
		SensorTemp=FrontSensorData[WATER_TEMPERATURE_POSITION_IN];    
	#elif		WATER_TEMPERATURE_SENSOR_IN==1
		SensorTemp=BackSensorData[WATER_TEMPERATURE_POSITION_IN];    
	#elif		WATER_TEMPERATURE_SENSOR_IN==2
		SensorTemp=TopSensorData[WATER_TEMPERATURE_POSITION_IN]; 
	#elif		WATER_TEMPERATURE_SENSOR_IN==3
		SensorTemp=PannalSensorData[WATER_TEMPERATURE_POSITION_IN]; 
	#endif

	#if			WATER_TEMPERATURE_SENSOR_IN!=0xff
	if(SensorTemp==SensorErorrData)
	{
		watTemp=0x00;
		WaterTempSensorErrorFlag=1;
	}
	else
	{
		WaterTempSensorErrorFlag=0;
		if(SensorTemp>ENGINE_WATER_TEMPERATURE_HIGH)
			watTemp=ENGINE_WATER_TEMPERATURE_HIGH;
		else if(SensorTemp<40)
			watTemp=40;
		else
		{
			watTemp=SensorTemp;
		}
	}
	#elif		WATER_TEMPERATURE_SENSOR_IN==0xff
		
	#endif
	
	if(watTemp>=WATER_OVER_VALUE)///水温过高
		WaterTemperatureFlag=1;
	else
		WaterTemperatureFlag=0;

	#if		WATER_TEMPERATURE_SENSOR_IN==0
		if(FrontDriveErrorFlag==1)
			watTemp=0;
	#elif		WATER_TEMPERATURE_SENSOR_IN==1
		if(BackDriveErrorFlag==1)
			watTemp=0;   
	#elif		WATER_TEMPERATURE_SENSOR_IN==2	
		if(TopDriveErrorFlag==1)
			watTemp=0;
	#elif		WATER_TEMPERATURE_SENSOR_IN==3	
		asm nop;
	#elif		WATER_TEMPERATURE_SENSOR_IN==0xff
		WaterTempSensorErrorFlag=0;
	#endif
	///车速传感器在具体模块及位置的处理
	#if		SPEED_SENSOR_IN==0
		if(FrontDriveErrorFlag==1)
		{
			carspeed=0;
			CARSPEED();
			Speed_Data_Flag=1;
		}
	#elif		SPEED_SENSOR_IN==1
		if(BackDriveErrorFlag==1)
		{
			carspeed=0;
			CARSPEED();
			Speed_Data_Flag=1;
		}
	#elif		SPEED_SENSOR_IN==2
		if(TopDriveErrorFlag==1)
		{
			carspeed=0;
			CARSPEED();
			Speed_Data_Flag=1;
		}
	#elif		SPEED_SENSOR_IN==3
		asm nop;	
	#elif		SPEED_SENSOR_IN==0xff
		asm nop;	
	#endif

  if(Speed_Data_Flag==1)                	
  {		
		if(carspeed>OVERCARSPEEDVALUE) 
			OvercarspeedFlag=1;
		else 
			OvercarspeedFlag=0;
  	VehicleSpeedData=carspeed*((dword)1000);///放大1000倍，给里程计算使用
  	#if		CAR_SPEED_SIGNAL_OUT==1
  		SpeedPwmOut();
		#endif
  	Speed_Data_Flag=0;
  }
	///转速传感器在具体模块及位置的处理
	#if		ROTATION_SENSOR_IN==0
		if(FrontDriveErrorFlag==1)
		{
			engSpeed=0x00;					
			EngRotation();
			Rotation_Data_Flag=1;
		}
	#elif		ROTATION_SENSOR_IN==1
		if(BackDriveErrorFlag==1)
		{
			engSpeed=0x00;					
			EngRotation();
			Rotation_Data_Flag=1;
		}
	#elif		ROTATION_SENSOR_IN==2
		if(TopDriveErrorFlag==1)
		{
			engSpeed=0x00;					
			EngRotation();
			Rotation_Data_Flag=1;
		}
	#elif		ROTATION_SENSOR_IN==3
		asm nop;	
	#elif		ROTATION_SENSOR_IN==0xff
		asm nop;	
	#endif

	if(Rotation_Data_Flag==1)
  {
 		VehicleRotation[0] = (uchar)(engSpeed >> 8); //change to send data
    VehicleRotation[1] = (uchar)engSpeed;
    if (engSpeed > OVERTACHOSPEEDVALUE)
      OvertachospeedFlag = 1;
    else
      OvertachospeedFlag = 0;
 		Rotation_Data_Flag=0;
  }
	/*****front drive temperature deal******/
	#ifdef	FRONTMODULE_CONNECT
		SensorTemp=FrontSensorData[MODULE_TEMPERATURE_POSITION_IN]; 
		if(SensorTemp<40)
		{
			SensorTemp=40-SensorTemp;
			FrontDriveModuleTemperatureFlag=1;
		}
		else
		{
			SensorTemp=SensorTemp-40;
			FrontDriveModuleTemperatureFlag=0;
		}
		QianQuWenDu[0]=SensorTemp/10;
		QianQuWenDu[1]=SensorTemp%10;
	#endif
	/*****back drive temperature deal******/
	#ifdef	BACKMODULE_CONNECT
		SensorTemp=BackSensorData[MODULE_TEMPERATURE_POSITION_IN];  
		if(SensorTemp<40)
		{
			SensorTemp=40-SensorTemp;
			BackDriveModuleTemperatureFlag=1;
		}
		else
		{
			SensorTemp=SensorTemp-40;
			BackDriveModuleTemperatureFlag=0;
		}
		HouQuWenDu[0]=SensorTemp/10;
		HouQuWenDu[1]=SensorTemp%10;
	#endif
	/*****top drive temperature deal******/
	#ifdef		TOPMODULE_CONNECT
		SensorTemp=TopSensorData[MODULE_TEMPERATURE_POSITION_IN]; 
		if(SensorTemp<40)
		{
			SensorTemp=40-SensorTemp;
			TopDriveModuleTemperatureFlag=1;
		}
		else
		{
			SensorTemp=SensorTemp-40;
			TopDriveModuleTemperatureFlag=0;
		}
		DingQuWenDu[0]=SensorTemp/10;
		DingQuWenDu[1]=SensorTemp%10;
	#endif	
	/*****power temperature deal******/
	#ifdef	POWERMODULE_CONNECT
		SensorTemp=PowerSensorData[MODULE_TEMPERATURE_POSITION_IN]; 
		if(SensorTemp<40)
		{
			SensorTemp=40-SensorTemp;
			PowerModuleTemperatureFlag=1;
		}
		else
		{
			SensorTemp=SensorTemp-40;
			PowerModuleTemperatureFlag=0;
		}
		PeiDianWenDu[0]=SensorTemp/10;
		PeiDianWenDu[1]=SensorTemp%10;
	#endif
			/*****power temperature deal******/
	#ifdef	POWERCONTRLMODULE_CONNECT
			SensorTemp=PowerContrlSensorData[MODULE_TEMPERATURE_POSITION_IN]; 
			if(SensorTemp<40)
			{
				SensorTemp=40-SensorTemp;
				PowerContrlModuleTemperatureFlag=1;
			}
			else
			{
				SensorTemp=SensorTemp-40;
				PowerContrlModuleTemperatureFlag=0;
			}
			PowerContrlWenDu[0]=SensorTemp/10;
			PowerContrlWenDu[1]=SensorTemp%10;
	#endif

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
static void module_check(void)
{     
	uchar i;
	if(OnKeyBit==KeyOn)
	{
		///engine ecu module check
		#ifdef	ENGINE_ECU_CONNECT
	  if(EngineECU_CANModuleFlag==1)					
	  {
	  	EngineECU_CANModuleFlag=0;
			ENGINE_ECU_CANErrorFlag=0;
			ECU_ModuleCheck_Time=0x00;	
	  }
	  else 
	  {
	  	if(ECU_ModuleCheck_Time>J1939MODULE_CONNECT_NUM)
	  	{
	  	 	ECU_ModuleCheck_Time=J1939MODULE_CONNECT_NUM+2;
				ENGINE_ECU_CANErrorFlag=1;
	  	}	
	  }
		#endif
		///SCR module check
		#ifdef	SCR_CONNECT
		if(SCRSystemFlag==1)
		{
			SCRSystemFlag=0;
			SCRModuleDetectTime=0;
			SCRCANModuleErrorFlag=0;
		}
		else
		{
			if(SCRModuleDetectTime>J1939MODULE_CONNECT_NUM)
			{
				SCRModuleDetectTime=J1939MODULE_CONNECT_NUM+2;
				SCRCANModuleErrorFlag=1;
			}
		}
		#endif
		///bms module check
		#ifdef	BMS_CONNECT
		if(BMS_CAN_ModuleFlag==1)
		{
			BMS_CAN_ModuleFlag=0;
			BMS_CAN_ModuleErrorFlag=0;
			Delay_BMS_ModuleCheck=0;
		}
		else
		{
			if(Delay_BMS_ModuleCheck>J1939MODULE_CONNECT_NUM)
			{
				Delay_BMS_ModuleCheck=J1939MODULE_CONNECT_NUM+2;
				BMS_CAN_ModuleErrorFlag=1;
			}
		}
		#endif
		///dmc module check
		#ifdef	DMC_CONNECT
		if(DMC_CAN_ModuleFlag==1)
		{
			DMC_CAN_ModuleFlag=0;
			DMC_CAN_ModuleErrorFlag=0;
			Delay_DMC_ModuleCheck=0;
		}
		else
		{
			if(Delay_DMC_ModuleCheck>J1939MODULE_CONNECT_NUM)
			{
				Delay_DMC_ModuleCheck=J1939MODULE_CONNECT_NUM+2;
				DMC_CAN_ModuleErrorFlag=1;
			}
		}
		#endif
		///FULL CONTROL module check
		#ifdef	FULLCONTROL_CONNECT
		/*if(FullControl_CAN_ModuleFlag==1)
		{
			FullControl_CAN_ModuleFlag=0;
			FULLCONTROL_CAN_ModuleErrorFlag=0;
			Delay_FULLCONTROL_ModuleCheck=0;
		}
		else
		{
			if(Delay_FULLCONTROL_ModuleCheck>=J1939MODULE_CONNECT_NUM)
			{

				Delay_FULLCONTROL_ModuleCheck=J1939MODULE_CONNECT_NUM+2;
				FULLCONTROL_CAN_ModuleErrorFlag=1;
			}
		}*/
		#endif
		///ASSITANT module check
		#ifdef	ASSITANT_CONNECT
		/*if(ASSISTANT_CAN_ModuleFlag==1)
		{
			ASSISTANT_CAN_ModuleFlag=0;
			ASSISTANT_CAN_ModuleErrorFlag=0;
			Delay_ASSISTANT_ModuleCheck=0;
		}
		else
		{
			if(Delay_ASSISTANT_ModuleCheck>=J1939MODULE_CONNECT_NUM)
			{

				Delay_ASSISTANT_ModuleCheck=J1939MODULE_CONNECT_NUM+2;
				ASSISTANT_CAN_ModuleErrorFlag=1;
			}
		}*/
		#endif
		///ISG module check
		#ifdef	ISG_CONNECT
		/*if(ISG_CAN_ModuleFlag==1)
		{
			ISG_CAN_ModuleFlag=0;
			ISG_CAN_ModuleErrorFlag=0;
			Delay_ISG_ModuleCheck=0;
		}
		else
		{
			if(Delay_ISG_ModuleCheck>=J1939MODULE_CONNECT_NUM)
			{

				Delay_ISG_ModuleCheck=J1939MODULE_CONNECT_NUM+2;
				ISG_CAN_ModuleErrorFlag=1;
			}
		}*/
		#endif
	}
	else
	{
		ECU_ModuleCheck_Time=0;
		SCRModuleDetectTime=0;
		Delay_DMC_ModuleCheck=0;
		Delay_BMS_ModuleCheck=0;
	}
#ifdef 	TPMSModule_CONNECT
	if(GetTPMSTime>=WALTERsample)
	{
		GetTPMSTime=WALTERsample ;
		TPMS_Err_Flag=0;						//清胎压错误标志
		TPMS_Moudle_Err_Flag=1;
		AirPress_ModuleErrorFlag=1;
		for(i=0;i<TYRE_COUNT;i++)
		{
			TyreState_Flag[i]=0;
			TyreGetData[i].TyrePressure=0;
			TyreGetData[i].TyreTemperature=0;
			TyreGetData[i].TyreStateFlag.Data=0;
		} 
	}
	else
	{
		TPMS_Moudle_Err_Flag=0;
		AirPress_ModuleErrorFlag=0;
//		GetTPMS_data_FL=1;
//		GetTPMS_FL=1;
	}	
#endif
	if(OnKeyBit==KeyOff&&AccKeyBit==KeyOff&&DangerWarnKeyBit==KeyOff/*(LeftTurnLampKeyBit==KeyOff)&&(RightTurnLampKeyBit==KeyOff)*/)						
	 	return;
	///front dirve check
  #ifdef	FRONTMODULE_CONNECT
  if(FrontDriveModuleFlag==1)
  {        									
    FrontDriveModuleFlag=0;
    FrontDriveErrorFlag=0;
    FrontModuleCheck_Time=0;
  }
  else
  {
    if(FrontModuleCheck_Time>TerminalConnectDetectTimeNum)
    {
      FrontModuleCheck_Time=RandomSetTimeNum;
      FrontDriveErrorFlag=1;   
    }
  }
	#endif
	///top drive check
	#ifdef	TOPMODULE_CONNECT
	if(TopDriveModuleFlag==1)
  {        									
    TopDriveModuleFlag=0;
    TopDriveErrorFlag=0;
    TopModuleCheck_Time=0;
  }
  else
  {
    if(TopModuleCheck_Time>TerminalConnectDetectTimeNum)
    {
      TopModuleCheck_Time=RandomSetTimeNum;
      TopDriveErrorFlag=1;   
    }
  }
	#endif
	///back drive check
	#ifdef	BACKMODULE_CONNECT
	if(BackDriveModuleFlag==1)
  {        									
    BackDriveModuleFlag=0;
    BackDriveErrorFlag=0;
    BackModuleCheck_Time=0;
  }
  else
  {
    if(BackModuleCheck_Time>TerminalConnectDetectTimeNum)
    {
      BackModuleCheck_Time=RandomSetTimeNum;
      BackDriveErrorFlag=1;   
    }
  }
	#endif
  ///power module check
  #ifdef	POWERMODULE_CONNECT
	if(PowerModuleFlag==1)
  {        									
    PowerModuleFlag=0;
    PowerErrorFlag=0;
    PowerModuleCheck_Time=0;
  }
  else
  {
    if(PowerModuleCheck_Time>TerminalConnectDetectTimeNum)
    {
      PowerModuleCheck_Time=RandomSetTimeNum;
      PowerErrorFlag=1;   
    }
  }
	#endif
  ///powerContrl module check
  #ifdef	POWERCONTRLMODULE_CONNECT
	if(PowerContrlModuleFlag==1)
  {        									
    PowerContrlModuleFlag=0;
    PowerContrlErrorFlag=0;
    PowerContrlModuleCheck_Time=0;
  }
  else
  {
    if(PowerContrlModuleCheck_Time>TerminalConnectDetectTimeNum)
    {
      PowerContrlModuleCheck_Time=RandomSetTimeNum;
      PowerContrlErrorFlag=1;   
    }
  }
	#endif	
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
static void Message_SpecialState_process(void)
{
	unsigned char i;
	
	///前控模块故障的数据归零
	if(FrontDriveErrorFlag==1)					
  {	   
    for(i=0;i<DRIVE_KEY_NUM;i++)
	   	FrontDriveKey[i].Data=0x00;
//		LeftFrontStopHoofKeyBit=KeyOn;
//		RightFrontStopHoofKeyBit=KeyOn;
		
		for(i=0;i<DRIVELOADNUM;i++)
			FrontDriveLoad[i].Data=0xff;
		FrontDriveModuleTemperatureFlag=0;
		QianQuWenDu[0]=0;
		QianQuWenDu[1]=0;
		for(i=0;i<8;i++)
			QianQuBanBen[i]=0X0B;//FONT_WORD_HENG
  } 
	///后控模块故障的数据归零
	if(BackDriveErrorFlag==1)					
  {	   
    for(i=0;i<DRIVE_KEY_NUM;i++)
	   	BackDriveKey[i].Data=0x00;
		BackRoomKeyBit=KeyOn;
//		LeftBackStopHoofKeyBit =KeyOn;
//		RightBackStopHoofKeyBit=KeyOn;			
		for(i=0;i<DRIVELOADNUM;i++)
			BackDriveLoad[i].Data=0xff;
		BackDriveModuleTemperatureFlag=0;
		HouQuWenDu[0]=0;
		HouQuWenDu[1]=0;
		for(i=0;i<8;i++)
			HouQuBanBen[i]=0X0B;//FONT_WORD_HENG	
  }
	///顶控模块故障的数据归零
	if(TopDriveErrorFlag==1)					
  {	   
    for(i=0;i<DRIVE_KEY_NUM;i++)
	   	TopDriveKey[i].Data=0x00;
		for(i=0;i<DRIVELOADNUM;i++)
			TopDriveLoad[i].Data=0xff;
		TopDriveModuleTemperatureFlag=0;
		DingQuWenDu[0]=0;
		DingQuWenDu[1]=0;
		for(i=0;i<8;i++)
			DingQuBanBen[i]=0X0B;//FONT_WORD_HENG	
  }
	///配电模块故障的数据归零
	if(PowerErrorFlag==1)					
  {	   
	  PowerKey[0].Data=0x00;
		for(i=0;i<POWER_LOAD_NUM;i++)
			PowerLoad[i].Data=0xff;
		for(i=0;i<POWER_FUSE_NUM;i++)
			PowerFuse[i].Data=0xff;
		PowerModuleTemperatureFlag=0;
		PeiDianWenDu[0]=0;
		PeiDianWenDu[1]=0;
		for(i=0;i<8;i++)
			PeiDianBanBen[i]=0X0B;//FONT_WORD_HENG	
  }
	///电源管理模块故障的数据归零
	if(PowerContrlErrorFlag==1)					
  {	   
	  PowerContrlKey[0].Data=0x00;
		for(i=0;i<POWERCONTRL_LOAD_NUM;i++)
			PowerContrlLoad[i].Data=0xff;
		PowerContrlOverLoadFlag.Data=0X00;
		for(i=0;i<8;i++)
			PowerContrlCurrent[i]=0x00;
		PowerContrlModuleTemperatureFlag=0;
		PowerContrlWenDu[0]=0;
		PowerContrlWenDu[1]=0;
		for(i=0;i<8;i++)
			PowerContrlBanBen[i]=0X0B;//FONT_WORD_HENG
  }	
	///发动机ECU故障时，对其接收数据的归零
	if(ENGINE_ECU_CANErrorFlag==1)
	{
		engSpeed=0x00;					
		EngRotation();
		Rotation_Data_Flag=1;
		watTemp=0x00;						
		WaterTemperatureFlag=0;
		if(oil_j1939_flag==1)
			oilPres=0x00;
		total_fual_used=0;
		fual_value=0;
		Engine_Diagnose_Lamp.Data=0x00;
		EngineDiagnose_PGN=0;
	}
	else
	{
		WaterTempFlahFlag=0;///对水温以1HZ闪烁的时间初始化
		WaterTempFlahTime=0;
	}
	///SCR故障时，对其接收数据的归零
	if(SCRCANModuleErrorFlag==1)
	{
		CatalystTankLevel=0;
		CatalystTankLevelFlag=0;
		SCRDiagnose_PGN=0;
		SCR_Diagnose_Lamp.Data=0x00;
		CatalystTankLevelFlag=0;
		CatalystTankLevelFlag2 =0;
	}
	///BMS故障时，对其接收数据归零
	if(BMS_CAN_ModuleErrorFlag==1)
	{
		HighestChargeVolt=0;
		BatteryNominalVolt=0;
		HighestWorkTemp=40;
		LowestWorkTemp=40;
		BatteryPitchNum=0;
		LECUnumber=0;
		
		SingleBatHighestVolt=0;
		SingleBatLowestVolt=0;
		BatteryNominalCurr=0;
		BatterySOH=0;

		BatteryPresentVolt=0;
		BatteryPresentCurr=5000;
		BatteryPresentSOC=0;
		BatteryPresentHighestTemp=40;
		BatteryPresentLowestTemp=40;

		SingleBatPresentHighestVolt=0;
		SingleBatPresentLowestVolt=0;
		PositiveBusInsulationRES=0;
		MinusBusInsulationRES=0;
		
		BatteryBMSCheckSum=0;
		BatteryBMSCount=0;
		BatteryBMSStatus1.Data=0;
		BatteryBMSStatus2.Data=0;
		BatteryBMSStatus3.Data=0;
	}	
	///DMC故障时，对其接收数据归零
	if(DMC_CAN_ModuleErrorFlag==1)
	{
		BMSContrllerFaultNumber=0;
		BMSContrllerWarnNumber=0;
		BMSContrllerCarspeed=0;
		carspeed=0;
		CARSPEED();
		BMSContrllerGears=0;
		DriverSystemFaultNumber=0;
		BMSContrllerEngineSpeed=0;
		engSpeed=0;
		EngRotation();

		InverterTemperature=40;
		Motor1InverterRadiatorTemp=40;
		Motor1WindingResTemp=40;
		Motor2InverterRadiatorTemp=40;
		Motor2WindingResTemp=40;
		ConnecterStatus=0;
		OperationStatus=0;
	}
	///HCU故障时，对其接收数据归零
	///if(FULLCONTROL_CAN_ModuleErrorFlag==1)
	{
		///carspeed=0;
		///CARSPEED();
		///OvercarspeedFlag=0;
		
		
	}
	///辅助控制故障时，对其接收数据归零
	///if(ASSISTANT_CAN_ModuleErrorFlag==1)
	{
		
	}
	///辅助控制故障时，对其接收数据归零
	///if(ISG_CAN_ModuleErrorFlag==1)
	{
		
	}
	///里程配置：清零与具体数据的配置
	if(ClearEepromFlag==1)
	{
		if(EepromClearFlag==0)
			ClearDistance();
		else
			ConfigDistance();
		ClearEepromFlag=0;
		ClearEepromOkFlag=1;
		SendConfigDataFlag=1;
		ConfigDataResult=0xfe;
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
static void push_dis(uchar tmp)
{
  uchar i;
  dis_count++;			            	
  for(i=dis_count;i>0;i--)
	{
     dis_content[i]=dis_content[i-1];
  }
  dis_content[0]=tmp;		           ///new display data push to front
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
static void pop_dis(uchar tmp)
{
	uchar i;
	uchar 	dis_location;
	
	if(dis_count>0)
	{
		for(i=0;i<dis_count;i++)
		{
			if(tmp==dis_content[i])
			{
				dis_location=i; 								///find cancel mulfunction comment
				break;
			}
		}
		dis_content[dis_count]=0x00;				///cancel mulfunction
		
		for(i=dis_location;i<dis_count;i++)
		{
			dis_content[i]=dis_content[i+1];	///rear mulfunction rising in turn
		}
		dis_count--;
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
static void ConnetDisplayDeal(void)
{
	unsigned char i,ErrorArray,ErrorNum,VM_DetectNum;///VM->Vehicle module
	unsigned char buf_tmp2=0,buf_tmp1=0;
	uchar	DispNumber=0;

	if(OnKeyBit==KeyOff)
    return;  
	
	ErrorArray=0;
	ErrorNum=0;
	DispNumber=WuQianQuDongMoKuai;
	///vehicle module error display 
	if(TempVehicleModuleError.Data!=VehicleModuleError.Data)
	{
		for(VM_DetectNum=0;VM_DetectNum<VehicleModuleNum;VM_DetectNum++)
		{

			buf_tmp2=(TempVehicleModuleError.Data>>VM_DetectNum)&0x01;
			buf_tmp1=(VehicleModuleError.Data>>VM_DetectNum)&0x01;
			if(buf_tmp1==0x00)
			{
				First_ErrorOld[ErrorArray]&=~(0x01<<ErrorNum);	
				if(buf_tmp2!=0x00)
				{
					pop_dis(DispNumber);       		 //clear status
					First_Error[ErrorArray]&=~(0x01<<ErrorNum);	
				}
			}
			else
			{
				First_ErrorOld[ErrorArray]|=0x01<<ErrorNum;
				if(buf_tmp2==0x00)
				{
					push_dis(DispNumber);
					First_Error[ErrorArray]|=0x01<<ErrorNum;						
				}
			}
			DispNumber++;	
			ErrorNum++;			
		}
		TempVehicleModuleError.Data=VehicleModuleError.Data;
	}
	///J1939 CAN error display
	ErrorArray=1;
	ErrorNum=0;
	DispNumber=FaDongJiECUGuZhan;
	for(i=0;i<sizeof(J1939ModuleError);i++)
	{
		if(TempJ1939ModuleError[i].Data!=J1939ModuleError[i].Data)
		{
			for(VM_DetectNum=0;VM_DetectNum<8;VM_DetectNum++)
			{

				buf_tmp2=(TempJ1939ModuleError[i].Data>>VM_DetectNum)&0x01;
				buf_tmp1=(J1939ModuleError[i].Data>>VM_DetectNum)&0x01;
				if(buf_tmp1==0x00)
				{
					First_ErrorOld[ErrorArray]&=~(0x01<<ErrorNum);	
					if(buf_tmp2!=0x00)
					{
						pop_dis(DispNumber);       		 //clear status
						First_Error[ErrorArray]&=~(0x01<<ErrorNum);	
					}
				}
				else
				{
					First_ErrorOld[ErrorArray]|=0x01<<ErrorNum;
					if(buf_tmp2==0x00)
					{
						push_dis(DispNumber);
						First_Error[ErrorArray]|=0x01<<ErrorNum;						
					}
				}
				DispNumber++;	
				ErrorNum++;		
			}
			TempJ1939ModuleError[i].Data=J1939ModuleError[i].Data;
		}
		else
		{
			DispNumber+=8;					
			ErrorNum+=8;						
		}
		if(ErrorNum>=8)
		{
			ErrorNum=0;
			ErrorArray++;
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
static void	MeterErrorDisplayDeal(void)
{
	uchar	i,ErrorArray,ErrorNum,StDetectNum,buf_tmp1=0,buf_tmp2=0;
	uchar	DispNumber=PanalLoadMulfunction_01;///the first malfunction number of drive module

	if(OnKeyBit==KeyOff)
  	return;
	
	ErrorNum=0; 
  ErrorArray=0;

	ReState[1].Data=0XFF;
	if(VideoPower1ErrorFlag==1)
	{
		ReState[1].St.S0=Load_Short;
	}
	if(VideoPower2ErrorFlag==1)
	{
		ReState[1].St.S1=Load_Short;
	}
	if(V_POWE4_ErrorFlag==1)//车速电源
	{
		ReState[1].St.S2=Load_Short;
	}
#ifdef _INIT_TEST
	ReState[1].Data=0X55;
#endif
	for(i=0;i<=Num_PowerDevice;i++)
	{
		if(TempReState[i].Data!=ReState[i].Data)
		{
			for(StDetectNum=0;StDetectNum<6;StDetectNum+=2)
			{	
				buf_tmp2=(TempReState[i].Data>>StDetectNum)&0x03;
				buf_tmp1=(ReState[i].Data>>StDetectNum)&0x03;
				if(buf_tmp1==0x03||buf_tmp1==0x00)   //normal status
				{					
					SmallErrorOld[ErrorArray]&=~(0x01<<ErrorNum);				
					if(buf_tmp2!=0x03&&buf_tmp2!=0x00)
					{
						pop_dis(DispNumber);       		 //clear status
						SmallError[ErrorArray]&=~(0x01<<ErrorNum);					
					}
				}
				else
				{				
					SmallErrorOld[ErrorArray]|=0x01<<ErrorNum;
					if(buf_tmp2==0x00||buf_tmp2==0x03)
					{
						push_dis(DispNumber);
						if(buf_tmp1==0x02)											////when open,the bit is 10
							OpenError[DispNumber]=OpenError_Num;						
						else if(buf_tmp1==0x01)									///when short,the bit is 01
							OpenError[DispNumber]=ShortError_Num;													
						SmallError[ErrorArray]|=0x01<<ErrorNum;						
					}
					else 
					{
						if(buf_tmp2!=buf_tmp1)
						{
							pop_dis(DispNumber);												///clear status
							SmallError[ErrorArray]&=~(0x01<<ErrorNum);				
							push_dis(DispNumber);
							if(buf_tmp1==0x02)														///when open,the bit is 10
								OpenError[DispNumber]=OpenError_Num;						
							else if(buf_tmp1==0x01)												///when short,the bit is 01
								OpenError[DispNumber]=ShortError_Num;														
							SmallError[ErrorArray]|=0x01<<ErrorNum;	
						}		
					}
				}
				DispNumber++;	
				ErrorNum++;									
			}
			TempReState[i].Data=ReState[i].Data;
		}
		else
		{
			DispNumber=DispNumber+3;
			ErrorNum =ErrorNum+3;
		}
	}

	if(st_status!=st_statusOld)
	{
		if(st_status==EngineIdleNum)
		{
			if(st_statusOld==EngineStartProcessNum)
				pop_dis(MeterStartAlarmNum);
			else if(st_statusOld==EngineStartTooLongNum)
				pop_dis(MeterStartTooLongAlarmNum);
			else if(st_statusOld==EngineStartNShieldNum)
				pop_dis(MeterNdangAlarmNum);
			else if(st_statusOld==EngineRestartStateNum)
				pop_dis(MeterReStartAlarmNum);
		}
		else if(st_status==EngineStartProcessNum)
		{
			if(st_statusOld==EngineIdleNum)
				push_dis(MeterStartAlarmNum);
			else if(st_statusOld==EngineStartNShieldNum)
			{
				pop_dis(MeterNdangAlarmNum);
				push_dis(MeterStartAlarmNum);
			}
			else if(st_statusOld==EngineRestartStateNum)
			{
				pop_dis(MeterReStartAlarmNum);
				push_dis(MeterStartAlarmNum);
			}
		}
		else if(st_status==EngineStartNShieldNum)
		{
			if(st_statusOld==EngineIdleNum)
			{
				push_dis(MeterNdangAlarmNum);
			}
			else if(st_statusOld==EngineStartProcessNum)
			{
				pop_dis(MeterStartAlarmNum);
				push_dis(MeterNdangAlarmNum);
			}
		}
		else if(st_status==EngineRestartStateNum)
		{
			if(st_statusOld==EngineIdleNum)
			{
				push_dis(MeterReStartAlarmNum);
			}
			else if(st_statusOld==EngineStartProcessNum)
			{
				pop_dis(MeterStartAlarmNum);
				push_dis(MeterReStartAlarmNum);
			}
		}
		else if(st_status==EngineStartTooLongNum)
		{
			if(st_statusOld==EngineStartProcessNum)
			{
				pop_dis(MeterStartAlarmNum);
				push_dis(MeterStartTooLongAlarmNum);		
			}
		}
	}
	st_statusOld=st_status;	
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
static void	DriveErrorDisplayDeal(void)
{
	uchar	i,ErrorArray,ErrorNum,StDetectNum,buf_tmp1=0,buf_tmp2=0;
	uchar	DispNumber=0;
	uchar LoadOpenNum;
	
	if(OnKeyBit==KeyOff)
    return;     
   
  ErrorNum=0; 
  ErrorArray=1;																			///1 be occupied in"connect module error and panal load"
	DispNumber=FrontDriveMulfunction_01;							///total load number 18
  ///front drive  
	for(i=0;i<DRIVELOADNUM;i++)												////front drive load malfunction
	{	
		if(FrontDriveLoadBuf[i].Data!=FrontDriveLoad[i].Data)     ///frontdriveload is current data
		{
			for(StDetectNum=0;StDetectNum<8;StDetectNum+=2)
			{	
				buf_tmp2=(FrontDriveLoadBuf[i].Data>>StDetectNum)&0x03;
				buf_tmp1=(FrontDriveLoad[i].Data>>StDetectNum)&0x03;
				if(buf_tmp1==0x03||buf_tmp1==0x00)   //normal status
				{					
					SmallErrorOld[ErrorArray]&=~(0x01<<ErrorNum);				
					if(buf_tmp2!=0x03&&buf_tmp2!=0x00)
					{
						//开路
						if(buf_tmp2==0x02)
						{
							LoadOpenNum=i<<2;
							LoadOpenNum=LoadOpenNum+(StDetectNum>>1);
//							LoadOpenNum+=FrontDriverMoudle;
							
							if(LoadOpenNum==LeftFarLamp1NUM||LoadOpenNum==LeftFarLamp2NUM)
								pop_dis(LeftFarLampOpenLoad);					//左远光灯开路							
							else if(LoadOpenNum==RightFarLamp1NUM||LoadOpenNum==RightFarLamp2NUM)
								pop_dis(RightFarLampOpenLoad);				//右远光灯开路	
							else if(LoadOpenNum==LeftNearLamp1NUM)	//左近光灯开路
								pop_dis(LeftNearLampOpenLoad);				
							else if(LoadOpenNum==RightNearLamp1NUM)	//右近光灯
								pop_dis(RightNearLampOpenLoad);				
							else if(LoadOpenNum==LeftFrontFogLamp1NUM)//左前雾灯开路
								pop_dis(LeftFrontFogLampOpenLoad);								
							else if(LoadOpenNum==RightFrontFogLamp1NUM)
								pop_dis(RightFrontFogLampOpenLoad);			//右前雾灯
							else
								pop_dis(DispNumber);				
						}
						else
							pop_dis(DispNumber);        //clear status
						SmallError[ErrorArray]&=~(0x01<<ErrorNum);									
					}
				}
				else
				{				
					SmallErrorOld[ErrorArray]|=0x01<<ErrorNum;
					if(buf_tmp2==0x00||buf_tmp2==0x03)
					{						
						if(buf_tmp1==0x02)											////when open,the bit is 10
						{
							LoadOpenNum=i<<2;
							LoadOpenNum=LoadOpenNum+(StDetectNum>>1);
//							LoadOpenNum+=FrontDriverMoudle;
							
							if(LoadOpenNum==LeftFarLamp1NUM||LoadOpenNum==LeftFarLamp2NUM)
								push_dis(LeftFarLampOpenLoad);					//左远光灯开路							
							else if(LoadOpenNum==RightFarLamp1NUM||LoadOpenNum==RightFarLamp2NUM)
								push_dis(RightFarLampOpenLoad);				//右远光灯开路	
							else if(LoadOpenNum==LeftNearLamp1NUM)	//左近光灯开路
								push_dis(LeftNearLampOpenLoad);				
							else if(LoadOpenNum==RightNearLamp1NUM)	//右近光灯
								push_dis(RightNearLampOpenLoad);				
							else if(LoadOpenNum==LeftFrontFogLamp1NUM)//左前雾灯开路
								push_dis(LeftFrontFogLampOpenLoad);								
							else if(LoadOpenNum==RightFrontFogLamp1NUM)
								push_dis(RightFrontFogLampOpenLoad);			//右前雾灯					
							else
								push_dis(DispNumber);				//clear status
							OpenError[DispNumber]=OpenError_Num;	
						}												
						else// if(buf_tmp1==0x01)									///when short,the bit is 01

						{
							OpenError[DispNumber]=ShortError_Num;			
							push_dis(DispNumber);
						}
						SmallError[ErrorArray]|=0x01<<ErrorNum;						
					}
					else 
					{
						if(buf_tmp2!=buf_tmp1)
						{
							LoadOpenNum=i<<2;
							LoadOpenNum=LoadOpenNum+(StDetectNum>>1);
//							LoadOpenNum+=FrontDriverMoudle;							
							if(buf_tmp2==0x02)
							{								
								if(LoadOpenNum==LeftFarLamp1NUM||LoadOpenNum==LeftFarLamp2NUM)
									pop_dis(LeftFarLampOpenLoad); 				//左远光灯开路							
								else if(LoadOpenNum==RightFarLamp1NUM||LoadOpenNum==RightFarLamp2NUM)
									pop_dis(RightFarLampOpenLoad);				//右远光灯开路	
								else if(LoadOpenNum==LeftNearLamp1NUM)	//左近光灯开路
									pop_dis(LeftNearLampOpenLoad);				
								else if(LoadOpenNum==RightNearLamp1NUM) //右近光灯
									pop_dis(RightNearLampOpenLoad); 			
								else if(LoadOpenNum==LeftFrontFogLamp1NUM)//左前雾灯开路
									pop_dis(LeftFrontFogLampOpenLoad);								
								else if(LoadOpenNum==RightFrontFogLamp1NUM)
									pop_dis(RightFrontFogLampOpenLoad); 		//右前雾灯
								else
									pop_dis(DispNumber);			
								
								OpenError[DispNumber]=ShortError_Num; 
								push_dis(DispNumber);
								
							}
							else
							{
								pop_dis(DispNumber);				//clear status

								if(LoadOpenNum==LeftFarLamp1NUM||LoadOpenNum==LeftFarLamp2NUM)
									push_dis(LeftFarLampOpenLoad);					//左远光灯开路							
								else if(LoadOpenNum==RightFarLamp1NUM||LoadOpenNum==RightFarLamp2NUM)
									push_dis(RightFarLampOpenLoad);				//右远光灯开路	
								else if(LoadOpenNum==LeftNearLamp1NUM)	//左近光灯开路
									push_dis(LeftNearLampOpenLoad);				
								else if(LoadOpenNum==RightNearLamp1NUM)	//右近光灯
									push_dis(RightNearLampOpenLoad);				
								else if(LoadOpenNum==LeftFrontFogLamp1NUM)//左前雾灯开路
									push_dis(LeftFrontFogLampOpenLoad);								
								else if(LoadOpenNum==RightFrontFogLamp1NUM)
									push_dis(RightFrontFogLampOpenLoad);			//右前雾灯					
								else
									push_dis(DispNumber);				//clear status
									
								OpenError[DispNumber]=OpenError_Num;										
							}
							SmallError[ErrorArray]|=0x01<<ErrorNum;
						}
					}
				}
				DispNumber++;	
				ErrorNum++;																		
			}
			FrontDriveLoadBuf[i].Data=FrontDriveLoad[i].Data; 	
		}
		else
		{
			DispNumber+=4;					////the malfunction is 4 of an array 
			ErrorNum+=4;						////the error bit is 4 of an array					
		}
		if(ErrorNum>=8)
		{
			ErrorNum=0;
			ErrorArray++;
		}	
	}	
	///back drive 
	ErrorNum=0; 
  ErrorArray=1+3;																		///3 be occupied in"front drive"
	DispNumber=BackDriveMulfunction_01;						///total load number 20
	for(i=0;i<DRIVELOADNUM;i++)												////back drive load malfunction
	{	
		if(BackDriveLoadBuf[i].Data!=BackDriveLoad[i].Data)     ///backdriveload is current data
		{
			for(StDetectNum=0;StDetectNum<8;StDetectNum+=2)
			{	
				buf_tmp2=(BackDriveLoadBuf[i].Data>>StDetectNum)&0x03;
				buf_tmp1=(BackDriveLoad[i].Data>>StDetectNum)&0x03;
				if(buf_tmp1==0x03||buf_tmp1==0x00)   //normal status
				{					
					SmallErrorOld[ErrorArray]&=~(0x01<<ErrorNum);				
					if(buf_tmp2!=0x03&&buf_tmp2!=0x00)
					{
						//开路
						if(buf_tmp2==0x02)
						{
							LoadOpenNum=i<<2;
							LoadOpenNum=LoadOpenNum+(StDetectNum>>1);
							LoadOpenNum+=BackDriverMoudle;
							
							if(LoadOpenNum==LeftFarLamp1NUM||LoadOpenNum==LeftFarLamp2NUM)
								pop_dis(LeftFarLampOpenLoad);					//左远光灯开路							
							else if(LoadOpenNum==RightFarLamp1NUM||LoadOpenNum==RightFarLamp2NUM)
								pop_dis(RightFarLampOpenLoad);				//右远光灯开路	
							else if(LoadOpenNum==LeftNearLamp1NUM)	//左近光灯开路
								pop_dis(LeftNearLampOpenLoad);				
							else if(LoadOpenNum==RightNearLamp1NUM)	//右近光灯
								pop_dis(RightNearLampOpenLoad);				
							else if(LoadOpenNum==LeftFrontFogLamp1NUM)//左前雾灯开路
								pop_dis(LeftFrontFogLampOpenLoad);								
							else if(LoadOpenNum==RightFrontFogLamp1NUM)
								pop_dis(RightFrontFogLampOpenLoad);			//右前雾灯
							else
								pop_dis(DispNumber);				
						}
						else
							pop_dis(DispNumber);        //clear status
						SmallError[ErrorArray]&=~(0x01<<ErrorNum);							
					}
				}
				else
				{ 			
					SmallErrorOld[ErrorArray]|=0x01<<ErrorNum;
					if(buf_tmp2==0x00||buf_tmp2==0x03)
					{ 					
						if(buf_tmp1==0x02)											////when open,the bit is 10
						{
							LoadOpenNum=i<<2;
							LoadOpenNum=LoadOpenNum+(StDetectNum>>1);
							LoadOpenNum+=BackDriverMoudle;
							
							if(LoadOpenNum==LeftFarLamp1NUM||LoadOpenNum==LeftFarLamp2NUM)
								push_dis(LeftFarLampOpenLoad);					//左远光灯开路							
							else if(LoadOpenNum==RightFarLamp1NUM||LoadOpenNum==RightFarLamp2NUM)
								push_dis(RightFarLampOpenLoad); 			//右远光灯开路	
							else if(LoadOpenNum==LeftNearLamp1NUM)	//左近光灯开路
								push_dis(LeftNearLampOpenLoad); 			
							else if(LoadOpenNum==RightNearLamp1NUM) //右近光灯
								push_dis(RightNearLampOpenLoad);				
							else if(LoadOpenNum==LeftFrontFogLamp1NUM)//左前雾灯开路
								push_dis(LeftFrontFogLampOpenLoad); 							
							else if(LoadOpenNum==RightFrontFogLamp1NUM)
								push_dis(RightFrontFogLampOpenLoad);			//右前雾灯					
							else
								push_dis(DispNumber); 			//clear status
							OpenError[DispNumber]=OpenError_Num;	
						} 											
						else// if(buf_tmp1==0x01) 								///when short,the bit is 01

						{
							OpenError[DispNumber]=ShortError_Num; 		
							push_dis(DispNumber);
						}
						SmallError[ErrorArray]|=0x01<<ErrorNum; 					
					}
					else 
					{
						if(buf_tmp2!=buf_tmp1)
						{
							LoadOpenNum=i<<2;
							LoadOpenNum=LoadOpenNum+(StDetectNum>>1);
							LoadOpenNum+=BackDriverMoudle; 						
							if(buf_tmp2==0x02)
							{ 							
								if(LoadOpenNum==LeftFarLamp1NUM||LoadOpenNum==LeftFarLamp2NUM)
									pop_dis(LeftFarLampOpenLoad); 				//左远光灯开路							
								else if(LoadOpenNum==RightFarLamp1NUM||LoadOpenNum==RightFarLamp2NUM)
									pop_dis(RightFarLampOpenLoad);				//右远光灯开路	
								else if(LoadOpenNum==LeftNearLamp1NUM)	//左近光灯开路
									pop_dis(LeftNearLampOpenLoad);				
								else if(LoadOpenNum==RightNearLamp1NUM) //右近光灯
									pop_dis(RightNearLampOpenLoad); 			
								else if(LoadOpenNum==LeftFrontFogLamp1NUM)//左前雾灯开路
									pop_dis(LeftFrontFogLampOpenLoad);								
								else if(LoadOpenNum==RightFrontFogLamp1NUM)
									pop_dis(RightFrontFogLampOpenLoad); 		//右前雾灯
								else
									pop_dis(DispNumber);			
								
								OpenError[DispNumber]=ShortError_Num; 
								push_dis(DispNumber);
								
							}
							else
							{
								pop_dis(DispNumber);				//clear status

								if(LoadOpenNum==LeftFarLamp1NUM||LoadOpenNum==LeftFarLamp2NUM)
									push_dis(LeftFarLampOpenLoad);					//左远光灯开路							
								else if(LoadOpenNum==RightFarLamp1NUM||LoadOpenNum==RightFarLamp2NUM)
									push_dis(RightFarLampOpenLoad); 			//右远光灯开路	
								else if(LoadOpenNum==LeftNearLamp1NUM)	//左近光灯开路
									push_dis(LeftNearLampOpenLoad); 			
								else if(LoadOpenNum==RightNearLamp1NUM) //右近光灯
									push_dis(RightNearLampOpenLoad);				
								else if(LoadOpenNum==LeftFrontFogLamp1NUM)//左前雾灯开路
									push_dis(LeftFrontFogLampOpenLoad); 							
								else if(LoadOpenNum==RightFrontFogLamp1NUM)
									push_dis(RightFrontFogLampOpenLoad);			//右前雾灯					
								else
									push_dis(DispNumber); 			//clear status
									
								OpenError[DispNumber]=OpenError_Num;										
							}
							SmallError[ErrorArray]|=0x01<<ErrorNum;
						}
					}
				}
				DispNumber++;	
				ErrorNum++;																		
			}
			BackDriveLoadBuf[i].Data=BackDriveLoad[i].Data; 	
		}
		else
		{
			DispNumber+=4;					////the malfunction is 4 of an array 
			ErrorNum+=4;						////the error bit is 4 of an array					
		}
		if(ErrorNum>=8)
		{
			ErrorNum=0;
			ErrorArray++;
		}	
	}	
	///Top drive 
#ifdef TOPMODULE_CONNECT	
	ErrorNum=0; 
  ErrorArray=1+3*2;																	///3 be occupied in"back drive"
	DispNumber=TopDriveMulfunction_01;					///total load number 20
	for(i=0;i<DRIVELOADNUM;i++)												////top drive load malfunction
	{	
		if(TopDriveLoadBuf[i].Data!=TopDriveLoad[i].Data)     ///TOPdriveload is current data
		{
			for(StDetectNum=0;StDetectNum<8;StDetectNum+=2)
			{	
				buf_tmp2=(TopDriveLoadBuf[i].Data>>StDetectNum)&0x03;
				buf_tmp1=(TopDriveLoad[i].Data>>StDetectNum)&0x03;
				if(buf_tmp1==0x03||buf_tmp1==0x00)   //normal status
				{					
					SmallErrorOld[ErrorArray]&=~(0x01<<ErrorNum);				
					if(buf_tmp2!=0x03&&buf_tmp2!=0x00)
					{
						//开路
						if(buf_tmp2==0x02)
						{
							LoadOpenNum=i<<2;
							LoadOpenNum=LoadOpenNum+(StDetectNum>>1);
							LoadOpenNum+=TopDriverMoudle;
							
							if(LoadOpenNum==LeftFarLamp1NUM||LoadOpenNum==LeftFarLamp2NUM)
								pop_dis(LeftFarLampOpenLoad);					//左远光灯开路							
							else if(LoadOpenNum==RightFarLamp1NUM||LoadOpenNum==RightFarLamp2NUM)
								pop_dis(RightFarLampOpenLoad);				//右远光灯开路	
							else if(LoadOpenNum==LeftNearLamp1NUM)	//左近光灯开路
								pop_dis(LeftNearLampOpenLoad);				
							else if(LoadOpenNum==RightNearLamp1NUM)	//右近光灯
								pop_dis(RightNearLampOpenLoad);				
							else if(LoadOpenNum==LeftFrontFogLamp1NUM)//左前雾灯开路
								pop_dis(LeftFrontFogLampOpenLoad);								
							else if(LoadOpenNum==RightFrontFogLamp1NUM)
								pop_dis(RightFrontFogLampOpenLoad);			//右前雾灯
							else
								pop_dis(DispNumber);				
						}
						else
							pop_dis(DispNumber);        //clear status
						SmallError[ErrorArray]&=~(0x01<<ErrorNum);							
					}
				}
				else
				{ 			
					SmallErrorOld[ErrorArray]|=0x01<<ErrorNum;
					if(buf_tmp2==0x00||buf_tmp2==0x03)
					{ 					
						if(buf_tmp1==0x02)											////when open,the bit is 10
						{
							LoadOpenNum=i<<2;
							LoadOpenNum=LoadOpenNum+(StDetectNum>>1);
							LoadOpenNum+=TopDriverMoudle;
							
							if(LoadOpenNum==LeftFarLamp1NUM||LoadOpenNum==LeftFarLamp2NUM)
								push_dis(LeftFarLampOpenLoad);					//左远光灯开路							
							else if(LoadOpenNum==RightFarLamp1NUM||LoadOpenNum==RightFarLamp2NUM)
								push_dis(RightFarLampOpenLoad); 			//右远光灯开路	
							else if(LoadOpenNum==LeftNearLamp1NUM)	//左近光灯开路
								push_dis(LeftNearLampOpenLoad); 			
							else if(LoadOpenNum==RightNearLamp1NUM) //右近光灯
								push_dis(RightNearLampOpenLoad);				
							else if(LoadOpenNum==LeftFrontFogLamp1NUM)//左前雾灯开路
								push_dis(LeftFrontFogLampOpenLoad); 							
							else if(LoadOpenNum==RightFrontFogLamp1NUM)
								push_dis(RightFrontFogLampOpenLoad);			//右前雾灯					
							else
								push_dis(DispNumber); 			//clear status
							OpenError[DispNumber]=OpenError_Num;	
						} 											
						else// if(buf_tmp1==0x01) 								///when short,the bit is 01

						{
							OpenError[DispNumber]=ShortError_Num; 		
							push_dis(DispNumber);
						}
						SmallError[ErrorArray]|=0x01<<ErrorNum; 					
					}
					else 
					{
						if(buf_tmp2!=buf_tmp1)
						{
							LoadOpenNum=i<<2;
							LoadOpenNum=LoadOpenNum+(StDetectNum>>1);
							LoadOpenNum+=TopDriverMoudle; 						
							if(buf_tmp2==0x02)
							{ 							
								if(LoadOpenNum==LeftFarLamp1NUM||LoadOpenNum==LeftFarLamp2NUM)
									pop_dis(LeftFarLampOpenLoad); 				//左远光灯开路							
								else if(LoadOpenNum==RightFarLamp1NUM||LoadOpenNum==RightFarLamp2NUM)
									pop_dis(RightFarLampOpenLoad);				//右远光灯开路	
								else if(LoadOpenNum==LeftNearLamp1NUM)	//左近光灯开路
									pop_dis(LeftNearLampOpenLoad);				
								else if(LoadOpenNum==RightNearLamp1NUM) //右近光灯
									pop_dis(RightNearLampOpenLoad); 			
								else if(LoadOpenNum==LeftFrontFogLamp1NUM)//左前雾灯开路
									pop_dis(LeftFrontFogLampOpenLoad);								
								else if(LoadOpenNum==RightFrontFogLamp1NUM)
									pop_dis(RightFrontFogLampOpenLoad); 		//右前雾灯
								else
									pop_dis(DispNumber);			
								
								OpenError[DispNumber]=ShortError_Num; 
								push_dis(DispNumber);
								
							}
							else
							{
								pop_dis(DispNumber);				//clear status

								if(LoadOpenNum==LeftFarLamp1NUM||LoadOpenNum==LeftFarLamp2NUM)
									push_dis(LeftFarLampOpenLoad);					//左远光灯开路							
								else if(LoadOpenNum==RightFarLamp1NUM||LoadOpenNum==RightFarLamp2NUM)
									push_dis(RightFarLampOpenLoad); 			//右远光灯开路	
								else if(LoadOpenNum==LeftNearLamp1NUM)	//左近光灯开路
									push_dis(LeftNearLampOpenLoad); 			
								else if(LoadOpenNum==RightNearLamp1NUM) //右近光灯
									push_dis(RightNearLampOpenLoad);				
								else if(LoadOpenNum==LeftFrontFogLamp1NUM)//左前雾灯开路
									push_dis(LeftFrontFogLampOpenLoad); 							
								else if(LoadOpenNum==RightFrontFogLamp1NUM)
									push_dis(RightFrontFogLampOpenLoad);			//右前雾灯					
								else
									push_dis(DispNumber); 			//clear status
									
								OpenError[DispNumber]=OpenError_Num;										
							}
							SmallError[ErrorArray]|=0x01<<ErrorNum;
						}
					}
				}
				DispNumber++;	
				ErrorNum++;																		
			}
			TopDriveLoadBuf[i].Data=TopDriveLoad[i].Data; 	
		}
		else
		{
			DispNumber+=4;					////the malfunction is 4 of an array 
			ErrorNum+=4;						////the error bit is 4 of an array					
		}
		if(ErrorNum>=8)
		{
			ErrorNum=0;
			ErrorArray++;
		}	
	}		
#endif	
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
static void PowerErrorDisplayDeal(void)
{
	uchar	i,ErrorArray,ErrorNum,StDetectNum,buf_tmp1=0,buf_tmp2=0;
	uchar	DispNumber=0;

	if(OnKeyBit==KeyOff)
    return; 
	///Power module 
	///power parts load
	ErrorNum=0; 
  ErrorArray=1+3*3;																	///3 be occupied in"middle drive"
	DispNumber=PowerLoadMulfunction_01;					///total load number 20
	for(i=0;i<POWER_LOAD_NUM;i++)											////power load malfunction
	{	
		if(PowerLoadBuf[i].Data!=PowerLoad[i].Data)     ///powerload is current data
		{
			for(StDetectNum=0;StDetectNum<8;StDetectNum+=2)
			{	
				buf_tmp2=(PowerLoadBuf[i].Data>>StDetectNum)&0x03;
				buf_tmp1=(PowerLoad[i].Data>>StDetectNum)&0x03;
				if(buf_tmp1==0x03||buf_tmp1==0x00)   //normal status
				{					
					SmallErrorOld[ErrorArray]&=~(0x01<<ErrorNum);				
					if(buf_tmp2!=0x03&&buf_tmp2!=0x00)
					{
						pop_dis(DispNumber);        //clear status
						SmallError[ErrorArray]&=~(0x01<<ErrorNum);					
					}
				}
				else
				{				
					SmallErrorOld[ErrorArray]|=0x01<<ErrorNum;
					if(buf_tmp2==0x00||buf_tmp2==0x03)
					{
						push_dis(DispNumber);
						if(buf_tmp1==0x02)											////when open,the bit is 10
							OpenError[DispNumber]=OpenError_Num;						
						else if(buf_tmp1==0x01)									///when short,the bit is 01
							OpenError[DispNumber]=ShortError_Num;													
						SmallError[ErrorArray]|=0x01<<ErrorNum;						
					}
					else 
					{
						if(buf_tmp2!=buf_tmp1)
						{
							pop_dis(DispNumber);												///clear status
							SmallError[ErrorArray]&=~(0x01<<ErrorNum);				
							push_dis(DispNumber);
							if(buf_tmp1==0x02)														///when open,the bit is 10
								OpenError[DispNumber]=OpenError_Num;						
							else if(buf_tmp1==0x01)												///when short,the bit is 01
								OpenError[DispNumber]=ShortError_Num;													
							SmallError[ErrorArray]|=0x01<<ErrorNum;	
						}
					}
				}
				DispNumber++;	
				ErrorNum++;																		
			}
			PowerLoadBuf[i].Data=PowerLoad[i].Data; 	
		}
		else
		{
			DispNumber+=4;					////the malfunction is 4 of an array 
			ErrorNum+=4;						////the error bit is 4 of an array					
		}
		if(ErrorNum>=8)
		{
			ErrorNum=0;
			ErrorArray++;
		}	
	}
	///fuse load
	ErrorNum=0; 
  ErrorArray=1+3*4;																	///3 be occupied in"power parts load"
	DispNumber=PowerFuseMulfunction_01;			///total load number 16
	for(i=0;i<POWER_FUSE_NUM;i++)											////power load malfunction
	{	
		if(PowerFuseBuf[i].Data!=PowerFuse[i].Data)     ///powerload is current data
		{
			for(StDetectNum=0;StDetectNum<8;StDetectNum+=2)
			{	
				buf_tmp2=(PowerFuseBuf[i].Data>>StDetectNum)&0x03;
				buf_tmp1=(PowerFuse[i].Data>>StDetectNum)&0x03;
				if(buf_tmp1==0x03||buf_tmp1==0x00)   //normal status
				{					
					SmallErrorOld[ErrorArray]&=~(0x01<<ErrorNum);				
					if(buf_tmp2!=0x03&&buf_tmp2!=0x00)
					{
						pop_dis(DispNumber);        //clear status
						SmallError[ErrorArray]&=~(0x01<<ErrorNum);					
					}
				}
				else
				{				
					SmallErrorOld[ErrorArray]|=0x01<<ErrorNum;
					if(buf_tmp2==0x00||buf_tmp2==0x03)
					{
						push_dis(DispNumber);
						if(buf_tmp1==0x02)											////when open,the bit is 10
							OpenError[DispNumber]=OpenError_Num;						
						else if(buf_tmp1==0x01)									///when short,the bit is 01
							OpenError[DispNumber]=ShortError_Num;													
						SmallError[ErrorArray]|=0x01<<ErrorNum;						
					}
					else 
					{
						if(buf_tmp2!=buf_tmp1)
						{
							pop_dis(DispNumber);												///clear status
							SmallError[ErrorArray]&=~(0x01<<ErrorNum);				
							push_dis(DispNumber);
							if(buf_tmp1==0x02)														///when open,the bit is 10
								OpenError[DispNumber]=OpenError_Num;						
							else if(buf_tmp1==0x01)												///when short,the bit is 01
								OpenError[DispNumber]=ShortError_Num;													
							SmallError[ErrorArray]|=0x01<<ErrorNum;	
						}		
					}
				}
				DispNumber++;	
				ErrorNum++;																		
			}
			PowerFuseBuf[i].Data=PowerFuse[i].Data; 	
		}
		else
		{
			DispNumber+=4;					////the malfunction is 4 of an array 
			ErrorNum+=4;						////the error bit is 4 of an array					
		}
		if(ErrorNum>=8)
		{
			ErrorNum=0;
			ErrorArray++;
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
static void PowerContrlCurrentDeal(void)
{
	uchar i,temp;
	uchar StDetectNum,index;
	StDetectNum=0;
	index=0;
	for(i==0;i<7;i++)
	{
		temp =PowerContrlLoad[index].Data>>StDetectNum;
		temp=temp&0x03;
		if(temp!=Load_Short)
		{			
			if(PowerContrlCurrent[i]>PowerContrlCurrentConfig[i])
				PowerContrlOverLoadFlag.Data|=(0x01<<i);
			else
				PowerContrlOverLoadFlag.Data&=~(0x01<<i);
		}
		StDetectNum=StDetectNum+2;
		if(StDetectNum>=8)
		{
			StDetectNum=0;
			index++;
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
static void PowerContrlErrorDisplayDeal(void)
{
	uchar	i,ErrorArray,ErrorNum,StDetectNum,buf_tmp1=0,buf_tmp2=0;
	uchar	DispNumber;

	if(OnKeyBit==KeyOff)
    return; 
	///Power module 
	///power parts load
	ErrorNum=0; 
  ErrorArray=1+3*5;																	///3 be occupied in"middle drive"
	DispNumber=PowerContrlLoadMulfunction_01;					///total load number 20
	for(i=0;i<POWERCONTRL_LOAD_NUM;i++)											////power load malfunction
	{	
		if(i==2)
		{
			DispNumber=PowerContrlLoadMulfunction_01+21;
			ErrorArray=1+3*5+7;
		}
		if(PowerContrlLoadBuf[i].Data!=PowerContrlLoad[i].Data)     ///powerload is current data
		{
			for(StDetectNum=0;StDetectNum<8;StDetectNum+=2)
			{	
				buf_tmp2=(PowerContrlLoadBuf[i].Data>>StDetectNum)&0x03;
				buf_tmp1=(PowerContrlLoad[i].Data>>StDetectNum)&0x03;
				if(buf_tmp1==0x03||buf_tmp1==0x00)   //normal status
				{					
					SmallErrorOld[ErrorArray]&=~(0x01<<ErrorNum);				
					if(buf_tmp2!=0x03&&buf_tmp2!=0x00)
					{				
						if(OpenError[DispNumber]==OpenError_Num)
							pop_dis(DispNumber+PowerContrlLoadNum*2);        //开路
						else if(OpenError[DispNumber]==OverLoadError_Num)
							pop_dis(DispNumber+PowerContrlLoadNum);      //过载
						else 
							pop_dis(DispNumber);        									 //短路
						SmallError[ErrorArray]&=~(0x01<<ErrorNum);					
					}
				}
				else
				{				
					SmallErrorOld[ErrorArray]|=0x01<<ErrorNum;
					if(buf_tmp2==0x00||buf_tmp2==0x03)
					{						
						if(buf_tmp1==0x02)											////when open,the bit is 10
						{
							OpenError[DispNumber]=OpenError_Num;		
							push_dis(DispNumber+PowerContrlLoadNum*2);
						}
						else if(buf_tmp1==0x01&&((PowerContrlOverLoadFlag.Data&(0x01<<ErrorNum))!=0)&&(i!=2))///when short,the bit is 01
						{
							OpenError[DispNumber]=OverLoadError_Num;		
							push_dis(DispNumber+PowerContrlLoadNum);
						}
						else if(buf_tmp1==0x01)
						{
							OpenError[DispNumber]=ShortError_Num;	
							push_dis(DispNumber);
						}
						SmallError[ErrorArray]|=0x01<<ErrorNum;						
					}
					else 
					{
						if(buf_tmp2!=buf_tmp1)
						{
							SmallError[ErrorArray]&=~(0x01<<ErrorNum);	
							
							if(OpenError[DispNumber]==OpenError_Num)
								pop_dis(DispNumber+PowerContrlLoadNum*2);        //开路
							else if(OpenError[DispNumber]==OverLoadError_Num)
								pop_dis(DispNumber+PowerContrlLoadNum);      	//过载
							else 
								pop_dis(DispNumber);        									 //短路							

							if(buf_tmp1==0x02)											////when open,the bit is 10
							{
								OpenError[DispNumber]=OpenError_Num;		
								push_dis(DispNumber+PowerContrlLoadNum*2);
							}
							else if(buf_tmp1==0x01&&((PowerContrlOverLoadFlag.Data&(0x01<<ErrorNum))!=0)&&(i!=2))///when short,the bit is 01
							{
								OpenError[DispNumber]=OverLoadError_Num;		
								push_dis(DispNumber+PowerContrlLoadNum);
							}
							else if(buf_tmp1==0x01)
							{
								OpenError[DispNumber]=ShortError_Num; 
								push_dis(DispNumber);
							}											
							SmallError[ErrorArray]|=0x01<<ErrorNum;	
						}
					}
				}
				DispNumber++;	
				ErrorNum++;																		
			}
			PowerContrlLoadBuf[i].Data=PowerContrlLoad[i].Data; 	
		}
		else
		{
			DispNumber+=4;					////the malfunction is 4 of an array 
			ErrorNum+=4;						////the error bit is 4 of an array					
		}
		if(ErrorNum>=8)
		{
			ErrorNum=0;
			ErrorArray++;
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
static void	SecondErrorState_Logic(void)
{
	unsigned char i;
	
	if(OnKeyBit==KeyOff)
		return;
	for(i=0;i<SECOND_ERROR_NUM;i++)
		SecondError[i].Data=0x00;
	///front air press
  if(((FrontAirPressWarnKeyBit==KeyOn)||(pres_value<AIR_PRESS_LOWER_NUM))&&(qy1cgerrflag==0)) 
  	FrontAirPressWarnFlag=1;	
	///back air press
  if(((BackAirPressWarnKeyBit==KeyOn)||(gaspress2<AIR_PRESS_LOWER_NUM))&&(qy2cgerrflag==0)) 
  	BackAirPressWarnFlag=1;
	///水温报警
 	if(WaterTemperatureFlag==1||EngineWaterWarnKeyBit==KeyOn)
  	WaterTemperatureWarnFlag=1; 
  ///机油压力报警 
  if((jycgerrflag==0)&&((FualPressWarnKeyBit==KeyOn)||(oilPres<FualLowNum)||(oilPres>FualHighNum)))
  	FualPressWarnFlag=1;
	///水位报警
	if(EngineLowerWaterKeyBit==KeyOn)
  	WaterLevelWarnFlag=1;	
	///batter volatage lower to 23 or higher to 30;电池电压报警
  if(lvi_error==1)
  	BatteryPressWarnFlag=1;
 ///前气压传感器故障
  if(qy1cgerrflag==1) 
 		FrontAirSensorWarnFlag=1;
 ///后气压传感器故障
 	if(qy2cgerrflag==1) 
 	 	BackAirSensorWarnFlag=1;
	///油量传感器故障
	if(ylcgerrflag==1)
 		OilSensorWarnFlag=1;
 ///机油压力传感器故障
 	if(jycgerrflag==1)	
		FualPressSensorWarnFlag=1;
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
static void SecondErrorDisplayDeal(void)
{
	unsigned char i,ErrorArray,ErrorNum,SE_DetectNum;///SE->SECOND ERROR
	unsigned char buf_tmp2=0,buf_tmp1=0;
	uchar	DispNumber=0;
	
  if(OnKeyBit==KeyOff)
    return; 

	///second error display
	ErrorArray=0;
	ErrorNum=0;
	DispNumber=QianQiYaBaoJing;
	for(i=0;i<SECOND_ERROR_NUM;i++)
	{
		if(TempSecondError[i].Data!=SecondError[i].Data)
		{
			for(SE_DetectNum=0;SE_DetectNum<8;SE_DetectNum++)
			{
				buf_tmp2=(TempSecondError[i].Data>>SE_DetectNum)&0x01;
				buf_tmp1=(SecondError[i].Data>>SE_DetectNum)&0x01;
				if(buf_tmp1==0x00)
				{
					Second_ErrorOld[ErrorArray]&=~(0x01<<ErrorNum);	
					if(buf_tmp2!=0x00)
					{
						pop_dis(DispNumber);       		 //clear status
						Second_Error[ErrorArray]&=~(0x01<<ErrorNum);	
					}
				}
				else
				{
					Second_ErrorOld[ErrorArray]|=0x01<<ErrorNum;
					if(buf_tmp2==0x00)
					{
						push_dis(DispNumber);
						Second_Error[ErrorArray]|=0x01<<ErrorNum;						
					}
				}
				DispNumber++;	
				ErrorNum++;		
			}
			TempSecondError[i].Data=SecondError[i].Data;
		}
		else
		{
			DispNumber+=8;					
			ErrorNum+=8;						
		}
		if(ErrorNum>=8)
		{
			ErrorNum=0;
			ErrorArray++;
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
static void SpecialSecondErrorDeal(void)
{
	unsigned char i,ErrorArray,ErrorNum,SE_DetectNum;///SE->SECOND ERROR
	unsigned char buf_tmp2=0,buf_tmp1=0;
	uchar	DispNumber=0;
	
  if(OnKeyBit==KeyOff)
    return; 

	for(i=0;i<sizeof(SpecialSecondError);i++)
		SpecialSecondError[i].Data=0x00;
	///special second error logic
	///左刹车片报警
//	if(LeftFrontStopHoofKeyBit==KeyOff||LeftBackStopHoofKeyBit==KeyOff)
//		LeftStopHoofWarnFlag=1;
	///右刹车片报警
//	if(RightFrontStopHoofKeyBit==KeyOff||RightBackStopHoofKeyBit==KeyOff)
//		RightStopHoofWarnFlag=1;
	///门泵低气压报警
//	if(DoorPumpLowAirKeyBit==KeyOn)
//		DoorBumpLowAirPressWarnFlag=1;
	///变速箱故障
	///if(GearBoxWarnKeyBit==KeyOn)
		///GearBoxWarnFlag=1;
	///卫生间低水位报警
//	if(WCIndicateKeyBit==KeyOn)
//		WCIndicateWarnFlag=1;
	///发动机仓温报警
	if(EngineRoomTemperatureKeyBit==KeyOn)
		EngineRoomTemperatureWarnFlag=1;
	///缓速器高温报警
//	if(DelaySpeedTemperatureKeyBit==KeyOn)
//		DelaySpeedTemperatureWarnFlag=1;
	///空滤器报警
//	if(AirFiltrateWarnKeyBit==KeyOn)
//		AirFiltrateWarnFlag=1;

	///右后刹车片报警
//	if(RightBackStopHoofKeyBit==KeyOff)///右后蹄片报警
//		RightBackStopHoofWarnFlag=1;
	
	///右前刹车片报警
//	if(RightFrontStopHoofKeyBit==KeyOff)///右前蹄片报警
//		RightFrontStopHoofWarnFlag=1;

	///左刹车片报警
//	if(LeftBackStopHoofKeyBit==KeyOff)///左后蹄片报警
//		LeftBackStopHoofWarnFlag=1;
	
	///左刹车片报警
//	if(LeftFrontStopHoofKeyBit==KeyOff)///左前蹄片报警
//		LeftFrontStopHoofWarnFlag=1;	

	///special second error excute
	ErrorArray=0;
	ErrorNum=0;
	DispNumber=ZuoSaChePian;
	for(i=0;i<sizeof(SpecialSecondError);i++)
	{
		if(TempSpecialSecondError[i].Data!=SpecialSecondError[i].Data)
		{
			for(SE_DetectNum=0;SE_DetectNum<8;SE_DetectNum++)
			{
				buf_tmp2=(TempSpecialSecondError[i].Data>>SE_DetectNum)&0x01;
				buf_tmp1=(SpecialSecondError[i].Data>>SE_DetectNum)&0x01;
				if(DispNumber==FaDongJiCangWen)	//仓温报警无文字
					continue;						
				if(buf_tmp1==0x00)
				{
					SpecialSecond_ErrorOld[ErrorArray]&=~(0x01<<ErrorNum);	
					if(buf_tmp2!=0x00)
					{
						pop_dis(DispNumber);       		 //clear status
						SpecialSecond_Error[ErrorArray]&=~(0x01<<ErrorNum);	
					}
				}
				else
				{
					SpecialSecond_ErrorOld[ErrorArray]|=0x01<<ErrorNum;
					if(buf_tmp2==0x00)
					{
						push_dis(DispNumber);
						SpecialSecond_Error[ErrorArray]|=0x01<<ErrorNum;						
					}
				}
				ErrorNum++;		
				DispNumber++;	
			}
			TempSpecialSecondError[i].Data=SpecialSecondError[i].Data;
		}
		else
		{
			DispNumber+=8;					
			ErrorNum+=8;						
		}
		if(ErrorNum>=8)
		{
			ErrorNum=0;
			ErrorArray++;
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
static void SpecialThirdErrorDeal(void)
{
	unsigned char i,ErrorArray,ErrorNum,TE_DetectNum;///TE->THIRD ERROR
	unsigned char buf_tmp2=0,buf_tmp1=0;
	
  if(OnKeyBit==KeyOff)
    return; 

	SpecialThirdError[0].Data=0x00;
	///special third error logic
	///车速>5km/h且前门未关报警
	if((carspeed>CAR_RUN_DOOR_OPEN_NUM)&&(FrontDoorOpenSignalKeyBit==KeyOn))
		FrontDoorOpenSignalFlag=1;
	///车速>5km/h且中门未关报警	
	if((carspeed>CAR_RUN_DOOR_OPEN_NUM)&&(MiddleDoorOpenSignalKeyBit==KeyOn))
		MiddleDoorOpenSignalFlag=1;
	///尿素液位过低报警
//	if(CatalystTankLevelFlag==1)
//		CatalystTankLevel_LOWER_Flag=1;
	///special third error excute
	ErrorArray=0;
	ErrorNum=0;
	for(i=0;i<sizeof(SpecialThirdError);i++)
	{
		if(TempSpecialThirdError[i].Data!=SpecialThirdError[i].Data)
		{
			for(TE_DetectNum=0;TE_DetectNum<8;TE_DetectNum++)
			{
				buf_tmp2=(TempSpecialThirdError[i].Data>>TE_DetectNum)&0x01;
				buf_tmp1=(SpecialThirdError[i].Data>>TE_DetectNum)&0x01;
				if(buf_tmp1==0x00)
				{
					SpecialThird_ErrorOld[ErrorArray]&=~(0x01<<ErrorNum);	
					if(buf_tmp2!=0x00)
					{
						SpecialThird_Error[ErrorArray]&=~(0x01<<ErrorNum);	
						//前门未关报警
						if(TE_DetectNum==2)
						{
								pop_dis(FrontDoorOpenAlarm);
						}//中门未关报警
						else if(TE_DetectNum==3)
						{
								pop_dis(MiddleDoorOpenAlarm);
						}
					}
				}
				else
				{
					SpecialThird_ErrorOld[ErrorArray]|=0x01<<ErrorNum;
					if(buf_tmp2==0x00)
					{
						SpecialThird_Error[ErrorArray]|=0x01<<ErrorNum;			
						//前门未关报警
						if(TE_DetectNum==2)
						{
								push_dis(FrontDoorOpenAlarm);
						}//中门未关报警
						else if(TE_DetectNum==3)
						{
								push_dis(MiddleDoorOpenAlarm);
						}
					}
				}
				ErrorNum++;		
			}
			TempSpecialThirdError[i].Data=SpecialThirdError[i].Data;
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
void WakeUpInit(void)
{	
  CAN0CTL0_SLPRQ=0;
	SleepFlag=0;  
	WakeUpInitFlag=0;
	CAN0_RS=0;
	CAN1_RS=0;
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
void SendCan0Message(unsigned char SendKind)
{
	unsigned	char Mi;	
	CAN_PACKET_T SendFrame;
	uint engSpeedTemp;
	uint	fual_value_Temp,HundredKilometerFuelConsumeTemp;

	if(bootloader_wait_flag||boot_reset_flag)
		return;
	SendFrame.identifier=METER_ID;
	
	switch(SendKind)
	{		
		case	SendMeterSwich:													
      SendFrame.byte_count=8;			
			SendFrame.data[0]=SendMeterSwich;
      for(Mi=0;Mi<7;Mi++)		
      {
				if(Mi<MeterKeyNum)
    	  	SendFrame.data[Mi+1] = SendKey[Mi].Data;	
				else
					SendFrame.data[Mi+1]=0x00;
      }
			break;	
		case	SendVirtualSwich:													
      SendFrame.byte_count=8;			
			SendFrame.data[0]=SendVirtualSwich;
      for(Mi=0;Mi<7;Mi++)		
      {
				if(Mi<2)
    	  	SendFrame.data[Mi+1] = VirtualMeterKey[Mi].Data;	
				else
					SendFrame.data[Mi+1]=0x00;
      }
			break;				
		case	Sendloadstatus:								
			SendFrame.byte_count =8;
			SendFrame.data[0] = Sendloadstatus;
    	SendFrame.data[1] = ReState[0].Data;		
			SendFrame.data[2] = ReState[1].Data;
			for(Mi=3;Mi<8;Mi++)	
				SendFrame.data[Mi] =0xff;
			break;	
		case	SendSensorstatus:	
			SendFrame.byte_count =8;			
			SendFrame.data[0] = SendSensorstatus;
			for(Mi=0;Mi<7;Mi++)	
				SendFrame.data[Mi+1]=MeterSensorData[Mi];
			break;	
		case	SendRainScratch:	
			SendFrame.byte_count =8;			
			SendFrame.data[0] = SendRainScratch;
			SendFrame.data[1] = 0xff;
			SendFrame.data[2] = rain_scr;
			SendFrame.data[3] = 0xff;
			SendFrame.data[4] = 0xff;
			SendFrame.data[5] = 0xff;
			SendFrame.data[6] = 0xff;
			SendFrame.data[7] = 0xff;
			break;			
		case	SendJ1939_oilwear:	
			
			fual_value_Temp=fual_value;
			HundredKilometerFuelConsumeTemp=HundredKilometerFuelConsume/5;
			
			SendFrame.byte_count =8;
			SendFrame.data[0] = SendJ1939_oilwear;
			SendFrame.data[1] = (uchar)fual_value_Temp;
			SendFrame.data[2] = (uchar)(fual_value_Temp>>8);			
			SendFrame.data[3] = (uchar)HundredKilometerFuelConsumeTemp;
			SendFrame.data[4] = (uchar)(HundredKilometerFuelConsumeTemp>>8);
			SendFrame.data[5] = 0xff;
			SendFrame.data[6] = 0xff;
			SendFrame.data[7] = 0xff;		
			break;	
		case	SendJ1939_oilwear1:		

			SendFrame.byte_count =8;
			SendFrame.data[0] = SendJ1939_oilwear1;
			
			SendFrame.data[1] = (uchar)total_fual_used;
			SendFrame.data[2] = (uchar)(total_fual_used>>8);
			SendFrame.data[3] = (uchar)(total_fual_used>>16);
			SendFrame.data[4] = (uchar)(total_fual_used>>24);
			SendFrame.data[5] = 0xff;
			SendFrame.data[6] = 0xff;
			SendFrame.data[7] = 0xff;				
			break;
   	case	SendDistance:														///send vehile distance 	
      SendFrame.byte_count =8;
			SendFrame.data[0] = SendDistance;
      SendFrame.data[1] = VehicleSmallDistanceNum[0];
			SendFrame.data[2] = VehicleSmallDistanceNum[1];
			SendFrame.data[3] = 0x00;
			SendFrame.data[4] = VehicleOverallDistanceNum[0];
			SendFrame.data[5] = VehicleOverallDistanceNum[1];
			SendFrame.data[6] = VehicleOverallDistanceNum[2];
			SendFrame.data[7] = 0x00;
			break;
		case	SendCarSpeed:	
			SendFrame.identifier=0x10fe6cee;
			SendFrame.byte_count =8;
			SendFrame.data[0] = SendCarSpeed;
			SendFrame.data[1] = 0xff;
			SendFrame.data[2] = 0xff;
			SendFrame.data[3] = 0xff;
			SendFrame.data[4] = 0xff;
			SendFrame.data[5] = 0xff;
			SendFrame.data[6] = 0x00;
			SendFrame.data[7] = carspeed;			
			break;			
		case	SendRotation:	
			SendFrame.identifier=0x10f004ee;
			SendFrame.byte_count =8;
			SendFrame.data[0] = SendRotation;
			SendFrame.data[1] = 0xff;
			SendFrame.data[2] = 0xff;
			engSpeedTemp =engSpeed;
			engSpeedTemp <<=3;
			SendFrame.data[3] = (uchar)engSpeedTemp;
			SendFrame.data[4] = (uchar)(engSpeedTemp>>8);
			SendFrame.data[5] = 0xff;
			SendFrame.data[6] = 0xff;
			SendFrame.data[7] = 0xff;			
			break;
		case	SendSleepFlash:            	
      SendFrame.byte_count =8;		
			SendFrame.data[0] = SendSleepFlash;
			SendFrame.data[1] = 0xff;			
    	SendFrame.data[2] = SleepCodeBuf;  	
			SendFrame.data[3] = FlashCodeBuf;  
			SendFrame.data[4] = 0xff;
			SendFrame.data[5] = 0xff;
			SendFrame.data[6] = 0xff;
			SendFrame.data[7] = 0xff;				
			break;
		case sendrequireID:    	
      SendFrame.byte_count =8;		
			SendFrame.data[0] = sendrequireID;
			SendFrame.data[1] = 0xff;	
			SendFrame.data[2] = 0xff;
			SendFrame.data[3] = 0xff;	
			SendFrame.data[4] = 0xff;
			SendFrame.data[5] = 0xff;
			SendFrame.data[6] = 0xff;
			SendFrame.data[7] = 0xff;	
  		break;			
		case	SendSelfID:   	
      SendFrame.byte_count =8;		
			SendFrame.data[0] = SendSelfID;
		
			SendFrame.data[1]='B';
			SendFrame.data[2]='D';
			SendFrame.data[3]='S';
			SendFrame.data[4]='L';
			SendFrame.data[5]='J';
			SendFrame.data[6]='R';
			SendFrame.data[7]='0';
			break;
		case	SendSelfID1: 		
			SendFrame.byte_count =8;		
			SendFrame.data[0] = SendSelfID1;
			SendFrame.data[1]='3';
			SendFrame.data[2]='-';
			SendFrame.data[3]='-';
			SendFrame.data[4]='-';
			SendFrame.data[5]='-';
			SendFrame.data[6]='-';
			SendFrame.data[7]='-';
			break;

		case SendConfigData:
			SendFrame.byte_count =8;
			SendFrame.data[0] = SendConfigData;
			SendFrame.data[1] = ConfigDataResult;
			if(ConfigDataResult== 0xfe)
			{
				SendFrame.data[2] = VehicleOverallDistanceNum[0];
				SendFrame.data[3] = VehicleOverallDistanceNum[1];
				SendFrame.data[4] = VehicleOverallDistanceNum[2];
				SendFrame.data[5] = 0x00;
				SendFrame.data[6] = Speed_Pusle_Num;
				
			}
			else if(ConfigDataResult== 0x01)
			{
				SendFrame.data[2] = (uchar)Speed_Rate_Num;
				SendFrame.data[3] = (uchar)(Speed_Rate_Num>>8);
				SendFrame.data[4] = 0x00;
				SendFrame.data[5] = 0x00;
				SendFrame.data[6] = Speed_Pusle_Num;				
			}
			SendFrame.data[7] = 0xff;
			break;
/*		case SendMeterLed:            	
     	SendFrame.byte_count =6;
		
			SendFrame.data[0]=SendMeterLedData[0].LedLoad;
			SendFrame.data[1]=SendMeterLedData[1].LedLoad;
			SendFrame.data[2]=SendMeterLedData[2].LedLoad;
			SendFrame.data[3]=SendMeterLedData[3].LedLoad;		
			SendFrame.data[4]=SendMeterLedData[4].LedLoad;
			SendFrame.data[5]=SendMeterLedData[5].LedLoad;
  		break;*/		
		case SendBootloaderAck:
			SendFrame.byte_count = 1;
			SendFrame.data[0] =ResponseData;	
			break;
		default:
			break;		
	}	
	CAN0_SendFrame(SendFrame);
}  
//////////////////////////////////////////////////////////////////////////////
static void CAN0_SendFrame(CAN_PACKET_T SendFrame)
{
	uchar i,cansel;

	CAN0TBSEL = CAN0TFLG; 		///select one available transmit buffer
	cansel = CAN0TBSEL;
	
	///PGN change to 32bit	 
	CAN0TXIDR0 = (SendFrame.identifier>>21) & 0xFF; 	
	CAN0TXIDR1 = (SendFrame.identifier>>18) & 0x07; 
	CAN0TXIDR1 = CAN0TXIDR1 << 5;
	CAN0TXIDR1 = CAN0TXIDR1 | ((SendFrame.identifier>>15) & 0x07);
	CAN0TXIDR1_SRR = 1;
	CAN0TXIDR1_IDE = 1; 				///extend frame IDE=1->29bit,IDE=0->11bit		
	CAN0TXIDR2 = (SendFrame.identifier>>7) & 0xFF; 
	CAN0TXIDR3 = (SendFrame.identifier<<1) & 0xFF; 
	CAN0TXIDR3_RTR = 0; 				///data frame	

	CAN0TXDLR=SendFrame.byte_count;	///length of data
	for(i=0;i<SendFrame.byte_count;i++)
		*(&CAN0TXDSR0+i)=SendFrame.data[i];
	
	CAN0TXTBPR=1;         			///configure data fifo of send priory
  CAN0TFLG=cansel;	
}
#pragma CODE_SEG DEFAULT

