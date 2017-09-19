#define 	ISR_C
#include	"ISR.H"
#include "BacklightControl.h"


BitCommon DisTimeFlag;
BitCommon TimeFlag;
UINT8  Power_Current_Valve[16]; 
UINT8 listen_mode_flag;
UINT32 PcFromMileage;
UINT8 _10msCnt;
UINT8 _50msCnt;
UINT8 _100msCnt;
St_CaptureTime   CapturePwm0,CapturePwm1;




void lpit0_isr(void){
	
	if(LPIT0_MSR & LPIT_MSR_TIF1_MASK) lpit0_ch1_isr();
	if(LPIT0_MSR & LPIT_MSR_TIF2_MASK) lpit0_ch2_isr();
	if(LPIT0_MSR & LPIT_MSR_TIF0_MASK) lpit0_ch0_isr();
	if(LPIT0_MSR & LPIT_MSR_TIF3_MASK) lpit0_ch3_isr();
}

UINT16 ShowDir=3,ShowCnt=0;
extern UINT8 RefreshCnt_100ms;
extern int speedStepRefreshCnt;

//extern St_ProgressBar  LiftColumnLed, RightColumnLed;

//1ms定时中断
void lpit0_ch0_isr(void)
{	
	static uint32_t _1msCnt = 0;
	UINT8 q;

	
	User_TimerInterrupt();//负载输出逻辑计数
	speedStepRefreshCnt--;
	
	for(q=0; q < CanMsgOutNum; q++)
	{
		Time[q].sendCycle--;

		if(Time[q].sendCycle == 0)
		{
			Time[q].Flag = 1;
			Time[q].sendCycle = Time[q].sendCycleBackup;
		}
	} 
	

	_1msCnt++; //L:应该是1ms计数
	if(_1msCnt >= 10)
	{
		//GPIOD_PTOR = GPIO3 ;
		if(listen_mode_flag==0) SendCan0Message();
		_1msCnt = 0;
		_10msCnt++;
           //doProgressBar(&ColumnBackLum);     
	}

	if (_10msCnt >= 5) 
	{
		_10msCnt = 0x00;
		_50msCnt++;
		GAIN_OPT_Flag = 1;	//采集光敏
		_50MS_F = 1;
		if (_50msCnt >= 2) 
		{
			RefreshCnt_100ms++;
			if(RefreshCnt_100ms == 10) RefreshCnt_100ms =0;
			
			_50msCnt = 0x00;
			_100msCnt++;
			_100MS_F = 1;
		       // doProgressBar(&LiftColumnLed);
			//doProgressBar(&RightColumnLed);
                        
			if (!(_100msCnt % 0x02)) //200ms
			{
				CAN0_200MS_F = 1;
			}
			if (!(_100msCnt % 0x03)) //300ms
			{
				_300MS_F = 1;
				BK_LED_refreshFlag = 1; //背光灯刷新
				BK_LCD_refreshFlag = 1; //LCD背光
			}
			if (_100msCnt % 0x02 == 0x00) //500ms
			{	
				if(Sys_Post_Flag)
				{
				  
                               if(ShowDir==0)
                               	{
				           ShowCnt++;
					    if(ShowCnt==12)ShowDir = 1;
                               	}
					else if (ShowDir==1)
					{
					      if(ShowCnt>=1)  ShowCnt--;

						//  if(ShowCnt==0) ShowDir = 3; 
					}
							   	
					
					/*static UINT8 dir = 0;
					
					Show = 0;	
					for(UINT16 i=0x200; i!=(0x0200>>ShowCnt);i>>=1)
					{
						Show |= i;
					}
					if(dir==0) 
					{
						if(ShowCnt<10)ShowCnt++;
						else dir = 1;
					}
					else if(dir == 1)
					{
						if(ShowCnt>0) ShowCnt--;
						else dir = 0;
					}*/
					
		
				}
				
				_500MS_F = 1;
			}
			if (_100msCnt>=10) //1000ms
			{
				
				if(StartLogoDelay) StartLogoDelay--;
				_100msCnt = 0;
				//_1S_F = 1;
				SENSOR_1S_F = 1;
				D_1S_F = 1;
				TPMS_1S_F = 1;
				SECOND_F = 1;
				_1S_DIS_F = 1;
				FUAL_REQUIRE_F = 1;

//				if((testCnt%10) == 0) *(UINT16 *)&StorageBuffer[27] += 500;
//				if(*(UINT16 *)&StorageBuffer[27]>7000)	*(UINT16 *)&StorageBuffer[27] = 0;
			}
		}
	}

	//clear flag
	LPIT0_MSR = LPIT_MSR_TIF0_MASK;
	//printf("Here is lpit_irq\n");
}
//转速处理定时器
void lpit0_ch1_isr(void){
	uint Temp;
	int data_Rotation;

	
	if(resetback0){
		LPIT0_TVAL1 = 10*417 * PrescaleNum;
		motor_update(TACHOMETER,CLOCKWISE);
	}
	else
	{
		data_Rotation =  MotorFinalStep[TACHOMETER]-MotorCurStep[TACHOMETER];
		//data_Rotation =  abs(data_Rotation);
		if(data_Rotation<0) data_Rotation = -1 * data_Rotation;
		//if(data_Rotation<0) data_Rotation = -1*data_Rotation;
		if(data_Rotation < 240)
		{
			Temp = data_Rotation / 24;
			switch(Temp)
			{
				case 0:
				case 1:
					LPIT0_TVAL1 = LPIT0_TVAL1-LPIT0_CVAL1+(10*417 + 8 *33)*PrescaleNum;
					break;
				case 2:
				case 3:
					LPIT0_TVAL1 = LPIT0_TVAL1-LPIT0_CVAL1+(10*417 + 0 *33)*PrescaleNum;
					break;
				//---------------------------------------------------------	
				case 4:
				case 5:
					LPIT0_TVAL1 = LPIT0_TVAL1-LPIT0_CVAL1+(9*417 + 8 *33)*PrescaleNum;
					break;
				case 6:
				case 7:
					LPIT0_TVAL1 = LPIT0_TVAL1-LPIT0_CVAL1+(9*417 + 4 *33)*PrescaleNum;
					break;
				//22222222222222222222222222222222222222222222222222222222
				default :
					LPIT0_TVAL1 =  LPIT0_TVAL1-LPIT0_CVAL1+(9*417 + 0 *33)*PrescaleNum;
					break;
			}
		}
		else
		{
			Temp=data_Rotation/240;
			switch(Temp)
			{
				case 0:
					LPIT0_TVAL1 = LPIT0_TVAL1-LPIT0_CVAL1+(9*417 + 10 *33)*PrescaleNum;
					break;
				case 1:
					LPIT0_TVAL1 = LPIT0_TVAL1-LPIT0_CVAL1+(9*417 + 6 *33)*PrescaleNum;
					break;
				case 2:
					LPIT0_TVAL1 = LPIT0_TVAL1-LPIT0_CVAL1+(8*417 + 2 *33)*PrescaleNum;
					break;
				case 3:
					LPIT0_TVAL1 = LPIT0_TVAL1-LPIT0_CVAL1+(8*417 + 0 *33)*PrescaleNum;
					break;
				case 4:
					LPIT0_TVAL1 = LPIT0_TVAL1-LPIT0_CVAL1+(7*417 + 10 *33)*PrescaleNum;
					break;
				case 5:
					LPIT0_TVAL1 = LPIT0_TVAL1-LPIT0_CVAL1+(7*417 + 12 *33)*PrescaleNum;
					break;
				case 6:
					LPIT0_TVAL1 = LPIT0_TVAL1-LPIT0_CVAL1+(6*417 + 8 *33)*PrescaleNum;
					break;
				case 7:
					LPIT0_TVAL1 = LPIT0_TVAL1-LPIT0_CVAL1+(6*417 + 6 *33)*PrescaleNum;
					break;
				default:
					LPIT0_TVAL1 = LPIT0_TVAL1-LPIT0_CVAL1+(5*417 + 4 *33)*PrescaleNum;
					break;
			}
		}
		motor_micro_update(TACHOMETER,CLOCKWISE); 
	}
	
  	//clear flag
	LPIT0_MSR = LPIT_MSR_TIF1_MASK;
}

