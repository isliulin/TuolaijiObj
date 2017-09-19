#define		_NO_LoadDeal_EXTERN
#include	"LoadDeal.h"
/*************************************************************************************************************

**************************************************************************************************************/
void	LoadDeal(void)
{		
  	ErrorReset(); 

	Module_Check();

	if(FrontDriveFlag==1)///bak flash send only used in front drive
		FlashLampDeal();
	 
  	StTest(); 

	LoadStateMask();
      	
	if(ChangeData()==1) 
	{
		unsigned char	i;

		for(i=0;i<3;i++)
			Out595(i,Open_All); 	
	 }

	 if(BackDriveFlag==1)
	 {
   		MotorReStartDetect();
   		MotorStartTooLongDetect();
	 }
}
/**********************************************
 * Function:        Get BTS724G st pin electric 
 *
 * call:    		  	none
 *
 * Input:           None
 *
 * Output:          None
 *
 * Note:            
**********************************************/
static void	GetSTProcess(unsigned char	GetStState)
{
	unsigned char	j,TempDataA,TempDataB;
	/*IO Direction Macros*/
	HC165_CLOCK_IO=1;
	HC165_LOAD_IO=1;
	HC165_DATA1_IO=0;
	HC165_DATA2_IO=0;

	St17_IO=0;
	St18_IO=0;

	/*hc165 st detect*/
	if(GetStState==StState0||GetStState==StStateAll)	
	{
		HC165_CLOCK=0;  
		HC165_LOAD=0;
   		asm nop;
		asm nop;
   		HC165_LOAD=1; 		
		StData[0].Data=0;
		TempDataB=0;
		TempDataA=0;
		for(j=0;j<8;j++)
		{
			HC165_CLOCK=0;
			TempDataA=HC165_DATA1;
			TempDataB|=TempDataA&0x01;
			if(j<7)
				TempDataB<<=1;
			TempDataA=0;
			HC165_CLOCK=1;
			asm nop;
			asm nop;
		}
		StData[0].Data=TempDataB;	
	}
	if(GetStState==StState1||GetStState==StStateAll)	
	{
		HC165_CLOCK=0;  
		HC165_LOAD=0;
   		asm nop;
		asm nop;
   		HC165_LOAD=1; 		
		StData[1].Data=0;
		TempDataB=0;
		TempDataA=0;
		for(j=0;j<8;j++)
		{
			HC165_CLOCK=0;
			TempDataA=HC165_DATA2;
			TempDataB|=TempDataA&0x01;
			if(j<7)
				TempDataB<<=1;
			TempDataA=0;
			HC165_CLOCK=1;
			asm nop;
			asm nop;
		}
		StData[1].Data=TempDataB;	
	}
	if(GetStState==StState2||GetStState==StStateAll)
	{
		ADC_ChannelSelect(14);		///10.06.02 add to resolve "can't detect load on and open state" 
		StAdcData[0]=(unsigned char)(ADresult.result);	

		ADC_ChannelSelect(21);
		StAdcData[1]=(unsigned char)(ADresult.result);	
		
		if(St17>StNormData)
			ST17Detcet=1;
		else 
			ST17Detcet=0;
		if(St18>StNormData)
			ST18Detcet=1;
		else 
			ST18Detcet=0;
		
		ST19Detcet=1;
		ST20Detcet=1;
		ST21Detcet=1;
		ST22Detcet=1;
		ST23Detcet=1;
		ST24Detcet=1;
	}
}
/*************************************************************************************************************

**************************************************************************************************************/
static void	OutputLoadChange(void)
{
	/*putdata[0](bit0.1.2.3) and putdata[2](bit0.1) change to realy*/
	LastPutData[0].Loads.Load0=0;
	LastPutData[0].Loads.Load1=0;
	LastPutData[0].Loads.Load2=PutData[0].Loads.Load0;
	LastPutData[0].Loads.Load3=PutData[0].Loads.Load1;
	LastPutData[0].Loads.Load4=PutData[0].Loads.Load2;
	LastPutData[0].Loads.Load5=PutData[0].Loads.Load3;
	LastPutData[0].Loads.Load6=PutData[2].Loads.Load1;
	LastPutData[0].Loads.Load7=PutData[2].Loads.Load0;	
	/*putdata[1] bit change to realy*/
	LastPutData[1].Loads.Load0=PutData[1].Loads.Load0;
	LastPutData[1].Loads.Load1=PutData[1].Loads.Load1;
	LastPutData[1].Loads.Load2=PutData[1].Loads.Load2;
	LastPutData[1].Loads.Load3=PutData[1].Loads.Load3;
	LastPutData[1].Loads.Load4=PutData[1].Loads.Load4;
	LastPutData[1].Loads.Load5=PutData[1].Loads.Load5;
	LastPutData[1].Loads.Load6=PutData[1].Loads.Load6;
	LastPutData[1].Loads.Load7=PutData[1].Loads.Load7;
	/*putdata[0](bit4.5.6.7) bit change to realy*/
	LastPutData[2].Loads.Load0=PutData[0].Loads.Load4;
	LastPutData[2].Loads.Load1=PutData[0].Loads.Load5;
	LastPutData[2].Loads.Load2=PutData[0].Loads.Load6;
	LastPutData[2].Loads.Load3=PutData[0].Loads.Load7;
	LastPutData[2].Loads.Load4=0;
	LastPutData[2].Loads.Load5=0;
	LastPutData[2].Loads.Load6=0;
	LastPutData[2].Loads.Load7=0;
}
/********************************************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、对是否开路检测做配置
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、根据电气逻辑表的设定需要，在每次软件制作时，只需要配置一次即可。
2、需要开路检测，将其位置0
*******************************************************************************************************/
StStruct	LoadOpenDetectBit[3];

