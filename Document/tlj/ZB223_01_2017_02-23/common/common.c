#include "common.h"

UINT8 write_data[21],read_data[21];

/*********************************************
funtion operation:delay time 1ms
funtion call:void
input parameter:delay,the number of delay time
return:void
 ********************************************/
void DelayMs(unsigned int Delay)
{
  unsigned int i, j;

  for (i = 0; i < Delay; i++)
  {
    for (j = 0; j < 1000; j++)
    {
      __asm("nop");//__asm("nop");
      __asm("nop");
    }
  }
}

/*********************************************
funtion operation:delay time 1us
funtion call:void
input parameter:delay,the number of delay time
return:void
 ********************************************/
void delay_us(word delay)
{
  while (--delay)
  {
    __asm("nop");
    __asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
  }
}


void PowerOffSave(void)
{                                          
	unsigned int i,j=0;

	write_data[0] = (unsigned char)(USR_s_DeviceConfigData.RelativeDistance);
	write_data[1] = (unsigned char)(USR_s_DeviceConfigData.RelativeDistance>>8);
	write_data[2] = (unsigned char)(USR_s_DeviceConfigData.RelativeDistance>>16 );
	write_data[3] = (unsigned char)(USR_s_DeviceConfigData.RelativeDistance>>24);	
	write_data[4] = (unsigned char)(USR_s_DeviceConfigData.TotalDistance);//nbapo:里程32b
	write_data[5] = (unsigned char)(USR_s_DeviceConfigData.TotalDistance>>8);
	write_data[6] = (unsigned char)(USR_s_DeviceConfigData.TotalDistance>>16 );
	write_data[7] = (unsigned char)(USR_s_DeviceConfigData.TotalDistance>>24);	
	write_data[8] = USR_s_DeviceConfigData.RShieldVideoChannel;
	write_data[9] = USR_s_DeviceConfigData.MiddleDoorVideoChannel;
	write_data[10] = USR_s_DeviceConfigData.FrontDoorVideoChannel;
	write_data[11] = USR_s_DeviceConfigData.TopVideoChannel;
	write_data[12] = USR_s_DeviceConfigData.Wippers;
	write_data[13] = USR_s_DeviceConfigData.LightLevel; 
	write_data[14] = USR_s_DeviceConfigData.BuzzerLevel;
	write_data[15] = USR_s_DeviceConfigData.PictureCfg[0]; 
	write_data[16] = USR_s_DeviceConfigData.PictureCfg[1];	
	write_data[17] = USR_s_DeviceConfigData.PictureCfg[2];
	write_data[18] = USR_s_DeviceConfigData.PictureCfg[3];  

	write_data[19] = USR_s_DeviceConfigData.maintain;  

	///for (i = 0; i < 20; i++)
	//	write_data[i] = 0xff;


	for (i = 0; i < 20; i++)
		write_data[20] += write_data[i];

_Eeprom_write: 

	Eeprom_Write_Fact_Ex(Parameters_EEp_Addr, write_data, 21);

	DelayMs(2);///when write data just,it must wait for a moment,then read data

	Eeprom_Read_Fact_Ex(Parameters_EEp_Addr, read_data, 21);

	for (i = 0; i < 20; i++)
	{
		if (read_data[i] != write_data[i]) 
		{
			j++;
			if(j<5)		///11.08.12更改，避免因EEPROM损坏而引起的死循环。
				goto _Eeprom_write;
		}
	}
	__asm("nop");
}

void PowerOffSaveRead(void)
{
  uchar i, j = 0;
  uchar read_data_check;

  _re_read: 
	Eeprom_Read_Fact_Ex(Parameters_EEp_Addr, read_data, 21);
  read_data_check = 0x00;
  for (i = 0; i < 20; i++)
    read_data_check += read_data[i];
  if (read_data_check != read_data[20])
  {
    j++;
    if (j < 5)
      goto _re_read;
  }
}


