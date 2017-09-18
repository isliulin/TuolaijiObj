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
  
  asm sei;///�ر��ж�
	
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
һ���������ͣ��ײ�������Ӧ�ò㡢��ʼ������Ӧ�ò�
��������������
1��
�����β�������
�ġ�����ֵ����
�塢���ļ�¼������ǵײ���������ʼ�����ͳ���ʱ����
1��
�������ò�����ʽ��
1��
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
һ���������ͣ��ײ�������Ӧ�ò㡢��ʼ������Ӧ�ò�
��������������
1��
�����β�������
�ġ�����ֵ����
�塢���ļ�¼������ǵײ���������ʼ�����ͳ���ʱ����
1��
�������ò�����ʽ��
1��
***************************************************************************/
void BootloadDeal(void)
{	
	uchar ErrorData;
	uint	delay_time;
	
	///������оƬFLASH�׵�ַ
	if(ErasureStartFlag==1)
	{		
		ErrorData=chip_erase();
		if(ErrorData==ERR_OK)///������ȷ
		{						 
			err_send(ResponseData);
		}
		else	///��������  
		{						 
			err_send(err_flash_clear);
		}
		CAN0_SendFrame();
		ErasureStartFlag=0;
	}
	///����ͼƬFLASH�׵�ַ
	if(ErasurePicStartFlag==1)
	{		
		Sector_Start_Adr = (uint)(FirstS19Address/FLASH_SECTOR_SIZE);
		ErrorData=Erase_Sector(Sector_Start_Adr);///ÿ��ֻ����һ������
		if(ErrorData==flash_OK)///������ȷ
		{						 
			err_send(ResponseData);
			Sector_Start_Adr += 1;///Ϊ��һ����������׼��
		}
		else	///��������  
		{						 
			err_send(err_flash_clear);
		}
		CAN0_SendFrame();
		ErasurePicStartFlag=0;
	}
	///��ֹ����
	if(BootStopFlag==1)
	{
		BootStopFlag=0;
		err_send(ResponseData);
		CAN0_SendFrame();
		_EntryPoint();
	}
	///׼����¼
	if(BootReadyFlag==1)
	{
		BootReadyFlag=0;
		Bootloader_Mode_ChangeFlag=1;
		Bootloader_Mode_Change();///ΪͼƬFLASH׼��
		err_send(ResponseData);
		CAN0_SendFrame();
		for(delay_time=0;delay_time<1000;delay_time++)
		{
			if(delay_time%100==0)
				out_wdt_reset();
		}
	}
	///��λ
	if(SoftRestFlag==1)
	{
		uchar gotdata1=0;
		SoftRestFlag=0;
		if(PictureBootLoadFlag==0)
		{
			(void)IFsh1_GetByteFlash(0XF08000,&gotdata1);
			while(gotdata1 == 0xFF)
			{
				(void)IFsh1_SetByteFlash(0xF08000,0X11);///��־ָ��ж��Ƿ�����д���

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
	///��оƬ256��S19������¼
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
	///ͼƬ���FLASH��S19������¼
	if(RxPicS19LineFlag!=0)
	{		
		ErrorData=PictureS19Program();
		if(ErrorData!=flash_OK)
		{
			for(delay_time=0;delay_time<65535;delay_time++)///�ȴ�һ��ʱ����ٷ�Ӧ�𣬷��������޷����յ�����
			{
				if(delay_time%100==0)
					out_wdt_reset();
			}
			err_send(ErrorData);
			CAN0_SendFrame(); 
		}
		RxPicS19LineFlag--;
	}
	///����У�����
	if(RXChecksumErrorFlag==1)
	{
		err_send(RX_Checksum_Error_ACK);
		CAN0_SendFrame();
		RXChecksumErrorFlag=0;
	}
}
/***************************************************************************
һ���������ͣ��ײ�������Ӧ�ò㡢��ʼ��������ʼ��
��������������
1����¼��оƬ256��FLASH
�����β�������
�ġ�����ֵ����
�塢���ļ�¼������ǵײ���������ʼ�����ͳ���ʱ����
1��
�������ò�����ʽ��
1��
***************************************************************************/
uchar S19Program(void)
{
	uchar function;
	uchar offset,i,gotdata;
	uchar checksum;
	uchar ErrData=flash_OK;
	unsigned long S19Address;

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
		ErrData=err_flash_unkown;///����У�����
		return ErrData;
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
		CAN0_SendFrame(); 	
	}
	else
	{
		ErrData=err_flash_checksum;		///����У�����
		return 	ErrData;
	}
	for(i=1;(i+offset)<S19DataBuf.S19Buf.len;i++)
	{
		(void)IFsh1_SetByteFlash(S19Address,S19DataBuf.data[i+offset]);
		(void)IFsh1_GetByteFlash(S19Address,&gotdata);
		S19Address++;
		if(gotdata!=S19DataBuf.data[i+offset])///д������ݼ����Ƿ����
		{
			ErrData=err_flash_Write_checksum;
			break;
		}
	}	
	return 	ErrData;	
}
/***************************************************************************
һ���������ͣ��ײ�������Ӧ�ò㡢��ʼ��������ʼ��
��������������
1����¼���ͼƬ��FLASH
�����β�������
�ġ�����ֵ����
�塢���ļ�¼������ǵײ���������ʼ�����ͳ���ʱ����
1��
�������ò�����ʽ��
1��
***************************************************************************/
uchar PictureS19Program(void)
{
	uint	i;
	uchar function;
	uchar gotdata[S19_DATA_NUMBER+8];
	uchar checksum;
	uchar ErrData=flash_OK;
	ulong S19Address;
	uchar Picture_S19_Write_Temp;///д��Ĵ�����ʣ���ֽڵĻ���
	uchar Index;
	ulong	Near_Program_Address,Temp_Program_Address;

	///��ȡS19��ַ
	function=PicS19DataBuf[Pic_S19_Write_Index].S19Buf.function;
	if(function==0x0d)///24λ��ַ,	��λ��ǰ
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
	///����������ݵ�У���
	checksum=0x00;
	for(i=0;i<PicS19DataBuf[Pic_S19_Write_Index].S19Buf.len;i++)
		checksum+=PicS19DataBuf[Pic_S19_Write_Index].data[i];
	checksum=0xff-checksum;///0xff-ǰ������ݺ�=S19����Э���У��͵�ֵ
	///д������ݳ���Ӧ�۳���ǰ������ַ+���һ��У���
	Pic_Flash_Write_Len[Pic_S19_Write_Index]=PicS19DataBuf[Pic_S19_Write_Index].S19Buf.len-4;
	///����ַҪ����Ҫȷ���Ƿ�Ҫ���������ռ䣬ÿ��ֻ����һ��
	Near_Program_Address=S19Address+Pic_Flash_Write_Len[Pic_S19_Write_Index];
	Temp_Program_Address=Sector_Start_Adr*(ulong)FLASH_SECTOR_SIZE;
	if(Near_Program_Address>Temp_Program_Address)
	{
		ErrData=Erase_Sector(Sector_Start_Adr);
		if(ErrData!=flash_OK)
			return	ErrData;
		Sector_Start_Adr += 1;
	}
	///���һ��������У���,��ȷ��дFLASH����
	if(PicS19DataBuf[Pic_S19_Write_Index].data[PicS19DataBuf[Pic_S19_Write_Index].S19Buf.len]==checksum)
	{
		err_send(ResponseData);///�ȷ�Ӧ������д�����У�鶯��
		CAN0_SendFrame(); 	
	}
	else
	{
		Pic_S19_Write_Index++;///��д�뻷�λ���ı�����1����ʾ1�����λ��汻���
		if(Pic_S19_Write_Index>=S19_RX_MAX_SIZE)
			Pic_S19_Write_Index=0;
		ErrData=err_flash_checksum;	///�������ݵ�У�����
		return 	ErrData;
	}	
	///ǰ��3������Ϊ��ַ,���Դӵ�4�����ݿ�ʼΪͼƬ����	
	if(Pic_Flash_Write_Len[Pic_S19_Write_Index]<PICTURE_S19_WRITE_SIZE)///һ��д��
	{
		ErrData=Flash_Page_Write(S19Address,Pic_Flash_Write_Len[Pic_S19_Write_Index],&PicS19DataBuf[Pic_S19_Write_Index].S19Buf.S19data[3]);
	}
	else///�ֶ��д��
	{
		Picture_S19_Write_Temp=Pic_Flash_Write_Len[Pic_S19_Write_Index]/PICTURE_S19_WRITE_SIZE;///д��Ĵ���
		for(Index=0;Index<Picture_S19_Write_Temp;Index++)
		{
			ErrData=Flash_Page_Write(S19Address+Index*PICTURE_S19_WRITE_SIZE,PICTURE_S19_WRITE_SIZE,&PicS19DataBuf[Pic_S19_Write_Index].S19Buf.S19data[3+Index*PICTURE_S19_WRITE_SIZE]);
			if(ErrData!=flash_OK)
				return	ErrData;
			CAN0_ReadFrame();
		}
		Picture_S19_Write_Temp=Pic_Flash_Write_Len[Pic_S19_Write_Index]%PICTURE_S19_WRITE_SIZE;///ʣ�µ��ֽ�
		if(Picture_S19_Write_Temp!=0)
			ErrData=Flash_Page_Write(S19Address+Index*PICTURE_S19_WRITE_SIZE,Picture_S19_Write_Temp,&PicS19DataBuf[Pic_S19_Write_Index].S19Buf.S19data[3+Index*PICTURE_S19_WRITE_SIZE]);
		if(ErrData!=flash_OK)
			return	ErrData;
	}
	CAN0_ReadFrame();
	ErrData=Flash_PageRead(S19Address,Pic_Flash_Write_Len[Pic_S19_Write_Index],gotdata);
	if(ErrData!=flash_OK)
		return	ErrData;
	///У��д��FLASH�е������Ƿ���ȷ
	for(i=0;i<Pic_Flash_Write_Len[Pic_S19_Write_Index];i++)
	{
		if(PicS19DataBuf[Pic_S19_Write_Index].S19Buf.S19data[3+i]!= gotdata[i]) 
		{
			ErrData=err_flash_Write_checksum;///д��У�����
			break;
		}
	}
	Pic_S19_Write_Index++;///��д�뻷�λ���ı�����1����ʾ1�����λ��汻���
	if(Pic_S19_Write_Index>=S19_RX_MAX_SIZE)
		Pic_S19_Write_Index=0;
	return 	ErrData;
}

