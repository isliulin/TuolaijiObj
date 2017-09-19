#define		M41T00_C
#include "M41T00.h"

#define M41T00		0
#define DS1302		1
#define REAL_TIME_CS	DS1302


byte  RealTimeCnt[6]; 
byte  OldSecond; 
byte IICDataBuf[8];
UINT8 TimeElement[12];  //12个时间日期元素值，包括year_H,year_L,month_H,month_L,date_H,date_L,hour_H,hour_L,minute_H,minute_L,second_H,second_L.
uchar monthday[12]={31,28,31,30,31,30,31,31,30,31,30,31};


/*******************************************
* M41T00控制引脚初始化
*******************************************/
void M41T00_IO_MACROS(void)
{ 
	M41T00_SCL_MUX();M41T00_SCL_PDDR_OUT ;//M41T00_SCL_IO = 1;
	M41T00_SCL_L;//M41T00_SCL_L;
	M41T00_SCL_MUX();M41T00_SCL_PDDR_OUT ;//M41T00_SDA_PDDR_OUT; 
	M41T00_SDA_L;//M41T00_SDA_L;
}
/*******************************************
function operation : IIC step control,to produce software SCL
*******************************************/
void M41T00_SCL_delay(void)
{
	uchar i;	
	for(i=0;i<4;i++)
		__asm("nop");	
}
/*********************************************************************
function operation : IIC start condition
function called : M41T00_SCL_delay(void)
return: void 
*********************************************************************/
void M41T00_IICstart(void)
{
	M41T00_SDA_PDDR_OUT;
	__asm("nop");
	M41T00_SDA_H;
	M41T00_SCL_delay();
	M41T00_SCL_H;
	M41T00_SCL_delay();
	M41T00_SDA_L;
	M41T00_SCL_delay();
	M41T00_SCL_L;
	M41T00_SCL_delay();
}
/*********************************************************************
function operation : IIC stop condition
function called :  M41T00_SCL_delay(void)
return : void
*********************************************************************/
void M41T00_IICstop(void)
{
	M41T00_SCL_L;
	M41T00_SCL_delay();
	M41T00_SDA_L;
	M41T00_SCL_delay();
	M41T00_SCL_H;
	M41T00_SCL_delay();
	M41T00_SDA_H;
	M41T00_SCL_delay();
}
/*********************************************************************
function operation : IIC wait ACK
function called : void M41T00_SCL_delay(void),void M41T00_IICstop(void)
return : werther receive ack signal,M41T00_ERROR or M41T00_OK
*********************************************************************/
byte M41T00_IICwaitack(void)
{
	unsigned char cErrTime = 255;

	M41T00_SDA_PDDR_IN;//define as input
	NOP();
	M41T00_SCL_H;
	M41T00_SCL_delay();
	while(M41T00_SDA_READ)
	{
		cErrTime--;
		if (cErrTime==0)
		{
			M41T00_IICstop();
			M41T00_SDA_PDDR_OUT;
			return M41T00_ERROR;
		}		
	}
	M41T00_SCL_L;
	M41T00_SCL_delay();
	M41T00_SDA_PDDR_OUT;	
	return M41T00_OK;	
}
/*********************************************************************
function operation : IIC send ACK
function called : void M41T00_SCL_delay(void)
return : void 
*********************************************************************/
void M41T00_IICsendack(void)
{
	M41T00_SDA_L;
	M41T00_SCL_delay();
	M41T00_SCL_H;
	M41T00_SCL_delay();
	M41T00_SCL_L;
	M41T00_SCL_delay();
}
/*********************************************************************
function operation : IIC send Not ACK
function called : void M41T00_SCL_delay(void)
return : void
*********************************************************************/
void M41T00_IICsendnotack(void)
{
	M41T00_SDA_H;
	M41T00_SCL_delay();
	M41T00_SCL_H;
	M41T00_SCL_delay();
	M41T00_SCL_L;
	M41T00_SCL_delay();
}
/*********************************************************************
function operation : send a byte over IIC bus
function called : void M41T00_SCL_delay(void)
function input parameters : csendbyte,the data needed to send on IIC
return : void
*********************************************************************/
void M41T00_IICsendbyte(unsigned char csendbyte)
{
	unsigned char i = 8;
	while (i--)
	{
		M41T00_SCL_L;
		if(csendbyte&0x80)		
			M41T00_SDA_H;
		else
			M41T00_SDA_L;
		M41T00_SCL_delay();
		M41T00_SCL_H;
		M41T00_SCL_delay();
		csendbyte<<=1;
	}
	M41T00_SCL_L;
	M41T00_SCL_delay();
}
/*********************************************************************
function operation: receive a byte over IIC bus
function called : void M41T00_SCL_delay(void)
input parameter:	void
return : cr_byte,a byte data receive from iic bus
*********************************************************************/
unsigned char M41T00_IICreceivebyte(void)
{
	unsigned char i = 8;
	unsigned char cr_byte = 0;

	M41T00_SDA_PDDR_IN;
	NOP();
	while (i--)
	{
		cr_byte <<= 1;
		M41T00_SCL_delay();
		M41T00_SCL_L;
		M41T00_SCL_delay();
		M41T00_SCL_H;
		if(M41T00_SDA_READ)
		{cr_byte++;}
	}
	M41T00_SCL_delay();
	M41T00_SCL_L;
	M41T00_SCL_delay();
	M41T00_SDA_PDDR_OUT;
	return cr_byte;
}

