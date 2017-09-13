#ifndef _MY_VECTORS_H
#define _MY_VECTORS_H

#include <MC9S12XET256.h>

#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */


void _EntryPoint(void);
void _UserEntry (void);

#pragma CODE_SEG DEFAULT

#endif