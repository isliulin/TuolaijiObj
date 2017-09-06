
#include "WaitTimer.h"
/*
When running on a 8-bit microcontroller systems,
accessing(Check TimeOver called) to 16-bit timer variables (data type UINT16) 
must be locked against simultaneous access by the timer interrupt 
or the timer task to maintain data consistency. 
With 16-bit microcontroller systems, the macros must be defined but left empty.
*/ 
unsigned char Stop_Time_Cnt = 0;
static volatile UINT32  U32WaitTimerTicks=0;


#define DISABLE_WAIT_TIMER_INT //COP_DISABLE_TIMER_INT /* Disable Timer Interrupts */
#define ENABLE_WAIT_TIMER_INT  //COP_ENABLE_TIMER_INT /* Enable Timer Interrupt   */

/*
return:
        false	    :not timeout
        true		:timeout
note: the func work well when you check the timer in now - mark <0x8000. (x8000=32s when 1tick is 1ms)
      the func may make waiting addtional more 0x8000 ticks (32s when 1tick is 1ms)
*/
BOOLEAN WaitTimerCheckOver(UINT32 waitTimerMarkVariable) {
    UINT32 dif= (UINT32) (U32WaitTimerTicks - waitTimerMarkVariable);
    DISABLE_WAIT_TIMER_INT;
    
    if (dif < 0x80000000) 
    {
        ENABLE_WAIT_TIMER_INT; 
        return TRUE;
    }

    ENABLE_WAIT_TIMER_INT;
    //PRINTF_ARG1("WARNING: WAIT TIME OUT,MORE THAN i% TICKS, SINCE BEGIN WAITING!",waitTimerMarkVariable);
    return FALSE;
}

void WaitTimerStart(UINT32 *waitTimerMarkVariable,UINT32 maxWaitTicks) 
{
    TAR_ASSERT(maxWaitTicks>0);
   // DISABLE_WAIT_TIMER_INT;
    *waitTimerMarkVariable=U32WaitTimerTicks+maxWaitTicks;
  //  ENABLE_WAIT_TIMER_INT;
}

void User_TimerInterrupt(void)
{
  if(Stop_Time_Cnt==0)  
  {
    
      U32WaitTimerTicks++;
      //if(U32WaitTimerTicks>=0x80000000) U32WaitTimerTicks = 0;
  #if (WAIT_TIMER_USING_SECONDS_CLOCK==1)
      if(U16WaitTimerMilisecons>=ONE_SECOND_TICKS) {
          U8WaitTimerSecons++;
          U16WaitTimerMilisecons=0;
      }else{
          U16WaitTimerMilisecons++;
      }
  #endif//WAIT_TIMER_USING_SECONDS_CLOCK==1  
  }
}

#if 0 
extern void User_TimerInterrupt(void);
/*the set value should be bigger than WAIT_TIMER_RESOLUTION to make sure MAX_WAIT_TIME >0*/
//#define MAX_WAIT_TIME   50/WAIT_TIMER_RESOLUTION 


#if (WAIT_TIMER_USING_SECONDS_CLOCK==1)
#define ONE_SECOND_TICKS    1000/WAIT_TIMER_RESOLUTION
static volatile UINT8   U8WaitTimerSecons=0;
static volatile UINT16  U16WaitTimerMilisecons=0;
#endif

void Timer0IntHander(void)
{
    User_TimerInterrupt();
    PITTF_PTF0=1;//to clear in ISR(0 0 0 0 PTF3 PTF2 PTF1 PTF0)
}






#if (WAIT_TIMER_USING_SECONDS_CLOCK==1)
void WaitTimerStart_Seconds(UINT8 *waitTimerVariable_Seconds,UINT8 maxSeconds) {
    TAR_ASSERT(maxSeconds>0);
    DISABLE_WAIT_TIMER_INT;
    *waitTimerVariable_Seconds=U8WaitTimerSecons+maxSeconds;
    ENABLE_WAIT_TIMER_INT;
}
/*
note: the func work well when you check the timer in now - maxSeconds <0x80. (x80=128s)
      the func may make waiting addtional more 0x80 seconds (128s )
*/
BOOLEAN WaitTimerCheckOver_Seconds(UINT8 waitTimerVariable_Seconds) {
    UINT8 diff;
    DISABLE_WAIT_TIMER_INT;
    diff=(UINT8) (U8WaitTimerSecons - waitTimerVariable_Seconds);
    if ( diff< 0x80) {
        ENABLE_WAIT_TIMER_INT; 
        return TRUE;
    }
    ENABLE_WAIT_TIMER_INT;
    PRINTF_ARG1("WARNING: WAIT TIME OUT,MORE THAN i% TICKS, SINCE BEGIN WAITING!",waitTimerMarkVariable);
    return FALSE;
}
#endif//(WAIT_TIMER_USING_SECONDS_CLOCK==1)

#endif //
