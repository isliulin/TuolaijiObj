#ifndef _MY_VECTORS_H
#define _MY_VECTORS_H

#include <mc9s12xhz256.h>
#include <hidef.h>
#include <start12.h>

#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */

void _EntryPoint(void);

#pragma CODE_SEG DEFAULT

#endif