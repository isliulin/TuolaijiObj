#define	_NO_COMMON_EXTERN
#include	"common.h"
/***************************************
****************************************/
void	External_WatchDog(void)
{		
		External_Wdg_Io=1;
		
		if(External_Wdg==0)
			External_Wdg=1;
		else
			External_Wdg=0;
}
/*****************************************************
* brief:	us delay	
* author:	yao qing rui
* input:	word delay:the delay time want to put
* output:	void
******************************************************/
void	delay_us(word delay)
{
	word	i;
	for(i=0;i<delay;i++)
	{
		asm nop;
		asm nop;
	}	
}
/*===================================================
* brief:    RTC overflow interrupt service routine
* author:   yao qing rui
=====================================================*/ 
interrupt void  rtc_isr(void)
{       		 	
	RTCSC|=0x80;  							///clear rtc recieve flag

	++delay3s_ModuleCheck;		///meter module check delay
	
	++KeyAdcTime;
	if(KeyAdcTime>=NUM_KEYADC)
		KeyAdcFlag=1;
	
  ++Time_SendSt;						///load state send time
  if(Time_SendSt>=NUM_300MS)
  	LoadSTChangFlag=1;   	
	
  ++Time_SendSensor;				///sensor send time
  if(Time_SendSensor>=NUM_300MS)
  	DriveSendSensorFlag=1;	
  	
  ++StDetectOpen_1s;				///st detect open load delay
  if(StDetectOpen_1s>=Num_StDetect)
  {
  	StDetectOpen_1s=0;
  	StDetectOpenFlag=1;
  } 
  	
  ++Time_SendKey;					///key send time
  if(Time_SendKey>=NUM_300MS)	  		 
  	KeyChangFlag=1;

	++Time_Change_Temperature;	///Temperature extact delay
	if(Time_Change_Temperature>=Num_Temperature_Change)
	{
		Temperature_ChangeFlag=1;
	}
	++Time_Send_Speed;				///vehicle speed send	time	
	if(Time_Send_Speed>=Num_Speed_Send)
		SendSpeedFlag=1;
	++Time_Send_Rotation;			///转速发送的时间
	if(Time_Send_Rotation>=Num_Rotation_Send)
		SendRotationFlag=1;	
	
	++Time_Capture_Zero;	///车速采集的基准时间		
	if(Time_Capture_Zero>=NUM_CaptureNoDetect)  
	{	
		Time_Capture_Zero=0;	
		SendSpeedData[0]=0;		   ///if no capture,the speed is 0 
		SendSpeedDataLast=0;
		SendSpeedDataUpdata1Num=0;
		SendSpeedDataUpdataNum=0;			
		Capture_Start_Flag=0;				
	} 
	++Time_Rotation_Zero;
	if(Time_Rotation_Zero>=NUM_CaptureNoDetect)  
	{	
		Time_Rotation_Zero=0;	
		SendRotationData[0]=0;		///if no capture,rotation is 0 
		Capture_Rotation_Flag=0;				
	} 
	++Time_Change_Oil;				///oil number extact delay
	if((!OnKeyBit)||(MeterErrorState&&(!BakOnKeyBit)))
	{
		if(Time_Change_Oil>=Num_OilSensor_KeyOffChange)  
			OilChangeFlag=1;
	}
	else
	{
		if(Time_Change_Oil>=Num_OilSensor_KeyOnChange)  
			OilChangeFlag=1;
	}
	++Time_Change_GasPress1;	///gas press1 number extact delay
	if(Time_Change_GasPress1>=Num_GasPress_Change)
	{
		GasPress1_ChangeFlag=1;
	}
	++Time_Change_GasPress2;	///gas press2 number extact delay	
	if(Time_Change_GasPress2>=Num_GasPress_Change)
	{
		GasPress2_ChangeFlag=1;
	}
	++Time_EngineRoomTemp;///发动机仓温传感器提取时间
	if(Time_EngineRoomTemp>=Num_EngineRoomTemp_Change)
		EngineRoomTempFlag=1;
	++Time_EngineWaterTemp;///发动机水温传感器提取时间
	if(Time_EngineWaterTemp>=Num_EngineRoomTemp_Change)
		EngineWaterTempFlag=1;
	++Time_Change_FuelLevel;
	if(Time_Change_FuelLevel>=Num_FuelLevel_Change)
	{
		FuelLevel_2sFlag=1;
	}
	
	if(FrontDriveFlag==1)
	{
		delayxms++;
    	if(delayxms>=dirdelay)		////flash delay 
   			delayxms_flag=1;
		#if	RainScratch_Out_From==1
		if(AccKeyBit||(MeterErrorState&&BakOnKeyBit))
		{
			if(CountWashStopTimeStartFlag==1)
			{
				if(WashStopFlag==1)
				{
					WashTime++;
					if(WashTime>=Num_WashTime)
						WashStopFlag=0;
				}
			}
			
			if(RainScratchIntermissionKeyBit)
	    	{
	   			RainScratchIntermissionTime++;	
				///because it is time with second,so must be enlarged 100*10ms
	   			if(RainScratchIntermissionTime>=(ReRainScratchIntermission+NUMINTERMISSIONADITIONAL)*Num_Intermission)
	   			{
	   			RainScratchIntermissionTime=0;
	   			IntermissionStartFlag=0;
	   			}
				if(IntermissionStopFlag==0)
				{
					IntermissionTime++;
					if(IntermissionTime>=Num_IntermissionTime)			///intermission time is 25*10ms
					{
						IntermissionStopFlag=1;
						RainScratchIntermissionTime=0;
					}
				}
			}		
		}
		#endif
	}
	else if(TopDriveFlag==1)
	{
		++SchoolBusBerthTime;			///黄色校车灯闪烁时间

		///++EngineControlPowerTime;	///空调控制电源延迟时间
	}
	else if(BackDriveFlag==1)
	{				
		++PowerOff_Delay;					///time of power off and acc delay 
			
  		++RestartDetectTime;				///time of st restart detect 
  		
  		++MotorStartDetect; 				///time of st start too long

		///++EngineProtectStartTime;		///启动保护延迟时间
	}
}


