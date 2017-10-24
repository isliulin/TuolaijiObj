#ifndef TW_CONTRL_H
#define TW_CONTRL_H

#include "TW_i2c.h"


#define ON  1
#define OFF 0

#define TW88I2CAddress		0x8a	// TW88XX


#define WriteTW88(a,b)			WriteI2C(TW88I2CAddress, a, b)
#define ReadTW88(a)					ReadI2C(TW88I2CAddress, a)
#define WriteTW88Page(b)		WriteTW88(0xff,b)

void	Tw_Pannal_Parameter_Init(void);
void	BackLightCmd(BYTE Cmd);


#endif //TW_CONTRL_H