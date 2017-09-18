/* MODULE IFsh1. */

#include "Flash.h"
#include <mc9s12xhz256.h>

#define SaveStatusReg()     { __asm pshc; __asm sei; __asm movb 1,SP+,CCR_reg; } 	/* This macro is used by Processor Expert. It saves CCR register and disable global interrupts. */
#define RestoreStatusReg()  { __asm movb CCR_reg, 1,-SP; __asm pulc; } 						/* This macro is used by Processor Expert. It restores CCR register saved in SaveStatusReg(). */


volatile unsigned char page;
/* Definition of DATA and CODE segments for this bean. User can specify where
   these segments will be located on "Build options" tab of the selected CPU bean. */
#pragma DATA_SEG IFsh1_DATA            /* Data section for this module. */
#pragma CODE_SEG IFsh1_CODE            /* Code section for this module. */

/* Global variables */
static byte WaitInRAMcode[]={
 0x1E, 0x01, 0x05, 0x20, 0x05,         /* BRSET _FSTAT,#32,*+10 */
 0x1F, 0x01, 0x05, 0x10, 0x03,         /* BRCLR _FSTAT,#16,*+8 */
 0xC6, 0x09,                           	/* LDAB  #9 */
 0x3D,                                 		/* RTS */
 0x1F, 0x01, 0x05, 0x80, 0xFB,         /* BRCLR _FSTAT,#128,*+0 */
 0x1F, 0x01, 0x05, 0x40, 0xFB,         /* BRCLR _FSTAT,#64,*+0 */
 0x3D                                  		/* RTS */
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
  IVBR    = 0XDF;     	//df10-dffe;
  FCLKDIV = 0X41;				// select clock divider depending on crystal  
}
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
static void SelectBank(dword Addr)
{
  FCNFG_BKSEL = (byte)((63 - (Addr >> 16)) / 4); /* Bank number */
  page= (byte)(Addr >> 16);
}

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
  FCMD = 32;                           /* Word program command */
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
static void WriteWord1(word AddrRow,word Data16)
{
  ///byte err;
	
	PPAGE=page;
  *(volatile word *) AddrRow = Data16; 	/* Array address and program data */
  FCMD = 32;                           		/* Word program command */
  FSTAT = 128;                         		/* Clear flag command buffer empty */
  asm {                                		/* Jump to Wait in RAM code */
    CLRB
    JSR WaitInRAMcode
    ///STAB err
  }
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
  ///out_wdt_reset();
  
  ClearFlags();                        /* Clear all flags */
  SelectBank(Addr);                   /* Select bank and page */
  PPAGE=page;
  if (FSTAT_CBEIF == 0)                	/* Is command buffer full ? */
    return ERR_BUSY;                   /* If yes then error */
  if (Addr&1)
    WriteWord1(Addr & 65534, 65280 | Data); /* Not Aligned word ?*/

  else 
    WriteWord1(Addr & 65534, (Data<<8) | 255); /* Not Aligned word ?*/
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
  ///out_wdt_reset();

  if(Addr<65536)
  {
		if((Addr<0X4000)||(Addr>0XDFFF)||((Addr>0X7FFF)&&(Addr<0XC000)))
    	return(ERR_RANGE);                			/* Check range of address */
  } 
  SelectBank(Addr);                    			/* Select bank and page */
  PPAGE=page;
  *Data = *(volatile byte *)(Addr&0xFFFF); 	/* Read byte from FLASH */
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

  ///out_wdt_reset();
  if(Addr<65536)
  {
      if((Addr<0X4000)||(Addr>0XDFFF)||((Addr>0X7FFF)&&(Addr<0XC000)))
            return(ERR_RANGE);           /* Check range of address */
  } 
  if (Addr & 1)                        	/* Aligned address ? */
    return ERR_NOTAVAIL;
  ClearFlags();                        	/* Clear all flags */
  SelectBank(Addr);                    	/* Select bank and page */
  PPAGE=page;
  if (*(volatile word*)(Addr & 65535) != 65535) { /* Word in Flash not erased ? */
    err=IFsh1_EraseSector(Addr);       /* Erase sector */
    if (err != ERR_OK) return err;     	/* Previous operation was error ? */
  }
  if (FSTAT_CBEIF == 0)                		/* Is command buffer full ? */
    return ERR_BUSY;                   		/* If yes then error */
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
  ///out_wdt_reset();

  if(Addr<65536)
  {
  	if((Addr<0X4000)||(Addr>0XDFFF)||((Addr>0X7FFF)&&(Addr<0XC000)))
			return(ERR_RANGE);                  	/* Check range of address */
  } 
  SelectBank(Addr);                    		/* Select bank and page */
  PPAGE=page;
  *Data = *(volatile word*)(Addr&65535); 	/* Get word form Flash */
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
byte IFsh1_EraseSector(dword Addr)
{
  byte err;

  out_wdt_reset();
  if(
    (((Addr>>16)>=0xe0)&&((Addr>>16)<=0xfe)||((Addr&65535)>=0x8000)||((Addr&65535)<=0xbfff))
    ||(((Addr&0xffffff)>=0x4000)&&((Addr&0xffffff)<=0x7fff))
    ||(((Addr&0xffffff)>=0xc000)&&((Addr&0xffffff)<=0xdfff))) 
  {
    
    ClearFlags();                        	/* Clear all flags */
    SelectBank(Addr);                    	/* Select bank and page */
    PPAGE=page;
    if (FSTAT_CBEIF == 0)                		/* Is command buffer full ? */
      return ERR_BUSY;                   		/* If yes then error */
    *(volatile word *) (Addr&65535) = 10; /* Write eny word to FLASH buffer */
    FCMD = 64;                           		/* Initiate Sector Erase commamd */
    FSTAT = 128;                         		/* Launch the command */
    asm {                                		/* Jump to Wait in RAM code */
      CLRB
      JSR WaitInRAMcode
      STAB err
    }
    if (err) 
			return err;                 					/* Return error if need */
    return ERR_OK;                       		/* OK */
  } 
  else      
		return(ERR_RANGE);             					/* Check range of address */
}
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
#define	 BOOT_GENERAL	0X00
#define BOOT_CONFIG		0X01
uchar chip_erase(void)			  //chip erase
{
	
	dword address_min,address_max,address_in;
	uchar err;
	uchar add1;

	if(BootLoader_Content==BOOT_CONFIG)
	{
		if((FirstS19Address>0x7BFF)&&(FirstS19Address<0x7fFF))   ///配置擦除;旧的协议
		{
		    FirstS19Address=0x00;
			for(address_in=0x7C00;address_in<0x8000;address_in=(address_in+0x400))
				err=IFsh1_EraseSector(address_in);   	///0xfb8000-0xfbbfff =16k	
		}
		else 
		{
			address_in  = FirstS19Address;
			address_max = FirstS19Address+0x4000;
	    	for(add1=0;add1<Sector_Number;add1++)								///0xe08000-0xfbbfff
			{
				address_min=address_in;
				for(address_min;address_min<address_max;address_min=(address_min+0x400))
			  	err=IFsh1_EraseSector(address_min); 
				address_in  = address_in  + 0x10000;
				address_max  = address_max + 0x10000;	
			}
		}
	}
	else
	{	
		for(address_in=0x4000;address_in<0x8000;address_in=(address_in+0x400))
			err=IFsh1_EraseSector(address_in);   	///0x4000-0x7f00
		for(address_in=0xc000;address_in<0xe000;address_in=(address_in+0x400))
	  		err= IFsh1_EraseSector(address_in);  	///0xc000-0xdfff
	  	
		address_in  = FirstS19Address;
		address_max = FirstS19Address+0x4000;
    	for(add1=0;add1<Sector_Number;add1++)								///0xe08000-0xfbbfff
		{
			address_min=address_in;
			for(address_min;address_min<address_max;address_min=(address_min+0x400))///1个扇区为0x400；1页为0x4000
		  	err=IFsh1_EraseSector(address_min); 
			address_in  = address_in  + 0x10000;///跨页64K
			address_max  = address_max + 0x10000;	
		}
	}
	asm nop;
	return	err;
}

