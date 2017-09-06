#include "Led.h"
#include "BacklightControl.h"
#include "IS31FL3733_I2C.h" 

UINT8 LeftTurnSignalWarn;
UINT8 RightTurnSignalWarn;

BitCommon ledShow[4] = {0,0,0,0};//= {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};  //;
BitCommon OldledShow[4] = {0,0,0,0};//= {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};  //;
const UINT8 lednumBuf[8] = {22,19,20,21,46,45,43,44};

extern uchar lastLedLeftState,lastLedRightState;
extern uchar SetLedLeftState,SetLedRightState;


void hc595_init(){

  HC595_CLOCK_MUX();
  HC595_CLOCK_PDDR();
  HC595_OUT_MUX();
  HC595_OUT_PDDR();
  HC595_OE_MUX();
  HC595_OE_PDDR();
  HC595_DATA1_MUX();
  HC595_DATA1_PDDR();
  HC595_DATA2_MUX();
  HC595_DATA2_PDDR();
  HC595_DATA9_MUX();
  HC595_DATA9_PDDR();

}

/**********************************************
 * Function:   output 74hc595 data to BTS724G IN pin
 *
 * call:    	 None
 *
 * Input:      None
 *
 * Output:     None
 *
 * Note:            

 输出数据到595控制负载
 高位先发
******************************************************************/
void	Out595(unsigned char	*buf)
{

	signed char	j;
	unsigned char TempDataA;
	/*load output control*/				
	//	HC595_OE=0;						///when data transmit,oe pin must be set to low
	HC595_CLOCK_L;				// 初始化控制脚
	HC595_OUT_L;					///lock 595 锁定输出
	if(!SleepFlag)
	{
	for(j=7;j>=0;j--)
	{
		TempDataA=buf[0]>>j;
		if(TempDataA&0x01)	// 从高位发送数据
		  HC595_DATA1_L;		///transmit 595 data
		else
		  HC595_DATA1_H;

		TempDataA=buf[1]>>j;
		if(TempDataA&0x01)	// 从高位发送数据
		  HC595_DATA2_L;		///transmit 595 data
		else
		  HC595_DATA2_H;

		TempDataA=buf[2]>>j;
		if(TempDataA&0x01)	// 从高位发送数据
		  HC595_DATA9_L;		///transmit 595 data
		else
		  HC595_DATA9_H;
		HC595_CLOCK_H;			// 输出移位时钟
		__asm("nop");
		__asm("nop");
		__asm("nop");
		__asm("nop");
		__asm("nop");
		__asm("nop");
		__asm("nop");
		__asm("nop");
		HC595_CLOCK_L;
	}	
	HC595_OUT_H;					///lock 595 锁定输出	
	HC595_OE_L;						///when data transmit,oe pin must be set to low
		}
	else{
		HC595_OUT_L;
		HC595_OE_L;	
		HC595_CLOCK_L;
		HC595_DATA9_L;
		HC595_DATA2_L;
		HC595_DATA1_L;
		
	}

}
const uint8 PWM_Gama16[64] = 
  {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
    0x08,0x09,0x0b,0x0d,0x0f,0x11,0x13,0x16,
    0x1a,0x1c,0x1d,0x1f,0x22,0x25,0x28,0x2e,
    0x34,0x38,0x3c,0x40,0x44,0x48,0x4b,0x4f,
    0x55,0x5a,0x5f,0x64,0x69,0x6d,0x72,0x77,
    0x7d,0x80,0x88,0x8d,0x94,0x9a,0xa0,0xa7,
    0xac,0xb0,0xb9,0xbf,0xc6,0xcb,0xcf,0xd6,
    0xe1,0xe9,0xed,0xf1,0xf6,0xfa,0xfe,0xff,
  };
