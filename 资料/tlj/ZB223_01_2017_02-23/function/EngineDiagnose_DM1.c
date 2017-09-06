#define		EngineDiagnoseDM1_C
#include "EngineDiagnose_DM1.h"  
//#pragma DATA_SEG PAGED_RAM
//#pragma CODE_SEG  USER_ROM 
BitCommon  EngineStatus; 
unsigned char enginediagFlag1;
unsigned char enginediagFlag2;
unsigned char enginediagFlag3;
UINT8  MultiPgckageErrorStatus[3];      //多包错误状态

UINT8	 MultiPgckage_TotalPackage_Num[3];
UINT16 MultiPgckage_AvailableByte_Num[3];
UINT16 OldMultiPgckage_AvailableByte_Num[3];
UINT8  EngineDiagnosePackageCount[3];
UINT8  PackageIndex[3]; 
UINT8  EngineDiagnoseCnt[3];

/********************************************
* EngineDiagnosePackageData[0]: 单包诊断数据
********************************************/
UINT32  EngineDiagnosePackageData[20];
UINT16  DiagnosePicIndx[20];

UINT32 EngineDiagnosePackageData1[20];
UINT16  DiagnosePicIndx1[20];

UINT32 EngineDiagnosePackageData2[20];
UINT16  DiagnosePicIndx2[20]; 


/***************************************************************************************************
函数名void doSignelPackageEngineDiagnose(CAN_PACKET_T *pMsg)
功能发动机单包处理
参数说明：CAN_PACKET_T *pMsg 
备注：当接收到单包id 故障个数为一 
***************************************************************************************************/
void EngineDiagnoseLost(void) 
{
	if(EngineDiagnose0_LOSE_F) 
	{
		MultiPgckage_AvailableByte_Num[0] = 0;
		Over_Rx_Engine0_F = 0;
		EngineDiagnosePackageCount[0] = 1;
		Dlay_Engine0_F = 1;
		MultiPgckage_TotalPackage_Num[0] =0;
		MultiPgckageErrorStatus[0] = 0;
	}
	if(EngineDiagnose1_LOSE_F) 
	{
		MultiPgckage_AvailableByte_Num[1] = 0;
		Over_Rx_Engine1_F = 0;
		EngineDiagnosePackageCount[1] = 1;
		Dlay_Engine1_F = 1;
		MultiPgckage_TotalPackage_Num[1] =0;
		MultiPgckageErrorStatus[1] = 0;
	}
	if(EngineDiagnose2_LOSE_F) 
	{
		MultiPgckage_AvailableByte_Num[2] = 0;
		Over_Rx_Engine2_F = 0;
		EngineDiagnosePackageCount[2] = 1;
		Dlay_Engine2_F = 1;
		MultiPgckage_TotalPackage_Num[2] =0;
		MultiPgckageErrorStatus[2] = 0;
	}
}

