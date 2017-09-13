/* MODULE IFsh1. */


#include "Flash.h"

volatile unsigned char page;
volatile unsigned char page_p,page_d;
CMD_BUF commandArray;

/* Definition of DATA and CODE segments for this bean. User can specify where
   these segments will be located on "Build options" tab of the selected CPU bean. */

#pragma CODE_SEG FLASH_RAM 

static void wait_program(void)
{
	FSTAT = CCIF_MASK;
	while (!FSTAT_CCIF) { 				/* wait for the command to complete */
	 ; /* Return status. */
	}
}
//#pragma CODE_SEG IFsh1_CODE            /* Code section for this module. */

/************************* global const arrays **********************************/

const uint FlashSectSize[2] = { 256, 1024 }; /* 256 sector size in bytes for D-flash, 1024 for P-flash */
const uint FlashStartAddr[2]={0x800,0x8000};

const byte EraseFlashSector[2] = {
  ERASE_D_FLASH_SECTOR, ERASE_P_FLASH_SECTOR
};

const byte EraseVerifyFlashSector[2] = {
  ERASE_VERIFY_D_FLASH_SECTION, ERASE_VERIFY_P_FLASH_SECTION
};

const byte ProgramFlash[2] = {
  PROGRAM_D_FLASH, PROGRAM_P_FLASH
};

void Flash_Init(void)
{                        	// 7FE000-7FFFFF保护
  FPROT   = 0x94;					// Flash block can be programmed and erased，无保护
//  DFPROT  = 0xff;					// D-Flash block can be programmed and erased
  IVBR    = 0XDF;
  //FCLKDIV = 0X41;
  FSTAT=0x30;							// 清ACCERR 、FPVIOL错误标志
	FERSTAT =0x03;					// DFDIF SFDIF 标志
  FCNFG=0x10;  						// Ignore Single Bit Fault
	if((FCLKDIV&0x80)==0)
  	FCLKDIV = 0X0F;				// select clock divider depending on crystal  (OSC/FCLKDIV=1M)
}

void ClearFlags(void)
{
  PPAGE = 0x7f;                           /* Select bank 3 */
                                       	/* Clear any error flags*/	
	FSTAT = (FPVIOL_MASK | ACCERR_MASK); 
}

void SelectBank(byte type,dword Addr)
{

	if(type==1)
	{
  	page_p =(byte)(Addr >> 16);
		if(page_p==0)
		{
			page_p=0x7f;
		}		
//		PPAGE =page_p;
	}
	else if(type==0)
	{
		page_d =(byte)(Addr >> 16);
//		DPAGE =page_d;
	}
}


byte LaunchFlashCommand(byte params, byte command, byte AddrH, uint AddrL, uint* Pdata)
{
	uchar err_num;

  if(FSTAT_CCIF == 1)
	{																	
		/* Clear any error flags*/	
	  FSTAT = (FPVIOL_MASK | ACCERR_MASK); 

    /* Write the command id / ccob0 */
		FCCOBIX = 0;
		FCCOBHI = command;
		FCCOBLO = AddrH;

    if(++FCCOBIX != params) {
      FCCOB = AddrL; 			/* Write next data word to CCOB buffer. */ 
      if(++FCCOBIX != params) {
  		  FCCOB = Pdata[0]; 			/* Write next data word to CCOB buffer. */
        if(++FCCOBIX != params) {
   		    FCCOB = Pdata[1]; 			/* Write next data word to CCOB buffer. */
          if(++FCCOBIX != params) {
            FCCOB = Pdata[2]; 			/* Write next data word to CCOB buffer. */
            if(++FCCOBIX != params) 
         		  FCCOB = Pdata[3]; 			/* Write next data word to CCOB buffer. */
          } 											
        }  
	    }
    }
	  FCCOBIX = params-1;
	  
	 // wait_program();			// 执行并等待完成
	 	/* Clear command buffer empty flag by writing a 1 to it */
		FSTAT = CCIF_MASK;
    while (!FSTAT_CCIF) {					/* wait for the command to complete */
     ; /* Return status. */
    }
		err_num =FSTAT;
		err_num &=0xfc;
    return(err_num);							/* command completed */
  } 
  err_num=FLASH_BUSY;
	return(err_num);								   /* state machine busy */
}


// 擦除扇区