UINT8 lednum= 0;
void ledInit(void){
 unsigned char	buf[3] = {0,0,0};  
  //背光灯
  BACKGROUD_LED_PORT_MUX();
  FTM_PWM_init(BACKGROUD_LED_MODULE, BACKGROUD_LED_CHANNL, METER_BACK_LIGHT_FRQ,100);
  hc595_init();
  Out595(buf);
}
//******************************************************************************
// brief:	led控制数据整理
//******************************************************************************
extern UINT16 PR;
void ledInterface(void){
      UINT8 i;
	LED15_595 = LogicOutPut[4].Loads.Load0;//32
       LED13_595 = LogicOutPut[4].Loads.Load1;//33
       LED11_595 = LogicOutPut[4].Loads.Load2;//34
       LED9_595   = LogicOutPut[4].Loads.Load3;//35
       LED38_595 = LogicOutPut[4].Loads.Load4;//36
       LED7_595  = LogicOutPut[4].Loads.Load5;
       LED5_595  = LogicOutPut[4].Loads.Load6;
       LED3_595  = LogicOutPut[4].Loads.Load7;

	LED6_595   = LogicOutPut[5].Loads.Load0;
	LED8_595   = LogicOutPut[5].Loads.Load1;
       LED10_595 = LogicOutPut[5].Loads.Load2;
       LED67_595 = LogicOutPut[5].Loads.Load3;
       LED68_595 = LogicOutPut[5].Loads.Load4;
       LED16_595 = LogicOutPut[5].Loads.Load5;
	LED14_595 = LogicOutPut[5].Loads.Load6; 
	LED12_595=  LogicOutPut[5].Loads.Load7;
      //电池SOC报警  LogicOutPut[6].Loads.Load0;
      //电池电压报警  LogicOutPut[6].Loads.Load1;
       LED1_595  =   LogicOutPut[6].Loads.Load2;
       LED2_595  =   LogicOutPut[6].Loads.Load3;
	//车速超速报警灯 
	 LED21_IS=   LogicOutPut[6].Loads.Load4;
       //转速超速报警灯 
       LED25_IS =   LogicOutPut[6].Loads.Load5;
	   
       LED4_595          =   LogicOutPut[6].Loads.Load6;  //位置灯

        LED34_IS   =   LogicOutPut[6].Loads.Load7;
	 LED32_IS   =   LogicOutPut[7].Loads.Load0;
	 LED30_IS   =   LogicOutPut[7].Loads.Load1;
	 LED31_IS   =   LogicOutPut[7].Loads.Load2;
	 LED35_IS   =   LogicOutPut[7].Loads.Load3;
	 LED33_IS   =   LogicOutPut[7].Loads.Load4;

	 


	

   if(ON_LOAD && GEAR_1_F)                      //内部处理一级报警图标
    {
        SLIGHT_FAULT = 1;//LogicOutPut[4].Loads.Load1;//39 - 33 轻微故障//MLed13 = 1;
    }
    else
    {
        SLIGHT_FAULT = 0;//MLed13 = 0;
    }
	if(ON_LOAD && GEAR_2_F)                     //内部处理二级报警图标
    {
       SERIOUS_FAULT = 1;//MLed16 = 1;
    }
    else
    {
       SERIOUS_FAULT = 0;//MLed16 = 0; 
    }

	//ledShow[0].Byte = 0xff;
	//ledShow[1].Byte = 0xff;
	//ledShow[2].Byte = 0xff;
	if((OldledShow[0].Byte!=ledShow[0].Byte)||(OldledShow[1].Byte!=ledShow[1].Byte)||(OldledShow[2].Byte!=ledShow[2].Byte))
		{
			    
	             Out595(&ledShow[0].Byte);
		      OldledShow[0].Byte=ledShow[0].Byte;
			OldledShow[1].Byte=ledShow[1].Byte;
			OldledShow[2].Byte=ledShow[2].Byte;
		}

	/************* 仪表背光*********************/
	if(BK_LED_refreshFlag)
	{
		static UINT16 exp = 0;
		/* 临时*/
		if(ACC_KEY)
			if(InsideData[10] < 1) BACK_LED_LOAD = 1;
		if(BACK_LED_LOAD)  
	    {  
	    	UINT16 pv= 0;
			
	    	pv = InsideData[10]*3/2+LedDutyCONST[USR_s_DeviceConfigData.LightLevel];
            if(pv >100) pv= 100;
			if(exp < pv){
				exp++;
			}else if(exp > pv){
				exp = pv;
			}
			
			///if(pv>100) pv = 100;
			BACKGROUD_LED_DUTY = FTM_PWM_Duty(TFT_LINGHT_FRQ,exp);
	        //BOTTOM_LIGHT_LED_ON; 
	     	
	    }
    	else
	    {
	   		exp = 0;
	        BOTTOM_LIGHT_LED_OFF;        
	    }

		BK_LED_refreshFlag = 0;



	}
	/***************气压亮度控制*************************/
  
#if 1
	{
	     const UINT8 preBacklightPluse[4] = {MIN_BACKLINGHT,MIN_BACKLINGHT+10,MIN_BACKLINGHT+20,MIN_BACKLINGHT+30};
	      static UINT16 exp = MIN_BACKLINGHT;
		 static UINT16  left_right_exp =0;
		#if defined(ZB223BYY0A) || defined(ZB223BYY00) || defined(ZB223YY01)
		//unsigned int pluseValue;
		//if(!SleepFlag){
                 if(ACC_KEY){
			unsigned int pv;
				pv = InsideData[10]+preBacklightPluse[USR_s_DeviceConfigData.LightLevel];

			if(pv >60) pv= 60;
			if(exp < pv){
				exp++;
			}else if(exp > pv){
				exp--;
			}

                       /* if( TPMS_1S_F)
                        {
                        lednum++;
                        TPMS_1S_F = 0;
                        if(lednum>12) lednum = 0;
                        IS31fL_SetLeftLeds(lednum,6,  PWM_Gama16[exp]);
                          IS31fL_SetRightLeds(lednum,4,  PWM_Gama16[exp]);
                        }*/

                         if((SetLedLeftState!=lastLedLeftState)||(left_right_exp!=exp))
                         	{
                         	   SetLedLeftState = lastLedLeftState;       
                              IS31fL_SetLeftLeds(lastLedLeftState,MotorStepPrm[0].minScale ,  PWM_Gama16[exp]);
                         	}
				if((SetLedRightState!=lastLedRightState)||(left_right_exp!=exp))
			       {
			          SetLedRightState = lastLedRightState;
                              IS31fL_SetRightLeds(lastLedRightState,MotorStepPrm[1].minScale,  PWM_Gama16[exp+3]);
			      }      
                              left_right_exp=exp;   
                                

                                    
						if(OldledShow[3].Byte!=ledShow[3].Byte)
					        {
					        OldledShow[3].Byte = ledShow[3].Byte;
                                         for(i=0;i<8;i++)
                                             {
                                               if(ledShow[3].Byte&(0x01<<i))
                                                IS31FL3738_SetLedPWM( lednumBuf[i]/8, lednumBuf[i]%8, 0xff/* PWM_Gama16[exp]*/);
                                                else
                                                 IS31FL3738_SetLedPWM( lednumBuf[i]/8, lednumBuf[i]%8, 0);		
                                              }
						}
                                        //pluseValue = getBacklightPluse(adV);//气压灯背光
                                        //SET_BACKLIGHT(pluseValue);
		}
		else{


			                       for(i=0;i<8;i++)
                                             {

                                                 IS31FL3738_SetLedPWM( lednumBuf[i]/8, lednumBuf[i]%8, 0);		
                                              }

			  IS31fL_SetLeftLeds(0,MotorStepPrm[0].minScale , 0);  	
                      IS31fL_SetRightLeds(0,MotorStepPrm[1].minScale,  0);

			 exp = MIN_BACKLINGHT;
		       left_right_exp =0;
			   OldledShow[3].Byte = 0;
	
		}
#endif	
	}
#endif
        
        
       //   SetColumnLed(&LiftColumnLed,0);
        //  SetColumnLed(&RightColumnLed,32);
          
        //  SetRightColumnLed();
       //   SetLiftColumnLed();
          
          
          
        //  SetColumnBackLum(InsideData[10]);
}