unsigned int speedBase = 0,speedBios=0,speedDealFlag = 0;
//时速处理定时器
void lpit0_ch2_isr(void)
	{

		//uint Temp;
		int data_speed;
	
		
		if(resetback0)
		{		  
			LPIT0_TVAL2 = 10*417 * PrescaleNum;
			motor_update(Carspeed,CLOCKWISE);
			
		} 
		else
		{
			data_speed =  MotorFinalStep[Carspeed]-MotorCurStep[Carspeed];
			data_speed =  abs(data_speed);
	
			//Temp = data_speed / 24;//CAR_SPEED_1KM_STEP_NUM;
			//Temp /= 8;
			//speedBase = 74 - Temp;
			if(speedBase > 90) speedBase= 91;
			
			LPIT0_TVAL2 = LPIT0_TVAL2-LPIT0_CVAL2+((100-speedBase)*417 + speedBios *33)*PrescaleNum;//1200*PrescaleNum;
			
			motor_micro_update(Carspeed,CLOCKWISE); 

		}	
		speedDealFlag = 1;

		


		//clear flag
		LPIT0_MSR = LPIT_MSR_TIF2_MASK;
	}


//5.12ms 定时中断
void lpit0_ch3_isr(void){
	static UINT16 RtiCnt = 0;
 // LED5_TOGGLE;

	RtiCnt++;
 
  	SleepDelayCnt++;


	if (RtiCnt % 2)
	{
		KEY_SCANF_10MS_F = 1;
		UPDOWN_F = 1;

		if(ON_LOAD)
		{
			if(LEFTLogicLed || RIGHTLogicLed)
			{

				if(TURN_LEFT_LED || TURN_RIGHT_LED)
				{  
					if((LeftTurnSignalWarn==0)&&(RightTurnSignalWarn==0)) 
					{
						GEAR_SOUND_OFF_F = 0;
						//MUTE_SOUNDS_F = 1;
						SetBuzzer;								 
					}
				}
				else
				{
				
					//GEAR_SOUND_OFF_F = 1;
					BuzzerOff;// BUZZER = 0;
				}
				
			}else
			{
				GEAR_SOUND_OFF_F = 1;
				if(!do2GearSound()) 	  //二级报警   1hz可关闭
				{
					if(!do1GearSound())    //一级报警  长响可关闭
					{
						if(!do3GearSound()) //三级报警	0.5hz可关闭  //4hz
						{
							do4GearSound();  //四级报警
						}
					}
				}
			}
			
			
			BuzzerOut(1,0);
		}
		else
		{
			if(TURN_LEFT_LED || TURN_RIGHT_LED)
			{
				if((LeftTurnSignalWarn==0)&&(RightTurnSignalWarn==0)) 
				{
					GEAR_SOUND_OFF_F = 0;
					//   MUTE_SOUNDS_F = 1;
					SetBuzzer;    
				}
			}
			else
			{
				GEAR_SOUND_OFF_F = 1;
				BuzzerOff;
			}
			BuzzerOut(1,0);
		}
	} 
	
	


	if (!(RtiCnt % 50)) 
    {
    	UINT8 i;
        for (i=0;i<8;i++) 
        {
            ModuleDetectCnt[i]++;
            CanLoseDataCnt[i]++;
        }
        J1939_Speed_Send();
        Gas_Fired_Req();
    }



	
	if (!(RtiCnt % 70)) //
	{
		if(NewDisplayStatus >= 8) 
		{
			battery_req(LcdMsg.curPage); 
		}      
	}
 	if(!(RtiCnt % 100))    //0.5s
	{
		UINT8 i;
		for(i=0;i<J1939IdNum;i++) 
		{
			if(J1939DetectCnt[i].b.valid==0)   //
			{
				if(J1939DetectCnt[i].b.cnt>0)   J1939DetectCnt[i].b.cnt--;
			}   
		}
	}

	count_speed++;
	if(count_speed>150)			//5.12ms*150
	{
		//SpeedDataNum=0;
		//speed_count = 0;
		//carspeed = 0;       
		//SendSpeedDataLast=0;
		//SendSpeedData[0]=0;
		//SendSpeedDataUpdata1Num=0;
		//SendSpeedDataUpdataNum=0;	
		CarSpeedFromDashboard = 0;	
		count_speed = 0;
	}

	if (resetback0==1)  ///back0 motor delay 
	{      
		delay1s++;  
		if (delay1s > MotorBackDelayNum)  ////5.12ms*600
			resetback0 = 0;
	}

	if(RSleepFlag) 
	{  
		ClrSleepCnt++;
		if(ClrSleepCnt>=20) 
		{
			ClrSleepCnt = 0;
			RSleepFlag =0;
			SleepFlag=0;  
		}
	}

	Fual_Require_Time++;///require total fual time delay
	WaterTempFlahTime++;///high can error then water temperature flash delay  


	
  	//clear flag
	LPIT0_MSR = LPIT_MSR_TIF3_MASK;
}




