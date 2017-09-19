#define    CanMsgOut_C
#include "CanMsgOut.h"

#define Tranverse16(X) ((((UINT16)(X)&0xff00)>>8)|(((UINT16)(X)&0x00ff)<<8)) 
#define Tranverse32(X) ((((UINT32)(X)&0xff000000)>>24)|(((UINT32)(X)&0x00ff0000)>>8)|(((UINT32)(X)&0x0000ff00)<<8)|(((UINT32)(X)&0x000000ff)<<24))
#define Tranverse24(X) ((((UINT32)(X)&0x00ff00))|(((UINT32)(X)&0x00ff)<<16)|(((UINT32)(X)&0xff0000)>>16)) 


const UINT8 StLable[5]={0,0x03,0x0f,0x3f,0xff};
Time_Info Time[CanMsgOutNum];
UINT8 IDNum;
CAN_PACKET_T J1939SendFrame;
UINT32 CanInputData;



void CanMsgOutInit(void)
{
	UINT8  i;
	UINT8 TimeUint;
	UINT32 SentTime;
	for(i=0;i<CanMsgOutNum;i++)
	{
		if((J1939ZFFindIndexConfig[i].dstIdentifier == 0xffffffff)||(J1939ZFFindIndexConfig[i].dstIdentifier == 0))
		{
			IDNum = i;
			return;
		}
		Time[i].dstIdentifier = 0x1fffffff & J1939ZFFindIndexConfig[i].dstIdentifier;
		TimeUint = (UINT8)(J1939ZFFindIndexConfig[i].sendCycle>>13);
		SentTime = J1939ZFFindIndexConfig[i].sendCycle&0x1fff;
		switch(TimeUint)
		{
			case 0:
				Time[i].sendCycle = SentTime;
				Time[i].sendCycleBackup = Time[i].sendCycle;
				break;
			case 1: //miao 
				Time[i].sendCycle = SentTime*1000;
				Time[i].sendCycleBackup = Time[i].sendCycle;
				break;
			case 2: //fen
				Time[i].sendCycle = SentTime*60000;
				Time[i].sendCycleBackup = Time[i].sendCycle;
				break;
			case 3: // xiaoshi
				Time[i].sendCycle = SentTime*3600000;
				Time[i].sendCycleBackup = Time[i].sendCycle;
				break;
			case 4:
				Time[i].sendCycle = SentTime*50;
				Time[i].sendCycleBackup = Time[i].sendCycle;
				break;
			default:
    		break;
		}
	}      
}
/***************************************************************************************************
函数名：void CANOutAssemblyData(UINT8 bytelndex,UINT8 bitIndex,UINT8 bitLenght,UINT32 dat) 
功能：处理发送数据组装
参数说明INT8 bytelndex,UINT8 bitIndex,UINT8 bitLenght,UINT32 dat  组装位置
***************************************************************************************************/
void CANOutAssemblyData(UINT8 bytelndex,UINT8 bitIndex,UINT8 bitLenght,UINT32 dat) 
{
    UINT8  residualBit,temp,k;
    UINT8  dstbyteIndex = bytelndex;
    UINT8  dstbitIndex =  bitIndex;
    UINT8  dstbitLenght=  bitLenght;
    UINT32 dstData = dat;
    if(dstbyteIndex>7)  return;
    if(dstbitIndex) 
  	 {
  			   residualBit = 8 -   dstbitIndex;
  			   if(residualBit>=dstbitLenght) 
  			   {
  			      dstData &=((((UINT8)1)<<dstbitLenght)-1);
  			      temp = J1939SendFrame.data[dstbyteIndex]; 
  			      temp >>= (dstbitIndex+dstbitLenght);
  			      temp <<= (dstbitIndex+dstbitLenght);
  			      temp |=  (J1939SendFrame.data[dstbyteIndex]&((1<< dstbitIndex)-1));
  			      J1939SendFrame.data[dstbyteIndex] = temp;
  			      J1939SendFrame.data[dstbyteIndex] |=  (UINT8)(dstData<< dstbitIndex); 
  			      dstbitLenght = 0;
  			   }
  			   else 
  			   {
  			       temp =  ((((UINT8)1)<<dstbitIndex)-1);
  			       J1939SendFrame.data[dstbyteIndex]&= temp;
  			       J1939SendFrame.data[dstbyteIndex] |=  (UINT8)(dstData<< dstbitIndex);
  			       dstData >>=residualBit;
  			       dstbyteIndex++;
  			       if(dstbyteIndex>7)  return;
  			       dstbitLenght -=  residualBit;  
  			   } 
  		}
		 if(dstbitLenght>8)   
		 {
  		   temp = dstbitLenght/8;
  		   for(k=0;k<temp;k++) 
  		   { 
  		     J1939SendFrame.data[dstbyteIndex] = (UINT8)dstData;
  		     dstbyteIndex++;
  		     if(dstbyteIndex>7)  return;
  		     dstData>>=8;
  		   }
  		   dstbitLenght=dstbitLenght%8;
		 }
		 if(dstbitLenght>0)      
		 {
  		   temp = ((((UINT8)1)<<dstbitLenght)-1);
  		   dstData &=temp;
  		   J1939SendFrame.data[dstbyteIndex]&=(~temp);
  	     J1939SendFrame.data[dstbyteIndex] |=  (UINT8)dstData;   
		 }
}
/***********************************************************************************
函数名：ModeChange(UINT8 Index,UINT8 Mode,UINT32 data) 
功能:数据查表
参数说明INT8 bytelndex,UINT8 bitIndex,UINT8 bitLenght,UINT32 dat  组装位置
************************************************************************************/
UINT32 ModeChange(UINT8 Index,UINT8 Mode,UINT32 data)
{
	UINT16 bitValue,offset;
	UINT8 dot,sign;
	UINT8 temp;
	const UINT16 scale[5] = {1,10,100,1000,10000};
	switch(Mode)
	{
		case 0:	break;
		case 1://L:状态转换
		        temp =  data&0xff;
		        data =  J1939ZFDataConfig.Buffer1[Index][temp];
	        	break;
		case 2://L:线性转换
      			bitValue = J1939ZFDataConfig.Buffer2[Index].bitv&0x0fff;
      			dot = (UINT8)(J1939ZFDataConfig.Buffer2[Index].bitv>>12);
      			sign = J1939ZFDataConfig.Buffer2[Index].offset>>15;
      			offset = J1939ZFDataConfig.Buffer2[Index].offset&0x7fff;
      			if(sign)
      			{
      			  if(offset>data)   data = 0;
      				else data = data-offset;
      			}
      			else
      			{
      				data = data+offset;
      			}
      			data = (data*(UINT32)scale[dot])/(UINT32)bitValue;
	        	break;	
		default:
		break;
	}
	return data;
}
/***************************************************************************************************
函数名：void CANOutAssemblyData(UINT8 bytelndex,UINT8 bitIndex,UINT8 bitLenght,UINT32 dat) 
功能：处理发送数据组装
参数说明INT8 bytelndex,UINT8 bitIndex,UINT8 bitLenght,UINT32 dat  组装位置
***************************************************************************************************/
void doJ1939Translate(void) 
{
	CAN_FRAME_INFO  *canFrame;
	UINT8  i,j,k;
	/**********配置数据**************/ 
	UINT8  dataType;
	UINT8  dealType;
	UINT8  tableIndex; 
	UINT16 srcTypeIndex; 
	UINT8  bitIndex;
	UINT8  bitLenght;
	//////////////////////////////
	UINT8  itemLenght;
	UINT16 itemOffset;
	UINT8  temp;	
	UINT8  bitIndexTemp;
	UINT8  byteIndexTemp;
	UINT8  J1939Type; 
	//UINT8  DataChgMode;
	UINT8  type,byteNum;
	UINT8  *pU32StatusData;  

	CAN_FRAME_INFO  canFrameTmp;
#if 0
	temp = CAN1TFLG;
	temp &= 0x7;
	if(temp != 7)		return;		// CAN发送缓存满
#endif
	if(SleepFlag) return;
	for(i=0;i<IDNum;i++) 
	{
		if(Time[i].Flag==1) 
		{
			Time[i].Flag = 0; 

			itemOffset = J1939ZFFindIndexConfig[i].offset; //nbapo:存储的偏移
			itemLenght = J1939ZFFindIndexConfig[i].number; //nbapo:一个ID里有几条信息       
			J1939SendFrame.identifier = 0x1fffffff & J1939ZFFindIndexConfig[i].dstIdentifier;	
			J1939SendFrame.byte_count = 8;
			for(k=0;k<8;k++) J1939SendFrame.data[k]=0xff;

			for(j=0;j<itemLenght;j++) 
			{
				FeedWatchDog();
				canFrameTmp = J1939ZFDataConfig.FRAME_INFO[itemOffset+j];
				canFrame =  &canFrameTmp;
				dataType      =   (UINT8)canFrame->mode >> 4;
				dealType      =   canFrame->mode & 0x0f;	
				tableIndex    =   canFrame->tableIndex;
				srcTypeIndex  =   canFrame->srcTypeIndex;
				bitIndex      =   canFrame->srcbitIndex;
				bitLenght     =   canFrame->srcbitLenght;	
				switch(dataType)
				{
					case 1:  //j1939
						{
							J1939Type  = J1939WordDataConfig[srcTypeIndex].attribute&0x0f; 
							switch(J1939Type) 
							{
								case J1939_Status_Type :  CanInputData=StorageBuffer[J1939_OFFSET_BYTE+srcTypeIndex]; break;            
								case J1939_Char_Type   :  //CanInputData=StorageBuffer[J1939_OFFSET_BYTE+srcTypeIndex]; break;  
								if(bitLenght>8)  CanInputData=*(UINT16 *)(&StorageBuffer[J1939_OFFSET_BYTE+srcTypeIndex]); 
								else  CanInputData=StorageBuffer[J1939_OFFSET_BYTE+srcTypeIndex];
								break;                                                        
								case J1939_Int_Type    :  CanInputData=*(UINT16 *)(&StorageBuffer[J1939_OFFSET_BYTE+2*srcTypeIndex-J1939_STATUS_NUMBER-J1939_CHAR_NUMBER]);break;                
								case J1939_Three_Type  :  CanInputData=*(UINT32 *)(&StorageBuffer[J1939_OFFSET_BYTE + 4*srcTypeIndex-3*J1939_STATUS_NUMBER - 3*J1939_CHAR_NUMBER-2*J1939_INT_NUMBER]);break; 
								case J1939_Word_Type   :  CanInputData=*(UINT32 *)(&StorageBuffer[J1939_OFFSET_BYTE + 4*srcTypeIndex-3*J1939_STATUS_NUMBER - 3*J1939_CHAR_NUMBER-2*J1939_INT_NUMBER]);break;                
							} 
							CanInputData >>= bitIndex;
							CanInputData  &= ((((UINT32)1) << bitLenght)-1);               
							CanInputData = ModeChange(tableIndex, dealType, CanInputData);
							break;
						}

					case 2: 
						{
							type = (UINT8)(srcTypeIndex>>8);	
							switch(type)                          
							{
								case 0x11: pU32StatusData=  DASHBOARD_MODULE_PHYSICAL_KEYS_ADDRESS;  break;   
								case 0x12: pU32StatusData=  FRONT_MODULE_PHYSICAL_KEYS_ADDRESS;      break;    
								case 0x13: pU32StatusData=  BACK_MODULE_PHYSICAL_KEYS_ADDRESS ;      break;    
								case 0x14: pU32StatusData=  MIDDLE_MODULE_PHYSICAL_KEYS_ADDRESS;     break;    
								case 0x15: pU32StatusData=  TOP_MODULE_PHYSICAL_KEYS_ADDRESS;        break;   
								case 0x16: pU32StatusData=  POWER_MODULE_PHYSICAL_KEYS_ADDRESS;      break;     
								case 0x17: pU32StatusData=  POWER_CONTOROL_MODULE_PHYSICAL_KEYS_ADDRESS; break;          
								case 0x18: pU32StatusData=  DASHBOARD_MODULE_VIRTUAL_KEYS_ADDRESS;       break;   
								case 0x19: pU32StatusData=  FRONT_DRIVER_MODULE_SHORT_OPEN_KEYS_ADDRESS; break;   
								case 0x1A: pU32StatusData=  BACK_DRIVER_MODULE_SHORT_OPEN_KEYS_ADDRESS;   break;  
								case 0x1B: pU32StatusData=  MIDDLE_DRIVER_MODULE_SHORT_OPEN_KEYS_ADDRESS; break;  
								case 0x1C: pU32StatusData=  TOP_DRIVER_MODULE_SHORT_OPEN_KEYS_ADDRESS;    break;  
								case 0x1D: pU32StatusData=  POWER_MODULE_SHORT_OPEN_KEYS_ADDRESS;          break; 
								case 0x1E: pU32StatusData=  POWER_CONTOROL_MODULE_SHORT_OPEN_KEYS_ADDRESS; break;               
								case 0x1F: pU32StatusData=  DASHBOARD_MODULE_PHYSICAL_LOAD_ADDRESS;      break;
								case 0x20: pU32StatusData=  FRONT_MODULE_PHYSICAL_LOAD_ADDRESS ;         break;
								case 0x21: pU32StatusData=  BACK_MODULE_PHYSICAL_LOAD_ADDRESS;           break;
								case 0x22: pU32StatusData=  MIDDLE_MODULE_PHYSICAL_LOAD_ADDRESS;         break;
								case 0x23: pU32StatusData=  TOP_MODULE_PHYSICAL_LOAD_ADDRESS;            break;
								case 0x24: pU32StatusData=  POWER_MODULE_PHYSICAL_LOAD_ADDRESS;          break; 

								case 0x25: pU32StatusData=  POWER_CONTOROL_MODULE_PHYSICAL_LOAD_ADDRESS; break;	

								case 0x26: pU32StatusData=  POWER_MODULE_PHYSICAL_Fuse_ADDRESS;break;   //0x26 pd202配电保险丝

								case 0x27: pU32StatusData=  LOAD_STATE_FROM_PIC_ADDRESS;       break;   //负载逻辑转图片

							}
							if(((type <= 0x1e)&&( type >= 0x11))||(type==0x27))
							{
								temp = (UINT8)(srcTypeIndex & 0xff);	
								byteIndexTemp = temp / 8;
								bitIndexTemp  = temp % 8;
								if(bitIndexTemp+bitLenght>8) // 16bit
								{  
									CanInputData= pU32StatusData[byteIndexTemp+1];
									CanInputData<<= 8;
									CanInputData|= pU32StatusData[byteIndexTemp];
								} 
								else 
								{
									CanInputData = pU32StatusData[byteIndexTemp];
								}
								
								CanInputData>>=bitIndexTemp;
								CanInputData&=((0x00001<<bitLenght)-1);   
							}
							else if((type <= 0x26) && (type >= 0x1f))
							{
								temp = (UINT8)(srcTypeIndex&0xff);
								byteIndexTemp = temp/4;
								bitIndexTemp  = temp%4;
								if(bitIndexTemp+bitLenght>4) 
								{  
									CanInputData= pU32StatusData[byteIndexTemp+1];
									CanInputData<<=8;
									CanInputData|= pU32StatusData[byteIndexTemp];
								} 
								else 
								{
									CanInputData = pU32StatusData[byteIndexTemp];
								}
								CanInputData>>=(UINT8)(bitIndexTemp*2);
								CanInputData&=StLable[bitLenght/2];   
							}
							CanInputData = ModeChange(tableIndex, dealType, CanInputData);
							break;	
						}
					case 3: // 内部数据 InsideData	 					
						CanInputData = InsideData[srcTypeIndex];
						CanInputData = ModeChange(tableIndex, dealType, CanInputData);
						break;

					case 4: 
						CanInputData=  USR_s_DeviceConfigData.TotalDistance/1000;
						CanInputData = ModeChange(tableIndex, dealType, CanInputData);
						break;   //总里程

					case 5:  CanInputData=  USR_s_DeviceConfigData.RelativeDistance/1000;
						CanInputData = ModeChange(tableIndex, dealType, CanInputData);
						break;  //里程小计


					//	case 4: // 内部数据 InsideData	 					
					//		CanInputData = srcTypeIndex;     //InsidePhys32Bit
					//	break;


					default:
					break;
				}
				byteNum = canFrame->dstbitLenght/8;
				temp = canFrame->dstbitLenght%8;
				if(temp) byteNum++;
				if(canFrame->mode >> 15) 
				{
					if(byteNum==2)			CanInputData = Tranverse16(CanInputData);
					else if(byteNum==3)		CanInputData = Tranverse24(CanInputData);
					else if(byteNum==4)		CanInputData = Tranverse32(CanInputData);
				}
				CANOutAssemblyData(canFrame->dstbyteIndex,canFrame->dstbitIndex,canFrame->dstbitLenght,CanInputData); 
			}
			k = (UINT8)(J1939ZFFindIndexConfig[i].dstIdentifier	>> 29);
			if(k != 2)	J1939_CanMsgOut_Send(&J1939SendFrame);//==0
			if(k != 0) J1939_CanMsgOut2_Send(&J1939SendFrame);//==1
		}
		
	}  
}
