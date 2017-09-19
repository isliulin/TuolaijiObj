
#include "IS31FL3733_I2C.h" 

/*******************************************
* IS31FL控制引脚初始化
*******************************************/
void IS31FL_IO_MACROS(void)
{ 
	IS31FL_SCL_MUX();IS31FL_SCL_PDDR_OUT ;//IS31FL_SCL_IO = 1;
	IS31FL_SCL_L;//IS31FL_SCL_L;
	
        IS31FL_SDA_MUX();IS31FL_SDA_PDDR_OUT ; 		
        IS31FL_SDA_L;//IS31FL_SDA_L;
        
        
        IS31FL_SDB_MUX(); 
        IS31FL_SDB_PDDR(); 
        IS31FL_SDB_H;     
        IS31FL_INTB_MUX();   		
        IS31FL_INTB_PDDR ;  		
        
        
}
/*******************************************
function operation : IIC step control,to produce software SCL
*******************************************/
void IS31FL_SCL_delay(void)
{
	uchar i;	
	for(i=0;i<1;i++)
		__asm("nop");	
}
/*********************************************************************
function operation : IIC start condition
function called : IS31FL_SCL_delay(void)
return: void 
*********************************************************************/
void IS31FL_IICstart(void)
{
	IS31FL_SDA_PDDR_OUT;
	__asm("nop");
	IS31FL_SDA_H;
	IS31FL_SCL_delay();
	IS31FL_SCL_H;
	IS31FL_SCL_delay();
	IS31FL_SDA_L;
	IS31FL_SCL_delay();
	IS31FL_SCL_L;
	IS31FL_SCL_delay();
}
/*********************************************************************
function operation : IIC stop condition
function called :  IS31FL_SCL_delay(void)
return : void
*********************************************************************/
void IS31FL_IICstop(void)
{
	IS31FL_SCL_L;
	IS31FL_SCL_delay();
	IS31FL_SDA_L;
	IS31FL_SCL_delay();
	IS31FL_SCL_H;
	IS31FL_SCL_delay();
	IS31FL_SDA_H;
	IS31FL_SCL_delay();
}
/*********************************************************************
function operation : IIC wait ACK
function called : void IS31FL_SCL_delay(void),void IS31FL_IICstop(void)
return : werther receive ack signal,IS31FL_ERROR or IS31FL_OK
*********************************************************************/
byte IS31FL_IICwaitack(void)
{
	unsigned char cErrTime = 255;

	IS31FL_SDA_PDDR_IN;//define as input
	NOP();
	IS31FL_SCL_H;
	IS31FL_SCL_delay();
	while(IS31FL_SDA_READ)
	{
		cErrTime--;
		if (cErrTime==0)
		{
			IS31FL_IICstop();
			IS31FL_SDA_PDDR_OUT;
			return IS31FL_ERROR;
		}		
	}
	IS31FL_SCL_L;
	IS31FL_SCL_delay();
	IS31FL_SDA_PDDR_OUT;	
	return IS31FL_OK;	
}
/*********************************************************************
function operation : IIC send ACK
function called : void IS31FL_SCL_delay(void)
return : void 
*********************************************************************/
void IS31FL_IICsendack(void)
{
	IS31FL_SDA_L;
	IS31FL_SCL_delay();
	IS31FL_SCL_H;
	IS31FL_SCL_delay();
	IS31FL_SCL_L;
	IS31FL_SCL_delay();
}
/*********************************************************************
function operation : IIC send Not ACK
function called : void IS31FL_SCL_delay(void)
return : void
*********************************************************************/
void IS31FL_IICsendnotack(void)
{
	IS31FL_SDA_H;
	IS31FL_SCL_delay();
	IS31FL_SCL_H;
	IS31FL_SCL_delay();
	IS31FL_SCL_L;
	IS31FL_SCL_delay();
}
/*********************************************************************
function operation : send a byte over IIC bus
function called : void IS31FL_SCL_delay(void)
function input parameters : csendbyte,the data needed to send on IIC
return : void
*********************************************************************/
void IS31FL_IICsendbyte(unsigned char csendbyte)
{
	unsigned char i = 8;
	while (i--)
	{
		IS31FL_SCL_L;
		if(csendbyte&0x80)		
			IS31FL_SDA_H;
		else
			IS31FL_SDA_L;
		IS31FL_SCL_delay();
		IS31FL_SCL_H;
		IS31FL_SCL_delay();
		csendbyte<<=1;
	}
	IS31FL_SCL_L;
	IS31FL_SCL_delay();
}
/*********************************************************************
function operation: receive a byte over IIC bus
function called : void IS31FL_SCL_delay(void)
input parameter:	void
return : cr_byte,a byte data receive from iic bus
*********************************************************************/
unsigned char IS31FL_IICreceivebyte(void)
{
	unsigned char i = 8;
	unsigned char cr_byte = 0;

	IS31FL_SDA_PDDR_IN;
	NOP();
	while (i--)
	{
		cr_byte <<= 1;
		IS31FL_SCL_delay();
		IS31FL_SCL_L;
		IS31FL_SCL_delay();
		IS31FL_SCL_H;
		if(IS31FL_SDA_READ)
		{cr_byte++;}
	}
	IS31FL_SCL_delay();
	IS31FL_SCL_L;
	IS31FL_SCL_delay();
	IS31FL_SDA_PDDR_OUT;
	return cr_byte;
}



void  IS_IIC_WriteByte(unsigned char internal_addr,unsigned char dat)
{
        IS31FL_IICstart();
	IS31FL_IICsendbyte(IS31FL_ADDR);
	if(IS31FL_ERROR == IS31FL_IICwaitack())
	{
		IS31FL_IICstop();		
		return;
	}
	IS31FL_IICsendbyte(internal_addr);
	if(IS31FL_ERROR == IS31FL_IICwaitack())
	{
		IS31FL_IICstop();		
		return;
	}		
        IS31FL_IICsendbyte(dat);
        if(IS31FL_ERROR == IS31FL_IICwaitack())
        {	
                IS31FL_IICstop();			
                return;
        }	
	IS31FL_IICstop();	
}