#define		Load_1_OpenDetectBit			LoadOpenDetectBit[0].b.ST0
#define		Load_2_OpenDetectBit			LoadOpenDetectBit[0].b.ST1
#define		Load_3_OpenDetectBit			LoadOpenDetectBit[0].b.ST2
#define		Load_4_OpenDetectBit			LoadOpenDetectBit[0].b.ST3
#define		Load_5_OpenDetectBit			LoadOpenDetectBit[0].b.ST4
#define		Load_6_OpenDetectBit			LoadOpenDetectBit[0].b.ST5
#define		Load_7_OpenDetectBit			LoadOpenDetectBit[0].b.ST6
#define		Load_8_OpenDetectBit			LoadOpenDetectBit[0].b.ST7

#define		Load_9_OpenDetectBit			LoadOpenDetectBit[1].b.ST0
#define		Load_10_OpenDetectBit		LoadOpenDetectBit[1].b.ST1
#define		Load_11_OpenDetectBit		LoadOpenDetectBit[1].b.ST2
#define		Load_12_OpenDetectBit		LoadOpenDetectBit[1].b.ST3
#define		Load_13_OpenDetectBit		LoadOpenDetectBit[1].b.ST4
#define		Load_14_OpenDetectBit		LoadOpenDetectBit[1].b.ST5
#define		Load_15_OpenDetectBit		LoadOpenDetectBit[1].b.ST6
#define		Load_16_OpenDetectBit		LoadOpenDetectBit[1].b.ST7

#define		Load_17_OpenDetectBit		LoadOpenDetectBit[2].b.ST0
#define		Load_18_OpenDetectBit		LoadOpenDetectBit[2].b.ST1
#define		Load_19_OpenDetectBit		LoadOpenDetectBit[2].b.ST2
#define		Load_20_OpenDetectBit		LoadOpenDetectBit[2].b.ST3
#define		Load_21_OpenDetectBit		LoadOpenDetectBit[2].b.ST4
#define		Load_22_OpenDetectBit		LoadOpenDetectBit[2].b.ST5
#define		Load_23_OpenDetectBit		LoadOpenDetectBit[2].b.ST6
#define		Load_24_OpenDetectBit		LoadOpenDetectBit[2].b.ST7

