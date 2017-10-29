#define 	EEPROM_C
#include	"typedef.h"
#pragma CODE_SEG  USER_ROM
/******************************************************************************************
 *******************************************************************************************/
static void Eeprom_IO_MACROS(void)
{
  Eeprom_SCL_IO = 1;
  Eeprom_SCL = 0;
  Eeprom_SDA_IO = 1;
  Eeprom_SDA = 0;
}

/*******************************************
function operation : IIC step control,to produce software SCL
 *******************************************/
static void Eeprom_SCL_delay(void)
{
  uchar i;

	#if	OSC_Number==16	
  	for (i = 0; i < 10; i++)
	#else if OSC_Number==8
		for (i = 0; i < 5; i++)
	#endif
    asm nop;
}

/******************************************************************************************
 *******************************************************************************************/
static void ReWrite_Delay(void)
{
  unsigned int i;

#if OSC_Number==16				///11.08.12更改，延长页写后写第二页的时间，避免第二页无ACK的发生。
  	for (i = 0; i < 2000; i++)
#else if OSC_Number==8
  for (i = 0; i < 1000; i++)///in "600" can well;
#endif
    asm nop;	
}

/*********************************************************************
function operation : IIC start condition
function called : Eeprom_SCL_delay(void)
return: void
 *********************************************************************/
static void Eeprom_IICstart(void)
{
  Eeprom_SDA_IO = 1;
  asm nop;
  Eeprom_SDA = 1;
  Eeprom_SCL_delay();
  Eeprom_SCL = 1;
  Eeprom_SCL_delay();
  Eeprom_SDA = 0;
  Eeprom_SCL_delay();
  Eeprom_SCL = 0;
  Eeprom_SCL_delay();
}

/*********************************************************************
function operation : IIC stop condition
function called :  Eeprom_SCL_delay(void)
return : void
 *********************************************************************/
static void Eeprom_IICstop(void)
{
  Eeprom_SCL = 0;
  Eeprom_SCL_delay();
  Eeprom_SDA = 0;
  Eeprom_SCL_delay();
  Eeprom_SCL = 1;
  Eeprom_SCL_delay();
  Eeprom_SDA = 1;
  Eeprom_SCL_delay();
}

/*********************************************************************
function operation : IIC wait ACK
function called : void Eeprom_SCL_delay(void),void Eeprom_IICstop(void)
return : werther receive ack signal,Eeprom_ERROR or Eeprom_OK
 *********************************************************************/
static byte Eeprom_IICwaitack(void)
{
  unsigned char cErrTime = 255;

  Eeprom_SDA_IO = 0; //define as input
  asm nop;
  Eeprom_SCL = 1;
  Eeprom_SCL_delay();
  while (Eeprom_SDA)
  {
    cErrTime--;
    if (cErrTime == 0)
    {
      Eeprom_IICstop();
      Eeprom_SDA_IO = 1;
      return Eeprom_ERROR;
    }
  }
  Eeprom_SCL = 0;
  Eeprom_SCL_delay();
  Eeprom_SDA_IO = 1;
  return Eeprom_OK;
}

/*********************************************************************
function operation : IIC send ACK
function called : void Eeprom_SCL_delay(void)
return : void
 *********************************************************************/
static void Eeprom_IICsendack(void)
{
  Eeprom_SDA = 0;
  Eeprom_SCL_delay();
  Eeprom_SCL = 1;
  Eeprom_SCL_delay();
  Eeprom_SCL = 0;
  Eeprom_SCL_delay();
}

/*********************************************************************
function operation : IIC send Not ACK
function called : void Eeprom_SCL_delay(void)
return : void
 *********************************************************************/
static void Eeprom_IICsendnotack(void)
{
  Eeprom_SDA = 1;
  Eeprom_SCL_delay();
  Eeprom_SCL = 1;
  Eeprom_SCL_delay();
  Eeprom_SCL = 0;
  Eeprom_SCL_delay();
}

/*********************************************************************
function operation : send a byte over IIC bus
function called : void Eeprom_SCL_delay(void)
function input parameters : csendbyte,the data needed to send on IIC
return : void
 *********************************************************************/
static void Eeprom_IICsendbyte(unsigned char csendbyte)
{
  unsigned char i = 8;
  while (i--)
  {
    Eeprom_SCL = 0;
    if (csendbyte &0x80)
      Eeprom_SDA = 1;
    else
      Eeprom_SDA = 0;
    Eeprom_SCL_delay();
    Eeprom_SCL = 1;
    Eeprom_SCL_delay();
    csendbyte <<= 1;
  }
  Eeprom_SCL = 0;
  Eeprom_SCL_delay();
}