void  IS_IIC_ReadByte(unsigned char internal_addr,unsigned char *dat)
{	

	IS31FL_IICstart();
	//send IS31FL_r to start reading operation
	IS31FL_IICsendbyte(IS31FL_ADDR);
	if(IS31FL_ERROR == IS31FL_IICwaitack())
	{
		IS31FL_IICstop();
		return;
	}
	//send internal address
	IS31FL_IICsendbyte(internal_addr);	
	if(IS31FL_ERROR == IS31FL_IICwaitack())
	{
		IS31FL_IICstop();
		return;
	}	
	//restart
	IS31FL_IICstart();
	IS31FL_IICsendbyte(IS31FL_ADDR+0x01);
	if(IS31FL_ERROR == IS31FL_IICwaitack())
	{
		IS31FL_IICstop();
		return;
	}
	
        *dat = IS31FL_IICreceivebyte();	
        IS31FL_IICsendnotack();	
	IS31FL_IICstop();	
}




/*******************************************************************
funtion operation:write a serial data to iic bus
fution call:void IS31FL_IICsendbyte(void);byte IS31FL_IICwaitack(void);void IS31FL_IICstop(void);
input parameter:internal_addr,the first addr of the serial data;*sp;the point data will be sent;data_len;
					the length of the data will be sent 
return:void				
*******************************************************************/
void	IS31FL_register_write(unsigned char internal_addr,byte * sp,unsigned char data_len)
{		
	IS31FL_IICstart();
	
	IS31FL_IICsendbyte(IS31FL_ADDR);
	if(IS31FL_ERROR == IS31FL_IICwaitack())
	{
		IS31FL_IICstop();		
		return;
	}
	IS31FL_IICsendbyte(internal_addr);
	if(IS31FL_ERROR == IS31FL_IICwaitack())
	{
		IS31FL_IICstop();		
		return;
	}		
	for(;data_len>0;data_len--)
	{
		IS31FL_IICsendbyte(*sp);
		if(IS31FL_ERROR == IS31FL_IICwaitack())
		{	
			IS31FL_IICstop();			
			return;
		}
		sp++;
	}	
	IS31FL_IICstop();	
}

void IS31FL_register_read(unsigned char internal_addr,byte *dp,unsigned char data_len)
{	
	IS31FL_IICstart();
	
	//send IS31FL_r to start reading operation
	IS31FL_IICsendbyte(IS31FL_ADDR);
	if(IS31FL_ERROR == IS31FL_IICwaitack())
	{
		IS31FL_IICstop();
		return;
	}
	
	//send internal address
	IS31FL_IICsendbyte(internal_addr);	
	if(IS31FL_ERROR == IS31FL_IICwaitack())
	{
		IS31FL_IICstop();
		return;
	}	
	
	//restart
	IS31FL_IICstart();
	IS31FL_IICsendbyte(IS31FL_ADDR+0x01);
	if(IS31FL_ERROR == IS31FL_IICwaitack())
	{
		IS31FL_IICstop();
		return;
	}
	
	//read data from register,and save to the array pointed by dp
	for(;data_len>0;data_len--)
	{		
		*dp = IS31FL_IICreceivebyte();		
		if(data_len !=0x01)
		{IS31FL_IICsendack();}
		else
		{IS31FL_IICsendnotack();}
		dp++;
	}	
	IS31FL_IICstop();	
}



const unsigned char PWM_Gama64[64]=
{
  0x00,0x01,0x02,0x03, // 30 40 50 60
  
  0x04,0x05,0x06,0x07,0x08,   //0x04,0X08,0X0F, 0X1A,0X22,0X34,0X44,0X48,0X4F,0X5A
  0x09,0x0b,0x0d,0x0f,0x11,   
  
  0x13,0x16,0x1a,0x1c,0x1d,   //0x13,0X1D,0X28, 0X3C,0X4B,0X5F,0X72,0X7D,0X88,0X8D
  0x1f,0x22,0x25,0x28,0x2e,
  
  0x34,0x38,0x3c,0x40,0x44,
  0x48,0x4b,0x4f,0x55,0x5a,  //0x34,0X44,0X55,0X69,0X7D,0X94.0XAC,0XB9,0XC6,0XCB,
 
  0x5f,0x64,0x69,0x6d,0x72,  //0X5F,0X72,0X88,0XA0,0XB9,0XCF,0XED,0XF6,0XFE,0XFF
  0x77,0x7d,0x80,0x88,0x8d,
 
  0x94,0x9a,0xa0,0xa7,0xac,  
  0xb0,0xb9,0xbf,0xc6,0xcb,
 
  0xcf,0xd6,0xe1,0xe9,0xed,
  0xf1,0xf6,0xfa,0xfe,0xff
};