void	LoadOpenDetect(void)
{
	///0表示需要做开路检测
	if(FrontDriveFlag==1)
	{
		///1~8负载现为3A输出,没有做开路检测
		Load_1_OpenDetectBit		=1;
		Load_2_OpenDetectBit		=1;
		Load_3_OpenDetectBit		=1;
		Load_4_OpenDetectBit		=1;
		Load_5_OpenDetectBit		=1;
		Load_6_OpenDetectBit		=1;
		Load_7_OpenDetectBit		=1;
		Load_8_OpenDetectBit		=1;
		///9~16为5A输出，可根据需要做开路检测  
		Load_9_OpenDetectBit		=0;
		Load_10_OpenDetectBit 	=0;
		Load_11_OpenDetectBit 	=1;
		Load_12_OpenDetectBit 	=1;
		Load_13_OpenDetectBit 	=1;
		Load_14_OpenDetectBit 	=1;
		Load_15_OpenDetectBit 	=1;
		Load_16_OpenDetectBit 	=1;
		///17~18为5A输出，可根据需要做开路检测    
		Load_17_OpenDetectBit 	=1;
		Load_18_OpenDetectBit 	=1;
		///19~24没有使用
		Load_19_OpenDetectBit 	=1;
		Load_20_OpenDetectBit 	=1;
		Load_21_OpenDetectBit 	=1;
		Load_22_OpenDetectBit 	=1;
		Load_23_OpenDetectBit 	=1;
		Load_24_OpenDetectBit 	=1;
	}
	else if(BackDriveFlag==1)
	{
		///1~8负载现为3A输出,没有做开路检测
		Load_1_OpenDetectBit		=1;
		Load_2_OpenDetectBit		=1;
		Load_3_OpenDetectBit		=1;
		Load_4_OpenDetectBit		=1;
		Load_5_OpenDetectBit		=1;
		Load_6_OpenDetectBit		=1;
		Load_7_OpenDetectBit		=1;
		Load_8_OpenDetectBit		=1;
		///9~16为5A输出，可根据需要做开路检测  
		Load_9_OpenDetectBit		=1;
		Load_10_OpenDetectBit 	=1;
		Load_11_OpenDetectBit 	=1;
		Load_12_OpenDetectBit 	=1;
		Load_13_OpenDetectBit 	=1;
		Load_14_OpenDetectBit 	=1;
		Load_15_OpenDetectBit 	=0;
		Load_16_OpenDetectBit 	=0;
		///17~18为5A输出，可根据需要做开路检测  
		Load_17_OpenDetectBit 	=1;
		Load_18_OpenDetectBit 	=1;
		///19~24没有使用
		Load_19_OpenDetectBit 	=1;
		Load_20_OpenDetectBit 	=1;
		Load_21_OpenDetectBit 	=1;
		Load_22_OpenDetectBit 	=1;
		Load_23_OpenDetectBit 	=1;
		Load_24_OpenDetectBit 	=1;
	}
	else if(TopDriveFlag==1)
	{
		///1~8负载现为3A输出,没有做开路检测
		Load_1_OpenDetectBit		=1;
		Load_2_OpenDetectBit		=1;
		Load_3_OpenDetectBit		=1;
		Load_4_OpenDetectBit		=1;
		Load_5_OpenDetectBit		=1;
		Load_6_OpenDetectBit		=1;
		Load_7_OpenDetectBit		=1;
		Load_8_OpenDetectBit		=1;
		///9~16为5A输出，可根据需要做开路检测  
		Load_9_OpenDetectBit	=1;
		Load_10_OpenDetectBit 	=1;
		Load_11_OpenDetectBit 	=1;
		Load_12_OpenDetectBit 	=1;
		Load_13_OpenDetectBit 	=1;
		Load_14_OpenDetectBit 	=1;
		Load_15_OpenDetectBit 	=1;
		Load_16_OpenDetectBit 	=1;
		///17~18为5A输出，可根据需要做开路检测  
		Load_17_OpenDetectBit 	=1;
		Load_18_OpenDetectBit 	=1;
		///19~24没有使用
		Load_19_OpenDetectBit 	=1;
		Load_20_OpenDetectBit 	=1;
		Load_21_OpenDetectBit 	=1;
		Load_22_OpenDetectBit 	=1;
		Load_23_OpenDetectBit 	=1;
		Load_24_OpenDetectBit 	=1;
	}
}
/*******************************************************************************************************
 * Function:   output 74hc595 data to BTS724G IN pin
 *
 * call:    	 None
 *
 * Input:      None
 *
 * Output:     None
 *
 * Note:            
*******************************************************************************************************/
void	Out595(unsigned char	Number595,unsigned	char	Kind)
{
	unsigned char	j;
	StStruct	TempDataA;
	unsigned char	TempDataB=0;
	
	/*IO macros*/
	HC595_CLOCK_IO=1;	
	HC595_LOAD0_IO=1;	
	HC595_DATA0_IO=1;	
	HC595_LOAD1_IO=1;	
	HC595_DATA1_IO=1;		
	HC595_OE_IO=1;

	OutputLoadChange();
	
	/*load output control*/				
	HC595_OE=0;					///if data transmit,oe pin must be set to low

	TempDataA.Data=0b00000000;
	if(Number595==0)
	{
		HC595_LOAD0=0;
		HC595_CLOCK=0;
		if(Kind==Close_All)
			TempDataA.Data=0;
		else if(Kind==Close_First595)
		{
			if(CloseFrontFlag==0)
			{
				TempDataA.Data=LastPutData[0].Load&LoadOpenDetectBit[0].Data;///2~5bit对应1.2.3.4路
			}
			else 
			{
				TempDataB=0b11111111;///2~5bit对应1.2.3.4路；
				if(Load_17_OpenDetectBit==0)///8对应17路；
					TempDataB&=0b01111111;
				if(Load_18_OpenDetectBit==0)///7对应18；
					TempDataB&=0b10111111;
				TempDataA.Data=LastPutData[0].Load&TempDataB;
			}
		}
		else
			TempDataA.Data=LastPutData[0].Load;
		for(j=0;j<8;j++)
		{
			TempDataB=((TempDataA.Data&0x80)>>7&0x01);
      if(TempDataB==1)
      	HC595_DATA0=1;		///transmit 595 data
      else
      	HC595_DATA0=0;
      TempDataA.Data=TempDataA.Data<<1;
			HC595_CLOCK=1;
			asm nop;
			HC595_CLOCK=0;
		}	
		HC595_LOAD0=1;				///lock 595
	}

	if(Number595==1)
	{
		HC595_LOAD1=0;
		HC595_CLOCK=0;
		if(Kind==Close_All)
			TempDataA.Data=0;
		if(Kind==Close_Second595)
		{
			TempDataA.Data=LastPutData[1].Load&LoadOpenDetectBit[1].Data;///0~7bit对应9/10/11/12/13/14/15/16路
		}
		else
			TempDataA.Data=LastPutData[1].Load;
		for(j=0;j<8;j++)
		{
			TempDataB=((TempDataA.Data&0x80)>>7&0x01);
      if(TempDataB==1)
      	HC595_DATA1=1;	///transmit 595 data
      else
      	HC595_DATA1=0;
      TempDataA.Data=TempDataA.Data<<1;
			HC595_CLOCK=1;
			asm nop;
			HC595_CLOCK=0;
		}	
		HC595_LOAD1=1;		///lock 595
	}
	if(Number595==2)
	{
		if(Kind==Close_All)
		{
			Bt724In5=0;
			Bt724In6=0;
			Bt724In7=0;
			Bt724In8=0;
			return;
		}
		else if(Kind==Close_Third595)
		{
			if(Load_5_OpenDetectBit==0)
				Bt724In5=0;
			if(Load_6_OpenDetectBit==0)
				Bt724In6=0;
			if(Load_7_OpenDetectBit==0)
				Bt724In7=0;
			if(Load_8_OpenDetectBit==0)
				Bt724In8=0;
		}
		else
		{
			if(DLoad5)
				Bt724In5=1;
			else
				Bt724In5=0;
			if(DLoad6)
				Bt724In6=1;
			else
				Bt724In6=0;
			if(DLoad7)
				Bt724In7=1;
			else
				Bt724In7=0;
			if(DLoad8)
				Bt724In8=1;
			else
				Bt724In8=0;
		}
	}
}
/**********************************************
 * Function:        ST TEST
 *
 * call:    		  	none
 *
 * Input:           None
 *
 * Output:          None
 *
 * Note:            
**********************************************/
static void StTest(void)
{
	unsigned	char	Sti,Stj,Stx,Sty,FData,PData,SData;		

	OnCheckOpen=0;
	if(StDetectOpenFlag==1)
	{
		StDetectOpenFlag=0;
		for(Sti=0;Sti<3;Sti++)
		{
			if(PutData[Sti].Load!=0)		///load open to test open or not
			{
				if(Sti==0)
				{
					CloseFrontFlag=0;				///clear the flag mean that it will close load 1.2.3.4
					Out595(Sti,Sti+2);			///load 1.2.3.4 is in out595[0] 
					Out595(Sti+2,Sti+4);		///load 5.6.7.8 is in out595[2]
				}
				else if(Sti==1)
					Out595(Sti,Sti+2);	
				else if(Sti==2)
				{
					CloseFrontFlag=1;					///set the flag mean that it will close load 17.18
					Out595(Sti-2,Sti);				///load 17.18 is in out595[0]
				}
				OnCheckOpen=1;				
				delay_us(OnOpenClose_Num);		
				GetSTProcess(Sti+2);	
				if(Sti==0)
				{
					Out595(Sti,Open_All);
					Out595(Sti+2,Open_All);
				}
				else if(Sti==1)
					Out595(Sti,Open_All);
				else if(Sti==2)
					Out595(Sti-2,Open_All);
			}	
		}
	}
	
	if(OnCheckOpen==0)
	{
		GetSTProcess(StStateAll);
	}
		
	Stx=0;
	Sty=0;	
	for(Sti=0;Sti<3;Sti++)
	{
		for(Stj=0;Stj<8;Stj++)
		{			
			FData=FlagsADError[Sti].Data;      
			FData=(FData>>Stj)&0x01;						
			if(FData==0||(FData==1&&FlagsADShort[Sti][Stj]<AD_SHORT_N))		
			{
				PData=PutData[Sti].Load;
				PData=(PData>>Stj)&0x01;				
				SData=StData[Sti].Data;
				SData=(SData>>Stj)&0x01;
				if(SData==0&&PData==1)															/*switch close but drive load will detect open or short*/								
				{
					if(OnCheckOpen==1)
					{		
						FlagsOnChechOpen[Sti][Stj]++;
						if(FlagsOnChechOpen[Sti][Stj]>=AD_ON_OPEN_N)						
						{
							ReState[Stx].Data&=~(0x03<<Sty);
			              ReState[Stx].Data|=0x02<<Sty;           				////Open is "10" 
			              FlagsADError[Sti].Data|=0x01<<Stj;
			              FlagsOnChechOpen[Sti][Stj]=AD_ON_OPEN_N;
							FlagsADNormalOpen[Sti][Stj]=0;								///if be judged to error,clear normal state counter
							FlagsADNormalClose[Sti][Stj]=0;
							FlagsADShort[Sti][Stj]=0;											///09.05.26 add to resolve"open and display to short"
						}						
					}
					else
					{
		            	FlagsADShort[Sti][Stj]++;
		            	if(FlagsADShort[Sti][Stj]>=AD_SHORT_N)
		            	{ 
		              	ReState[Stx].Data&=~(0x03<<Sty);
		              	ReState[Stx].Data|=0x01<<Sty;          			/*short is "01"*/
		              	FlagsADError[Sti].Data|=0x01<<Stj;
		              	FlagsADShort[Sti][Stj]=AD_SHORT_N;
						FlagsADNormalOpen[Sti][Stj]=0;							///if be judged to error,clear normal state counter
						FlagsADNormalClose[Sti][Stj]=0;
              }
            } 
         }
         else if(SData==0&&PData==0) 												/*switch close and not drive load will detect open*/
         {
            FlagsADOpen[Sti][Stj]++;
          	if(FlagsADOpen[Sti][Stj]>=AD_OPEN_N)
          	{ 
            	ReState[Stx].Data&=~(0x03<<Sty);
            	ReState[Stx].Data|=0x02<<Sty;            			/*load Open is 10*/
            	FlagsADError[Sti].Data|=0x01<<Stj;
           		FlagsADOpen[Sti][Stj]=AD_OPEN_N;
							FlagsADNormalOpen[Sti][Stj]=0;								///if be judged to error,clear normal state counter
							FlagsADNormalClose[Sti][Stj]=0;
           	}
          }
          else	if(SData==1&&PData==1)														
         	{
         		FlagsADNormalOpen[Sti][Stj]++;
				if(FlagsADNormalOpen[Sti][Stj]>=AD_NORMAL_OPEN)
				{
					FlagsADNormalOpen[Sti][Stj]=AD_NORMAL_OPEN;
         			ReState[Stx].Data&=~(0x03<<Sty);							/*normal open is 00*/
	            	FlagsADError[Sti].Data&=~(0x01<<Stj);					/*error(short or open) flag clear*/              		        	
	            	FlagsADOpen[Sti][Stj]=0;      									/*when switch not open and load open counter clear*/             		         	           	     		
	            	FlagsADShort[Sti][Stj]=0;             							/*short counter clear*/           		
         			if(FlagsOnChechOpen[Sti][Stj]>0&&OnCheckOpen==1)
         				FlagsOnChechOpen[Sti][Stj]=0; 							/*when switch open and temporal open counter is cleared*/ 
         			if(FlagsOnChechOpen[Sti][Stj]>=AD_ON_OPEN_N) 	///add 08-09-15 to resolve temporal open error display
         			{
         				ReState[Stx].Data&=~(0x03<<Sty);
         				ReState[Stx].Data|=0x02<<Sty;           			////Open is "10" 
              			FlagsADError[Sti].Data|=0x01<<Stj;
         			}
				}
         	} 	
         	else	if(SData==1&&PData==0)
         	{	 
         		FlagsADNormalClose[Sti][Stj]++;
         		if(FlagsADNormalClose[Sti][Stj]>=AD_NORMAL_CLOSE)
         		{
         			FlagsADNormalClose[Sti][Stj]=AD_NORMAL_CLOSE;
		 			ReState[Stx].Data&=~(0x03<<Sty);					
		 			ReState[Stx].Data|=0x03<<Sty;									/*normal close is 11*/
           		FlagsADError[Sti].Data&=~(0x01<<Stj);					/*error(short or open) flag clear*/              		        	
           		FlagsADOpen[Sti][Stj]=0;      									/*when switch not open and load open counter clear*/             		         	
           		FlagsOnChechOpen[Sti][Stj]=0; 								/*when switch open and temporal open counter is cleared*/            	     		
           		FlagsADShort[Sti][Stj]=0;             				/*short counter clear*/
         		}
          }        	
			}
			FData=FlagsADError[Sti].Data;
	      FData=(FData>>Stj)&0x01;
	      if(FData==1)
	      {  
	        if(FlagsADShort[Sti][Stj]>=AD_SHORT_N)      				///if load is short,close it 	
	         	PutData[Sti].Load&=~(0x01<<Stj);
	        if(FirstADErrorFlag[Sti][Stj]==0)										/*when first short error,must send to can_bus immediately;08-08-04 add*/
	        {
	         	LoadSTChangFlag=1;																	/*set st change and can send flag*/ 
	         	FirstADErrorFlag[Sti][Stj]=1;
	         }          		                  
	       }
	       else       			
	        FirstADErrorFlag[Sti][Stj]=0;          	
	        				
			Sty+=2;      																					/*2bit show a load state which is open,short or normal*/
	      	if(Sty>=8)
	      	{
	        	Stx++; 
	        	Sty=0;
	      	}
		}		
	} 	
	if(OnCheckOpen==1)
		OnCheckOpen=0;
}  
/**********************************************
 * Function:        change drive module data
 *
 * call:    		  	none
 *
 * Input:           none
 *
 * Output:          unsigned char TempDataC:if change happen return 1;else return 0;
 *
 * Note:            
**********************************************/
///configure near lamp
static volatile const unsigned	char Near_Lamp_Configure1 @0xE805=2;	

