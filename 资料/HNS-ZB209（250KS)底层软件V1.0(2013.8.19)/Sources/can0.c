#define	_NO_can0_EXTERN
#include "can0.h"
#include "MX25L12845D.h"

#pragma CODE_SEG IFsh1_CODE

////////////////////////////////////////////////////////////
void CAN0Init(void)
{
	unsigned long identifier;

  CAN0CTL0_INITRQ=1;              	/* request into initial mode */ 
  while(!CAN0CTL1_INITAK);        	/* wait into intial mode */  

  CAN0CTL1 = 0x80;               		/*open normal  mode,OSC source*/
  CAN0IDAC_IDAM = 0;              	/*two 32 bit acceptance filter*/
#if 1	
	identifier=(unsigned long )METER_ID;
	CAN0IDAR0 = (identifier>>21) & 0xFF; 	///PGN change to 32bit	 
	CAN0IDAR1 = (identifier>>18) & 0x07; 
	CAN0IDAR1 = CAN0IDAR1 << 5;
	CAN0IDAR1 = CAN0IDAR1 | ((identifier>>15) & 0x07);
	CAN0IDAR2 = (identifier>>7) & 0xFF; 
	CAN0IDAR3 = (identifier<<1) & 0xFF; 

	CAN0IDAR4 =CAN0IDAR0;
	CAN0IDAR5 =CAN0IDAR1;
	CAN0IDAR6 =CAN0IDAR2;
	CAN0IDAR7 =CAN0IDAR3;

  CAN0IDMR0 = 0x00;               	/*set filter*/
  CAN0IDMR1 = 0x18;
  CAN0IDMR2 = 0x00;
  CAN0IDMR3 = 0x01;
  CAN0IDMR4 = 0x00;
  CAN0IDMR5 = 0x18;
  CAN0IDMR6 = 0x00;
  CAN0IDMR7 = 0x01;
#else
	CAN0IDMR0 = 0xff;               	/*not set filter*/
  CAN0IDMR1 = 0xff;
  CAN0IDMR2 = 0xff;
  CAN0IDMR3 = 0xff;
  CAN0IDMR4 = 0xff;
  CAN0IDMR5 = 0xff;
  CAN0IDMR6 = 0xff;
  CAN0IDMR7 = 0xff;
#endif  
  CAN0BTR0 = 0x87;             			/*8M OSC,SJW is 3 Tq cycle,prescaler value is 8 bit rate is about 250k*/
  CAN0BTR1 = 0x32;             			/*one sample per bit,time segment 1 is 2 Tq and time segment 2 is 3 Tq*/ 
  CAN0CTL1_CLKSRC = 0;              /* Select the clock source from crystal */  
  CAN0CTL0_INITRQ = 0;              /* Start device */
  while(CAN0CTL1_INITAK);          /* Wait for device initialization acknowledge */
  while(!CAN0CTL0_SYNCH); 

  CAN0RFLG |= 254; /* Reset Receiver Flags*/

}
///////////////////////////////////////////////
void CAN0_ReadFrame(void)
{
  uchar i,function;
	uchar Index;
	uint	delay_time;  
	
	if ((CAN0RFLG&0x01)!=0x01)	///���յ�����
	   return;
	else 
		count_delay=0X00;				///�յ����ģ�

	rev_can_packet.identifier = CAN0RXIDR0;
	rev_can_packet.identifier <<= 3;
	rev_can_packet.identifier |= (CAN0RXIDR1>>5);
	rev_can_packet.identifier <<= 3;
	rev_can_packet.identifier |= (CAN0RXIDR1&0X07);
	rev_can_packet.identifier <<= 8;
	rev_can_packet.identifier |= CAN0RXIDR2;
	rev_can_packet.identifier <<= 7;
	rev_can_packet.identifier |= (CAN0RXIDR3>>1);	

	CAN0RXIDR0=0x00;
	CAN0RXIDR1=0x00;  
	CAN0RXIDR2=0x00;
	CAN0RXIDR3=0x00;   

	if(rev_can_packet.identifier==METER_ID)
	{	
		rev_can_packet.byte_count= CAN0RDLR & 0x0F;		///Result length of the message
		
  	for (i=0; i<rev_can_packet.byte_count; i++) 
    	rev_can_packet.data[i] = *(&CAN0RDSR0 + i); ///receive data

		function=GetMeterFunction();
		ResponseData =function;
		switch(function)
		{
			case 0x03:	/// ��������_S1					
			case 0x04:	/// ��������_S2
			case 0x05:	/// ��������_S3
			case 0x06:	/// ��������_S4
			case 0x07:	/// ��������_S5	
				BootStartFlag=1;
				S19DataBuf.S19Buf.function=function;	///���湦����
				if(s19RxCount==0)///�������ݵĵ�1֡ʱ
					s19LineLen=GetMeterS19Len();										///����S19���ݳ���
				Index =s19RxCount;
				s19RxCount +=rev_can_packet.byte_count-1;					///��ȡ�ܹ����յ����ݳ���
				
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
			case 0x01:		///��¼׼��
				PictureBootLoadFlag=0;
				BootStartFlag=0;
				BootReadyFlag=1;
				s19RxCount=0;
				Pic_Buffer_Intial();
				break;
			case 0x02:	///������оƬ256��FLASH�׵�ַ			
				if(BootStartFlag==0)
				{
					BootStartFlag=1;
					s19RxCount=0;
					BootLoader_Content=rev_can_packet.data[1];
					Sector_Number=rev_can_packet.data[2];
					Sector_Number<<=8;
					Sector_Number+=rev_can_packet.data[3];
					
					FirstS19Address=rev_can_packet.data[4];
					FirstS19Address <<=8;
					FirstS19Address +=rev_can_packet.data[5];
					FirstS19Address <<=8;
					FirstS19Address +=rev_can_packet.data[6];			
					FirstS19Address <<=8;
					FirstS19Address +=rev_can_packet.data[7];			
					ErasureStartFlag=1;
				}
				break;
			case 0x08:	///��λ
				s19RxCount=0;
				Pic_Buffer_Intial();
				BootStartFlag=0;	///����¼�б�־
				SoftRestFlag=1;
				break;
			case 0x09:	///��ֹ��д		
				s19RxCount=0;
				Pic_Buffer_Intial();
				BootStartFlag=0;	///����¼�б�־
				BootStopFlag=1;
				break;
			case 0x0c:///�������ͼƬFLASH�׵�ַ
				if(BootStartFlag==0)
				{
					if(Bootloader_Mode_ChangeFlag==0)
					{
						Bootloader_Mode_ChangeFlag=1;
						Bootloader_Mode_Change();///ΪͼƬFLASH׼��				
						for(delay_time=0;delay_time<1000;delay_time++)
						{
							if(delay_time%100==0)
								out_wdt_reset();
						}
					}
					BootStartFlag=1;
					PictureBootLoadFlag=1;
					Pic_Buffer_Intial();
					BootLoader_Content=rev_can_packet.data[1];
					Sector_Number=rev_can_packet.data[2];
					Sector_Number<<=8;
					Sector_Number+=rev_can_packet.data[3];
					
					FirstS19Address=rev_can_packet.data[4];
					FirstS19Address <<=8;
					FirstS19Address +=rev_can_packet.data[5];
					FirstS19Address <<=8;
					FirstS19Address +=rev_can_packet.data[6];			
					FirstS19Address <<=8;
					FirstS19Address +=rev_can_packet.data[7];	
					ErasurePicStartFlag=1;
				}
				break;
			case 0x0d:///��������_ͼƬ
				BootStartFlag=1;
				PicS19DataBuf[Pic_S19_RX_Index].S19Buf.function=function;	///���湦����
				if(PicS19RxCount[Pic_S19_RX_Index]==0)
					PicS19LineLen[Pic_S19_RX_Index]=GetMeterS19Len();		///����S19���ݳ���
				Index =PicS19RxCount[Pic_S19_RX_Index];
				PicS19RxCount[Pic_S19_RX_Index] +=rev_can_packet.byte_count-1;	///��ȡ�ܹ����յ����ݳ���
				
				for(i=1;i<rev_can_packet.byte_count;i++)
				{
					PicS19DataBuf[Pic_S19_RX_Index].data[Index]=rev_can_packet.data[i];
					Index++;
				}
				if(PicS19RxCount[Pic_S19_RX_Index]>PicS19LineLen[Pic_S19_RX_Index])
				{
					RxPicS19LineFlag++;		///����һ���������
					PicS19RxCount[Pic_S19_RX_Index]=0;
					Pic_S19_RX_Index++;///�����λ���ı�����1����ʾ1�����λ��汻ռ��
					if(Pic_S19_RX_Index>=S19_RX_MAX_SIZE)
						Pic_S19_RX_Index=0;
				}
				break;
			case RX_Checksum_Error_ACK:///����λ���յ�0xF1�Ĵ���ʱ�������ϳ�������дͼƬ��FLASH��
				Pic_Buffer_Intial();
				RXChecksumErrorFlag=1;
				break;
			default:
				break;
		}
	}
  CAN0RFLG = 0X81;       ///Reset the receive interrupt request flag
}
//////////////////////////////////////////////
static void	Pic_Buffer_Intial(void)
{
	unsigned char Index;

	Pic_S19_RX_Index=0;
	Pic_S19_Write_Index=0;
	for(Index=0;Index<S19_RX_MAX_SIZE;Index++)
	{
		PicS19RxCount[Index]=0x00;
		PicS19LineLen[Index]=0x00;
		Pic_Flash_Write_Len[Index]=0x00;
	}
}
//////////////////////////////////////////////
void CAN0_SendFrame(void)
{
	unsigned char i,cansel;

	CAN0TBSEL = CAN0TFLG; 		///select one available transmit buffer
	cansel = CAN0TBSEL;
	
	///PGN change to 32bit	 
	CAN0TXIDR0 = (send_can_packet.identifier>>21) & 0xFF; 	
	CAN0TXIDR1 = (send_can_packet.identifier>>18) & 0x07; 
	CAN0TXIDR1 = CAN0TXIDR1 << 5;
	CAN0TXIDR1 = CAN0TXIDR1 | ((send_can_packet.identifier>>15) & 0x07);
	CAN0TXIDR1_SRR = 1;
	CAN0TXIDR1_IDE = 1; 										///extend frame IDE=1->29bit,IDE=0->11bit		
	CAN0TXIDR2 = (send_can_packet.identifier>>7) & 0xFF; 
	CAN0TXIDR3 = (send_can_packet.identifier<<1) & 0xFF; 
	CAN0TXIDR3_RTR = 0; 										///data frame	

	CAN0TXDLR=send_can_packet.byte_count;	///length of data
	for(i=0;i<send_can_packet.byte_count;i++)
	{
		*(&CAN0TXDSR0+i)=send_can_packet.data[i];
	}
	CAN0TXTBPR=1;         ///configure data fifo of send priory
  CAN0TFLG=cansel;	
}
///////////////////////////////////////////////////////
void err_send(uchar data)
{	
	send_can_packet.identifier= METER_ID;
	send_can_packet.byte_count= 1;
	send_can_packet.data[0] =data;
}   
/////////////////////////////////////////////////////
void out_wdt_reset(void)
{
  	EXTERNAL_WATCHDOG_IO=1;///��оƬ��ҿ��Ź���ι��
  
	if(EXTERNAL_WATCHDOG==1)
		EXTERNAL_WATCHDOG=0;		
	else 
		EXTERNAL_WATCHDOG=1;
}

#pragma CODE_SEG DEFAULT


