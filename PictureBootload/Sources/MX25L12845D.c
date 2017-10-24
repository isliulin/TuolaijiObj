#define	  _NO_MX25L12845D_EXTERN
#include "MX25L12845D.h"
#include "can0.h"
/********************************************************************************* 
*����ԭ�ͣ�SPI_WriteByte(U8 byte) 
*�������ܣ� SCK��������оƬ����������Ч  
*���������� 
*����ֵ�� 
*˵��������ǣ�sckΪ�ߣ�������sck��Ϊ�� 
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
*����ԭ�ͣ�U8 SPI_ReadByte() 
*�������ܣ�SCK�½��ش�оƬ����������Ч 
*���������� 
*����ֵ�� 
*˵��������ǣ�sckΪ�ߣ�������sck��Ϊ�� 
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
һ���������ͣ��ײ�������Ӧ�ò㡢��ʼ��������ʼ��
��������������
1��
�����β�������
�ġ�����ֵ����
�塢���ļ�¼������ǵײ���������ʼ�����ͳ���ʱ����
1��
�������ò�����ʽ��
1��
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
		if(WaitDelay%1000==0)///���ȴ���ʱ�����Ϊ1000�ı���ʱ�������ÿ��Ź�
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
	while(state_reg&0x01); ///�Ƿ��ڿ���״̬
	CS_H(); 
	return	Error_Data;
} 
/***************************************************************************
һ���������ͣ��ײ�������Ӧ�ò㡢��ʼ��������ʼ��
��������������
1��
�����β�������
�ġ�����ֵ����
�塢���ļ�¼������ǵײ���������ʼ�����ͳ���ʱ����
1��
�������ò�����ʽ��
1��
***************************************************************************/ 
uchar Flash_Read_ID(void)   //ID��ȡ 
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
*����ԭ�ͣ�void Erase_Flash(void) 
*�������ܣ���Ƭ���� 
*���������� 
*����ֵ�� 
*˵������Ƭ����ʱ��Ϊ10������ 
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
//����ԭ�ͣ�void Erase_Sector(U16 i) 
//�������ܣ���������������ʱ��90~300ms
//�������������ַ 
//����ֵ�� 
//˵����  4K-byte	  4096sector  addr:0~4095
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
  SPI_WriteByte(0x00); ///��FLASH
  CS_H(); 
  Error_Data=Flash_WaitBusy();
	return	Error_Data;
} 
/********************************************************************************* 
//����ԭ�ͣ�void Erase_Block(U16 i) 
//�������ܣ������ ������ʱ�䣺0.7S~2S
//�������������ַ 
//����ֵ�� 
//˵����  64K-byte	   256 block
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
  SPI_WriteByte(0x00); ///��FLASH					   
  CS_H(); 
  Error_Data=Flash_WaitBusy();
  return	Error_Data;
} 
/********************************************************************************* 
*����ԭ�ͣ�Flash_PageWrite(u16 page,u8 *Data_Buf) 
*�������ܣ� 
*���������� 16MB = 256B*65536    128Mb/8=16MB=16777216�ֽ�
*����ֵ�� 
*˵����	 ��ȡһ��ҳ256B��ҳ��Χ0-65535 Page��
						134217728bit/8  = 16777216�ֽڣ��ֽڵ�ַ0~ 16777215
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
		if(i%1000==0)///�����������ݹ���ʱ����һ�����ÿ��Ź�
	  	out_wdt_reset();	
	} 
  	CS_H(); 
  
  	Error_Data=Flash_WaitBusy(); 
	return	Error_Data;
} 
/********************************************************************************* 
*����ԭ�ͣ�Flash_PageWrite(u16 page,u8 *Data_Buf) 
*�������ܣ� 
*���������� 16MB = 256B*65536    128Mb/8=16MB=16777216�ֽ�
*����ֵ�� 
*˵����	 ��ȡһ��ҳ256B��ҳ��Χ0-65535 Page��134217728bit/8  = 16777216�ֽڣ��ֽڵ�ַ0~ 16777215
*********************************************************************************/ 
uchar Flash_Page_Write(ulong addr,uint Data_Len,uchar *pdata) 
{ 
	uchar	Error_Data;
	uint	star_page,star_addr;
	uint	empty_len,nextpage_use_len;
	uint i,Page_Number;

	///ҳ�뼰���׵�ַ
	star_page=(uint)(addr/FLASH_PAGE_SIZE);
	star_addr=(uint)(addr%FLASH_PAGE_SIZE);

	empty_len=FLASH_PAGE_SIZE-star_addr;///�Ƿ�ʣ���㹻�Ŀռ���д��ǰ������
	if(empty_len>=Data_Len)///�������һҳ�ķ�Χ����ֱ��д
	{   
    	Error_Data=Flash_PageWrite(star_page,star_addr,Data_Len,pdata);
		if(Error_Data!=flash_OK)
			return	Error_Data;
	}
	else 
	{
		Error_Data=Flash_PageWrite(star_page,star_addr,empty_len,pdata);///�Ȱ�ʣ��ҳ�����õĿռ�����
		if(Error_Data!=flash_OK)
			return	Error_Data;
		star_page=star_page+1;
	  	star_addr=0;
		Page_Number=Data_Len/FLASH_PAGE_SIZE;
		if(Page_Number==0)///����һҳ
		{
		  nextpage_use_len=Data_Len-empty_len;
		  Error_Data=Flash_PageWrite(star_page,star_addr,nextpage_use_len,pdata+empty_len);
			if(Error_Data!=flash_OK)
				return	Error_Data;
		}
		else	///����һҳ
		{
			nextpage_use_len=Data_Len%FLASH_PAGE_SIZE;
			for(i=0;i<Page_Number;i++)
			{
				Error_Data=Flash_PageWrite(star_page,star_addr,FLASH_PAGE_SIZE,pdata+empty_len+FLASH_PAGE_SIZE*i);///��д��ҳ��д����һҳ
				if(Error_Data!=flash_OK)
					return	Error_Data;
			}
			Error_Data=Flash_PageWrite(star_page,star_addr,nextpage_use_len,pdata+empty_len+FLASH_PAGE_SIZE*Page_Number);
		}
	}
	return	Error_Data;
} 
/********************************************************************************* 
*����ԭ�ͣ�void Flash_PageRead(U16 page,U8 *Data_Buf) 
*�������ܣ� 
*���������� 16MB = 256B*65536
*����ֵ�� 
*˵����	 ��ȡһ��ҳ256B��ҳ��Χ0-65536 Page 
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
		if(i%1000==0)///�����������ݹ���ʱ����һ�����ÿ��Ź�
    	out_wdt_reset();					
  } 
  CS_H();	   
  
  Error_Data=Flash_WaitBusy(); 
	return	Error_Data;
}
/***************************************************************************
һ���������ͣ��ײ�������Ӧ�ò㡢��ʼ��������ʼ��
��������������
1��
�����β�������
�ġ�����ֵ����
�塢���ļ�¼������ǵײ���������ʼ�����ͳ���ʱ����
1��
�������ò�����ʽ��
1��
***************************************************************************/
void	Bootloader_Mode_Change(void)
{
	///����Ƶ�幩��
	V_5V_CTRL_IO=1;
	V_5V_CTRL=0;       
	///�������FLASHʹ�ܽ�
	DOWNLOAD_CTRL_IO=1; 
	DOWNLOAD_CTRL=1; 
	///��ƽת��оƬ��S�ţ���ģ���л�
	MODE_CTRL_IO=1;
	MODE_CTRL=1;///Ϊ�ߵ�ƽʱ����д���FLASH
	///�ź�����
	ISD_MISO_IO=0;
	///�ź������
	ISD_MOSI_IO=1;
	ISD_MOSI=1;
	///ʱ�ӽ�
	ISD_SCLK_IO=1;
	ISD_SCLK=1;
	///ʹ�ܽ�
	ISD_CS_IO=1; 
	ISD_CS=1;
}