const unsigned char PWM_RGB[512]=
{
  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
  0x08,0x09,0x0b,0x0d,0x0f,0x11,0x13,0x16,
  0x1a,0x1c,0x1d,0x1f,0x22,0x25,0x28,0x2e,
  0x34,0x38,0x3c,0x40,0x44,0x48,0x4b,0x4f,
  0x55,0x5a,0x5f,0x64,0x69,0x6d,0x72,0x77,
  0x7d,0x80,0x88,0x8d,0x94,0x9a,0xa0,0xa7,
  0xac,0xb0,0xb9,0xbf,0xc6,0xcb,0xcf,0xd6,
  0xe1,0xe9,0xed,0xf1,0xf6,0xfa,0xfe,0xff,

  0xff,0xfe,0xfa,0xf6,0xf1,0xed,0xe9,0xe1,
  0xd6,0xcf,0xcb,0xc6,0xbf,0xb9,0xb0,0xac,
  0xa7,0xa0,0x9a,0x94,0x8d,0x88,0x80,0x7d,
  0x77,0x72,0x6d,0x69,0x64,0x5f,0x5a,0x55,
  0x4f,0x4b,0x48,0x44,0x40,0x3c,0x38,0x34,
  0x2e,0x28,0x25,0x22,0x1f,0x1d,0x1c,0x1a,
  0x16,0x13,0x11,0x0f,0x0d,0x0b,0x09,0x08,
  0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00,
  
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,

  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
  0x08,0x09,0x0b,0x0d,0x0f,0x11,0x13,0x16,
  0x1a,0x1c,0x1d,0x1f,0x22,0x25,0x28,0x2e,
  0x34,0x38,0x3c,0x40,0x44,0x48,0x4b,0x4f,
  0x55,0x5a,0x5f,0x64,0x69,0x6d,0x72,0x77,
  0x7d,0x80,0x88,0x8d,0x94,0x9a,0xa0,0xa7,
  0xac,0xb0,0xb9,0xbf,0xc6,0xcb,0xcf,0xd6,
  0xe1,0xe9,0xed,0xf1,0xf6,0xfa,0xfe,0xff,

  0xff,0xfe,0xfa,0xf6,0xf1,0xed,0xe9,0xe1,
  0xd6,0xcf,0xcb,0xc6,0xbf,0xb9,0xb0,0xac,
  0xa7,0xa0,0x9a,0x94,0x8d,0x88,0x80,0x7d,
  0x77,0x72,0x6d,0x69,0x64,0x5f,0x5a,0x55,
  0x4f,0x4b,0x48,0x44,0x40,0x3c,0x38,0x34,
  0x2e,0x28,0x25,0x22,0x1f,0x1d,0x1c,0x1a,
  0x16,0x13,0x11,0x0f,0x0d,0x0b,0x09,0x08,
  0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00,
  
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,

  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
  0x08,0x09,0x0b,0x0d,0x0f,0x11,0x13,0x16,
  0x1a,0x1c,0x1d,0x1f,0x22,0x25,0x28,0x2e,
  0x34,0x38,0x3c,0x40,0x44,0x48,0x4b,0x4f,
  0x55,0x5a,0x5f,0x64,0x69,0x6d,0x72,0x77,
  0x7d,0x80,0x88,0x8d,0x94,0x9a,0xa0,0xa7,
  0xac,0xb0,0xb9,0xbf,0xc6,0xcb,0xcf,0xd6,
  0xe1,0xe9,0xed,0xf1,0xf6,0xfa,0xfe,0xff,

  0xff,0xfe,0xfa,0xf6,0xf1,0xed,0xe9,0xe1,
  0xd6,0xcf,0xcb,0xc6,0xbf,0xb9,0xb0,0xac,
  0xa7,0xa0,0x9a,0x94,0x8d,0x88,0x80,0x7d,
  0x77,0x72,0x6d,0x69,0x64,0x5f,0x5a,0x55,
  0x4f,0x4b,0x48,0x44,0x40,0x3c,0x38,0x34,
  0x2e,0x28,0x25,0x22,0x1f,0x1d,0x1c,0x1a,
  0x16,0x13,0x11,0x0f,0x0d,0x0b,0x09,0x08,
  0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00
};

void Init_3738(void)
{
  uint8_t i;
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write function register
  IS_IIC_WriteByte(0x00,0x00);//software shutdown
  
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x00);//write LED ON OFF
  for(i=0;i<0x18;i++)
  {
    IS_IIC_WriteByte(i,0xff);//open all LED
  }
  for(i=0x04;i<0x06;i++)
  {
    IS_IIC_WriteByte(i,0x00);//turn off two R LED
  }
  for(i=0x10;i<0x12;i++)
  {
    IS_IIC_WriteByte(i,0x00);//turn off two R LED
  }
  for(i=0x08;i<0x0A;i++)
  {
    IS_IIC_WriteByte(i,0x55);//turn off two B LED
  }
  for(i=0x14;i<0x16;i++)
  {
    IS_IIC_WriteByte(i,0x55);//turn off two B LED
  }
  
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x01);//write PWM 
  for(i=0;i<192;i++)
  IS_IIC_WriteByte(i,0x00);

  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write function register
  IS_IIC_WriteByte(0x00,0x01);//normal operation
  IS_IIC_WriteByte(0x01,0xFF);//global current
}

void IS31FL3738_mode1(void)//white LED
{
  uint8_t i;
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write page 3
  IS_IIC_WriteByte(0x01,0x00);//set global current  
  
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x02);//write  page 2
  for (i=0;i<=0xBF;i++)
  {
     IS_IIC_WriteByte(i,0x00);// picture mode  
  } 
  
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x01);//write PWM
  for (i=0;i<=0xBF;i++)
  {
     IS_IIC_WriteByte(i,0xFF);// PWM set 0xff  
  } 

  for (i=0x40;i<=0x60;i++)
  {
     IS_IIC_WriteByte(i,0xC0);//B LED PWM   
  } 
  for (i=0xA0;i<0xC0;i++)
  {
     IS_IIC_WriteByte(i,0xC0);//B LED PWM   
  } 
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write page 3
  IS_IIC_WriteByte(0x00,0x01);//picturee mode and normal operation
  IS_IIC_WriteByte(0x01,0xFF);//set global current 
  
 // while(mode_num == 1);  
}

