#include "main.h"

///extern uchar PFlash_Program(dword address, uint *ptr);
///extern uchar PFlash_GetByteFlash(dword Addr,uint *Data);

#pragma CODE_SEG DEFAULT
/***************************************************************
*	main() : Entry point to the main program
****************************************************************/

#define POWER_LED_DDR       DDRB_DDRB0
#define POWER_LED_PORT      PORTB_PB0


#define  DATA_BUFFER_SIZE 128
#define  SPM_PAGESIZE   DATA_BUFFER_SIZE
 
byte rtIndex;
byte nRtIndex;
byte pcdat[DATA_BUFFER_SIZE];  



byte FlashBuffer[100];

byte FlashbufferPoint=0;




byte temp;
dword dat;
dword AcsiiToBin(byte index,byte lenght) 
{
	//byte temp;
	//dword dat;
	byte i;
	dat = 0;
	for(i=index;i<lenght+index;i++) 
	{
		dat<<=4;
		if(FlashBuffer[i]<=0x39)
		temp = FlashBuffer[i] - 0x30;
		else 
		temp = FlashBuffer[i] - 0x37;
		dat|= temp;
	}
	return dat; 
}

byte buffer[36]; 
byte CheckSum,ucCheckSum=0;

byte S1_Flash_Program(void) 
{

	// byte CheckSum;
	dword FlashS19Address; 
	byte FlashProgramLenght;
	byte i;
	byte index;
	FlashProgramLenght = AcsiiToBin(2,2);
	FlashS19Address  =   AcsiiToBin(4,4);
	//if(FlashS19Address<0xc000) return ;
	FlashProgramLenght = FlashProgramLenght;
	for(i=0;i<FlashProgramLenght;i++) 
	{
		out_wdt_reset();
		index = 4+i*2;
		buffer[i] = (byte)AcsiiToBin(index,2);
	}
	ucCheckSum =  FlashProgramLenght;
	for(i=0;i<FlashProgramLenght-1;i++)
	ucCheckSum+=buffer[i];
	ucCheckSum=0xff-ucCheckSum;
	CheckSum = buffer[FlashProgramLenght-1];
	if(ucCheckSum!=CheckSum)return 1;
	//FlashProgram( (dword)FlashS19Address,(byte *)&buffer[2],(FlashProgramLenght-3));
	//FlashProgram(0xc000,pt,6);
  	FlashProgram((dword)FlashS19Address,(byte *)&buffer[2],(FlashProgramLenght-3));	
}

byte S2_Flash_Program(void) 
{

	// byte CheckSum;
	dword FlashS19Address; 
	byte FlashProgramLenght;
	byte i;
	byte index;
	FlashProgramLenght = AcsiiToBin(2,2);
	FlashS19Address  =   AcsiiToBin(4,6);
	//if(FlashS19Address<0xc000) return ;
	FlashProgramLenght = FlashProgramLenght;
	for(i=0;i<FlashProgramLenght;i++) 
	{
		out_wdt_reset();
		index = 4+i*2;
		buffer[i] = (byte)AcsiiToBin(index,2);
	}
	ucCheckSum =  FlashProgramLenght;
	for(i=0;i<FlashProgramLenght-1;i++)
	ucCheckSum+=buffer[i];
	ucCheckSum=0xff-ucCheckSum;
	CheckSum = buffer[FlashProgramLenght-1];
	if(ucCheckSum!=CheckSum)return 1;
	
	
	//byte IFsh1_SetBlockFlash(IFsh1_TDataAddress Source, IFsh1_TAddress Addr, word Count)
	//IFsh1_SetBlockFlash((byte *)&buffer[3], (dword)FlashS19Address,(FlashProgramLenght-4));	
	FlashProgram((dword)FlashS19Address,(byte *)&buffer[3],(FlashProgramLenght-4));
}

byte S3_Flash_Program(void) 
{

	// byte CheckSum;
	dword FlashS19Address; 
	byte FlashProgramLenght;
	byte i;
	byte index;
	FlashProgramLenght = AcsiiToBin(2,2);
	FlashS19Address  =   AcsiiToBin(4,8);
	//if(FlashS19Address<0x1000) return ;
	FlashProgramLenght = FlashProgramLenght;
	for(i=0;i<FlashProgramLenght;i++) 
	{
		out_wdt_reset();
		index = 4+i*2;
		buffer[i] = (byte)AcsiiToBin(index,2);
	}
	ucCheckSum =  FlashProgramLenght;
	for(i=0;i<FlashProgramLenght-1;i++)
	ucCheckSum+=buffer[i];
	ucCheckSum=0xff-ucCheckSum;
	CheckSum = buffer[FlashProgramLenght-1];
	if(ucCheckSum!=CheckSum)return 1;
	//FlashProgram( (dword)FlashS19Address,(byte *)&buffer[4],(FlashProgramLenght-5)/4);	
  //IFsh1_SetBlockFlash((byte *)&buffer[3], (dword)FlashS19Address,(FlashProgramLenght-4));	
  FlashProgram((dword)FlashS19Address,(byte *)&buffer[4],(FlashProgramLenght-5));
}

 void S19_Dat_Program(void) 
 {

    if(FlashBuffer[0]!='S') return ;
    switch(FlashBuffer[1]) 
    {
      case '0'  : return ;break;
      case '1'  : S1_Flash_Program();break;
      case '2'  : S2_Flash_Program();break;
      case '3'  : S3_Flash_Program();break;
    }
    
 }

