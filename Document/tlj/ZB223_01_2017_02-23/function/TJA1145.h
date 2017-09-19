//2016-7-7 by L
#ifndef __TJ1145_H_
#define __TJ1145_H_
#include "S32K144.h"
#include "HwMacroPatch.h"
#define spidly(x) //for(int i=0; i<10*x; i++) __asm("nop")




/*System control registers */	
//Mode control register (address 01h) -- 	RW
#define ADDR_SystemControlRegisters 	0x01
#define ADDR_TransceiverEventCaptureEnableRegister  	0x23

	//MC [2:0]
typedef enum {
	CMD_SleepMode = 0x01,
	CMD_StandbyMode = 0x04,
	CMD_NormalMode = 0x07
}TJA1145Mode;
//Main status register (address 03h)
#define ADDR_MainStatusRegister  		0x03
/*CAN partial networking configuration registers*/
//Data rate register (address 26h)
#define ADDR_DataRateRegister  			0x26
#define CMD_250Kb					0x03

/*Event status and event capture registers */
//Global event status register (address 60h)
#define GlobalStatus		0x60
//System event status register (address 61h)
#define SystemStatus		0x61


void SPI0_Init(void);
void SPI0Write(unsigned char dat);
unsigned char spi_read(); 
void TJA1145_ModeChange(TJA1145Mode mode,int loopMax);


#endif
