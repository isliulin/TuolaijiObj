#define	  _NO_MX25L12845D_EXTERN
#include "MX25L12845D.h"
#include "can0.h"
/********************************************************************************* 
*函数原型：SPI_WriteByte(U8 byte) 
*函数功能： SCK上升沿向芯片输入数据有效  
*函数参数： 
*返回值： 
*说明：入口是，sck为高，出口是sck仍为高 
*********************************************************************************/ 

void SPI_WriteByte(uchar byte) 
{ 
  uchar count; 
  for(count=0;count<8;count++) 
  {  
	  SCK_L(); 
	  if(byte&0x80) 
		SI_H(); 
	  else 		
		SI_L(); 
	  byte=byte<<1; 	
	  SCK_H(); 
  } 
} 
/********************************************************************************* 
*函数原型：U8 SPI_ReadByte() 
*函数功能：SCK下降沿从芯片读出数据有效 
*函数参数： 
*返回值： 
*说明：入口是，sck为高，出口是sck仍为高 
*********************************************************************************/ 
uchar SPI_ReadByte() 
{ 
  uchar count,byte; 
	
  byte=0; 
  for(count=0;count<8;count++) 
  { 	 
	  SCK_L(); 
	  byte<<=1; 
	  if(ISD_MISO)	
		byte++; 
	  SCK_H();    
  } 
  return(byte); 
} 
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static uchar Flash_WaitBusy(void) 
{ 
	uchar Error_Data=flash_OK;
	uchar state_reg = 0x00;			 
	uint WaitDelay=0;
	  
	CS_L(); 
	SPI_WriteByte(FLASH_READ_STATAS);	///read status register
	do 
	{  
		state_reg = SPI_ReadByte();  
		WaitDelay++;
		if(WaitDelay%1000==0)///当等待的时间过长为1000的倍数时需清外置看门狗
		{
			out_wdt_reset();
		 	if(WaitDelay>60000)
		 	{
		 		Error_Data=err_flash_unkown;
		 		break;
		 	}
			//CAN0_ReadFrame();
		}
	} 
	while(state_reg&0x01); ///是否在空闲状态
	CS_H(); 
	return	Error_Data;
} 
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/ 
uchar Flash_Read_ID(void)   //ID读取 
{   
	ISD_sendbuf[0] = 0x00;
	ISD_sendbuf[1] = 0x00;
	ISD_sendbuf[2] = 0x00;
	(void)Flash_WaitBusy(); 
	  
	CS_L(); 
	SPI_WriteByte(FLASH_ID_READ); //ID = C2	//read identification
	ISD_sendbuf[0] = SPI_ReadByte(); 
	ISD_sendbuf[1] = SPI_ReadByte();
	ISD_sendbuf[2] = SPI_ReadByte();
	CS_H(); 
	(void)Flash_WaitBusy(); 
	return ISD_sendbuf[0]; 
} 
/********************************************************************************* 
*函数原型：void Erase_Flash(void) 
*函数功能：整片擦除 
*函数参数： 
*返回值： 
*说明：整片擦除时间为10秒左右 
*********************************************************************************/ 
uchar Erase_Flash(void) 
{ 
	uchar Error_Data=flash_OK;
	
  CS_L(); 
  SPI_WriteByte(FLASH_WRITE_ENABLE); 
  CS_H();  
  Error_Data=Flash_WaitBusy();
  if(Error_Data!=flash_OK)
		return	Error_Data;
  CS_L(); 
  SPI_WriteByte(FLASH_WHOLE_ERASE);  
  CS_H();   
  ///Flash_WaitBusy();
  return	Error_Data;
}    
/********************************************************************************* 
//函数原型：void Erase_Sector(U16 i) 
//函数功能：扇区擦除，所需时间90~300ms
//函数参数：块地址 
//返回值： 
//说明：  4K-byte	  4096sector  addr:0~4095
*********************************************************************************/ 
uchar Erase_Sector(uint Sector_Number) 
{ 
 	uchar Error_Data;

	out_wdt_reset();
	CS_L(); 
	SPI_WriteByte(FLASH_WRITE_ENABLE); 
	CS_H(); 
	Error_Data=Flash_WaitBusy();
	if(Error_Data!=flash_OK)
		return	Error_Data;
  
  CS_L(); 
  SPI_WriteByte(FLASH_SECTOR_ERASE);   
  SPI_WriteByte( (uchar)(Sector_Number>>4) ); 
  SPI_WriteByte( (uchar)(Sector_Number<<4) );  
  SPI_WriteByte(0x00); ///清FLASH
  CS_H(); 
  Error_Data=Flash_WaitBusy();
	return	Error_Data;
} 
/********************************************************************************* 
//函数原型：void Erase_Block(U16 i) 
//函数功能：块擦除 ，所需时间：0.7S~2S
//函数参数：块地址 
//返回值： 
//说明：  64K-byte	   256 block
*********************************************************************************/ 
uchar Erase_Block(uchar Block_Number) 
{ 
  uchar Error_Data;

	out_wdt_reset();
  CS_L(); 
  SPI_WriteByte(FLASH_WRITE_ENABLE); 
  CS_H(); 
  Error_Data=Flash_WaitBusy();
	if(Error_Data!=flash_OK)
		return	Error_Data;

  CS_L(); 
  SPI_WriteByte(FLASH_BLOCK_ERASE); 
  SPI_WriteByte( Block_Number ); 
  SPI_WriteByte(0x00); ///清FLASH					   
  CS_H(); 
  Error_Data=Flash_WaitBusy();
  return	Error_Data;
} 
/********************************************************************************* 
*函数原型：Flash_PageWrite(u16 page,u8 *Data_Buf) 
*函数功能： 
*函数参数： 16MB = 256B*65536    128Mb/8=16MB=16777216字节
*返回值： 
*说明：	 读取一整页256B，页范围0-65535 Page，
						134217728bit/8  = 16777216字节，字节地址0~ 16777215
*********************************************************************************/ 
uchar Flash_PageWrite(uint page,uint addr,uint len,uchar *pdata) 
{ 
	uchar Error_Data;
  	ulong Address_Temp; 
  	uint i;

	out_wdt_reset();
	Address_Temp=( ((ulong)page)<<8 )+addr;     
	CS_L();
	SPI_WriteByte(FLASH_WRITE_ENABLE);	
	CS_H(); 

	Error_Data=Flash_WaitBusy();
	if(Error_Data!=flash_OK)
		return	Error_Data;

	CS_L();
	SPI_WriteByte(FLASH_PAGE_WRITE);   
	SPI_WriteByte((uchar)( (Address_Temp&0x00FF0000) >>16 )); 
	SPI_WriteByte((uchar)( (Address_Temp& 0x0000FF00) >>8 )); 
	SPI_WriteByte((uchar)( (Address_Temp& 0x000000FF) )); 
	for(i = 0; i< len; i++)   
	{ 
		SPI_WriteByte(*(pdata+i));
		if(i%1000==0)///当所读的数据过大时，清一次外置看门狗
	  	out_wdt_reset();	
	} 
  	CS_H(); 
  
  	Error_Data=Flash_WaitBusy(); 
	return	Error_Data;
} 
/********************************************************************************* 
*函数原型：Flash_PageWrite(u16 page,u8 *Data_Buf) 
*函数功能： 
*函数参数： 16MB = 256B*65536    128Mb/8=16MB=16777216字节
*返回值： 
*说明：	 读取一整页256B，页范围0-65535 Page，134217728bit/8  = 16777216字节，字节地址0~ 16777215
*********************************************************************************/ 
uchar Flash_Page_Write(ulong addr,uint Data_Len,uchar *pdata) 
{ 
	uchar	Error_Data;
	uint	star_page,star_addr;
	uint	empty_len,nextpage_use_len;
	uint i,Page_Number;

	///页码及其首地址
	star_page=(uint)(addr/FLASH_PAGE_SIZE);
	star_addr=(uint)(addr%FLASH_PAGE_SIZE);

	empty_len=FLASH_PAGE_SIZE-star_addr;///是否剩下足够的空间来写当前的数据
	if(empty_len>=Data_Len)///如果是在一页的范围内则直接写
	{   
    	Error_Data=Flash_PageWrite(star_page,star_addr,Data_Len,pdata);
		if(Error_Data!=flash_OK)
			return	Error_Data;
	}
	else 
	{
		Error_Data=Flash_PageWrite(star_page,star_addr,empty_len,pdata);///先把剩余页中能用的空间填满
		if(Error_Data!=flash_OK)
			return	Error_Data;
		star_page=star_page+1;
	  	star_addr=0;
		Page_Number=Data_Len/FLASH_PAGE_SIZE;
		if(Page_Number==0)///不足一页
		{
		  nextpage_use_len=Data_Len-empty_len;
		  Error_Data=Flash_PageWrite(star_page,star_addr,nextpage_use_len,pdata+empty_len);
			if(Error_Data!=flash_OK)
				return	Error_Data;
		}
		else	///超过一页
		{
			nextpage_use_len=Data_Len%FLASH_PAGE_SIZE;
			for(i=0;i<Page_Number;i++)
			{
				Error_Data=Flash_PageWrite(star_page,star_addr,FLASH_PAGE_SIZE,pdata+empty_len+FLASH_PAGE_SIZE*i);///先写多页后写不足一页
				if(Error_Data!=flash_OK)
					return	Error_Data;
			}
			Error_Data=Flash_PageWrite(star_page,star_addr,nextpage_use_len,pdata+empty_len+FLASH_PAGE_SIZE*Page_Number);
		}
	}
	return	Error_Data;
} 
/********************************************************************************* 
*函数原型：void Flash_PageRead(U16 page,U8 *Data_Buf) 
*函数功能： 
*函数参数： 16MB = 256B*65536
*返回值： 
*说明：	 读取一整页256B，页范围0-65536 Page 
*********************************************************************************/ 
uchar Flash_PageRead(ulong addr,uint len,uchar *pdata) 
{ 
	uchar	Error_Data;
  uint	i;   

	out_wdt_reset();
  CS_L(); 
  SPI_WriteByte(FLASH_WRITE_ENABLE);	
  CS_H(); 	  
  Error_Data=Flash_WaitBusy();
	if(Error_Data!=flash_OK)
		return	Error_Data;
  
  CS_L(); 
  SPI_WriteByte(FLASH_FAST_READ);	
  SPI_WriteByte((uchar)( (addr&0x00FF0000) >>16)); 
  SPI_WriteByte((uchar)( (addr&0x0000FF00) >>8)); 
  SPI_WriteByte((uchar)(addr&0x000000FF)); 
  SPI_WriteByte((uchar)0x00); 
  for(i = 0 ; i< len; i++) 
  { 
		*(pdata+i) = SPI_ReadByte(); 
		if(i%1000==0)///当所读的数据过大时，清一次外置看门狗
    	out_wdt_reset();					
  } 
  CS_H();	   
  
  Error_Data=Flash_WaitBusy(); 
	return	Error_Data;
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void	Bootloader_Mode_Change(void)
{
	///给视频板供电
	V_5V_CTRL_IO=1;
	V_5V_CTRL=0;       
	///下载外挂FLASH使能脚
	DOWNLOAD_CTRL_IO=1; 
	DOWNLOAD_CTRL=1; 
	///电平转换芯片的S脚，即模块切换
	MODE_CTRL_IO=1;
	MODE_CTRL=1;///为高电平时可烧写外挂FLASH
	///信号输入
	ISD_MISO_IO=0;
	///信号输出脚
	ISD_MOSI_IO=1;
	ISD_MOSI=1;
	///时钟脚
	ISD_SCLK_IO=1;
	ISD_SCLK=1;
	///使能脚
	ISD_CS_IO=1; 
	ISD_CS=1;
}

