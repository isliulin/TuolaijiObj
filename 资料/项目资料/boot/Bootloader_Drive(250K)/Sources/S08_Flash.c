#include "S08_Flash.h"

#pragma MESSAGE DISABLE C1805 
/*********************************
**********************************/
void FlashInit(void)
{
  if (!(FCDIV & 0x80))
  {
    if (FSTAT&0x30)         		///Check to see if FACCERR is set
    {
        FSTAT |= 0x30;  			///write a 1 to FACCERR to clear
    }
    FCDIV = FLASH_CLOCK;
  }
}
/**********************************************************************************************************/
/**********************************************************************************************************/
/**********************************************************************************************************/
/* This is the implementation to copy a function to RAM; Read Technical Note 228 for more information */
extern char __SEG_START_FLASH_ROUTINES[];
extern char __SEG_SIZE_FLASH_ROUTINES[];
/*
Start_Copy_In_RAM refers to the begining of the segment
ToCopyToRAM. This segment contains the functions after
they have been copied to RAM.
*/
#define Start_Copy_In_RAM __SEG_START_FLASH_ROUTINES
#define Size_Copy_In_RAM __SEG_SIZE_FLASH_ROUTINES
/*
Start_In_ROM refers to the begining of the segment
ToCopyToRAM. This segment contains the functions in ROM.
*/
void CopyInRAM(void) 
{
  char *srcPtr;
  char *dstPtr;
  int count;
  srcPtr = (char *)Start_Copy_In_RAM;
  dstPtr = (char *)&Flash_Cmd;
  for (count = 0; count < (int) Size_Copy_In_RAM;  count++, dstPtr++, srcPtr++) 
  {
    *dstPtr = *srcPtr;
  }
}
/****************************************
****************************************/
#pragma CODE_SEG FLASH_ROUTINES
unsigned char Flash_Cmd(unsigned int FlashAddress, unsigned int FlashDataCounter, unsigned char* pFlashDataPtr, unsigned char FlashCommand)
{
    /* Clear Flags if set*/
    FSTAT = 0x30;  
    do
    {
        /* Wait for the Last Burst Command to complete */
        while(!(FSTAT&0x80));
        
        /* Write Data into Flash*/
        (*((volatile unsigned char *)(FlashAddress++))) = *pFlashDataPtr;
        pFlashDataPtr++;
        /* Write Command */
        FCMD = FlashCommand;
        _asm NOP;
        _asm NOP;
        _asm NOP;
        _asm NOP;
        
        /* Put FCBEF at 1 */
        FSTAT = 0x80;
        /* Check if Flash Access Error or Protection Violation Error are Set */
        if (FSTAT&0x30)
        {     
          /* If so, finish the function returning 1 to indicate error */
          return (1);
        }
    }while (--FlashDataCounter);
    /* wait for the last command to complete */
    while ((FSTAT&0x40)==0);
    /* Return zero to indicate that the function executed Ok */
    return (0);
}
/****************************************
****************************************/
unsigned char Flash_Read(unsigned long FlashAddress)
{
	return (*((volatile unsigned char *)(FlashAddress)));
}
#pragma CODE_SEG DEFAULT

