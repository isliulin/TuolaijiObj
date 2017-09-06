#define		_NO_BOOTLOADER_EXTERN
#include	"Bootloader.h"
/************************************
*************************************/
#pragma MESSAGE DISABLE C4000 
/*************************************
**************************************/
void	Bootloader_Deal(void)
{
	unsigned char CmdData;///缓存变量
	unsigned int delay_time;
	
	if(RxS19LineFlag==0)
		CANRx_Deal();
	else
	{
		CmdData=S19Program();
		if(CmdData!=flash_OK)
		{
			for(delay_time=0;delay_time<65535;delay_time++)///等待一段时间后再发应答，否则网关无法接收到数据
			{
				if(delay_time%100==0)
				{
					__RESET_WATCHDOG(); /* feeds the dog */
					Bootloader_External_WatchDog();
				}
			}
			err_send(CmdData);
			CAN_SendFrame();
			while(1);
		}
	}
}
/****************************************
*****************************************/
static void  CANRx_Deal(void)
{
	unsigned char	Index,i;///缓冲变量
	unsigned char	Module_Ok;///缓冲变量
	
	if(CANRFLG&0x01==0x01)
	{		
		///读取ID	
		rev_can_packet.identifier = CANRIDR0;
		rev_can_packet.identifier <<= 3;
		rev_can_packet.identifier |= (CANRIDR1>>5);
		rev_can_packet.identifier <<= 3;
		rev_can_packet.identifier |= (CANRIDR1&0X07);
		rev_can_packet.identifier <<= 8;
		rev_can_packet.identifier |= CANRIDR2;
		rev_can_packet.identifier <<= 7;
		rev_can_packet.identifier |= (CANRIDR3>>1);	
		///将ID的缓存清零
		CANRIDR0=0x00;
		CANRIDR1=0x00;  
		CANRIDR2=0x00;
		CANRIDR3=0x00;   
		///读取数据帧长度	
		rev_can_packet.byte_count = (CANRDLR & 0x0F);	
		///读取数据内容
		for (Index=0; Index < rev_can_packet.byte_count; Index++)	
	    	rev_can_packet.data[Index] = *(&CANRDSR0 + Index); 

		ModuleID=GetID();
		Module_Ok=0;
		if(FrontDriveFlag==1)
		{
			if(ModuleID== FrontDriveID)
				Module_Ok=1;
		}
		else if(BackDriveFlag==1)
		{
			if(ModuleID== BackDriveID)
				Module_Ok=1;
		}
		else if(TopDriveFlag==1)
		{
			if(ModuleID== TopDriveID)
				Module_Ok=1;
		}
		else if(PowerModuleFlag==1)
		{
			if(ModuleID== PowerModuleID)
				Module_Ok=1;
		}
		else if(PowerControlModuleFlag==1)
		{
			if(ModuleID== PowerControlModuleID)
				Module_Ok=1;
		}
		else if(MainTestBoardModuleFlag==1)
		{
			if(ModuleID== MainTestBordID)
				Module_Ok=1;
		}
		else if(SlaveTestBoardModuleFlag==1)
		{
			if(ModuleID== SlaveTestBordID)
				Module_Ok=1;
		}
		if(Module_Ok==1)
		{
   			count_delay=0X00;///收到对应模块报文，清检测报文计时变量
			ResponseData=GetFunction();
			switch(ResponseData)
			{
				case 0x03:	/// 发送数据_S1					
				case 0x04:	/// 发送数据_S2
				case 0x05:	/// 发送数据_S3
				case 0x06:	/// 发送数据_S4
				case 0x07:	/// 发送数据_S5	
					BootStartFlag=1;
					S19DataBuf.S19Buf.function=ResponseData;		///保存功能码
					if(s19RxCount==0)												///当为第一包时接收其数据长度
						s19LineLen=GetS19Len();								///保存S19数据长度
					Index =s19RxCount;
					s19RxCount +=rev_can_packet.byte_count-1;	///获取总共接收的数据长度
					
					if(s19RxCount>s19LineLen)
					{
						RxS19LineFlag=1;		///接收一行数据完成
						s19RxCount=0;
					}
					for(i=1;i<rev_can_packet.byte_count;i++)	
					{
						S19DataBuf.data[Index]=rev_can_packet.data[i];
						Index++;
					}
					break;
				case 0x08:	///软复位
					while(1);
					break;
				case 0x09:	///中止烧写	
					err_send(ResponseData);
					CAN_SendFrame();
					while(1);
					break;
				default:
					break;
			}	
		}	
		CANRFLG |= 0x01;/// Clear reception flag  
	}
	else 
	{
		count_delay++;
		if(count_delay>=BootloaderDelay_Num)	///if not recieve data in 2 second,then reset
		{
			err_send(err_flash_unkown);
			CAN_SendFrame();
			while(1);///wait watchdog reset
		}
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、烧录主芯片freescale 的dz60的FLASH
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
unsigned char S19Program(void)
{
	unsigned char function;///缓冲变量
	unsigned char offset;		///所写入的地址所占几位
	unsigned char i,j,gotdata;///缓冲变量
	unsigned char checksum;///校验和的缓存
	unsigned long S19Address;///S19写入地址的缓存
	unsigned char	Error=flash_OK;///缓冲变量

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
		Error = 0XF4;///err_flash_clear;
		return	Error;
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
		CAN_SendFrame(); 	
	}
	else
	{
		Error = err_flash_checksum;///校验错误
		return	Error;
	}
	for(i=1;(i+offset)<S19DataBuf.S19Buf.len;i++)///1->指第一个所接收的数据为长度
	{
		(void)Flash_Program(S19Address,S19DataBuf.data[i+offset]);
		for(j=0;j<50;j++)
			asm nop;
		gotdata=Flash_Read(S19Address);
		S19Address++;
		if(gotdata!=S19DataBuf.data[i+offset])
		{
			Error = 0XF5;///err_flash_clear;///写入错误
			break;
		}
	}
	RxS19LineFlag=0;///再次启动接收
	return	Error;
}
/**********************************
***********************************/
void	Bootloader_External_WatchDog(void)
{
	Bootloader_External_Wdg_Io=1;
		
	if(Bootloader_External_Wdg==0)
		Bootloader_External_Wdg=1;
	else 
		Bootloader_External_Wdg=0;
}
/***********************************
************************************/
void	Erase_Flash_Zero(void)
{
	unsigned long		Erase_Address;
	unsigned char		Sector_Num=0;
	
	Erase_Address=ReStart_Flash_Write_Addres;
	for(;;)
	{
		__RESET_WATCHDOG();/*feed watch dog*/
		Bootloader_External_WatchDog();
		ClearCanBuf();///当擦除FLASH时有接收CAN报文，需把CAN缓存清除，否则接下来的操作CAN数据会异常
		if(Flash_Erase(Erase_Address))
		{
			err_send(err_flash_clear);///应答擦写FLASH 错误
			CAN_SendFrame();
			while(1);		///wait watchdog reset				
		}
		else
		{
			Erase_Address+= Flash_Sector_Area;	
			Sector_Num++;
			if(ConfigFlag==0)
			{
				if(Sector_Num>=Flash_Sector_Num)	
					break;			
			}
			else
			{
				if(Sector_Num>=Config_Sector_Num)		
					break;			
			}
		}
	}
	asm nop;
	asm nop;
	err_send(ResponseData);///应答擦除FLASH 正常
	CAN_SendFrame();
	s19RxCount=0;///为接下来接收数据准备
}
/***********************************
************************************/
static void	ClearCanBuf(void)
{
	unsigned char	Index;///缓冲变量
	
	if(CANRFLG&0x01==0x01)
	{	
		count_delay=0X00;				///收到报文，清检测报文计时变量
		///读取ID	
		rev_can_packet.identifier = CANRIDR0;
		rev_can_packet.identifier <<= 3;
		rev_can_packet.identifier |= (CANRIDR1>>5);
		rev_can_packet.identifier <<= 3;
		rev_can_packet.identifier |= (CANRIDR1&0X07);
		rev_can_packet.identifier <<= 8;
		rev_can_packet.identifier |= CANRIDR2;
		rev_can_packet.identifier <<= 7;
		rev_can_packet.identifier |= (CANRIDR3>>1);	
		CANRIDR0=0x00;///将ID的缓存清零
		CANRIDR1=0x00;  
		CANRIDR2=0x00;
		CANRIDR3=0x00;   
		///读取数据帧长度	
		rev_can_packet.byte_count = (CANRDLR & 0x0F);	
		///读取数据内容
		for (Index=0; Index < rev_can_packet.byte_count; Index++)	
	    	rev_can_packet.data[Index] = *(&CANRDSR0 + Index); 
		ResponseData=GetFunction();
		switch(ResponseData)
		{
			case 0x09:				///中止烧写		
				err_send(ResponseData);
				CAN_SendFrame();
				while(1);
				break;
			default:
				break;
		}
		CANRFLG |= 0x01;	/// Clear reception flag  
	}
}
///////////////////////////////////////////////////////
void err_send(unsigned char data)
{	
	if(FrontDriveFlag==1)
		send_can_packet.identifier= FrontDriveID;
	else if(BackDriveFlag==1)
		send_can_packet.identifier=BackDriveID;
	else if(TopDriveFlag==1)
		send_can_packet.identifier=TopDriveID;
	else if(PowerModuleFlag==1)
		send_can_packet.identifier=PowerModuleID;
	else if(PowerControlModuleFlag==1)
		send_can_packet.identifier=PowerControlModuleID;
	else if(MainTestBoardModuleFlag==1)
		send_can_packet.identifier=MainTestBordID;
	else if(SlaveTestBoardModuleFlag==1)
		send_can_packet.identifier=SlaveTestBordID;
	
	send_can_packet.byte_count= 1;
	send_can_packet.data[0] =data;
}
//////////////////////////////////////////////
void CAN_SendFrame(void)
{
	byte i,cansel;

	CANTBSEL = CANTFLG;    /// Select lowest empty buffer 
  	cansel = CANTBSEL;		 /// Backup selected buffer
	
	///PGN change to 32bit	 
	CANTIDR0 = (send_can_packet.identifier>>21) & 0xFF; 	
	CANTIDR1 = (send_can_packet.identifier>>18) & 0x07; 
	CANTIDR1 = CANTIDR1 << 5;
	CANTIDR1 = CANTIDR1 | ((send_can_packet.identifier>>15) & 0x07);
	CANTIDR1_SRR = 1;
	CANTIDR1_IDE = 1; 		///extend frame IDE=1->29bit,IDE=0->11bit		
	CANTIDR2 = (send_can_packet.identifier>>7) & 0xFF; 
	CANTIDR3 = (send_can_packet.identifier<<1) & 0xFF; 
	CANTIDR3_RTR = 0; 		///data frame	

	CANTDLR=send_can_packet.byte_count;	///length of data
	for(i=0;i<send_can_packet.byte_count;i++)
	{
		*(&CANTDSR0+i)=send_can_packet.data[i];
	}
	CANTTBPR=1;         ///configure data fifo of send priory
    CANTFLG=cansel;	
}