/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
///////////////			CAN				//////////////////////////
/////////////////////////////////////////////////////////////////
//第三路CAN
void can0Recieve_isr(void)
{
	MSCAN1Rec(0);
	CANClearFlag(0,CAN0_REV_IMB);
	CANEnableRXBuff(0,CAN0_REV_IMB);
//while(1);
}

//对应209 外CAN
void can1Recieve_isr(void)
{
	MSCAN1Rec(1);
	CANClearFlag(1,CAN1_REV_IMB);
	CANEnableRXBuff(1,CAN1_REV_IMB);
//while(1);
}
//对应209内CAN
void can2Recieve_isr(void)
{
	CAN0rxd_int();	
	CANClearFlag(2,CAN2_REV_IMB);
	CANEnableRXBuff(2,CAN2_REV_IMB);
//while(1);
}




/***************************************************************
* @brief	内CAN 数据接收,移植209.由can2中断调用
 ***************************************************************/
 UINT8 listen_mode_flag = 0;
void CAN0rxd_int(void)
{
	UINT8  dest_id;
	UINT8  source_id;
	UINT16  func_type;
	UINT8 sub_func_type;
	UINT8  index;
	UINT32 get_cofig_addr;
	CAN_PACKET_T  RX_Message,TX_Message;
	uint8 canReFlag;

	canReFlag = CANRecData(2,CAN2_REV_IMB, &RX_Message.identifier, &RX_Message.byte_count,RX_Message.data);//失败返回1
	if(canReFlag) return;
#if 0
	RX_Message.identifier = CAN0RIDR0;
	RX_Message.identifier <<= 3;
	RX_Message.identifier |= (CAN0RIDR1>>5);
	RX_Message.identifier <<= 3;
	RX_Message.identifier |= (CAN0RIDR1&0X07);
	RX_Message.identifier <<= 8;
	RX_Message.identifier |=  CAN0RIDR2;
	RX_Message.identifier <<= 7;
	RX_Message.identifier |= (CAN0RIDR3>>1);	 
	RX_Message.byte_count= CAN0RDLR & 0x0F; 
	
	for (index=0; index<RX_Message.byte_count; index++) 
	{
		RX_Message.data[index] = *(&CAN0RDSR0 + index);
	}
#endif

	//b28-b21 目的地址
	//b8-b11 sub_func_type
	//b20-b12 func_type
	//b7-b0 来源
	dest_id   = (UINT8)(RX_Message.identifier>>21); 
	source_id = (UINT8)(RX_Message.identifier); 
	func_type = (UINT16)(RX_Message.identifier>>8); //13bit
	func_type &= 0x1fff;
	sub_func_type =(UINT8)(func_type);
	func_type>>=4;
	sub_func_type &= 0x0f;
	if((dest_id == CAN0_METER_SOURCE_ID) || (dest_id == 0)) 
	{
		switch(source_id) 
		{
			case CAN0_FRONT_SOURCE_ID:
				{
					FRONT_DRIVE_MODULE_F = 1;
					listen_mode_flag = 0;
					switch(func_type) 
					{
						case  CAN0_SW_TYPE:           
							switch(sub_func_type) 
							{
								case 0:  for(index=0;index<3;index++) USR_s_OV.FrontDriveKey[index].Data = RX_Message.data[index]; break;
								default: 	break;
							} 
							break;     
						case  CAN0_VIRTUAL_SW_TYPE:   
							switch(sub_func_type) 
							{
								case 0:  for(index=0;index<4;index++) USR_s_OV.FrontDriverVirtualKey[index].Data = RX_Message.data[index];break;
								default: 	break;
							}
							break;
						case  CAN0_LOAD_TYPE:
							switch(sub_func_type) 
							{
								case 0: for(index=0;index<5;index++) USR_LOAD_OV.FrontDriveLoad[index].Data =RX_Message.data[index];/*DisLoadWarnShow();*/ break;
								default: 	break;
							}
							break;
						case  CAN0_SENSOR_TYPE:
							switch(sub_func_type) 
							{
							case 0:  InsideData[20]=  RX_Message.data[0];   InsideData[20]<<=8;  InsideData[20] |=RX_Message.data[1];
							InsideData[21]=  RX_Message.data[2];   InsideData[21]<<=8;  InsideData[21] |=RX_Message.data[3];
							InsideData[22]=  RX_Message.data[4];   InsideData[22]<<=8;  InsideData[22] |=RX_Message.data[5];
							InsideData[23]=  RX_Message.data[6];   InsideData[23]<<=8;  InsideData[23] |=RX_Message.data[7];   break;

							case 1:  InsideData[24]=  RX_Message.data[0];   InsideData[24]<<=8;  InsideData[24] |=RX_Message.data[1];
							InsideData[25]=  RX_Message.data[2];   InsideData[25]<<=8;  InsideData[25] |=RX_Message.data[3];
							InsideData[26]=  RX_Message.data[4];   InsideData[26]<<=8;  InsideData[26] |=RX_Message.data[5];
							InsideData[27]=  RX_Message.data[6];   InsideData[27]<<=8;  InsideData[27] |=RX_Message.data[7];   break;

							case 2:  InsideData[28]=  RX_Message.data[0];   break;

							case 3:  InsideData[29]=  RX_Message.data[0];   InsideData[29]<<=8;  InsideData[29] |=RX_Message.data[1];
							InsideData[30]=  RX_Message.data[2];   InsideData[30]<<=8;  InsideData[30] |=RX_Message.data[3];
							InsideData[31]=  RX_Message.data[4];   InsideData[31]<<=8;  InsideData[31] |=RX_Message.data[5];
							InsideData[32]=  RX_Message.data[6];   InsideData[32]<<=8;  InsideData[32] |=RX_Message.data[7];  break;

							case 4:  InsideData[33]=  RX_Message.data[0];   InsideData[33]<<=8;  InsideData[33] |=RX_Message.data[1];
							InsideData[34]=  RX_Message.data[2];   InsideData[34]<<=8;  InsideData[34] |=RX_Message.data[3]; break;

							default: 	
							break;
							}
							break;
						case  CAN0_SYS_CONFIG_TYPE :
							switch(sub_func_type) 
							{
								case 2:   SPEED_DEVICE_F=1; ROTATION_DEVICE_F=1;
									USR_s_OV.CarSpeedFromDriver =  RX_Message.data[0];    
									USR_s_OV.CarSpeedFromDriver <<=8;   
									USR_s_OV.CarSpeedFromDriver |= RX_Message.data[1];     //车速

									USR_s_OV.engSpeedFromDriver =  RX_Message.data[2];    
									USR_s_OV.engSpeedFromDriver <<=8;   
									USR_s_OV.engSpeedFromDriver |= RX_Message.data[3];  
									break; 
								case 3:  
									USR_s_OV.speed_Rate =  RX_Message.data[0];    
									USR_s_OV.speed_Rate <<=8;   
									USR_s_OV.speed_Rate  |=RX_Message.data[1];     //车速比
									USR_s_OV.cycle_capture_num =  RX_Message.data[2];    
									break; 
								default: 	break;
							}
							break;

						case CAN0_MODE_VERSION_TYPE:
							switch(sub_func_type) 
							{
								case 0:  for(index=0;index<8;index++) USR_s_OV.frontDriverVersion[index] = RX_Message.data[index]; break;  
								default: 	break;
							}
							break;
						default:
							break;
					}
					break;
				}
			case CAN0_BACK_SOURCE_ID:
				{
					BACK_DRIVE_MODULE_F = 1;
					listen_mode_flag = 0;
					
					switch(func_type) 
					{
						case  CAN0_SW_TYPE:           
							switch(sub_func_type) 
							{
								case 0:  for(index=0;index<3;index++) USR_s_OV.BackDriveKey[index].Data = RX_Message.data[index]; break;
								default: 	break;
							} 
							break;     
						case  CAN0_VIRTUAL_SW_TYPE:                               
							switch(sub_func_type) 
							{
								case 0:  for(index=0;index<4;index++) USR_s_OV.BackDriverVirtualKey[index].Data = RX_Message.data[index];break;
								default: 	break;
							}
							break;
						case  CAN0_LOAD_TYPE:
							switch(sub_func_type) 
							{
							case 0: for(index=0;index<5;index++) USR_LOAD_OV.BackDriveLoad[index].Data =RX_Message.data[index];/*DisLoadWarnShow();*/ break;
							default: 	break;
							}
							break;
						case  CAN0_SENSOR_TYPE:
							switch(sub_func_type) 
							{
								case 0:  InsideData[40]=  RX_Message.data[0];   InsideData[40]<<=8;  InsideData[40] |=RX_Message.data[1];
								InsideData[41]=  RX_Message.data[2];   InsideData[41]<<=8;  InsideData[41] |=RX_Message.data[3];
								InsideData[42]=  RX_Message.data[4];   InsideData[42]<<=8;  InsideData[42] |=RX_Message.data[5];
								InsideData[43]=  RX_Message.data[6];   InsideData[43]<<=8;  InsideData[43] |=RX_Message.data[7];   break;

								case 1:  InsideData[44]=  RX_Message.data[0];   InsideData[44]<<=8;  InsideData[44] |=RX_Message.data[1];
								InsideData[45]=  RX_Message.data[2];   InsideData[45]<<=8;  InsideData[45] |=RX_Message.data[3];
								InsideData[46]=  RX_Message.data[4];   InsideData[46]<<=8;  InsideData[46] |=RX_Message.data[5];
								InsideData[47]=  RX_Message.data[6];   InsideData[47]<<=8;  InsideData[47] |=RX_Message.data[7];   break;

								case 2:  InsideData[48]=  RX_Message.data[0];   break;         

								case 3:  InsideData[49]=  RX_Message.data[0];   InsideData[49]<<=8;  InsideData[49] |=RX_Message.data[1];
								InsideData[50]=  RX_Message.data[2];   InsideData[50]<<=8;  InsideData[50] |=RX_Message.data[3];
								InsideData[51]=  RX_Message.data[4];   InsideData[51]<<=8;  InsideData[51] |=RX_Message.data[5];
								InsideData[52]=  RX_Message.data[6];   InsideData[52]<<=8;  InsideData[52] |=RX_Message.data[7];  break;

								case 4:  InsideData[53]=  RX_Message.data[0];   InsideData[53]<<=8;  InsideData[53] |=RX_Message.data[1];
								InsideData[54]=  RX_Message.data[2];   InsideData[54]<<=8;  InsideData[54] |=RX_Message.data[3]; break;


								default: 	break;
							}
							break;
						case  CAN0_SYS_CONFIG_TYPE :
							switch(sub_func_type) 
							{
								case 2:   SPEED_DEVICE_F=1; ROTATION_DEVICE_F=1;
								USR_s_OV.CarSpeedFromDriver =  RX_Message.data[0]; USR_s_OV.CarSpeedFromDriver <<=8;   USR_s_OV.CarSpeedFromDriver  |=RX_Message.data[1];     //车速
								USR_s_OV.engSpeedFromDriver =  RX_Message.data[2];    USR_s_OV.engSpeedFromDriver <<=8;   USR_s_OV.engSpeedFromDriver  |=  RX_Message.data[3];  break; 
								case 3:  
								USR_s_OV.speed_Rate =  RX_Message.data[0];    
								USR_s_OV.speed_Rate <<=8;   
								USR_s_OV.speed_Rate  |=RX_Message.data[1];     //车速比
								USR_s_OV.cycle_capture_num =  RX_Message.data[2];    break; 
								default: 	
								break;
							}
							break;
						case CAN0_MODE_VERSION_TYPE:
							switch(sub_func_type) 
							{
								case 0:  for(index=0;index<8;index++) USR_s_OV.backDriverVersion[index] = RX_Message.data[index]; break;  
								default: 	break;
							}
							break;
							default:
							break;
					}
					break;
				}
			case CAN0_TOP_SOURCE_ID:
				{
					TOP_DRIVE_MODULE_F = 1;
					listen_mode_flag = 0;
					switch(func_type) 
					{
						case  CAN0_SW_TYPE:           
							switch(sub_func_type) 
							{
							case 0:  for(index=0;index<3;index++) USR_s_OV.TopDriveKey[index].Data = RX_Message.data[index]; break;
							default: 	break;
							} 
							break;     
						case  CAN0_VIRTUAL_SW_TYPE:                               
							switch(sub_func_type) 
							{
							case 0:  for(index=0;index<4;index++) USR_s_OV.TopDriverVirtualKey[index].Data = RX_Message.data[index];break;
							default: 	break;
							}
							break;
						case  CAN0_LOAD_TYPE:
							switch(sub_func_type) 
							{
							case 0: for(index=0;index<5;index++) USR_LOAD_OV.TopDriveLoad[index].Data =RX_Message.data[index]; /*DisLoadWarnShow();*/break;
							default: 	break;
							}
							break;

						case  CAN0_SENSOR_TYPE:
							switch(sub_func_type) 
							{
							case 0:  InsideData[80]=  RX_Message.data[0];   InsideData[80]<<=8;  InsideData[80] |=RX_Message.data[1];
							InsideData[81]=  RX_Message.data[2];   InsideData[81]<<=8;  InsideData[81] |=RX_Message.data[3];
							InsideData[82]=  RX_Message.data[4];   InsideData[82]<<=8;  InsideData[82] |=RX_Message.data[5];
							InsideData[83]=  RX_Message.data[6];   InsideData[83]<<=8;  InsideData[83] |=RX_Message.data[7];   break;

							case 1:  InsideData[84]=  RX_Message.data[0];   InsideData[84]<<=8;  InsideData[84] |=RX_Message.data[1];
							InsideData[85]=  RX_Message.data[2];   InsideData[85]<<=8;  InsideData[85] |=RX_Message.data[3];
							InsideData[86]=  RX_Message.data[4];   InsideData[86]<<=8;  InsideData[86] |=RX_Message.data[5];
							InsideData[87]=  RX_Message.data[6];   InsideData[87]<<=8;  InsideData[87] |=RX_Message.data[7];   break;

							case 2:  InsideData[88]=  RX_Message.data[0];   break;

							case 3:  InsideData[89]=  RX_Message.data[0];   InsideData[89]<<=8;  InsideData[89] |=RX_Message.data[1];
							InsideData[90]=  RX_Message.data[2];   InsideData[90]<<=8;  InsideData[90] |=RX_Message.data[3];
							InsideData[91]=  RX_Message.data[4];   InsideData[91]<<=8;  InsideData[91] |=RX_Message.data[5];
							InsideData[92]=  RX_Message.data[6];   InsideData[92]<<=8;  InsideData[92] |=RX_Message.data[7];  break;

							case 4:  InsideData[93]=  RX_Message.data[0];   InsideData[93]<<=8;  InsideData[93] |=RX_Message.data[1];
							InsideData[94]=  RX_Message.data[2];   InsideData[94]<<=8;  InsideData[94] |=RX_Message.data[3]; break;

							default: 	break;
							}
							break;
						case  CAN0_SYS_CONFIG_TYPE :
							switch(sub_func_type) 
							{
								case 2:   SPEED_DEVICE_F=1; ROTATION_DEVICE_F=1;
								USR_s_OV.CarSpeedFromDriver =  RX_Message.data[0];    USR_s_OV.CarSpeedFromDriver <<=8;   USR_s_OV.CarSpeedFromDriver  |=RX_Message.data[1];     //车速
								USR_s_OV.engSpeedFromDriver =  RX_Message.data[2];    USR_s_OV.engSpeedFromDriver <<=8;   USR_s_OV.engSpeedFromDriver |=  RX_Message.data[3];  break; 
								case 3:  
								USR_s_OV.speed_Rate =  RX_Message.data[0];    
								USR_s_OV.speed_Rate <<=8;   
								USR_s_OV.speed_Rate  |=RX_Message.data[1];     //车速比
								USR_s_OV.cycle_capture_num =  RX_Message.data[2];    break; 


								default: 	
								break;
							}
							break;
						case CAN0_MODE_VERSION_TYPE:
							switch(sub_func_type) 
							{
								case 0:  for(index=0;index<8;index++) USR_s_OV.topDriverVersion[index] = RX_Message.data[index]; break;  
								default: 	break;
							}
							break;
						default:
						break;
					}

					break;
				}
			case CAN0_POWER_SOURCE_ID:
				{
					POWER_MODULE_F = 1;  
					listen_mode_flag = 0;
					switch(func_type) 
					{
						case  CAN0_SW_TYPE:           
							switch(sub_func_type) 
							{
							case 0:  for(index=0;index<1;index++) USR_s_OV.PowerDriveKey[index].Data = RX_Message.data[index];break;  
							default: 	break;
							} 
							break;     
						case  CAN0_VIRTUAL_SW_TYPE:                               
							switch(sub_func_type) 
							{
							case 0:  for(index=0;index<5;index++) USR_s_OV.PowerVirtualKey[index].Data = RX_Message.data[index];break; 
							default: 	break;
							}
							break;
						case  CAN0_LOAD_TYPE:
							switch(sub_func_type) 
							{
								case 0: for(index=0;index<5;index++) USR_LOAD_OV.PowerLoadState[index].Data = RX_Message.data[index];/*DisLoadWarnShow();*/break;
								case 1: for(index=0;index<5;index++) USR_s_OV.PowerFuse[index].Data = RX_Message.data[index];break; 
								default: 	break;
							}
							break;
						case CAN0_MODE_VERSION_TYPE:
							switch(sub_func_type) 
							{
								case 0:  for(index=0;index<8;index++) USR_s_OV.powerVersion[index] = RX_Message.data[index]; break;
								default: 	break;
							}
							break;
						case  CAN0_SENSOR_TYPE:
							switch(sub_func_type) 
							{
								case 2:  InsideData[100]=  RX_Message.data[0];  break;            
								default: 	break;
							}
							break;
						default:	break;
					} 
					break;
				}
			case CAN0_POWER_CONTOROL_ID:  
				{
					POWER_CONTOROL_MODULE_F = 1;  
					listen_mode_flag = 0;
					switch(func_type) 
					{
						case  CAN0_SW_TYPE:           
							switch(sub_func_type) 
							{
								case 0:  
									for(index=0;index<1;index++) 
										USR_s_OV.PowerContorolKey[index].Data = RX_Message.data[index];
									break;  
								default: 	break;
							} 
							break;     
						case  CAN0_VIRTUAL_SW_TYPE:                               
							switch(sub_func_type) 
							{
								case 0:  
									for(index=0;index<3;index++) 
										USR_s_OV.PowerContorolVirtualKey[index].Data = RX_Message.data[index];
									break; 
								default: 	break;
							}
							break;
						case  CAN0_LOAD_TYPE:
							switch(sub_func_type) 
							{
								case 0: for(index=0;index<4;index++) USR_LOAD_OV.PowerContorolLoadState[index].Data = RX_Message.data[index];/*DisLoadWarnShow();*/break;
								default: 	break;
							}
							break;
						case CAN0_MODE_VERSION_TYPE:
							switch(sub_func_type) 
							{
							case 0:  for(index=0;index<8;index++) USR_s_OV.PowerContorolVersion[index] = RX_Message.data[index]; break;
							default: 	break;
							}
							break;
						case  CAN0_SENSOR_TYPE:
							switch(sub_func_type) 
							{
								case 0:  InsideData[120]=  RX_Message.data[0];   InsideData[120]<<=8;  InsideData[120] |=RX_Message.data[1];
								InsideData[121]=  RX_Message.data[2];   InsideData[121]<<=8;  InsideData[121] |=RX_Message.data[3];
								InsideData[122]=  RX_Message.data[4];   InsideData[122]<<=8;  InsideData[122] |=RX_Message.data[5];
								break;   
								case 1:  InsideData[123]=  RX_Message.data[0];   InsideData[123]<<=8;  InsideData[123] |=RX_Message.data[1];
								InsideData[124]=  RX_Message.data[2];   InsideData[124]<<=8;  InsideData[124] |=RX_Message.data[3];
								InsideData[125]=  RX_Message.data[4];   InsideData[125]<<=8;  InsideData[125] |=RX_Message.data[5];
								InsideData[126]=  RX_Message.data[6];   InsideData[126]<<=8;  InsideData[126] |=RX_Message.data[7];   
								break;
								case 2:  InsideData[127]=  RX_Message.data[0];   InsideData[127]<<=8;  InsideData[127] |=RX_Message.data[1];
								InsideData[128]=  RX_Message.data[2];   InsideData[128]<<=8;  InsideData[128] |=RX_Message.data[3];
								InsideData[129]=  RX_Message.data[4];   InsideData[129]<<=8;  InsideData[129] |=RX_Message.data[5];
								InsideData[130]=  RX_Message.data[6];   InsideData[130]<<=8;  InsideData[130] |=RX_Message.data[7];  
								break;
								case 3:  InsideData[131]=  RX_Message.data[0];   InsideData[131]<<=8;  InsideData[131] |=RX_Message.data[1];
								InsideData[132]=  RX_Message.data[2];   InsideData[132]<<=8;  InsideData[132] |=RX_Message.data[3];
								InsideData[133]=  RX_Message.data[4];   InsideData[133]<<=8;  InsideData[133] |=RX_Message.data[5];
								InsideData[134]=  RX_Message.data[6];   InsideData[134]<<=8;  InsideData[134] |=RX_Message.data[7];  
								break;
								case 4:  InsideData[135]=  RX_Message.data[0];   InsideData[135]<<=8;  InsideData[135] |=RX_Message.data[1];
								InsideData[136]=  RX_Message.data[2];   InsideData[136]<<=8;  InsideData[136] |=RX_Message.data[3];
								InsideData[137]=  RX_Message.data[4];   InsideData[137]<<=8;  InsideData[137] |=RX_Message.data[5];
								InsideData[138]=  RX_Message.data[6];   InsideData[138]<<=8;  InsideData[138] |=RX_Message.data[7];  
								break; 
								case 5:  InsideData[139]=  RX_Message.data[0];   InsideData[139]<<=8;  InsideData[139] |=RX_Message.data[1];
								InsideData[140]=  RX_Message.data[2];   InsideData[140]<<=8;  InsideData[140] |=RX_Message.data[3];
								InsideData[141]=  RX_Message.data[4];   InsideData[141]<<=8;  InsideData[141] |=RX_Message.data[5];
								InsideData[142]=  RX_Message.data[6];   InsideData[142]<<=8;  InsideData[142] |=RX_Message.data[7];  
								break;  
								case 7:    for(index=0;index<8;index++)Power_Current_Valve[index] = RX_Message.data[index]; break;
								case 8:    for(index=0;index<8;index++)Power_Current_Valve[index+8] = RX_Message.data[index]; break;
								default: 	break;
							}
							break;
						default:
						break;
					} 
					break;  	    
				}
			case CAN0_PC_SOURCE_ID:
				{
					switch(func_type) 
					{
						case  CAN0_BT_TYPE:  
						{
							listen_mode_flag = 1;      
							switch(sub_func_type) 
							{
								case 1: break;//if(dest_id==CAN0_METER_SOURCE_ID) for(;;); break;
								case 2: 
									if((RX_Message.data[0]==0x55)&&(RX_Message.data[1]==0xaa)&&(RX_Message.data[2]==0x5a)&&(RX_Message.data[3]==0xa5)&&
											(RX_Message.data[4]==0x01)&&(RX_Message.data[5]==0xfe)) ;
										for(;;);
									break;   
								case 0x0a:
									TX_Message.identifier = 0x00201a02;
									TX_Message.byte_count = 8;
									get_cofig_addr = (UINT32)(&PcConfigAddress);
									TX_Message.data[0]  =  (UINT8)(get_cofig_addr>>24);
									TX_Message.data[1]  =  (UINT8)(get_cofig_addr>>16);
									TX_Message.data[2]  =  (UINT8)(get_cofig_addr>>8);
									TX_Message.data[3]  =  (UINT8)(get_cofig_addr);
									TX_Message.data[4]  =  (UINT8)(sizeof(PcConfigAddress)>>8);
									TX_Message.data[5]  =  (UINT8)(sizeof(PcConfigAddress));
									TX_Message.data[6]  =  0;
									TX_Message.data[7]  =  0;
									listen_mode_flag = 0;
									CAN0_send(&TX_Message);  break;

								case 7: 	listen_mode_flag = 0;  break;   // CAN0_Init(LISTEN_MODE);
								default: 	break;
							} 
							break;  
						}
						case CAN0_SET_SYS_TYPE:
							switch(sub_func_type) 
							{
								case 0: 
									SetMileageFlag = 1;
									PcFromMileage= RX_Message.data[0] ;
									PcFromMileage<<= 8;
									PcFromMileage |= RX_Message.data[1];  
									PcFromMileage<<= 8;
									PcFromMileage |= RX_Message.data[2] ;
									PcFromMileage<<= 8;  	                             
									PcFromMileage |= RX_Message.data[3] ;
									TX_Message.identifier = 0x20A002;
									TX_Message.byte_count = 8;
									for(index=0;index<8;index++)
										TX_Message.data[index] = RX_Message.data[index];
									CAN0_send(&TX_Message);
									break;//if(dest_id==CAN0_METER_SOURCE_ID) for(;;); break;
								case 1: 
									TX_Message.identifier = 0x20A102;
									TX_Message.byte_count = 8;
									TX_Message.data[0]  =  (UINT8)(InsideData[18]>>8);
									TX_Message.data[1]  =  (UINT8)(InsideData[18]&0XFF);
									TX_Message.data[2]  =  (UINT8)(InsideData[19]);
									TX_Message.data[3]  =  0;
									TX_Message.data[4]  =  0;
									TX_Message.data[5]  =  0;
									TX_Message.data[6]  =  0;
									TX_Message.data[7]  =  0;
									CAN0_send(&TX_Message);
									break; 
								default: 	break;
							} 
							break;  
						break;
					}   
					break;
				}
			default:	break;
		}
	}


	//CANClearFlag(0,1);
	//CAN0RXIDR0 = 0x00;//ClearRXFifo;
	//CAN0RXIDR1 = 0x00;//ClearRXFifo;
	//CAN0RFLG |= 0x01; //clear flag of full fifo,show that the next data can enter
}