void IS31FL3738_mode2(void)//rainbow
{
  uint8_t com1,i;
  uint8_t x=128;
  uint8_t y=64;
  uint8_t z=8;
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write page 3
  IS_IIC_WriteByte(0x01,0xFF);//set global current 

  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x01);//write page 1
  for(i=0;i<192;i++)
  {
    IS_IIC_WriteByte(i,0x00);//set PWM
  }
  
 // while(mode_num == 2)
  {
    for(com1=0;com1<193;com1++)
    { 
      //RGB1      
      IS_IIC_WriteByte(0x30,PWM_RGB[com1]);//R1
      IS_IIC_WriteByte(0x31,PWM_RGB[com1]);
      IS_IIC_WriteByte(0x20,PWM_RGB[com1]);
      IS_IIC_WriteByte(0x21,PWM_RGB[com1]);
      IS_IIC_WriteByte(0x10,PWM_RGB[com1+x]);//G1
      IS_IIC_WriteByte(0x11,PWM_RGB[com1+x]);
      IS_IIC_WriteByte(0x00,PWM_RGB[com1+x]);
      IS_IIC_WriteByte(0x01,PWM_RGB[com1+x]);
      
      IS_IIC_WriteByte(0x50,PWM_RGB[com1+x+y]);//B1
      IS_IIC_WriteByte(0x51,PWM_RGB[com1+x+y]);
      IS_IIC_WriteByte(0x40,PWM_RGB[com1+x+y]);
      IS_IIC_WriteByte(0x41,PWM_RGB[com1+x+y]);     
      //RGB2      
      IS_IIC_WriteByte(0x32,PWM_RGB[com1+z*1]);//R2
      IS_IIC_WriteByte(0x33,PWM_RGB[com1+z*1]);
      IS_IIC_WriteByte(0x22,PWM_RGB[com1+z*1]);
      IS_IIC_WriteByte(0x23,PWM_RGB[com1+z*1]);
      IS_IIC_WriteByte(0x12,PWM_RGB[com1+x+z*1]);//G2
      IS_IIC_WriteByte(0x13,PWM_RGB[com1+x+z*1]);
      IS_IIC_WriteByte(0x02,PWM_RGB[com1+x+z*1]);
      IS_IIC_WriteByte(0x03,PWM_RGB[com1+x+z*1]);
      IS_IIC_WriteByte(0x52,PWM_RGB[com1+y+z*1]);//B2
      IS_IIC_WriteByte(0x53,PWM_RGB[com1+y+z*1]);
      IS_IIC_WriteByte(0x42,PWM_RGB[com1+y+z*1]);
      IS_IIC_WriteByte(0x43,PWM_RGB[com1+y+z*1]);          

      //RGB3      
      IS_IIC_WriteByte(0x34,PWM_RGB[com1+z*2]);//R3
      IS_IIC_WriteByte(0x35,PWM_RGB[com1+z*2]);
      IS_IIC_WriteByte(0x24,PWM_RGB[com1+z*2]);
      IS_IIC_WriteByte(0x25,PWM_RGB[com1+z*2]);
      IS_IIC_WriteByte(0x14,PWM_RGB[com1+x+z*2]);//G3
      IS_IIC_WriteByte(0x15,PWM_RGB[com1+x+z*2]);
      IS_IIC_WriteByte(0x04,PWM_RGB[com1+x+z*2]);
      IS_IIC_WriteByte(0x05,PWM_RGB[com1+x+z*2]);
      IS_IIC_WriteByte(0x54,PWM_RGB[com1+y+z*2]);//B3
      IS_IIC_WriteByte(0x55,PWM_RGB[com1+y+z*2]);
      IS_IIC_WriteByte(0x44,PWM_RGB[com1+y+z*2]);
      IS_IIC_WriteByte(0x45,PWM_RGB[com1+y+z*2]); 
      //RGB4      
      IS_IIC_WriteByte(0x36,PWM_RGB[com1+z*3]);
      IS_IIC_WriteByte(0x37,PWM_RGB[com1+z*3]);
      IS_IIC_WriteByte(0x26,PWM_RGB[com1+z*3]);
      IS_IIC_WriteByte(0x27,PWM_RGB[com1+z*3]);
      IS_IIC_WriteByte(0x16,PWM_RGB[com1+x+z*3]);
      IS_IIC_WriteByte(0x17,PWM_RGB[com1+x+z*3]);
      IS_IIC_WriteByte(0x06,PWM_RGB[com1+x+z*3]);
      IS_IIC_WriteByte(0x07,PWM_RGB[com1+x+z*3]);
      IS_IIC_WriteByte(0x56,PWM_RGB[com1+y+z*3]);
      IS_IIC_WriteByte(0x57,PWM_RGB[com1+y+z*3]);
      IS_IIC_WriteByte(0x46,PWM_RGB[com1+y+z*3]);
      IS_IIC_WriteByte(0x47,PWM_RGB[com1+y+z*3]); 
      //RGB5      
      IS_IIC_WriteByte(0x38,PWM_RGB[com1+z*4]);
      IS_IIC_WriteByte(0x39,PWM_RGB[com1+z*4]);
      IS_IIC_WriteByte(0x28,PWM_RGB[com1+z*4]);
      IS_IIC_WriteByte(0x29,PWM_RGB[com1+z*4]);
      IS_IIC_WriteByte(0x18,PWM_RGB[com1+x+z*4]);
      IS_IIC_WriteByte(0x19,PWM_RGB[com1+x+z*4]);
      IS_IIC_WriteByte(0x08,PWM_RGB[com1+x+z*4]);
      IS_IIC_WriteByte(0x09,PWM_RGB[com1+x+z*4]);
      IS_IIC_WriteByte(0x58,PWM_RGB[com1+y+z*4]);
      IS_IIC_WriteByte(0x59,PWM_RGB[com1+y+z*4]);
      IS_IIC_WriteByte(0x48,PWM_RGB[com1+y+z*4]);
      IS_IIC_WriteByte(0x49,PWM_RGB[com1+y+z*4]); 
      //RGB6    
      IS_IIC_WriteByte(0x3A,PWM_RGB[com1+z*5]);
      IS_IIC_WriteByte(0x3B,PWM_RGB[com1+z*5]);
      IS_IIC_WriteByte(0x2A,PWM_RGB[com1+z*5]);
      IS_IIC_WriteByte(0x2B,PWM_RGB[com1+z*5]);
      IS_IIC_WriteByte(0x1A,PWM_RGB[com1+x+z*5]);
      IS_IIC_WriteByte(0x1B,PWM_RGB[com1+x+z*5]);
      IS_IIC_WriteByte(0x0A,PWM_RGB[com1+x+z*5]);
      IS_IIC_WriteByte(0x0B,PWM_RGB[com1+x+z*5]);
      IS_IIC_WriteByte(0x5A,PWM_RGB[com1+y+z*5]);
      IS_IIC_WriteByte(0x5B,PWM_RGB[com1+y+z*5]);
      IS_IIC_WriteByte(0x4A,PWM_RGB[com1+y+z*5]);
      IS_IIC_WriteByte(0x4B,PWM_RGB[com1+y+z*5]);   
      //RGB7      
      IS_IIC_WriteByte(0x3C,PWM_RGB[com1+z*6]);
      IS_IIC_WriteByte(0x3D,PWM_RGB[com1+z*6]);
      IS_IIC_WriteByte(0x2C,PWM_RGB[com1+z*6]);
      IS_IIC_WriteByte(0x2D,PWM_RGB[com1+z*6]);
      IS_IIC_WriteByte(0x1C,PWM_RGB[com1+x+z*6]);
      IS_IIC_WriteByte(0x1D,PWM_RGB[com1+x+z*6]);
      IS_IIC_WriteByte(0x0C,PWM_RGB[com1+x+z*6]);
      IS_IIC_WriteByte(0x0D,PWM_RGB[com1+x+z*6]);
      IS_IIC_WriteByte(0x5C,PWM_RGB[com1+y+z*6]);
      IS_IIC_WriteByte(0x5D,PWM_RGB[com1+y+z*6]);
      IS_IIC_WriteByte(0x4C,PWM_RGB[com1+y+z*6]);
      IS_IIC_WriteByte(0x4D,PWM_RGB[com1+y+z*6]);     
      //RGB8      
      IS_IIC_WriteByte(0x3E,PWM_RGB[com1+z*7]);
      IS_IIC_WriteByte(0x3F,PWM_RGB[com1+z*7]);
      IS_IIC_WriteByte(0x2E,PWM_RGB[com1+z*7]);
      IS_IIC_WriteByte(0x2F,PWM_RGB[com1+z*7]);
      IS_IIC_WriteByte(0x1E,PWM_RGB[com1+x+z*7]);
      IS_IIC_WriteByte(0x1F,PWM_RGB[com1+x+z*7]);
      IS_IIC_WriteByte(0x0E,PWM_RGB[com1+x+z*7]);
      IS_IIC_WriteByte(0x0F,PWM_RGB[com1+x+z*7]);
      IS_IIC_WriteByte(0x5E,PWM_RGB[com1+y+z*7]);
      IS_IIC_WriteByte(0x5F,PWM_RGB[com1+y+z*7]);
      IS_IIC_WriteByte(0x4E,PWM_RGB[com1+y+z*7]);
      IS_IIC_WriteByte(0x4F,PWM_RGB[com1+y+z*7]); 
      //RGB9      
      IS_IIC_WriteByte(0x90,PWM_RGB[com1+z*8]);
      IS_IIC_WriteByte(0x91,PWM_RGB[com1+z*8]);
      IS_IIC_WriteByte(0x80,PWM_RGB[com1+z*8]);
      IS_IIC_WriteByte(0x81,PWM_RGB[com1+z*8]);
      IS_IIC_WriteByte(0x70,PWM_RGB[com1+x+z*8]);
      IS_IIC_WriteByte(0x71,PWM_RGB[com1+x+z*8]);
      IS_IIC_WriteByte(0x60,PWM_RGB[com1+x+z*8]);
      IS_IIC_WriteByte(0x61,PWM_RGB[com1+x+z*8]);
      IS_IIC_WriteByte(0xB0,PWM_RGB[com1+y+z*8]);
      IS_IIC_WriteByte(0xB1,PWM_RGB[com1+y+z*8]);
      IS_IIC_WriteByte(0xA0,PWM_RGB[com1+y+z*8]);
      IS_IIC_WriteByte(0xA1,PWM_RGB[com1+y+z*8]);
      //RGB10     
      IS_IIC_WriteByte(0x92,PWM_RGB[com1+z*9]);
      IS_IIC_WriteByte(0x93,PWM_RGB[com1+z*9]);
      IS_IIC_WriteByte(0x82,PWM_RGB[com1+z*9]);
      IS_IIC_WriteByte(0x83,PWM_RGB[com1+z*9]);
      IS_IIC_WriteByte(0x72,PWM_RGB[com1+x+z*9]);
      IS_IIC_WriteByte(0x73,PWM_RGB[com1+x+z*9]);
      IS_IIC_WriteByte(0x62,PWM_RGB[com1+x+z*9]);
      IS_IIC_WriteByte(0x63,PWM_RGB[com1+x+z*9]);
      IS_IIC_WriteByte(0xB2,PWM_RGB[com1+y+z*9]);
      IS_IIC_WriteByte(0xB3,PWM_RGB[com1+y+z*9]);
      IS_IIC_WriteByte(0xA2,PWM_RGB[com1+y+z*9]);
      IS_IIC_WriteByte(0xA3,PWM_RGB[com1+y+z*9]);   
      //RGB11     
      IS_IIC_WriteByte(0x94,PWM_RGB[com1+z*10]);
      IS_IIC_WriteByte(0x95,PWM_RGB[com1+z*10]);
      IS_IIC_WriteByte(0x84,PWM_RGB[com1+z*10]);
      IS_IIC_WriteByte(0x85,PWM_RGB[com1+z*10]);
      IS_IIC_WriteByte(0x74,PWM_RGB[com1+x+z*10]);
      IS_IIC_WriteByte(0x75,PWM_RGB[com1+x+z*10]);
      IS_IIC_WriteByte(0x64,PWM_RGB[com1+x+z*10]);
      IS_IIC_WriteByte(0x65,PWM_RGB[com1+x+z*10]);
      IS_IIC_WriteByte(0xB4,PWM_RGB[com1+y+z*10]);
      IS_IIC_WriteByte(0xB5,PWM_RGB[com1+y+z*10]);
      IS_IIC_WriteByte(0xA4,PWM_RGB[com1+y+z*10]);
      IS_IIC_WriteByte(0xA5,PWM_RGB[com1+y+z*10]);  
      //RGB12     
      IS_IIC_WriteByte(0x96,PWM_RGB[com1+z*11]);
      IS_IIC_WriteByte(0x97,PWM_RGB[com1+z*11]);
      IS_IIC_WriteByte(0x86,PWM_RGB[com1+z*11]);
      IS_IIC_WriteByte(0x87,PWM_RGB[com1+z*11]);
      IS_IIC_WriteByte(0x76,PWM_RGB[com1+x+z*11]);
      IS_IIC_WriteByte(0x77,PWM_RGB[com1+x+z*11]);
      IS_IIC_WriteByte(0x66,PWM_RGB[com1+x+z*11]);
      IS_IIC_WriteByte(0x67,PWM_RGB[com1+x+z*11]);
      IS_IIC_WriteByte(0xB6,PWM_RGB[com1+y+z*11]);
      IS_IIC_WriteByte(0xB7,PWM_RGB[com1+y+z*11]);
      IS_IIC_WriteByte(0xA6,PWM_RGB[com1+y+z*11]);
      IS_IIC_WriteByte(0xA7,PWM_RGB[com1+y+z*11]);
      //RGB13     
      IS_IIC_WriteByte(0x98,PWM_RGB[com1+z*12]);
      IS_IIC_WriteByte(0x99,PWM_RGB[com1+z*12]);
      IS_IIC_WriteByte(0x88,PWM_RGB[com1+z*12]);
      IS_IIC_WriteByte(0x89,PWM_RGB[com1+z*12]);
      IS_IIC_WriteByte(0x78,PWM_RGB[com1+x+z*12]);
      IS_IIC_WriteByte(0x79,PWM_RGB[com1+x+z*12]);
      IS_IIC_WriteByte(0x68,PWM_RGB[com1+x+z*12]);
      IS_IIC_WriteByte(0x69,PWM_RGB[com1+x+z*12]);
      IS_IIC_WriteByte(0xB8,PWM_RGB[com1+y+z*12]);
      IS_IIC_WriteByte(0xB9,PWM_RGB[com1+y+z*12]);
      IS_IIC_WriteByte(0xA8,PWM_RGB[com1+y+z*12]);
      IS_IIC_WriteByte(0xA9,PWM_RGB[com1+y+z*12]);  
      //RGB14     
      IS_IIC_WriteByte(0x9A,PWM_RGB[com1+z*13]);
      IS_IIC_WriteByte(0x9B,PWM_RGB[com1+z*13]);
      IS_IIC_WriteByte(0x8A,PWM_RGB[com1+z*13]);
      IS_IIC_WriteByte(0x8B,PWM_RGB[com1+z*13]);
      IS_IIC_WriteByte(0x7A,PWM_RGB[com1+x+z*13]);
      IS_IIC_WriteByte(0x7B,PWM_RGB[com1+x+z*13]);
      IS_IIC_WriteByte(0x6A,PWM_RGB[com1+x+z*13]);
      IS_IIC_WriteByte(0x6B,PWM_RGB[com1+x+z*13]);
      IS_IIC_WriteByte(0xBA,PWM_RGB[com1+y+z*13]);
      IS_IIC_WriteByte(0xBB,PWM_RGB[com1+y+z*13]);
      IS_IIC_WriteByte(0xAA,PWM_RGB[com1+y+z*13]);
      IS_IIC_WriteByte(0xAB,PWM_RGB[com1+y+z*13]);      
      //RGB15     
      IS_IIC_WriteByte(0x9C,PWM_RGB[com1+z*14]);
      IS_IIC_WriteByte(0x9D,PWM_RGB[com1+z*14]);
      IS_IIC_WriteByte(0x8C,PWM_RGB[com1+z*14]);
      IS_IIC_WriteByte(0x8D,PWM_RGB[com1+z*14]);
      IS_IIC_WriteByte(0x7C,PWM_RGB[com1+x+z*14]);
      IS_IIC_WriteByte(0x7D,PWM_RGB[com1+x+z*14]);
      IS_IIC_WriteByte(0x6C,PWM_RGB[com1+x+z*14]);
      IS_IIC_WriteByte(0x6D,PWM_RGB[com1+x+z*14]);
      IS_IIC_WriteByte(0xBC,PWM_RGB[com1+y+z*14]);
      IS_IIC_WriteByte(0xBD,PWM_RGB[com1+y+z*14]);
      IS_IIC_WriteByte(0xAC,PWM_RGB[com1+y+z*14]);
      IS_IIC_WriteByte(0xAD,PWM_RGB[com1+y+z*14]);  
      //RGB16     
      IS_IIC_WriteByte(0x9E,PWM_RGB[com1+z*15]);
      IS_IIC_WriteByte(0x9F,PWM_RGB[com1+z*15]);
      IS_IIC_WriteByte(0x8E,PWM_RGB[com1+z*15]);
      IS_IIC_WriteByte(0x8F,PWM_RGB[com1+z*15]);
      IS_IIC_WriteByte(0x7E,PWM_RGB[com1+x+z*15]);
      IS_IIC_WriteByte(0x7F,PWM_RGB[com1+x+z*15]);
      IS_IIC_WriteByte(0x6E,PWM_RGB[com1+x+z*15]);
      IS_IIC_WriteByte(0x6F,PWM_RGB[com1+x+z*15]);
      IS_IIC_WriteByte(0xBE,PWM_RGB[com1+y+z*15]);
      IS_IIC_WriteByte(0xBF,PWM_RGB[com1+y+z*15]);
      IS_IIC_WriteByte(0xAE,PWM_RGB[com1+y+z*15]);
      IS_IIC_WriteByte(0xAF,PWM_RGB[com1+y+z*15]);        
     // if(mode_num != 2) break;
    } 
  }
}

