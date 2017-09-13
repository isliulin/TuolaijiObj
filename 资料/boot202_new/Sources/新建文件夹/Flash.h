#ifndef __IFsh1
#define __IFsh1
#include <MC9S12XET256.h>
#include "type.h"
#include "can0.h"
#include "my_vectors.h"

/* MODULE IFsh1. */

//#include "Cpu.h"

extern volatile unsigned char page;
extern volatile unsigned char page_p,page_d;
extern CMD_BUF commandArray;


#pragma CODE_SEG IFsh1_CODE  

/* MODULE IFsh1. */
#define flash_flag						 (unsigned long)0x0800
//#include "Cpu.h"
#ifndef ALIGNED_WORD_MASK
#define ALIGNED_WORD_MASK	     0x0001
#define ALIGNED_PHRASE_MASK	   0x0007
#define SIZEOF_PHRASE          8
#endif

#define EraseSector  0
#define WrodProgram  1

#define CCIF_MASK               0x80
#define ACCERR_MASK             0x20
#define FPVIOL_MASK             0x10
#define CCIF_FPVIOL_MASK        0x90
#define CCIF_ACCERR_MASK        0xA0
#define CCIF_ACCERR_FPVIOL_MASK 0xB0
#define CCIF_MGSTAT1_MASK       0x82
#define CCIF_MGSTAT_MASK        0x83

#define FLASH_OK                0
#define FLASH_BUSY              1
#define FLASH_PROGRAM_ERROR     2
#define FLASH_PROTECT_ERROR     4  
#define FLASH_COMMAND_ERROR     8
#define FLASH_ACCESS_ERROR      16  

#define FLASH_TYPE_D            0
#define FLASH_TYPE_P            1

 
#define FLASH_D_PAGENUM         1
#define FLASH_D_PAGESIZE        0x1000u

#define FLASH_D_SECTSIZE        256 /* sector size of D-flash in bytes */
#define FLASH_D_SECTNUM         16


/************************* global const arrays **********************************/
 
/************************* global variables **********************************/

/**** P-Flash and D-Flash Commands ****/

#define ERASE_VERIFY_ALL_BLOCKS  0x01 
/* Verify that all program and data Flash blocks are erased. */
/* CCOBIX end = 0 */
/* CCOB Params - NONE */
/* MGSTAT set if fault */

#define ERASE_VERIFY_BLOCK       0X02      
/* Verify that a Flash block is erased. */
/* CCOBIX end = 0 */
/* CCOB Params - gpage */
/* MGSTAT set if fault */

#define ERASE_ALL_BLOCKS         0x08 
/* Erase all program and data Flash blocks.
   An erase of all Flash blocks is only possible when the FPLDIS, FPHDIS, and FPOPEN
   bits in the FPROT register and the EPDIS and EPOPEN bits in the EPROM register are
   set prior to launching the command. */
/* CCOBIX end = 0 */
/* CCOB Params - NONE */
/* MGSTAT set if fault, FPVIOL / ACCERR set where appropriate */

#define UNSECURE_FLASH           0x0B 
/*Supports a method of releasing MCU security by erasing all program and data Flash
  blocks and verifying that all program and data Flash blocks are erased. */
/* CCOBIX end = 0 */
/* CCOB Params - NONE */
/* MGSTAT set if fault */

#define SET_USER_MARGIN_LEVEL    0x0D 
/*Specifies a user margin read level for all program Flash blocks. */
/* CCOBIX end = 1 */
/* CCOB Params - gpage, level setting (0-2) in CCOB[1] */
/* ACCERR set if invalid level */

#define SET_FIELD_MARGIN_LEVEL   0x0E 
/*Specifies a field margin read level for all program Flash blocks (special modes only). */
/* CCOBIX end = 1 */
/* CCOB Params - gpage, level setting (0-4) in CCOB[1] */
/* ACCERR set if invalid level */

/* **** P-Flash Only Commands ****/

#define ERASE_VERIFY_P_FLASH_SECTION 0x03  
/*Verify that a given number of words starting at the address provided are erased. */
/* CCOBIX end = 2 */
/* CCOB Params - global address, number of phrases in CCOB[2]*/
/* MGSTAT set if fault */

#define READ_ONCE	               0x04  
/* Read a phrase from a dedicated 64 word area in a hidden region of a programFlash block
   that was previously programmed using the Program Once command. */