/************************************************************************
* @brief				电机单包诊断
* @parameter			*pMsg:	j1939接收数据
*						pgckageCnt: 不同的电机协议
*************************************************************************/
void doSignelPackageEngineDiagnose(CAN_PACKET_T *pMsg,UINT8 pgckageCnt)
{
   UINT32 dat;
   
   dat = pMsg->data[5];  
   dat <<= 8;
   dat |= pMsg->data[4];
   dat <<= 8;
   dat |= pMsg->data[3];
   dat <<= 8;
   dat |= pMsg->data[2];       
   
   MultiPgckageErrorStatus[pgckageCnt] = pMsg->data[0];  //多包错误状态
   
   if((dat==0)||(dat==0x00ffffff))  
   		MultiPgckage_AvailableByte_Num[pgckageCnt] = 0;
   else 
   		MultiPgckage_AvailableByte_Num[pgckageCnt] = 1;
   
   if(pgckageCnt==0) 
   {
	    Over_Rx_Engine0_F = 1;
	    EngineDiagnose0_F = 1;
	    EngineDiagnosePackageData[0] = dat; 
   }
   else if(pgckageCnt==1) 
   {
	    EngineDiagnose1_F = 1;
	    Over_Rx_Engine1_F = 1;
	    EngineDiagnosePackageData1[0] = dat; 
   }
   else if(pgckageCnt==2) 
   {
	    EngineDiagnose2_F = 1;
	    Over_Rx_Engine2_F = 1;
	    EngineDiagnosePackageData2[0] = dat; 
   }   
}
/***************************************************************************************************
函数名void doCfgPgckageEngineDiagnose(CAN_PACKET_T *pMsg) 
功能宣告包处理
参数说明：CAN_PACKET_T *pMsg 
备注：提取故障包数 故障个数 和校验
* @parameter			*pMsg			数据包
*						pgckageCnt		多包模式
***************************************************************************************************/
void doCfgPgckageEngineDiagnose(CAN_PACKET_T *pMsg,UINT8 pgckageCnt) 
{
	UINT32 engineDiagnosePGN ;
	UINT16 pgn;

	if(pgckageCnt==0) 
	{
		enginediagFlag1 = 0;
		if(Over_Rx_Engine0_F) return ;
	}
	else if(pgckageCnt==1) 
	{
		enginediagFlag2 = 0;
		if(Over_Rx_Engine1_F) return ;
	}
	else if(pgckageCnt==2) 
	{
		enginediagFlag3 = 0;
		if(Over_Rx_Engine2_F) return ;
	}   

	engineDiagnosePGN = pMsg->data[7];
	engineDiagnosePGN <<= 8;
	engineDiagnosePGN += pMsg->data[6];
	engineDiagnosePGN <<= 8;
	engineDiagnosePGN += pMsg->data[5];

	if(pgckageCnt==0) 	pgn =  (UINT16)(EngineDiagnoseCfg.SignelPackageID>>8);
	else if(pgckageCnt==1) pgn =  (UINT16)(EngineDiagnoseCfg1.SignelPackageID>>8);
	else if(pgckageCnt==2) pgn =  (UINT16)(EngineDiagnoseCfg2.SignelPackageID>>8);

	if(engineDiagnosePGN==0xfee3)  return ;
	if(engineDiagnosePGN==pgn)///confirm it is DM1 message 
	{ 
		if(pgckageCnt==0) 
		{
			enginediagFlag1 = 1;
		}
		else if(pgckageCnt==1) 
		{
			enginediagFlag2 = 1;
		}
		else if(pgckageCnt==2) 
		{
			enginediagFlag3 = 1;
		}    
		MultiPgckage_AvailableByte_Num[pgckageCnt]=pMsg->data[2];
		MultiPgckage_AvailableByte_Num[pgckageCnt] <<= 8;
		MultiPgckage_AvailableByte_Num[pgckageCnt] += pMsg->data[1];  //255*7    总共几个数据 

		if(MultiPgckage_AvailableByte_Num[pgckageCnt]<10)  MultiPgckage_AvailableByte_Num[pgckageCnt]=0;
		else MultiPgckage_AvailableByte_Num[pgckageCnt] = (MultiPgckage_AvailableByte_Num[pgckageCnt]-2)/4;
		MultiPgckage_TotalPackage_Num[pgckageCnt]=pMsg->data[3];///max is 255  几包数
		if( MultiPgckage_TotalPackage_Num[pgckageCnt]>10)  MultiPgckage_TotalPackage_Num[pgckageCnt]= 10;

	}

	EngineDiagnosePackageCount[pgckageCnt] = 1;
	PackageIndex[pgckageCnt] = 0;
	if(pgckageCnt==0)EngineDiagnose0_F = 1;
	else if(pgckageCnt==1) EngineDiagnose1_F = 1;
	else if(pgckageCnt==2) EngineDiagnose2_F = 1;

}
/***************************************************************************************************
函数名void doMultiPgckageEngineDiagnose(CAN_PACKET_T *pMsg)
功能多包数据接收处理
参数说明：CAN_PACKET_T *pMsg 
备注：采用4个一组方式处理  节约内存
***************************************************************************************************/
void doMultiPgckageEngineDiagnose(CAN_PACKET_T *pMsg,UINT8 pgckageCnt)
{
	  UINT32 data;
	  UINT8  index;
	  if(pgckageCnt==0) 
	 {
	  	if(enginediagFlag1==0)  return ;
	  	if(Over_Rx_Engine0_F) return ;
	 }
	 else if(pgckageCnt==1) 
	 {
	  if(enginediagFlag2==0)  return ;
	  if(Over_Rx_Engine1_F) return ;
	 }  
	 else if(pgckageCnt==2) 
	 {
	  if(enginediagFlag3==0)  return ;
	  if(Over_Rx_Engine2_F) return ;
	 }  

	 
	 if(pgckageCnt==0)EngineDiagnose0_F = 1;
	 else if(pgckageCnt==1) EngineDiagnose1_F = 1;
	 else if(pgckageCnt==2) EngineDiagnose2_F = 1;
	   
	if(pMsg->data[0]==EngineDiagnosePackageCount[pgckageCnt]) //nbapo:包计数
	{
		EngineDiagnosePackageCount[pgckageCnt]++;
		if(pMsg->data[0]==1) 
		{
			PackageIndex[pgckageCnt] = 0;
			MultiPgckageErrorStatus[pgckageCnt] = pMsg->data[1];             //1
			data =  pMsg->data[6];  data <<= 8;               //0
			data |= pMsg->data[5];  data <<= 8;               //1
			data |= pMsg->data[4];  data <<= 8;               //
			data |= pMsg->data[3]; 
			
			if(pgckageCnt==0)       EngineDiagnosePackageData[PackageIndex[pgckageCnt]] = data; 
			else if(pgckageCnt==1)  EngineDiagnosePackageData1[PackageIndex[pgckageCnt]] = data; 
			else if(pgckageCnt==2)  EngineDiagnosePackageData2[PackageIndex[pgckageCnt]] = data; 
			PackageIndex[pgckageCnt]++;
			// if(PackageIndex[pgckageCnt]>=MultiPgckage_AvailableByte_Num[pgckageCnt])return;
			data = pMsg->data[7]; 
			if(pgckageCnt==0) 
			{
				EngineDiagnosePackageData[PackageIndex[pgckageCnt]]&=0xffffff00;
				EngineDiagnosePackageData[PackageIndex[pgckageCnt]]|= data;  
			}
			else if(pgckageCnt==1) 
			{
				EngineDiagnosePackageData1[PackageIndex[pgckageCnt]]&=0xffffff00;
				EngineDiagnosePackageData1[PackageIndex[pgckageCnt]] |= data; 
			}
			else if(pgckageCnt==2) 
			{
				EngineDiagnosePackageData2[PackageIndex[pgckageCnt]]&=0xffffff00;
				EngineDiagnosePackageData2[PackageIndex[pgckageCnt]] |= data; 
			}
		} 
		else 
		{
			if(pMsg->data[0]<2) return;
			if(pMsg->data[0]>MultiPgckage_TotalPackage_Num[pgckageCnt]) return;
			
			if(pMsg->data[0]==MultiPgckage_TotalPackage_Num[pgckageCnt])
			{ 
				if(pgckageCnt==0) 
				{
					Over_Rx_Engine0_F = 1;
				}
				else if(pgckageCnt==1) 
				{
					Over_Rx_Engine1_F = 1;
				}  
				else if(pgckageCnt==2) 
				{
					Over_Rx_Engine2_F = 1;
				} 
				EngineDiagnosePackageCount[pgckageCnt] = 1;
			}
			
			index =(UINT8)((pMsg->data[0]-2)%4);
			switch(index) 
			{
				case 0: 
					{
						data =  pMsg->data[3]; 
						data  <<=8; data |=  pMsg->data[2]; 
						data  <<=8; data |=  pMsg->data[1];  
						data  <<=8;                           //加上一
						if(pgckageCnt==0) 
						{
							EngineDiagnosePackageData[PackageIndex[pgckageCnt]]&=0x000000ff;
							EngineDiagnosePackageData[PackageIndex[pgckageCnt]] |= data;   
						}
						else if(pgckageCnt==1) 
						{
							EngineDiagnosePackageData1[PackageIndex[pgckageCnt]]&=0x000000ff;
							EngineDiagnosePackageData1[PackageIndex[pgckageCnt]] |= data;   
						}
						else if(pgckageCnt==2) 
						{
							EngineDiagnosePackageData2[PackageIndex[pgckageCnt]]&=0x000000ff;
							EngineDiagnosePackageData2[PackageIndex[pgckageCnt]] |= data;   
						}		
						
						PackageIndex[pgckageCnt]++;   

						// if(PackageIndex[pgckageCnt]>=MultiPgckage_AvailableByte_Num[pgckageCnt])return; 
						data =  pMsg->data[7]; 
						data  <<=8; data |=  pMsg->data[6]; 
						data  <<=8; data |=  pMsg->data[5];  
						data  <<=8; data |=  pMsg->data[4];  
						if(pgckageCnt==0)       EngineDiagnosePackageData[PackageIndex[pgckageCnt]]   = data; 
						else if(pgckageCnt==1)  EngineDiagnosePackageData1[PackageIndex[pgckageCnt]]  = data; 
						else if(pgckageCnt==2)  EngineDiagnosePackageData2[PackageIndex[pgckageCnt]]  = data; 
						PackageIndex[pgckageCnt]++;
						// if(PackageIndex[pgckageCnt]>=MultiPgckage_AvailableByte_Num[pgckageCnt])return;
						break;
					}      
				case 1:  
					data =  pMsg->data[4]; data  <<=8; data |=  pMsg->data[3]; 
				data  <<=8; data |=  pMsg->data[2];  
				data  <<=8; data |=  pMsg->data[1];  
				if(pgckageCnt==0)       EngineDiagnosePackageData[PackageIndex[pgckageCnt]]  = data; 
				else if(pgckageCnt==1)  EngineDiagnosePackageData1[PackageIndex[pgckageCnt]] = data;   
				else if(pgckageCnt==2)  EngineDiagnosePackageData2[PackageIndex[pgckageCnt]] = data;   
				PackageIndex[pgckageCnt]++; 
				//if(PackageIndex[pgckageCnt]>=MultiPgckage_AvailableByte_Num[pgckageCnt])return;
				data =  pMsg->data[7]; data  <<=8; data |=  pMsg->data[6]; 
				data  <<=8; data |=  pMsg->data[5];  
				if(pgckageCnt==0)     
				{
					EngineDiagnosePackageData[PackageIndex[pgckageCnt]]&=0xff000000;
					EngineDiagnosePackageData[PackageIndex[pgckageCnt]]|= data;  
				}
				else if(pgckageCnt==1) 
				{
				EngineDiagnosePackageData1[PackageIndex[pgckageCnt]]&=0xff000000;
				EngineDiagnosePackageData1[PackageIndex[pgckageCnt]]|= data;   
				}//差1个   
				else if(pgckageCnt==2) 
				{
				EngineDiagnosePackageData2[PackageIndex[pgckageCnt]]&=0xff000000;
				EngineDiagnosePackageData2[PackageIndex[pgckageCnt]]|= data;   
				}//差1个                   
				break;
				case 2:  
				data =  pMsg->data[1]; 
				data <<=24;
				if(pgckageCnt==0) 
				{
				EngineDiagnosePackageData[PackageIndex[pgckageCnt]] &=0x00ffffff;
				EngineDiagnosePackageData[PackageIndex[pgckageCnt]] |= data;   
				}
				else if(pgckageCnt==1)  
				{
				EngineDiagnosePackageData1[PackageIndex[pgckageCnt]] &=0x00ffffff;
				EngineDiagnosePackageData1[PackageIndex[pgckageCnt]] |= data; 
				}
				else if(pgckageCnt==2)  
				{
				EngineDiagnosePackageData2[PackageIndex[pgckageCnt]] &=0x00ffffff;
				EngineDiagnosePackageData2[PackageIndex[pgckageCnt]] |= data; 
				}
				PackageIndex[pgckageCnt]++; 
				// if(PackageIndex[pgckageCnt]>=MultiPgckage_AvailableByte_Num[pgckageCnt])return;
				data =  pMsg->data[5]; data  <<=8; data |=  pMsg->data[4]; 
				data  <<=8; data |=  pMsg->data[3];  
				data  <<=8; data |=  pMsg->data[2];   
				if(pgckageCnt==0)       EngineDiagnosePackageData[PackageIndex[pgckageCnt]] = data; 
				else if(pgckageCnt==1)  EngineDiagnosePackageData1[PackageIndex[pgckageCnt]] = data; 
				else if(pgckageCnt==2)  EngineDiagnosePackageData2[PackageIndex[pgckageCnt]] = data; 
				PackageIndex[pgckageCnt]++;     
				// if(PackageIndex[pgckageCnt]>=MultiPgckage_AvailableByte_Num[pgckageCnt])return;       
				data =  pMsg->data[7]; 
				data <<=8;
				data |=  pMsg->data[6]; 
				if(pgckageCnt==0) 
				{
				EngineDiagnosePackageData[PackageIndex[pgckageCnt]]&=0xffff0000;
				EngineDiagnosePackageData[PackageIndex[pgckageCnt]] |= data; 
				}
				else if(pgckageCnt==1)  
				{
				EngineDiagnosePackageData1[PackageIndex[pgckageCnt]]&=0xffff0000;
				EngineDiagnosePackageData1[PackageIndex[pgckageCnt]] |= data; 
				}//差两个
				else if(pgckageCnt==2)  
				{
				EngineDiagnosePackageData2[PackageIndex[pgckageCnt]]&=0xffff0000;
				EngineDiagnosePackageData2[PackageIndex[pgckageCnt]] |= data; 
				}//差两个
				break;
				case 3:
				data =  pMsg->data[2]; 
				data <<=8;
				data |=  pMsg->data[1];   data <<= 16;
				if(pgckageCnt==0)      
				{
				EngineDiagnosePackageData[PackageIndex[pgckageCnt]] &=0x0000ffff;
				EngineDiagnosePackageData[PackageIndex[pgckageCnt]] |= data;
				}
				else if(pgckageCnt==1)
				{
				EngineDiagnosePackageData1[PackageIndex[pgckageCnt]] &=0x0000ffff;
				EngineDiagnosePackageData1[PackageIndex[pgckageCnt]] |= data; 
				}
				else if(pgckageCnt==2)
				{
				EngineDiagnosePackageData2[PackageIndex[pgckageCnt]] &=0x0000ffff;
				EngineDiagnosePackageData2[PackageIndex[pgckageCnt]] |= data; 
				}
				PackageIndex[pgckageCnt]++;   
				// if(PackageIndex[pgckageCnt]>=MultiPgckage_AvailableByte_Num[pgckageCnt])return;
				data =  pMsg->data[6]; data  <<=8; data |=  pMsg->data[5]; 
				data  <<=8; data |=  pMsg->data[4];  
				data  <<=8; data |=  pMsg->data[3]; 
				if(pgckageCnt==0)       EngineDiagnosePackageData[PackageIndex[pgckageCnt]] = data; 
				else if(pgckageCnt==1)  EngineDiagnosePackageData1[PackageIndex[pgckageCnt]] = data; 
				else if(pgckageCnt==2)  EngineDiagnosePackageData2[PackageIndex[pgckageCnt]] = data; 
				PackageIndex[pgckageCnt]++;  
				// if(PackageIndex[pgckageCnt]>=MultiPgckage_AvailableByte_Num[pgckageCnt])return;
				data =  pMsg->data[7]; 
				if(pgckageCnt==0)       
				{
				EngineDiagnosePackageData[PackageIndex[pgckageCnt]]&=0xffffff00;
				EngineDiagnosePackageData[PackageIndex[pgckageCnt]] |= data;
				}
				else if(pgckageCnt==1) 
				{
				EngineDiagnosePackageData1[PackageIndex[pgckageCnt]]&=0xffffff00;
				EngineDiagnosePackageData1[PackageIndex[pgckageCnt]] |= data; 
				}
				else if(pgckageCnt==2) 
				{
				EngineDiagnosePackageData2[PackageIndex[pgckageCnt]]&=0xffffff00;
				EngineDiagnosePackageData2[PackageIndex[pgckageCnt]] |= data; 
				}				 
				break;                      
			}

		}
	}    
}