void IS31FL3738_mode3(void)//R LED breath
{
  uint8_t i;
  
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write page 3
  IS_IIC_WriteByte(0x00,0x00);//shutdown mode
  
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x01);//write PWM
  for (i=0;i<=0xBF;i++)
  {
     IS_IIC_WriteByte(i,0x00);// PWM set 0x00 
  }
  
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x02);//write page 2
  for (i=0x00;i<0xC0;i++)
  {
    IS_IIC_WriteByte(i,0x00);//all LED picturn  
  }
  
  for (i=0x20;i<0x40;i++)
  {
    IS_IIC_WriteByte(i,0x01);//R LED breath  
  }
  for (i=0x80;i<0xA0;i++)
  {
    IS_IIC_WriteByte(i,0x01);//R LED breath  
  }

  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write page 3
  IS_IIC_WriteByte(0x01,0xFF);//global current
  IS_IIC_WriteByte(0x02,0x64);//T1 1.68S and T2 0.42S
  IS_IIC_WriteByte(0x03,0x64);//T3 1.68S and T4 0.42S
  IS_IIC_WriteByte(0x04,0x00);//begin T1 and end T2 and endless loop
  IS_IIC_WriteByte(0x05,0x00);//endless loop
  IS_IIC_WriteByte(0x0E,0x00);//up data
  IS_IIC_WriteByte(0x00,0x03);//normal mode and auto breath mode
  //while(mode_num == 3);
}

