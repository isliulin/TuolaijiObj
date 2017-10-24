#ifndef _TW_I2C_H_
#define _TW_I2C_H_

typedef unsigned char BYTE;

void I2CStart(void);
void I2CWriteData(BYTE value);
BYTE I2CReadData(void);
void I2CStop(void);
void I2Cdelay(void);
BYTE ReadI2C		(BYTE addr, BYTE index);
void WriteI2C(BYTE addr, BYTE index, BYTE val);
void WriteI2CnD(BYTE addr, BYTE index, BYTE val, BYTE cnt);

#endif //_TW_I2C_H_