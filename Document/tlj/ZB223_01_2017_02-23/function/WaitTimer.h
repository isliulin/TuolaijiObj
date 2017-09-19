#ifndef _WAITTIMER_H
#define _WAITTIMER_H
#include "typedef.h"

#define TAR_ASSERT(Expr)
/*
how to using:
 UINT16 waitTimerMarkVar;
 WaitTimerStart(&waitTimerMarkVar,x/WAIT_TIMER_RESOLUTION );//wait ms at max
do something....
if(WaitTimerCheckOver(waitTimerMarkVar))....Time Out
*/
/*
UINT16 waitTimerMarkVar=100/WAIT_TIMER_RESOLUTION;
void AppFunctions(void){
    // check if 100ms are over 
    if (WaitTimerCheckOver(waitTimerMarkVar)) {
       //do something
        WaitTimerStart(&waitTimerMarkVar,100/WAIT_TIMER_RESOLUTION);//wait ms at max
    }
*/
/*the period of the timer interrupt in ms,refer to the timer interrupt function and adjust the value*/

#define WAIT_TIMER_USING_SECONDS_CLOCK  0
extern unsigned char Stop_Time_Cnt;

#define WAIT_TIMER_RESOLUTION   TAR_k_TIMER_RESOLUTION 

void WaitTimerStart(UINT32 *waitTimerMarkVariable,UINT32 maxWaitTicks); 
void Timer0IntHander(void);
void User_TimerInterrupt(void);

BOOLEAN WaitTimerCheckOver(UINT32 waitTimerMarkVariable);

#if WAIT_TIMER_USING_SECONDS_CLOCK==1
void    WaitTimerStart_Seconds(UINT8 *waitTimerVariable_Seconds,UINT8 maxSeconds);
BOOLEAN WaitTimerCheckOver_Seconds(UINT8 waitTimerVariable_Seconds);
#endif //WAIT_TIMER_USING_SECONDS_CLOCK

#endif
