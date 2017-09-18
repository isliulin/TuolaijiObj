
#include "Isr_vectors.h"

#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */
///__interrupt void UnimplementedISR(void) 
///{
  /// _EntryPoint;
///}       
typedef void (*near tIsrFunc)(void);
 /* Reset vector table */
const tIsrFunc _ResetVectorTable[] @0xFFFA = { 
  /* Reset handler name                   Address  Name            Description */
  _EntryPoint,                          /* 0xFFFA  ivVcop          unused by PE */
  _EntryPoint,                          /* 0xFFFC  ivVclkmon       unused by PE */
  _EntryPoint                           /* 0xFFFE  ivVreset        used by PE */
};
//////////////////////////////////////////////////
void _EntryPoint(void)
{
  ECLKCTL=192; 
  MMCCTL1=1;
  DIRECT=0;
  IVBR=0xff;
  CLKSEL =0x00;
  PLLCTL=48;
  __asm jmp _Startup;                   /* Jump to C startup code */
}
#pragma CODE_SEG DEFAULT

