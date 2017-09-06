#undef	EXT
#ifdef	M41T00_C
#define	EXT
#else
#define	EXT extern
#endif
/* common defines and macros */
#include <hidef.h>      	
#include <mc9s12xhz512.h>
#include	"typedef.h"
///location of timer in RealTimeCnt[6]
#define  YEAR                0x00
#define  MONTH               0x01
#define  DAY                 0x02
#define  HOUR                0x03
#define  MINUTE              0x04
#define  SECOND              0x05

/****************ram macros***************************/
EXT		unsigned char	IICDataBuf[8];
EXT		unsigned char  RealTimeCnt[6];  
EXT		unsigned char	year_h,year_l;
EXT		unsigned char	month_h,month_l;
EXT		unsigned char	date_h,date_l;
EXT		unsigned char	hour_h,hour_l;
EXT		unsigned char  minute_h,minute_l;
EXT		unsigned char	second_h,second_l;
EXT		unsigned char	delay250ms;
EXT  	unsigned char  OldMinute; 
EXT  	unsigned char  OldSecond; 
EXT		M4100_TIME	real_time;
#ifdef	M41T00_C
uchar monthday[12]={31,28,31,30,31,30,31,31,30,31,30,31};

#endif
/****************fution macros************************/
#pragma CODE_SEG  USER_ROM
void M41T00_IO_MACROS(void);
void M41T00_SCL_delay(void);
void M41T00_IICstart(void);
void M41T00_IICstop(void);
unsigned char M41T00_IICwaitack(void);
void M41T00_IICsendack(void);
void M41T00_IICsendnotack(void);
void M41T00_IICsendbyte(unsigned char csendbyte);
unsigned char M41T00_IICreceivebyte(void);
//EXT	void	M41T00_register_write(unsigned char internal_addr,byte *sp,unsigned char data_len);
void  M41T00_register_read(unsigned char internal_addr,unsigned char *dp,unsigned char data_len);
void	TimeSet(void);
//EXT	void	TimeRead(void);
void updataRealTime(void);

uchar month_n_day(uint year,uchar month);

unsigned char writeClendarTime(unsigned char *data);
unsigned char readClendarAndTime(unsigned char *data);
void updateRtc(void);
static void hourCome(void);
static void dayCome(void);

#define M41T00_time_read(dp)		M41T00_register_read(0x00,dp,0x07)
#define M41T00_time_set(sp)		M41T00_register_write(0x00,sp,0x07)
#define M41T00_time_init(sp)		M41T00_register_write(0x0f,sp,0x01)
#pragma CODE_SEG DEFAULT
/*********parameter macros**********/
#define	M41T00_OK						0x80
#define	M41T00_ERROR					0x00
#define	M41T00_ADDR					0xd0

#define FALSE_            		0x01
#define TRUE_             		0x00
/*********IO macros**********/
#define	M41T00_SCL_IO				DDRC_DDRC5  
#define	M41T00_SCL						PORTC_PC5
#define	M41T00_SDA_IO				DDRC_DDRC4  
#define	M41T00_SDA						PORTC_PC4

#define  IIC_WRITE_ADDR       0xd0
#define  IIC_READ_ADDR        0xd1



