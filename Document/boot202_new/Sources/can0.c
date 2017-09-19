#define	_NO_can0_EXTERN
#include "can0.h"

//extern void pflash_chip_erase(void)	;


#pragma CODE_SEG IFsh1_CODE

////////////////////////////////////////////////////////////
void CAN0Init(void)
{  
  CAN0CTL0_INITRQ=1;              	/* request into initial mode */ 
  while(!CAN0CTL1_INITAK);        	/* wait into intial mode */  
  
  CAN0CTL1 = 0x80;               	/*open normal  mode,OSC source*/
  CAN0IDAC_IDAM0 = 1;              	/*two 16 bit acceptance filter*/
  CAN0IDMR0 = 0xfF;               	/*not set filter*/
  CAN0IDMR1 = 0xFf;
  CAN0IDMR2 = 0xff;
  CAN0IDMR3 = 0xff;
  CAN0IDMR4 = 0xff;
  CAN0IDMR5 = 0xff;
  CAN0IDMR6 = 0xff;
  CAN0IDMR7 = 0xff;
  CAN0BTR0 = 0x8F;             		/*8M OSC,SJW is 3 Tq cycle,prescaler value is 8 bit rate is about 125k*/
  CAN0BTR1 = 0x32;             		/*one sample per bit,time segment 1 is 2 Tq and time segment 2 is 3 Tq*/ 
  
  CAN0CTL1_CLKSRC = 0;              /* Select the clock source from crystal */  
  CAN0CTL0_INITRQ = 0;              /* Start device */
  while(CAN0CTL1_INITAK);           /* Wait for device initialization acknowledge */
}

 ///////////////////////////////////////////////
 void CAN0_ReadFrame(void)
{
  byte i;
  dword tmpId = 0,tmpId1=0;
  

  if ((CAN0RFLG&0x01)!=0x01)
     return;
  tmpId  = CAN0RIDR0;
  tmpId  = tmpId<<3;
  tmpId1 = CAN0RIDR1;
  tmpId1 = tmpId1>>5;
  
  tmpId = tmpId + tmpId1;
  ID_value=tmpId;

  length_value = CAN0RDLR & 0x0F;            /* Result length of the message */
  for (i=0; i<length_value; i++) 
     data_value[i] = *(&CAN0RDSR0 + i); /* Return received data */

	//if((ID_value==0x70)||(ID_value==0x0f)||((ID_value==0x00)&&(data_value[0]==0x01)))
  if((ID_value==0x74)||(ID_value==0x0f)||((ID_value==0x04)&&(data_value[0]==0x01)))
  		count_delay=0X00;
  
  CAN0RFLG = 0X81;          /* Reset the receiver interrupt request flag */
}
//////////////////////////////////////////////
void CAN0_SendFrame(dword MessageID_send,byte length_send,byte *data_send)
{
	byte i,cansel;
	
  	CAN0TBSEL = CAN0TFLG; 						/*select one available transmit buffer*/ 
  	cansel = CAN0TBSEL;

	MessageID_send = MessageID_send<<1;
	MessageID_send = MessageID_send<<4;
	CAN0TXIDR0=(uchar)(MessageID_send>>8);					/*ID=0B00000101000 */
	CAN0TXIDR1=(uchar)MessageID_send;  
	CAN0TXDLR=length_send;						   /*length of data*/
	for(i=0;i<length_send;i++)
	{
		*(&CAN0TXDSR0+i)=*data_send;
		data_send++;
	}
	CAN0TXTBPR=1;                     			/*configure data fifo of send priory*/
  	CAN0TFLG=cansel;	
}



/////////////////////////////////////////////////////////

///////////////////////////////////////////////////////
void err_send(void)
{	
	////MessageID_s = 0x80;
	MessageID_s = 0x84;
	length_s	= 2;
	data_s[0]	= 0x27;
	data_s[1]	= err_command; 
}   

/////////////////////////////////////////////////////
void rev_data(void)
{
 uchar i,j=0;
 uchar temp_lenth,temp_fc,gotdata=0;
 uchar data_out[8],checksum_flag=0;
 uchar max_checksum = 0,checksum_value = 0;

	temp_fc = data_value[0]&0x07;			// 数据长度
	if((temp_fc==0x01)||(temp_fc==0x02)||   //s19 data
	   	(temp_fc==0x03)||(temp_fc==0x04)||(temp_fc==0x05))             
	{
		checksum_flag = data_value[0]&0x08;
		temp_lenth=data_value[0]>>4;		// 数据长度	

		j = (temp_lenth-1)*7;					// 计数便宜量
        for(i=1;i<length_value;i++)          //data in buf
		{
			data_flash[j] = data_value[i];
			j++;
			data_count++;
		}    
		if(checksum_flag==0x08)				//last s19 data
		{
			    	// 计算数据的长度是否正确
            if(data_count==((temp_lenth-1)*7+length_value-1))
            {
               	data_address_in = data_flash[0];
				max_checksum	= data_flash[0];
				for(i=1;i<=temp_fc;i++)
				{
					data_address_in = data_address_in<<8;
					data_address_in = data_address_in + data_flash[i];	// 计数地址
					max_checksum    = max_checksum + data_flash[i];			// 计数校验和
				}
				checksum_value = data_flash[data_count-1];						// 获取接收的校验和

				i=temp_fc+1;
				temp_lenth=data_count-temp_fc;
				temp_lenth=temp_lenth-2;
				FlashProgram(data_address_in,&(data_flash[i]),temp_lenth);
				GetFlashDataSum(data_address_in,&max_checksum,temp_lenth);
/*				
				for(i=temp_fc+1;i<data_count-1;i++)
				{
					IFsh1_SetByteFlash(data_address_in,data_flash[i]);
					IFsh1_GetByteFlash(data_address_in,&gotdata);
					max_checksum=max_checksum + gotdata; 
					data_address_in ++;	
				}*/
				max_checksum = max_checksum + data_count;
				max_checksum = 0xff - max_checksum; 
				if( max_checksum == checksum_value )
				{
					MessageID_s = 0x84;
 					length_s    = 2;
					data_s[0]   = 0x08;
					data_s[0]  |= temp_fc;
					data_s[1]   = 0x00;
					CAN0_SendFrame(MessageID_s,length_s,data_s); 
				}
				else 
				{
					err_command = err_flash_checksum;
					err_send();	
					CAN0_SendFrame(MessageID_s,length_s,data_s); 
				}	
            }
			else 
			{
				err_command=err_flash_lenth;
				err_send();
				CAN0_SendFrame(MessageID_s,length_s,data_s); 
			}
			data_count  = 0x00;
			for(i=0;i<100;i++)
				data_flash[i]=0x00;
		}
	}
	else  if(temp_fc==0x06)         //终止命令
	{		
		MessageID_s = 0x84;
		length_s	= 2;
		data_s[0]	= 0X06;
		data_s[1]	= 0X00;
		reset_stop();
		_EntryPoint();
	} 
	else if(temp_fc==0x00)        	//重复发送擦除首地址
	{
			MessageID_s = 0x84;
			length_s    = 2;
			data_s[0]   = 0x10;				
			data_s[1]   = 0x00;
			CAN0_SendFrame(MessageID_s,length_s,data_s); 
	}
}