byte EraseSectorFlash(dword Addr)
{
	uint temp_W, Addr_L;
	uchar Addr_H,type;
	Addr_L=(uint)Addr;
	// DFLASH
	if((0x0800<=Addr_L)&&(0x0C00>=Addr_L))
	{                           // 00 0111  0000  0000 00  00 0000
		temp_W=(uint)(Addr>>6);	 //0000 0000	0000  1100 00  00 0000
		temp_W=temp_W&0x1c00;

		Addr_L=(uint)Addr;
		Addr_L &=0x03FF;
		Addr_L =Addr_L|temp_W;
		Addr_H =0x10;	
		type=0x00;
	}// PFLASH
	else 
	{
		Addr_H=(uchar)(Addr>>18);      //fa8000 1111 10 10
		if(Addr_H==0)
		{
			Addr_H=0x7f;
			Addr_L=(uint)Addr;
		}
		else
		{ 
			Addr_H|=0x40; 		// 该位需要置1
			temp_W =(uint)(Addr>>2);			
			temp_W =temp_W&0xc000;
			Addr_L=(uint)Addr;
			Addr_L &=0x3FFF;
			Addr_L =Addr_L|temp_W;
		}
		type=0x01;
	}
	
	if(LaunchFlashCommand(2,EraseFlashSector[type], Addr_H,Addr_L,commandArray.w_buf) != CCIF_MASK) // timing 1 word program command 
     return FLASH_ACCESS_ERROR;  
	commandArray.w_buf[0]=FlashSectSize[type]>>3;		// 设置校验的长度
	if(LaunchFlashCommand(3,EraseVerifyFlashSector[type], Addr_H,Addr_L,commandArray.w_buf) != CCIF_MASK)
		return FLASH_ACCESS_ERROR;  
	return FLASH_TRUE;
}

uchar falg_flash;
void EraseFlash(void)
{
	uchar i,j;
	dword flashadd;
	flashadd = 0x4000;
	for(i=0;i<16;i++) //4000-7fff
	{
		out_wdt_reset();
		EraseSectorFlash(flashadd);
		//GetFlashDataSum(flashadd,&falg_flash,128);
		flashadd +=0x400;	
	}
	flashadd = 0xc000;
	for(i=0;i<8;i++) //c000-dfff
	{
		out_wdt_reset();
		EraseSectorFlash(flashadd);
		flashadd +=0x400;
	}
	flashadd = 0xE08000;
	for(j=0;j<8;j++)
	{	
		for(i=0;i<16;i++)
		{
			out_wdt_reset();
			EraseSectorFlash(flashadd);
			flashadd +=0x400;
		}
		flashadd += 0xc000;					
	}
	flashadd = 0xf88000;
	for(j=0;j<6;j++)
	{	
		for(i=0;i<16;i++)
		{
			out_wdt_reset();
			EraseSectorFlash(flashadd);
			flashadd +=0x400;
		}
		if(j==4)
		{
			flashadd += 0x1c000;	
		}
		else
		{
			flashadd += 0xc000;
		}							
	}
}
byte GetFlashData(dword Addr,uchar* pdata,byte len)
{
	uint addr_temp;

	out_wdt_reset();
	addr_temp =(uint)Addr;
	if((0x0800<=addr_temp)&&(0x0C00>=addr_temp))
	{	
		SelectBank(0,Addr);
		EPAGE=page_d;
	}
	else
	{
		SelectBank(1,Addr);
		PPAGE=page_p;
	}
	if(FSTAT_CCIF != 1)
		return FLASH_BUSY;
	addr_temp=(uint)Addr;
	while(len!=0)
	{
		len--;
		*pdata=*((volatile byte *)(addr_temp));
		pdata++;
		addr_temp++;
	}
	return FLASH_TRUE;
}

byte GetFlashDataSum(dword Addr,uchar* checkSum,byte len)
{
	byte addr_temp;

	out_wdt_reset();
	addr_temp =(uint)Addr;	

	if((0x0800<=addr_temp)&&(0x0C00>=addr_temp))
	{	
		SelectBank(0,Addr);
		EPAGE=page_d;
	}
	else 
	{
		SelectBank(1,Addr);
		PPAGE=page_p;
	}

	if(FSTAT_CCIF != 1)
		return FLASH_BUSY;
	addr_temp=(uint)Addr;
	while(len!=0)
	{
		len--;
		*checkSum =*checkSum+*((volatile byte *)(addr_temp));
		addr_temp++;
	}
	return FLASH_TRUE;
}


byte FlashProgram(dword Addr,byte *PData,byte len)
{
	uint Addr_temp;
	uchar status;
	
	out_wdt_reset();
	Addr_temp =(uint)Addr;
	if((0x0800<=Addr_temp)&&(0x0C00>=Addr_temp)) 
	{
	  status = 	DFlashProgram(Addr,PData,len);
	  if(status) 
	  return   status;
	}
	else 
	{
	  
		 status = PFlashProgram(Addr,PData,len);
		  if(status) 
		  return   status;
	}
	return FLASH_TRUE;
}


