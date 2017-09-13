/* MODULE IFsh1. */

#include "Flash.h"
#include <MC9S12XET256.h>

#define SaveStatusReg()     { __asm pshc; __asm sei; __asm movb 1,SP+,CCR_reg; } /* This macro is used by Processor Expert. It saves CCR register and disable global interrupts. */
#define RestoreStatusReg()  { __asm movb CCR_reg, 1,-SP; __asm pulc; } /* This macro is used by Processor Expert. It restores CCR register saved in SaveStatusReg(). */


volatile unsigned char page;
/* Definition of DATA and CODE segments for this bean. User can specify where
   these segments will be located on "Build options" tab of the selected CPU bean. */
/////#pragma DATA_SEG IFsh1_DATA            /* Data section for this module. */
//#pragma CODE_SEG FLASH_RAM

//#pragma CODE_SEG IFsh1_CODE            /* Code section for this module. */



static void wait_program(void)
{
	FSTAT = CCIF_MASK;
	while (!FSTAT_CCIF) { 				/* wait for the command to complete */
	 ; /* Return status. */
	}
}




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




/* Global variables */
static byte WaitInRAMcode[]={
 0x1E, 0x01, 0x05, 0x20, 0x05,         /* BRSET _FSTAT,#32,*+10 */
 0x1F, 0x01, 0x05, 0x10, 0x03,         /* BRCLR _FSTAT,#16,*+8 */
 0xC6, 0x09,                           /* LDAB  #9 */
 0x3D,                                 /* RTS */
 0x1F, 0x01, 0x05, 0x80, 0xFB,         /* BRCLR _FSTAT,#128,*+0 */
 0x1F, 0x01, 0x05, 0x40, 0xFB,         /* BRCLR _FSTAT,#64,*+0 */
 0x3D                                  /* RTS */
};
/*
** ===================================================================
**     Method      :  ClearFlags (bean IntFLASH)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/

void FlashInit(void)
{
  FPROT   = 0x94;				// Flash block can be programmed and erased
  //IVBR    = 0XDF;     //df10-dffe;
  FCLKDIV = 0X41;			// select clock divider depending on crystal  
}

	/*void FlashInit(void)
	{	// 7FE000-7FFFFF保护
	  FPROT   = 0x94;					// Flash block can be programmed and erased，无保护
	 // DFPROT  = 0xff;					// D-Flash block can be programmed and erased
	  FSTAT=0x30;							// 清ACCERR 、FPVIOL错误标志
		FERSTAT =0x03;					// DFDIF SFDIF 标志
	  FCNFG=0x10;						// Ignore Single Bit Fault
		if((FCLKDIV&0x80)==0)
		FCLKDIV = 0X0F; 			// select clock divider depending on crystal  (OSC/FCLKDIV=1M)
	}
*/
/////////////////////////////////////////////////////////////////////
static void ClearFlags(void)
{
  FCNFG = 3;                           /* Select bank 3 */
  FSTAT = 48;                          /* Clear PVIOL & ACERR flag */
  FCNFG = 2;                           /* Select bank 2 */
  FSTAT = 48;                          /* Clear PVIOL & ACERR flag */
  FCNFG = 1;                           /* Select bank 1 */
  FSTAT = 48;                          /* Clear PVIOL & ACERR flag */
  FCNFG = 0;                           /* Select bank 0 */
  FSTAT = 48;                          /* Clear PVIOL & ACERR flag */
}

/*
** ===================================================================
**     Method      :  SelectBank (bean IntFLASH)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
/*ic void SelectBank(dword Addr)
{
page= (byte)(Addr >> 16);
}
*/
/*
** ===================================================================
**     Method      :  WriteWord (bean IntFLASH)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
static byte WriteWord(word AddrRow,word Data16)
{
  byte err;
PPAGE=page;
  *(volatile word *) AddrRow = Data16; /* Array address and program data */