///////////////////////////////////////////////////
void ack_send(void)
{
	////MessageID_s = 0x80;
	
	MessageID_s = 0x84;
	length_s	= 4;
	data_s[0]	= data_s_buf;
	data_s[1]	= 0x00;
	data_s[2]	= 0x01;
	data_s[3]	= status_commmd;
}



void chip_erase(void)			  //chip erase
{
	dword address_min,address_max,address_in;
	//uchar err;
	uchar add1,i,j;
  	if((clr_address>0x7BFF)&&(clr_address<0x7fFF))                    //配置擦除
  	{
	  clr_address=0x7C00;
	  err=EraseSectorFlash(clr_address);  
	  err=EraseSectorFlash(0x10800);			
	}
	else 
	{	
		for(address_in=0x4000;address_in<0x8000;address_in=(address_in+0x400))
	  		err =EraseSectorFlash(address_in);   //0x4000-0x7f00
	  	asm nop;
		
		for(address_in=0xc000;address_in<0xe000;address_in=(address_in+0x400))
	  		err =EraseSectorFlash(address_in);   //0xc000-0xdfff
	  	asm nop;


		/*address_in  = 0xc08000;
		//address_max = 0xc0bff8;
		err=EraseSectorFlash(address_in);
		asm nop;

		address_in  = 0xc08400;
		err=EraseSectorFlash(address_in);
		asm nop;
		address_in  = 0xc08800;
		err=EraseSectorFlash(address_in);
		asm nop;

		address_in  = 0xc08c00;
		err=EraseSectorFlash(address_in);
		asm nop;
		*/
		for(j=0;j<3;j++)//48k
		{
			for(i=0;i<16;i++)
			{
				err=EraseSectorFlash(0xfc8000+j*0x10000+i*0x400);//FCXXXX,FDXXX,FEXXX
				if(err==1)
					break;
			}
			if(err==1)
				break;
			//out_wdt_reset();
		}
		
/*
    	for(add1=0;add1<63;add1++)		
    	//0xf08000-0xfcbfff
		{
			address_min=address_in;
			for(address_min;address_min<address_max;address_min=(address_min+0x400))
		  	err=EraseSectorFlash(address_min);
			address_in  = address_in  + 0x10000;
			address_max  = address_max + 0x10000;	
		}//0xfe8000-0xfebfff
		*/
		asm nop;
		
		
		/*address_in  = 0xC08000;
		address_max = 0xc0c000;

    	for(add1=0;add1<63;add1++)		
    	//0xf08000-0xfcbfff
		{
			address_min=address_in;
			for(address_min;address_min<address_max;address_min=(address_min+0x400))
		  	err=EraseSectorFlash(address_min);
			address_in  = address_in  + 0x10000;
			address_max  = address_max + 0x10000;	
		}//0xfe8000-0xfebfff
		asm nop;
		*/
	//////	for(address_min=0xfe8000;address_min<0xfec000;address_min=(address_min+0x400))
	//////	  err=EraseSectorFlash(address_min); 

	}
}
/////////////////////////////////////////////////////
void out_wdt_reset(void)

{
	DDRE_DDRE4 = 1;
   if(PORTE_PE4 == 1)
  		PORTE_PE4 = 0;
   else PORTE_PE4 = 1;
   
}
////////
void reset_stop(void)
{
  uchar i;
  uint delay_c;
  
	CAN0_SendFrame(MessageID_s,length_s,data_s); 

	flag=0;
	clr_address=0X00;
	
	data_count = 0x00;
	for(i=0;i<100;i++)
		data_flash[i] = 0x00;

		
	ID_value=0X04;
	for(i=0;i<7;i++) 
	{
		data_value[i]=0x00;
	}
//	EraseSectorFlash(flash_flag);			// 清除flash_flag
//	IFsh1_SetByteFlash(0xF08000,0Xff);
//	IFsh1_SetByteFlash(0xF08000,0Xff);
//	for(delay_c=0;delay_c<0x3ff;delay_c++)
//		asm nop;
}





