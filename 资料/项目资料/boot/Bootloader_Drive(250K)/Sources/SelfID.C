#define		_NO_SelfID_EXTERN
#include	"SelfID.h"
#include	"Bootloader.h"
/*========================================================
 *brief:    ADC initialization.
 *author:   yao qing rui
 *input:    void
 *output:   void
========================================================*/ 
void	ADC_Init(void)
{
	ADCCFG = 0x91;			/*bit7->Low Power Configuration;bit6.5->Clock Divide Select;bit4->Long Sample Time Configuration;
										bit3.2->Conversion Mode Selection:10bit adc;bit1.0->Input Clock Select:bus clock/2*/							
  APCTL1=0b01110110;	/*bit1.2.4.5.6 is select to adc pin;other is common io*/
  APCTL2=0b01110100;	/*bit1.2.4.5 is select to adc pin;other is common io*/
  APCTL3=0b10110000;	/*bit4.7 is select to adc pin;other is common io*/
}
/**********************************************
 * Function:        74HC4051D	channel select
 *
 * call:    		  	None
 *
 * Input:           Channel4051;the channel will input
 *
 * Output:          None
 *
 * Note:            
**********************************************/
static void	Hc4051_ChannelSelect(unsigned char Num_Hc4051,unsigned char Channel4051)
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
 * call:    		  	None
 *
 * Input:           AdcChannel;the channel will input
 *
 * Output:          None
 *
 * Note:            
**********************************************/
static void	ADC_ChannelSelect(unsigned	char	AdcChannel)
{	
	ADCSC1_ADCO=0;						////Conversion Disable 
  ADCSC1_ADCH=AdcChannel;	////select channel          
  asm nop;
  ADCSC1_ADCO=1;						////Conversion Enable
  ADresult=0;
  while(!ADCSC1_COCO);			///wait adc complete
  
  ADCSC1_ADCO=0;						////Conversion Disable
  ADresult=ADCRL;
  ADCSC1_ADCH=0x00;
}
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
	unsigned	int	AdcWaitTime;
	unsigned char	FirstSelfID=0,SecondSelfID=0;
	///adc io direction
	V_Power_IO=1;

	V_Power=1;																					///open v power then start to detect adc
	
	for(AdcWaitTime=0;AdcWaitTime<4000;AdcWaitTime++)	///delay for adc module stabilization
	{
		asm nop;
		asm nop;
	}
	_SelfIdTestStart:
	for(j=0;j<2;j++)
	{
		for(i=0;i<8;i++)							///09.12.24 add,because the norm ad is change to another address
		{
			Hc4051_ChannelSelect(Mcu_U24,i);	
			ADC_ChannelSelect(NORM_AD_NUM);
			Hc4051ChannelAdcData[i]=ADresult;
			NormAdcData=Hc4051ChannelAdcData[7]; 
		}
		for(i=0;i<4;i++)
		{
			Hc4051_ChannelSelect(Mcu_U13,i);	
			ADC_ChannelSelect(ADDR_AD_NUM);
			Hc4051ChannelAdcData[i+8]=ADresult;
		}
		if(Hc4051ChannelAdcData[3+8]<LowKey)	    	
    	TempSelfID.b.b0=0;       	
  	else if(Hc4051ChannelAdcData[3+8]>HighKey)   		
  		TempSelfID.b.b0=1;   	
		else																			///if it is hang in the air,the module is no used
		{
			goto	_SelfIdTestStart;								///09.09.10 modify from "break to goto _SelfIdTestStart"to resolve "be regard to front drive ever"
		}
		if(Hc4051ChannelAdcData[0+8]<LowKey)	   		
    	TempSelfID.b.b1=0;     
  	else if(Hc4051ChannelAdcData[0+8]>HighKey)    		
  		TempSelfID.b.b1=1;    		  			
		else																			///if it is hang in the air,the module is no used
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
	}
	else if((SelfID.b.b0==1)&&(SelfID.b.b1==0))
	{
		BackDriveFlag=1;
	}
	else
	{
		TopDriveFlag=1;
	}
}

