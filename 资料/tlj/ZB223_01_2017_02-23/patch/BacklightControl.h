#ifndef __BACKLIGHTCONTROL_H_
#define __BACKLIGHTCONTROL_H_
#include "HwMacroPatch.h"
#include "ap_S32K1xx_FTM.h"

#define MIN_BACKLINGHT		5
#define MAX_BACKLINGHT		63
#define SET_BACKLIGHT(x)	PRESSURE_PWMVALUE = FTM_PWM_Duty(PRESSURE_BACKLIGHT_FRQ,x)	
unsigned int getValueOPTO(void);
void setBacklightPluse(void);
void BacklightInit(void);
unsigned int getBacklightPluse(unsigned int adV);

typedef struct   //背光和气压灯自动调节
{
 uint8 rtpres;//预分频数值  
 uint8 crtv;  //计数器   
 uint32 finalStep;//目标值
 uint32 curStep;     //当前值
 uint32 dispStep;     //显示值
 uint8  warnVal;      //报警值
 
}St_ProgressBar;

extern St_ProgressBar  LiftColumnLed, RightColumnLed,ColumnBackLum;
extern  void SetColumnLed(St_ProgressBar *progressBar,uint8 startLedNumber);
extern  void SetProgressBarFinal(St_ProgressBar *proBar, uint32 final);
extern void  doProgressBar(St_ProgressBar *proBar);

extern void SetLiftColumnLed(void);
extern void SetRightColumnLed(void);
extern void SetColumnBackLum(uint8 lum);
#endif
