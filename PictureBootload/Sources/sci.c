#include "sci.h"
//#include <MC9S12XET256.h> 
//#include "message.h"
//#include "crc16.h"
//#include "cananalysis.h"
// #include "ioport.h"
//#include "rtc.h"
//#include "misc.h"
U8  SCI0_Status=0;
SCI_QUE sci_que;
SCIDAT sci0_dat;
U16 sci0_index=0;
U16 crccheck=0;
U8  val=0;

U8  SCI1_Status=0;
SCI1DAT sci1_dat;
U16 sci1_index=0;


//U8  tempBuf[100];
U8  temp_index=0;

U8 SCI_data;

/***********************************************
*函数名：SCI0_Init
*参  数：void
*返  回：void
************************************************/
void SCI0_Init(void)
{
#if 1
    SCI0SR2 = 0x00;
    SCI0BDH = 0x00;
    SCI0BDL = 0x0D; //波特率=8M/(16*0x34)=9600          
                    //0x1a;//波特率=8M/(16*0x1a)=19200      
                    // 0x0d;//波特率=8M/(16*0x0d)=38400     
                    // 0x09;//波特率=8M/(16*0x09)=57600     
#else
    SCI0SR2 = 0x00;
    SCI0BDH = 0x00;
    SCI0BDL = 0x0d; //波特率=24M/(16*0x9c)=9600          
                    ////波特率=24M/(16*0x4e)=19200      
                    // //波特率=24M/(16*0x27)=38400     
                    // //波特率=24M/(16*0x1a)=57600   
                    ////波特率=24M/(16*0xd)=115200   
#endif
    SCI0CR1 = 0x00;
    SCI0CR2 = 0x0c; //使能接收、发送
}

/***********************************************
*函数名：SCI0_Send
*参  数：dat,要发送的数据
*返  回：void
************************************************/
void SCI0_Send(U8 dat)
{
    while(!SCI0SR1_TDRE);//等待TDRE置一，发送数据寄存器空
    SCI0DRL = dat;  
    while(SCI0SR1_TDRE);//A new transmission will not result until the TDRE flag has been cleared.
}

/***********************************************
*函数名：SCI0_Sendbytes
*参  数：*pdat:要发送数据的数组或指针，len：发送字节数
*返  回：void
************************************************/
void SCI0_Sendbytes(U8 *pdat,U16 len)
{
    U16 num=len;

    while(num--)
    {
        SCI0_Send(*pdat);
        pdat++;
    }

}
U8 SCI0_Getbyte(void)
{
	U8 temp,temp1;
	temp1 = SCI0SR1;
	if(!(SCI0SR1&0x20))
	{
		
		//SCI0DRL;
		return 0;
	}//no data to be received
	//while(SCI0SR1_RDRF==0)
	//{
		//out_wdt_reset();
		//return 0xff;
	//}
	//temp = SCI0SR1;
	temp1 = SCI0SR1;
	temp = SCI0DRL;//清接收标志
	
	return temp;
    
}

U8 SCI1_Getbyte(void)
{
	U8 temp,temp1;
	temp1 = SCI1SR1;
	if(!(SCI1SR1&0x20))
	{
		
		//SCI0DRL;
		return 0;
	}//no data to be received
	//while(SCI0SR1_RDRF==0)
	//{
		//out_wdt_reset();
		//return 0xff;
	//}
	//temp = SCI0SR1;
	temp1 = SCI1SR1;
	temp = SCI1DRL;//清接收标志
	
	return temp;
    
}

