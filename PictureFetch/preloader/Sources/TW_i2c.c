#include "TW_i2c.h"
  
#include <mc9s12xhz256.h>

#pragma CODE_SEG  USER_ROM

  
#define	I2C_SCL						PTT_PTT5
#define	I2C_SCL_IO				DDRT_DDRT5
#define	I2C_SDA						PTT_PTT5
#define	I2C_SDA_IO				DDRT_DDRT4


#define I2CDelayNum 2



#define dd()   I2Cdelay()
//////////////////////////////////
void I2Cdelay(void)
{
	BYTE i;	
	for(i=0; i<I2CDelayNum; i++)
		asm nop;
}
///////////////////////////////////
void I2CStart()
{
	I2C_SCL_IO=1;
	I2C_SDA_IO=1;
	
	I2C_SDA = 1;	dd();
	I2C_SCL = 1;	dd();
	I2C_SDA = 0;	dd();
	I2C_SCL = 0;	
}


void I2CStop()
{
	I2C_SDA = 0;	dd();
	I2C_SCL = 1;
	I2C_SDA = 0;	dd();
	I2C_SCL = 1;	dd();
	I2C_SDA = 1;
}


void I2CWriteData(BYTE value) 
{
	I2C_SCL_IO=1;
	I2C_SDA_IO=1;

	I2C_SCL=0;	I2C_SDA=(value & 0x80)? 1:0;	I2C_SCL=1;	dd();
	I2C_SCL=0;	I2C_SDA=(value & 0x40)? 1:0;	I2C_SCL=1;	dd();
	I2C_SCL=0;	I2C_SDA=(value & 0x20)? 1:0;	I2C_SCL=1;	dd();
	I2C_SCL=0;	I2C_SDA=(value & 0x10)? 1:0;	I2C_SCL=1;	dd();

	I2C_SCL=0;	I2C_SDA=(value & 0x08)? 1:0;	I2C_SCL=1;	dd();
	I2C_SCL=0;	I2C_SDA=(value & 0x04)? 1:0;	I2C_SCL=1;	dd();
	I2C_SCL=0;	I2C_SDA=(value & 0x02)? 1:0;	I2C_SCL=1;	dd();
	I2C_SCL=0;	I2C_SDA=(value & 0x01)? 1:0;	I2C_SCL=1;	dd();


	I2C_SCL = 0;	
	I2C_SCL = 0;	
	I2C_SCL = 0;	
	I2C_SDA = 1;	

	I2C_SCL = 1;	
	I2C_SCL = 1;	
	I2C_SCL = 1;	
	dd();
	I2C_SCL = 0;

}

BYTE I2CReadData()
{
	BYTE i;
	BYTE value=0;
	
	I2C_SDA_IO=0;				///as input
	
	I2C_SCL = 0;
	
	for(i=0; i<8; i++) {
		I2C_SCL = 1;
		value <<= 1;
		if(I2C_SDA) value |= 1;
		I2C_SCL = 0;
	}
	
	I2C_SCL = 1;	
	I2C_SCL = 1;	
	I2C_SCL = 1;
	I2C_SCL = 0;
	
	I2C_SDA_IO=1;			///as output
	
	return value;

}

void WriteI2C(BYTE addr, BYTE index, BYTE val)
{
	I2CStart();
	I2CWriteData(addr);
	I2CWriteData(index);
	I2CWriteData(val);
	I2CStop();
}
///////////////////////////////
BYTE ReadI2C(BYTE addr, BYTE index)
{
	BYTE val;

	I2CStart();
	I2CWriteData(addr);
	I2CWriteData(index);
	I2CStart();
	I2CWriteData(addr | 0x01);
	val = I2CReadData();
	I2CStop();

	return val;
}
/////////////////////////

























