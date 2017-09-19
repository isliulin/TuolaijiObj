#include "Isr_vectors.h"
#include "flash.h"
#include "can0.h"
#include "type.h"
#include "MX25L12845D.h"
  
//////////////////////////////////////////////////////////////////////
void BootloadDeal(void);
uchar S19Program(void);
uchar PictureS19Program(void);
void	StateDetectWait(void);
/************************************************************************

************************************************************************/
void main(void)
{
  
  asm sei;///关闭中断
	
	out_wdt_reset();
	CAN0Init();
	FlashInit();
	count_delay=0x00;
	s19RxCount =0;
	for(;;)
	{
	   	out_wdt_reset();
		CAN0_ReadFrame();
		BootloadDeal();
		StateDetectWait();
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void	StateDetectWait(void)
{
	uchar gotdata1;

	count_delay++;		
	if(count_delay>0x03200*2)  
	{
		gotdata1=0X00;
		(void)IFsh1_GetByteFlash(0xF08000,&gotdata1);
		if(gotdata1!=0xFF)
		{
			count_delay=0x00;					
			__asm("jmp 0XC000");       //goto user code
			PPAGE = 0X00;	
		}
  }		
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void BootloadDeal(void)
{	
	uchar ErrorData;
	uint	delay_time;
	
	///擦除主芯片FLASH首地址
	if(ErasureStartFlag==1)
	{		
		ErrorData=chip_erase();
		if(ErrorData==ERR_OK)///擦除正确
		{						 
			err_send(ResponseData);
		}
		else	///擦除错误  
		{						 
			err_send(err_flash_clear);
		}
		CAN0_SendFrame();
		ErasureStartFlag=0;
	}
	///擦除图片FLASH首地址
	if(ErasurePicStartFlag==1)
	{		
		Sector_Start_Adr = (uint)(FirstS19Address/FLASH_SECTOR_SIZE);
		ErrorData=Erase_Sector(Sector_Start_Adr);///每次只擦除一个扇区
		if(ErrorData==flash_OK)///擦除正确
		{						 
			err_send(ResponseData);
			Sector_Start_Adr += 1;///为下一个擦除扇区准备
		}
		else	///擦除错误  
		{						 
			err_send(err_flash_clear);
		}
		CAN0_SendFrame();
		ErasurePicStartFlag=0;
	}
	///终止命令
	if(BootStopFlag==1)
	{
		BootStopFlag=0;
		err_send(ResponseData);
		CAN0_SendFrame();
		_EntryPoint();
	}
	///准备烧录
	if(BootReadyFlag==1)
	{
		BootReadyFlag=0;
		Bootloader_Mode_ChangeFlag=1;
		Bootloader_Mode_Change();///为图片FLASH准备
		err_send(ResponseData);
		CAN0_SendFrame();
		for(delay_time=0;delay_time<1000;delay_time++)
		{
			if(delay_time%100==0)
				out_wdt_reset();
		}
	}
	///软复位
	if(SoftRestFlag==1)
	{
		uchar gotdata1=0;
		SoftRestFlag=0;
		if(PictureBootLoadFlag==0)
		{
			(void)IFsh1_GetByteFlash(0XF08000,&gotdata1);
			while(gotdata1 == 0xFF)
			{
				(void)IFsh1_SetByteFlash(0xF08000,0X11);///标志指令，判断是否已烧写完成

				(void)IFsh1_GetByteFlash(0XF08000,&gotdata1);
			}
			_EntryPoint();
		}
		else
		{			
			(void)IFsh1_GetByteFlash(0XF08000,&gotdata1);
			if(gotdata1 == 0x11)
			{
				PictureBootLoadFlag=0;
				_EntryPoint();
			}
		}
	}
	///主芯片256的S19数据烧录
	if(RxS19LineFlag==1)
	{		
		ErrorData=S19Program();
		if(ErrorData!=flash_OK)
		{
			err_send(ErrorData);
			CAN0_SendFrame(); 
		}
		RxS19LineFlag=0;
	}
	///图片外挂FLASH的S19数据烧录
	if(RxPicS19LineFlag!=0)
	{		
		ErrorData=PictureS19Program();
		if(ErrorData!=flash_OK)
		{
			for(delay_time=0;delay_time<65535;delay_time++)///等待一段时间后再发应答，否则网关无法接收到数据
			{
				if(delay_time%100==0)
					out_wdt_reset();
			}
			err_send(ErrorData);
			CAN0_SendFrame(); 
		}
		RxPicS19LineFlag--;
	}
	///接收校验错误
	if(RXChecksumErrorFlag==1)
	{
		err_send(RX_Checksum_Error_ACK);
		CAN0_SendFrame();
		RXChecksumErrorFlag=0;
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、烧录主芯片256的FLASH
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
uchar S19Program(void)
{
	uchar function;
	uchar offset,i,gotdata;
	uchar checksum;
	uchar ErrData=flash_OK;
	unsigned long S19Address;

	///读取S19地址
	function=S19DataBuf.S19Buf.function;
	if(function==0x03)///发送数据_S1 地址16位，高位在前
	{
		S19Address=S19DataBuf.S19Buf.S19data[0];
		S19Address <<=8;
		S19Address +=S19DataBuf.S19Buf.S19data[1];
		offset=2;
	}
	else if(function==0x04)///发送数据_S2	地址24位，高位在前
	{
		S19Address=S19DataBuf.S19Buf.S19data[0];
		S19Address <<=8;
		S19Address +=S19DataBuf.S19Buf.S19data[1];
		S19Address <<=8;
		S19Address +=S19DataBuf.S19Buf.S19data[2];		
		offset=3;
	}
	else if(function==0x05||function==0x06||function==0x07)///发送数据_S3 S4 S5，32位地址	高位在前
	{
		S19Address=S19DataBuf.S19Buf.S19data[0];
		S19Address <<=8;
		S19Address +=S19DataBuf.S19Buf.S19data[1];
		S19Address <<=8;
		S19Address +=S19DataBuf.S19Buf.S19data[2];		
		S19Address <<=8;
		S19Address +=S19DataBuf.S19Buf.S19data[3];		
		offset=4;
	}
	else
	{
		ErrData=err_flash_unkown;///接收校验错误
		return ErrData;
	}
	///判断校验和是否正确
	checksum=0x00;
	for(i=0;i<S19DataBuf.S19Buf.len;i++)
		checksum+=S19DataBuf.data[i];
	checksum=0xff-checksum;///0xff-前面的数据和为S19数据协议的校验和的值
	///最后一个数据是校验和,正确后写FLASH数据
	if(S19DataBuf.data[S19DataBuf.S19Buf.len]==checksum)
	{
		err_send(ResponseData);///先发应答，再做写入与读校验动作
		CAN0_SendFrame(); 	
	}
	else
	{
		ErrData=err_flash_checksum;		///接收校验错误
		return 	ErrData;
	}
	for(i=1;(i+offset)<S19DataBuf.S19Buf.len;i++)
	{
		(void)IFsh1_SetByteFlash(S19Address,S19DataBuf.data[i+offset]);
		(void)IFsh1_GetByteFlash(S19Address,&gotdata);
		S19Address++;
		if(gotdata!=S19DataBuf.data[i+offset])///写入的数据检验是否错误
		{
			ErrData=err_flash_Write_checksum;
			break;
		}
	}	
	return 	ErrData;	
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、烧录外挂图片的FLASH
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
uchar PictureS19Program(void)
{
	uint	i;
	uchar function;
	uchar gotdata[S19_DATA_NUMBER+8];
	uchar checksum;
	uchar ErrData=flash_OK;
	ulong S19Address;
	uchar Picture_S19_Write_Temp;///写入的次数或剩余字节的缓存
	uchar Index;
	ulong	Near_Program_Address,Temp_Program_Address;

	///读取S19地址
	function=PicS19DataBuf[Pic_S19_Write_Index].S19Buf.function;
	if(function==0x0d)///24位地址,	高位在前
	{
		S19Address=PicS19DataBuf[Pic_S19_Write_Index].S19Buf.S19data[0];
		S19Address <<=8;
		S19Address +=PicS19DataBuf[Pic_S19_Write_Index].S19Buf.S19data[1];
		S19Address <<=8;
		S19Address +=PicS19DataBuf[Pic_S19_Write_Index].S19Buf.S19data[2];		
		///S19Address <<=8;
		///S19Address +=PicS19DataBuf[Pic_S19_Write_Index].S19Buf.S19data[3];		
	}
	else
	{
		ErrData=err_flash_unkown;	
		return ErrData;
	}
	///计算接收数据的校验和
	checksum=0x00;
	for(i=0;i<PicS19DataBuf[Pic_S19_Write_Index].S19Buf.len;i++)
		checksum+=PicS19DataBuf[Pic_S19_Write_Index].data[i];
	checksum=0xff-checksum;///0xff-前面的数据和=S19数据协议的校验和的值
	///写入的数据长度应扣除：前三个地址+最后一个校验和
	Pic_Flash_Write_Len[Pic_S19_Write_Index]=PicS19DataBuf[Pic_S19_Write_Index].S19Buf.len-4;
	///按地址要求需要确认是否要擦除扇区空间，每次只擦除一个
	Near_Program_Address=S19Address+Pic_Flash_Write_Len[Pic_S19_Write_Index];
	Temp_Program_Address=Sector_Start_Adr*(ulong)FLASH_SECTOR_SIZE;
	if(Near_Program_Address>Temp_Program_Address)
	{
		ErrData=Erase_Sector(Sector_Start_Adr);
		if(ErrData!=flash_OK)
			return	ErrData;
		Sector_Start_Adr += 1;
	}
	///最后一个数据是校验和,正确后写FLASH数据
	if(PicS19DataBuf[Pic_S19_Write_Index].data[PicS19DataBuf[Pic_S19_Write_Index].S19Buf.len]==checksum)
	{
		err_send(ResponseData);///先发应答，再做写入与读校验动作
		CAN0_SendFrame(); 	
	}
	else
	{
		Pic_S19_Write_Index++;///将写入环形缓存的变量加1，表示1个环形缓存被清空
		if(Pic_S19_Write_Index>=S19_RX_MAX_SIZE)
			Pic_S19_Write_Index=0;
		ErrData=err_flash_checksum;	///接收数据的校验错误
		return 	ErrData;
	}	
	///前面3个数据为地址,所以从第4个数据开始为图片数据	
	if(Pic_Flash_Write_Len[Pic_S19_Write_Index]<PICTURE_S19_WRITE_SIZE)///一次写完
	{
		ErrData=Flash_Page_Write(S19Address,Pic_Flash_Write_Len[Pic_S19_Write_Index],&PicS19DataBuf[Pic_S19_Write_Index].S19Buf.S19data[3]);
	}
	else///分多次写入
	{
		Picture_S19_Write_Temp=Pic_Flash_Write_Len[Pic_S19_Write_Index]/PICTURE_S19_WRITE_SIZE;///写入的次数
		for(Index=0;Index<Picture_S19_Write_Temp;Index++)
		{
			ErrData=Flash_Page_Write(S19Address+Index*PICTURE_S19_WRITE_SIZE,PICTURE_S19_WRITE_SIZE,&PicS19DataBuf[Pic_S19_Write_Index].S19Buf.S19data[3+Index*PICTURE_S19_WRITE_SIZE]);
			if(ErrData!=flash_OK)
				return	ErrData;
			CAN0_ReadFrame();
		}
		Picture_S19_Write_Temp=Pic_Flash_Write_Len[Pic_S19_Write_Index]%PICTURE_S19_WRITE_SIZE;///剩下的字节
		if(Picture_S19_Write_Temp!=0)
			ErrData=Flash_Page_Write(S19Address+Index*PICTURE_S19_WRITE_SIZE,Picture_S19_Write_Temp,&PicS19DataBuf[Pic_S19_Write_Index].S19Buf.S19data[3+Index*PICTURE_S19_WRITE_SIZE]);
		if(ErrData!=flash_OK)
			return	ErrData;
	}
	CAN0_ReadFrame();
	ErrData=Flash_PageRead(S19Address,Pic_Flash_Write_Len[Pic_S19_Write_Index],gotdata);
	if(ErrData!=flash_OK)
		return	ErrData;
	///校验写入FLASH中的数据是否正确
	for(i=0;i<Pic_Flash_Write_Len[Pic_S19_Write_Index];i++)
	{
		if(PicS19DataBuf[Pic_S19_Write_Index].S19Buf.S19data[3+i]!= gotdata[i]) 
		{
			ErrData=err_flash_Write_checksum;///写入校验错误
			break;
		}
	}
	Pic_S19_Write_Index++;///将写入环形缓存的变量加1，表示1个环形缓存被清空
	if(Pic_S19_Write_Index>=S19_RX_MAX_SIZE)
		Pic_S19_Write_Index=0;
	return 	ErrData;
}

