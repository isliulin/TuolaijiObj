#include "BacklightControl.h"
#include "ap_S32K1xx_adc.h"
#include "ap_S32K1xx_FTM.h"
#include "IS31FL3733_I2C.h" 
void ProgressBarInit(St_ProgressBar *proBar, uint8 rtpres);
void SetProgressBarWarn(St_ProgressBar *proBar, uint32 warnVal);

//St_ProgressBar  LiftColumnLed, RightColumnLed,ColumnBackLum;
/*void BacklightInit(void){
	FTM_PWM_init(PRESSURE_BACKLIGHT_MODULE, PRESSURE_BACKLIGHT_CH, PRESSURE_BACKLIGHT_FRQ,100);
        ProgressBarInit(&LiftColumnLed,1);//200ms
        ProgressBarInit(&RightColumnLed,1);//200ms
        SetProgressBarFinal(&LiftColumnLed, 0);
        SetProgressBarFinal(&RightColumnLed, 0);
        SetProgressBarWarn(&LiftColumnLed, MotorStepPrm[0].filterGear);
        SetProgressBarWarn(&RightColumnLed,MotorStepPrm[1].filterGear);
        
        ProgressBarInit(&ColumnBackLum,1);//200ms
        SetProgressBarFinal(&ColumnBackLum, 0);  

        
}*/
#define NEED_TIMES		3
unsigned int getValueOPTO(void)
{
	unsigned int adV = 0;
	static unsigned int oldAdV = 0,rtnAdv = 0;
	static unsigned int countinueCnt = 0;//连续相同次数
	adV = AD_Trans(OPTO_ADC_MODULE,OPTO_ADC_Ch);
	adV = 255 - adV;
	adV /= 5;
	if(adV !=oldAdV){
		countinueCnt = 0;
		oldAdV = adV;
	}else{
		countinueCnt++;
		if(countinueCnt == NEED_TIMES){
			rtnAdv = adV;
		}
		
	}
	/*	< 2 开背光*/
        return rtnAdv;
}
const UINT8 preBacklightPluse[4] = {MIN_BACKLINGHT,MIN_BACKLINGHT+10,MIN_BACKLINGHT+20,MIN_BACKLINGHT+30};
unsigned int getBacklightPluse(unsigned int adV){
    
	unsigned int pluseValue;
                                         //10 20 30  40
	if(adV<18) pluseValue =preBacklightPluse[USR_s_DeviceConfigData.LightLevel];//90
	else pluseValue = preBacklightPluse[USR_s_DeviceConfigData.LightLevel] + adV;
	//pluseValue = 2 * pluseValue;
	if(pluseValue < MIN_BACKLINGHT)	pluseValue = MIN_BACKLINGHT;
	else if(pluseValue > MAX_BACKLINGHT) pluseValue = MAX_BACKLINGHT;
	
#if 0
	if(adV >210) adV = 210;
	else if(adV <120) adV = 110;
	//越亮adV越小-> pwm ^
	pluseValue = 210 -adV;
	//pluseValue = 2 * pluseValue;
	if(pluseValue < MIN_BACKLINGHT)	pluseValue = MIN_BACKLINGHT;
	else if(pluseValue > MAX_BACKLINGHT) pluseValue = MAX_BACKLINGHT;
#endif
	return pluseValue;
}
/*
void setBacklightPluse(void){
#if defined(ZB223BYY0A) || defined(ZB223BYY00) || defined(ZB223YY01)
	unsigned int pluseValue;
	if(!SleepFlag){
		unsigned int adV = 0;
		adV =  getValueOPTO();
		pluseValue = getBacklightPluse(adV);//气压灯背光
		SET_BACKLIGHT(pluseValue);
	}
	else{
		SET_BACKLIGHT(0);
	}
#endif	
}*/

//进度条处理相关
/*
void ProgressBarInit(St_ProgressBar *proBar, uint8 rtpres)
{
   proBar->finalStep  = 0;
   proBar->curStep    = 0;
   proBar->crtv       = 0;
   proBar->dispStep   = 0;
   proBar->rtpres     = rtpres;
}

void SetProgressBarFinal(St_ProgressBar *proBar, uint32 final)
{
   proBar->finalStep  = final;
}
void SetProgressBarWarn(St_ProgressBar *proBar, uint32 warnVal)
{
   proBar->warnVal  = warnVal;
}

uint32 GetProgressBarcur(St_ProgressBar *proBar)
{
   return proBar->curStep;
}

uint32 GetProgressBarDisp(St_ProgressBar *proBar)
{
   return proBar->dispStep;
}

void  doProgressBar(St_ProgressBar *proBar)
{
  proBar->crtv++;
  if(proBar->crtv>=proBar->rtpres)
  {
   proBar->crtv = 0;
   if(proBar->curStep>proBar->finalStep) proBar->curStep--;
   else if(proBar->curStep<proBar->finalStep)  proBar->curStep++;
  }
}

*/

