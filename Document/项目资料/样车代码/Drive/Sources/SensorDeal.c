#define	_NO_SensorDeal_EXTERN
#include	"SensorDeal.h"
/**************************************************************************************
* brief:    tmp2 channel1 capture interrupt service routine
* author:   yao qing rui
*arithmetic:															1公里
		单个脉冲距离(公里）= --------------------------------------------------------
													                速比（1公里走了几圈）*脉冲个数（1圈几个霍尔传感器）

													                   捕获时钟周期（目前以16US*2）*两次捕获寄存器差值
		单个脉冲时间（小时）= ------------------------------------------------------------
																		    3600*1000*1000(小时转换成us)
													            单个脉冲距离(公里）
		速度（公里/小时）=  --------------------------
													           单个脉冲时间（小时）
**************************************************************************************/ 
/////////////////////////////////////
interrupt void tpm2ch0_isr(void)
{
	unsigned long	Km_Capture_Num=0;
	unsigned long	Time_100us=0;
	unsigned char	TempSpeedData=0;
	unsigned long	Capture_Time_Temp=0;
	unsigned int	SpeedSendSum=0; 
	unsigned char i,Speed_Max,Speed_Min,Temp;
	
  Time_Capture_Zero=0;		///when no capture,it will not enter this
  if(Capture_Start_Flag==0)
		Capture_Timer_Num[0]=TPM2C0V;
	else
	{

		Capture_Timer_Num[1]=TPM2C0V;

		
		if(Capture_Timer_Num[1]>Capture_Timer_Num[0])
			Capture_Time_Temp=Capture_Timer_Num[1]-Capture_Timer_Num[0];
		else
			Capture_Time_Temp=0xffff+Capture_Timer_Num[1]-Capture_Timer_Num[0];
		Time_100us=3600*((unsigned long)1000);	///because 3600 or 1000 is not "unsigned long", So must change type
		Time_100us=Time_100us*(unsigned long)Capture_Time;
		Time_100us=Time_100us/4;
		Km_Capture_Num=Speed_Pulse_Number*((unsigned long)Speed_Parameter);
		Km_Capture_Num=Km_Capture_Num*Capture_Time_Temp;
		TempSpeedData=(unsigned char)(Time_100us/Km_Capture_Num);

		SpeedDataNum++;
		SpeedData[SpeedDataNum-1]=TempSpeedData;
		if(TempSpeedData<SPEED_CAPTURE_SEC1)///1~10km/h
		{
			SendSpeedData[0]=TempSpeedData;
			SpeedDataNum=0;
		}
		else if(TempSpeedData<SPEED_CAPTURE_SEC2)///10~15km/h
		{
			if(SpeedDataNum>=SEC2_CAPTURE_NUM)
			{
				Speed_Max=SpeedData[0];
				Speed_Min=SpeedData[0];
				SpeedSendSum=SpeedData[0];
				
				for(i=1;i<SEC2_CAPTURE_NUM;i++)
				{
					SpeedSendSum+=SpeedData[i];
					if(Speed_Max<SpeedData[i])
						Speed_Max=SpeedData[i];
				}
				SendSpeedData[0]=(unsigned char)((SpeedSendSum-Speed_Max)/2);
				SpeedDataNum=0;///start the next capture
			}
		}
		else if(TempSpeedData<SPEED_CAPTURE_SEC3)///15~20km/h
		{
			if(SpeedDataNum>=SEC3_CAPTURE_NUM)
			{
				Speed_Max=SpeedData[0];
				Speed_Min=SpeedData[0];
				SpeedSendSum=SpeedData[0];
				
				for(i=1;i<SEC3_CAPTURE_NUM;i++)
				{
					SpeedSendSum+=SpeedData[i];
					if(Speed_Max<SpeedData[i])
						Speed_Max=SpeedData[i];
					if(Speed_Min>SpeedData[i])
						Speed_Min=SpeedData[i];
				}
				SendSpeedData[0]=(unsigned char)((SpeedSendSum-Speed_Max-Speed_Min)/2);
				SpeedDataNum=0;///start the next capture
			}
		}
		else 
		{
			Temp=TempSpeedData/10;
			switch(Temp)
			{
				case 2: 		///20~30km/h
				if(SpeedDataNum>=SEC4_CAPTURE_NUM)
				{
					Speed_Max=SpeedData[0];
					Speed_Min=SpeedData[0];
					SpeedSendSum=SpeedData[0];
					
					for(i=1;i<SEC4_CAPTURE_NUM;i++)
					{
						SpeedSendSum+=SpeedData[i];
						if(Speed_Max<SpeedData[i])
							Speed_Max=SpeedData[i];
						if(Speed_Min>SpeedData[i])
							Speed_Min=SpeedData[i];
					}
					SendSpeedData[0]=(unsigned char)((SpeedSendSum-Speed_Max-Speed_Min)/(SEC4_CAPTURE_NUM-2));
					SpeedDataNum=0;///start the next capture
				}
				break;
				case 3: 		///30~50km/h
				case 4:
				if(SpeedDataNum>=SEC5_CAPTURE_NUM)
				{
					Speed_Max=SpeedData[0];
					Speed_Min=SpeedData[0];
					SpeedSendSum=SpeedData[0];
					
					for(i=1;i<SEC5_CAPTURE_NUM;i++)
					{
						SpeedSendSum+=SpeedData[i];
						if(Speed_Max<SpeedData[i])
							Speed_Max=SpeedData[i];
						if(Speed_Min>SpeedData[i])
							Speed_Min=SpeedData[i];
					}
					SendSpeedData[0]=(unsigned char)((SpeedSendSum-Speed_Max-Speed_Min)/(SEC5_CAPTURE_NUM-2));
					SpeedDataNum=0;///start the next capture
				}
				break;
				case 5: 		///50~70km/h
				case 6:
				if(SpeedDataNum>=SEC6_CAPTURE_NUM)
				{
					Speed_Max=SpeedData[0];
					Speed_Min=SpeedData[0];
					SpeedSendSum=SpeedData[0];
					
					for(i=1;i<SEC6_CAPTURE_NUM;i++)
					{
						SpeedSendSum+=SpeedData[i];
						if(Speed_Max<SpeedData[i])
							Speed_Max=SpeedData[i];
						if(Speed_Min>SpeedData[i])
							Speed_Min=SpeedData[i];
					}
					SendSpeedData[0]=(unsigned char)((SpeedSendSum-Speed_Max-Speed_Min)/(SEC6_CAPTURE_NUM-2));
					SpeedDataNum=0;///start the next capture
				}
				break;
				case 7: 		///70~90km/h
				case 8:
				if(SpeedDataNum>=SEC7_CAPTURE_NUM)
				{
					Speed_Max=SpeedData[0];
					Speed_Min=SpeedData[0];
					SpeedSendSum=SpeedData[0];
					
					for(i=1;i<SEC7_CAPTURE_NUM;i++)
					{
						SpeedSendSum+=SpeedData[i];
						if(Speed_Max<SpeedData[i])
							Speed_Max=SpeedData[i];
						if(Speed_Min>SpeedData[i])
							Speed_Min=SpeedData[i];
					}
					SendSpeedData[0]=(unsigned char)((SpeedSendSum-Speed_Max-Speed_Min)/(SEC7_CAPTURE_NUM-2));
					SpeedDataNum=0;///start the next capture
				}
				break;
				case 9: 		///90~110km/h
				case 10:
				if(SpeedDataNum>=SEC8_CAPTURE_NUM)
				{
					Speed_Max=SpeedData[0];
					Speed_Min=SpeedData[0];
					SpeedSendSum=SpeedData[0];
					
					for(i=1;i<SEC8_CAPTURE_NUM;i++)
					{
						SpeedSendSum+=SpeedData[i];
						if(Speed_Max<SpeedData[i])
							Speed_Max=SpeedData[i];
						if(Speed_Min>SpeedData[i])
							Speed_Min=SpeedData[i];
					}
					SendSpeedData[0]=(unsigned char)((SpeedSendSum-Speed_Max-Speed_Min)/(SEC8_CAPTURE_NUM-2));
					SpeedDataNum=0;///start the next capture
				}
				break;
				case 11:			///110~140km/h
				case 12:
				case 13:	
				case 14:
				if(SpeedDataNum>=SEC9_CAPTURE_NUM)
				{
					Speed_Max=SpeedData[0];
					Speed_Min=SpeedData[0];
					SpeedSendSum=SpeedData[0];
					
					for(i=1;i<SEC9_CAPTURE_NUM;i++)
					{
						SpeedSendSum+=SpeedData[i];
						if(Speed_Max<SpeedData[i])
							Speed_Max=SpeedData[i];
						if(Speed_Min>SpeedData[i])
							Speed_Min=SpeedData[i];
					}
					SendSpeedData[0]=(unsigned char)((SpeedSendSum-Speed_Max-Speed_Min)/(SEC9_CAPTURE_NUM-2));
					SpeedDataNum=0;///start the next capture
				}
				break;
				default:
				if(SpeedDataNum>=SEC9_CAPTURE_NUM)
				{
					Speed_Max=SpeedData[0];
					Speed_Min=SpeedData[0];
					SpeedSendSum=SpeedData[0];
					
					for(i=1;i<SEC9_CAPTURE_NUM;i++)
					{
						SpeedSendSum+=SpeedData[i];
						if(Speed_Max<SpeedData[i])
							Speed_Max=SpeedData[i];
						if(Speed_Min>SpeedData[i])
							Speed_Min=SpeedData[i];
					}
					SendSpeedData[0]=(unsigned char)((SpeedSendSum-Speed_Max-Speed_Min)/(SEC9_CAPTURE_NUM-2));
					SpeedDataNum=0;///start the next capture
				}
				break;
			}
		}
		if(SpeedDataNum==0)///当采集结束时判断状态是否有异常干扰
		{
			///如果车速小于2km且超过5km的变化，过滤2次
			if((SendSpeedData[0] > (SendSpeedDataLast+5 ))&&(SendSpeedDataLast<2))
			{
				SendSpeedDataUpdata1Num++;
				if(SendSpeedDataUpdata1Num<2)
					SendSpeedData[0]= SendSpeedDataLast;	 
			}
			///如果车速一次采集超过10km的变化，过滤3次
			else if((SendSpeedData[0] > (SendSpeedDataLast+10 )))
			{
				 SendSpeedDataUpdataNum++;
				 if(SendSpeedDataUpdataNum<3)
					SendSpeedData[0]= SendSpeedDataLast; 		 
			}
			else
			{
				SendSpeedDataUpdata1Num=0;
				SendSpeedDataUpdataNum=0;
			}
			SendSpeedDataLast=SendSpeedData[0];
			ReSpeedData=SendSpeedData[0];///在本模块采集时统一用ReSpeedData参数来控制负载
		}			
	 }
	Capture_Start_Flag=~Capture_Start_Flag; 	///when capture is change,then determine the speed data
	TPM2C0SC_CH0F=0;	   //clear interrupt flag
}
/**************************************************************************************
* brief:    tmp2 channel1 capture interrupt service routine
* author:   yao qing rui
*arithmetic:															
						       1				                实际转速值
   ---------------------------- = ------------------------------------
 （飞轮齿数*单个脉冲时间（us））	 60*1000*1000（将min转换成us）

  公式描述：转速单位是rpm/min，即1分钟多少转；发动机1转即为飞轮动作一周，所以飞轮齿数*单个脉冲时间即为1转所用的时间。
		
**************************************************************************************/ 
///configure rotation macros
static volatile const unsigned	int Rotation_Rate @0xE803=141;///占了0xe803及0xe804两个位置		
/////////////////////////////////////
interrupt void tpm2ch1_isr(void)
{
	unsigned long	Rotation_Capture_Num=0;
	unsigned long	Time_100us=0;
	unsigned int	TempRotationData=0;
	unsigned long	Capture_Time_Temp=0;
	unsigned long	RotationSendSum=0;	
	unsigned char i;
	unsigned int	Rotation_Max,Rotation_Min,Temp;
	
	
	Time_Rotation_Zero=0;		///when no capture,it will not enter this
	 
	if(Capture_Rotation_Flag==0)
		Rotation_Capture_Timer_Num[0]=TPM2C1V;
	else
	{
	 	Rotation_Capture_Timer_Num[1]=TPM2C1V;
		if(Rotation_Capture_Timer_Num[1]>Rotation_Capture_Timer_Num[0])
			Capture_Time_Temp=Rotation_Capture_Timer_Num[1]-Rotation_Capture_Timer_Num[0];
		else
			Capture_Time_Temp=0xffff+Rotation_Capture_Timer_Num[1]-Rotation_Capture_Timer_Num[0];
	 	Time_100us=60*((unsigned long)1000);	///because 60 or 1000 is not "unsigned long", So must change type
		Time_100us=Time_100us*((unsigned long)1000);
		Rotation_Capture_Num=Time_100us/((unsigned long)Rotation_Rate);
		Rotation_Capture_Num=Rotation_Capture_Num/Capture_Time_Temp;
		TempRotationData=(unsigned int)(Rotation_Capture_Num/((unsigned long)Rotation_Capture_Time));

		RotationDataNum++;
		CaptureRotationData[RotationDataNum-1]=TempRotationData;
		if(TempRotationData<ROTATION_CAPTURE_SEC1)///200rpm
		{
			SendRotationData[0]=TempRotationData;
			RotationDataNum=0;
		}
		else if(TempRotationData<ROTATION_CAPTURE_SEC2)///200~400
		{
			if(RotationDataNum>=SEC2_ROTATION_NUM)
			{
				Rotation_Max=CaptureRotationData[0];
				Rotation_Min=CaptureRotationData[0];
				RotationSendSum=CaptureRotationData[0];
				
				for(i=1;i<SEC2_ROTATION_NUM;i++)
				{
					RotationSendSum+=CaptureRotationData[i];
					if(Rotation_Max<CaptureRotationData[i])
						Rotation_Max=CaptureRotationData[i];
				}
				SendRotationData[0]=(unsigned int)((RotationSendSum-Rotation_Max)/2);
				RotationDataNum=0;///start the next capture
			}
		}
		else if(TempRotationData<ROTATION_CAPTURE_SEC3)///400~600
		{
			if(RotationDataNum>=SEC3_ROTATION_NUM)
			{
				Rotation_Max=CaptureRotationData[0];
				Rotation_Min=CaptureRotationData[0];
				RotationSendSum=CaptureRotationData[0];
				
				for(i=1;i<SEC3_ROTATION_NUM;i++)
				{
					RotationSendSum+=CaptureRotationData[i];
					if(Rotation_Max<CaptureRotationData[i])
						Rotation_Max=CaptureRotationData[i];
					if(Rotation_Min>CaptureRotationData[i])
						Rotation_Min=CaptureRotationData[i];
				}
				SendRotationData[0]=(unsigned int)((RotationSendSum-Rotation_Max-Rotation_Min)/2);
				RotationDataNum=0;///start the next capture
			}
		}
		else 
		{
			Temp=TempRotationData/100;
			switch(Temp)
			{
				case 6:			///600~1000
				case 7:	
				case 8:		
				case 9:
					if(RotationDataNum>=SEC4_ROTATION_NUM)
					{
						Rotation_Max=CaptureRotationData[0];
						Rotation_Min=CaptureRotationData[0];
						RotationSendSum=CaptureRotationData[0];
					
						for(i=1;i<SEC4_ROTATION_NUM;i++)
						{
							RotationSendSum+=CaptureRotationData[i];
							if(Rotation_Max<CaptureRotationData[i])
								Rotation_Max=CaptureRotationData[i];
							if(Rotation_Min>CaptureRotationData[i])
								Rotation_Min=CaptureRotationData[i];
						}
						SendRotationData[0]=(unsigned int)((RotationSendSum-Rotation_Max-Rotation_Min)/(SEC4_ROTATION_NUM-2));
						RotationDataNum=0;///start the next capture
					}
					break;
				case 10:			///1000~1400
				case 11:
				case 12:
				case 13:
					if(RotationDataNum>=SEC5_ROTATION_NUM)
					{
						Rotation_Max=CaptureRotationData[0];
						Rotation_Min=CaptureRotationData[0];
						RotationSendSum=CaptureRotationData[0];
					
						for(i=1;i<SEC5_ROTATION_NUM;i++)
						{
							RotationSendSum+=CaptureRotationData[i];
							if(Rotation_Max<CaptureRotationData[i])
								Rotation_Max=CaptureRotationData[i];
							if(Rotation_Min>CaptureRotationData[i])
								Rotation_Min=CaptureRotationData[i];
						}
						SendRotationData[0]=(unsigned int)((RotationSendSum-Rotation_Max-Rotation_Min)/(SEC5_ROTATION_NUM-2));
						RotationDataNum=0;///start the next capture
					}
					break;
				case 14:			///1400~1800
				case 15:
				case 16:
				case 17:
					if(RotationDataNum>=SEC6_ROTATION_NUM)
					{
						Rotation_Max=CaptureRotationData[0];
						Rotation_Min=CaptureRotationData[0];
						RotationSendSum=CaptureRotationData[0];
					
						for(i=1;i<SEC6_ROTATION_NUM;i++)
						{
							RotationSendSum+=CaptureRotationData[i];
							if(Rotation_Max<CaptureRotationData[i])
								Rotation_Max=CaptureRotationData[i];
							if(Rotation_Min>CaptureRotationData[i])
								Rotation_Min=CaptureRotationData[i];
						}
						SendRotationData[0]=(unsigned int)((RotationSendSum-Rotation_Max-Rotation_Min)/(SEC6_ROTATION_NUM-2));
						RotationDataNum=0;///start the next capture
					}
					break;
				case 18:			///1800~2200
				case 19:			
				case 20:
				case 21:
					if(RotationDataNum>=SEC7_ROTATION_NUM)
					{
						Rotation_Max=CaptureRotationData[0];
						Rotation_Min=CaptureRotationData[0];
						RotationSendSum=CaptureRotationData[0];
					
						for(i=1;i<SEC7_ROTATION_NUM;i++)
						{
							RotationSendSum+=CaptureRotationData[i];
							if(Rotation_Max<CaptureRotationData[i])
								Rotation_Max=CaptureRotationData[i];
							if(Rotation_Min>CaptureRotationData[i])
								Rotation_Min=CaptureRotationData[i];
						}
						SendRotationData[0]=(unsigned int)((RotationSendSum-Rotation_Max-Rotation_Min)/(SEC7_ROTATION_NUM-2));
						RotationDataNum=0;///start the next capture
					}
					break;
				case 22:			///2200~2600
				case 23:
				case 24:
				case 25:
					if(RotationDataNum>=SEC8_ROTATION_NUM)
					{
						Rotation_Max=CaptureRotationData[0];
						Rotation_Min=CaptureRotationData[0];
						RotationSendSum=CaptureRotationData[0];
					
						for(i=1;i<SEC8_ROTATION_NUM;i++)
						{
							RotationSendSum+=CaptureRotationData[i];
							if(Rotation_Max<CaptureRotationData[i])
								Rotation_Max=CaptureRotationData[i];
							if(Rotation_Min>CaptureRotationData[i])
								Rotation_Min=CaptureRotationData[i];
						}
						SendRotationData[0]=(unsigned int)((RotationSendSum-Rotation_Max-Rotation_Min)/(SEC8_ROTATION_NUM-2));
						RotationDataNum=0;///start the next capture
					}
					break;
				case 26:			///2600~3000
				case 27:
				case 28:
				case 29:
					if(RotationDataNum>=SEC9_ROTATION_NUM)
					{
						Rotation_Max=CaptureRotationData[0];
						Rotation_Min=CaptureRotationData[0];
						RotationSendSum=CaptureRotationData[0];
					
						for(i=1;i<SEC9_ROTATION_NUM;i++)
						{
							RotationSendSum+=CaptureRotationData[i];
							if(Rotation_Max<CaptureRotationData[i])
								Rotation_Max=CaptureRotationData[i];
							if(Rotation_Min>CaptureRotationData[i])
								Rotation_Min=CaptureRotationData[i];
						}
						SendRotationData[0]=(unsigned int)((RotationSendSum-Rotation_Max-Rotation_Min)/(SEC9_ROTATION_NUM-2));
						RotationDataNum=0;///start the next capture
					}
					break;
				default:
					if(RotationDataNum>=SEC9_ROTATION_NUM)
					{
						Rotation_Max=CaptureRotationData[0];
						Rotation_Min=CaptureRotationData[0];
						RotationSendSum=CaptureRotationData[0];
					
						for(i=1;i<SEC9_ROTATION_NUM;i++)
						{
							RotationSendSum+=CaptureRotationData[i];
							if(Rotation_Max<CaptureRotationData[i])
								Rotation_Max=CaptureRotationData[i];
							if(Rotation_Min>CaptureRotationData[i])
								Rotation_Min=CaptureRotationData[i];
						}
						SendRotationData[0]=(unsigned int)((RotationSendSum-Rotation_Max-Rotation_Min)/(SEC9_ROTATION_NUM-2));
						RotationDataNum=0;///start the next capture
					}
					break;
			}
		}
	 }
	///_Capture_Out:
	Capture_Rotation_Flag=~Capture_Rotation_Flag;		///when capture is change,then determine the speed data
	TPM2C1SC_CH1F=0;       //clear interrupt flag
}
/***********************************************
*brief    Sensor data deal
*author   karry
*input    void
*output   void  
************************************************/
///type configure
static volatile const unsigned	char Model_Type @0xE80E=1;///1->6802,2->6753