void IS31FL3738_mode4(void)// G LED breath
{
  uint8_t i;

  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write page 3
  IS_IIC_WriteByte(0x00,0x02);//shutdown mode and auto breath mode
  
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x02);//write page 2
  for (i=0x00;i<0xC0;i++)
  {
    IS_IIC_WriteByte(i,0x00);//all LED picturn  
  }
  for (i=0x00;i<0x20;i++)
  {
    IS_IIC_WriteByte(i,0x01);//G LED breath  
  }
  for (i=0x60;i<0x80;i++)
  {
    IS_IIC_WriteByte(i,0x01);//G LED breath  
  }

  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write page 3
//  IS_IIC_WriteByte(0x01,0xFF);//global current
//  IS_IIC_WriteByte(0x02,0x28);//T1 0.42S and T2 1.68S
//  IS_IIC_WriteByte(0x03,0x28);//T3 0.42S and T4 1.68S
//  IS_IIC_WriteByte(0x04,0x00);//begin T1 and end T2 and endless loop
//  IS_IIC_WriteByte(0x05,0x00);//endless loop
  IS_IIC_WriteByte(0x0E,0x00);//up data
  IS_IIC_WriteByte(0x00,0x03);//normal mode and auto breath mode
  //while(mode_num == 4);
}

void IS31FL3738_mode5(void)//B
{
  uint8_t i;

  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write page 3
  IS_IIC_WriteByte(0x00,0x02);//shutdown mode and auto breath mode
  
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x02);//write page 2
  for (i=0x00;i<0xC0;i++)
  {
    IS_IIC_WriteByte(i,0x00);//all LED picturn  
  }
  for (i=0x40;i<0x60;i++)
  {
    IS_IIC_WriteByte(i,0x01);//B LED breath  
  }
  for (i=0xA0;i<0xC0;i++)
  {
    IS_IIC_WriteByte(i,0x01);//B LED breath  
  }

  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write page 3