/*********************************************************************
function operation: receive a byte over IIC bus
function called : void Eeprom_SCL_delay(void)
input parameter:	void
return : cr_byte,a byte data receive from iic bus
 *********************************************************************/
static unsigned char Eeprom_IICreceivebyte(void)
{
  unsigned char i = 8;
  unsigned char cr_byte = 0;

  Eeprom_SDA_IO = 0;
  asm nop;
  while (i--)
  {
    cr_byte <<= 1;
    Eeprom_SCL_delay();
    Eeprom_SCL = 0;
    Eeprom_SCL_delay();
    Eeprom_SCL = 1;
    if (Eeprom_SDA)
    {
      cr_byte++;
    }
  }
  Eeprom_SCL_delay();
  Eeprom_SCL = 0;
  Eeprom_SCL_delay();
  Eeprom_SDA_IO = 1;
  return cr_byte;
}

/*******************************************************************
funtion operation:write a serial data to iic bus
fution call:void Eeprom_IICsendbyte(void);byte Eeprom_IICwaitack(void);void Eeprom_IICstop(void);
input parameter:internal_addr,the first addr of the serial data;*sp;the point data will be sent;data_len;
the length of the data will be sent
return:void				
 *******************************************************************/
static unsigned char Eeprom_register_write(unsigned char internal_addr, unsigned char
  *sp, unsigned char data_len)
{
  Eeprom_IICstart();

  //send Eeprom_W to start writing operation
  Eeprom_IICsendbyte(Eeprom_ADDR);
  if (Eeprom_ERROR == Eeprom_IICwaitack())
  {
    Eeprom_IICstop();
    return Eeprom_ERROR;
  }
  //send internal address
  Eeprom_IICsendbyte(internal_addr);
  if (Eeprom_ERROR == Eeprom_IICwaitack())
  {
    Eeprom_IICstop();
    return Eeprom_ERROR;
  }
  //send the data needed to written into register
  for (; data_len > 0; data_len--)
  {
    Eeprom_IICsendbyte(*sp);
    if (Eeprom_ERROR == Eeprom_IICwaitack())
    {
      Eeprom_IICstop();
      return Eeprom_ERROR;
    }
    sp++;
  }
  Eeprom_IICstop();
  ReWrite_Delay();
  return Eeprom_OK;
}

/***********************************************************************
funtion operation:read a serial data from iic bus
funtion call:	Eeprom_IICsendbyte(void);byte Eeprom_IICwaitack(void);void Eeprom_IICstop(void);
void Eeprom_IICsendack(void);void Eeprom_IICsendnotack(void);
input	parameter:internal_addr;the first addr of the serial data;*dp;the point data will be recieved;data_len;
the length of the data will be recieved
return:void
 ***********************************************************************/
static unsigned char Eeprom_register_read(unsigned char internal_addr, unsigned char *dp,
  unsigned char data_len)
{
  Eeprom_IICstart();

  //send Eeprom_r to start reading operation
  Eeprom_IICsendbyte(Eeprom_ADDR);
  if (Eeprom_ERROR == Eeprom_IICwaitack())
  {
    Eeprom_IICstop();
    return Eeprom_ERROR;
  }

  //send internal address
  Eeprom_IICsendbyte(internal_addr);
  if (Eeprom_ERROR == Eeprom_IICwaitack())
  {
    Eeprom_IICstop();
    return Eeprom_ERROR;
  }

  //restart
  Eeprom_IICstart();
  Eeprom_IICsendbyte(Eeprom_ADDR + 0x01);
  if (Eeprom_ERROR == Eeprom_IICwaitack())
  {
    Eeprom_IICstop();
    return Eeprom_ERROR;
  }

  //read data from register,and save to the array pointed by dp
  for (; data_len > 0; data_len--)
  {
    *dp = Eeprom_IICreceivebyte();
    if (data_len != 0x01)
    {
      Eeprom_IICsendack();
    }
    dp++;
  }
  Eeprom_IICsendnotack();
  Eeprom_IICstop();
  return Eeprom_OK;
}

/******************************************************************************************
 *******************************************************************************************/
