#define		_NO_SelfID_EXTERN
#include	"SelfID.h"
/**********************************************
 * Function:      Test self ID
 *
 * call:    		  None
 *
 * Input:         None
 *
 * Output:        None
 *
 * Note:            
**********************************************/
void	TestSelfID(void)
{
	unsigned	char	i,j;
	unsigned char	FirstSelfID=0,SecondSelfID=0;
	///adc io direction
	V_Power_IO=1;

	V_Power=1;												///open v power then start to detect adc
	delay_us(AD_ENABLE_DELAY_NUM);		///delay for adc module stabilization
	_SelfIdTestStart:
	for(j=0;j<2;j++)
	{
		for(i=0;i<8;i++)								///09.12.24 add;because norm ad is change to another address
		{
			Hc4051_ChannelSelect(Mcu_U24,i);	
			ADC_ChannelSelect(1);
			Hc4051ChannelAdcData[i]=(unsigned char)(ADresult.result);
			NormAdcData=Hc4051ChannelAdcData[7]; 
			
		}
		for(i=0;i<4;i++)
		{
			Hc4051_ChannelSelect(Mcu_U13,i);	
			ADC_ChannelSelect(2);
			/**************hc4051 adc result****************/
			Hc4051ChannelAdcData[i+16]=(unsigned char)(ADresult.result);
		}
		if(Hc4051ChannelAdcData[19]<ModuleCheckLowKey)	    	
    	TempSelfID.b.b0=0;       	
  	else if(Hc4051ChannelAdcData[19]>ModuleCheckHighKey)   		
  		TempSelfID.b.b0=1;   	
		else						///if it is hang in the air,the module is no used
		{
			goto	_SelfIdTestStart;
		}
		if(Hc4051ChannelAdcData[16]<ModuleCheckLowKey)	   		
    	TempSelfID.b.b1=0;     
  	else if(Hc4051ChannelAdcData[16]>ModuleCheckHighKey)    		
  		TempSelfID.b.b1=1;    		  			
		else						///if it is hang in the air,the module is no used
		{
			goto	_SelfIdTestStart;
		}
		TempSelfID.Data&=0x03;
		if(j==0)
			FirstSelfID=TempSelfID.Data;
		else if(j==1)
		{
			SecondSelfID=TempSelfID.Data;
			if(SecondSelfID!=FirstSelfID)
			{
				TempSelfID.Data=0;
				FirstSelfID=0;
				SecondSelfID=0;
				goto	_SelfIdTestStart;
			}
			else
			{
				SelfID.Data=SecondSelfID;
				break;
			}
		}
	}
	FrontDriveFlag=0;
	BackDriveFlag=0;	
	TopDriveFlag=0;
	if((SelfID.b.b0==0)&&(SelfID.b.b1==0))		
	{
		FrontDriveFlag=1;
		BackDriveFlag=0;	
		TopDriveFlag=0;	
	}
	else if((SelfID.b.b0==1)&&(SelfID.b.b1==0))
	{
		FrontDriveFlag=0;
		BackDriveFlag=1;
		TopDriveFlag=0;
	}
	else
	{
		FrontDriveFlag=0;
		BackDriveFlag=0;
		TopDriveFlag=1;
	}
}