byte Flash_from_S19(byte *dat,byte leng)
{
	byte i=0,j=0;
	for(i=0;i<leng;i++)
	{
		out_wdt_reset();
		if((FlashbufferPoint>0)&&(dat[i]=='S')) 
		{
			FlashbufferPoint = 0;
			S19_Dat_Program();
		}
		FlashBuffer[FlashbufferPoint] = dat[i];  
		FlashbufferPoint++;   
	}
}

//byte rtIndex;
//byte nRtIndex;
byte dasdsad[10];
uint a,b;
byte overtime,overtime1;

union
{
   U8 CharData[2];
  int Int16Data;
}Int2Char1;

#define crc Int2Char1.Int16Data
#define crcLow Int2Char1.CharData[0]
#define crcHigh Int2Char1.CharData[1]


void main(void)
{
	ulong
  	uchar i,j,tempp;
  //	uchar pt[128]={1,2,3,4,5,6};
	//uint crc;
	byte packNO = 1;
	dword  timercount = 0;
	byte data;
	uint bufferPoint = 0;
	//byte rtIndex;
	//byte nRtIndex;
	dword clearAddr;
	
    asm sei;
	Cpu_Init();
	out_wdt_reset();
	Flash_Init();
	count_delay=0x00;
	flag=0;
	clr_address=0X00;
	ID_value=0X04;
	err =ERR_OK;
	for(;;)
	{
	    out_wdt_reset();
		POWER_LED_DDR = 1;
		//POWER_LED_PORT = 1;
		while(1)
		{
			//POWER_LED_DDR = 1;
			//POWER_LED_PORT = 1;
			out_wdt_reset();
			if(SCI0_Getbyte()== 'd')
			 break;
			
				if (++timercount > 0x100000)
				{
					gotdata1=0X00;
					//(void)IFsh1_GetByteFlash(0xdf10,&gotdata1);
					GetFlashData(0xdf10,&gotdata1,1);
					timercount=0x00;
					if(gotdata1!=0xFF)
					{		
						__asm("jmp 0XC000");       //goto user code
					}
				}
			
		}		
		
		
		EraseFlash();

		while(SCI0_Getbyte()!=XMODEM_SOH)        //receive the start of Xmodem
	    {
	        out_wdt_reset();
            if(++timercount > 10000)                        //wait about 1 second
            {
                SCI0_Send(XMODEM_RECIEVING_WAIT_CHAR);    //send a "C"
                timercount=0;
            }
	        
	    }
		
		overtime=0;
		overtime1=0;
		do
	    {		
		  	rtIndex  =  uart_waitchar();	
	    	nRtIndex =  uart_waitchar();
			nRtIndex = ~nRtIndex;
	      if ((packNO >= rtIndex) && (rtIndex ==nRtIndex))
			 {    //核对数据块编号正确
				POWER_LED_PORT=1;
 				for(bufferPoint=0,i=0;i<DATA_BUFFER_SIZE;i++)                //接收128个字节数据
				{
			
					pcdat[bufferPoint]= uart_waitchar();
					bufferPoint++;    
				}
				
				crcLow = uart_waitchar();	
				crcHigh= uart_waitchar();            //接收2个字节的CRC效验字
			
        POWER_LED_PORT=0;
			//	  a=calcrc(&pcdat[0],DATA_BUFFER_SIZE);
				if(calcrc(&pcdat[0],DATA_BUFFER_SIZE)==crc)    //CRC校验验证		
				{    //正确接收128个字节数据
					while(bufferPoint >= SPM_PAGESIZE)
					{    
						Flash_from_S19(pcdat,128);
						bufferPoint = 0;                   
					}    
					overtime=0;
					packNO++;                        //数据块编号加1
			  SCI0_Send(XMODEM_ACK);        //正确收到一个数据块	
				}
				else
				{
						for(i=0;i<(DATA_BUFFER_SIZE+2);i++)                //接收128+2校验个字节数据
			     	{			
					    uart_waitchar();
				    
			    	}
					
				   overtime++;	
					  if(overtime>=10)
				  {
				 
		
				   while(1);
				 // POWER_LED_PORT=0;
				  }
															   		
				  SCI0_Send(XMODEM_NAK);        //要求重发数据块		               
				}
				
			}
			else
			{

  
				      SCI0_Send(XMODEM_NAK);                //要求重发数据块				      	
				     if(packNO>1)
				     {
					   overtime1++;
					  if(overtime1>=10)
					  {
						while(1);
					   }
				    }
                  
			}	    
		 tempp=uart_waitchar();	   	
	  	out_wdt_reset();	
	    }while(tempp!=XMODEM_EOT);
	    
		SCI0_Send(XMODEM_ACK);
		__asm("jmp 0XC000");       //goto user code
		//packNO=1;
		
	}
	
}

void CRGModuleInitial(void)
{
	#if(OSC_CLK==8)				// osc=8M
	SYNR  =0x0f;
	REFDV =0x07;
	#else if(OSC_CLK==16)	// OSC=16M
	SYNR  =0x0F;
	REFDV =0x0F;
  #endif 	
	POSTDIV=0x01;				// PLL=16M
  CLKSEL_PLLSEL=0;    //bus clock=OSC/2 8M
//  CLKSEL_PLLSEL=1;    //bus clock=PLL/2 8M
  CLKSEL_RTIWAI=1;	  //rtc stop in wait mode,等待模式中停止工作
  CLKSEL_COPWAI=1;	  //cop stop in wait mode，等待模式中停止工作
  #if(OSC_CLK==8)
  RTICTL=0x49;        //8M OSC 10ms enter RTI;
  #else if(OSC_CLK==16)
  RTICTL=0x59;        //16M OSC 10ms enter RTI;
  #endif 
  COPCTL=0x00;        // COP  funtion  disable                 
  CRGINT_RTIE=0;      //RTI interrupt enable   
}