//////  FCMD = 32;                           /* Word program command */
  FSTAT = 128;                         /* Clear flag command buffer empty */
  asm {                                /* Jump to Wait in RAM code */
    CLRB
    JSR WaitInRAMcode
    STAB err
  }
  if (err) return ERR_NOTAVAIL;        /* Return error if is it need */
  if (*(volatile word *) AddrRow != Data16) /* Was attempt to write data to the given address errorneous? */
    return ERR_VALUE;                  /* If yes then error */
 return ERR_OK;                        /* OK */
}    
/////////////////////////////////////////////////////
static byte WriteWord1(word AddrRow,word Data16)
{
  byte err;
PPAGE=page;
  *(volatile word *) AddrRow = Data16; /* Array address and program data */
//////  FCMD = 32;                           /* Word program command */
  FSTAT = 128;                         /* Clear flag command buffer empty */
  asm {                                /* Jump to Wait in RAM code */
    CLRB
    JSR WaitInRAMcode
    STAB err
  }
  //if (err) return ERR_NOTAVAIL;        /* Return error if is it need */
  //if (*(volatile word *) AddrRow != Data16) /* Was attempt to write data to the given address errorneous? */
  //  return ERR_VALUE;                  /* If yes then error */
 //return ERR_OK;                        /* OK */
}
/*
** ===================================================================
**     Method      :  IFsh1_SetByteFlash (bean IntFLASH)
**
**     Description :
**         Write byte to address in FLASH.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Addr            - Address to FLASH
**         Data            - Data to write
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_NOTAVAIL - Requested value not
**                           available
**                           ERR_RANGE - Value is out of range
**                           ERR_VALUE - Parameter of incorrect value\
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte IFsh1_SetByteFlash(dword Addr,byte Data)
{

  out_wdt_reset();
  //if(Addr<65536)
  //{
  //    if((Addr<0X4000)||(Addr>0XDFFF)||((Addr>0X7FFF)&&(Addr<0XC000)))
  //          return(ERR_RANGE);                  /* Check range of address */
  //} 
  //else if(((Addr&65535)<32768)||((Addr&65535)>49151)||((Addr>>16)<224)||((Addr>>16)>251))
  //   return(ERR_RANGE);                  /* Check range of address */
  ClearFlags();                        /* Clear all flags */
  ////SelectBank(Addr);                    /* Select bank and page */
  PPAGE=page;
  //////if (FSTAT_CBEIF == 0)                /* Is command buffer full ? */
    ///////return ERR_BUSY;                   /* If yes then error */
  if (Addr&1) //{
    WriteWord1(Addr & 65534, 65280 | Data); /* Not Aligned word ?*/
    //if((err==ERR_VALUE)&&(((*(volatile word*)(Addr&65534))&255)!=Data)) return (ERR_VALUE); /* Error check only one byte in word */
    //else return(ERR_OK);
  //} 
  else //{
    WriteWord1(Addr & 65534, (Data<<8) | 255); /* Not Aligned word ?*/
    //if((err==ERR_VALUE)&&((((*(volatile word*)(Addr&65534))&65280)>>8)!=Data)) return (ERR_VALUE); /* Error check only one byte in word */
    //else return(ERR_OK);
  //}
}