/***************************************************************************************************
函数名void FindEngineDiagnose(void) 
功能故障码转换为图片索引
参数说明：
备注：为了解决效率问题  采用一次处理5个
***************************************************************************************************/
void FindEngineDiagnose(void) 
{
	UINT16 i;
	UINT8  j;
	
	if(Over_Rx_Engine0_F) 
	{
		for(j=0;j<5;j++) 
		{
			for(i=0;i<EngineDiagnoseCfg.FailureNumber;i++) 
			{
				if((EngineDiagnosePackageData[EngineDiagnoseCnt[0]]&0x00ffffff) == (EngineDiagnoseCfg.FMI_SPN[i]&0x00ffffff)) 
				{
					DiagnosePicIndx[EngineDiagnoseCnt[0]] = i+1;  break;
				}
			}
			
			if(i==EngineDiagnoseCfg.FailureNumber)	DiagnosePicIndx[EngineDiagnoseCnt[0]] = 0;

			EngineDiagnoseCnt[0]++;
			if(EngineDiagnoseCnt[0] == MultiPgckage_AvailableByte_Num[0])
			{
				Over_Rx_Engine0_F = 0;
				Dlay_Engine0_F    = 1;
				EngineDiagnoseCnt[0] = 0;
				break;  //结束
			}
		}
	}

	if(Over_Rx_Engine1_F) 
	{
	for(j=0;j<5;j++) 
	{
	for(i=0;i<EngineDiagnoseCfg1.FailureNumber;i++) 
	{
	if((EngineDiagnosePackageData1[EngineDiagnoseCnt[1]]&0x00ffffff)==(EngineDiagnoseCfg1.FMI_SPN[i]&0x00ffffff)) 
	{
	DiagnosePicIndx1[EngineDiagnoseCnt[1]] = i+1;  break;
	}
	}
	if(i==EngineDiagnoseCfg1.FailureNumber) DiagnosePicIndx1[EngineDiagnoseCnt[1]] = 0;
	EngineDiagnoseCnt[1]++;
	if(EngineDiagnoseCnt[1]==MultiPgckage_AvailableByte_Num[1])
	{
	Over_Rx_Engine1_F = 0;
	Dlay_Engine1_F    = 1;
	EngineDiagnoseCnt[1] = 0;
	break;  //结束
	}

	}
	}

	if(Over_Rx_Engine2_F) 
	{
	for(j=0;j<5;j++) 
	{
	for(i=0;i<EngineDiagnoseCfg2.FailureNumber;i++) 
	{
	if((EngineDiagnosePackageData2[EngineDiagnoseCnt[2]]&0x00ffffff)==(EngineDiagnoseCfg2.FMI_SPN[i]&0x00ffffff)) 
	{
	DiagnosePicIndx2[EngineDiagnoseCnt[2]] = i+1;  break;
	}
	}
	if(i==EngineDiagnoseCfg2.FailureNumber) DiagnosePicIndx2[EngineDiagnoseCnt[2]] = 0;
	EngineDiagnoseCnt[2]++;
	if(EngineDiagnoseCnt[2]==MultiPgckage_AvailableByte_Num[2])
	{
	Over_Rx_Engine2_F = 0;
	Dlay_Engine2_F    = 1;
	EngineDiagnoseCnt[2] = 0;
	break;  //结束
	}

	}
	}   
}

