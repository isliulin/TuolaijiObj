#define		J1939_C
#include	"config.h"
#pragma CODE_SEG USER_ROM
#pragma MESSAGE DISABLE C12056///屏蔽无必要的编译提示 
/****************************************************************************************

*****************************************************************************************/
void	J1939_SignalCheck(void)
{
	
	if(J1939_oilpress_FLAG==0x03)///机油压力从J1939获得
	{
		jycgerrflag=0;
		PannalSensorData[OIL_PRESS_POSITION_IN]=0;
		oil_j1939_flag=1;
	}
	else
		oil_j1939_flag=0;
		
	if(J1939_TEMPTURE_FLAG==0x02)///水温从J1939获得
		WaterTemp_J1939_Flag=1;
	else
		WaterTemp_J1939_Flag=0;

}
/****************************************************************************************

*****************************************************************************************/
static	void Dll_pdu(void)                  
{ 
  uchar    m_DA,m_SA,m_PF,i;
  ulong    m_ID;
  uint     m_PGN;

  m_DA = 0;
  m_SA = 0;
  m_PF = 0;
  m_ID = 0;
  m_PGN = 0;
    
  i = 0;    
        
  /******call transport layer funtion,message change to J1939 format*********/    
  m_ID = rev_can_packet.identifier;      
  m_SA = (uchar)m_ID;
  m_DA = (uchar)(m_ID>>8);
  m_PGN = (uint)(m_ID>>8);
  m_PF = (uchar)(m_ID>>16);

	J1939_Sa=m_SA;
						
  if(1)// (m_PF < 240)    ///PDU1 format
  {/*
     if(m_DA==GLOBADDR)
     {
       J1939MSG.PGN = m_PGN & 0xFF00;
       J1939MSG.byte_count = rev_can_packet.byte_count;
       J1939MSG.dest_addr = GLOBADDR;
       J1939MSG.source_addr = m_SA; 
       for(i=0;i<8;i++)
       {
          J1939MSG.data[i] = rev_can_packet.data[i]; ///save data to special buffer
       }
     }
     else if(m_DA==NODEADDR)        ///special address and whole address     
     {
       J1939MSG.PGN = m_PGN & 0xFF00;
       J1939MSG.byte_count = rev_can_packet.byte_count;
       J1939MSG.dest_addr = m_DA;
       J1939MSG.source_addr = m_SA; 
       for(i=0;i<8;i++)
       {
         J1939MSG.data[i] = rev_can_packet.data[i];
       }
     }
	}
  else             ///PDU2 format
  {*/
     J1939MSG.PGN = m_PGN;
     J1939MSG.byte_count = rev_can_packet.byte_count;
     J1939MSG.dest_addr = GLOBADDR;
     J1939MSG.source_addr = m_SA;
     for(i=0;i<8;i++)
     {
       J1939MSG.data[i] = rev_can_packet.data[i];
     }          
   }
	 rev_can_packet.identifier=0x00;
}
/****************************************************************************************
***Application Interface Functions


*****************************************************************************************/
static void Rev_J1939msg(J1939_PDU_T msg_ptr)    
{
	uchar i;
  uint  tmp1; ///整形变量的缓冲
	uint  tmp2;///字符形变量的缓冲
	uchar tmp3[3],ReCarSpeedTemp;
	ulong oil_wear_buf1;///总油耗提取的缓冲
  uchar j,k,tmp4=0;  
	uint  engSpeedTemp;
  switch (msg_ptr.PGN)
  { 
    case 61444:	        ///engine rotation 		                
		  if(J1939_TACHO_FLAG==0x01)
		  {
		    tmp1 = msg_ptr.data[4];
		    tmp1 = (tmp1<<8) + msg_ptr.data[3]; 
		    tmp1 = tmp1 /8;
				if((resetback0==0x00)&&(OnKeyBit==KeyOn)) 
				{
					engSpeed = tmp1;
					EngRotation();	
				}
				Rotation_Data_Flag=1;
		  }
	  	break;	 
	  case 65262:			///engine cooling water temperature
		   if(J1939_TEMPTURE_FLAG==0x02)
		   {
					tmp2 = msg_ptr.data[0];
					if(tmp2<80) 
						tmp2=80;  
					tmp2 = tmp2-40;	///实际温度
					if((resetback0==0x00)&&(OnKeyBit==KeyOn)) 
						watTemp = (uchar)tmp2;
		   }
	   	 break;
	   case 65263:		///fual press *4kpa
	   	 if(J1939_oilpress_FLAG==0x03)
		   {
		   		tmp2 = msg_ptr.data[3];
	    		tmp1 = tmp2*4;
	    		tmp1 = tmp1/10;
					oilPres = (unsigned char)tmp1;
		   	}
	    	break;
	  	case 65257:       ///tatol oil consume
	  	  if(J1939_FualTotal_FLAG==0x04)
	  	  {
					oil_wear_buf1= 0x00;
          for(i=0;i<3;i++)
          {
						oil_wear_buf1 = oil_wear_buf1+msg_ptr.data[7-i];
			    	oil_wear_buf1 = oil_wear_buf1<<8;
          }
					oil_wear_buf1 = oil_wear_buf1+msg_ptr.data[4];
					total_fual_used= oil_wear_buf1;
					LastTotalFuelConsume=total_fual_used*100;
					LastTotalFuelConsume=LastTotalFuelConsume/2;
					LastTotalFuelConsume_Small=LastTotalFuelConsume;
					TotalFuelConsumeCompleteFlag=1;
	  	  }
	  		break;

	    case 65226:		   ///DM1
				if(J1939_Sa==SCR_SA)
		 		{
					SCR_Diagnose_Lamp.Data=msg_ptr.data[0];	
					SCR_Diagnose_Flag=SCR_DIAGNOSE_NO_PACKAGE;
		 		}
				else if(J1939_Sa==ENGINE_SA)
				{
					SCRSystemFlag=1;	
					Engine_Diagnose_Lamp.Data=msg_ptr.data[0];
					Engine_Diagnose_Flag=ENGINE_DIAGNOSE_NO_PACKAGE;
				}
	    	break;	
			case 0xECFF:
				if(J1939_Sa==ENGINE_SA)
				{
					EngineDiagnose_PGN=msg_ptr.data[7];
					EngineDiagnose_PGN<<=8;
					EngineDiagnose_PGN+=msg_ptr.data[6];
					EngineDiagnose_PGN<<=8;
					EngineDiagnose_PGN+=msg_ptr.data[5];
					if(EngineDiagnose_PGN==DM1_PGN)
					{
						Engine_Diagnose_Flag=ENGINE_DIAGNOSE_PACKAGE;						
					}
				}
				else if(J1939_Sa==SCR_SA)
				{
					SCRSystemFlag=1;	
					SCRDiagnose_PGN=msg_ptr.data[7];
					SCRDiagnose_PGN<<=8;
					SCRDiagnose_PGN+=msg_ptr.data[6];
					SCRDiagnose_PGN<<=8;
					SCRDiagnose_PGN+=msg_ptr.data[5];
					if(SCRDiagnose_PGN==DM1_PGN)
					{
						SCR_Diagnose_Flag=SCR_DIAGNOSE_PACKAGE;						
					}					
				}
				break;
			case 0xEBFF:
				if((J1939_Sa==ENGINE_SA)&&(Engine_Diagnose_Flag==ENGINE_DIAGNOSE_PACKAGE)&&(EngineDiagnose_PGN==DM1_PGN))
				{
					tmp1=msg_ptr.data[0];
					if(tmp1==1)///indicate first package
						Engine_Diagnose_Lamp.Data=msg_ptr.data[1];			
				}
				else if((J1939_Sa==SCR_SA)&&(SCR_Diagnose_Flag==SCR_DIAGNOSE_PACKAGE)&&(SCRDiagnose_PGN==DM1_PGN))
				{
					SCRSystemFlag=1;	
					tmp1=msg_ptr.data[0];
					if(tmp1==1)///indicate first package
						SCR_Diagnose_Lamp.Data=msg_ptr.data[1];	
				}
				break;		
				
			case	65268://TPMS
				TPMS_CAN_ModuleFlag=1;
				TPMS_CAN_LearnEndFlag=0;	//	学习完成标志
				TPMS_CAN_LearnEnd_CT=0;		//  发送胎压学习结束的次数
				GetTPMSTime=0;						//  胎压模块通信时间
				TPMS_Learn_Time=0;				// 学习时间超时清零
				k=msg_ptr.data[0];
				if(k)
					k--;
				tmp2=msg_ptr.data[1];
				tmp2=tmp2<<8;
				tmp2=tmp2+msg_ptr.data[2]; 
				TyreGetData[k].TyrePressure=tmp2;		 						// 轮胎气压
				TyreGetData[k].TyreTemperature=msg_ptr.data[3]; // 轮胎温度
				TyreGetData[k].TyreVoltage=msg_ptr.data[4] ;		// 传感器电源
				TyreGetData[k].TyreStateFlag.Data=msg_ptr.data[5]; //传感器状态
				/////////////////////////////////////////////////
				//保存胎压状态
				TyreState_Flag[k]=msg_ptr.data[5]&0x61;
				if(TyreGetData[k].TyrePressure>TPMS_GAOYA)
					TyreState_Flag[k]+=2;

				if(TyreGetData[k].TyrePressure<TPMS_DIYA)
				 TyreState_Flag[k]+=4;

				if((TyreGetData[k].TyreTemperature>TPMS_GAOWEN)&&(TyreGetData[k].TyreStateFlag.b.HuWenDuFlag==0))//高温
					TyreState_Flag[k]+=8;
				TPMS_Err_Flag=0;
				for(j=0;j<TYRE_COUNT;j++)
				{
					if(TyreState_Flag[j]!=0)
						TPMS_Err_Flag=1;
				}
				break;				
	  	case 65266:			///oil consume rate
				if(J1939_Sa==TPMS_SA)
				{
					TPMS_CAN_ModuleFlag=1;
					GetTPMSTime=0;			 	// 胎压模块通信时间					
					if((menu_id == ThirdMenuNum)&&(SubMenu_Cursor == TyrePressMenuNum)&&(TyrePressMenuItem==3))
				 	{
						j=msg_ptr.data[6];
						if(j)
						j--;
						tmp3[0]=msg_ptr.data[3];
						tmp3[1]=msg_ptr.data[4];
						tmp3[2]=msg_ptr.data[5];
						if((tmp3[0]==0xff)&&(tmp3[1]==0xff)&&(tmp3[2]==0xff))
						{
							TyreState[j]=WAIT_SUCCEED;// 				 
						}
						else
						{
							TyreState[j]=TPMS_SUCCEED;//
							TPMS_ID[j][0]=tmp3[0];//
							TPMS_ID[j][1]=tmp3[1];//
							TPMS_ID[j][2]=tmp3[2];//
							TPMSPosition=j;
							TPMS_CAN_LearnEndFlag=1;	//学习完成
						}
					}
					else
					{
							j=msg_ptr.data[6];
							if(j)
							j--;
							TPMSPosition=j;
							TPMS_CAN_LearnEndFlag=1; // 
					}						 
				}
				else
				{			
		  	  if(J1939_OilComsumeRate==0x06)
			  	{
			  	  fual_value = msg_ptr.data[1];
				  	fual_value = fual_value<<8;
				  	fual_value = fual_value + msg_ptr.data[0];
			  	}	
				}
	  		break;
		case 0xfe56:			///catalyst tank level
				tmp1=msg_ptr.data[0];
				CatalystTankLevel=tmp1*4;///enlarge 10 time
				if(CatalystTankLevel<10*10)///小于10%
				{
					CatalystTankLevelFlag2=1;
					CatalystTankLevelFlag=0;
				}
				else if(CatalystTankLevel<20*10)///小于20%
				{
					CatalystTankLevelFlag=1;
					CatalystTankLevelFlag2=0;
				}
				else	
				{
					CatalystTankLevelFlag=0;
					CatalystTankLevelFlag2 =0;
				}
				break;
		case 	0x26d0:
			if(J1939_Sa==BMS_SA)
			{
				HighestChargeVolt=msg_ptr.data[1];
				HighestChargeVolt<<=8;
				HighestChargeVolt+=msg_ptr.data[0];
				HighestChargeVolt&=0x3fff; 
				if(HighestChargeVolt>10000)
					HighestChargeVolt=10000;
				
				BatteryNominalVolt=msg_ptr.data[3];
				BatteryNominalVolt<<=8;
				BatteryNominalVolt+=msg_ptr.data[2];
				BatteryNominalVolt&=0x03ff; 	
				if(BatteryNominalVolt>1000)
					BatteryNominalVolt=1000;
				
				HighestWorkTemp=msg_ptr.data[4];
				LowestWorkTemp=msg_ptr.data[5];
				BatteryPitchNum=msg_ptr.data[6];
				LECUnumber=msg_ptr.data[7];
				LECUnumber&=0x1f;
			}
			break;
		case 	0x27d0:	
			if(J1939_Sa==BMS_SA)
			{
				SingleBatHighestVolt=msg_ptr.data[1];
				SingleBatHighestVolt<<=8;
				SingleBatHighestVolt+=msg_ptr.data[0];
				SingleBatHighestVolt&=0x3fff; 
				if(SingleBatHighestVolt>5000)
					SingleBatHighestVolt=5000;
				
				SingleBatLowestVolt=msg_ptr.data[3];
				SingleBatLowestVolt<<=8;
				SingleBatLowestVolt+=msg_ptr.data[2];
				SingleBatLowestVolt&=0x3fff; 	
				if(SingleBatLowestVolt>5000)
					SingleBatLowestVolt=5000;
				
				BatteryNominalCurr=msg_ptr.data[5];
				BatteryNominalCurr<<=8;
				BatteryNominalCurr+=msg_ptr.data[4];		
				BatteryNominalCurr&=0x03ff; 	
				if(BatteryNominalCurr>5000)
					BatteryNominalCurr=5000;				
				BatterySOH=msg_ptr.data[6];
			}			
			break;
		case 	0x23d0:	
			if(J1939_Sa==BMS_SA)
			{
				BatteryPresentVolt=msg_ptr.data[1];
				BatteryPresentVolt<<=8;
				BatteryPresentVolt+=msg_ptr.data[0];
				BatteryPresentVolt&=0x3fff; 
				if(BatteryPresentVolt>10230)
					BatteryPresentVolt=10230;
				
				BatteryPresentCurr=msg_ptr.data[3];
				BatteryPresentCurr<<=8;
				BatteryPresentCurr+=msg_ptr.data[2];
				BatteryPresentCurr&=0x3fff; 	
				if(BatteryPresentCurr>15230)
					BatteryPresentCurr=15230;
				
				BatteryPresentSOC=msg_ptr.data[5];
				BatteryPresentSOC<<=8;
				BatteryPresentSOC+=msg_ptr.data[4];
				BatteryPresentSOC&=0x01ff; 	
				if(BatteryPresentSOC>200)
					BatteryPresentSOC=200;
				BatteryPresentHighestTemp=msg_ptr.data[6];
				BatteryPresentLowestTemp=msg_ptr.data[7];
			}			
			break;	
		case 	0x24d0:	
			if(J1939_Sa==BMS_SA)
			{
				SingleBatPresentHighestVolt=msg_ptr.data[1];
				SingleBatPresentHighestVolt<<=8;
				SingleBatPresentHighestVolt+=msg_ptr.data[0];
				SingleBatPresentHighestVolt&=0x3fff; 
				if(SingleBatPresentHighestVolt>9999)
					SingleBatPresentHighestVolt=9999;
				
				SingleBatPresentLowestVolt=msg_ptr.data[3];
				SingleBatPresentLowestVolt<<=8;
				SingleBatPresentLowestVolt+=msg_ptr.data[2];
				SingleBatPresentLowestVolt&=0x3fff;				
				if(SingleBatPresentHighestVolt>9999)
					SingleBatPresentHighestVolt=9999;
				
				PositiveBusInsulationRES=msg_ptr.data[5];
				PositiveBusInsulationRES<<=8;
				PositiveBusInsulationRES+=msg_ptr.data[4];
				PositiveBusInsulationRES&=0x01ff; 	
				
				MinusBusInsulationRES=msg_ptr.data[7];
				MinusBusInsulationRES<<=8;
				MinusBusInsulationRES+=msg_ptr.data[6];
				MinusBusInsulationRES&=0x01ff; 
			}			
			break;				
		case 	0x1fd0:	
			if(J1939_Sa==BMS_SA)
			{
				BatteryBMSCheckSum=msg_ptr.data[0];

				BatteryBMSCount=msg_ptr.data[1];
				BatteryBMSCount&=0x0f; 

				BatteryBMSStatus1.Data=msg_ptr.data[2];

				BatteryBMSStatus2.Data=msg_ptr.data[4];	

				BatteryBMSStatus3.Data=msg_ptr.data[5];
					
			}			
			break;	
		case 	0xff0f:	
			if(J1939_Sa==DMC_SA)
			{
				BMSContrllerFaultNumber=msg_ptr.data[0];

				BMSContrllerWarnNumber=msg_ptr.data[1];				

				BMSContrllerCarspeed=msg_ptr.data[3];	
				BMSContrllerCarspeed<<=8;
				BMSContrllerCarspeed+=msg_ptr.data[2];

				ReCarSpeedTemp=(uchar)(BMSContrllerCarspeed/10);
				if(OnKeyBit==KeyOn)
				{
					carspeed = ReCarSpeedTemp; ///vehicle speed 	
				
					if (ReCarSpeedTemp > SPEEDHIGHNUM)
						carspeed = SPEEDHIGHNUM;
					else if(ReCarSpeedTemp<=SPEEDRESIVENUM)
						carspeed=ReCarSpeedTemp;
					else
					{
						if((ReCarSpeedTemp>=(carspeed+SPEEDRESIVENUM))||(ReCarSpeedTemp<=(carspeed-SPEEDRESIVENUM)))///10.04.25 delete
							carspeed=ReCarSpeedTemp;
					}
					if (resetback0 == 0x00)
						CARSPEED();
					Speed_Data_Flag = 1;
				}

				BMSContrllerGears=msg_ptr.data[4];
				
				DriverSystemFaultNumber=msg_ptr.data[5];
				BMSContrllerEngineSpeed=msg_ptr.data[6];	
				engSpeedTemp =BMSContrllerEngineSpeed;
				engSpeedTemp =engSpeedTemp*50;	
				engSpeedTemp =engSpeedTemp/4;
				if(resetback0==0x00) 
				{			
					if(OnKeyBit==KeyOff)
					{
						engSpeed=0x00;
						EngRotation();	
					}	
					else
					{					
						engSpeed = engSpeedTemp;
						if(engSpeed>6000)
							engSpeed=6000;
						EngRotation();	
					}		
				}				
			}		
		case 	0xff0e:	
			if(J1939_Sa==DMC_SA)
			{
				InverterTemperature=msg_ptr.data[0];

				Motor1InverterRadiatorTemp=msg_ptr.data[1];				

				Motor1WindingResTemp=msg_ptr.data[2];	
	
				Motor2InverterRadiatorTemp=msg_ptr.data[3];

				Motor2WindingResTemp=msg_ptr.data[4];
				
				ConnecterStatus=msg_ptr.data[5];
				OperationStatus=msg_ptr.data[6];					
			}					
			break;			
      default:
      	break; 
    }
}  
/****************************************************************************************

*****************************************************************************************/              
static void Trans_CANpkt(void)    
{
    char cansel=0;
 
    cansel = CAN1TFLG;      ///select one available transmit buffer
    CAN1TBSEL = cansel;     ///send buffer 0 is unselect,1 is select
    cansel = CAN1TBSEL;
     
    CAN1TXIDR0 = (send_can_packet.identifier>>21) & 0xFF;   ///PGN change to 32bit   
    CAN1TXIDR1 = (send_can_packet.identifier>>18) & 0x07; 
    CAN1TXIDR1 = CAN1TXIDR1 << 5;
    CAN1TXIDR1 = CAN1TXIDR1 | ((send_can_packet.identifier>>15) & 0x07);
    CAN1TXIDR1_SRR = 1;
    CAN1TXIDR1_IDE = 1;                            ///extend frame IDE=1->29bit,IDE=0->11bit		
    CAN1TXIDR2 = (send_can_packet.identifier>>7) & 0xFF; 
    CAN1TXIDR3 = (send_can_packet.identifier<<1) & 0xFF; 
    CAN1TXIDR3_RTR = 0;                            ///data frame  
    CAN1TXDSR0 = send_can_packet.data[0];
    CAN1TXDSR1 = send_can_packet.data[1];
    CAN1TXDSR2 = send_can_packet.data[2];
    CAN1TXDSR3 = send_can_packet.data[3];
    CAN1TXDSR4 = send_can_packet.data[4];
    CAN1TXDSR5 = send_can_packet.data[5];
    CAN1TXDSR6 = send_can_packet.data[6];
    CAN1TXDSR7 = send_can_packet.data[7];   
    CAN1TXDLR  = send_can_packet.byte_count;        ///byte count
         
    CAN1TXTBPR = 0;		                            	///send buffer priority

    CAN1TFLG = cansel;  	///clearing TxE to lauch transmission 
}
/****************************************************************************************

*****************************************************************************************/ 
void fual_value_require(void)
{
	send_can_packet.identifier = 0x18EA0021;
	send_can_packet.byte_count = 0X03;
	send_can_packet.data[0] = 0XE9;
	send_can_packet.data[1] = 0XFE;
	send_can_packet.data[2] = 0X00;
	Trans_CANpkt();
}
/****************************************************************************************

*****************************************************************************************/ 
void	J1939_Speed_Send(void)
{
	unsigned int	Temp_Carspeed;
	unsigned int	Temp_EngineSpeed;
	Temp_Carspeed=carspeed*256;///j1939 car speed Resolution is 256;
	Temp_EngineSpeed=engSpeed*8;
	send_can_packet.identifier = 0x0CFE6CEE;
	send_can_packet.byte_count = 8;
	send_can_packet.data[0] = 0xff;
	send_can_packet.data[1] = 0xff;
	send_can_packet.data[2] = 0xff;
	send_can_packet.data[3] = 0xff;
	send_can_packet.data[4] = (unsigned char)Temp_EngineSpeed;
	send_can_packet.data[5] = (unsigned char)(Temp_EngineSpeed>>8);
	send_can_packet.data[6] = (unsigned char)Temp_Carspeed;
	send_can_packet.data[7] = (unsigned char)(Temp_Carspeed>>8);
	Trans_CANpkt();
}
/****************************************************************************************

*****************************************************************************************/ 
void	J1939_CCVS_Send(void)
{
	unsigned int	Temp_Carspeed;
	Temp_Carspeed=carspeed*256;///j1939 car speed Resolution is 256;
	
	send_can_packet.identifier = 0x18FEF100;
	send_can_packet.byte_count = 8;
	send_can_packet.data[0] = 0xff;
	send_can_packet.data[1] = (unsigned char)Temp_Carspeed;
	send_can_packet.data[2] = (unsigned char)(Temp_Carspeed>>8);
	send_can_packet.data[3] = 0xff;
	send_can_packet.data[4] = 0xff;
	send_can_packet.data[5] = 0xff;
	send_can_packet.data[6] = (unsigned char)Temp_Carspeed;
	send_can_packet.data[7] = (unsigned char)(Temp_Carspeed>>8);
	Trans_CANpkt();
}

