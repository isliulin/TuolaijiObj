#ifndef __M41T00_H_
#define __M41Too_H_

#include	"typedef.h"
#include "DS1302.h"
#include "common.h"

#define NOP() __asm("nop");__asm("nop");
///location of timer in RealTimeCnt[6]
#define  YEAR                0x00
#define  MONTH               0x01
#define  DAY                 0x02
#define  HOUR                0x03
#define  MINUTE              0x04
#define  SECOND              0x05

/****************ram macros***************************/
EXT		byte	IICDataBuf[8];
EXT		byte  RealTimeCnt[6];  
EXT		byte	delay250ms;
EXT  	byte  OldMinute; 
EXT  	byte  OldSecond; 
EXT UINT8 TimeElement[12];  //12个时间日期元素值，包括year_H,year_L,month_H,month_L,date_H,date_L,hour_H,hour_L,minute_H,minute_L,second_H,second_L.
//EXT		M4100_TIME	real_time;
#ifdef	M41T00_C
extern uchar monthday[12];//={31,28,31,30,31,30,31,31,30,31,30,31};

#endif
/****************fution macros************************/
//#pragma CODE_SEG  USER_ROM
EXT void M41T00_IO_MACROS(void);
EXT void M41T00_SCL_delay(void);
EXT void M41T00_IICstart(void);
EXT void M41T00_IICstop(void);
EXT byte M41T00_IICwaitack(void);
EXT void M41T00_IICsendack(void);
EXT void M41T00_IICsendnotack(void);
EXT void M41T00_IICsendbyte(unsigned char csendbyte);
EXT unsigned char M41T00_IICreceivebyte(void);
EXT void	M41T00_register_write(unsigned char internal_addr,byte * sp,unsigned char data_len);
EXT void	TimeSet(void);
EXT void  updataRealTime(void);
EXT uchar month_n_day(uint year,uchar month);
EXT UINT8 readClendarAndTime(UINT8 *data);
EXT UINT8 writeClendarTime(UINT8 *data);
EXT void dayCome(void);
EXT void hourCome(void);
EXT void updateRtc(void);

#define M41T00_time_read(dp)		M41T00_register_read(0x00,dp,0x07)
#define M41T00_time_set(sp)		M41T00_register_write(0x00,sp,0x07)
#define M41T00_time_init(sp)		M41T00_register_write(0x0f,sp,0x01)
//#pragma CODE_SEG DEFAULT
/*********parameter macros**********/
#define	M41T00_OK						0x80
#define	M41T00_ERROR				0x00
#define	M41T00_ADDR					0xd0
/*********IO macros**********/
#define	M41T00_SCL_IO				DDRC_DDRC5  
#define	M41T00_SCL					PORTC_PC5
#define	M41T00_SDA_IO				DDRC_DDRC4  
#define	M41T00_SDA					PORTC_PC4
#define M41T00_SCL_MUX()   		PORTE->PCR1 = PORT_PCR_MUX(1) 
#define M41T00_SCL_PDDR_OUT  		GPIOE_PDDR |= GPIO_PDDR_PDD(GPIO1) 
#define M41T00_SCL_PDDR_IN   		GPIOE_PDDR &= ~GPIO1
#define M41T00_SCL_H       		GPIOE_PSOR = GPIO1
#define M41T00_SCL_L       		GPIOE_PCOR = GPIO1 
#define M41T00_SDA_MUX()   		PORTE->PCR0 = PORT_PCR_MUX(1) 
#define M41T00_SDA_PDDR_OUT  		GPIOE_PDDR |= GPIO_PDDR_PDD(GPIO0) 
#define M41T00_SDA_PDDR_IN   		GPIOE_PDDR &= ~GPIO0 
#define M41T00_SDA_H       		GPIOE_PSOR = GPIO0
#define M41T00_SDA_L       		GPIOE_PCOR = GPIO0
#define M41T00_SDA_READ    		(GPIOE_PDIR & GPIO0) >> 0


#define  IIC_WRITE_ADDR       0xd0
#define  IIC_READ_ADDR        0xd1

#endif