//  IS_IIC_WriteByte(0x01,0xFF);//global current
//  IS_IIC_WriteByte(0x02,0x28);//T1 0.42S and T2 1.68S
//  IS_IIC_WriteByte(0x03,0x28);//T3 0.42S and T4 1.68S
//  IS_IIC_WriteByte(0x04,0x00);//begin T1 and end T2 and endless loop
//  IS_IIC_WriteByte(0x05,0x00);//endless loop
  IS_IIC_WriteByte(0x0E,0x00);//up data
  IS_IIC_WriteByte(0x00,0x03);//normal mode and auto breath mode
 // while(mode_num == 5);
}

void IS31FL3738_mode6(void)//RB
{
  uint8_t i;

  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write page 3
  IS_IIC_WriteByte(0x00,0x02);//shutdown mode and auto breath mode
  
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x02);//write page 2
  for (i=0x00;i<0xC0;i++)
  {
    IS_IIC_WriteByte(i,0x00);//all LED picturn  
  }
  for (i=0x20;i<0x60;i++)
  {
    IS_IIC_WriteByte(i,0x01);//RB LED breath  
  }
  for (i=0x80;i<0xC0;i++)
  {
    IS_IIC_WriteByte(i,0x01);//RB LED breath  
  }

  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write page 3
//  IS_IIC_WriteByte(0x01,0xFF);//global current
//  IS_IIC_WriteByte(0x02,0x28);//T1 0.42S and T2 1.68S
//  IS_IIC_WriteByte(0x03,0x28);//T3 0.42S and T4 1.68S
//  IS_IIC_WriteByte(0x04,0x00);//begin T1 and end T2 and endless loop
//  IS_IIC_WriteByte(0x05,0x00);//endless loop
  IS_IIC_WriteByte(0x0E,0x00);//up data
  IS_IIC_WriteByte(0x00,0x03);//normal mode and auto breath mode
  //while(mode_num == 6);
}

void IS31FL3738_mode7(void)
{
  uint8_t i;

  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write page 3
  IS_IIC_WriteByte(0x00,0x02);//shutdown mode and auto breath mode
  
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x02);//write page 2
  for (i=0x00;i<0xC0;i++)
  {
    IS_IIC_WriteByte(i,0x00);//all LED picturn  
  }
  for (i=0x00;i<0x40;i++)
  {
    IS_IIC_WriteByte(i,0x01);//RG LED breath  
  }
  for (i=0x60;i<0xA0;i++)
  {
    IS_IIC_WriteByte(i,0x01);//RG LED breath  
  }

  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write page 3