static unsigned char Eeprom_Page_Write(unsigned char internal_addr, unsigned char *sp,
  unsigned char data_len)
{
  unsigned char eep_page_remain = 0; //the space remain in this page
  unsigned int eep_start_page;
    //the first page which will be wriiten in this operation

  eep_start_page = internal_addr / EEP_24CXX_PAGE_SIZE;
  eep_page_remain = (unsigned long)EEP_24CXX_PAGE_SIZE *(eep_start_page + 1) -
    internal_addr; //calculate the space remained

  if (eep_page_remain >= data_len)
    //if the residual space of this page is larger than the data needed to be written
  {
    if (Eeprom_ERROR == Eeprom_register_write(internal_addr, sp, data_len))
    {
      return Eeprom_ERROR;
    }

    sp += data_len;
    internal_addr += data_len;
    data_len = 0;
  }
  else
  //else full fill the space of this page first
  {
    if (Eeprom_ERROR == Eeprom_register_write(internal_addr, sp,
      eep_page_remain))
    {
      return Eeprom_ERROR;
    }

    sp += eep_page_remain;
    internal_addr += eep_page_remain;
    data_len -= eep_page_remain;
  }

  //write the residual data
  while (data_len > 0x00)
  //write the data page by page
  {
    if (data_len >= EEP_24CXX_PAGE_SIZE)
    {
      if (Eeprom_ERROR == Eeprom_register_write(internal_addr, sp,
        EEP_24CXX_PAGE_SIZE))
      {
        return Eeprom_ERROR;
      }

      sp += EEP_24CXX_PAGE_SIZE;
      internal_addr += EEP_24CXX_PAGE_SIZE;
      data_len -= EEP_24CXX_PAGE_SIZE;
    }
    else
    {
      if (Eeprom_ERROR == Eeprom_register_write(internal_addr, sp, data_len))
      {
        return Eeprom_ERROR;
      }

      sp += data_len;
      internal_addr += data_len;
      data_len = 0;
    }
  }

  return Eeprom_OK;
}

/******************************************************************************************
 *******************************************************************************************/
void Eeprom_Write_Fact(unsigned char internal_addr, unsigned char *sp, unsigned char
  data_len)
{
  uchar i = 0;

  Eeprom_IO_MACROS();
  for (;;)
  {
    if (Eeprom_OK == Eeprom_Page_Write(internal_addr, sp, data_len))
      break;
    i++;
    if (i >= 5)
      break;
  }
}

/******************************************************************************************
 *******************************************************************************************/
void Eeprom_Read_Fact(unsigned char internal_addr, unsigned char *dp, unsigned char
  data_len)
{
  uchar i = 0;

  Eeprom_IO_MACROS();
  for (;;)
  {
    if (Eeprom_OK == Eeprom_register_read(internal_addr, dp, data_len))
      break;
    i++;
    if (i >= 5)
      break;
  }
}

/******************************************************************************************
 *******************************************************************************************/
void PowerOffSave(void)
{
  unsigned int i,j=0;;

#if 0
  write_data[0] = (unsigned char)(VehicleSmallDistanceLow);
  write_data[1] = (unsigned char)(VehicleSmallDistanceLow>>8);
  write_data[2] = (unsigned char)(VehicleSmallDistanceHigh );
  write_data[3] = (unsigned char)(VehicleSmallDistanceHigh>> 8);

  write_data[4] = (unsigned char)(VehicleOverallDistanceLow );
  write_data[5] = (unsigned char)(VehicleOverallDistanceLow>> 8);
  write_data[6] = (unsigned char)(VehicleOverallDistanceHigh );
  write_data[7] = (unsigned char)(VehicleOverallDistanceHigh>> 8);
  write_data[8] = 0x00;

  for (i = 0; i < 8; i++)
    write_data[8] += write_data[i];
	
  _Eeprom_write: 
		
	Eeprom_Write_Fact(SmallDistance_Addr, write_data, 9);

  DelayMs(1);///when write data just,it must wait for a moment,then read data
  
  Eeprom_Read_Fact(SmallDistance_Addr, read_data, 9);

  for (i = 0; i < 9; i++)
  {
    if (read_data[i] != write_data[i]) 
    {
      j++;
      if(j<5)								///11.08.12更改，避免因EEPROM损坏而引起的死循环。
        goto _Eeprom_write;
    }
  }
  asm nop;
 #endif
}

/******************************************************************************************
 *******************************************************************************************/
void PowerOffSaveRead(void)
{
  uchar i, j = 0,z=0;
  uchar read_data_check;

  _re_read: 
	Eeprom_Read_Fact(SmallDistance_Addr, read_data, 9);
  read_data_check = 0x00;
  for (i = 0; i < 8; i++)
    read_data_check += read_data[i];
  if (read_data_check != read_data[8])
  {
    j++;
    if (j < 5)
      goto _re_read;
  }
	///Eeprom_Read_Fact(SmallDistance_Addr, read_data, 9);
}
#pragma CODE_SEG DEFAULT