/*
** ===================================================================
**     Method      :  IFsh1_GetByteFlash (bean IntFLASH)
**
**     Description :
**         Method gets byte from address in FLASH
**     Parameters  :
**         NAME            - DESCRIPTION
**         Addr            - Address to FLASH
**       * Data            - Pointer to returned 8-bit data
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte IFsh1_GetByteFlash(dword Addr,byte *Data)
{
  out_wdt_reset();

  if(Addr<65536)
  {
      if((Addr<0X4000)||(Addr>0XDFFF)||((Addr>0X7FFF)&&(Addr<0XC000)))
            return(ERR_RANGE);                  /* Check range of address */
  } 
  //else if(((Addr&65535)<32768)||((Addr&65535)>49151)||((Addr>>16)<224)||((Addr>>16)>251))
  //   return(ERR_RANGE);                  /* Check range of address */
  /////SelectBank(Addr);                    /* Select bank and page */
  PPAGE=page;
  *Data = *(volatile byte *)(Addr&0xFFFF); /* Read byte from FLASH */
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  IFsh1_SetWordFlash (bean IntFLASH)
**
**     Description :
**         Write word to address in FLASH.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Addr            - Address to FLASH
**         Data            - Data to write
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_NOTAVAIL - Requested value not
**                           available
**                           ERR_RANGE - Value is out of range
**                           ERR_VALUE - Parameter of incorrect value\
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte IFsh1_SetWordFlash(dword Addr,word Data)
{
  byte err;

  out_wdt_reset();
  if(Addr<65536)
  {
      if((Addr<0X4000)||(Addr>0XDFFF)||((Addr>0X7FFF)&&(Addr<0XC000)))
            return(ERR_RANGE);                  /* Check range of address */
  } 
  //else if(((Addr&65535)<32768)||((Addr&65535)>49151)||((Addr>>16)<224)||((Addr>>16)>251))
  //   return(ERR_RANGE);                  /* Check range of address */
  if (Addr & 1)                        /* Aligned address ? */
    return ERR_NOTAVAIL;
  ClearFlags();                        /* Clear all flags */
  /////SelectBank(Addr);                    /* Select bank and page */
  PPAGE=page;
  if (*(volatile word*)(Addr & 65535) != 65535) { /* Word in Flash not erased ? */
    err=IFsh1_EraseSector(Addr);       /* Erase sector */
    if (err != ERR_OK) return err;     /* Previous operation was error ? */
  }
  //////if (FSTAT_CBEIF == 0)                /* Is command buffer full ? */
   ////// return ERR_BUSY;                   /* If yes then error */
  err = WriteWord(Addr & 65535, Data); /* Write new data to Flash */
  return(err);
}
/*
** ===================================================================
**     Method      :  IFsh1_GetWordFlash (bean IntFLASH)
**
**     Description :
**         Method gets word from address in FLASH
**     Parameters  :
**         NAME            - DESCRIPTION
**         Addr            - Address to FLASH
**       * Data            - Pointer to returned 16-bit data
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte IFsh1_GetWordFlash(dword Addr,word *Data)
{
  out_wdt_reset();

  if(Addr<65536)
  {
      if((Addr<0X4000)||(Addr>0XDFFF)||((Addr>0X7FFF)&&(Addr<0XC000)))
            return(ERR_RANGE);                  /* Check range of address */
  } 
  //else if(((Addr&65535)<32768)||((Addr&65535)>49151)||((Addr>>16)<224)||((Addr>>16)>251))
  //   return(ERR_RANGE);                  /* Check range of address */
  ////SelectBank(Addr);                    /* Select bank and page */
  PPAGE=page;
  *Data = *(volatile word*)(Addr&65535); /* Get word form Flash */
  return ERR_OK;
}
/*
** ===================================================================
**     Method      :  IFsh1_EraseSector (bean IntFLASH)
**
**     Description :
**         Erase sector to which address Addr belongs.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Addr            - Address in FLASH
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_NOTAVAIL - Requested value not
**                           available
**                           ERR_RANGE - Value is out of range
**                           ERR_VALUE - Parameter of incorrect value\
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
/*
byte IFsh1_EraseSector(dword Addr)
{
  byte err;

  out_wdt_reset();
  //if(((Addr&65535)<32768)||((Addr&65535)>49151)||((Addr>>16)<224)||((Addr>>16)>251))
  if(
    (((Addr>>16)>=0xe0)&&((Addr>>16)<=0xfe)||((Addr&65535)>=0x8000)||((Addr&65535)<=0xbfff))
    ||(((Addr&0xffffff)>=0x4000)&&((Addr&0xffffff)<=0x7fff))
    ||(((Addr&0xffffff)>=0xc000)&&((Addr&0xffffff)<=0xdfff))) 
  {
    
    ClearFlags();                        // Clear all flags 
   //// SelectBank(Addr);                    // Select bank and page 
    PPAGE=page;
    //////if (FSTAT_CBEIF == 0)                // Is command buffer full ? 
      //////return ERR_BUSY;                   // If yes then error 
    *(volatile word *) (Addr&65535) = 10; // Write eny word to FLASH buffer 
   ////// FCMD = 64;                           // Initiate Sector Erase commamd 
    FSTAT = 128;                         // Launch the command 
    asm {                                // Jump to Wait in RAM code 
      CLRB
      JSR WaitInRAMcode
      STAB err
    }
    if (err) return err;                 // Return error if need 
    return ERR_OK;                       // OK /
  } 
  else      return(ERR_RANGE);                  // Check range of address /
}
*/
/*
** ===================================================================
**     Method      :  IFsh1_Init (bean IntFLASH)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
void IFsh1_Init(void)
{
	//IVBR = 0XDF;
  FCLKDIV = 41;                        /* Set up Clock Divider Register */
}
/* END IFsh1. */



/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 03.11 for 
**     the Motorola HCS12 series of microcontrollers.
**
** ###################################################################
*/
/*
static void wait_program(void)
{
	FSTAT = CCIF_MASK;
	while (!FSTAT_CCIF) { 				
	 ; 
	}
}
*/