#if 0
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
  
   //{0x04,0X08,0X0F, 0X1A,0X22,0X34,0X44,0X48,0X4F,0X5A  ,0x04,0X08,0X0F, 0X1A,0X22,0X34}, 
   //{0x13,0X1D,0X28, 0X3C,0X4B,0X5F,0X72,0X7D,0X88,0X8D  ,0x13,0X1D,0X28, 0X3C,0X4B,0X5F},
  // {0x34,0X44,0X55,0X69,0X7D,0X94,0XAC,0XB9,0XC6,0XCB   ,0x34,0X44,0X55,0X69,0X7D,0X94},
  // {0X5F,0X72,0X88,0XA0,0XB9,0XCF,0XED,0XF6,0XFE,0XFF   ,0X5F,0X72,0X88,0XA0,0XB9,0XCF},
 
// };
 void SetColumnLed(St_ProgressBar *progressBar,uint8 startLedNumber)
 {
 //  static uint8 lightLevel;
   uint8   ledStep;
   uint32   maskled;
   uint32   oldmaskled;
   uint32 LuncurStep;     //当前值
   uint32 LumdispStep;     //显示值
   LuncurStep  = ColumnBackLum.curStep;
   LumdispStep = ColumnBackLum.dispStep;
   
   if(LuncurStep>63)LuncurStep=63;
  if(startLedNumber==0) 
  {  
   ledStep = GetProgressBarcur(progressBar);
   if(ledStep>10) ledStep = 10;
   maskled =(0x01UL)<<ledStep;
   maskled -=1;

   if((ledStep<=progressBar->warnVal)/*&&(progressBar->finalStep<=progressBar->warnVal)*/)  maskled<<=10;
   
   ledStep = GetProgressBarDisp(progressBar);
   if(ledStep>10) ledStep = 10;
   oldmaskled =(0x01UL)<<ledStep;
   oldmaskled -=1;
  //  if(ledStep>10) oldmaskled&=0xfc00;
    if((ledStep<=progressBar->warnVal)/*&&(progressBar->finalStep<=progressBar->warnVal)*/) oldmaskled<<=10;
   
  //  if((lightLevel!=USR_s_DeviceConfigData.LightLevel)) 
  // {
  //   oldmaskled = 0;
  //   lightLevel=USR_s_DeviceConfigData.LightLevel;
  // }
    
    if(LuncurStep !=LumdispStep )
    {
      oldmaskled = 0;
     
  //   lightLevel=USR_s_DeviceConfigData.LightLevel;
    }
   IS31fL_SetColumnLed(oldmaskled,maskled,startLedNumber,&PWM_Gama16[LuncurStep]);
   
   progressBar->dispStep =  progressBar->curStep;
  }
  else
  {
   ledStep = GetProgressBarcur(progressBar);//当前
   if(ledStep>10) ledStep = 10;
   maskled =(0xfffffc00UL)>>ledStep;
   maskled&=0x03ff;  
   //maskled -=1;

   if((ledStep<=progressBar->warnVal)/*&&(progressBar->finalStep<=progressBar->warnVal)*/)  
   {
     maskled<<=6;
     maskled&=0xfc00;
   }
   
   ledStep = GetProgressBarDisp(progressBar);
   if(ledStep>10) ledStep = 10;
   
   oldmaskled =(0xfffffc00UL)>>ledStep;
   oldmaskled&=0x03ff;
   

  //  if(ledStep>10) oldmaskled&=0xfc00;
    if((ledStep<=progressBar->warnVal)/*&&(progressBar->finalStep<=progressBar->warnVal)*/) 
    {
      oldmaskled<<=6;
      oldmaskled&=0xfc00;
    }
   
   // if(lightLevel!=USR_s_DeviceConfigData.LightLevel) 
   //{
   //  oldmaskled = 0;
   //  lightLevel=USR_s_DeviceConfigData.LightLevel;
   //}
    if(LuncurStep !=LumdispStep )
    {
      oldmaskled = 0;
      ColumnBackLum.dispStep =  ColumnBackLum.curStep;
  //   lightLevel=USR_s_DeviceConfigData.LightLevel;
    }
   IS31fL_SetColumnLed(oldmaskled,maskled,startLedNumber,&PWM_Gama16[LuncurStep]);
   
   progressBar->dispStep =  progressBar->curStep;
  
  }
    
   
   
 }
 
 void SetLiftColumnLed(void)
 {
    UINT8  dot;
    UINT8  data;
    data = getDisParameter(MotorStepPrm[0].type,MotorStepPrm[0].Index,&dot);
    //省略量程那些
    data /= 10;
    SetProgressBarFinal(&LiftColumnLed, data);
 
 }
 
 void SetRightColumnLed(void)
 {
    UINT8  dot;
    UINT32  data;
    data = getDisParameter(MotorStepPrm[1].type,MotorStepPrm[1].Index,&dot);
    //省略量程那些
    data /= 10;
    SetProgressBarFinal(&RightColumnLed, data);
 
 }
 
 
 void SetColumnBackLum(uint8 lum)
 {
   if(lum<10) lum = 10;
   SetProgressBarFinal(&ColumnBackLum, lum);
 }
   
  // St_ProgressBar  LiftColumnLed, RightColumnLed;
 
 
 
#endif

//St_BacklightData