/*******************************************************************
funtion operation:write a serial data to iic bus
fution call:void M41T00_IICsendbyte(void);byte M41T00_IICwaitack(void);void M41T00_IICstop(void);
input parameter:internal_addr,the first addr of the serial data;*sp;the point data will be sent;data_len;
					the length of the data will be sent 
return:void				
*******************************************************************/
void	M41T00_register_write(unsigned char internal_addr,byte * sp,unsigned char data_len)
{		
	M41T00_IICstart();
	
	M41T00_IICsendbyte(M41T00_ADDR);
	if(M41T00_ERROR == M41T00_IICwaitack())
	{
		M41T00_IICstop();		
		return;
	}
	M41T00_IICsendbyte(internal_addr);
	if(M41T00_ERROR == M41T00_IICwaitack())
	{
		M41T00_IICstop();		
		return;
	}		
	for(;data_len>0;data_len--)
	{
		M41T00_IICsendbyte(*sp);
		if(M41T00_ERROR == M41T00_IICwaitack())
		{	
			M41T00_IICstop();			
			return;
		}
		sp++;
	}	
	M41T00_IICstop();	
}
/***********************************************************************
funtion operation:read a serial data from iic bus
funtion call:	M41T00_IICsendbyte(void);byte M41T00_IICwaitack(void);void M41T00_IICstop(void);
					void M41T00_IICsendack(void);void M41T00_IICsendnotack(void);
input	parameter:internal_addr;the first addr of the serial data;*dp;the point data will be recieved;data_len;
					the length of the data will be recieved
return:void 
***********************************************************************/
/*
void M41T00_register_read(unsigned char internal_addr,byte *dp,unsigned char data_len)
{	
	M41T00_IICstart();
	
	//send M41T00_r to start reading operation
	M41T00_IICsendbyte(M41T00_ADDR);
	if(M41T00_ERROR == M41T00_IICwaitack())
	{
		M41T00_IICstop();
		return;
	}
	
	//send internal address
	M41T00_IICsendbyte(internal_addr);	
	if(M41T00_ERROR == M41T00_IICwaitack())
	{
		M41T00_IICstop();
		return;
	}	
	
	//restart
	M41T00_IICstart();
	M41T00_IICsendbyte(M41T00_ADDR+0x01);
	if(M41T00_ERROR == M41T00_IICwaitack())
	{
		M41T00_IICstop();
		return;
	}
	
	//read data from register,and save to the array pointed by dp
	for(;data_len>0;data_len--)
	{		
		*dp = M41T00_IICreceivebyte();		
		if(data_len !=0x01)
		{M41T00_IICsendack();}
		else
		{M41T00_IICsendnotack();}
		dp++;
	}	
	M41T00_IICstop();	
}
*/
/********************************************
funtion operation:set the m41t00 real timer
funtion call:M41T00_time_set(sp);
input parameter:void
return:void
********************************************/
void	TimeSet(void)
{	
	byte	temp=0;
	M4100_TIME	real_time;
	temp=(TimeElement[0] << 0x04)+ TimeElement[1];
	real_time.year=temp;	
	
	temp=(TimeElement[2] << 0x04)+ TimeElement[3];
	real_time.month=temp;	
	
	temp=(TimeElement[4] << 0x04)+ TimeElement[5];
	real_time.date=temp;	
	
	temp=(TimeElement[6] << 0x04)+ TimeElement[7];
	real_time.hour=temp;	
	
	temp=(TimeElement[8] << 0x04)+ TimeElement[9];
	real_time.minute=temp;	
	
	temp=(TimeElement[10] << 0x04)+ TimeElement[11];
	real_time.second=temp;  
#if REAL_TIME_CS == M41T00
	M41T00_time_set((unsigned char *)&real_time);
#elif REAL_TIME_CS == DS1302
	unsigned char tmp[8];
	tmp[0] = real_time.second;
	tmp[1] = real_time.minute;
	tmp[2] = real_time.hour;
	tmp[3] = real_time.date;
	tmp[4] = real_time.month;
	tmp[5] = 1;
	tmp[6] = real_time.year;
	DS1302BurstWrite(tmp);
#endif
}
/***********************************
funtion operation:read time from m41t00
funtion call:M41T00_time_read(sp);
input parameter:void
return:void
**************************************/
//void	TimeRead(void)
#define M41T00		0
#define DS1302		1
#define REAL_TIME_CS	DS1302
void    updataRealTime(void)
{		
	if(SleepFlag) return;
	FeedWatchDog();
#if REAL_TIME_CS == M41T00
	if(!readClendarAndTime(IICDataBuf))	
	{
		    updateRtc();  
			if(OldSecond != RealTimeCnt[SECOND])
			{									
				OldSecond = RealTimeCnt[SECOND];
			}	
	}
#elif REAL_TIME_CS == DS1302
	unsigned char time[8]; //当前时间数组
	DS1302BurstRead(time); //读取 DS1302 当前时间
	doGetRealTimeCompatible(time,RealTimeCnt);

	if(OldSecond != RealTimeCnt[SECOND])
	{									
		OldSecond = RealTimeCnt[SECOND];
	}	
#endif

}
/******************************************
funtion operation:account the date in the year and month
funtion call:void
input parameter:year;the year will be account;month;the month will be account
return:max_monthday;the month what account result
*******************************************/
uchar month_n_day(uint year,uchar month)
{     
  uchar	max_monthday=0;   
  if(month==2)
  {
    if((year%4==0)&&(year%100!=0)||(year%400 == 0))
      max_monthday=29;
    else
      max_monthday=28;
  }
  else
    max_monthday=monthday[month-1];
     
  return	max_monthday;
}
/*****************************************************************************

*****************************************************************************/
UINT8 readClendarAndTime(UINT8 *data)
{
    UINT8 i;
    M41T00_IICstart();
    M41T00_IICsendbyte(IIC_WRITE_ADDR);
    if(M41T00_IICwaitack()==FALSE_)
    {
        return FALSE_;
    }
    M41T00_IICsendbyte(0x00);              //从秒开始读起。
        if(M41T00_IICwaitack()==FALSE_)
    {
        return FALSE_;
    }
    M41T00_IICstart();
    M41T00_IICsendbyte(IIC_READ_ADDR);
    if(M41T00_IICwaitack()==FALSE_)
    {
        return FALSE_;
    }
    for(i=0;i<6;i++)
    {
        *data++ = M41T00_IICreceivebyte();
        M41T00_IICsendack();
    }
    *data++ = M41T00_IICreceivebyte();
    M41T00_IICsendnotack();
    M41T00_IICstop();
    return  TRUE_;
}

