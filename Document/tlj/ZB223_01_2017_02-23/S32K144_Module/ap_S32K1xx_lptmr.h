#ifndef __ap_S32K1XX_LPTMR_H_
#define __ap_S32K1XX_LPTMR_H_
#include "S32K144.h"
#include "Typedef.h"

//void lptmr_init();
//void lptmr_waitting_us(UINT16 us);
uint8 LPLD_LPTMR_Init(uint8 pluseacc_input);
void lptmr_set_hz(UINT16 hz);

void LPLD_LPTMR_ResetCounter(void);
uint16 LPLD_LPTMR_GetPulseAcc(void);

#endif
