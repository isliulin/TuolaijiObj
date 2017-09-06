/*
 * File:       evb_s32k144.h
 * Purpose:     Definitions for the S32K144 EVB *
 * Notes:
 */

#ifndef __S32K144_EVB_H__
#define __S32K144_EVB_H__

#include "arm_cm4.h"
//#include "mcg.h"

/********************************************************************/

/* Global defines to use for all boards */
#define DEBUG_PRINT


/* Defines specific */


/* Define for the CPU */

#define CPU_S32K144

/*
 * System Bus Clock Info
 */
#define CLK0_FREQ_HZ        8000000
#define CLK0_TYPE           CRYSTAL



#define TERM_PORT           LPUART1_BASE_PTR
#define TERMINAL_BAUD       115200
#undef  HW_FLOW_CONTROL



#endif /* __S32K144_EVB_H__ */