void	SensorDataDeal(void)
{
	unsigned char	i;
	unsigned char	FuelLevelNum_Temp=0,OilPressureNum_Temp=0;
	unsigned char	GasPress1Num_Temp=0,GasPress2Num_Temp=0;
	unsigned char	TeperatureNum_Temp=0,EngineRoomTemp_Temp=0;

	
	if(BackDriveFlag==1)
	{
		////fuel level sensor 
		if(FuelLevel_2sFlag==1)							///0.5s extract 1 time
		{
			FuelLevel_2sFlag=0;
			Time_Change_FuelLevel=0;
			if(SensorData[0]>SensorOpenNum)					
			{
				FuelLevelNum_Temp=SensorErrorNum;
				goto	_FuelLevelSensorDeal;				
			}
			for(i=0;i<100;i++)
			{
				if((SensorData[0]>=FuelLevel_Table[i])&&(SensorData[0]<=FuelLevel_Table[i+1]))
				{
					FuelLevelNum_Temp=i;	
					break;
				}		
			}	
			if(SensorData[0]>FuelLevel_Table[100])	///09.11.21add to resolve"sensor parameter out of area"
				FuelLevelNum_Temp=100;
			///compare 2 time to obtain a realize data
			_FuelLevelSensorDeal:
			Count_FuelLevel_Num++;
			if(Count_FuelLevel_Num==1)
			{
				FuelLevel_Num[0]=FuelLevelNum_Temp;
				goto	_FuelLevelSensorDealOut;
			}
		
			if(Count_FuelLevel_Num==2)
			{
				FuelLevel_Num[1]=FuelLevelNum_Temp;
				Count_FuelLevel_Num=0;
			}
			if(FuelLevel_Num[0]==FuelLevel_Num[1])
				FuelLevelNum=FuelLevel_Num[1];
			SendSensorData[3]=FuelLevelNum;
			_FuelLevelSensorDealOut:
			asm nop;			
		}		
		#if 0
		////oil pressure sensor adc
		if(OilChangeFlag==1)						///when power off 0.5s extract 1 time,when power on 15s extract 1 time
		{
			OilChangeFlag=0;
			Time_Change_Oil=0;
			if(SensorData[1]>SensorOpenNum)					
			{
				OilPressureNum_Temp=SensorErrorNum;
				goto	_OilSensorDeal;				
			}
			for(i=0;i<46;i++)
			{
				if((SensorData[1]>=OilPressure_Table1[i])&&(SensorData[1]<=OilPressure_Table1[i+1]))
				{
					OilPressureNum_Temp=i;	
					break;
				}		
			}		
			if(SensorData[1]>OilPressure_Table1[45])
				OilPressureNum_Temp=45;
			/*
			for(i=45;i>0;i--)
			{
				if((SensorData[0]>=OilPressure_Table2[i])&&(SensorData[0]<=OilPressure_Table2[i-1]))
				{
					OilPressureNum_Temp=i;	
					break;
				}		
			}
			*/
			/***compare 2 time to obtain a realize data***/
			_OilSensorDeal:
			Count_OilPressure_Num++;
			if(Count_OilPressure_Num==1)
			{
				OilPressure_Num[0]=OilPressureNum_Temp;
				goto	_OilSensorDealOut;
			}	
			if(Count_OilPressure_Num==2)
			{
				OilPressure_Num[1]=OilPressureNum_Temp;
				Count_OilPressure_Num=0;
			}
			if(OilPressure_Num[0]==OilPressure_Num[1])
				OilPressureNum=OilPressure_Num[1];
			SendSensorData[2]=OilPressureNum;
			_OilSensorDealOut:
			asm nop;
		}
		////fuel level sensor 
		if(FuelLevel_2sFlag==1)							///0.5s extract 1 time
		{
			FuelLevel_2sFlag=0;
			Time_Change_FuelLevel=0;
			if(SensorData[0]>SensorOpenNum)					
			{
				FuelLevelNum_Temp=SensorErrorNum;
				goto	_FuelLevelSensorDeal;				
			}
			for(i=0;i<100;i++)
			{
				if((SensorData[0]>=FuelLevel_Table[i])&&(SensorData[0]<=FuelLevel_Table[i+1]))
				{
					FuelLevelNum_Temp=i;	
					break;
				}		
			}	
			if(SensorData[0]>FuelLevel_Table[100])	///09.11.21add to resolve"sensor parameter out of area"
				FuelLevelNum_Temp=100;
			///compare 2 time to obtain a realize data
			_FuelLevelSensorDeal:
			Count_FuelLevel_Num++;
			if(Count_FuelLevel_Num==1)
			{
				FuelLevel_Num[0]=FuelLevelNum_Temp;
				goto	_FuelLevelSensorDealOut;
			}
		
			if(Count_FuelLevel_Num==2)
			{
				FuelLevel_Num[1]=FuelLevelNum_Temp;
				Count_FuelLevel_Num=0;
			}
			if(FuelLevel_Num[0]==FuelLevel_Num[1])
				FuelLevelNum=FuelLevel_Num[1];
			SendSensorData[1]=FuelLevelNum;
			_FuelLevelSensorDealOut:
			asm nop;			
		}
		////仓温传感器adc
		if(EngineRoomTempFlag==1)						///0.5s extract 1 time
		{
			EngineRoomTempFlag=0;
			Time_EngineRoomTemp=0;
			if(SensorData[2]>SensorOpenNum)					
			{
				EngineRoomTemp_Temp=SensorErrorNum;			
			}
			else
			{		
				for(i=80;i>0;i--)
				{
					if((SensorData[2]>=EngineRoom_Table[i])&&(SensorData[2]<=EngineRoom_Table[i-1]))
					{
						EngineRoomTemp_Temp=i;	
						EngineRoomTemp_Temp=EngineRoomTemp_Temp+40;///温度从40度开始
						break;
					}		
				}		
				if(SensorData[2]<EngineRoom_Table[79])
					EngineRoomTemp_Temp=120;///最大120度
			}
			/***compare 2 time to obtain a realize data***/
			Count_EngineRoomTemp_Num++;
			if(Count_EngineRoomTemp_Num==1)
			{
				EngineRoomTemp_Num[0]=EngineRoomTemp_Temp;
			}
			else if(Count_EngineRoomTemp_Num==2)
			{
				EngineRoomTemp_Num[1]=EngineRoomTemp_Temp;
				Count_EngineRoomTemp_Num=0;
				if(EngineRoomTemp_Num[0]==EngineRoomTemp_Num[1])
					SendSensorData[4]=EngineRoomTemp_Num[1];
			}			
		}
		////水温传感器adc
		if(EngineWaterTempFlag==1)						///0.5s extract 1 time
		{
			EngineWaterTempFlag=0;
			Time_EngineWaterTemp=0;
			if(SensorData[3]>SensorOpenNum)					
			{
				EngineRoomTemp_Temp=SensorErrorNum;			
			}
			else
			{		
				for(i=80;i>0;i--)
				{
					if((SensorData[3]>=EngineRoom_Table[i])&&(SensorData[3]<=EngineRoom_Table[i-1]))
					{
						EngineRoomTemp_Temp=i;	
						EngineRoomTemp_Temp=EngineRoomTemp_Temp+40;///温度从40度开始
						break;
					}		
				}		
				if(SensorData[3]<EngineRoom_Table[79])
					EngineRoomTemp_Temp=120;///最大120度
			}
			/***compare 2 time to obtain a realize data***/
			Count_EngineWaterTemp_Num++;
			if(Count_EngineWaterTemp_Num==1)
			{
				EngineWaterTemp_Num[0]=EngineRoomTemp_Temp;
			}
			else if(Count_EngineWaterTemp_Num==2)
			{
				EngineWaterTemp_Num[1]=EngineRoomTemp_Temp;
				Count_EngineWaterTemp_Num=0;
				if(EngineWaterTemp_Num[0]==EngineWaterTemp_Num[1])
					SendSensorData[0]=EngineWaterTemp_Num[1];
			}			
		}
		#endif
	}
	
	if(FrontDriveFlag==1)
	{
		///oil pressure sensor adc
		if(OilChangeFlag==1)						///if power off 0.5s extract 1 time,else 15s extract 1 time
		{
			OilChangeFlag=0;
			Time_Change_Oil=0;
			if(SensorData[0]>SensorOpenNum) 				
				OilPressureNum_Temp=SensorErrorNum;
			else
			{
				for(i=0;i<46;i++)
				{
					if((SensorData[0]>=OilPressure_Table1[i])&&(SensorData[0]<=OilPressure_Table1[i+1]))
					{
						OilPressureNum_Temp=i;	
						break;
					} 	
				} 	
				if(SensorData[0]>OilPressure_Table1[45])
					OilPressureNum_Temp=45;
			}
			///compare 2 time to obtain a realize data
			Count_OilPressure_Num++;
			if(Count_OilPressure_Num==1)
				OilPressure_Num[0]=OilPressureNum_Temp;
			else if(Count_OilPressure_Num==2)
			{
				OilPressure_Num[1]=OilPressureNum_Temp;
				Count_OilPressure_Num=0;
				if(OilPressure_Num[0]==OilPressure_Num[1])
					SendSensorData[0]=OilPressure_Num[1];
			}
		}
		
		////gas1 pressure sensor adc
		if(GasPress1_ChangeFlag==1)				///0.5s extract 1 time
			{
				GasPress1_ChangeFlag=0;
				Time_Change_GasPress1=0;
				if(SensorData[1]>SensorOpenNum)					
				{
					GasPress1Num_Temp=SensorErrorNum;
					goto	_GasPress1Deal;				
				}
				for(i=0;i<100;i++)
				{
					if((SensorData[1]>=GasPressure_Table[i])&&(SensorData[1]<=GasPressure_Table[i+1]))
					{
						GasPress1Num_Temp=i;	
						break;
					}		
				}
				if(SensorData[1]>GasPressure_Table[100])///09.11.21add to resolve"sensor parameter out of area"
					GasPress1Num_Temp=100;	
				/***compare 2 time to obtain a realize data***/
				_GasPress1Deal:
				Count_GasPress1_Num++;
				if(Count_GasPress1_Num==1)
				{
					GasPress1_Num[0]=GasPress1Num_Temp;
					goto	_GasPress1DealOut;
				}
				if(Count_GasPress1_Num==2)
				{
					GasPress1_Num[1]=GasPress1Num_Temp;
					Count_GasPress1_Num=0;
				}
				if(GasPress1_Num[0]==GasPress1_Num[1])
					GasPress1Num=GasPress1_Num[1];
				SendSensorData[1]=GasPress1Num;
				_GasPress1DealOut:
				asm nop;
			}
			////gas2 pressure adc
			if(GasPress2_ChangeFlag==1)						///0.5s extract 1 time
			{
				GasPress2_ChangeFlag=0;
				Time_Change_GasPress2=0;
				if(SensorData[2]>SensorOpenNum)					
				{
					GasPress2Num_Temp=SensorErrorNum;
					goto	_GasPrees2Deal;				
				}
				for(i=0;i<100;i++)
				{
					if((SensorData[2]>=GasPressure_Table[i])&&(SensorData[2]<=GasPressure_Table[i+1]))
					{
						GasPress2Num_Temp=i;		
						break;
					}	
				}			
				if(SensorData[2]>GasPressure_Table[100])///09.11.21add to resolve"sensor parameter out of area"
					GasPress2Num_Temp=100;
				/***compare 2 time to obtain a realize data***/
				_GasPrees2Deal:
				Count_GasPress2_Num++;
				if(Count_GasPress2_Num==1)
				{
					GasPress2_Num[0]=GasPress2Num_Temp;
					goto	_GasPress2DealOut;
				}
				if(Count_GasPress2_Num==2)
				{
					GasPress2_Num[1]=GasPress2Num_Temp;
					Count_GasPress2_Num=0;
				}
				if(GasPress2_Num[0]==GasPress2_Num[1])
					GasPress2Num=GasPress2_Num[1];
				SendSensorData[2]=GasPress2Num;
				_GasPress2DealOut:
				asm nop;
			}
	}
	////Teperature sensor adc
	if(Temperature_ChangeFlag==1)				///0.5s extract 1 time
	{
		Temperature_ChangeFlag=0;
		Time_Change_Temperature=0;
		TeperatureNum_Temp=SensorData[6];	
		/***compare 2 time to obtain a realize data***/
		Count_Temperature_Num++;
		if(Count_Temperature_Num==1)
		{
			Temperature_Num[0]=TeperatureNum_Temp;
			goto	_TemperatureDealOut;
		}
		if(Count_Temperature_Num==2)
		{
			Temperature_Num[1]=TeperatureNum_Temp;
			Count_Temperature_Num=0;
		}
		if(Temperature_Num[0]==Temperature_Num[1])
		{
			/***********************************************
			temperature arithmatic:x=100y-50+40;		///x,Temperatrue number;y,Voltage of adc;40,x origin number; 
														 y=(AD_value)/256*5
			***********************************************/			
			unsigned int	CountTemperature;
			CountTemperature=Temperature_Num[1];
			CountTemperature*=5;				
			CountTemperature*=100;
			///CountTemperature/=256;
			CountTemperature>>=8;
			CountTemperature+=40;
			CountTemperature-=50;
			TemperatureNum=(unsigned char)CountTemperature;
		}
			SendSensorData[5]=TemperatureNum;
		_TemperatureDealOut:
		asm nop;	
	}
}