static unsigned char ChangeData(void)
{
	unsigned	char	TempDataB,TempDataC;
	unsigned	char	Mi,Mj;	
  /*load out realize*/  	
  for(Mi=0;Mi<3;Mi++)
		PutData[Mi].Load=0x00;						///first close all,then open what you need	

	if(FrontDriveFlag==1)
	{
		///SpeedSenorPower_IO=1;
		
		if(SleepStartFlags==1)						///if sleep,not open load
		{
			///SpeedSenorPower=0;
			goto _LoadOut;
		}

		///SpeedSenorPower=1;

		if((AccKeyBit||(MeterErrorState&&BakOnKeyBit))&&SyringeKeyBit)
			DLoad1=1;
		
		if((AccKeyBit||(MeterErrorState&&BakOnKeyBit))&&LocationLampKeyBit&&FrontDoorOpenSignalKeyBit)
			DLoad2=1;
		
		if(OnKeyBit||(MeterErrorState&&BakOnKeyBit))
			DLoad3=1;
		
		if(((((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&LeftTurnLampKeyBit)||DangerWarnKeyBit)&&(LeftFlashCode==1))
			||(MeterErrorState&&BakDangerWarnKeyBit&&(BakLeftFlashCode==1)))
			DLoad4=1;
		
   		if(((((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&RightTurnLampKeyBit)||DangerWarnKeyBit)&&(RightFlashCode==1))
			||(MeterErrorState&&BakDangerWarnKeyBit&&(BakRightFlashCode==1)))
			DLoad5=1;
			
		if((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&RShieldSignalKeyBit)
			DLoad6=1;			
		
//		if((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&RShieldSignalKeyBit)
//			DLoad7=1;
//		if((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&NShieldSignalKeyBit)
//			DLoad8=1;
		
		if((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&LocationLampKeyBit&&FrontFogKeyBit)
		{
			DLoad9=1;
			DLoad10=1;
		}

		if((AccKeyBit||(MeterErrorState&&BakOnKeyBit))&&ReadLampKeyBit)
		{
			DLoad11=1;
			DLoad12=1;
		}
		
		if((AccKeyBit||(MeterErrorState&&BakOnKeyBit))&&BugleKeyBit)
			DLoad15=1;

		if((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&!BackFlameoutKeyBit)
			DLoad16=1;

		if(FrontDoorCloseKeyBit)
			DLoad17=1;
		
		if((ReSpeedData<=5)&&FrontDoorOpenKeyBit)
			DLoad18=1;

		#if	RainScratch_Out_From==1
		RelayControl_RainScratchDeal();
		#endif
	}
	else if(BackDriveFlag==1)
	{
		SpeedSenorPower_IO=1;
		
		if(SleepStartFlags==1)						///if sleep,not open load
		{
			SpeedSenorPower=0;
			goto _LoadOut;
		}

		SpeedSenorPower=1;

		///启动电源总开关
		if((OnKeyBit==KeyOn||(MeterErrorFlag&&BakOnKeyBit==KeyOn))&&((RotationData<=450)&&(!motor_st_flag)))
			DLoad1=1;		
		if((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&LocationLampKeyBit&&FrontFogKeyBit&&BackFogKeyBit)
			DLoad2=1;
		if((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&!BackFlameoutKeyBit)
			DLoad3=1;	
		if(!BackRoomKeyBit)
			DLoad4=1;
		if(((((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&LeftTurnLampKeyBit)||DangerWarnKeyBit)&&(LeftFlashCode==1))
			||(MeterErrorState&&BakDangerWarnKeyBit&&(BakLeftFlashCode==1)))
			DLoad5=1;
   		if(((((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&RightTurnLampKeyBit)||DangerWarnKeyBit)&&(RightFlashCode==1))
			||(MeterErrorState&&BakDangerWarnKeyBit&&(BakRightFlashCode==1)))
			DLoad6=1;
		if((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&RShieldSignalKeyBit)
			DLoad7=1;
		if(AccKeyBit||(MeterErrorState&&BakOnKeyBit)) 				 
	    {
	      DLoad8=1;
	    	PowerOff_Delay=0;							///initialize Power off delay
	    	PowerOff_Flag=1;
	    }  
		else															
		{
			if(PowerOff_Delay>=Num_PowerOff)
				PowerOff_Flag=0;
			
			if(PowerOff_Flag==1)							///delay 10s when ACC off
				DLoad8=1;
			else
				DLoad8=0;
		}
		if(OnKeyBit||(MeterErrorState&&BakOnKeyBit))
			DLoad11=1;
		
		if((AccKeyBit||(MeterErrorState&&BakOnKeyBit))&&LocationLampKeyBit)
			DLoad12=1;
		
		if((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&(FootStopKeyBit||DelaySpeedWorkIndicateKeyBit))
			DLoad13=1;

		if((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&NShieldSignalKeyBit&&(RotationData<550)&&(!MotorStartTooLongFlag)
			&&(((StartKeyBit||(MeterErrorState&&BakStartKeyBit))&&(BackRoomKeyBit))||BackStartKeyBit)) 	
			DLoad14=1;
		
		if(OnKeyBit||(MeterErrorState&&BakOnKeyBit))
		{
			DLoad15=1;
			DLoad16=1;
		}	
	}
	else if(TopDriveFlag==1)
	{
		///SpeedSenorPower_IO=1;
		if(SleepStartFlags==1)						///if sleep,not open load
		{
			///SpeedSenorPower=0;
			goto _LoadOut;
		}  
		///SpeedSenorPower=1;

		if(0)//(AccKeyBit||(MeterErrorState&&BakOnKeyBit))&&MotorManKeyBit)
			DLoad1=1;
		if(0)//(AccKeyBit||(MeterErrorState&&BakOnKeyBit))&&InsideRoomLamp3KeyBit)
			DLoad2=1;
		if(OnKeyBit||(MeterErrorState&&BakOnKeyBit))
			DLoad3=1;
//		if((AccKeyBit||(MeterErrorState&&BakOnKeyBit))&&LocationLampKeyBit&&BackDoorOpenSignalKeyBit)
//			DLoad4=1;
		if(0)//(ReSpeedData<5)&&FrontDoorOpenKeyBit||FrontDoorPreventNipKeyBit)
			DLoad5=1;
		if(FrontDoorCloseKeyBit)
			DLoad6=1;
		if(((((AccKeyBit||(MeterErrorState&&BakOnKeyBit))&&LeftTurnLampKeyBit)||DangerWarnKeyBit)&&(LeftFlashCode==1))
			||(MeterErrorState&&BakDangerWarnKeyBit&&(BakLeftFlashCode==1)))
			DLoad7=1;
   		if(((((AccKeyBit||(MeterErrorState&&BakOnKeyBit))&&RightTurnLampKeyBit)||DangerWarnKeyBit)&&(RightFlashCode==1))
			||(MeterErrorState&&BakDangerWarnKeyBit&&(BakRightFlashCode==1)))
			DLoad8=1;
//		if((ReSpeedData<5)&&BackDoorOpenKeyBit||BackDoorPreventNipKeyBit)
			DLoad9=1;
//		if(BackDoorCloseKeyBit)
			DLoad10=1;
		if((AccKeyBit||(MeterErrorState&&BakOnKeyBit))&&LocationLampKeyBit)
		{
			DLoad13=1;	
			DLoad14=1;	
		}
		if(0)//(AccKeyBit||(MeterErrorState&&BakOnKeyBit))&&GuidepostLampKeyBit)
		{
			DLoad15=1;	
			DLoad16=1;
		}
		if(0)//(AccKeyBit||(MeterErrorState&&BakOnKeyBit))&&InsideRoomLamp2KeyBit)
			DLoad17=1;	
		if(0)//(AccKeyBit||(MeterErrorState&&BakOnKeyBit))&&InsideRoomLamp1KeyBit)
			DLoad18=1;	
	}
   	  	
  for(Mi=0;Mi<3;Mi++)
  {
  	for(Mj=0;Mj<8;Mj++)
  	{  
  		TempDataB=FlagsADError[Mi].Data;
   		TempDataB=(TempDataB>>Mj)&0x01;    			
   		if(TempDataB==0x01)   
   		{
   			if(FlagsADShort[Mi][Mj]>=AD_SHORT_N)////if short,must close load
   				PutData[Mi].Load&=~(0x01<<Mj); 
   		}
   	}
  }
	/************aptitude lamp change**************/		
	if(FrontDriveFlag==1)
	{
		unsigned char LeftFarLamp1Error=0,LeftFarLamp2Error=0;
		unsigned char RightFarLamp1Error=0,RightFarLamp2Error=0;
		unsigned char	LeftNearLampError=0,RightNearLampError=0;
		unsigned char	LeftFrontFogError=0,RightFrontFogError=0;

		if(PowerLoadState[2].St.S0==OpenStateNum||PowerLoadState[2].St.S0==ShortStateNum)///number 9
			LeftFarLamp1Error=1;	
		if(PowerLoadState[2].St.S1==OpenStateNum||PowerLoadState[2].St.S1==ShortStateNum)///number 10
			LeftFarLamp2Error=1;
		
		if(PowerLoadState[0].St.S2==OpenStateNum||PowerLoadState[0].St.S2==ShortStateNum)///number 3
			RightFarLamp1Error=1; 	
		if(PowerLoadState[0].St.S3==OpenStateNum||PowerLoadState[0].St.S3==ShortStateNum)///number 4
			RightFarLamp2Error=1;
		
		if(PowerLoadState[1].St.S3==OpenStateNum||PowerLoadState[1].St.S3==ShortStateNum)///number 8
			LeftNearLampError=1;		
		if(PowerLoadState[1].St.S0==OpenStateNum||PowerLoadState[1].St.S0==ShortStateNum)///number 5
			RightNearLampError=1;
			
		if(LastState[2].St.S0==OpenStateNum||LastState[2].St.S0==ShortStateNum)///number 9
			LeftFrontFogError=1;
		if(LastState[2].St.S1==OpenStateNum||LastState[2].St.S1==ShortStateNum)///number 10
			RightFrontFogError=1;
		
		if(((AccKeyBit)||(MeterErrorState&&BakOnKeyBit)))
		{
			///if left near and far lamp error all,open left fog lamp
			if((FarLampKeyBit)&&(!NearLampKeyBit))
			{
				if(LeftFarLamp1Error&&LeftFarLamp2Error)
				{
					if(LeftNearLampError == 1&&(LeftFrontFogError == 0))
						DLoad9 = 1;
				}
				if(RightFarLamp1Error&&RightFarLamp2Error)
				{
					if(RightNearLampError == 1&&(RightFrontFogError == 0))
						DLoad10 = 1;
				}
			}
			if(NearLampKeyBit&&FarLampKeyBit)
			{
				if(LeftNearLampError&&(LeftFrontFogError == 0))
					DLoad9 = 1;
				if(RightNearLampError&&(RightFrontFogError == 0))
					DLoad10 = 1;
			}
		}
	} 
	else if(BackDriveFlag==1)
	{
		unsigned	char	BackStopError=0,BackFogError=0;
		if(LastState[3].St.S0==OpenStateNum||LastState[3].St.S0==ShortStateNum)///number 13
			BackStopError = 1;
		if(LastState[0].St.S1==ShortStateNum)///number 2
			BackFogError=1;		
	   	if((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&(FootStopKeyBit||DelaySpeedWorkIndicateKeyBit))
	   	{
	   		///if left back stop lamp error,turn back fog
	   		if(BackStopError&&(BackFogError==0))	
	   			DLoad2=1;
	   	}
	}
_LoadOut:  	
  /*judge if load drive will be changed*/  	
  TempDataC=0;
  for(Mi=0;Mi<3;Mi++)
  {  
    if(PutData[Mi].Load!=OldPutData[Mi].Load)
    {
      TempDataC=1;                            
      OldPutData[Mi].Load=PutData[Mi].Load;
     }     
   } 
  return(TempDataC); 	
}
/*********************************************************


**********************************************************/
static void RelayControl_RainScratchDeal(void)
{
	if(AccKeyBit||(MeterErrorState&&BakOnKeyBit))
	{
		if(SyringeKeyBit)
		{
			RainScratchLowOpen;
			WashStartFlag=1;									
			CountWashStopTimeStartFlag=0;	
			WashStopFlag=1;	
			goto _RainScratchDealOut;
		}
		else 
		{
			if((RainScratchHighSpeedKeyBit||RainScratchLowSpeedKeyBit
				||RainScratchIntermissionKeyBit)&&(WashStopFlag==0))
				goto _RainScratchDeal;
			if(WashStartFlag==1)
			{
				RainScratchLowOpen;
				WashStartFlag=0;		
				CountWashStopTimeStartFlag=1;		
				WashTime=0;		
				goto	_RainScratchDealOut;
			}
			else 
			{
				if(WashStopFlag==1)							///if wash time not out then continue scratching
				{
					RainScratchLowOpen;
					goto	_RainScratchDealOut;
				}
			}
		}		
		_RainScratchDeal:			
		if(RainScratchHighSpeedKeyBit)
		{
			RainScratchHighOpen;
			RainScratchLowOpen;///11.04.09 add
			goto _RainScratchDealOut;
		}
		if(RainScratchLowSpeedKeyBit)
			RainScratchLowOpen;
		if(RainScratchIntermissionKeyBit)
		{
			if(IntermissionStartFlag==0)
			{
				RainScratchLowOpen;
				IntermissionStartFlag=1;
				IntermissionTime=0;						
				IntermissionStopFlag=0;					
			}
			else 
			{
				if(IntermissionStopFlag==0)			///if intermission time not out then continue scratching
					RainScratchLowOpen;
			}
		}
		else 
		{
			RainScratchIntermissionTime=0;
			IntermissionStartFlag=0;
		}	
		_RainScratchDealOut:
			asm nop;
	}
	else														///acc off then initialize rain scratch time and flag
	{
		WashStartFlag=0;							///wash initialize
		WashStopFlag=0;
		CountWashStopTimeStartFlag=0;	
		WashTime=0;	
		IntermissionStartFlag=0;			///intermission initialize
		IntermissionTime=0;		
		IntermissionStopFlag=1;	
	}
}
/**********************************************
 * Function:        ST RESET
 *
 * call:    		  	none
 *
 * Input:           None
 *
 * Output:          None
 *
 * Note:            
**********************************************/
static void	ErrorReset(void)
{
	unsigned	char	Mi,Mj;
  if(((!AccKeyBit)&&(!OnKeyBit)&&(!DangerWarnKeyBit))
		||((MeterErrorState&&(!BakOnKeyBit)&&(!BakDangerWarnKeyBit))))
	{
		if(ClearErrorFlag==1)
		{
 			ClearErrorFlag=0;
			for(Mi=0;Mi<3;Mi++)
	    	{
	   			ReState[2*Mi].Data=0xff;///when start,st state is normal close;08-09-05 modify
	   			LastState[2*Mi].Data=0xff;
				ReState[2*Mi+1].Data=0xff;
				LastState[2*Mi+1].Data=0xff;
	   	
	   			FlagsADError[Mi].Data=0x00;
	   	
		   		for(Mj=0;Mj<8;Mj++)
		   		{
		   			FlagsADShort[Mi][Mj]=0x00;     	
		   			FlagsOnChechOpen[Mi][Mj]=0x00;    	
		   			FlagsADOpen[Mi][Mj]=0x00; 
		   			FlagsADNormalOpen[Mi][Mj]=0x00;	
					FlagsADNormalClose[Mi][Mj]=0x00;
		   		}
	   		} 
		}
	}
	else
	{
		if(ClearErrorFlag==0)
			ClearErrorFlag=1;
	}		
}
/*************************************************************************************************************

**************************************************************************************************************/
static void	MotorReStartDetect(void)
{
	switch(motor_st_index)
   {
    case 0:
      if(!EngineChargeIndicateKeyBit)		///when start,the key is high so must mask;10.01.06modify
      {		                
        RestartDetectTime=0;
        motor_st_index=1;
      }
      else
        motor_st_flag=0;
      break;
    case 1:
      if(RestartDetectTime<ReStartDelayNum)						
      {					    
        if(EngineChargeIndicateKeyBit)           
          motor_st_index=0;
      }
      else
      {
        motor_st_flag=1;
        motor_st_index=0;
      }
      break;
    default:
      motor_st_index=0;
      break;
  }
}
/*************************************************************************************************************

**************************************************************************************************************/
static void MotorStartTooLongDetect(void)
{
 switch(st_index)
  {
    case 0:
      if((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&NShieldSignalKeyBit&&(((StartKeyBit||(MeterErrorState&&BakStartKeyBit))&&(BackRoomKeyBit))||BackStartKeyBit))		
      {	                  
        MotorStartDetect=0;
        st_index=1;
      }
      else
      {
        MotorStartTooLongFlag=0;
      }
      break;
    case 1:
      if((OnKeyBit||(MeterErrorState&&BakOnKeyBit))&&NShieldSignalKeyBit&&(((StartKeyBit||(MeterErrorState&&BakStartKeyBit))&&(BackRoomKeyBit))||BackStartKeyBit))		
      {
        if(MotorStartDetect>EngineStartDelayNum)//engine start too long
          MotorStartTooLongFlag=1;             			
      }
      else
      	st_index=0;
      break;
    default:
      st_index=0;
      break;
  }
}
/***********************************************************


************************************************************/
#define	AirerState1			LastState[3].St.S2///第15路
#define	AirerState2			LastState[3].St.S3///第16路

static void LoadStateMask(void)
{
	unsigned	char	Sti,Stj,Stx,Sty,FData,PData,SData;		

	Stx=0;
	Sty=0;	
	for(Sti=0;Sti<3;Sti++)
	{
		for(Stj=0;Stj<8;Stj++)
		{		
			FData=LoadOpenDetectBit[Sti].Data;
			FData=(FData>>Stj)&0x01;	
			PData=PutData[Sti].Load;
			PData=(PData>>Stj)&0x01;
			SData=ReState[Stx].Data;
			SData=(SData>>Sty)&0x03;
			if(FData==1)		///不需要做开路检测
			{		
				if(SData==OpenStateNum)///出现开路									
				{
					if(PData==1)///负载打开
					{
						LastState[Stx].Data&=~(0x03<<Sty);///表示为正常开			
					}
					else
					{
						LastState[Stx].Data&=~(0x03<<Sty);///表示为正常关					
				 		LastState[Stx].Data|=0x03<<Sty;
					}
				}
				else
				{
					if(SData==NormalOpenStateNum)
						LastState[Stx].Data&=~(0x03<<Sty);///表示为正常开		
					else if(SData==ShortStateNum)
					{

						LastState[Stx].Data&=~(0x03<<Sty);		///表示为短路
           	LastState[Stx].Data|=0x01<<Sty;    
					}
					else if(SData==NormalCloseStateNum)
					{

						LastState[Stx].Data&=~(0x03<<Sty);///表示为正常关					
				 		LastState[Stx].Data|=0x03<<Sty;
					}
				}
			}
			else///需要做开路检测；此时该是怎么状态还是怎么状态
			{
				if(SData==OpenStateNum)
				{
					LastState[Stx].Data&=~(0x03<<Sty);///表示为开路
          LastState[Stx].Data|=0x02<<Sty;            			
				}
				else if(SData==NormalOpenStateNum)
					LastState[Stx].Data&=~(0x03<<Sty);///表示为正常开		
				else if(SData==ShortStateNum)
				{
					LastState[Stx].Data&=~(0x03<<Sty);		///表示为短路
         	LastState[Stx].Data|=0x01<<Sty;    
				}
				else if(SData==NormalCloseStateNum)
				{
					LastState[Stx].Data&=~(0x03<<Sty);///表示为正常关					
			 		LastState[Stx].Data|=0x03<<Sty;
				}
			}
			Sty+=2;  ///两位表示一种负载状态
    	if(Sty>=8)
    	{
      	Stx++; 
      	Sty=0;
    	}
		}
	}
	///对干燥器的特殊状态处理；当开路时置为正常关，让仪表不做显示。
	if(BackDriveFlag==1)
	{
		if(AirerState1==OpenStateNum)
			AirerState1=NormalCloseStateNum;
		if(AirerState2==OpenStateNum)
			AirerState2=NormalCloseStateNum;
	}
} 
/*********************************************
**********************************************/
static void Module_Check(void)
{
	unsigned char	FrontKeyTemp;

	FrontKeyTemp=FrontKeyData[0].Data&0b00010101;		///3bit is sleep detect key bit(on/powertotal/danger warn);
	if(FrontKeyTemp==0)															///the key is off,possible sleep
		BakSleepKeyOffFlag=1;
	else
	{
		BakSleepKeyOffFlag=0;
		SleepStartFlags=0;														///bak key start so clear sleep flag
	}
	
	if((MeterModuleFlag==1)||(SleepStartFlags==1)||(BakSleepKeyOffFlag==1))	
  {        																				////when sleep key is on and meter module not exit then start detect. 
    MeterModuleFlag=0;
    MeterErrorFlag=0;    
    delay3s_ModuleCheck=0;
  }
  else
  {
		if(delay3s_ModuleCheck>ConnectDetectTimeNum)
		{
			unsigned char	i;
			delay3s_ModuleCheck=RandomSetTimeNum;
			MeterErrorFlag=1;   
			for(i=0;i<5;i++)
				MeterKeyData[i].Data=0x00;
			LeftFlashCode=0x00;														///mask flash synchronization information
			RightFlashCode=0x00;
   		}
  }
}
/**********************************************************************************

**********************************************************************************/
static void FlashLampDeal(void)
{	
  switch(FlashIndex)
  {
    case 0:
      if((MeterErrorState)&&(BakDangerWarnKeyBit))
      {      
         dirdelay=HighSpeedFlashDelay;   
		 BakFlashKey[0]=1;						///open flash
		 BakFlashKey[1]=1;
         SendFlashData[0]=AllFlashOpenCode;     
		 SendMessage(SendSleep_Danger);
         FlashIndex=1;       
      }
      delayxms=0;
      delayxms_flag=0;   
      break;
    case 1:
      if(delayxms_flag)
      {
        SendFlashData[0]=AllFlashCloseCode;       
        SendMessage(SendSleep_Danger);
		 BakFlashKey[0]=0;							///close flash
		 BakFlashKey[1]=0;
        delayxms=0;
        delayxms_flag=0;
        FlashIndex=2;  
      }
      break;
    case 2:      
     	if(delayxms_flag)
      		FlashIndex=0;
		else
			asm nop;
      break;
    default:
      FlashIndex=0;
      break;
  }
}

