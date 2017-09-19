#ifndef __IFsh1
#define __IFsh1
#include <mc9s12xhz512.h>
#include "type.h"
#include "can0.h"
#include "Isr_vectors.h"

#pragma CODE_SEG IFsh1_CODE            /* Code section for this module. */

void IFsh1_Init(void);
unsigned char IFsh1_SetByteFlash(unsigned long Addr,unsigned char Data);
byte IFsh1_GetByteFlash(dword Addr,byte *Data);
byte IFsh1_SetWordFlash(dword Addr,word Data);
byte IFsh1_GetWordFlash(dword Addr,word *Data);
byte IFsh1_EraseSector(dword Addr);
void FlashInit(void);
static void WriteWord1(word AddrRow,word Data16);
uchar chip_erase(void);
#pragma CODE_SEG DEFAULT               /* Change code section to DEFAULT. */

#endif /* ifndef __IFsh1 */