void  CaptureTimeIsr(St_CaptureTime *stCaptureTime,uint16 capTimeval,uint8 isrSrc)
{
  uint32 captureTemp;
  if(isrSrc==1)   //定时器异常中断
  {
    stCaptureTime->highTime++;
  }
  else
  {
   stCaptureTime->captureCount++; 
    if(stCaptureTime->captureCount==1)
    {
      stCaptureTime->preCaptureTime  =  stCaptureTime->highTime;
      stCaptureTime->preCaptureTime <<=16;
      stCaptureTime->preCaptureTime |= capTimeval;  //第一次捕获   不每次捕获处理	   
    }
    else
    {
      stCaptureTime->captureCount = 0;
      stCaptureTime->nextCaptureTime  =  stCaptureTime->highTime;
      stCaptureTime->nextCaptureTime <<=16;
      stCaptureTime->nextCaptureTime |= capTimeval;  //第二次捕获     
      if(stCaptureTime->nextCaptureTime> stCaptureTime->preCaptureTime)
	  captureTemp=stCaptureTime->nextCaptureTime - stCaptureTime->preCaptureTime;
      else
	  captureTemp=0xFFFFFFFF-stCaptureTime->preCaptureTime+stCaptureTime->nextCaptureTime;
          stCaptureTime->captureToFre = (80000000/128)/captureTemp;
          stCaptureTime->upflag = 1;
        //Freq1=(g_bus_clock/LPLD_FTM_GetClkDiv(FTM2))/cnt; 
    }
  }
}


