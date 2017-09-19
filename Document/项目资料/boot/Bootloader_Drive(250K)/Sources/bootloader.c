#define		_NO_BOOTLOADER_EXTERN
#include	"Bootloader.h"
/************************************
*************************************/
#pragma MESSAGE DISABLE C4000 
/*************************************
**************************************/
void	Bootloader_Deal(void)
{
	unsigned char CmdData;///�������
	unsigned int delay_time;
	
	if(RxS19LineFlag==0)
		CANRx_Deal();
	else
	{
		CmdData=S19Program();
		if(CmdData!=flash_OK)
		{
			for(delay_time=0;delay_time<65535;delay_time++)///�ȴ�һ��ʱ����ٷ�Ӧ�𣬷��������޷����յ�����
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
	unsigned char	Index,i;///�������
	unsigned char	Module_Ok;///�������
	
	if(CANRFLG&0x01==0x01)
	{		
		///��ȡID	
		rev_can_packet.identifier = CANRIDR0;
		rev_can_packet.identifier <<= 3;
		rev_can_packet.identifier |= (CANRIDR1>>5);
		rev_can_packet.identifier <<= 3;
		rev_can_packet.identifier |= (CANRIDR1&0X07);
		rev_can_packet.identifier <<= 8;
		rev_can_packet.identifier |= CANRIDR2;
		rev_can_packet.identifier <<= 7;
		rev_can_packet.identifier |= (CANRIDR3>>1);	
		///��ID�Ļ�������
		CANRIDR0=0x00;
		CANRIDR1=0x00;  
		CANRIDR2=0x00;
		CANRIDR3=0x00;   
		///��ȡ����֡����	
		rev_can_packet.byte_count = (CANRDLR & 0x0F);	
		///��ȡ��������
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
   			count_delay=0X00;///�յ���Ӧģ�鱨�ģ����ⱨ�ļ�ʱ����
			ResponseData=GetFunction();
			switch(ResponseData)
			{
				case 0x03:	/// ��������_S1					
				case 0x04:	/// ��������_S2
				case 0x05:	/// ��������_S3
				case 0x06:	/// ��������_S4
				case 0x07:	/// ��������_S5	
					BootStartFlag=1;
					S19DataBuf.S19Buf.function=ResponseData;		///���湦����
					if(s19RxCount==0)												///��Ϊ��һ��ʱ���������ݳ���
						s19LineLen=GetS19Len();								///����S19���ݳ���
					Index =s19RxCount;
					s19RxCount +=rev_can_packet.byte_count-1;	///��ȡ�ܹ����յ����ݳ���
					
					if(s19RxCount>s19LineLen)
					{
						RxS19LineFlag=1;		///����һ���������
						s19RxCount=0;
					}
					for(i=1;i<rev_can_packet.byte_count;i++)	
					{
						S19DataBuf.data[Index]=rev_can_packet.data[i];
						Index++;
					}
					break;
				case 0x08:	///��λ
					while(1);
					break;
				case 0x09:	///��ֹ��д	
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
һ���������ͣ��ײ�������Ӧ�ò㡢��ʼ��������ʼ��
��������������
1����¼��оƬfreescale ��dz60��FLASH
�����β�������
�ġ�����ֵ����
�塢���ļ�¼������ǵײ���������ʼ�����ͳ���ʱ����
1��
�������ò�����ʽ��
1��
***************************************************************************/
unsigned char S19Program(void)
{
	unsigned char function;///�������
	unsigned char offset;		///��д��ĵ�ַ��ռ��λ
	unsigned char i,j,gotdata;///�������
	unsigned char checksum;///У��͵Ļ���
	unsigned long S19Address;///S19д���ַ�Ļ���
	unsigned char	Error=flash_OK;///�������

	///��ȡS19��ַ
	function=S19DataBuf.S19Buf.function;
	if(function==0x03)///��������_S1 ��ַ16λ����λ��ǰ
	{
		S19Address=S19DataBuf.S19Buf.S19data[0];
		S19Address <<=8;
		S19Address +=S19DataBuf.S19Buf.S19data[1];
		offset=2;
	}
	else if(function==0x04)///��������_S2	��ַ24λ����λ��ǰ
	{
		S19Address=S19DataBuf.S19Buf.S19data[0];
		S19Address <<=8;
		S19Address +=S19DataBuf.S19Buf.S19data[1];
		S19Address <<=8;
		S19Address +=S19DataBuf.S19Buf.S19data[2];		
		offset=3;
	}
	else if(function==0x05||function==0x06||function==0x07)///��������_S3 S4 S5��32λ��ַ	��λ��ǰ
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
	///�ж�У����Ƿ���ȷ
	checksum=0x00;
	for(i=0;i<S19DataBuf.S19Buf.len;i++)
		checksum+=S19DataBuf.data[i];
	checksum=0xff-checksum;///0xff-ǰ������ݺ�ΪS19����Э���У��͵�ֵ
	///���һ��������У���,��ȷ��дFLASH����
	if(S19DataBuf.data[S19DataBuf.S19Buf.len]==checksum)
	{
		err_send(ResponseData);///�ȷ�Ӧ������д�����У�鶯��
		CAN_SendFrame(); 	
	}
	else
	{
		Error = err_flash_checksum;///У�����
		return	Error;
	}
	for(i=1;(i+offset)<S19DataBuf.S19Buf.len;i++)///1->ָ��һ�������յ�����Ϊ����
	{
		(void)Flash_Program(S19Address,S19DataBuf.data[i+offset]);
		for(j=0;j<50;j++)
			asm nop;
		gotdata=Flash_Read(S19Address);
		S19Address++;
		if(gotdata!=S19DataBuf.data[i+offset])
		{
			Error = 0XF5;///err_flash_clear;///д�����
			break;
		}
	}
	RxS19LineFlag=0;///�ٴ���������
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
		ClearCanBuf();///������FLASHʱ�н���CAN���ģ����CAN�������������������Ĳ���CAN���ݻ��쳣
		if(Flash_Erase(Erase_Address))
		{
			err_send(err_flash_clear);///Ӧ���дFLASH ����
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
	err_send(ResponseData);///Ӧ�����FLASH ����
	CAN_SendFrame();
	s19RxCount=0;///Ϊ��������������׼��
}
/***********************************
************************************/
static void	ClearCanBuf(void)
{
	unsigned char	Index;///�������
	
	if(CANRFLG&0x01==0x01)
	{	
		count_delay=0X00;				///�յ����ģ����ⱨ�ļ�ʱ����
		///��ȡID	
		rev_can_packet.identifier = CANRIDR0;
		rev_can_packet.identifier <<= 3;
		rev_can_packet.identifier |= (CANRIDR1>>5);
		rev_can_packet.identifier <<= 3;
		rev_can_packet.identifier |= (CANRIDR1&0X07);
		rev_can_packet.identifier <<= 8;
		rev_can_packet.identifier |= CANRIDR2;
		rev_can_packet.identifier <<= 7;
		rev_can_packet.identifier |= (CANRIDR3>>1);	
		CANRIDR0=0x00;///��ID�Ļ�������
		CANRIDR1=0x00;  
		CANRIDR2=0x00;
		CANRIDR3=0x00;   
		///��ȡ����֡����	
		rev_can_packet.byte_count = (CANRDLR & 0x0F);	
		///��ȡ��������
		for (Index=0; Index < rev_can_packet.byte_count; Index++)	
	    	rev_can_packet.data[Index] = *(&CANRDSR0 + Index); 
		ResponseData=GetFunction();
		switch(ResponseData)
		{
			case 0x09:				///��ֹ��д		
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

