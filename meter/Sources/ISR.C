#define 	ISR_C
#include	"config.h"
/*************************************
funtion operation:rtc interrupt,5.12ms enter 1 time
 **************************************/
#pragma CODE_SEG  __NEAR_SEG	NON_BANKED
interrupt void rti_int(void) ///vector number is 7
{
  CRGFLG_RTIF = 1;

	GetTPMSTime++;///TPMS high CAN error
	TPMS_Learn_Time++;
	CPL_Time++;
	if((CPL_Time&0X7F)==0)
	{
		TPMS_TX1939_Time_Flag=1;
	  Display_CPL500MS_Flag^=1;			
	}
	
	delay1s_1++;
	if(delay1s_1==50)
	{
		if(SendConfigDataWaitFlag==1)
			SendConfigDataFlag=1;
	}
	else if(delay1s_1>200)
	{
		ID_require_flag=1;
		delay1s_1 =0;
	}
	
  MeterLedSend250ms++; ///meter led send delay
  if (MeterLedSend250ms >= CanSendMeterLedTimeNum)
    SendMeterLedFlag = 1;

  MeterKeySendTime++; ///meter send delay
  if (MeterKeySendTime >= Delay200MsNum)
    SendMeterKeyFlag = 1;

	VirsualMeterKeySendTime++;
  if (VirsualMeterKeySendTime >= Delay200MsNum)
    SendVirsualMeterKeyFlag = 1;

  MeterSendLoadStTime++; ///meter load send delay
  if (MeterSendLoadStTime >= Delay200MsNum)
    SendMeterLoadFlag = 1;

  MeterSendFuelConsumeTime++;////Fuel consume send delay	   					
  if (MeterSendFuelConsumeTime >= NUM_2S)
  {
    MeterSendFuelConsumeFlag = 1;
  }
	
  MeterSendFuelConsumeTime1++;////Fuel consume send delay	   					
  if (MeterSendFuelConsumeTime1 >= NUM_30S)
    MeterSendFuelConsumeFlag1 = 1;

  MeterSendPanelTemperatureSensorTime++;////PanelTemperature Sensor send delay	   					
  if (MeterSendPanelTemperatureSensorTime >= Delay200MsNum)
    SendMeterPanelTemperatureSensorFlag= 1;
	
	MeterSendDistanceTime++;
	if(MeterSendDistanceTime >= NUM_1S)
		SendDistanceFlag =1;
  VehicleTime++; ///vehicle distance count delay
  if (VehicleTime >= VehicleSmallDistance1Minute)
  {
    VehicleTime = 0;
    Vehicle1MinutueFlag = 1;
  }

	if((resetback0==1)||(resetback0==3))		///back0 motor delay 
	{
		delay1s++;	
		if(resetback0==1)
		{
			if(delay1s>MotorBackDelayNum)  ////5.12ms*800
		  	resetback0 = 0;
		}
		else
		{
			if(delay1s>(MotorBackDelayNum/2))  ////5.12ms*800
		  	resetback0 = 0;
		}
	}

  Delay_SystemVoltage++; ///voltage detect delay

  delay60s++; ///sleep if detect delay over
  SCRModuleDetectTime++;

	delay15s++;///start too long delay

	Fual_Require_Time++;///require total fual time delay

	WaterTempFlahTime++;///high can error then water temperature flash delay
	if(RainScratchWaitAckFlag==1)
	{
		rains_count++;
		if(rains_count>=NUM_1S)
		{
			rains_count=0;
			RainScratchChangeFlag=1;///if wait 1s and not ack then resend the data;
		}
	}
  RecordSwitchSend100ms++; 
  if (RecordSwitchSend100ms >= CanSendBENANRecordSwitchNum)
    SendRecordSwitchFlag = 1;
	///新加入的时间校准
	PushKeyUpDelayTime++;///按键弹起时间延时
	PushKeyDownDelayTime++;///按键按下时间延时
	///J1939相关的底盘信息，模块检测时间
  ECU_ModuleCheck_Time++;	///发动机ECU
  SCRModuleDetectTime++;		///后处理SCR
  Delay_BMS_ModuleCheck++;///电池bms
	Delay_DMC_ModuleCheck++;///电机驱动dmc
	///车身模块通讯是否正常检测时间
	FrontModuleCheck_Time++;
	TopModuleCheck_Time++;
	BackModuleCheck_Time++;
	PowerModuleCheck_Time++;
	PowerContrlModuleCheck_Time++;
	///在仪表采集的传感器检测时间
	#if		OIL_PRESS_SENSOR_IN==3
		++Time_Change_OilPress;	///oil press extact delay
		if(Time_Change_OilPress>=Num_OilPress_Change)
			OilPressFlag=1;
	#endif
	#if		WATER_TEMPERATURE_SENSOR_IN==3
		++Time_Change_WaterTemp;	///Temperature extact delay
		if(Time_Change_WaterTemp>=Num_Temperature_Change)
			WaterTemperature_2sFlag=1;
	#endif
	#if		FUAL_LEVEL_SENSOR_IN==3
		++Time_Change_FuelLevel;				///fuel level number extact delay
		if((AccKeyBit==KeyOff)&&(OnKeyBit==KeyOff))
		{
			if(Time_Change_FuelLevel>=Num_FuelLevelSensor_KeyOffChange)  
				FuelLevelFlag=1;
		}
		else
		{
			if(Time_Change_FuelLevel>=Num_FuelLevelOilSensor_KeyOnChange)  
				FuelLevelFlag=1;
		}
	#endif
	#if		FRONT_AIR_SENSOR_IN==3
		++Time_Change_GasPress1;	///gas press1 number extact delay
		if(Time_Change_GasPress1>=Num_GasPress_Change)
			GasPress1_ChangeFlag=1;
	#endif
	#if		BACK_AIR_SENSOR_IN==3
		++Time_Change_GasPress2;	///gas press2 number extact delay	
		if(Time_Change_GasPress2>=Num_GasPress_Change)
			GasPress2_ChangeFlag=1;
	#endif
	///车速信号归零
	#if		SPEED_SENSOR_IN==3
		++Time_Capture_Speed_Zero;			///time of speed capture not enter detect 
	  if(Time_Capture_Speed_Zero>=NUM_CaptureNoDetect)  
	  {	
	  	Time_Capture_Speed_Zero=0;	
			SendSpeedData[0]=0;						///when no speed capture,speed is 0 
			carspeed=0;
			CARSPEED();
			Capture6CountNum=0;
			Speed_Data_Flag = 1;
	  } 
	#endif
	///acc档关闭后的延时时间
	if(AccKeyBit==KeyOff)
	{
		ACCOff_Delay++;
		if(ACCOff_Delay>=Num_ACCOff)
			FirstACCOffFlag=0;
	}
	///启动电源总开关延时时间
	StartDelay_Time++;
	if(StartDelay_Time>=Num_StartOn)
		StartDelayFlag=0;
	//胎压设置超时
	/*
	if(TyrePressMenuStatus==1)
	{
		TyrePressMenuTimeOut++;
		if(TyrePressMenuTimeOut>200)
		{
			TyrePressMenuStatus=2;
			TyrePressSetSelChange_Flag=1;
		}
	}
	else
		TyrePressMenuTimeOut=0;
	*/
	if(((TyreState[TPMSPosition]==WAIT_SUCCEED)||(TyreState[TPMSPosition]==WAIT_REPLY))&&(TPMS_Learn_Time>TPMS_TIMEOUT)&&(TyrePressMenuStatus==1))
	{
		TyreState[TPMSPosition]=TPMS_TimeOut;//学习超时
		TyrePressMenuStatus=2;
		TyrePressSetSelChange_Flag=1;		
		TyrePressMenuLocate=TPMSPosition;
	}
	else if((TyreState[TPMSPosition]==TPMS_SUCCEED)&&(TyrePressMenuStatus==1))
	{
		TyrePressMenuStatus=3;
		TyrePressSetSelChange_Flag=1;	
	}
	
	///小计清零闪烁时间
	SmallDistanceClearFlash1s++;
	if(SmallDistanceClearFlash1s>100)
	{
		SmallDistanceClearFlash1s=0;
		SmallDistanceClearFlash1s_Flag=1;
		SmallDistanceClearFlash_Flag^=1;
	}
	///主界面故障轮显延时
	MainMenuDis_countDelay++;				
	if(MainMenuDis_countDelay>400)
	{
		MainMenuDis_countDelay=0;
		MainMenuDis_count++;
		if(MainMenuDis_count>=dis_count)
			MainMenuDis_count=0;
	}
	if((OnKeyBit==KeyOn)&&(menu_id==LogoMenuNum))
		MainMenuLogoTime++;
	
	if(OnKeyBit==KeyOn)
	{
		++J1939CarSpeedSendTime;
		if(J1939CarSpeedSendTime==(NUM_CARSPEED_SEND_TIME/2))
			SendCan0Message(SendCarSpeed);
		else 
		if(J1939CarSpeedSendTime>=NUM_CARSPEED_SEND_TIME)
		{
			J1939_Speed_Send();
			J1939CarSpeedSendTime=0;
		}/*
		++J1939CCVSSendTime;
		if(J1939CCVSSendTime>20)
		{
			J1939_CCVS_Send();
			J1939CCVSSendTime=0;
		}*/
		SendJ1939RotationTime++;///water temperature and rotation send delay 
		if (SendJ1939RotationTime >= 10)
		{
			SendJ1939RotationTime=0;
			SendCan0Message(SendRotation);
		}
	}	
	if(MiddleDoorOpenSignalDelayFlag==1&&MiddleDoorOpenSignalKeyBit==KeyOff)
	{
		MiddleDoorOpenSignalDelayNum++;
		if(MiddleDoorOpenSignalDelayNum>1000)
			MiddleDoorOpenSignalDelayFlag=0;
	}
	else
		MiddleDoorOpenSignalDelayNum=0;	
}
/******************************************************************************
funtion operation:if CAN1 bus communite error,the funtion will respond
 *******************************************************************************/
