#define	_NO_MessageDeal_EXTERN
#include	"MessageDeal.h"

#define SPEED_SENSOR_MODULE_IN 	1

/******************************************************
Description:Send module state when need
Input:	None
Output:	None
Return:	None
******************************************************/
void	MessageDeal(void)
{
	if(KeyChangFlag==1)
	{		
		SendMessage(SendKey1);
		Time_SendKey=0; 
		KeyChangFlag=0;     	 	
	}
	else if(DriveSendSensorFlag==1)
	{
		SendMessage(SendSensor);
		Time_SendSensor=0;
		DriveSendSensorFlag=0;
	}	
	else if(LoadSTChangFlag==1)
	{
		/*if(LoadSTSendFlag==0)
		{
			LoadSTSendFlag=1;
			SendMessage(SendST1);
		}
		else
		{
			LoadSTSendFlag=0;
			SendMessage(SendST2);
			Time_SendSt=0;
			LoadSTChangFlag=0;
		}*/
		SendMessage(SendST1);
		Time_SendSt=0;
		LoadSTChangFlag=0;
	}
	else if(IdRequestFlag==1)
	{	
		if(IDRequestSendFlag==0)
		{
			SendMessage(SendId2);
			IDRequestSendFlag=1;
		}
		else 
		{
			SendMessage(SendId3);
			IDRequestSendFlag=0;
			IdRequestFlag=0;
		}
	}
	else if(ListenModeFlag==1)
	{
		MSCAN_Init(ListenCanMode);
		ListenModeFlag=0;
	}
	else if(StartBootloaderFlag==1)
	{
		MSCAN_Init(NormalCanMode);
		SendMessage(StartBootLoader);
		StartBootloaderFlag=0;
		for(;;);							///soft reset
	}
	/*else if(SendConfigDataFlag==1)
	{
		SendConfigDataNum++;
		if(SendConfigDataNum==1)
			;
		else if(SendConfigDataNum==2)
		{
			SendConfigDataFlag=0;
			SendConfigDataNum=0;
		}
	}*/
	else if(DistanceConfigACKFlag==1)
	{
		SendMessage(SendDistanceConfig);
		DistanceConfigACKFlag=0;
	}
	else if(RainScratchAckFlag==1)
	{
		SendMessage(SendRainScratchTime);
		RainScratchAckFlag=0;
	}
	
	#if		SPEED_SENSOR_MODULE_IN==0
		if(FrontDriveFlag==1)	
	#elif		SPEED_SENSOR_MODULE_IN==1
		if(BackDriveFlag==1)	
	#elif		SPEED_SENSOR_MODULE_IN==2
		if(TopDriveFlag==1)				
	#endif
		{
			if(SendSpeedFlag==1)
			{
				SendMessage(SendSpeed);
				Time_Send_Speed=0;
				SendSpeedFlag=0;	
			}
		}	
	/*
	#if		Rotation_INFORM_IN==1
		else if(SendRotationFlag==1)
		{
			SendMessage(SendRotattion);
			Time_Send_Rotation=0;
			SendRotationFlag=0;
		}
	#endif		*/	
}    	
/*********************************************************
* brief:    CAN reception interrupt service routine
* author:   karry
**********************************************************/
interrupt void  CANRx_ISR(void)
{
	unsigned char	Index;///缓冲变量
	unsigned int 	Temp;///缓冲变量
	///读取ID	
	rev_can_packet.identifier = CANRIDR0;
	rev_can_packet.identifier <<= 3;
	rev_can_packet.identifier |= (CANRIDR1>>5);
	rev_can_packet.identifier <<= 3;
	rev_can_packet.identifier |= (CANRIDR1&0X07);
	rev_can_packet.identifier <<= 8;
	rev_can_packet.identifier |= CANRIDR2;
	rev_can_packet.identifier <<= 7;
	rev_can_packet.identifier |= (CANRIDR3>>1);	
	///将ID的缓存清零
	CANRIDR0=0x00;
	CANRIDR1=0x00;  
	CANRIDR2=0x00;
	CANRIDR3=0x00;   
	///读取数据帧长度	
	rev_can_packet.byte_count = (CANRDLR & 0x0F);	
	///读取数据内容
	for (Index=0; Index < rev_can_packet.byte_count; Index++)	
    	rev_can_packet.data[Index] = *(&CANRDSR0 + Index); 

	ResponseData=GetFunction();
	ModuleIDData=GetModuleID();
	switch(ResponseData)
	{
		case	BootloaderWaitFrame:		
			if(((ModuleIDData&0X1FFFF0FF)==0X10FF0021)||(ModuleIDData==PannalModuleID)) 
			{			
				if(FrontDriveFlag==1)
				{
					if(ModuleIDData!=FrontDriveID)
						ListenModeFlag=1;
				}
				else if(BackDriveFlag==1)
				{
					if(ModuleIDData!=BackDriveID)
						ListenModeFlag=1;
				}
				else if(TopDriveFlag==1)
				{
					if(ModuleIDData!=TopDriveID)
						ListenModeFlag=1;
				}
			}
			break;
		case BootloaderStartFrame:	
			if(((ModuleIDData&0X1FFFF0FF)==0X10FF0021)||(ModuleIDData==PannalModuleID)) 
			{
				if((FrontDriveFlag==1)&&(ModuleIDData==FrontDriveID))
				{
					StartBootloaderFlag=1;
				}
				else if((BackDriveFlag==1)&&(ModuleIDData==BackDriveID))
				{
					StartBootloaderFlag=1;
				}
				else if((TopDriveFlag==1)&&(ModuleIDData==TopDriveID))
				{
					StartBootloaderFlag=1;
				}
			}
			break;
		case BootloaderResetFrame:	///不管是哪一个模块下载后本模块都需软复位
		case BootloaderStopFrame:///
			if(((ModuleIDData&0X1FFFF0FF)==0X10FF0021)||(ModuleIDData==PannalModuleID))
			{
				for(;;);
			}
			break;
		default:
			break;
	}
	if((rev_can_packet.identifier==FrontDriveID)
		||(rev_can_packet.identifier==BackDriveID)
		||(rev_can_packet.identifier==TopDriveID)
		||(rev_can_packet.identifier==PowerControlModuleID)
		)
	{
		switch(ResponseData)
		{
			case Key1StateFrame:
				if(FrontDriveFlag==1)
				{
					SleepStartFlags=0;	
					if(ModuleIDData==BackDriveID)
					{
						for(Index=0;Index<2;Index++)
							BackKeyData[Index].Data=rev_can_packet.data[Index+1];
					}
					else if(ModuleIDData==TopDriveID)
					{
						for(Index=0;Index<2;Index++)
							TopKeyData[Index].Data=rev_can_packet.data[Index+1];
					}
				}
				else if(BackDriveFlag==1)
				{
					SleepStartFlags=0;	
					if(ModuleIDData==FrontDriveID)
					{
						for(Index=0;Index<2;Index++)
							FrontKeyData[Index].Data=rev_can_packet.data[Index+1];
					}
					else if(ModuleIDData==TopDriveID)
					{
						for(Index=0;Index<2;Index++)
							TopKeyData[Index].Data=rev_can_packet.data[Index+1];
					}
				}
				else if(TopDriveFlag==1)
				{
					SleepStartFlags=0;	
					if(ModuleIDData==BackDriveID)
					{
						for(Index=0;Index<2;Index++)
							BackKeyData[Index].Data=rev_can_packet.data[Index+1];
					}
					else if(ModuleIDData==FrontDriveID)
					{
						for(Index=0;Index<2;Index++)
							FrontKeyData[Index].Data=rev_can_packet.data[Index+1];
					}
				}
				if(ModuleIDData==PowerControlModuleID)
				{
					for(Index=0;Index<1;Index++)
						PowerControlKeyData[Index].Data=rev_can_packet.data[Index+1];
				}
				break;			
			case Sleep_DangerFrame:
				if(BackDriveFlag==1||TopDriveFlag==1)
				{
					BakLeftFlashCode=0;																
					BakRightFlashCode=0; 
					if(rev_can_packet.data[3]==LeftFlashOpenCode)  			
						BakLeftFlashCode=1;	
					else if(rev_can_packet.data[3]==RightFlashOpenCode)	
						BakRightFlashCode=1;
					else if(rev_can_packet.data[3]==AllFlashOpenCode)		
					{
						BakLeftFlashCode=1;
						BakRightFlashCode=1;
					}
				}
				break;
			 case LoadState1Frame:
			 	if(ModuleIDData==PowerModuleID)
			 	{
					for(Index=0;Index<5;Index++)
						PowerLoadState[Index].Data=rev_can_packet.data[Index+1];
			 	}
				break;
			default:
				break;
		}	
	}
	else	if(rev_can_packet.identifier==PannalModuleID)
	{
		switch(ResponseData)
		{
			case Key1StateFrame:
				SleepStartFlags=0;			
				for(Index=0;Index<5;Index++)
					MeterKeyData[Index].Data=rev_can_packet.data[Index+1];
				MeterModuleFlag=1;
				break;
			case Key2StateFrame:	
				for(Index=0;Index<1;Index++)
					MeterDummyKeyData[Index].Data=rev_can_packet.data[Index+1];
				break;
			case Sleep_DangerFrame:
				if(rev_can_packet.data[2]==SleepCode)									
					SleepStartFlags=1;
				else
				{
					SleepStartFlags=0;
					LeftFlashCode=0;																
					RightFlashCode=0; 
					if(rev_can_packet.data[3]==LeftFlashOpenCode)  			
						LeftFlashCode=1;	
					else if(rev_can_packet.data[3]==RightFlashOpenCode)	
						RightFlashCode=1;
					else if(rev_can_packet.data[3]==AllFlashOpenCode)		
					{
						LeftFlashCode=1;
						RightFlashCode=1;
					}
				}
				break;
			case RainScratchTimeFrame:
				ReRainScratchIntermission=rev_can_packet.data[2];			
				RainScratchAckFlag=1;
				break;	
			case IDState1Frame:
				IdRequestFlag=1;
				break;
			case DistanceConfigStateFrame:
				Temp=rev_can_packet.data[3];
				Temp<<=8;
				Temp+=rev_can_packet.data[2];
				Speed_Parameter=Temp;
				Speed_Pulse_Number=rev_can_packet.data[6];
				DistanceConfigACKFlag=1;
				break;
			default:
				break;
		}
	}
	else if(rev_can_packet.identifier==SpeedID)
	{
		switch(ResponseData)
		{
			case	SpeedFrame:
				ReSpeedData=rev_can_packet.data[7];
				break;
			default:
				break;
		}
	}
	else if(rev_can_packet.identifier==RotationId)
	{
		switch(ResponseData)
		{
			case	RotationFrame:
				if(ModuleIDData==PannalModuleID)
				{
					RotationData=rev_can_packet.data[4];
					RotationData<<=8;
					RotationData+=rev_can_packet.data[3];
					RotationData=RotationData/8;
				}
				break;
			default:
				break;
		}
	}
	CANRFLG |= 0x01;  /// Clear reception flag  
}
/***********************************************
* Description: CAN Error interrupt service routine;when rx error then reset 
error flags to obtain synchronous message
* author:   karry
***********************************************/
interrupt void CAN0error_int(void)
{
  	CANRFLG |= 0x02;          /*Reset Overrun Interrupt flag*/
 	asm nop;  
	if(((CANRFLG_TSTAT1==1)&&(CANRFLG_TSTAT0==0))
		||((CANRFLG_RSTAT1==1)&&(CANRFLG_RSTAT0==0)))    /* 127 < error counter */
    CANRFLG |= 254; 				/* Reset all error flags */
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、发送CAN 报文数据
三、形参数：CanPacket->所要发送的数据包
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void CAN_SendFrame(CAN_PACKET_T	CanPacket)
{
	unsigned char	i,cansel;

	CANTBSEL = CANTFLG;    /// Select lowest empty buffer 
  	cansel = CANTBSEL;		 /// Backup selected buffer
	
	///PGN change to 32bit	 
	CANTIDR0 = (CanPacket.identifier>>21) & 0xFF; 	
	CANTIDR1 = (CanPacket.identifier>>18) & 0x07; 
	CANTIDR1 = CANTIDR1 << 5;
	CANTIDR1 = CANTIDR1 | ((CanPacket.identifier>>15) & 0x07);
	CANTIDR1_SRR = 1;
	CANTIDR1_IDE = 1; 		///extend frame IDE=1->29bit,IDE=0->11bit		
	CANTIDR2 = (CanPacket.identifier>>7) & 0xFF; 
	CANTIDR3 = (CanPacket.identifier<<1) & 0xFF; 
	CANTIDR3_RTR = 0; 		///data frame	

	CANTDLR=CanPacket.byte_count;	///length of data
	for(i=0;i<CanPacket.byte_count;i++)
	{
		*(&CANTDSR0+i)=CanPacket.data[i];
	}
	CANTTBPR=1;         		///configure data fifo of send priory
    CANTFLG=cansel;	
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、对应协议发送CAN 报文
三、形参数：SendKind->发送报文的种类
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void SendMessage(unsigned char SendKind)
{
	unsigned char Mi;
	
	if(FrontDriveFlag==1)
	{
		send_can_packet.identifier= FrontDriveID;
	}
	else if(BackDriveFlag==1)
	{
		send_can_packet.identifier=BackDriveID;
	}
	else if(TopDriveFlag==1)
	{
		send_can_packet.identifier=TopDriveID;
	}
	
	switch(SendKind)
	{		
		case	StartBootLoader:
			send_can_packet.byte_count= 1;
			send_can_packet.data[0] = BootloaderStartFrame;
			break;
		case	SendKey1:																	
			send_can_packet.byte_count= 8;
			send_can_packet.data[0] = Key1StateFrame;
			for(Mi=0;Mi<2;Mi++)
				send_can_packet.data[Mi+1] =~OldKeyData[Mi].Data;
			for(Mi=3;Mi<8;Mi++)
				send_can_packet.data[Mi] =0xff;
			break;
		case SendST1:		
			send_can_packet.byte_count= 8;
			send_can_packet.data[0] = LoadState1Frame;
			for(Mi=0;Mi<5;Mi++)
				send_can_packet.data[Mi+1] =LastState[Mi].Data;
			for(Mi=6;Mi<8;Mi++)
				send_can_packet.data[Mi] =0xff;
     	 	break;
		case SendST2:		
			
     	 	break;
		case	SendSensor:															
			send_can_packet.byte_count= 8;
			send_can_packet.data[0] = SensorStateFrame;
			for(Mi=0;Mi<5;Mi++)
				send_can_packet.data[Mi+1] =SendSensorData[Mi];
			send_can_packet.data[6] =0xff;
			send_can_packet.data[7] =SendSensorData[5];
			break;
		case	SendCurrent1:
			
			break;
		case	SendCurrent2:
			
			break;
		case	SendCurrent3:
			
			break;
		case	SendSleep_Danger:
			send_can_packet.byte_count= 8;
			send_can_packet.data[0] = Sleep_DangerFrame;
			send_can_packet.data[1] = 0xff;
			send_can_packet.data[2] = WakeCode;
			send_can_packet.data[3] =SendFlashData[0];
			for(Mi=4;Mi<8;Mi++)
				send_can_packet.data[Mi] =0xff;
			break;
		case	SendSpeed:
			send_can_packet.identifier= SpeedID;
			send_can_packet.byte_count= 8;
			send_can_packet.data[0] = SpeedFrame;
			for(Mi=1;Mi<6;Mi++)
				send_can_packet.data[Mi] =0xff;
			send_can_packet.data[6] =0x00;
			send_can_packet.data[7] =SendSpeedData[0];
			break;
		case	SendRotattion:
			send_can_packet.identifier= RotationId;
			send_can_packet.byte_count= 8;
			send_can_packet.data[0] = RotationFrame;
			send_can_packet.data[1] =0xff;
			send_can_packet.data[2] =0xff;
			send_can_packet.data[3] =(unsigned char)SendRotationData[0];
			send_can_packet.data[4] =(unsigned char)(SendRotationData[0]>>8);
			for(Mi=5;Mi<8;Mi++)
				send_can_packet.data[Mi] =0xff;
			break;
		case	SendRainScratchTime:
			send_can_packet.byte_count= 8;
			send_can_packet.data[0] = RainScratchTimeFrame;
			send_can_packet.data[1] = 0xff;
			send_can_packet.data[2] = 0xfe;
			for(Mi=3;Mi<8;Mi++)
				send_can_packet.data[Mi] =0xff;
			break;
		case	SendOilConsume1:
			
			break;
		case	SendOilConsume2:
			
			break;
		case	SendDistanceState:
			
			break;
		case	SendId1:
			
			break;
		case	SendId2:
			send_can_packet.byte_count= 8;
			send_can_packet.data[0] = IDState2Frame;
			send_can_packet.data[1] = 'Q';
			send_can_packet.data[2] = 'A';
			send_can_packet.data[3] = 'S';
			send_can_packet.data[4] = 'L';
			send_can_packet.data[5] = 'J';
			send_can_packet.data[6] = 'R';
			send_can_packet.data[7] = '0';
			break;
		case	SendId3:
			send_can_packet.byte_count= 8;
			send_can_packet.data[0] = IDState3Frame;
			send_can_packet.data[1] = 'G';
			send_can_packet.data[2] = 0XFF;
			send_can_packet.data[3] = 0XFF;
			send_can_packet.data[4] = 0XFF;
			send_can_packet.data[5] = 0XFF;
			send_can_packet.data[6] = 0XFF;
			send_can_packet.data[7] = 0XFF;
			break;
		case	SendDistanceConfig:
			send_can_packet.byte_count= 8;
			send_can_packet.data[0] = DistanceConfigStateFrame;
			send_can_packet.data[1] = 0XFE;
			for(Mi=2;Mi<8;Mi++)
				send_can_packet.data[Mi] =0xff;
			break;
		case	SendKey2:
			
			break;
		default:
			break;		
	}
	CAN_SendFrame(send_can_packet);  
}