/* CCOBIX end = 1 */
/* CCOB Params - read once index (0-3) in CCOB[1], phrase in CCOB [5:2] */
/* returns phrase in CCOB [4:1] */

#define PROGRAM_P_FLASH          0x06 
/* Program a phrase in a program Flash block and any previously loaded phrases for any
   other program Flash block (see Load Data Field command). */
/* CCOBIX end = 5 */
/* CCOB Params - global address, phrase in CCOB [5:2] */
/* MGSTAT set if fault, FPVIOL / ACCERR set where appropriate */

#define PROGRAM_ONCE             0x07 
/* Program a dedicated 64 word area in a hidden region of a program Flash block that is
   allowed to be programmed only once. */
/* CCOBIX end = 5 */
/* CCOB Params - read once index (0-3) in CCOB[1], phrase in CCOB [5:2] */
/* MGSTAT set if fault */

#define ERASE_P_FLASH_BLOCK      0x09 
/* Erase a program Flash block.
   An erase of the full program Flash block is only possible when FPLDIS, FPHDIS and
   FPOPEN bits in the FPROT register are set prior to launching the command. */
/* CCOBIX end = 1 */
/* CCOB Params - global address */
/* MGSTAT set if fault, FPVIOL / ACCERR set where appropriate */

#define ERASE_P_FLASH_SECTOR 0x0A 
/* Erase all bytes in a program Flash sector. */
/* CCOBIX end = 1 */
/* CCOB Params - global address */
/* MGSTAT set if fault, FPVIOL / ACCERR set where appropriate */

#define VERIFY_BACKDOOR_ACCESS_KEY 0x0C 
/*Supports a method of releasing MCU security by verifying a set of security keys. */
/* CCOBIX end = 4 */
/* CCOB Params - backdoor key in CCOB [1:4] */
/* ACCERR set if not verified */

/**** D-Flash Only Commands ****/
#define ERASE_D_FLASH_BLOCK      0x09 
/* Erase a program Flash block.
   An erase of the full program Flash block is only possible when DPOPEN bit in the DFPROT
     register is set prior to launching the command. */
/* CCOBIX end = 1 */
/* CCOB Params - global address */
/* MGSTAT set if fault, FPVIOL / ACCERR set where appropriate */

#define ERASE_VERIFY_D_FLASH_SECTION 0x10 
/* Verify that a given number of words starting at the address provided are erased. */
/* CCOBIX end = 2 */
/* CCOB Params - global address of first word, number of words to verify CCOB[2]*/
/* MGSTAT set if fault */

#define PROGRAM_D_FLASH         0x11 
/* Program up to four words in the data Flash block (see Load Data Field command). */
/* CCOBIX end = 2 */
/* CCOB Params - global address, up to 4 data words in CCOB [2:5] */
/* MGSTAT set if fault, EPVIOL / ACCERR set where appropriate */

#define ERASE_D_FLASH_SECTOR    0x12 
/* Erase all bytes in a data Flash sector. */
/* CCOBIX end = 2 */
/* CCOB Params - global address */
/* MGSTAT set if fault, EPVIOL  set where appropriate */

/************************* function prototypes *******************************/


void IFsh1_Init(void);
unsigned char IFsh1_SetByteFlash(unsigned long Addr,unsigned char Data);
byte IFsh1_GetByteFlash(dword Addr,byte *Data);
byte IFsh1_SetWordFlash(dword Addr,word Data);
byte IFsh1_GetWordFlash(dword Addr,word *Data);
byte IFsh1_EraseSector(dword Addr);
void FlashInit(void);



void ClearFlags(void);

void Flash_Init(void);

void SelectBank(byte type,dword Addr);

extern byte LaunchFlashCommand(byte params, byte command, byte AddrH, uint AddrL, uint* Pdata);

extern byte FlashProgram(dword Addr,byte *PData,byte len);

extern byte EraseSectorFlash(dword Addr);

extern byte GetFlashData(dword Addr,uchar* pdata,byte len);

extern byte DFlashProgram(dword Addr,byte *PData,byte len);

extern byte PFlashProgram(dword Addr,byte *PData,byte len);

extern byte GetFlashDataSum(dword Addr,uchar* checkSum,byte len);

extern unsigned short calcrc ( char *ptr, unsigned short count );

#pragma CODE_SEG DEFAULT               /* Change code section to DEFAULT. */

/* END IFsh1. */

#endif /* ifndef __IFsh1 */
/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 03.11 for 
**     the Motorola HCS12 series of microcontrollers.
**
** ###################################################################
*/