byte EraseSectorFlash(dword Addr)
{
	uint temp_W, Addr_L;
	uchar Addr_H,type;

	out_wdt_reset();

	Addr_L=(uint)Addr;
	// DFLASH
	if((0x0800<=Addr_L)&&(0x0C00>=Addr_L))
	{
		temp_W=(uint)(Addr>>6);		
		temp_W=temp_W&0x1c00;

		Addr_L=(uint)Addr;
		Addr_L &=0x03FF;
		Addr_L =Addr_L|temp_W;
		Addr_H =0x10;	
		type=0x00;
	}// PFLASH
	else 
	{
		Addr_H=(uchar)(Addr>>18);

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
			asm nop;
			
		}
		type=0x01;
		
		//Addr_H=(uchar)(Addr>>16);
		//Addr_L=(uint)Addr;
		//type=0x01;
	}
	
	if(LaunchFlashCommand(2,EraseFlashSector[type], Addr_H,Addr_L,commandArray.w_buf) != CCIF_MASK) // timing 1 word program command 
	{
		asm nop;
    	return FLASH_ACCESS_ERROR; 
	}
	commandArray.w_buf[0]=FlashSectSize[type]>>3;		// 设置校验的长度
	if(LaunchFlashCommand(3,EraseVerifyFlashSector[type], Addr_H,Addr_L,commandArray.w_buf) != CCIF_MASK)
	{
		asm nop;
		return FLASH_ACCESS_ERROR;  
	}
	asm nop;
	return FLASH_OK;
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
	return FLASH_OK;
}

byte FlashProgram(dword Addr,byte *PData,byte len)
{
	uint Addr_temp;
	
	out_wdt_reset();
	Addr_temp =(uint)Addr;
	if((0x0800<=Addr_temp)&&(0x0C00>=Addr_temp))
		DFlashProgram(Addr,PData,len);
	else
		PFlashProgram(Addr,PData,len);
	return 0;
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
		if(GetFlashData(Addr&0xfffffe,commandArray.b_buf,2)!=FLASH_OK)
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
	return FLASH_OK;
}


byte PFlashProgram(dword Addr,byte *PData,byte len)
{
	uint temp_W, Addr_L;
	uchar lenTemp,Addr_H;
	uchar i,flag;
	
	flag=0;
	if((Addr&0x07)!=0)
	{
		flag=1;
		if(GetFlashData(Addr&0xfffff8,commandArray.b_buf,8)!=FLASH_OK)
			return FLASH_ACCESS_ERROR;
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
				else
					return FLASH_ACCESS_ERROR; 
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
      return FLASH_ACCESS_ERROR;   		
		Addr_L=Addr_L+8;			
	}
	return FLASH_OK;
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

    	if(++FCCOBIX != params) 
		{
      		FCCOB = AddrL; 			/* Write next data word to CCOB buffer. */ 
      		if(++FCCOBIX != params) 
			{
  		  		FCCOB = Pdata[0]; 			/* Write next data word to CCOB buffer. */
        		if(++FCCOBIX != params) 
				{
   		    		FCCOB = Pdata[1]; 			/* Write next data word to CCOB buffer. */
          			if(++FCCOBIX != params) 
					{
            			FCCOB = Pdata[2]; 			/* Write next data word to CCOB buffer. */
            			if(++FCCOBIX != params) 
         		  		FCCOB = Pdata[3]; 			/* Write next data word to CCOB buffer. */
          			} 											
        		}  
	   		}
    	}
	  	FCCOBIX = params-1;
	  	wait_program();			// 执行并等待完成
	 	/* Clear command buffer empty flag by writing a 1 to it */
//		FSTAT = CCIF_MASK;
//    while (!FSTAT_CCIF) {					/* wait for the command to complete */
//     ; /* Return status. */
//    }
		err_num =FSTAT;
		err_num &=0xfc;
    	return(err_num);							/* command completed */
  	} 
  	err_num=FLASH_BUSY;
	return(err_num);								   /* state machine busy */
}

byte GetFlashDataSum(dword Addr,uchar* checkSum,byte len)
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
		*checkSum =*checkSum+*((volatile byte *)(addr_temp));
		addr_temp++;
	}
	return FLASH_OK;
}



/*
uint calcrc(uchar *ptr, uint count)
{
    uint crc = 0;
    uchar i;
    while (--count >= 0)
    {
        crc = crc ^ (int) *ptr++ << 8;
        i = 8;
        do
        {
        if (crc & 0x8000)
            crc = crc << 1 ^ 0x1021;
        else
            crc = crc << 1;
        } while(--i);
    }
    return (crc&0xffff);
}
*/
unsigned short calcrc ( char *ptr, unsigned short count )
{
	unsigned short crc, i;
	crc = 0;
	while(count--)
	{
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