interrupt void CAN0error_int(void) ///vector number is 37
{
  byte Status1 = CAN0RFLG; /* Read the status register */

  CAN0RFLG |= 0x02;
  if (((Status1 &CAN_STATUS_TX_MASK) > CAN_STATUS_TX1_MASK) 
			|| ((Status1&CAN_STATUS_RX_MASK) > CAN_STATUS_RX1_MASK))	/* 127 < error counter */
    CAN0RFLG |= 254;/* Reset error flags */
}
/******************************************************************************
funtion operation:CAN1 RECEIVE INTERRUPT
*********************************************************************************/
interrupt void CAN0rxd_int(void) ///vector number is 38
{
	uchar i,function;
	uchar Temp;
	unsigned char	ReCarSpeedTemp=0;
	unsigned int	ReRotationTemp=0;
	ulong RecID;
	
  Inside_Reveive_Can_Packet.identifier = CAN0RXIDR0;
  Inside_Reveive_Can_Packet.identifier <<= 3;
  Inside_Reveive_Can_Packet.identifier |= (CAN0RXIDR1>>5);
  Inside_Reveive_Can_Packet.identifier <<= 3;
  Inside_Reveive_Can_Packet.identifier |= (CAN0RXIDR1&0X07);
  Inside_Reveive_Can_Packet.identifier <<= 8;
  Inside_Reveive_Can_Packet.identifier |= CAN0RXIDR2;
  Inside_Reveive_Can_Packet.identifier <<= 7;
  Inside_Reveive_Can_Packet.identifier |= (CAN0RXIDR3>>1);

  CAN0RXIDR0=0;
  CAN0RXIDR1=0;  
  CAN0RXIDR2=0;
  CAN0RXIDR3=0;   

	Inside_Reveive_Can_Packet.byte_count= CAN0RDLR & 0x0F;		///Result length of the message
	
	for (i=0; i<Inside_Reveive_Can_Packet.byte_count; i++) 
		Inside_Reveive_Can_Packet.data[i] = *(&CAN0RDSR0 + i);	///receive data
	
	function=Inside_Reveive_Can_Packet.data[0];
	RecID =Inside_Reveive_Can_Packet.identifier;

	if(function==0x00)							///待机准备
	{
		if(((RecID&0X1FFFF0FF)==0X10FF0021)||(RecID==METER_ID))
		{		
			TIE = 0X00;								///中断关闭，步进电机也不再动作
			bootloader_wait_flag = 1;
		}
	}
	else if(function==0x01)					///烧录准备
	{		
		if(((RecID&0X1FFFF0FF)==0X10FF0021)||(RecID==METER_ID)) 
		{		
			if(Inside_Reveive_Can_Packet.identifier==METER_ID)
			{	
				bootloader_wait_flag = 0;
				ResponseData =function;
				SendCan0Message(SendBootloaderAck);			
				boot_reset_flag = 1;				///软复位
				for (;;);
			}
			else
				bootloader_wait_flag = 1;
		}
	}
	else if((function==0x08)||(function==0x09))				///烧录完成复位
	{
		if(((RecID&0X1FFFF0FF)==0X10FF0021)||(RecID==METER_ID))
		{		
			for (;;);
		}
	}
	else if(Inside_Reveive_Can_Packet.identifier==METER_ID)
	{
		switch(function)
		{
			case sendrequireID:
				boot_identify_flag=1;
				SendSelfIdNum =0;
				break;
			case SendConfigData:		
				Temp =Inside_Reveive_Can_Packet.data[1];
				if(Temp==0x00)				//里程值配置
				{
					ClearEepromFlag=1;
					EepromClearFlag=1;
					EepromConfigData[0]=Inside_Reveive_Can_Packet.data[2];						
					EepromConfigData[1]=Inside_Reveive_Can_Packet.data[3];						
					EepromConfigData[2]=Inside_Reveive_Can_Packet.data[4];
				}
				else if(Temp==0x01) //里程参数配置
				{
					bootloader_wait_flag = 0;
					boot_reset_flag = 1;				///软复位
					for (;;);
				}
				break;
			default:
				break;
		}
	}
	else if(Inside_Reveive_Can_Packet.identifier==FrontDriver_ID)
	{
		switch(function)
		{
			case SendRainScratch:				
				Temp=Inside_Reveive_Can_Packet.data[2];
				if((Temp==0xfe)&&(FrontDriver_ID==RainScratchLocation))
					RainScratchWaitAckFlag=0;
				break;	
			case SendConfigData:	
				Temp=Inside_Reveive_Can_Packet.data[1];
				if((CarSpeedCaptureLocation==FrontDriver_ID)&&(Temp==0xfe))
				{
					SendConfigDataFlag=0;
					SendConfigDataWaitFlag=0;
				}
				break;	
			case SendCarBodySwich:
					SleepFlag = 0;
					FrontDriveModuleFlag=1;
					FrontDriveKey[0].Data=Inside_Reveive_Can_Packet.data[1];
					FrontDriveKey[1].Data=Inside_Reveive_Can_Packet.data[2];
					FrontDriveKey[2].Data=Inside_Reveive_Can_Packet.data[3];

				break;
			case Sendloadstatus:	
				for(i=0;i<DRIVELOADNUM;i++)
					FrontDriveLoad[i].Data=Inside_Reveive_Can_Packet.data[i+1];
				break;		
			case SendSensorstatus:		
				for(i=0;i<7;i++)
						FrontSensorData[i]=Inside_Reveive_Can_Packet.data[i+1];
				break;	
			case SendCurrentstatus: 						
				break;					
			case SendCurrentstatus1:				
				break;			
			case SendCurrentstatus2:					
				break;	
			case SendSelfID:	
				for(i=0;i<7;i++)
					QianQuBanBen[i]=Inside_Reveive_Can_Packet.data[i+1];
				break;		
			case SendSelfID1:	
				QianQuBanBen[7]=Inside_Reveive_Can_Packet.data[1];
				break;			
			default:
				break;
		}		
	}
	else if(Inside_Reveive_Can_Packet.identifier==BackDriver_ID)
	{
		switch(function)
		{
			case SendRainScratch:				
				Temp=Inside_Reveive_Can_Packet.data[2];
				if((Temp==0xfe)&&(BackDriver_ID==RainScratchLocation))
					RainScratchWaitAckFlag=0;
				break;	
			case SendConfigData:	
				Temp=Inside_Reveive_Can_Packet.data[1];
				if((CarSpeedCaptureLocation==BackDriver_ID)&&(Temp==0xfe))
				{
					SendConfigDataFlag=0;
					SendConfigDataWaitFlag=0;
				}
				break;	
			case SendCarBodySwich:
				SleepFlag = 0;
				BackDriveModuleFlag=1;
				BackDriveKey[0].Data=Inside_Reveive_Can_Packet.data[1];
				BackDriveKey[1].Data=Inside_Reveive_Can_Packet.data[2];
				BackDriveKey[2].Data=Inside_Reveive_Can_Packet.data[3];
				break;
			case Sendloadstatus:	
				for(i=0;i<DRIVELOADNUM;i++)
					BackDriveLoad[i].Data=Inside_Reveive_Can_Packet.data[i+1];
				break;		
			case SendSensorstatus:		
				for(i=0;i<7;i++)
					BackSensorData[i]=Inside_Reveive_Can_Packet.data[i+1];
				break;
			case SendSelfID:	
				for(i=0;i<7;i++)
					HouQuBanBen[i]=Inside_Reveive_Can_Packet.data[i+1];
				break;		
			case SendSelfID1:	
					HouQuBanBen[7]=Inside_Reveive_Can_Packet.data[1];
				break;			
			default:
				break;
		}		
	}
	else if(Inside_Reveive_Can_Packet.identifier==TopDriver_ID)
	{
		switch(function)
		{
			case SendRainScratch:				
				Temp=Inside_Reveive_Can_Packet.data[2];
				if((Temp==0xfe)&&(TopDriver_ID==RainScratchLocation))
					RainScratchWaitAckFlag=0;
				break;	
			case SendConfigData:	
				Temp=Inside_Reveive_Can_Packet.data[1];
				if((CarSpeedCaptureLocation==TopDriver_ID)&&(Temp==0xfe))
				{
					SendConfigDataFlag=0;
					SendConfigDataWaitFlag=0;
				}
				break;	
			case SendCarBodySwich:
				SleepFlag = 0;
				TopDriveModuleFlag=1;
				TopDriveKey[0].Data=Inside_Reveive_Can_Packet.data[1];
				TopDriveKey[1].Data=Inside_Reveive_Can_Packet.data[2];
				TopDriveKey[2].Data=Inside_Reveive_Can_Packet.data[3];
				break;
			case Sendloadstatus:	
				for(i=0;i<DRIVELOADNUM;i++)
					TopDriveLoad[i].Data=Inside_Reveive_Can_Packet.data[i+1];
				break;		
			case SendSensorstatus:		
				for(i=0;i<7;i++)
					TopSensorData[i]=Inside_Reveive_Can_Packet.data[i+1];
				break;	
			case SendSelfID:	
				for(i=0;i<7;i++)
					DingQuBanBen[i]=Inside_Reveive_Can_Packet.data[i+1];
				break;		
			case SendSelfID1:	
				DingQuBanBen[7]=Inside_Reveive_Can_Packet.data[1];
				break;			
			default:
				break;
		}		
	}
	else if(Inside_Reveive_Can_Packet.identifier==Power_ID)
	{
		switch(function)
		{
			case SendCarBodySwich:
				SleepFlag = 0;
				PowerModuleFlag=1;
				PowerKey[0].Data=Inside_Reveive_Can_Packet.data[1];
				break;
			case SendRainScratch:				
				Temp=Inside_Reveive_Can_Packet.data[2];
				if((Temp==0xfe)&&(Power_ID==RainScratchLocation))
					RainScratchWaitAckFlag=0;
				break;	
			case SendConfigData:	
				Temp=Inside_Reveive_Can_Packet.data[1];
				if((CarSpeedCaptureLocation==Power_ID)&&(Temp==0xfe))
				{
					SendConfigDataFlag=0;
					SendConfigDataWaitFlag=0;
				}
				break;	
			case Sendloadstatus:	
				PowerModuleFlag=1;
				SleepFlag = 0;
				for(i=0;i<POWER_LOAD_NUM;i++)
					PowerLoad[i].Data=Inside_Reveive_Can_Packet.data[i+1];
				break;		
			case Sendloadstatus1:		
				for(i=0;i<POWER_FUSE_NUM;i++)
					PowerFuse[i].Data=Inside_Reveive_Can_Packet.data[i+1];
				break;
			case SendSensorstatus:		
				for(i=0;i<7;i++)
					PowerSensorData[i]=Inside_Reveive_Can_Packet.data[i+1];
				break;	
			case SendSelfID:	
				for(i=0;i<7;i++)
					PeiDianBanBen[i]=Inside_Reveive_Can_Packet.data[i+1];
				break;		
			case SendSelfID1:	
					PeiDianBanBen[7]=Inside_Reveive_Can_Packet.data[1];
				break;			
			default:
				break;
		}		
	}
	else if(Inside_Reveive_Can_Packet.identifier==PowerContrl_ID)
	{
		switch(function)
		{
			case SendRainScratch:				
				Temp=Inside_Reveive_Can_Packet.data[2];
				if((Temp==0xfe)&&(PowerContrl_ID==RainScratchLocation))
					RainScratchWaitAckFlag=0;
				break;	
			case SendConfigData:	
				Temp=Inside_Reveive_Can_Packet.data[1];
				if((CarSpeedCaptureLocation==PowerContrl_ID)&&(Temp==0xfe))
				{
					SendConfigDataFlag=0;
					SendConfigDataWaitFlag=0;
				}
				break;	
			case SendCarBodySwich:
				SleepFlag = 0;
				PowerContrlModuleFlag=1;
				PowerContrlKey[0].Data=Inside_Reveive_Can_Packet.data[1];
				break;
			case Sendloadstatus:	
				for(i=0;i<POWERCONTRL_LOAD_NUM;i++)
					PowerContrlLoad[i].Data=Inside_Reveive_Can_Packet.data[i+1];
				break;		
			case SendSensorstatus:		
				for(i=0;i<7;i++)
					PowerContrlSensorData[i]=Inside_Reveive_Can_Packet.data[i+1];
				break;	
			case SendCurrentstatus: 
				PowerContrlCurrent[0] =Inside_Reveive_Can_Packet.data[2];
				PowerContrlCurrent[0] =PowerContrlCurrent[0]<<8;
				PowerContrlCurrent[0] +=Inside_Reveive_Can_Packet.data[1];
		
				PowerContrlCurrent[1] =Inside_Reveive_Can_Packet.data[4];
				PowerContrlCurrent[1] =PowerContrlCurrent[1]<<8;
				PowerContrlCurrent[1] +=Inside_Reveive_Can_Packet.data[3];
		
				PowerContrlCurrent[2] =Inside_Reveive_Can_Packet.data[6];
				PowerContrlCurrent[2] =PowerContrlCurrent[2]<<8;
				PowerContrlCurrent[2] +=Inside_Reveive_Can_Packet.data[5];						
				break;					
			case SendCurrentstatus1:	
				PowerContrlCurrent[3] =Inside_Reveive_Can_Packet.data[2];
				PowerContrlCurrent[3] =PowerContrlCurrent[3]<<8;
				PowerContrlCurrent[3] +=Inside_Reveive_Can_Packet.data[1];
		
				PowerContrlCurrent[4] =Inside_Reveive_Can_Packet.data[4];
				PowerContrlCurrent[4] =PowerContrlCurrent[4]<<8;
				PowerContrlCurrent[4] +=Inside_Reveive_Can_Packet.data[3];
		
				PowerContrlCurrent[5] =Inside_Reveive_Can_Packet.data[6];
				PowerContrlCurrent[5] =PowerContrlCurrent[5]<<8;
				PowerContrlCurrent[5] +=Inside_Reveive_Can_Packet.data[5];						
				break;			
			case SendCurrentstatus2:	
				PowerContrlCurrent[6] =Inside_Reveive_Can_Packet.data[2];
				PowerContrlCurrent[6] =PowerContrlCurrent[6]<<8;
				PowerContrlCurrent[6] +=Inside_Reveive_Can_Packet.data[1];
		
				PowerContrlCurrent[7] =Inside_Reveive_Can_Packet.data[4];
				PowerContrlCurrent[7] =PowerContrlCurrent[7]<<8;
				PowerContrlCurrent[7] +=Inside_Reveive_Can_Packet.data[3];					
				break;	
			case SendSelfID:	
				for(i=0;i<7;i++)
					PowerContrlBanBen[i]=Inside_Reveive_Can_Packet.data[i+1];
				break;		
			case SendSelfID1:	
				PowerContrlBanBen[7]=Inside_Reveive_Can_Packet.data[1];
				break;			
			default:
				break;
		}		
	}
	else if(Inside_Reveive_Can_Packet.identifier==CarSpeed_ID)
	{
		if(CarSpeedCaptureLocation!=CarSpeedCanMessage)
		{
			if(OnKeyBit==KeyOn)
			{				
				ReCarSpeedTemp = Inside_Reveive_Can_Packet.data[7]; ///vehicle speed		

				if (ReCarSpeedTemp > SPEEDHIGHNUM)
		  		carspeed = SPEEDHIGHNUM;
				else if(ReCarSpeedTemp<=SPEEDRESIVENUM)
					carspeed=ReCarSpeedTemp;
				else
				{
					if((ReCarSpeedTemp>=carspeed+SPEEDRESIVENUM)||(ReCarSpeedTemp<=carspeed-SPEEDRESIVENUM))///10.04.25 delete
						carspeed=ReCarSpeedTemp;
				}
				if (resetback0 == 0x00)
		  		CARSPEED();
				Speed_Data_Flag = 1;
				SleepFlag = 0;
			}		
		}
	}
	else if(Inside_Reveive_Can_Packet.identifier==EngineSpeed_ID)
	{
		if(OnKeyBit==KeyOn)
		{			
		 	ReRotationTemp = Inside_Reveive_Can_Packet.data[3]; ///vehicle rotation	
			ReRotationTemp = ReRotationTemp<<8;
			ReRotationTemp += Inside_Reveive_Can_Packet.data[4];

			if(ReRotationTemp>ENGINEHIGHNUM)
	  		engSpeed=ENGINEHIGHNUM;
			else if(ReRotationTemp<=ENGINERESIVENUM)
				engSpeed=ReRotationTemp;
			else
			{
				if((ReRotationTemp>engSpeed+ENGINERESIVENUM)||(ReRotationTemp<engSpeed-ENGINERESIVENUM))
					engSpeed=ReRotationTemp;
			}
			if (resetback0 == 0x00)
	  		EngRotation();
			Rotation_Data_Flag=1;
			SleepFlag = 0;
		}	
	}
  CAN0RFLG |= 0x01; //clear flag of full fifo,show that the next data can enter  
 
}
/*****************************************************************************
 ******************************************************************************/
interrupt void Can0WakeUpInterrupt(void) ///vector number is 36
{
  WakeUpInit();
  CAN0RFLG_WUPIF = 1;
}
/*******************************************************************/
interrupt void WatchDogIsr(void) ///vector number is 2
{
  asm nop;
  asm nop;
  PowerOffSave();
  //when cop overflow,save stepper and distance data first;08.12.19 add		
  __asm jmp _Startup;
}
#pragma CODE_SEG DEFAULT