//  IS_IIC_WriteByte(0x01,0xFF);//global current
//  IS_IIC_WriteByte(0x02,0x28);//T1 0.42S and T2 1.68S
//  IS_IIC_WriteByte(0x03,0x28);//T3 0.42S and T4 1.68S
//  IS_IIC_WriteByte(0x04,0x00);//begin T1 and end T2 and endless loop
//  IS_IIC_WriteByte(0x05,0x00);//endless loop
  IS_IIC_WriteByte(0x0E,0x00);//up data
  IS_IIC_WriteByte(0x00,0x03);//normal mode and auto breath mode
 // while(mode_num == 7);
}

void IS31FL3738_mode8(void)
{
   uint8_t i;

  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write page 3
  IS_IIC_WriteByte(0x00,0x02);//shutdown mode and auto breath mode
  
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x02);//write page 2
  for (i=0x00;i<0xC0;i++)
  {
    IS_IIC_WriteByte(i,0x00);//all LED picturn  
  }
  for (i=0x00;i<0x20;i++)
  {
    IS_IIC_WriteByte(i,0x01);//BG LED breath  
  }
  for (i=0x40;i<0x80;i++)
  {
    IS_IIC_WriteByte(i,0x01);//BG LED breath  
  }
  for (i=0xA0;i<0xC0;i++)
  {
    IS_IIC_WriteByte(i,0x01);//BG LED breath  
  }
  
  IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  IS_IIC_WriteByte(0xFD,0x03);//write page 3
//  IS_IIC_WriteByte(0x01,0xFF);//global current
//  IS_IIC_WriteByte(0x02,0x28);//T1 0.42S and T2 1.68S
//  IS_IIC_WriteByte(0x03,0x28);//T3 0.42S and T4 1.68S
//  IS_IIC_WriteByte(0x04,0x00);//begin T1 and end T2 and endless loop
//  IS_IIC_WriteByte(0x05,0x00);//endless loop
  IS_IIC_WriteByte(0x0E,0x00);//up data
  IS_IIC_WriteByte(0x00,0x03);//normal mode and auto breath mode
  //while(mode_num == 8);
}



void IS31FL3738_SetLedPWM(uint8 sw,uint8 cs, uint8 pwmVal)//固定pwm  sw=0~5
{
  uint8_t addr,line;

  //IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
  //IS_IIC_WriteByte(0xFD,0x03);//write page 3
  //IS_IIC_WriteByte(0x01,0xFF);//set global current 
      IS_IIC_WriteByte(0xFE,0xc5);//unlock FDh
      IS_IIC_WriteByte(0xFD,0x01);//write page 1
     // if(sw==1)  sw = 0;
     // else if(sw==0)  sw = 1;
      addr = sw*2;  //地址
      addr <<=4;
      line = cs*2;      
      IS_IIC_WriteByte(addr+line+0x10,pwmVal);
      IS_IIC_WriteByte(addr+line+0x11,pwmVal);
      IS_IIC_WriteByte(addr+line     ,pwmVal);
      IS_IIC_WriteByte(addr+line+0x01,pwmVal);
}

void IS31fL_SetLeftLeds(uint8 LedVal,uint8 redMax,  uint8 pwmVal)//固定pwm  sw=1~5//左边
{
  uint8_t i;
  uint32 ledStBit,mask;
  if(LedVal>redMax)
  {
    ledStBit = 0x01ul<<LedVal;  //BIT0~BIT11;
    ledStBit -= 1;
    ledStBit&=0xfff;
  }
  else
  {
   ledStBit = 0x01ul<<LedVal;  //BIT12~BIT18;
   ledStBit -= 1;
   ledStBit<<=12;  
   ledStBit&=0xff000;
  }
  for(i=0;i<19;i++)
  {
          mask = 0x01ul<<i;
          if(ledStBit&mask)  //亮
         {
            IS31FL3738_SetLedPWM( i/8, i%8,  pwmVal);
          }
          else
          {
             IS31FL3738_SetLedPWM( i/8, i%8, 0);
          }
  }
  
}


void IS31fL_SetRightLeds(uint8 LedVal,uint8 redMax,  uint8 pwmVal)//固定pwm  sw=1~5//左边
{
  uint8_t i;
  uint32 ledStBit,mask;
  if(LedVal>redMax)
  {
    ledStBit = 0x01ul<<LedVal;  //BIT0~BIT11;
    ledStBit -= 1;
  }
  else
  {
   ledStBit = 0x01ul<<LedVal;  //BIT12~BIT18;
   ledStBit -= 1;
   ledStBit<<=12;  
  }
  for(i=0;i<19;i++)
  {
          mask = 0x01ul<<i;
          if(ledStBit&mask)  //亮
         {
            IS31FL3738_SetLedPWM( i/8+3, i%8,  pwmVal);
          }
          else
          {
             IS31FL3738_SetLedPWM( i/8+3, i%8, 0);
          }
  }
  
}

/*void IS31fL_SetColumnLed(uint16 oldLedSt,uint16 newLedSt,uint8 startLedNumber, const uint8 *PWM_Gama64)//固定pwm  sw=1~5//左边
{
  uint8_t i;
  uint8_t sw,cs;
  uint8 oldStBit,newStBit;
  
  if(oldLedSt==newLedSt) return ;  //不需刷新led

  for(i=0;i<16;i++)
  {
        oldStBit = (oldLedSt>>i)&0x01;
        newStBit = (newLedSt>>i)&0x01;
        if(oldStBit!=newStBit)  //有led状态改变
        {
          sw = (startLedNumber+i)/8;
          cs = (startLedNumber+i)%8;
          if(newStBit) 
          {   
            IS31FL3738_SetLedPWM( sw, cs,  *PWM_Gama64);
          }
          else
          {
             IS31FL3738_SetLedPWM( sw, cs, 0);
          }
        }
  }
  
}*/





