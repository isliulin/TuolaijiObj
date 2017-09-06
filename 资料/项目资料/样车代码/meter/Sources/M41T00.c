#define		M41T00_C
#include	"config.h"
#pragma CODE_SEG  USER_ROM
/*******************************************

*******************************************/
void M41T00_IO_MACROS(void)
{ 
	M41T00_SCL_IO = 1;
	M41T00_SCL = 0;
	M41T00_SDA_IO = 1; 
	M41T00_SDA = 0;
}
/*******************************************
function operation : IIC step control,to produce software SCL
*******************************************/
void M41T00_SCL_delay(void)
{
	uchar i;
	
	for(i=0;i<4;i++)
		NOP();	
}
/*********************************************************************
function operation : IIC start condition
function called : M41T00_SCL_delay(void)
return: void 
*********************************************************************/
void M41T00_IICstart(void)
{
	M41T00_SDA_IO = 1;
	NOP();
	M41T00_SDA = 1;
	M41T00_SCL_delay();
	M41T00_SCL = 1;
	M41T00_SCL_delay();
	M41T00_SDA = 0;
	M41T00_SCL_delay();
	M41T00_SCL = 0;
	M41T00_SCL_delay();
}
/*********************************************************************
function operation : IIC stop condition
function called :  M41T00_SCL_delay(void)
return : void
*********************************************************************/
void M41T00_IICstop(void)
{
	M41T00_SCL = 0;
	M41T00_SCL_delay();
	M41T00_SDA = 0;
	M41T00_SCL_delay();
	M41T00_SCL = 1;
	M41T00_SCL_delay();
	M41T00_SDA = 1;
	M41T00_SCL_delay();
}
/*********************************************************************
function operation : IIC wait ACK
function called : void M41T00_SCL_delay(void),void M41T00_IICstop(void)
return : werther receive ack signal,M41T00_ERROR or M41T00_OK
*********************************************************************/
unsigned char M41T00_IICwaitack(void)
{
	unsigned char cErrTime = 255;

	M41T00_SDA_IO = 0;//define as input
	NOP();
	M41T00_SCL = 1;
	M41T00_SCL_delay();
	while(M41T00_SDA)
	{
		cErrTime--;
		if (cErrTime==0)
		{
			M41T00_IICstop();
			M41T00_SDA_IO = 1;
			return M41T00_ERROR;
		}		
	}
	M41T00_SCL = 0;
	M41T00_SCL_delay();
	M41T00_SDA_IO = 1;	
	return M41T00_OK;	
}
/*********************************************************************
function operation : IIC send ACK
function called : void M41T00_SCL_delay(void)
return : void 
*********************************************************************/
void M41T00_IICsendack(void)
{
	M41T00_SDA = 0;
	M41T00_SCL_delay();
	M41T00_SCL = 1;
	M41T00_SCL_delay();
	M41T00_SCL = 0;
	M41T00_SCL_delay();
}
/*********************************************************************
function operation : IIC send Not ACK
function called : void M41T00_SCL_delay(void)
return : void
*********************************************************************/
void M41T00_IICsendnotack(void)
{
	M41T00_SDA = 1;
	M41T00_SCL_delay();
	M41T00_SCL = 1;
	M41T00_SCL_delay();
	M41T00_SCL = 0;
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
		M41T00_SCL = 0;
		if(csendbyte&0x80)		
			M41T00_SDA=1;
		else
			M41T00_SDA=0;
		M41T00_SCL_delay();
		M41T00_SCL = 1;
		M41T00_SCL_delay();
		csendbyte<<=1;
	}
	M41T00_SCL = 0;
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

	M41T00_SDA_IO = 0;
	NOP();
	while (i--)
	{
		cr_byte <<= 1;
		M41T00_SCL_delay();
		M41T00_SCL = 0;
		M41T00_SCL_delay();
		M41T00_SCL = 1;
		if(M41T00_SDA)
		{cr_byte++;}
	}
	M41T00_SCL_delay();
	M41T00_SCL = 0;
	M41T00_SCL_delay();
	M41T00_SDA_IO = 1;
	return cr_byte;
}

/*******************************************************************
funtion operation:write a serial data to iic bus
fution call:void M41T00_IICsendbyte(void);byte M41T00_IICwaitack(void);void M41T00_IICstop(void);
input parameter:internal_addr,the first addr of the serial data;*sp;the point data will be sent;data_len;
					the length of the data will be sent 
return:void				
*******************************************************************/
void	M41T00_register_write(unsigned char internal_addr,unsigned char *sp,unsigned char data_len)
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
	
	temp=(year_h<<4)+year_l;
	real_time.year=temp;	
	
	temp=(month_h<<4)+month_l;
	real_time.month=temp;	
	
	temp=(date_h<<4)+date_l;
	real_time.date=temp;	
	
	temp=(hour_h<<4)+hour_l;
	real_time.hour=temp;	
	
	temp=(minute_h<<4)+minute_l;
	real_time.minute=temp;	
	
	temp=(second_h<<4)+second_l;
	real_time.second=temp;

	M41T00_time_set(&real_time.second);
}
/***********************************
funtion operation:read time from m41t00
funtion call:M41T00_time_read(sp);
input parameter:void
return:void
**************************************/
//void	TimeRead(void)
void    updataRealTime(void)
{	
	if(M41t00StopFlag==1||(OnKeyBit==KeyOff))					//when modify the real timer ,let it not to running
		return;
	
	if(!readClendarAndTime(IICDataBuf))	
	{
		  updateRtc();  
			if(OldSecond != RealTimeCnt[SECOND])
			{									
				OldSecond = RealTimeCnt[SECOND];
			}
			
			if(SystemDataTimeSave_Flag==0)
			{
				year_h=(RealTimeCnt[YEAR]&0xf0)>>4;	
				year_l=RealTimeCnt[YEAR]&0x0f;
		
				month_h=(RealTimeCnt[MONTH]&0x10)>>4;
				month_l=RealTimeCnt[MONTH]&0x0f;
		
				date_h=(RealTimeCnt[DAY]&0x30)>>4;
				date_l=RealTimeCnt[DAY]&0x0f;
			}
	
			hour_h=(RealTimeCnt[HOUR]&0x30)>>4;
			hour_l=RealTimeCnt[HOUR]&0x0f;	
	
			minute_h=(RealTimeCnt[MINUTE]&0x70)>>4; 
			minute_l=RealTimeCnt[MINUTE]&0x0f;
	
			second_h=(RealTimeCnt[SECOND]&0x70)>>4;
			second_l=RealTimeCnt[SECOND]&0x0f;	
	}
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
unsigned char readClendarAndTime(unsigned char *data)
{
    unsigned char i;
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
unsigned char writeClendarTime(unsigned char *data)
{
    unsigned char i;
    unsigned char j=0x00;
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
/********************************************************************************

********************************************************************************/
void updateRtc(void)
{
    RealTimeCnt[0] = IICDataBuf[6];         ///year
    RealTimeCnt[1] = IICDataBuf[5] & 0x1f;  ///month
    RealTimeCnt[2] = IICDataBuf[4] & 0x3f;  ///date
    RealTimeCnt[3] = IICDataBuf[2] & 0x3f;  ///hour
    RealTimeCnt[4] = IICDataBuf[1] & 0x7f;  ///minute
    RealTimeCnt[5] = IICDataBuf[0] & 0x7f;  ///second
}
#pragma CODE_SEG DEFAULT
