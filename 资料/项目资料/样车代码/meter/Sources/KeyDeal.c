#define 	KEYDEAL_C
#include	"config.h"
#pragma CODE_SEG  USER_ROM
#pragma MESSAGE DISABLE C12056///屏蔽无必要的编译提示 
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
void KeyDeal(void)
{
	AdcValueExtract();
	AdcResultChangetoRealy();
	AdcValueChangeToSwitchState();
	AdcResultProcess();	
	VirtualMeterKeyDeal();
	VolatageValueOut();	
	lvi_cx();							

	BoardTempDeal();			

 	PushKey_Scan();			

  menu_action();				
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
void Meterswitchconfig(void)
{
  SwLogicConfiguar[0] = 0b11001000;
  SwLogicConfiguar[1] = 0b10001011;
  SwLogicConfiguar[2] = 0b00000000;
  SwLogicConfiguar[3] = 0b00000000;
  SwLogicConfiguar[4] = 0b00000011;
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
void VirtualMeterKeyDeal(void)
{
#if 0	
	uchar i;
	
	VirtualMeterKey[0].Data=0X00;
	VirtualMeterKey[1].Data=0X00;
	
	if((BMSContrllerGears & 0x01)!=0)//N档
		NShieldSignalKeyBit=KeyOn;
	
	if((BMSContrllerGears & 0x02)!=0)//D档
		DShieldSignalKeyBit=KeyOn;	

	if((BMSContrllerGears & 0x04)!=0)//R档
		RShieldSignalKeyBit=KeyOn;	
	
	if((BMSContrllerGears & 0x20)!=0)//P档
		PShieldSignalKeyBit=KeyOn;	

//	if((OperationStatus&0x03)==0x03)
//		StartKeyBit=KeyOn;

	if((BMSContrllerGears & 0x80)!=0)//系统准备好
		DriverSystemReadyKeyBit=KeyOn;	
	
	for(i=0;i<sizeof(VirtualMeterKey);i++)
	{
		if(VirtualMeterKey[i].Data!=VirtualMeterKeyLast[i].Data)
			SendVirsualMeterKeyFlag=1;
		VirtualMeterKeyLast[i].Data=VirtualMeterKey[i].Data;
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
static uchar AD_Trans(uchar channel)           
{
 unsigned int sum;
 unsigned char	Max,Min,ad_catch[5];
 unsigned char i;
 uint j=0;

 for(i=0;i<5;i++)
 	ad_catch[i]=0;
  ATDCTL5=0xa0+channel; //right together,no sign number,single change
  while(!ATDSTAT0_SCF); //wait a change queue complete
  ad_catch[0]=ATDDR0L;
  ad_catch[1]=ATDDR1L;
  ad_catch[2]=ATDDR2L;
  ad_catch[3]=ATDDR3L;
  ad_catch[4]=ATDDR4L;
  
  Max=ad_catch[0];
  Min=ad_catch[0];
  sum=ad_catch[0];
  for(i=1;i<5;i++)
  {
     sum=sum+ad_catch[i];
     if(ad_catch[i]>Max)  Max=ad_catch[i];
     if(ad_catch[i]<Min)  Min=ad_catch[i];
  }
  return (uchar)((sum-Max-Min)/3);
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
static void	Hc4051_ChannelSelect(unsigned char Num_Hc4051,unsigned char Channel4051)
{
	HC4051_DA1_IO=1;
	HC4051_DB1_IO=1;
	HC4051_DC1_IO=1;	

	HC4051_DA2_IO=1;
	HC4051_DB2_IO=1;
	HC4051_DC2_IO=1;

	HC4051_DA3_IO=1;
	HC4051_DB3_IO=1;
	HC4051_DC3_IO=1;
	
	if(Num_Hc4051==Mcu_U14||Num_Hc4051==Mcu_U19)
	{
		switch(Channel4051)
		{
  		case 0:                        
  		  HC4051_DA1=0;HC4051_DB1=0;HC4051_DC1=0;  		  
  		  break;
  		case 1:
  		  HC4051_DA1=1;HC4051_DB1=0;HC4051_DC1=0;  		  
  		  break;
  		case 2:
  		  HC4051_DA1=0;HC4051_DB1=1;HC4051_DC1=0;  		  
  		  break;
  		case 3:
  		  HC4051_DA1=1;HC4051_DB1=1;HC4051_DC1=0;  		  
  		  break;
  		case 4:
  		  HC4051_DA1=0;HC4051_DB1=0;HC4051_DC1=1;
  		  break;
  		case 5:
  		  HC4051_DA1=1;HC4051_DB1=0;HC4051_DC1=1;
  		  break;
  		case 6:
  		  HC4051_DA1=0;HC4051_DB1=1;HC4051_DC1=1;
  		  break;
  		case 7:
  		  HC4051_DA1=1;HC4051_DB1=1;HC4051_DC1=1;
  		  break;
  		default:
   	  	break;	  
  	} 
	} 	
	else if(Num_Hc4051==Mcu_U15||Num_Hc4051==Mcu_U16)
	{
		switch(Channel4051)
		{
  		case 0:                        
  		  HC4051_DA2=0;HC4051_DB2=0;HC4051_DC2=0;  		  
  		  break;
  		case 1:
  		  HC4051_DA2=1;HC4051_DB2=0;HC4051_DC2=0;  		  
  		  break;
  		case 2:
  		  HC4051_DA2=0;HC4051_DB2=1;HC4051_DC2=0;  		  
  		  break;
  		case 3:
  		  HC4051_DA2=1;HC4051_DB2=1;HC4051_DC2=0;  		  
  		  break;
  		case 4:
  		  HC4051_DA2=0;HC4051_DB2=0;HC4051_DC2=1;
  		  break;
  		case 5:
  		  HC4051_DA2=1;HC4051_DB2=0;HC4051_DC2=1;
  		  break;
  		case 6:
  		  HC4051_DA2=0;HC4051_DB2=1;HC4051_DC2=1;
  		  break;
  		case 7:
  		  HC4051_DA2=1;HC4051_DB2=1;HC4051_DC2=1;
  		  break;
  		default:
   	  	break;	  
  	} 
	}
	else if(Num_Hc4051==Mcu_U17||Num_Hc4051==Mcu_U18)
	{
		switch(Channel4051)
		{
  		case 0:                        
  		 HC4051_DA3=0;HC4051_DB3=0;HC4051_DC3=0;  		  
  		  break;
  		case 1:
  		  HC4051_DA3=1;HC4051_DB3=0;HC4051_DC3=0;  		  
  		  break;
  		case 2:
  		  HC4051_DA3=0;HC4051_DB3=1;HC4051_DC3=0;  		  
  		  break;
  		case 3:
  		  HC4051_DA3=1;HC4051_DB3=1;HC4051_DC3=0;  		  
  		  break;
  		case 4:
  		  HC4051_DA3=0;HC4051_DB3=0;HC4051_DC3=1;
  		  break;
  		case 5:
  		  HC4051_DA3=1;HC4051_DB3=0;HC4051_DC3=1;
  		  break;
  		case 6:
  		  HC4051_DA3=0;HC4051_DB3=1;HC4051_DC3=1;
  		  break;
  		case 7:
  		  HC4051_DA3=1;HC4051_DB3=1;HC4051_DC3=1;
  		  break;
  		default:
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
void AdcValueExtract(void)
{
  volatile uchar i, k;

	k=Mcu_U15;
  for (i = 0; i < 8; i++)
  {
    Hc4051_ChannelSelect(k,i); ///select hc4051 number and channel

    AD[0][i] = AD_Trans(9);		///adc channel is 9
  }
	k=Mcu_U16;
  for (i = 0; i < 8; i++)
  {
    Hc4051_ChannelSelect(k,i); ///select hc4051 number and channel

    AD[1][i] = AD_Trans(10);		///adc channel is 10
  }
	k=Mcu_U17;
  for (i = 0; i < 8; i++)
  {
    Hc4051_ChannelSelect(k,i); ///select hc4051 number and channel

    AD[2][i] = AD_Trans(1);		///adc channel is 1
  }
	k=Mcu_U18;
  for (i = 0; i < 8; i++)
  {
    Hc4051_ChannelSelect(k,i); ///select hc4051 number and channel

    AD[3][i] = AD_Trans(2);		///adc channel is 2
  }
	k=Mcu_U14;
  for (i = 0; i < 8; i++)
  {
    Hc4051_ChannelSelect(k,i); ///select hc4051 number and channel

    AD[4][i] = AD_Trans(15);		///adc channel is 15
  }
	k=Mcu_U19;
  for (i = 0; i < 8; i++)
  {
    Hc4051_ChannelSelect(k,i); ///select hc4051 number and channel

    AD[5][i] = AD_Trans(14);		///adc channel is 14
  }
	Bts724St[0]=AD_Trans(3);
	Bts724St[1]=AD_Trans(4);
	SensorADCData[4]=AD_Trans(13);
	Video_Power[0]=AD_Trans(7);
	Video_Power[1]=AD_Trans(6);
	SPEED_PWM_AD=AD_Trans(12);
	V12_Power=AD_Trans(5);
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
void	AdcResultChangetoRealy(void)
{
	Last_AD[0][0]=AD[0][0];		
	Last_AD[0][1]=AD[0][1];
	Last_AD[0][2]=AD[0][2];
	Last_AD[0][3]=AD[0][3];
	Last_AD[0][4]=AD[0][4];
	Last_AD[0][5]=AD[0][5];
	Last_AD[0][6]=AD[0][6];
	Last_AD[0][7]=AD[0][7];
	
	Last_AD[1][0]=AD[1][0];	
	Last_AD[1][1]=AD[1][1];
	Last_AD[1][2]=AD[1][2];
	Last_AD[1][3]=AD[1][3];
	Last_AD[1][4]=AD[1][4];
	Last_AD[1][5]=AD[1][5];
	Last_AD[1][6]=AD[1][6];
	Last_AD[1][7]=AD[1][7];
	
	PWM_Out_AD[1]=AD[2][0];		
	PWM_Out_AD[0]=AD[2][1];
	Last_AD[2][0]=AD[2][2];
	Last_AD[2][1]=AD[2][3];
	Last_AD[2][2]=AD[2][4];
	Last_AD[2][3]=AD[2][5];
	Last_AD[2][4]=AD[2][6];
	Last_AD[2][5]=AD[2][7];

	Last_AD[2][6]=AD[3][0];		
	Last_AD[2][7]=AD[3][1];
	Last_AD[3][0]=AD[3][2];
	Last_AD[3][1]=AD[3][3];
	Last_AD[3][2]=AD[3][4];
	Last_AD[3][3]=AD[3][5];
	NormAdcData=AD[3][6];
	///NormAdcData=AD[3][7];

	Last_AD[3][4]=AD[4][0];		
	Last_AD[3][5]=AD[4][1];
	Last_AD[3][6]=AD[4][2];
	Last_AD[3][7]=AD[4][3];
	SensorADCData[0]=AD[4][4];
	SensorADCData[1]=AD[4][5];
	SensorADCData[2]=AD[4][6];
	SensorADCData[3]=AD[4][7];

	///Last_AD[4][0]=AD[5][0];		
	Last_AD[4][0]=AD[5][1];
	Last_AD[4][1]=AD[5][2];
	Last_AD[4][2]=AD[5][3];
	Last_AD[4][3]=AD[5][4];
	Last_AD[4][4]=AD[5][5];
	Last_AD[4][5]=AD[5][6];
	Last_AD[4][6]=AD[5][7];
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
void AdcValueChangeToSwitchState(void)
{
  uchar i,j;
	
	i=0;
	for (j = 0; j < 8; j++)
	{
  	if (Last_AD[i][j] > ADCCompareHighData)
  	{
    	if((SwLogicConfiguar[i]>> j) &0x01)///the key is set to high side
    		MeterKeyBuf[i].Data |= 0x01 << j;
    	else
      	MeterKeyBuf[i].Data &= ~(0x01 << j);
  	}
  	else if ((Last_AD[i][j] < ADCCompareLowData)&&(j!=3))//IN4只能正控
  	{
    	if (!((SwLogicConfiguar[i] >> j) &0x01))///the key is set to low side 
    		MeterKeyBuf[i].Data |= 0x01 << j;
    	else
      	MeterKeyBuf[i].Data &= ~(0x01 << j);
  	}
  	else
    	MeterKeyBuf[i].Data &= ~(0x01 << j);///the key is pulled up    	 		
	}
	i=1;
	for (j = 0; j < 8; j++)
	{
  	if (Last_AD[i][j] > ADCCompareHighData)
  	{
    	if((SwLogicConfiguar[i]>> j) &0x01)///the key is set to high side
    		MeterKeyBuf[i].Data |= 0x01 << j;
    	else
      	MeterKeyBuf[i].Data &= ~(0x01 << j);
  	}
  	else if ((Last_AD[i][j] < ADCCompareLowData)&&(j!=0)&&(j!=7))
  	{
    	if (!((SwLogicConfiguar[i] >> j) &0x01))///the key is set to low side 
    		MeterKeyBuf[i].Data |= 0x01 << j;
    	else
      	MeterKeyBuf[i].Data &= ~(0x01 << j);
  	}
  	else
    	MeterKeyBuf[i].Data &= ~(0x01 << j);///the key is pulled up    	 		
	}
	i=2;
	for (j = 0; j < 8; j++)
	{
		if(j<6)
		{
	  	if (Last_AD[i][j] < ADCCompareLowData)
	  	{
	    	if (!((SwLogicConfiguar[i] >> j) &0x01))///the key is set to low side 
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else
	    	MeterKeyBuf[i].Data &= ~(0x01 << j);///the key is pulled up    	 		
		}
		else
		{
			if (Last_AD[i][j] > ADCCompareHighData)
	  	{
	    	if((SwLogicConfiguar[i]>> j) &0x01)///the key is set to high side
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else if (Last_AD[i][j] < ADCCompareLowData)
	  	{
	    	if (!((SwLogicConfiguar[i] >> j) &0x01))///the key is set to low side 
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else
	    	MeterKeyBuf[i].Data &= ~(0x01 << j);///the key is pulled up    	
		}
	}
	i=3;
	for (j = 0; j < 8; j++)
	{
		if(j<4)
		{
	  	if (Last_AD[i][j] > ADCCompareHighData)
	  	{
	    	if((SwLogicConfiguar[i]>> j) &0x01)///the key is set to high side
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else if (Last_AD[i][j] < ADCCompareLowData)
	  	{
	    	if (!((SwLogicConfiguar[i] >> j) &0x01))///the key is set to low side 
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else
	    	MeterKeyBuf[i].Data &= ~(0x01 << j);///the key is pulled up    
		}
		else///in29\in30\in31\in32
		{
			if (Last_AD[i][j] < ADCCompareLow1Data)
			{
	   		if (!((SwLogicConfiguar[i] >> j) &0x01))///the key is set to low side 
	   			MeterKeyBuf[i].Data |= 0x01 << j;
	   		else
	     		MeterKeyBuf[i].Data &= ~(0x01 << j);
	 		}
	 		else
	   		MeterKeyBuf[i].Data &= ~(0x01 << j);	///the key is pulled up    
		}
	}
	i=4;
	for (j = 0; j < 8; j++)
	{
		if(j<2)///in33\in34
		{
			if (Last_AD[i][j] > ADCCompareHigh1Data)
	  	{
	    	if((SwLogicConfiguar[i]>> j) &0x01)///the key is set to high side
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else
	    	MeterKeyBuf[i].Data &= ~(0x01 << j);///the key is pulled up    
		}
		else if((2<=j)&&(j<6))///in35,in36,in37,in38
		{
			if (Last_AD[i][j] > ADCCompareHighData)
	  	{
	    	if((SwLogicConfiguar[i]>> j) &0x01)///the key is set to high side
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else if (Last_AD[i][j] < ADCCompareLowData)
	  	{
	    	if (!((SwLogicConfiguar[i] >> j) &0x01))///the key is set to low side 
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else
	    	MeterKeyBuf[i].Data &= ~(0x01 << j);///the key is pulled up    
		}
/*		else if(j<6)///in37\in38
		{
			if (Last_AD[i][j] > ADCCompareHighData)
	  	{
	    	if((SwLogicConfiguar[i]>> j) &0x01)///the key is set to high side
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else
	    	MeterKeyBuf[i].Data &= ~(0x01 << j);///the key is pulled up    
		}*/
		else///in39
		{
			if (Last_AD[i][j] < ADCCompareLow1Data)
			{
	   		if (!((SwLogicConfiguar[i] >> j) &0x01))///the key is set to low side 
	   			MeterKeyBuf[i].Data |= 0x01 << j;
	   		else
	     		MeterKeyBuf[i].Data &= ~(0x01 << j);
	 		}
	 		else
	   		MeterKeyBuf[i].Data &= ~(0x01 << j);	///the key is pulled up    
		}
	}
}

/*
void AdcValueChangeToSwitchState(void)
{
  uchar i,j;
	
	i=0;
	for (j = 0; j < 8; j++)
	{
  	if (Last_AD[i][j] > ADCCompareHighData)
  	{
    	if((SwLogicConfiguar[i]>> j) &0x01)///the key is set to high side
    		MeterKeyBuf[i].Data |= 0x01 << j;
    	else
      	MeterKeyBuf[i].Data &= ~(0x01 << j);
  	}
  	else if (Last_AD[i][j] < ADCCompareLowData)
  	{
    	if (!((SwLogicConfiguar[i] >> j) &0x01))///the key is set to low side 
    		MeterKeyBuf[i].Data |= 0x01 << j;
    	else
      	MeterKeyBuf[i].Data &= ~(0x01 << j);
  	}
  	else
    	MeterKeyBuf[i].Data &= ~(0x01 << j);///the key is pulled up    	 		
	}
	i=1;
	for (j = 0; j < 8; j++)
	{
  	if (Last_AD[i][j] > ADCCompareHighData)
  	{
    	if((SwLogicConfiguar[i]>> j) &0x01)///the key is set to high side
    		MeterKeyBuf[i].Data |= 0x01 << j;
    	else
      	MeterKeyBuf[i].Data &= ~(0x01 << j);
  	}
  	else if (Last_AD[i][j] < ADCCompareLowData)
  	{
    	if (!((SwLogicConfiguar[i] >> j) &0x01))///the key is set to low side 
    		MeterKeyBuf[i].Data |= 0x01 << j;
    	else
      	MeterKeyBuf[i].Data &= ~(0x01 << j);
  	}
  	else
    	MeterKeyBuf[i].Data &= ~(0x01 << j);///the key is pulled up    	 		
	}
	i=2;
	for (j = 0; j < 8; j++)
	{
		if(j<6)
		{
	  	if (Last_AD[i][j] < ADCCompareLowData)
	  	{
	    	if (!((SwLogicConfiguar[i] >> j) &0x01))///the key is set to low side 
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else
	    	MeterKeyBuf[i].Data &= ~(0x01 << j);///the key is pulled up    	 		
		}
		else
		{
			if (Last_AD[i][j] > ADCCompareHighData)
	  	{
	    	if((SwLogicConfiguar[i]>> j) &0x01)///the key is set to high side
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else if (Last_AD[i][j] < ADCCompareLowData)
	  	{
	    	if (!((SwLogicConfiguar[i] >> j) &0x01))///the key is set to low side 
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else
	    	MeterKeyBuf[i].Data &= ~(0x01 << j);///the key is pulled up    	
		}
	}
	i=3;
	for (j = 0; j < 8; j++)
	{
		if(j<4)
		{
	  	if (Last_AD[i][j] > ADCCompareHighData)
	  	{
	    	if((SwLogicConfiguar[i]>> j) &0x01)///the key is set to high side
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else if (Last_AD[i][j] < ADCCompareLowData)
	  	{
	    	if (!((SwLogicConfiguar[i] >> j) &0x01))///the key is set to low side 
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else
	    	MeterKeyBuf[i].Data &= ~(0x01 << j);///the key is pulled up    
		}
		else///in29\in30\in31\in32
		{
			if (Last_AD[i][j] < ADCCompareLow1Data)
			{
	   		if (!((SwLogicConfiguar[i] >> j) &0x01))///the key is set to low side 
	   			MeterKeyBuf[i].Data |= 0x01 << j;
	   		else
	     		MeterKeyBuf[i].Data &= ~(0x01 << j);
	 		}
	 		else
	   		MeterKeyBuf[i].Data &= ~(0x01 << j);	///the key is pulled up    
		}
	}
	i=4;
	for (j = 0; j < 8; j++)
	{
		if(j<3)///in33\in34\in35
		{
			if (Last_AD[i][j] > ADCCompareHighData)
	  	{
	    	if((SwLogicConfiguar[i]>> j) &0x01)///the key is set to high side
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else
	    	MeterKeyBuf[i].Data &= ~(0x01 << j);///the key is pulled up    
		}
		else if(j==3)///in36
		{
			if (Last_AD[i][j] > ADCCompareHighData)
	  	{
	    	if((SwLogicConfiguar[i]>> j) &0x01)///the key is set to high side
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else if (Last_AD[i][j] < ADCCompareLowData)
	  	{
	    	if (!((SwLogicConfiguar[i] >> j) &0x01))///the key is set to low side 
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else
	    	MeterKeyBuf[i].Data &= ~(0x01 << j);///the key is pulled up    
		}
		else if(j<6)///in37\in38
		{
			if (Last_AD[i][j] > ADCCompareHighData)
	  	{
	    	if((SwLogicConfiguar[i]>> j) &0x01)///the key is set to high side
	    		MeterKeyBuf[i].Data |= 0x01 << j;
	    	else
	      	MeterKeyBuf[i].Data &= ~(0x01 << j);
	  	}
	  	else
	    	MeterKeyBuf[i].Data &= ~(0x01 << j);///the key is pulled up    
		}
		else///in39
		{
			if (Last_AD[i][j] < ADCCompareLow1Data)
			{
	   		if (!((SwLogicConfiguar[i] >> j) &0x01))///the key is set to low side 
	   			MeterKeyBuf[i].Data |= 0x01 << j;
	   		else
	     		MeterKeyBuf[i].Data &= ~(0x01 << j);
	 		}
	 		else
	   		MeterKeyBuf[i].Data &= ~(0x01 << j);	///the key is pulled up    
		}
	}
}*/
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
void AdcResultProcess(void)
{
  unsigned char i;
	 
  switch (r_index)
  {
    case 0:
      for (i = 0; i < MeterKeyNum; i++)
        FirstKeyTmp[i] = MeterKeyBuf[i].Data;
      r_index = 1;
      break;
    case 1:
      for (i = 0; i < MeterKeyNum; i++)
        SecondKeyTmp[i] = MeterKeyBuf[i].Data;
			/*judge if the meter key be changed and want to be send*/
  		for (i = 0; i < MeterKeyNum; i++)
  		{
    		if (FirstKeyTmp[i] != SecondKeyTmp[i])
    			goto _KeyDealOut;
 		 	}
  		for (i = 0; i < MeterKeyNum; i++)
  		{
  			MeterKey[i].Data = SecondKeyTmp[i];
				if(OnKeyBit==KeyOn)
					AccKeyBit=KeyOn;
    		if (OldKey[i] != MeterKey[i].Data)
    		{
      		OldKey[i] = MeterKey[i].Data;
					SendKeyChange();
      		SendCan0Message(SendMeterSwich); 			///if key changed,it will send immediately
					MeterKeySendTime=0;
					SendMeterKeyFlag=0;
    		}
   		}
			_KeyDealOut:
      r_index = 0;
      break;
    default:
      r_index = 0;
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
static void SendKeyChange(void)
{

	SendKey[0].Data=MeterKey[0].Data;
	SendKey[1].Data=MeterKey[1].Data;
	SendKey[2].Data=MeterKey[2].Data;
	SendKey[3].Data=MeterKey[3].Data;
	SendKey[4].Data=MeterKey[4].Data;
/*	
	SendKey[2].b.Key0=MeterKey[2].b.Key0;
	SendKey[2].b.Key1=MeterKey[2].b.Key1;
	SendKey[2].b.Key2=MeterKey[2].b.Key2;
	SendKey[2].b.Key3=MeterKey[2].b.Key3;
	SendKey[2].b.Key4=MeterKey[2].b.Key4;
	SendKey[2].b.Key5=MeterKey[2].b.Key5;
	SendKey[2].b.Key6=MeterKey[3].b.Key0;
	SendKey[2].b.Key7=MeterKey[3].b.Key1;

	SendKey[3].b.Key0=MeterKey[3].b.Key2;
	SendKey[3].b.Key1=MeterKey[3].b.Key3;
	SendKey[3].b.Key2=MeterKey[3].b.Key4;
	SendKey[3].b.Key3=MeterKey[3].b.Key5;
	SendKey[3].b.Key4=MeterKey[4].b.Key0;
	SendKey[3].b.Key5=MeterKey[4].b.Key1;
	SendKey[3].b.Key6=MeterKey[4].b.Key2;
	SendKey[3].b.Key7=MeterKey[4].b.Key3;
	*/
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
static void VolatageValueOut(void)
{
  uchar i;
  uint max = 0, min = 0,sum = 0;
  uint buf_v,bat_v_buf1;
	
	buf_v = ReferenceVoltage;
  buf_v = buf_v * 91;
  buf_v = buf_v / 50;
	
  bat_v_buf[count_v] = buf_v;///count battery voltage and enlarge 10 double
    
  count_v++;
  if (count_v ==VolatageCountNum)
  {
    count_v = 0x00;

    max = bat_v_buf[0];
    min = bat_v_buf[0];
    sum = bat_v_buf[0];

    for (i = 1; i < VolatageCountNum; i++)
    {
      sum += bat_v_buf[i];
      if (bat_v_buf[i] > max)							///extract maximun and min data
        max = bat_v_buf[i];
      if (bat_v_buf[i] < min)
        min = bat_v_buf[i];
    }
    for (i = 0; i < VolatageCountNum; i++)///clear
      bat_v_buf[i] = 0x00;

    bat_v_buf1 = ((sum - max - min) / (VolatageCountNum-2));	///reduce max and min data then extract average data 

		if(bat_v_buf1>=(SystemVolatage+VOLTAGE_REVISE_NUM)||bat_v_buf1<=(SystemVolatage-VOLTAGE_REVISE_NUM))
			SystemVolatage = bat_v_buf1;					///deal for step 
//    if ((bat_v_buf1 < (BATTER_LOWER_DATA *10)) || (bat_v_buf1 > (BATTER_HIGHER_DATA *10)))
		if ((SystemVolatage < (BATTER_LOWER_DATA *10)) || (SystemVolatage > (BATTER_HIGHER_DATA *10)))
      lvi_flag = 1;
    else
      lvi_flag = 0;
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
static void lvi_cx(void)
{
  switch (lvi_index)
  {
    case 0:
      if (lvi_flag == 1)
        lvi_index = 1;
      else
        lvi_index = 2;
      Delay_SystemVoltage = 0;
      break;
    case 1:
      if (lvi_flag == 1)
      {
        if (Delay_SystemVoltage > BatterDetectTimeNum)
        {
          lvi_error = 1;
          lvi_index = 0;
        }
      }
      else
        lvi_index = 0;
      break;
    case 2:
      if (lvi_flag == 0)
      {
        if (Delay_SystemVoltage > BatterDetectTimeNum)
        {
          lvi_error = 0;
          lvi_index = 0;
        }
      }
      else
        lvi_index = 0;
      break;
    default:
      lvi_index = 0;
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
static void	BoardTempDeal(void)
{
	unsigned char	TeperatureNum_Temp;
	///Teperature sensor adc
	TeperatureNum_Temp=AD_Trans(0);	
	///compare 2 time to obtain a realize data
	Count_Temperature_Num++;
	if(Count_Temperature_Num==1)
	{
		Temperature_Num[0]=TeperatureNum_Temp;
	}
	else if(Count_Temperature_Num==2)
	{
		Temperature_Num[1]=TeperatureNum_Temp;
		Count_Temperature_Num=0;
		if(Temperature_Num[0]==Temperature_Num[1])
		{
			/*************************************************************************
			///x,Temperatrue number;y,Voltage of adc;40,x origin number; 
			temperature arithmatic:x=100y-50+40;		
														 y=(AD_value)/256*5
			*************************************************************************/			
			unsigned int	CountTemperature;
			CountTemperature=Temperature_Num[1];
			CountTemperature*=5;				
			CountTemperature*=100;
			CountTemperature>>=8;
			CountTemperature+=40;
			CountTemperature-=50;
			if(CountTemperature<40)///low to zero temperature
			{
				CountTemperature=40-CountTemperature;
				PanelModuleTemperatureFlag=1;
			}
			else
			{
				CountTemperature=CountTemperature-40;
				PanelModuleTemperatureFlag=0;
			}
			panel_temp=(unsigned char)CountTemperature;
			MeterSensorData[5]=0x00;//未定义
			MeterSensorData[6]=panel_temp;
			YiBiaoWenDu[0]=((unsigned char)CountTemperature)/10;
			YiBiaoWenDu[1]=((unsigned char)CountTemperature)%10;;
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
static void PushKey_Scan(void)
{ 
	unsigned char PushKeyTemp;

	///端口设置为输入
  PushKey_1_IO=0;
	PushKey_2_IO=0;
	PushKey_3_IO=0;
	PushKey_4_IO=0;
	PushKey_5_IO=0;
	PushKey_6_IO=0;

	PushKeyTemp=ResetKeyNum;
	if(PushKey_1==0)
		PushKeyTemp=MenuTabKeyNum;
	else if(PushKey_2==0)
		PushKeyTemp=MoveUpKeyNum;
	else if(PushKey_3==0)
		PushKeyTemp=MoveDownKeyNum;
	else if(PushKey_4==0)
		PushKeyTemp=EnterKeyNum;
	else if(PushKey_5==0)
		PushKeyTemp=SilenceNum;
	else if(PushKey_6==0)
		PushKeyTemp=VideoChangeNum;
	
	if(PushKeyTemp==ResetKeyNum)              
	{
		PushKeyDownDelayTime=0;
		PushKeyFlag=0;
	  if(PushKeyUpDelayTime>PushKeyDelayNum) 	
		{
			PushKeyUpDelayTime=0;
			key_number=0;
	  }
	}
	else 
	{	
		PushKeyUpDelayTime=0;
    if(PushKeyDownDelayTime>PushKeyDelayNum)
    {
     	PushKeyDownDelayTime=0;
			if(PushKeyFlag==0)
			{
				key_number=PushKeyTemp;
				PushKeyFlag=1;///当有按键按下后须等到弹起后才能再次响应
			}
		}
	}
}
/*************************************************************************
funtion operation:the process of deal push key funtion;
key1----enter key;key2----number increase or move up, move left;
key3----number decrease or move down,move right;key4----cancel key
funtion call:void
input parameter:void
return:void
 ************************************************************************/
void menu_action(void)
{
	/*****************switch silence process*************/
	Switch_Silence();
	/*****************switch Video control process*************/
	Switch_Video();
	if(VideoOperationFlag==1)
		return;
  /*****************first menu process*************/
 	Switch_MainMenu();
  ///******************second menu process*****************
  Switch_SecondMenu();
  ///******************three menu process*******************
 	Switch_ThirdMenu_Malfunction();
	Switch_ThirdMenu_Diagnose();
	Switch_ThirdMenu_SystemParameter();	
	Switch_ThirdMenu_DistanceFualConsume();	
	Switch_ThirdMenu_PictureAndChannel();
	Switch_ThirdMenu_TimeSet();
	Switch_ThirdMenu_TyrePress();
	Switch_ThirdMenu_HelpInfomation();
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
static void	Switch_Video(void)
{
	if (key_number == VideoChangeNum)
  {
  	key_number = ResetKeyNum;
  	++Switch_Video_Cursor;
		#if VIDOE_NUMBER==0
		Switch_Video_Cursor=0;
		return;
		#elif		VIDOE_NUMBER==1
			if(Switch_Video_Cursor>1)
		#elif VIDOE_NUMBER==2
			if(Switch_Video_Cursor>2)
		#elif VIDOE_NUMBER==3
			if(Switch_Video_Cursor>3)
		#endif
			Switch_Video_Cursor=0;
		switch(Switch_Video_Cursor)
		{
			case SWITCH_VIDEO_OSD:
				SpiOsdWinAllOff();
				FontOsdWinDisplay(FONT_OSD_ALL_WIN,OFF);
				Osd_Video_Change(Osd_State);		
				OsdCmd(OsdOpen);
				VideoOperationFlag=0;
				menu_id = MainMenuNum;
				Display_MainMenu_Flag=1;
				MainMenuDispIndex=0;
				break;
			case	SWITCH_VIDEO_MIDDLE_DOOR:
				OsdCmd(OsdClose);
				VideoChannelSelect(MiddleDoorVideoChannel);
				VideoChannelTemp =VIDEO_CHANNEL_MIDDLE_DOOR;
				Osd_Video_Change(Video_State);
				VideoOperationFlag=1;
				Display_VideoMenu_Flag=1;
				menu_id=VideoMenuNum;
				break;		
			case SWITCH_VIDEO_R_SHIELD:
				OsdCmd(OsdClose);
				VideoChannelSelect(RShieldVideoChannel);
				VideoChannelTemp =VIDEO_CHANNEL_R_SHIELD;
				Osd_Video_Change(Video_State);
				Display_VideoMenu_Flag=1;
				VideoOperationFlag=1;
				menu_id=VideoMenuNum;
				break;
			case SWITCH_VIDEO_FRONT_DOOR:
				OsdCmd(OsdClose);
				VideoChannelSelect(FrontDoorVideoChannel);
				VideoChannelTemp =VIDEO_CHANNEL_FRONT_DOOR;
				Osd_Video_Change(Video_State);
				Display_VideoMenu_Flag=1;
				VideoOperationFlag=1;
				menu_id=VideoMenuNum;				
				break;
			case SWITCH_VIDEO_UP_VIDEO:
				OsdCmd(OsdClose);
				VideoChannelSelect(TopVideoChannel);
				VideoChannelTemp =VIDEO_CHANNEL_UP_VIDEO;
				Osd_Video_Change(Video_State);
				Display_VideoMenu_Flag=1;
				VideoOperationFlag=1;
				menu_id=VideoMenuNum;
				break;
			default:
				break;
		}
		if(M41t00StopFlag==1)///当做时间设置而未保存之前，操作了此键则时间无法继续行走
			M41t00StopFlag = 0;
		///UpVideoDispDelayFlag=0;///clear up video delay display state
		///UpVideoDispDelayTime=0;
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
static void	Switch_Silence(void)
{
	if (key_number == SilenceNum)			///cancel noise
	{
  	key_number = ResetKeyNum;
  	BuzzerCancelFlag = 1;
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
static void	Switch_MainMenu(void)
{
	if (menu_id == MainMenuNum)
  {
  	if (key_number == MenuTabKeyNum)///enter sub menu   
  	{
    	key_number = ResetKeyNum;
    	menu_id = SecondMenuNum;
			Display_SubMenu_Flag=1;
			SubMenu_Cursor=0;
  	}
		else 
  		key_number = ResetKeyNum;
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
static void	Switch_SecondMenu(void)
{
	if (menu_id == SecondMenuNum)
  {
  	if (key_number == MenuTabKeyNum)///return to main menu   
  	{
  		key_number = ResetKeyNum;
  		menu_id = MainMenuNum;
			MainMenuDispIndex=0;
			Display_MainMenu_Flag=1;
  	}
  	else if (key_number == EnterKeyNum)
  	{
  		key_number = ResetKeyNum;
  		menu_id = ThirdMenuNum;
			switch(SubMenu_Cursor)
			{
				case MalfunctionMenuNum:
					Display_ThirdMenu_Mulfunction_Flag=1;
					MulFunctionQueryCursor=0;
					break;
				case DiagnoseMenuNum:
					Display_ThirdMenu_DiagnoseQuery_Flag=1;
					ThirdMenu_DiagnoseQuery_Cursor=0;
					ThirdMenu_DiagnoseQuery_Panal_Cursor=0;
					break;
				case SystemParameterMenuNum:
					Display_ThirdMenu_SystemParameter_Flag=1;
					ThirdMenu_SystemParameter_Cursor=0xFF;
					break;
				case	DistanceFualConsumeMenuNum:
					Display_ThirdMenu_DistanceFualConsume_Flag=1;
					break;
				case PictureAndChannleMenuNum:
					ThirdMenu_Cursor=0;
					FourMenu_Cursor =0xff;
					Display_ThirdMenu_PictureChannle_Flag=1;
					break;
				case TimeSetMenuNum:
					Display_ThirdMenu_TimeSet_Flag=1;
					SystemDataTimeSet_Flag=0;
					TimeSetCursor=0xff;
					break;
				case TyrePressMenuNum:
					Display_ThirdMenu_TyrePress_Flag=1;
					TyrePressMenuStatus=0xff;
					TyrePressMenuLocate=0xff;
					TyrePressMenuItem = 0;
					Eeprom_Read_Fact(TPMSIDSAVEStateAddr, (uchar*)&TPMS_ID, 18);
					break;
				case HelpInfomationMenuNum:
					Display_ThirdMenu_HelpInfomation_Flag=1;
					VersionQuery_Flag=0;
					break;	
				default:
					break;
				}
    	}
    	else if (key_number ==MoveDownKeyNum )
    	{
    		key_number = ResetKeyNum;
				if (SubMenu_Cursor >= 7)
        	SubMenu_Cursor = 0;
    		else
      		SubMenu_Cursor++;
    	}
    	else if (key_number ==MoveUpKeyNum )
    	{
    		key_number = ResetKeyNum;
				if (SubMenu_Cursor == 0)
        	SubMenu_Cursor = 7;
      	else
        	SubMenu_Cursor--;
    	}
    	else
    	{
      	key_number = ResetKeyNum;
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
static void	Switch_ThirdMenu_Malfunction(void)//故障查询
{
	unsigned char Temp;
	
	if (menu_id == ThirdMenuNum)
  {
	 	if (SubMenu_Cursor == MalfunctionMenuNum)
    {
			if (key_number == EnterKeyNum||key_number == MenuTabKeyNum)
      {
        key_number = ResetKeyNum;
				menu_id = SecondMenuNum;
				Display_SubMenu_Flag=1;
      }
			else if(key_number == MoveDownKeyNum)
			{
				key_number = ResetKeyNum;
				ThirdMenu_Mal_OperatFlag=1;
				if (dis_count > 0)
        {
          if ((((MulFunctionQueryCursor+1) == dis_count/ThirdMenu_PageDispNum)&&((dis_count%ThirdMenu_PageDispNum)==0))||(MulFunctionQueryCursor==(dis_count/ThirdMenu_PageDispNum)))
          {
            MulFunctionQueryCursor = 0;
          }			
          else
          {
          	Temp=dis_count%ThirdMenu_PageDispNum;
						if(Temp!=0||((dis_count/ThirdMenu_PageDispNum)>1))///不是刚好整页时
            	MulFunctionQueryCursor++;
          }
        }
			}
			else if(key_number ==MoveUpKeyNum )
			{
				key_number = ResetKeyNum;
				ThirdMenu_Mal_OperatFlag=1;
				if (dis_count > 0)
        {
          if (MulFunctionQueryCursor == 0)
          {
          	Temp=dis_count%ThirdMenu_PageDispNum;
						if(Temp!=0)///不是刚好整页时
            	MulFunctionQueryCursor = dis_count/ThirdMenu_PageDispNum;
						else// if((dis_count/ThirdMenu_PageDispNum)>1))
							MulFunctionQueryCursor = dis_count/ThirdMenu_PageDispNum-1;
          }
          else
          {
            MulFunctionQueryCursor--;
          }
        }
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
static void	Switch_ThirdMenu_Diagnose(void)///系统诊断
{
	if (menu_id == ThirdMenuNum)
  {
	 	if (SubMenu_Cursor == DiagnoseMenuNum)
    {
			if (key_number == EnterKeyNum)
      {
        key_number = ResetKeyNum;
        menu_id = SecondMenuNum;
				Display_SubMenu_Flag=1;	
      }
			else if(key_number == MenuTabKeyNum)
			{
				key_number = ResetKeyNum;
				Display_ThirdMenu_DiagnoseQuery_Flag=1;
				ThirdMenu_DiagnoseQuery_Cursor++;
				if(ThirdMenu_DiagnoseQuery_Cursor>=8)
					ThirdMenu_DiagnoseQuery_Cursor=0;
				ThirdMenu_DiagnoseQuery_FrontDrive_Cursor=0;
				ThirdMenu_DiagnoseQuery_BackDrive_Cursor=0;
				ThirdMenu_DiagnoseQuery_TopDrive_Cursor=0;
				ThirdMenu_DiagnoseQuery_Panal_Cursor=0;
				ThirdMenu_DiagnoseQuery_Power_Cursor=0;
				ThirdMenu_DiagnoseQuery_PowerManage_Cursor=0;
				ThirdMenu_DiagnoseQuery_BMSPower_Cursor=0;
			}
			else if (key_number == MoveDownKeyNum)
      {
				key_number = ResetKeyNum;
				Display_ThirdMenu_DiagnoseQuery_Flag=1;
				switch(ThirdMenu_DiagnoseQuery_Cursor)
				{
					case	0:
						if(ThirdMenu_DiagnoseQuery_Panal_Cursor==2)
							ThirdMenu_DiagnoseQuery_Panal_Cursor=0;
						else
							ThirdMenu_DiagnoseQuery_Panal_Cursor++;
						break;
					case	1:	//前控					
						if(ThirdMenu_DiagnoseQuery_FrontDrive_Cursor==2)
							ThirdMenu_DiagnoseQuery_FrontDrive_Cursor=0;
						else
							ThirdMenu_DiagnoseQuery_FrontDrive_Cursor++;							
						break;
					case	2:  //后控
						if(ThirdMenu_DiagnoseQuery_BackDrive_Cursor==2)
							ThirdMenu_DiagnoseQuery_BackDrive_Cursor=0;
						else
							ThirdMenu_DiagnoseQuery_BackDrive_Cursor++;	
						break;
					case	3:	//顶控
						if(ThirdMenu_DiagnoseQuery_TopDrive_Cursor==2)
							ThirdMenu_DiagnoseQuery_TopDrive_Cursor=0;
						else
							ThirdMenu_DiagnoseQuery_TopDrive_Cursor++;	
						break;
					case	4:	//配电
						if(ThirdMenu_DiagnoseQuery_Power_Cursor==2)
							ThirdMenu_DiagnoseQuery_Power_Cursor=0;
						else
							ThirdMenu_DiagnoseQuery_Power_Cursor++;								
						break;
					case	5:	//电源管理模块
						if(ThirdMenu_DiagnoseQuery_PowerManage_Cursor==1)
							ThirdMenu_DiagnoseQuery_PowerManage_Cursor=0;
						else
							ThirdMenu_DiagnoseQuery_PowerManage_Cursor++;	

						break;
					case	6:	//温度

						break;
					case	7:	//发动机
						if(ThirdMenu_DiagnoseQuery_BMSPower_Cursor==6)
							ThirdMenu_DiagnoseQuery_BMSPower_Cursor=0;
						else
							ThirdMenu_DiagnoseQuery_BMSPower_Cursor++;	
						break;
					default:
						break;
				}
			}
			else if (key_number == MoveUpKeyNum)
      {
				key_number = ResetKeyNum;
				Display_ThirdMenu_DiagnoseQuery_Flag=1;
				switch(ThirdMenu_DiagnoseQuery_Cursor)
				{
					case	0:
						if(ThirdMenu_DiagnoseQuery_Panal_Cursor==0)
							ThirdMenu_DiagnoseQuery_Panal_Cursor=2;
						else
							ThirdMenu_DiagnoseQuery_Panal_Cursor--;
						break;
					case	1:
						if(ThirdMenu_DiagnoseQuery_FrontDrive_Cursor==0)
							ThirdMenu_DiagnoseQuery_FrontDrive_Cursor=2;
						else
							ThirdMenu_DiagnoseQuery_FrontDrive_Cursor--;
						break;
					case	2:
						if(ThirdMenu_DiagnoseQuery_BackDrive_Cursor==0)
							ThirdMenu_DiagnoseQuery_BackDrive_Cursor=2;
						else
							ThirdMenu_DiagnoseQuery_BackDrive_Cursor--;
						break;
					case	3:
						if(ThirdMenu_DiagnoseQuery_TopDrive_Cursor==0)
							ThirdMenu_DiagnoseQuery_TopDrive_Cursor=2;
						else
							ThirdMenu_DiagnoseQuery_TopDrive_Cursor--;
						break;
					case	4:
						if(ThirdMenu_DiagnoseQuery_Power_Cursor==0)
							ThirdMenu_DiagnoseQuery_Power_Cursor=2;
						else
							ThirdMenu_DiagnoseQuery_Power_Cursor--;
						break;
					case	5:
						if(ThirdMenu_DiagnoseQuery_PowerManage_Cursor==0)
							ThirdMenu_DiagnoseQuery_PowerManage_Cursor=1;
						else
							ThirdMenu_DiagnoseQuery_PowerManage_Cursor--;
						break;
					case	6:
						break;
					case	7:
						if(ThirdMenu_DiagnoseQuery_BMSPower_Cursor==0)
							ThirdMenu_DiagnoseQuery_BMSPower_Cursor=6;
						else
							ThirdMenu_DiagnoseQuery_BMSPower_Cursor--;							
						break;
					default:
						break;
				}
			}
		}
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、三级界面系统参数设置
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void	Switch_ThirdMenu_SystemParameter(void)
{
	uchar SystemParameterTemp[3];///系统参数的设置项
	
	if (menu_id == ThirdMenuNum)
  {
	 	if (SubMenu_Cursor == SystemParameterMenuNum)
    {	
			if(ThirdMenu_SystemParameter_Cursor==0xff)//未进入设置状态
			{
				if(key_number == MenuTabKeyNum)
				{
					key_number = ResetKeyNum;
					menu_id = SecondMenuNum;
					Display_SubMenu_Flag=1;		
				}
				else if (key_number == EnterKeyNum)
				{
					key_number = ResetKeyNum;
					SystemParameter_CursorChange_Flag=1;
					ThirdMenu_SystemParameter_Cursor=0;
				}
				else if (key_number == MoveDownKeyNum||key_number == MoveUpKeyNum)
				{
					key_number = ResetKeyNum;
				}
			}
			else
			{ //进入设置状态
				//系统参数设置页
				if (key_number == MenuTabKeyNum)////move to next position
				{
					key_number = ResetKeyNum;
					SystemParameter_CursorChange_Flag=1;
					BuzzerLevelSetFlag=0;
					LightLevelSetFlag=0;
					ThirdMenu_SystemParameter_Cursor++;
					if(ThirdMenu_SystemParameter_Cursor>2)
						ThirdMenu_SystemParameter_Cursor=0;
				}
				else if (key_number == EnterKeyNum)
				{
					key_number = ResetKeyNum;
					SystemParameter_CursorChange_Flag=1;					
					ThirdMenu_SystemParameter_Cursor=0xff;
					BuzzerLevelSetFlag=0;
					LightLevelSetFlag=0;
					//保存参数设置到EEPROM
					SystemParameterTemp[0]=rain_scr;		
					SystemParameterTemp[1]=LightLevel;	
					SystemParameterTemp[2]=BuzzerLevel;			
					Eeprom_Write_Fact(RainScratch_Addr, SystemParameterTemp, sizeof(SystemParameterTemp));							
				}
				else if (key_number == MoveDownKeyNum)
				{
					key_number = ResetKeyNum;			
					if(ThirdMenu_SystemParameter_Cursor==0)
					{	//雨刮设置
						RainScratchChangeFlag = 1;
						rain_scr++;
						if(rain_scr>RainScratchLowSpeedNum)
							rain_scr=RainScratchLowSpeedNum;
					}
					else if(ThirdMenu_SystemParameter_Cursor==1)
					{	//仪表背光
						LightLevelSetFlag=1;
						LightLevel--;
						if(LightLevel==0X00)
							LightLevel=1;
					}
					else if(ThirdMenu_SystemParameter_Cursor==2)
					{ //蜂鸣器设置
						BuzzerLevel--;
						BuzzerLevelSetFlag=1;
						if(BuzzerLevel==0X00)
							BuzzerLevel=1;
					}
				}
				else if (key_number == MoveUpKeyNum)
				{
					key_number = ResetKeyNum;
					if(ThirdMenu_SystemParameter_Cursor==0)
					{//雨刮间歇值
						RainScratchChangeFlag = 1;
						rain_scr--;
						if(rain_scr<RainScratchHighSpeedNum)
							rain_scr=RainScratchHighSpeedNum;
					}
					else if(ThirdMenu_SystemParameter_Cursor==1)
					{	//仪表背光
						LightLevelSetFlag=1;
						LightLevel++;
						if(LightLevel>3)
							LightLevel=3;
					}
					else if(ThirdMenu_SystemParameter_Cursor==2)
					{ //蜂鸣器设置
						BuzzerLevelSetFlag=1;
						BuzzerLevel++;
						if(BuzzerLevel>3)
							BuzzerLevel=3;
					}
				}			
			
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
//旅程查询
static void Switch_ThirdMenu_DistanceFualConsume(void)
{
	if (menu_id == ThirdMenuNum)
	{
		if (SubMenu_Cursor == DistanceFualConsumeMenuNum)
		{
			if (key_number == MenuTabKeyNum)////move to next position
			{
				key_number = ResetKeyNum;
				menu_id = SecondMenuNum;
				Display_SubMenu_Flag=1;
			}
			if(key_number == EnterKeyNum)
			{
				key_number = ResetKeyNum;
				SmallDistanceCancelFlag=1;
				CountSmallFuelConsumeFlag=1;
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
static void	Switch_ThirdMenu_PictureAndChannel(void)///图像视频通道设置
{
	uchar TftShine[4];///视频色彩的调制
	
	if (menu_id == ThirdMenuNum)
  {
	 	if (SubMenu_Cursor == PictureAndChannleMenuNum)///picture set
    {
			if(FourMenu_Cursor==0xff)
			{
				if(key_number == MoveUpKeyNum||key_number == MoveDownKeyNum)////move to next position
				{
					Display_ThirdMenu_PictureChannle_Flag=1;
					PictureChannleSelChange_Flag=0;
					key_number = ResetKeyNum;
					ThirdMenu_Cursor^=0x01;
				}
				else if(key_number == MenuTabKeyNum)
	      {
	        key_number = ResetKeyNum;        	
					menu_id = SecondMenuNum;
					Display_SubMenu_Flag=1;
	      }
				else if(key_number == EnterKeyNum)
	      {
	        key_number = ResetKeyNum;
					menu_id = ThirdMenuNum;
					FourMenu_Cursor=0x00;
					PictureChannleSelChange_Flag=1;
					if(ThirdMenu_Cursor==ChannleSetSubMenuNum)
					{
						Display_ThirdMenu_PictureChannle_Flag=1;					
						VideoChannelSelect(FrontDoorVideoChannel);
						Osd_Video_Change(Video_State);		
//						VideoSignalDetect();
					}
					else
					{
						if(FrontDoorVideoSignalNum)
						{
							VideoChannelSelect(FrontDoorVideoChannel);
							Osd_Video_Change(Video_State);
						}
						else if(MiddleDoorVideoSignalNum)
						{
							VideoChannelSelect(MiddleDoorVideoChannel);
							Osd_Video_Change(Video_State);
						}
						else if(RShieldVideoSignalNum)
						{
							VideoChannelSelect(RShieldVideoChannel);
						  Osd_Video_Change(Video_State);
						}
						else if(TopVideoSignalNum)
						{
							VideoChannelSelect(TopVideoChannel);
							Osd_Video_Change(Video_State);
						}		
					}
	      }		
			}
			else
			{
				if(key_number == MoveUpKeyNum)////move to next position
				{
					key_number = ResetKeyNum;
					PictureChannleNumChange_Flag=1;
					if(ThirdMenu_Cursor==ChannleSetSubMenuNum)
					{
						if(FourMenu_Cursor==0)
						{
							FrontDoorVideoChannel++;
							if(FrontDoorVideoChannel>3)
								FrontDoorVideoChannel=0;						
							VideoChannelSelect(FrontDoorVideoChannel);
						}
						else if(FourMenu_Cursor==1)
						{
							MiddleDoorVideoChannel++;
							if(MiddleDoorVideoChannel>3)
								MiddleDoorVideoChannel=0;									
							VideoChannelSelect(MiddleDoorVideoChannel);

						}
						else if(FourMenu_Cursor==2)
						{
							RShieldVideoChannel++;
							if(RShieldVideoChannel>3)
								RShieldVideoChannel=0;								
							VideoChannelSelect(RShieldVideoChannel);
						}
						else if(FourMenu_Cursor==3)
						{
							TopVideoChannel++;
							if(TopVideoChannel>3)
								TopVideoChannel=0;								
							VideoChannelSelect(TopVideoChannel);
						}
					}
					else if(ThirdMenu_Cursor==PictureSetSubMenuNum)
					{
						if(FourMenu_Cursor==0)
						{
							PictureContrastNum++;
							if(PictureContrastNum>10)
								PictureContrastNum=10;		
						}
						else if(FourMenu_Cursor==1)
						{
							PictureBrightNum++;
							if(PictureBrightNum>10)
								PictureBrightNum=10;									
						}
						else if(FourMenu_Cursor==2)
						{
							PictureChromaNum++;
							if(PictureChromaNum>10)
								PictureChromaNum=10;		
						}
						else if(FourMenu_Cursor==3)
						{
							PictureDefinitionNum++;
							if(PictureDefinitionNum>10)
								PictureDefinitionNum=10;	
						}
					}
				}
				else if(key_number == MoveDownKeyNum)
				{
					key_number = ResetKeyNum;
					PictureChannleNumChange_Flag=1;					
					if(ThirdMenu_Cursor==ChannleSetSubMenuNum)
					{
						if(FourMenu_Cursor==0)
						{
							FrontDoorVideoChannel--;
							if(FrontDoorVideoChannel==0xff)
								FrontDoorVideoChannel=3;
							VideoChannelSelect(FrontDoorVideoChannel);
						}
						else if(FourMenu_Cursor==1)
						{
							MiddleDoorVideoChannel--;
							if(MiddleDoorVideoChannel==0xff)
								MiddleDoorVideoChannel=3;		
							VideoChannelSelect(MiddleDoorVideoChannel);
						}
						else if(FourMenu_Cursor==2)
						{
							RShieldVideoChannel--;
							if(RShieldVideoChannel==0xff)
								RShieldVideoChannel=3;		
							VideoChannelSelect(RShieldVideoChannel);
						}
						else if(FourMenu_Cursor==3)
						{
							TopVideoChannel--;
							if(TopVideoChannel==0xff)
								TopVideoChannel=3;			
							VideoChannelSelect(TopVideoChannel);
						}		
//						Osd_Video_Change(Video_State);
//						VideoSignalDetect();
					}
					else if(ThirdMenu_Cursor==PictureSetSubMenuNum)
					{
						if(FourMenu_Cursor==0)
						{
							PictureContrastNum--;
							if(PictureContrastNum==0xff)
								PictureContrastNum=0;		
						}
						else if(FourMenu_Cursor==1)
						{
							PictureBrightNum--;
							if(PictureBrightNum==0xff)
								PictureBrightNum=0;									
						}
						else if(FourMenu_Cursor==2)
						{
							PictureChromaNum--;
							if(PictureChromaNum==0xff)
								PictureChromaNum=0;		
						}
						else if(FourMenu_Cursor==3)
						{
							PictureDefinitionNum--;
							if(PictureDefinitionNum==0xff)
								PictureDefinitionNum=0;	
						}
					}					
				}
				else if(key_number == MenuTabKeyNum)
				{
					PictureChannleSelChange_Flag=1;
					key_number = ResetKeyNum; 				
					FourMenu_Cursor++;
					if(ThirdMenu_Cursor==PictureSetSubMenuNum)
					{
						if(FourMenu_Cursor>3)
							FourMenu_Cursor=0;
					}
					else if(ThirdMenu_Cursor==ChannleSetSubMenuNum)
					{
						if(FourMenu_Cursor>3)
							FourMenu_Cursor=0;
						if(FourMenu_Cursor==0)
							VideoChannelSelect(FrontDoorVideoChannel);
						else if(FourMenu_Cursor==1)
							VideoChannelSelect(MiddleDoorVideoChannel);
						else if(FourMenu_Cursor==2)
							VideoChannelSelect(RShieldVideoChannel);
						else if(FourMenu_Cursor==3)
							VideoChannelSelect(TopVideoChannel);						
					}
				}
				else if(key_number == EnterKeyNum)
				{
	        key_number = ResetKeyNum;    
					PictureChannleSelChange_Flag=1;
					//保存图像设置值
					if(ThirdMenu_Cursor==PictureSetSubMenuNum)
					{
						TftShine[0]=PictureContrastNum;		//DuiBiDuNum;
						TftShine[1]=PictureBrightNum;			//LiangDuNum;
						TftShine[2]=PictureChromaNum;			//SeDuNum;
						TftShine[3]=PictureDefinitionNum;	//QingXiDuNum

						Eeprom_Write_Fact(LcdShine_Addr, TftShine, sizeof(TftShine));

						Osd_Video_Change(Osd_State);
					}
					else
					{
						TftShine[0]=FrontDoorVideoChannel;		//前门
						TftShine[1]=MiddleDoorVideoChannel;		//中门
						TftShine[2]=RShieldVideoChannel;			//倒车
						TftShine[3]=TopVideoChannel;					//顶部
						Eeprom_Write_Fact(FrontDoorVideoChannelAddr, TftShine, sizeof(TftShine));						

						TftShine[0]=FrontDoorVideoSignalNum;		//前门视频状态
						TftShine[1]=MiddleDoorVideoSignalNum;		//中门视频状态
						TftShine[2]=RShieldVideoSignalNum;			//倒车视频状态
						TftShine[3]=TopVideoSignalNum;					//顶部视频状态
						Eeprom_Write_Fact(FrontDoorVideoSignalStateAddr, TftShine, sizeof(TftShine));		
						Osd_Video_Change(Osd_State);
//						VideoChannelSelect(TftShine[FourMenu_Cursor]);
						Display_ThirdMenu_PictureChannle_Flag=1;
					}					
					FourMenu_Cursor=0xff;
				} 
			}
	 	}
	}
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、三级界面时间设置
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void	Switch_ThirdMenu_TimeSet(void)
{
	uint year = 0;
  uchar month_day = 0, date_count = 0;
	if (menu_id == ThirdMenuNum)
  {
	 	if (SubMenu_Cursor == TimeSetMenuNum)
    {
			if(TimeSetCursor==0xff)
			{
				if(key_number == MenuTabKeyNum)////move to next position
				{
					key_number = ResetKeyNum;
					menu_id = SecondMenuNum;
					Display_SubMenu_Flag=1;		
				}
				else if (key_number == EnterKeyNum)
				{
					key_number = ResetKeyNum;
					TimeSetCursor=0;
					if(time_error_flag==1)
					{
						SystemDataTimeSelChange_Flag=1;
						time_error_flag=0;
					}
				}
				else if (key_number == MoveDownKeyNum)
				{
					key_number = ResetKeyNum;
					SystemDataTimeSet_Flag^=1;
					SystemDataTimeSelChange_Flag=1;
				}
				else if (key_number == MoveUpKeyNum)
				{
					key_number = ResetKeyNum;
					SystemDataTimeSet_Flag^=1;
					SystemDataTimeSelChange_Flag=1;
				}
			}
			else
			{
				if (key_number == MenuTabKeyNum)////move to next position
				{
					key_number = ResetKeyNum;
					TimeSetCursor++;
					if(TimeSetCursor>7)
						TimeSetCursor=0;
					if(TimeSetCursor==2||TimeSetCursor==5)
						TimeSetCursor++;
				}
				else if (key_number == EnterKeyNum)
				{
					key_number = ResetKeyNum;
					TimeSetCursor=0xff;
					
					if(SystemDataTimeSave_Flag==1)
					{
						SystemDataTimeSave_Flag=0;
						M41t00StopFlag = 0;
						time_error_flag = 0;
						if ((month_h > 1) || ((month_h == 1) && (month_l > 2)))
							time_error_flag = 1;
						
						year = 2000+year_h * 10+year_l;
						month_day = month_h * 10+month_l;
						date_count = date_h * 10+date_l;
						
						if (date_count > month_n_day(year, month_day))
								time_error_flag = 1;
						
						if ((hour_h > 2) || ((hour_h == 2) && (hour_l > 4)))
								time_error_flag = 1;
						
						if (minute_h > 6)
								time_error_flag = 1;
						
						if (second_h > 6)
							time_error_flag = 1;
						
						if (time_error_flag != 1)
							TimeSet(); ////write M41t00 timer
						else
							SystemDataTimeSelChange_Flag=1;
					}					
				}
				else if (key_number == MoveDownKeyNum)
				{
					SystemDataTimeSave_Flag=1;
					key_number = ResetKeyNum;				
					if(SystemDataTimeSet_Flag==0)
					{
						if (TimeSetCursor == 0)
		        {
		          if (year_h == 0)
		            year_h = 9;
		          else
		            year_h--;
		        }
		        else if (TimeSetCursor == 1)
		        {
		          if (year_l == 0)
		            year_l = 9;
		          else
		            year_l--;
		        }
		        else if (TimeSetCursor == 3)
		        {
		          if (month_h == 0)
		            month_h = 1;
		          else
		            month_h = 0;
		        }
		        else if (TimeSetCursor ==4)
		        {
		          if (month_l == 0)
		            month_l = 9;
		          else
		            month_l--;
		        }
		        else if (TimeSetCursor == 6)
		        {
		          if (date_h == 0)
		            date_h = 3;
		          else
		            date_h--;
		        }
		        else if (TimeSetCursor == 7)
		        {
		          if (date_l == 0)
		            date_l = 9;
		          else
		            date_l--;
		        }				
					}
					else
					{
						M41t00StopFlag = 1;  
						if (TimeSetCursor == 0)
						{
							if (hour_h == 0)
								hour_h = 2;
							else
								hour_h--;
						}
						else if (TimeSetCursor == 1)
						{
							if (hour_l == 0)
								hour_l = 9;
							else
								hour_l--;
						}
						else if (TimeSetCursor == 3)
						{
							if (minute_h == 0)
								minute_h = 6;
							else
								minute_h--;
						}
						else if (TimeSetCursor == 4)
						{
							if (minute_l == 0)
								minute_l = 9;
							else
								minute_l--;
						}
						else if (TimeSetCursor == 6)
						{
							if (second_h == 0)
								second_h = 6;
							else
								second_h--;
						}
						else if (TimeSetCursor == 7)
						{
							if (second_l < 9)
								second_l++;
							else
								second_l--;
						}
					}
				}
				else if (key_number == MoveUpKeyNum)
				{
					key_number = ResetKeyNum;
					SystemDataTimeSave_Flag=1;		
					
					if(SystemDataTimeSet_Flag==1)
					{
						M41t00StopFlag = 1;		
						if (TimeSetCursor == 0)
						{
							if (hour_h < 2)
								hour_h++;
							else
								hour_h = 0;
						}
						else if (TimeSetCursor == 1)
						{
							if (hour_l < 9)
								hour_l++;
							else
								hour_l = 0;
						}
						else if (TimeSetCursor == 3)
						{
							if (minute_h < 6)
								minute_h++;
							else
								minute_h = 0;
						}
						else if (TimeSetCursor == 4)
						{
							if (minute_l < 9)
								minute_l++;
							else
								minute_l = 0;
						}
						else if (TimeSetCursor == 6)
						{
							if (second_h < 6)
								second_h++;
							else
								second_h = 0;
						}
						else if (TimeSetCursor == 7)
						{
							if (second_l < 9)
								second_l++;
							else
								second_l = 0;
						}
					}			
					else
					{
						if (TimeSetCursor == 0)
						{
							if (year_h < 9)
								year_h++;
							else
								year_h = 0;
						}
						else if (TimeSetCursor == 1)
						{
							if (year_l < 9)
								year_l++;
							else
								year_l = 0;
						}
						else if (TimeSetCursor == 3)
						{
							if (month_h == 0)
								month_h = 1;
							else
								month_h = 0;
						}
						else if (TimeSetCursor == 4)
						{
							if (month_l < 9)
								month_l++;
							else
								month_l = 0;
						}
						else if (TimeSetCursor == 6)
						{
							if (date_h < 3)
								date_h++;
							else
								date_h = 0;
						}
						else if (TimeSetCursor == 7)
						{
							if (date_l < 9)
								date_l++;
							else
								date_l = 0;
						}
					}
				}			
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
static void	Switch_ThirdMenu_TyrePress(void)
{
	uchar i;
	if (menu_id == ThirdMenuNum)
  {
	 	if (SubMenu_Cursor == TyrePressMenuNum)///
    {	//胎压查询
			if(TyrePressMenuItem==0)
			{
				if (key_number == MenuTabKeyNum||key_number == EnterKeyNum)////move to next position
				{
					key_number = ResetKeyNum;
					menu_id = SecondMenuNum;
					Display_SubMenu_Flag=1;
				}
				else if(key_number == MoveDownKeyNum)
				{
					key_number = ResetKeyNum;
					TyrePressMenuItem=1;
					Display_ThirdMenu_TyrePress_Flag=1;
				}
				else if(key_number == MoveUpKeyNum)
				{
					key_number = ResetKeyNum;
					TyrePressMenuItem=2;
					Display_ThirdMenu_TyrePress_Flag=1;
				}				
			}//胎压监测
			else if(TyrePressMenuItem==1)
			{
				if (key_number == MenuTabKeyNum||key_number == EnterKeyNum)////move to next position
				{
					key_number = ResetKeyNum;
					menu_id = SecondMenuNum;
					Display_SubMenu_Flag=1;
					TyrePressUpDataCount=0;
				}
				else if(key_number ==MoveDownKeyNum )
				{
					key_number = ResetKeyNum;
					TyrePressMenuItem=2;
					Display_ThirdMenu_TyrePress_Flag=1;
				}
				else if(key_number ==MoveUpKeyNum )
				{
					key_number = ResetKeyNum;
					TyrePressMenuItem=0;
					Display_ThirdMenu_TyrePress_Flag=1;
				}				
			}//胎压学习密码输入
			else if(TyrePressMenuItem==2)
			{//未进入设置
				if(TyrePressMenuStatus==0xff)
				{
					if(key_number == MenuTabKeyNum)
					{
						key_number = ResetKeyNum;
						menu_id = SecondMenuNum;
						TyrePressPassWordErr_Flag=0;
						Display_SubMenu_Flag=1;
					}
					else if(key_number ==MoveDownKeyNum )
					{
						key_number = ResetKeyNum;
						TyrePressMenuItem=0;
						TyrePressPassWordErr_Flag=0;
						Display_ThirdMenu_TyrePress_Flag=1;
					}
					else if(key_number ==MoveUpKeyNum )
					{
						key_number = ResetKeyNum;
						TyrePressMenuItem=1;
						TyrePressPassWordErr_Flag=0;
						Display_ThirdMenu_TyrePress_Flag=1;
					}					
					else if(key_number == EnterKeyNum)
					{
						key_number = ResetKeyNum;
						TyrePressPassWordNum=0;
						TyrePressPassWord[0]=0;
						TyrePressPassWord[1]=0;
						TyrePressPassWord[2]=0;
						TyrePressPassWord[3]=0;						
						TyrePressSetSelChange_Flag=1;
						TyrePressPassWordErr_Flag=0;
						TyrePressMenuStatus=0;
					}
				}
				else
				{	//密码设置中
					if(key_number == MenuTabKeyNum)
					{
						key_number = ResetKeyNum;
						TyrePressPassWordNum++;
						if(TyrePressPassWordNum>3)
							TyrePressPassWordNum=0;
					}
					else if(key_number == MoveUpKeyNum)
					{
						key_number = ResetKeyNum;
						TyrePressPassWord[TyrePressPassWordNum]++;
						if(TyrePressPassWord[TyrePressPassWordNum]>0X0F)
							TyrePressPassWord[TyrePressPassWordNum]=0;
					}
					else if(key_number == MoveDownKeyNum)
					{
						key_number = ResetKeyNum;
						TyrePressPassWord[TyrePressPassWordNum]--;		
						if(TyrePressPassWord[TyrePressPassWordNum]==0xff)
							TyrePressPassWord[TyrePressPassWordNum]=0X0F;
					}					
					else if(key_number == EnterKeyNum)
					{
						key_number = ResetKeyNum;
						TyrePressPassWordErr_Flag=0;
						for(i=0;i<4;i++)
						{
							if(TyrePressPassWord[i]!=TyrePressPassWord_c[i])
								TyrePressPassWordErr_Flag=1;
						}
						if(TyrePressPassWordErr_Flag==0)
						{
							TyrePressMenuItem=3;		
							Display_ThirdMenu_TyrePress_Flag=1;
						}
						else
							TyrePressSetSelChange_Flag=1;
						TyrePressMenuStatus=0xff;
						
					}
				}
			}//胎压学习			
			else if(TyrePressMenuItem==3)
			{//未进入设置
				if(TyrePressMenuStatus==0xff)
				{
					if(key_number == MenuTabKeyNum)
					{
						key_number = ResetKeyNum;
						menu_id = SecondMenuNum;
						Display_SubMenu_Flag=1;
						for(i=0;i<TYRE_COUNT;i++)
						{
							if((TyreState[i]!=WAIT_SELECT)||(TyreState[i]!=TPMS_SUCCEED))
							{
								TPMS_CAN_LearnEndFlag=1;
								TPMS_CAN_LearnEnd_CT=5; 
							}
							TyreState[i]=WAIT_SELECT;	
						}
					}
					else if(key_number == MoveUpKeyNum)
					{
						key_number = ResetKeyNum;
						TyrePressMenuItem=0;
						Display_ThirdMenu_TyrePress_Flag=1;
						for(i=0;i<TYRE_COUNT;i++)
						{
							if((TyreState[i]!=WAIT_SELECT)||(TyreState[i]!=TPMS_SUCCEED))
							{
								TPMS_CAN_LearnEndFlag=1;
								TPMS_CAN_LearnEnd_CT=5; 
							}
							TyreState[i]=WAIT_SELECT;	
						}						
					}
					else if(key_number == MoveDownKeyNum)
					{
						key_number = ResetKeyNum;
						TyrePressMenuItem=1;
						Display_ThirdMenu_TyrePress_Flag=1;
						for(i=0;i<TYRE_COUNT;i++)
						{
							if((TyreState[i]!=WAIT_SELECT)||(TyreState[i]!=TPMS_SUCCEED))
							{
								TPMS_CAN_LearnEndFlag=1;
								TPMS_CAN_LearnEnd_CT=5; 
							}
							TyreState[i]=WAIT_SELECT;	
						}						
					}					
					else if(key_number == EnterKeyNum)
					{
						key_number = ResetKeyNum;
						TyrePressMenuStatus=0;
						TyrePressMenuLocate=0;
						TyrePressSetSelChange_Flag=1;
					}
				}
				else
				{	//设置中
					if(key_number == MenuTabKeyNum)
					{
						key_number = ResetKeyNum;
						if(TyrePressMenuStatus==0xff)
						{
							menu_id = SecondMenuNum;
							Display_SubMenu_Flag=1;
						}
					}
					else if(key_number == MoveUpKeyNum)
					{
						key_number = ResetKeyNum;
						TyrePressSetSelChange_Flag=1;
						TyrePressMenuStatus=0;
						TyrePressMenuLocate++;
						if(TyrePressMenuLocate>5)
							TyrePressMenuLocate=0;
					}
					else if(key_number == MoveDownKeyNum)
					{
						key_number = ResetKeyNum;
						TyrePressSetSelChange_Flag=1;
						TyrePressMenuStatus=0;
						TyrePressMenuLocate--;
						if(TyrePressMenuLocate==0xFF)
							TyrePressMenuLocate=5;						
					}					
					else if(key_number == EnterKeyNum)
					{
						key_number = ResetKeyNum;
						if(TyrePressMenuStatus==0)
							TyrePressMenuStatus=1;
						else 
							TyrePressMenuStatus=0xFF;
						TyrePressSetSelChange_Flag=1;
						if(TyrePressMenuStatus==1)
						{
							//添加数据发送
							TyreState[TyrePressMenuLocate]=WAIT_REPLY;
							TPMS_Learn_Time=0;// 学习时间清零
							TPMSPosition =TyrePressMenuLocate;
							TPMS_CAN_LearnEnd_CT=5;
							TX1939_TPMS_Flag=1;
						}
					}
				}
			}
	 	}
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、三级界面帮助信息
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Switch_ThirdMenu_HelpInfomation(void)
{
	if (menu_id == ThirdMenuNum)
	{
		if (SubMenu_Cursor == HelpInfomationMenuNum)
		{
			if (key_number == MenuTabKeyNum)////move to next position
			{
				key_number = ResetKeyNum;
				Display_ThirdMenu_HelpInfomation_Flag=1;
				VersionQuery_Flag^=1;
//				HelpInfomationPageNum=0;
			}
			else if (key_number == EnterKeyNum)
			{
				key_number = ResetKeyNum;		
				menu_id = SecondMenuNum;
				Display_SubMenu_Flag=1;
			}
			else if (key_number == MoveUpKeyNum)
			{
				key_number = ResetKeyNum;			
/*				if(VersionQuery_Flag==1)
				{	
					Display_ThirdMenu_HelpInfomation_Flag=1;
					HelpInfomationTurnPage_Flag=1;
					HelpInfomationPageNum--;
					if(HelpInfomationPageNum==0xff)
						HelpInfomationPageNum=1;
				}*/
			}
			else if (key_number == MoveDownKeyNum)
			{
				key_number = ResetKeyNum;				
/*				if(VersionQuery_Flag==1)
				{
					Display_ThirdMenu_HelpInfomation_Flag=1;
					HelpInfomationTurnPage_Flag=1;
					HelpInfomationPageNum++;
					if(HelpInfomationPageNum==2)
						HelpInfomationPageNum=0;
				}*/
			}
		}
	}
}
#pragma CODE_SEG DEFAULT