U8 uart_waitchar(void) {
  U8 c ,temp1;
	static U16 temp;			
	while(!(SCI0SR1&0x20))
	{
		temp++;
    //out_wdt_reset();
	//	if(temp>=60000){
	//		while(1);
	//	}
	}
	temp = 0;
	temp1=SCI0SR1;	
	c=SCI0DRL;
  return  c;
}
/***********************************************
*函数名：SCI1_Init
*参  数：void
*返  回：void
************************************************/
void SCI1_Init(void)
{
  
	  DDRS_DDRS2 =0;
    DDRS_DDRS3 =1;
    
    SCI1SR2 = 0x00;
    SCI1BDH = 0x00;   
   	SCI1BDL = 0x34; //00x34波特率=8M/(16*0x34)=9600          
                    //0x1a;//波特率=8M/(16*0x1a)=19200      
                    // 0x0d;//波特率=8M/(16*0x0d)=38400     
                    // 0x09;//波特率=8M/(16*0x09)=57600     
    SCI1CR1 = 0x00;
    SCI1CR2 = 0x2c; //使能、接收 00100100
}

/***********************************************
*函数名：SCI1_Send
*参  数：dat:要发送的数据
*返  回：void
************************************************/
void SCI1_Send(U8 dat)
{
    while((!SCI1SR1_TDRE)&&(SCI1SR1_TC));        
    SCI1DRL = dat;    
    while(!SCI1SR1_TC);
}
/***********************************************
*函数名：SCI1_Sendbytes
*参  数：*pdat:要发送数据的数组或指针，len：发送字节数
*返  回：void
************************************************/
void SCI1_Sendbytes(U8 *pdat,U16 len)
{
    U16 num=len;
    
    while(num--)
    {
        SCI1_Send(*pdat);
        pdat++;
        // CLEAR_WDT();
    }
}

/***********************************************
*函数名：SCI2_Init
*参  数：void
*返  回：void
************************************************/
void SCI2_Init(void)
{
 //   SCI2SR2 = 0x00;
 //   SCI2BDH = 0x00;   
 //  	SCI2BDL = 0x1a; //波特率=8M/(16*0x34)=9600          
                    //0x1a;//波特率=8M/(16*0x1a)=19200      
                    // 0x0d;//波特率=8M/(16*0x0d)=38400     
                    // 0x09;//波特率=8M/(16*0x09)=57600     
  //  SCI2CR1 = 0x00;
  //  SCI2CR2 = 0x2c; //使能、接收 00100100
}

/***********************************************
*函数名：SCI2_Send
*参  数：dat:要发送的数据
*返  回：void
************************************************/
void SCI2_Send(U8 dat)
{
  U8  temp = dat;
  //  while((!SCI2SR1_TDRE)&&(SCI2SR1_TC));        
  //  SCI2DRL = dat;    
  //  while(!SCI2SR1_TC);
}
/***********************************************
*函数名：SCI2_Sendbytes
*参  数：*pdat:要发送数据的数组或指针，len：发送字节数
*返  回：void
************************************************/
void SCI2_Sendbytes(U8 *pdat,U16 len)
{
    U16 num=len;
    
    while(num--)
    {
        SCI2_Send(*pdat);
        pdat++;
        // CLEAR_WDT();
    }
}

/***********************************************
*函数名：SCI3_Init
*参  数：void
*返  回：void
************************************************/
void SCI4_Init(void)
{    
   // SCI4SR2 = 0x00;
  //  SCI4BDH = 0x00;   
  // 	SCI4BDL = 0x34; //波特率=8M/(16*0x34)=9600          
                    //0x1a;//波特率=8M/(16*0x1a)=19200      
                    // 0x0d;//波特率=8M/(16*0x0d)=38400     
                    // 0x09;//波特率=8M/(16*0x09)=57600     
  //  SCI4CR1 = 0x00;
  //  SCI4CR2 = 0x2c; //使能、接收 00100100
}

/***********************************************
*函数名：SCI4_Send
*参  数：dat:要发送的数据
*返  回：void
************************************************/
void SCI4_Send(U8 dat)
{
  U8 temp = dat;
   // while((!SCI4SR1_TDRE)&&(SCI4SR1_TC));        
  // SCI4DRL = dat;    
   // while(!SCI4SR1_TC);
}











#pragma CODE_SEG DEFAULT





