#define	_NO_DriveKeyDeal_EXTERN
#include	"DriveKeyDeal.h"
/*****************************************************

******************************************************/
void	KeyDeal(void)
{

	ScanKeySensor();
	
  KeyChangeProcess();  

	KeyProcess();

}
/*****************************************************
 * Function:        74HC4051D	channel select
 *
 * call:    		  	None
 *
 * Input:           Channel4051;the channel will input
 *
 * Output:          None
 *
 * Note:            
*******************************************************/
void	Hc4051_ChannelSelect(unsigned char Num_Hc4051,unsigned char Channel4051)
{
	AN_MUX_A1_IO=1;
	AN_MUX_B1_IO=1;
	AN_MUX_C1_IO=1;	

	AN_MUX_A2_IO=1;
	AN_MUX_B2_IO=1;
	AN_MUX_C2_IO=1;

	AN_MUX_A3_IO=1;
	AN_MUX_B3_IO=1;
	AN_MUX_C3_IO=1;

	if(Num_Hc4051==Mcu_U24)
	{
		switch(Channel4051)
		{
  		case 0:                        
  		  AN_MUX_A2=0;AN_MUX_B2=0;AN_MUX_C2=0;  		  
  		  break;
  		case 1:
  		  AN_MUX_A2=1;AN_MUX_B2=0;AN_MUX_C2=0;  		  
  		  break;
  		case 2:
  		  AN_MUX_A2=0;AN_MUX_B2=1;AN_MUX_C2=0;  		  
  		  break;
  		case 3:
  		  AN_MUX_A2=1;AN_MUX_B2=1;AN_MUX_C2=0;  		  
  		  break;
  		case 4:
  		  AN_MUX_A2=0;AN_MUX_B2=0;AN_MUX_C2=1;
  		  break;
  		case 5:
  		  AN_MUX_A2=1;AN_MUX_B2=0;AN_MUX_C2=1;
  		  break;
  		case 6:
  		  AN_MUX_A2=0;AN_MUX_B2=1;AN_MUX_C2=1;
  		  break;
  		case 7:
  		  AN_MUX_A2=1;AN_MUX_B2=1;AN_MUX_C2=1;
  		  break;
  		default:
   	  	break;	  
  	} 
	}
	else if(Num_Hc4051==Mcu_U26)
	{
		switch(Channel4051)
		{
  		case 0:                        
  		  AN_MUX_A1=0;AN_MUX_B1=0;AN_MUX_C1=0;  		  
  		  break;
  		case 1:
  		  AN_MUX_A1=1;AN_MUX_B1=0;AN_MUX_C1=0;  		  
  		  break;
  		case 2:
  		  AN_MUX_A1=0;AN_MUX_B1=1;AN_MUX_C1=0;  		  
  		  break;
  		case 3:
  		  AN_MUX_A1=1;AN_MUX_B1=1;AN_MUX_C1=0;  		  
  		  break;
  		case 4:
  		  AN_MUX_A1=0;AN_MUX_B1=0;AN_MUX_C1=1;
  		  break;
  		case 5:
  		  AN_MUX_A1=1;AN_MUX_B1=0;AN_MUX_C1=1;
  		  break;
  		case 6:
  		  AN_MUX_A1=0;AN_MUX_B1=1;AN_MUX_C1=1;
  		  break;
  		case 7:
  		  AN_MUX_A1=1;AN_MUX_B1=1;AN_MUX_C1=1;
  		  break;
  		default:
   	  	break;	  
  	} 
	} 	
	else if(Num_Hc4051==Mcu_U13)
	{
		switch(Channel4051)
		{
  		case 0:                        
  		  AN_MUX_A3=0;AN_MUX_B3=0;AN_MUX_C3=0;  		  
  		  break;
  		case 1:
  		  AN_MUX_A3=1;AN_MUX_B3=0;AN_MUX_C3=0;  		  
  		  break;
  		case 2:
  		  AN_MUX_A3=0;AN_MUX_B3=1;AN_MUX_C3=0;  		  
  		  break;
  		case 3:
  		  AN_MUX_A3=1;AN_MUX_B3=1;AN_MUX_C3=0;  		  
  		  break;
  		case 4:
  		  AN_MUX_A3=0;AN_MUX_B3=0;AN_MUX_C3=1;
  		  break;
  		case 5:
  		  AN_MUX_A3=1;AN_MUX_B3=0;AN_MUX_C3=1;
  		  break;
  		case 6:
  		  AN_MUX_A3=0;AN_MUX_B3=1;AN_MUX_C3=1;
  		  break;
  		case 7:
  		  AN_MUX_A3=1;AN_MUX_B3=1;AN_MUX_C3=1;
  		  break;
  		default:
   	  	break;	  
  	} 
	}	
}
/**********************************************
 * Function:        Adc	channel select
 *
 * call:    		  		None
 *
 * Input:           AdcChannel;the channel will input
 *
 * Output:          None
 *
 * Note:            
**********************************************/
void	ADC_ChannelSelect(unsigned	char	AdcChannel)
{
	unsigned char AdcDelayTime;

	ADCSC1_ADCO=0;										////Conversion Disable 
  ADCSC1_ADCH=AdcChannel;					////select channel          
  asm nop;
  ADCSC1_ADCO=1;										////Conversion Enable
  ADresult.result=0;
  AdcDelayTime=0;
  while(!ADCSC1_COCO)								///wait adc complete
  {
  	AdcDelayTime++;
  	if(AdcDelayTime>=ADC_WAIT_TIME)
  		goto _AdcOut;								///if not adc ack,go to out
  }  		  	
	ADCSC1_ADCO=0;										///Conversion Disable
  ADresult.result=ADCRL;
  ADCSC1_ADCH=0x00;
_AdcOut:
	asm nop;
}
/**********************************************
 * Function:        Scan key and sensor
 *
 * call:    		  		None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Note:            
**********************************************/
static void ScanKeySensor(void)
{
	unsigned	char	i,j;
	///IO Direction 
	Sensor_Power_IO=1;
	V_Power_IO=1;

	if(SleepStartFlags==0)
	{
		Sensor_Power=1;														///Open Adc Power
		V_Power=1;
	}
	else 
	{
		V_Power=0;										///09.11.24 modify
		
		Sensor_Power=0;
		
		Time_SendSensor=0;							///be sure that not to send key at the moment of wake up
  	DriveSendSensorFlag=0;	
		for(i=0;i<2;i++)								///when sleep,clear all key state to prevent opening the load at the moment of wake up
		{
			FrontKeyData[i].Data=0x00;
			BackKeyData[i].Data=0x00;
			TopKeyData[i].Data=0x00;
			OldKeyData[i].Data=0xff;
		}
		if(BackDriveFlag==1)
		{
			OldKeyData[0].b.Key6=0;			///engine room open key is key 7
			BackRoomKeyBit=KeyOn;
		}
		for(i=0;i<5;i++)
			MeterKeyData[i].Data=0x00;
	}
	/********************24 channel adc*********************/	
	for(i=0;i<24;i++)
	{		
		if(i==1)
		{
			for(j=0;j<8;j++)
			{
				Hc4051_ChannelSelect(Mcu_U24,j);	
				ADC_ChannelSelect(i);
				/**************hc4051 adc result****************/
				Hc4051ChannelAdcData[j]=(unsigned char)(ADresult.result);
				NormAdcData1=Hc4051ChannelAdcData[7]; ////obtain norm adc data1	
			}
		}
		else if(i==10)
		{
			if(SleepStartFlags==1)///09.11.24 add to save power of system
				goto	_SleepJump;
			for(j=0;j<8;j++)
			{
				Hc4051_ChannelSelect(Mcu_U26,j);	
				ADC_ChannelSelect(i);
				/**************hc4051 adc result****************/
				Hc4051ChannelAdcData[j+8]=(unsigned char)(ADresult.result);
			}
			_SleepJump:
				asm nop;
		}		
		else if(i==6)			
		{
			ADC_ChannelSelect(i);
			SensorData[0]=(unsigned char)(ADresult.result);	
		}	
		else if(i==13)			
		{
			ADC_ChannelSelect(i);
			SensorData[1]=(unsigned char)(ADresult.result);	
		}	
		else if(i==5)			
		{
			ADC_ChannelSelect(i);
			SensorData[2]=(unsigned char)(ADresult.result);	
		}	
		else if(i==20)			
		{
			ADC_ChannelSelect(i);
			SensorData[3]=(unsigned char)(ADresult.result);	
		}	
		else if(i==12)			
		{
			ADC_ChannelSelect(i);
			SensorData[4]=(unsigned char)(ADresult.result);	
		}	
		else if(i==4)			
		{
			ADC_ChannelSelect(i);
			SensorData[5]=(unsigned char)(ADresult.result);	
		}	
		else if(i==23)
		{
			ADC_ChannelSelect(i);
			SensorData[6]=(unsigned char)(ADresult.result);	
		}	
		else if(i==14)
		{
			ADC_ChannelSelect(i);
			StAdcData[0]=(unsigned char)(ADresult.result);	
		}
		else if(i==21)
		{
			ADC_ChannelSelect(i);
			StAdcData[1]=(unsigned char)(ADresult.result);	
		}
	}		
}
/***********************************************
 * Function:        Key Data Change to realy,base to the principle map
 *
 * call:    		  		none
 *
 * Input:           None
 *
 * Output:          None
 *
 * Note:
************************************************/
static void KeyChangeProcess(void)
{
	/*Hc4051ChannelAdcData change to realy*/
	LastHc4051ChannelAdcData[0]=Hc4051ChannelAdcData[0];	
	LastHc4051ChannelAdcData[1]=Hc4051ChannelAdcData[1];
	LastHc4051ChannelAdcData[2]=Hc4051ChannelAdcData[2];
	LastHc4051ChannelAdcData[3]=Hc4051ChannelAdcData[3];
	LastHc4051ChannelAdcData[4]=Hc4051ChannelAdcData[4];
	LastHc4051ChannelAdcData[5]=Hc4051ChannelAdcData[5];
	LastHc4051ChannelAdcData[6]=Hc4051ChannelAdcData[6];
	
	LastHc4051ChannelAdcData[7]=Hc4051ChannelAdcData[8];	
	LastHc4051ChannelAdcData[8]=Hc4051ChannelAdcData[9];
	LastHc4051ChannelAdcData[9]=Hc4051ChannelAdcData[10];
	LastHc4051ChannelAdcData[10]=Hc4051ChannelAdcData[11];
	LastHc4051ChannelAdcData[11]=Hc4051ChannelAdcData[12];
	LastHc4051ChannelAdcData[12]=Hc4051ChannelAdcData[13];
	LastHc4051ChannelAdcData[13]=Hc4051ChannelAdcData[14];
	LastHc4051ChannelAdcData[14]=Hc4051ChannelAdcData[15];
}
/******************************************

*******************************************/
static void KeyProcess(void)
{
	unsigned char i,j,x,y,z;
  		
   j=0;
   x=0;
   y=0;
   z=0;
   for(i=0;i<16;i++)
   {
   		if(i<7)
   		{
   			if(LastHc4051ChannelAdcData[i]<LowKey)   			
				{					
   				if(!((SwitchConfigure[z].Data>>j)&0x01))
   					KeyData[x].Data&=~(0x01<<y);
   				else
   					KeyData[x].Data|=0x01<<y;
   			}
   			else if(LastHc4051ChannelAdcData[i]>HighKey)
   			{
   				if((SwitchConfigure[z].Data>>j)&0x01)
   					KeyData[x].Data&=~(0x01<<y);  
   				else
   					KeyData[x].Data|=0x01<<y; 				
   			}
   			else
   				KeyData[x].Data|=0x01<<y; 
   		}
			else	
   		{
   			if(LastHc4051ChannelAdcData[i]<LowKey1)   			
				{					
   				if(!((SwitchConfigure[z].Data>>j)&0x01))
   					KeyData[x].Data&=~(0x01<<y);
   				else
   					KeyData[x].Data|=0x01<<y;
   			}
   			else
   				KeyData[x].Data|=0x01<<y; 		
   		}
		j++;   			
		if(j>=8)			///8 bit is a SwitchConfigure[].Data byte
		{
			j=0;		
			z++;
		}
		y++;
		if(y>=8)			///8 bit is a KeyData[].Data byte
		{
			y=0;
			x++;
		}
  }

	if(KeyAdcFlag==1)  
 	{
	 	KeyAdcFlag=0;
		KeyAdcTime=0;
	   AdcNumber++;		
	   if(AdcNumber==1)
	   {
	   		for(i=0;i<2;i++)
	   			FirstKeyData[i]=KeyData[i].Data;
	   		goto _KeyProcessOut;			///if first,not enter underside
	   }
	   if(AdcNumber==2)
	   {
	   		for(i=0;i<2;i++)
	   			SecondKeyData[i]=KeyData[i].Data;
	   		AdcNumber=0;	///restart the next adc
	   }
	   
	   for(i=0;i<2;i++)
	   {
	   		if(FirstKeyData[i]!=SecondKeyData[i])
					goto	_KeyProcessOut;
	   }
		 /*judge if Key data need to change and send immediately */
   		for(i=0;i<2;i++)	
	   {
	   		if(OldKeyData[i].Data!=SecondKeyData[i])
	   		{
	   			OldKeyData[i].Data=SecondKeyData[i];
				if(FrontDriveFlag==1)
				{
					FrontKeyData[i].Data=~OldKeyData[i].Data;
				}
				else if(BackDriveFlag==1)
				{
					BackKeyData[i].Data=~OldKeyData[i].Data;
//	        if((!BackRoomKeyBit)&&(SleepStartFlags==1))
//	         	SleepStartFlags=0;
				}
				else if(TopDriveFlag==1)
				{
					TopKeyData[i].Data=~OldKeyData[i].Data;
//					if((BackDoorPreventNipKeyBit||FrontDoorPreventNipKeyBit)&&(SleepStartFlags==1))
//						SleepStartFlags=0;						
				}
	   			KeyChangFlag=1;
	   		}
	   }   
	}
	_KeyProcessOut:
	asm nop;
}   
/************************************************************************************************

*************************************************************************************************/
///configure switch macros
void	SwitchLogicConfigure(void)
{
	if(FrontDriveFlag==1)											
	{
		SwitchConfigure[0].Data=0b01011101;		
		SwitchConfigure[1].Data=0b00000000;
	}
	else if(BackDriveFlag==1)
	{
		SwitchConfigure[0].Data=0b00000000;
		SwitchConfigure[1].Data=0b00000000;
	}
	else if(TopDriveFlag==1)
	{
		SwitchConfigure[0].Data=0b00000101;
		SwitchConfigure[1].Data=0b00000000;
	}
}