byte DFlashProgram(dword Addr,byte *PData,byte len)
{
	uint temp_W, Addr_L;
	uchar lenTemp,Addr_H;
	uchar i,flag;
	
	flag=0;
	if((Addr&0x01)!=0)
	{
		flag=1;
		if(GetFlashData(Addr&0xfffffe,commandArray.b_buf,2)!=FLASH_TRUE)
			return FLASH_ACCESS_ERROR;
		commandArray.w_buf[1]=0xffff;
		commandArray.w_buf[2]=0xffff;
		commandArray.w_buf[2]=0xffff;
	}
	
// DFLASH

	temp_W=(uint)(Addr>>6);		
	temp_W=temp_W&0x1c00;

	Addr_L=(uint)Addr;
	Addr_L &=0x03FF;
	Addr_L =Addr_L|temp_W;
	Addr_H =0x10;	

	if(Addr_L&0x01!=0) // 调整奇数地址
		len+=1;

	while(len)
	{
		if(len>8)
		{
			len=len-8;
			lenTemp=8;
		}
		else
		{
			lenTemp=len;
			len=0;
			if(flag==0)
				commandArray.w_buf[0]=0xffff;
			commandArray.w_buf[1]=0xffff;
			commandArray.w_buf[2]=0xffff;
			commandArray.w_buf[3]=0xffff;
		}
		for(i=(uchar)(Addr_L&0x01);i<lenTemp;i++)
		{
			if(flag==1)
			{
				if((commandArray.b_buf[i]==0xff)||(commandArray.b_buf[i]==*PData))
				{			
					commandArray.b_buf[i]=*PData;
					PData++;
				}
				else
					return FLASH_ACCESS_ERROR; 
			}
			else
			{
				commandArray.b_buf[i]=*PData;
				PData++;
			}
				
		}
		flag =0;
		Addr_L=Addr_L&0xfffe;
		if(lenTemp&0x01!=0)
			lenTemp++;
		lenTemp=lenTemp>>1;
		lenTemp=lenTemp+2;

	  if(LaunchFlashCommand(lenTemp, ProgramFlash[0] , Addr_H , Addr_L, commandArray.w_buf) != CCIF_MASK) //timing 1 word program command 
      return FLASH_ACCESS_ERROR;   		
		Addr_L=Addr_L+8;			
	}
	return FLASH_TRUE;
}


byte PFlashProgram(dword Addr,byte *PData,byte len)
{
	uint temp_W, Addr_L;
	uchar lenTemp,Addr_H;
	uchar i,flag;
	if(Addr==0xFA80A1) 
	{
	
	asm nop;
	asm nop;
	asm nop;
	}
	
	flag=0;
	if((Addr&0x07)!=0)
	{
		flag=1;
		if(GetFlashData(Addr&0xfffff8,commandArray.b_buf,8)!=FLASH_TRUE) 
		{
			return FLASH_ACCESS_ERROR;  
		}

	}
	// PFALSH
	Addr_H=(uchar)(Addr>>18);       
	if(Addr_H==0)
	{
			Addr_H=0x7f;
			Addr_L=(uint)Addr;
	}
	else
	{	
		Addr_H|=0x40;			// 该位需要置1
		temp_W =(uint)(Addr>>2);
		temp_W =temp_W&0xc000;
		Addr_L=(uint)Addr;
		Addr_L &=0x3FFF;
		Addr_L =Addr_L|temp_W;
	}
	lenTemp =(uchar)(Addr_L&0x07);// 调整非对齐地址
	len +=lenTemp;
	while(len)
	{
		if(len>8)
		{
			len=len-8;
			lenTemp=8;
		}
		else
		{
			lenTemp=len;
			len=0;
			if(flag==0)
			{
				commandArray.w_buf[0]=0xffff;
				commandArray.w_buf[1]=0xffff;
				commandArray.w_buf[2]=0xffff;
				commandArray.w_buf[3]=0xffff;
			}
		}
		for(i=(uchar)(Addr_L&0x07);i<lenTemp;i++)
		{
			if(flag==1)
			{
				if((commandArray.b_buf[i]==0xff)||(commandArray.b_buf[i]==*PData))
				{			
					commandArray.b_buf[i]=*PData;
					PData++;
				}
				else{
					return FLASH_ACCESS_ERROR; 
				}
			}
			else
			{
				commandArray.b_buf[i]=*PData;
				PData++;
			}
		}
		flag=0;
		Addr_L=Addr_L&0xfff8;
	  if(LaunchFlashCommand(6, ProgramFlash[1] , Addr_H , Addr_L, commandArray.w_buf) != CCIF_MASK) //timing 1 word program command 
     {				  
      return FLASH_ACCESS_ERROR;  
     }
		Addr_L=Addr_L+8;			
	}
	return FLASH_TRUE;
}



unsigned short calcrc ( char *ptr, unsigned short count )
{
	unsigned short crc, i;
	crc = 0;
	while(count--)
	{
		out_wdt_reset();
   crc = crc ^ (int) *ptr++ << 8;//从packet_data中取一个字节强转为16为int
                         //，再把低八位移到高八位，赋值给crc
	
		for(i = 0; i < 8; i++)
		{
			if(crc & 0x8000)//判断数据的最高位数据是否为1
				crc = ((crc << 1) ^ 0x1021); //	CRC-ITU
			else			
				crc = crc << 1;
		}
	}

 return (crc & 0xFFFF);
}
/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 03.11 for 
**     the Motorola HCS12 series of microcontrollers.
**
** ###################################################################
*/