const UINT8 HexToAscii[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
void DispHexToAscii(UINT8 *pBuf,UINT32 data) 
{
  UINT8 i, dat;
  for(i=0;i<4;i++) 
  {
    dat =  (UINT8)(data>>((3-i)*8));
    pBuf[i*3] =   HexToAscii[dat>>4];
    pBuf[i*3+1] = HexToAscii[dat&0x0f];
    pBuf[i*3+2] = ' '; 
  }
}
void Disp2ToAscii(UINT8 *pBuf,UINT8 data) 
{
  signed char i;
  for(i=7;i>=0;i--) 
  {
    if(data&(1<<i)) pBuf[7-i] = '1';
    else pBuf[7-i] = '0';
  }
}
/*************************************************************************
函数名：void bit16hexToAscii(UINT8 *pBuf,UINT16 data)
功能：把16进制数转换为Ascii码，存放在*pBuf指向的内存单元
参数说明:UINT8 *pBuf -> 存放转换完毕的非压缩十进制数据。存放方式：小端存放。
         UINT16 data -> 转换的原始16进制数据
返回值：*pBuf
*************************************************************************/
void bit16HexToAscii(UINT8 *pBuf,UINT16 data)
{
    UINT8 i;
    for(i=0;i<5;i++)
    {
        pBuf[4-i] = data % 10 + 0x30;
        data /= 10;
    }
}
/*************************************************************************
函数名：void bit8hexToAscii(UINT8 *pBuf,UINT8 data)
功能：把16进制数转换为Ascii码，存放在*pBuf指向的内存单元
参数说明:UINT8 *pBuf -> 存放转换完毕的非压缩十进制数据。存放方式：小端存放。
         UINT8 data -> 转换的原始16进制数据
返回值：*pBuf
*************************************************************************/
void bit8HexToAscii(UINT8 *pBuf,UINT8 data)
{
    UINT8 i;
    for(i=0;i<3;i++)
    {
        pBuf[2-i] = data % 10 + 0x30;
        data /= 10;
    }
}
/*************************************************************************
函数名：void bit16hexToAscii(UINT8 *pBuf,UINT16 data)
功能：把16进制数转换为Ascii码，存放在*pBuf指向的内存单元
参数说明:UINT8 *pBuf -> 存放转换完毕的非压缩十进制数据。存放方式：小端存放。
         UINT16 data -> 转换的原始16进制数据
返回值：*pBuf
*************************************************************************/
void bit32HexToAscii(UINT8 *pBuf,UINT32 data)
{
    UINT8 i;
    for(i=0;i<10;i++)
    {
        pBuf[9-i] = data % 10 + 0x30;
        data /= 10;
    }
}
/************************************************************************
函数名：void bit32hexToAscii(UINT8 *pBuf,UINT32 data)
功能：把16进制数转换为Ascii码，存放在*pBuf指向的内存单元
参数说明:UINT8 *pBuf -> 存放转换完毕的非压缩十进制数据。存放方式：小端存放。
         UINT8 data -> 转换的原始16进制数据
返回值：*pBuf
*************************************************************************/
/*void bit32HexToAscii(UINT8 *pBuf,UINT32 data)
{
    UINT8 i;
    UINT8 residue;
    for(i=0;i<6;i++)
    {
        pBuf[5-i] = data % 10 + 0x30;
        data /= 10;
    }
}*/
/*************************************************************************
函数名:void insertDot(UINT8 *pData,UINT8 dot,UINT8 cnt)
功能：插入小数点。
参数说明：UINT8 *pData -> 在*pData指向的数组中插入小数点的原始数据
          UINT8 dot -> 小数点位置，从右边数起，
          比如dot == 0 则为：XXXX
              dot == 1 则为：XXX.X       01234 4-1  = 2      
              dot == 2 则为：XX.XX       1234    12.34
          UINT8 cnt -> 指示*pData指向的原始数据的长度
返回值：*pData
*************************************************************************/
void insertDot(UINT8 *pData,UINT8 dot,UINT8 cnt)
{
    UINT8 i;
    if(!dot || dot>=cnt)
    {
        return;
    }
    else
    {
        /*cnt = cnt - dot - 1; 
        for(i=0;i<cnt;i++)
        {
            pData[i] = pData[i+1];
        }
        pData[cnt] = '.';  
        */
       
        for(i=cnt;i>cnt-dot;i--)      //右移
        {
         pData[i] = pData[i-1];
        }
        pData[i] = '.';  
        
      //  pData[cnt-dot] = '.';
        /*cnt = cnt - dot - 1; 
        for(i=cnt;i>cnt-dot;i--)
        {
            pData[i] = pData[i-1];                     //10-1-1 =8
        }                                           //0123456789    .9
        pData[i] = '.';                                 1234567
        */
    }
}
/*************************************************************************
函数名:void insertDot(UINT8 *pData,UINT8 dot,UINT8 cnt)
功能：插入小数点。
参数说明：UINT8 *pData -> 在*pData指向的数组中插入小数点的原始数据
          UINT8 dot -> 小数点位置，从右边数起，
          比如dot == 0 则为：XXXX
              dot == 1 则为：XXX.X       01234 4-1  = 2      
              dot == 2 则为：XX.XX       1234    12.34
          UINT8 cnt -> 指示*pData指向的原始数据的长度
返回值：*pData
*************************************************************************/
UINT8 arrangeDispBuffer(UINT8 *pData,UINT8 *pDispData,UINT8 lenght, UINT8 disDotLenght,UINT8 disIntegerLenght) 
{
  UINT8 i;
  UINT8 sign;     //0 整数
  UINT8 integerLenght;
  UINT8 DotLenght;
  UINT8 relenght;
  if(pData[0]=='-'||pData[0]==' ')sign = 1;//有符号位
  else sign = 0;
  integerLenght = 0;
  DotLenght = 0;
  for(i=0;i<lenght;i++) 
  {
   if(pData[i]!='.') integerLenght++;         //符号+整数长度
   else 
   {
     DotLenght = lenght - integerLenght-1;  //小数后长度
     break;
    }
  }
 // if(sign) 
 // {
//  DelAsciiInvalidZero(&pData[1],lenght-1);
  //} 
 //else DelAsciiInvalidZero(pData,lenght);
  
  if(disIntegerLenght>=integerLenght) 
    {
       for(i=0;i<integerLenght;i++)
       pDispData[i] = pData[i];
       relenght = integerLenght;
    } 
  else 
    { 
     if(sign) 
       {
            pDispData[0] = pData[0];                        
            for(i=0;i<disIntegerLenght-1;i++)   //01234.902;
            {
             pDispData[1+i] =  pData[integerLenght-disIntegerLenght+i+1];
            }
       } 
      else 
       {
          for(i=0;i<disIntegerLenght;i++) 
          {
           pDispData[i] =  pData[integerLenght-disIntegerLenght+i];
          }
       }
      relenght = disIntegerLenght;  
    }
  if(DotLenght>0&&disDotLenght>0) 
  { 
    pDispData[relenght] = '.';
    relenght++;
    if(disDotLenght>DotLenght)  //大于 
    {
       for(i=0;i<DotLenght;i++) 
       {
         pDispData[relenght] =  pData[integerLenght+1+i];
         relenght++;  
       }  
    } 
    else 
    {
       for(i=0;i<disDotLenght-1;i++) 
       {
         pDispData[relenght] =  pData[integerLenght+1+i];
         relenght++;  
       }   
    }
  }
  return relenght;
  
}
/***************************************************************************
函数名：void DelAsciiInvalidZero(UINT8 *pData,UINT8 cnt)
功能：删除数据中的无效0
参数说明：UINT8 *pData -> 数据源
          UINT8 cnt -> 指定数据源中的数据长度
返回值：*pData
***************************************************************************/
void DelAsciiInvalidZero(UINT8 *pData,UINT8 cnt)
{
    UINT8 i;
    for(i=0;i<cnt;i++)
    {
        if(pData[i] == '0')
        {
            if(i == cnt-1)
            {
                return;
            }
            pData[i] = ' ';
        }
        else
        {
            if(pData[i] == '.')
            {
                pData[i-1] = '0';
            }
            return;
        }
    }
}
/***************************************************************************
函数名：void DelAsciiInvalidZero(UINT8 *pData,UINT8 cnt)
功能：删除数据中的无效0
参数说明：UINT8 *pData -> 数据源
          UINT8 cnt -> 指定数据源中的数据长度
返回值：*pData
***************************************************************************/
void DelAsciiInvalidZeroEx(UINT8 *pData,UINT8 cnt)
{
    UINT8 i;
    if((pData[0]>='0')&&(pData[0]<='9')) 
    {   
      for(i=0;i<cnt;i++)
      {
          if(pData[i] == '0')
          {
              if(i == cnt-1)
              {
                  return;
              }
              pData[i] = ' ';
          }
          else
          {
              if(pData[i] == '.')
              {
                  pData[i-1] = '0';
              }
              return;
          }
      } 
    } 
    else 
    {
      for(i=1;i<cnt;i++)      //符号
      {
          if(pData[i] == '0')
          {
              if(i == cnt-1)
              {
                 pData[i-1]  =   pData[0];
                 pData[0] = ' ';
                  return;
              }
              pData[i] = ' ';
          }
          else
          {
              if(pData[i] == '.')
              {
                  pData[i-2] =  pData[0];
                  pData[i-1] = '0';
              } 
              else 
              {
                pData[i-1] = pData[0];
              }
              if(i!=1) pData[0] = ' ';
              return;
          }
      } 
    
    }
}

/******************************************************************************************
函数名:void binToDec(uchar bin, uchar *pDec) 
功能:二进制转化为十进制
参数说明:uchar bin -> 二进制数
                         uchar *pDec -> 十进制数
******************************************************************************************/
void binToDec(uchar bin, uchar *pDec) 
{
   uchar temp;
	 temp = bin/10;
	 if(temp)
	 {
	    *pDec++ = temp + 0x30;
			*pDec = (bin%10)+0x30;
	 }
	 else
	 {
      //*pDec++ = ' ';
			*pDec++ = (bin%10)+0x30;
			*pDec++ = ' ';
	 }
	 
}
/*****************************************************************************************
函数名：void dataCopy(UINT8 *pDst,UINT8 * pScr,UINT8 cnt)
*****************************************************************************************/
void dataCopy(UINT8 *pDst,UINT8 * pScr,UINT8 cnt)
{
    UINT8 i;
    for(i=0;i<cnt;i++)
    {
        *pDst++ = *pScr++;
    }
}
/****************************************************************************************
函数名：void asciiToGb2312(UINT8 *pGb2312,UINT8 *pAscii,UINT8 cnt)
****************************************************************************************/
void asciiToGb2312(UINT8 *pGb2312,UINT8 *pAscii,UINT8 cnt)
{
    UINT8 i;
    for(i=0;i<cnt;i++)
    {
        pGb2312[2*i] = ' ';
        pGb2312[2*i+1] = pAscii[i];
    }
}
/****************************************************************************************
函数名：UINT8 sign32Bit(UINT32 *pData,UINT32 offset)
功能：判断16位数据的符号
参数说明：UINT16 * pData -> 原始数据，去掉偏移后也存在此
          UINT16 offset -> 数据偏移量
返回值：0x00 -> 为正数
        0x01 -> 为负数
****************************************************************************************/
UINT8 sign16Bit(UINT16 *pData,UINT16 offset)
{
    UINT16 data = *pData;
    if(data >= offset)
    {
        data -= offset; 
        *pData = data;
        return 0x00;
    }
    else
    {
        data = offset - data;
        *pData = data;
        return 0x01;
    }    
}
/****************************************************************************************
函数名：UINT8 sign32Bit(UINT32 *pData,UINT32 offset)
功能：判断16位数据的符号
参数说明：UINT16 * pData -> 原始数据，去掉偏移后也存在此
          UINT16 offset -> 数据偏移量
返回值：0x00 -> 为正数
        0x01 -> 为负数
****************************************************************************************/
UINT8 sign32Bit(UINT32 *pData,UINT32 offset)
{
    UINT32 data = *pData;
    if(data >= offset)
    {
        data -= offset; 
        *pData = data;
        return 0x00;
    }
    else
    {
        data = offset - data;
        *pData = data;
        return 0x01;
    }    
}

void Eeprom_Read_Fact_Ex(unsigned char internal_addr, byte *dp, unsigned char data_len)
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
函数名：void Eeprom_IO_MACROS(void)
功能：EEPROM端口初始化
 *****************************************************************************************/
void Eeprom_IO_MACROS(void)
{
    Eeprom_SCL_MUX();Eeprom_SCL_PDDR_OUT ;	//Eeprom_SCL_IO = 1;
    Eeprom_SCL_L;//Eeprom_SCL_L;
    Eeprom_SDA_MUX();Eeprom_SDA_PDDR_OUT;	//Eeprom_SDA_PDDR_OUT;
    Eeprom_SDA_L;//Eeprom_SDA_L;
}

/***********************************************************************
funtion operation:read a serial data from iic bus
funtion call:	Eeprom_IICsendbyte(void);byte Eeprom_IICwaitack(void);void Eeprom_IICstop(void);
void Eeprom_IICsendack(void);void Eeprom_IICsendnotack(void);
input	parameter:internal_addr;the first addr of the serial data;*dp;the point data will be recieved;data_len;
the length of the data will be recieved
return:void
 ***********************************************************************/
unsigned char Eeprom_register_read(unsigned char internal_addr, byte *dp,unsigned char data_len)
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

/*******************************************************************
funtion operation:write a serial data to iic bus
fution call:void Eeprom_IICsendbyte(void);byte Eeprom_IICwaitack(void);void Eeprom_IICstop(void);
input parameter:internal_addr,the first addr of the serial data;*sp;the point data will be sent;data_len;
the length of the data will be sent
return:void				
 *******************************************************************/
unsigned char Eeprom_register_write(unsigned char internal_addr, byte  *sp, unsigned char data_len)
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
/******************************************************************************************
 *******************************************************************************************/
unsigned char Eeprom_Page_Write(unsigned char internal_addr, byte *sp,unsigned char data_len)
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
  while (data_len > 0x00)
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
void Eeprom_Write_Fact_Ex(unsigned char internal_addr, byte *sp, unsigned char  data_len)
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
/*******************************************************************************************
函数名：void initEepromData(void)
功能：初始化时获取Eeprom数据。
*******************************************************************************************/
void initEepromData(void)
{

  PowerOffSaveRead();
   USR_s_DeviceConfigData.RelativeDistance = read_data[3];
   USR_s_DeviceConfigData.RelativeDistance <<=8;
   USR_s_DeviceConfigData.RelativeDistance |=read_data[2];
   USR_s_DeviceConfigData.RelativeDistance <<=8;
   USR_s_DeviceConfigData.RelativeDistance |=read_data[1];
   USR_s_DeviceConfigData.RelativeDistance <<=8;
   USR_s_DeviceConfigData.RelativeDistance |=read_data[0];
   
   USR_s_DeviceConfigData.TotalDistance    = read_data[7];
   USR_s_DeviceConfigData.TotalDistance  <<= 8; 
   USR_s_DeviceConfigData.TotalDistance   |= read_data[6];    
   USR_s_DeviceConfigData.TotalDistance  <<= 8; 
   USR_s_DeviceConfigData.TotalDistance   |= read_data[5];  
   USR_s_DeviceConfigData.TotalDistance  <<= 8; 
   USR_s_DeviceConfigData.TotalDistance   |= read_data[4];  
 
   	USR_s_DeviceConfigData.RShieldVideoChannel = read_data[8];
  	USR_s_DeviceConfigData.MiddleDoorVideoChannel = read_data[9];
    USR_s_DeviceConfigData.FrontDoorVideoChannel  = read_data[10];
    USR_s_DeviceConfigData.TopVideoChannel        = read_data[11];
    USR_s_DeviceConfigData.Wippers                = read_data[12];
    USR_s_DeviceConfigData.LightLevel             = read_data[13];
    USR_s_DeviceConfigData.BuzzerLevel            = read_data[14];
    USR_s_DeviceConfigData.PictureCfg[0]          = read_data[15];
    USR_s_DeviceConfigData.PictureCfg[1]	        = read_data[16];
    USR_s_DeviceConfigData.PictureCfg[2]          = read_data[17];
    USR_s_DeviceConfigData.PictureCfg[3]          = read_data[18];
    USR_s_DeviceConfigData.maintain               = read_data[19]; 


   if(USR_s_DeviceConfigData.RelativeDistance==0xffffffff)   USR_s_DeviceConfigData.RelativeDistance = 0;
   if(USR_s_DeviceConfigData.TotalDistance==0xffffffff)      USR_s_DeviceConfigData.TotalDistance = 0;
   if(USR_s_DeviceConfigData.RShieldVideoChannel==0xff)      USR_s_DeviceConfigData.RShieldVideoChannel=0;
   if(USR_s_DeviceConfigData.MiddleDoorVideoChannel==0xff)   USR_s_DeviceConfigData.MiddleDoorVideoChannel = 2;
   if(USR_s_DeviceConfigData.FrontDoorVideoChannel==0xff)    USR_s_DeviceConfigData.FrontDoorVideoChannel=1; 
   if(USR_s_DeviceConfigData.TopVideoChannel==0xff||USR_s_DeviceConfigData.TopVideoChannel==0)          USR_s_DeviceConfigData.TopVideoChannel = 3;
   if(USR_s_DeviceConfigData.Wippers==0xff||USR_s_DeviceConfigData.Wippers==0)       USR_s_DeviceConfigData.Wippers = 5;
   if(USR_s_DeviceConfigData.LightLevel==0xff||USR_s_DeviceConfigData.LightLevel==0)    USR_s_DeviceConfigData.LightLevel=1;
   if(USR_s_DeviceConfigData.BuzzerLevel==0xff||USR_s_DeviceConfigData.BuzzerLevel==0)   USR_s_DeviceConfigData.BuzzerLevel=2;
   if(USR_s_DeviceConfigData.PictureCfg[0]==0xff) USR_s_DeviceConfigData.PictureCfg[0]=5;
   if(USR_s_DeviceConfigData.PictureCfg[1]==0xff) USR_s_DeviceConfigData.PictureCfg[1]=5; 
   if(USR_s_DeviceConfigData.PictureCfg[2]==0xff) USR_s_DeviceConfigData.PictureCfg[2]=5;
   if(USR_s_DeviceConfigData.PictureCfg[3]==0xff) USR_s_DeviceConfigData.PictureCfg[3]=5;  
   
   USR_s_OV.rain_scr = USR_s_DeviceConfigData.Wippers; 
  
   CountDistanceSave = USR_s_DeviceConfigData.TotalDistance; 
   
   if(USR_s_DeviceConfigData.maintain>1)   USR_s_DeviceConfigData.maintain = 0;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////		I2C读写部分  		////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*********************************************************************
function operation : IIC start condition
function called : Eeprom_SCL_delay(void)
return: void
 *********************************************************************/
void Eeprom_IICstart(void)
{
  Eeprom_SDA_PDDR_OUT;
  __asm("nop");
  Eeprom_SDA_H;
  Eeprom_SCL_delay();
  Eeprom_SCL_H;
  Eeprom_SCL_delay();
  Eeprom_SDA_L;
  Eeprom_SCL_delay();
  Eeprom_SCL_L;
  Eeprom_SCL_delay();
}
/*********************************************************************
function operation : send a byte over IIC bus
function called : void Eeprom_SCL_delay(void)
function input parameters : csendbyte,the data needed to send on IIC
return : void
 *********************************************************************/
void Eeprom_IICsendbyte(unsigned char csendbyte)
{
  unsigned char i = 8;
  while (i--)
  {
    Eeprom_SCL_L;
    if (csendbyte &0x80)
      Eeprom_SDA_H;
    else
      Eeprom_SDA_L;
    Eeprom_SCL_delay();
    Eeprom_SCL_H;
    Eeprom_SCL_delay();
    csendbyte <<= 1;
  }
  Eeprom_SCL_L;
  Eeprom_SCL_delay();
}
/*********************************************************************
function operation : IIC wait ACK
function called : void Eeprom_SCL_delay(void),void Eeprom_IICstop(void)
return : werther receive ack signal,Eeprom_ERROR or Eeprom_OK
 *********************************************************************/
byte Eeprom_IICwaitack(void)
{
  unsigned char cErrTime = 255;

  Eeprom_SDA_PDDR_IN; //define as input
  __asm("nop");
  Eeprom_SCL_H;
  Eeprom_SCL_delay();
  while (Eeprom_SDA_READ)
  {
    cErrTime--;
    if (cErrTime == 0)
    {
      Eeprom_IICstop();
      Eeprom_SDA_PDDR_OUT;
      return Eeprom_ERROR;
    }
  }
  Eeprom_SCL_L;
  Eeprom_SCL_delay();
  Eeprom_SDA_PDDR_OUT;
  return Eeprom_OK;
}

/*********************************************************************
function operation : IIC stop condition
function called :  Eeprom_SCL_delay(void)
return : void
 *********************************************************************/
void Eeprom_IICstop(void)
{
  Eeprom_SCL_L;
  Eeprom_SCL_delay();
  Eeprom_SDA_L;
  Eeprom_SCL_delay();
  Eeprom_SCL_H;
  Eeprom_SCL_delay();
  Eeprom_SDA_H;
  Eeprom_SCL_delay();
}
/*********************************************************************
function operation: receive a byte over IIC bus
function called : void Eeprom_SCL_delay(void)
input parameter:	void
return : cr_byte,a byte data receive from iic bus
 *********************************************************************/
unsigned char Eeprom_IICreceivebyte(void)
{
  unsigned char i = 8;
  unsigned char cr_byte = 0;

  Eeprom_SDA_PDDR_IN;
  __asm("nop");
  while (i--)
  {
    cr_byte <<= 1;
    Eeprom_SCL_delay();
    Eeprom_SCL_L;
    Eeprom_SCL_delay();
    Eeprom_SCL_H;
    if (Eeprom_SDA_READ)
    {
      cr_byte++;
    }
  }
  Eeprom_SCL_delay();
  Eeprom_SCL_L;
  Eeprom_SCL_delay();
  Eeprom_SDA_PDDR_OUT;
  return cr_byte;
}
/*********************************************************************
function operation : IIC send ACK
function called : void Eeprom_SCL_delay(void)
return : void
 *********************************************************************/
void Eeprom_IICsendack(void)
{
  Eeprom_SDA_L;
  Eeprom_SCL_delay();
  Eeprom_SCL_H;
  Eeprom_SCL_delay();
  Eeprom_SCL_L;
  Eeprom_SCL_delay();
}
/*********************************************************************
function operation : IIC send Not ACK
function called : void Eeprom_SCL_delay(void)
return : void
 *********************************************************************/
void Eeprom_IICsendnotack(void)
{
  Eeprom_SDA_H;
  Eeprom_SCL_delay();
  Eeprom_SCL_H;
  Eeprom_SCL_delay();
  Eeprom_SCL_L;
  Eeprom_SCL_delay();
}
/*******************************************
function operation : IIC step control,to produce software SCL
 *******************************************/
void Eeprom_SCL_delay(void)
{
  UINT16  i;


  	for (i = 0; i < 1800; i++)
    __asm("nop");
}
/******************************************************************************************
 *******************************************************************************************/
void ReWrite_Delay(void)
{
  unsigned int i;

  #if	OSC_Number==16				///11.08.12更改，延长页写后写第二页的时间，避免第二页无ACK的发生。
  	for (i = 0; i < 40000; i++)
  #endif
    __asm("nop");
}
void CarMaintain(void) 
{
  UINT8 MaintainVal;
  dword TotalDistance,temp;
  
  TotalDistance = USR_s_DeviceConfigData.TotalDistance/10000;  //Km
  if(TotalDistance<8000) 
  {
    if((TotalDistance>3000)&&(TotalDistance<=3300)) 
    {
       MaintainVal = 1;
       MaintainClear_1F = 1;
    } 
    else 
    {
       MaintainVal = 0; 
       MaintainClear_1F = 0;
      
    }
  } 
  else 
  {
     TotalDistance = TotalDistance - 3000;
     temp =  TotalDistance%5000;
     if((temp>0)&&(temp<=300)) 
     {
      MaintainVal = 1;
      MaintainClear_1F = 1;
     } 
     else
     {
       MaintainVal = 0;
       MaintainClear_1F = 0;
        
     }
  }
  if((MaintainVal==0)&&(USR_s_DeviceConfigData.maintain!=0)) 
  {
    USR_s_DeviceConfigData.maintain = 0;   //不需维保
    PowerOffSave();          
  }
  if((MaintainVal==1)&&(USR_s_DeviceConfigData.maintain==0)) 
  {
   MaintainClear_F = 1;// return 1; ////需维保
  } 
  else
  {
    MaintainClear_F = 0;  //无需维保
  }
  

}

void doHighResponse(void){
	//提高仪表- 仪外负载开关响应
	AdcValueExtract();
	AdcValueChangeToSwitchState();	
	//GPIOD_PTOR = GPIO3 ;
}