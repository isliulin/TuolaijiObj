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

typedef struct   //�������ѹ���Զ�����
{
 uint8 rtpres;//Ԥ��Ƶ��ֵ  
 uint8 crtv;  //������   
 uint32 finalStep;//Ŀ��ֵ
 uint32 curStep;     //��ǰֵ
 uint32 dispStep;     //��ʾֵ
 uint8  warnVal;      //����ֵ
 
}St_ProgressBar;

extern St_ProgressBar  LiftColumnLed, RightColumnLed,ColumnBackLum;
extern  void SetColumnLed(St_ProgressBar *progressBar,uint8 startLedNumber);
extern  void SetProgressBarFinal(St_ProgressBar *proBar, uint32 final);
extern void  doProgressBar(St_ProgressBar *proBar);

extern void SetLiftColumnLed(void);
extern void SetRightColumnLed(void);
extern void SetColumnBackLum(uint8 lum);
#endif
