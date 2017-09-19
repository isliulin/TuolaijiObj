#include "TJA1145.h"


void SPI0_Init(void){
	SPI0_CS_MUX();
	SPI0_SOUT_MUX();  
	SPI0_SIN_MUX();
	SPI0_SCK_MUX();

	SPI0_CS_PDDR(1)   ;  
	SPI0_SOUT_PDDR(1)  ; 
	SPI0_SIN_PDDR(0)  ;  
	SPI0_SCK_PDDR(1)  ;
	SPI0_CS(0)   ; // 默认片选
	SPI0_SOUT(1)  ;
	SPI0_SCK(0)  ;
}

void SPI0Write(unsigned char dat)
{
//第一数据下降沿  其他下降沿   数据采样 上升沿 
	unsigned char n;
	//SPI0_SCK(0);
	//spidly(4);

	for(n=0;n<8;n++)
	{
	  SPI0_SCK(1);
	  spidly(4);
	  if(dat&0x80) SPI0_SOUT(1);
	  else SPI0_SOUT(0);
	  spidly(4);
	  dat<<=1;
	  SPI0_SCK(0);
		spidly(4);
	}
	SPI0_SCK(0); 

	//return rx_dat;
}

unsigned char spi_read()  
{  
	unsigned char i,spi_dat=0; 
	//SPI0_SCK(0);
	//
	for (i=0;i<8;i++)  
	{   	__asm("nop");//                               
		SPI0_SCK(1);  
		//spidly(4);
		spi_dat=(spi_dat<<1);  
		
		spidly(4);
		SPI0_SCK(0);
		spidly(4);

		if(SPI0_SIN())spi_dat|=0x01;  
		else spi_dat&=~0x01; 
		spidly(4);
		 
	}  
	SPI0_SCK(0); 
	return spi_dat;  
}

UINT8 readRegister(UINT8 address){
	SPI0_CS(0);
	SPI0Write( (address<<1) | 0x01);
	
	SPI0_CS(1);
	return  spi_read();
}
void writeRegister(UINT8 address,UINT8 data){
	SPI0_CS(0);
	SPI0Write( (address<<1));
	SPI0Write(data);
	SPI0_CS(1);
}


UINT8 status;
void TJA1145_ModeChange(TJA1145Mode mode,int loopMax){
	//status = readRegister(0x01);
	
	switch(mode){
		case CMD_NormalMode:
			writeRegister(ADDR_SystemControlRegisters,(UINT8)CMD_NormalMode);
			break;
		case CMD_SleepMode:
			writeRegister(ADDR_TransceiverEventCaptureEnableRegister,0);
			writeRegister(ADDR_SystemControlRegisters,(UINT8)CMD_SleepMode);
			break;
		case CMD_StandbyMode:
			writeRegister(ADDR_TransceiverEventCaptureEnableRegister,0);
			writeRegister(ADDR_SystemControlRegisters,(UINT8)CMD_StandbyMode);
			break;
		default:break;
	}
	status = readRegister(0x01);
	if(status == mode) 
          return;
	loopMax--;
	if(loopMax <= 0) return;
		TJA1145_ModeChange(mode,loopMax);
}