/**********************************************************************************

**********************************************************************************/
UINT8 writeClendarTime(UINT8 *data)
{
    UINT8 i;
    UINT8 j=0x00;
    M41T00_IICstart();
    for(i=0;i<9;i++) 
    {
        j++;
        M41T00_IICsendbyte(*data++);
        if(M41T00_IICwaitack()==FALSE_)
        {
            return FALSE_;
        }        
    }
    M41T00_IICstop();

	updataRealTime();
    return TRUE_;	
}
/***************************************************************************

***************************************************************************/
void dayCome(void)
{
        if(!RealTimeCnt[HOUR])
        {
            if(!RealTimeCnt[MINUTE])
            {
                if(RealTimeCnt[SECOND] == 0x09)
                {
                   if(OldSecond == 0x08)
                   { 
                       _1DAYF = 1; 
                   }
                }
            }
        }
}                                                                      
/***************************************************************************

***************************************************************************/
void hourCome(void)
{
        if(!RealTimeCnt[MINUTE])
        {
            if(RealTimeCnt[SECOND] == 0x05)
            {
                if(OldSecond == 0x04)
                {  
                    _1HOURF = 1; 
                }
            }
        }
}
/********************************************************************************

********************************************************************************/
void updateRtc(void)
{                                                        //2012-12-10
   RealTimeCnt[0] = IICDataBuf[6];         ///year
   RealTimeCnt[1] = IICDataBuf[5] & 0x1f;  ///month
   RealTimeCnt[2] = IICDataBuf[4] & 0x3f;  ///date
   RealTimeCnt[3] = IICDataBuf[2] & 0x3f;  ///hour
   RealTimeCnt[4] = IICDataBuf[1] & 0x7f;  ///minute
   RealTimeCnt[5] = IICDataBuf[0] & 0x7f;  ///second
}
//#pragma CODE_SEG DEFAULT