//TPMS发送进入学习命令

void  TXWaitRelay(void)
{
	TX1939_TPMS_Flag=1;
	send_can_packet.identifier = 0x18fef317;
	send_can_packet.byte_count = 8;
	send_can_packet.data[0] = 0xff;
	send_can_packet.data[1] = 0x88;
	send_can_packet.data[2] = 0x88;
	send_can_packet.data[3] = 0x0;
	send_can_packet.data[4] = 0x0;
	send_can_packet.data[5] = 0x0;
	send_can_packet.data[6] = TPMSPosition+1;
	send_can_packet.data[7] = 0x01;
	Trans_CANpkt();

}

void  TX_TPMS_LearnEnd(void)
{ 
	TX1939_TPMS_Flag=1;
	send_can_packet.identifier = 0x18fef317;
	send_can_packet.byte_count = 8;
	send_can_packet.data[0] = 0xff;
	send_can_packet.data[1] = 0x88;
	send_can_packet.data[2] = 0x88;
	send_can_packet.data[3] = 0xff;
	send_can_packet.data[4] = 0xff;
	send_can_packet.data[5] = 0xff;
	send_can_packet.data[6] = TPMSPosition+1;
	send_can_packet.data[7] = 0x01;
	Trans_CANpkt();
}



/****************************************************************************************

*****************************************************************************************/ 
void SwitchLoad_BitSwitch(void)
{
	///if(ElectricBugleKeyBit == KeyOn) //喇叭
		///SendRecordSwitch[0].St.S0= RecordSwitchOn;
	///else
		SendRecordSwitch[0].St.S0 = RecordSwitchOff;
	if(RShieldSignalKeyBit == KeyOn) //倒车
		SendRecordSwitch[0].St.S1 = RecordSwitchOn;
	else
		SendRecordSwitch[0].St.S1 = RecordSwitchOff;		
	if(FrontFogKeyBit == KeyOn)   //前雾灯
		SendRecordSwitch[0].St.S2 = RecordSwitchOn;
	else
		SendRecordSwitch[0].St.S2 = RecordSwitchOff;	
	if(BackFogKeyBit == KeyOn) //后雾灯
		SendRecordSwitch[0].St.S3 = RecordSwitchOn;
	else
		SendRecordSwitch[0].St.S3 = RecordSwitchOff;	

	if(NearLampKeyBit == KeyOn) //近光
		SendRecordSwitch[1].St.S0 = RecordSwitchOn;
	else
		SendRecordSwitch[1].St.S0 = RecordSwitchOff;
	if(FarLampKeyBit == KeyOn) //远光
		SendRecordSwitch[1].St.S1 = RecordSwitchOn;
	else
		SendRecordSwitch[1].St.S1 = RecordSwitchOff;		
	if(LeftTurnLampKeyBit == KeyOn) //左转向
		SendRecordSwitch[1].St.S2 = RecordSwitchOn;
	else
		SendRecordSwitch[1].St.S2 = RecordSwitchOff;	
	if(RightTurnLampKeyBit == KeyOn) //右转向
		SendRecordSwitch[1].St.S3 = RecordSwitchOn;
	else
		SendRecordSwitch[1].St.S3 = RecordSwitchOff;		

	if(oilPres<FualLowNum)   ///机油压力报警
		SendRecordSwitch[2].St.S0 = RecordSwitchOn;
	else
		SendRecordSwitch[2].St.S0 = RecordSwitchOff;
	if(AirFiltrateWarnKeyBit == KeyOn) //空滤阻塞报警
		SendRecordSwitch[2].St.S1 = RecordSwitchOn;
	else
		SendRecordSwitch[2].St.S1 = RecordSwitchOff;		
	if(EngineLowerWaterKeyBit == KeyOn) //水位报警 
		SendRecordSwitch[2].St.S2 = RecordSwitchOn;
	else
		SendRecordSwitch[2].St.S2 = RecordSwitchOff;	
	if(WaterTemperatureFlag ==1)  //水温高报警
		SendRecordSwitch[2].St.S3 = RecordSwitchOn;
	else
		SendRecordSwitch[2].St.S3 = RecordSwitchOff;	

	if(FrontAirPressWarnKeyBit == KeyOn)  //气压I报警
		SendRecordSwitch[3].St.S0 = RecordSwitchOn;
	else
		SendRecordSwitch[3].St.S0 = RecordSwitchOff;
	if(BackAirPressWarnKeyBit == KeyOn) //气压II报警
		SendRecordSwitch[3].St.S1 = RecordSwitchOn;
	else
		SendRecordSwitch[3].St.S1 = RecordSwitchOff;		
	if(MiddleDoorOpenSignalKeyBit == KeyOn)  //中门开信号
		SendRecordSwitch[3].St.S2 = RecordSwitchOn;
	else
		SendRecordSwitch[3].St.S2 = RecordSwitchOff;	
	if(FrontDoorOpenSignalKeyBit == KeyOn)  //前门开信号
		SendRecordSwitch[3].St.S3 = RecordSwitchOn;
	else
		SendRecordSwitch[3].St.S3 = RecordSwitchOff;		

	if(LocationLampKeyBit == KeyOn)  
		SendRecordSwitch[4].St.S0 = RecordSwitchOn;
	else
		SendRecordSwitch[4].St.S0 = RecordSwitchOff;	
	if(FootStopKeyBit == KeyOn)  
		SendRecordSwitch[4].St.S1 = RecordSwitchOn;
	else
		SendRecordSwitch[4].St.S1 = RecordSwitchOff;	

		SendRecordSwitch[4].St.S2 = UndefinedRecordSwitch;	
		SendRecordSwitch[4].St.S3 = UndefinedRecordSwitch;		
}
/****************************************************************************************

*****************************************************************************************/ 
void BeiAn_Record_SwitchLoad_require(void)
{
 	SwitchLoad_BitSwitch();
	send_can_packet.identifier = 0x18FF0223;
	send_can_packet.byte_count = 0X05;
	send_can_packet.data[0] = SendRecordSwitch[0].Data;
	send_can_packet.data[1] = SendRecordSwitch[1].Data;
	send_can_packet.data[2] = SendRecordSwitch[2].Data;
	send_can_packet.data[3] = SendRecordSwitch[3].Data;
	send_can_packet.data[4] = SendRecordSwitch[4].Data;		
	Trans_CANpkt();
}
/****************************************************************************************

*****************************************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt void MSCAN1Rec(void)      ///vector number 42
{
  dword ID_PGN;
	uchar Index;
	
  rev_can_packet.identifier = CAN1RXIDR0;
  rev_can_packet.identifier <<= 3;
  rev_can_packet.identifier |= (CAN1RXIDR1>>5);
  rev_can_packet.identifier <<= 3;
  rev_can_packet.identifier |= (CAN1RXIDR1&0X07);
  rev_can_packet.identifier <<= 8;
  rev_can_packet.identifier |= CAN1RXIDR2;
  rev_can_packet.identifier <<= 7;
  rev_can_packet.identifier |= (CAN1RXIDR3>>1);

  CAN1RXIDR0=0;
  CAN1RXIDR1=0;  
  CAN1RXIDR2=0;
  CAN1RXIDR3=0;   
  rev_can_packet.byte_count = CAN1RXDLR & 0x0F;

  ID_PGN=rev_can_packet.identifier&0X00FFFF00;  
  ID_PGN = ID_PGN>>8;

	switch(ID_PGN)
	{
		///normal j1939 can data pgn
		case 0xf004:		///engine rotation 		

		case 0xfeee:		///engine cooling water temperature

		case 0xfeeF:		///fual press 

		case 65257:			///tatol oil consume
			EngineECU_CANModuleFlag=1;	///set the bit show that high CAN is ok
		case 65226:			///DM1

		case 65266:			///oil consume rate
		case 65268:
		case 0xECFF:

		case 0xEBFF:
		goto	_J1939Can_DataDeal;
//		case 0x26d0:
//		case 0x27d0:
//		case 0x23d0:
//		case 0x24d0:	
//		case 0x1FD0:
//			BMS_CAN_ModuleFlag=1;
//			goto	_J1939Can_DataDeal;
		case  0xff0f:
		case  0xff0e:		
			DMC_CAN_ModuleFlag=1;
			goto	_J1939Can_DataDeal;			
		case	0xfe56:			///catalyst tank level
		SCRSystemFlag=1;			
		_J1939Can_DataDeal:
      for (Index = 0; Index < rev_can_packet.byte_count; Index++)
    		rev_can_packet.data[Index] = *(&CAN1RDSR0 + Index);	

      Dll_pdu();
      Rev_J1939msg(J1939MSG);
			break;
		default:
			break;
	}
  CAN1RFLG = 0x81;   ///Clear RXF  and WUPIF
}
/***********************************************************************
funtion operation:if CAN1 bus communite error,the funtion will respond
***********************************************************************/
interrupt  void CAN1error_int(void)     ///vector number 41
{
  byte Status1 = CAN1RFLG;              ///Read the status register

  CAN1RFLG|=0x02;                      
  
  if(((Status1&CAN_STATUS_TX_MASK)>CAN_STATUS_TX1_MASK)||
  	((Status1&CAN_STATUS_RX_MASK)>CAN_STATUS_RX1_MASK)) ///Bus-Off state 
      CAN1RFLG |= 254;                   /// Reset error flags
}
#pragma CODE_SEG USER_ROM