void  CaptureSpeed(St_CaptureTime *stCaptureTime)
{	
UINT16 speed;
//两秒内没有捕获到脉冲  要清零
  if(stCaptureTime->upflag)
  {
       stCaptureTime->upflag = 0;//车速有变化
       if(stCaptureTime->captureToFre>2000)return; // 频率大于2k过滤
       speed = calcSpeed(stCaptureTime->captureToFre);   //hz
       CarSpeedFromDashboard = filter(speed);   
       count_speed = 0;
  }
  else
  {
    
  }
		

}





void FTM3_IRQHandler(void)
{
 uint32_t cnt;
  
  if((FTM3->SC & FTM_SC_TOF_MASK))//溢出中断
  {
    CaptureTimeIsr(&CapturePwm0,0,1) ;
    CaptureTimeIsr(&CapturePwm1,0,1) ;    
    FTM3->SC = FTM3->SC & ~FTM_SC_TOF_MASK; 
  }
   // rpm
  //判断是否为FTM1的Ch0通道产生捕获中断
  if((FTM3->C5SC & FTM_C5SC_CHF_MASK))
  {
    //获取FTM1的Ch0通道计数值
    cnt=FTM3->C5V;   
    //CaptureTimeIsr(&CapturePwmSpeed,cnt,0);
    CaptureTimeIsr(&CapturePwm0,cnt,0);             //JC 5
    
    //根据总线频率、分频系数、计数值计算脉冲频率
    //脉冲频率=(总线频率/输入捕获分频)/计数值
    //Freq1=(g_bus_clock/LPLD_FTM_GetClkDiv(FTM2))/cnt; 
    
    //清空FTM1 COUNTER
    //LPLD_FTM_ClearCounter(FTM2);
    //清除输入中断标志
      FTM3->C5SC = FTM3->C5SC & ~FTM_C5SC_CHF_MASK;
  }
  
  // speed
   //判断是否为FTM1的Ch0通道产生捕获中断
   //判断是否为FTM1的Ch0通道产生捕获中断
  if((FTM3->C6SC & FTM_C6SC_CHF_MASK))
  {
    //获取FTM1的Ch0通道计数值
    cnt=FTM3->C6V;   
    //CaptureTimeIsr(&CapturePwmSpeed,cnt,0);
    CaptureTimeIsr(&CapturePwm1,cnt,0);                         //JC3 
    
    //根据总线频率、分频系数、计数值计算脉冲频率
    //脉冲频率=(总线频率/输入捕获分频)/计数值
    //Freq1=(g_bus_clock/LPLD_FTM_GetClkDiv(FTM2))/cnt; 
    
    //清空FTM1 COUNTER
    //LPLD_FTM_ClearCounter(FTM2);
    //清除输入中断标志
      FTM3->C6SC = FTM3->C6SC & ~FTM_C6SC_CHF_MASK;
  }
  

}

extern unsigned int PWM_OUT_frq;
void LPTMR_Isr(void)
{

      LPTMR0_CSR|=LPTMR_CSR_TCF_MASK;
	PWMOUT1_Toggle;
	lptmr_set_hz(PWM_OUT_frq);
}
