/*
 * COPYRIGHT (c) 2010 MACRONIX INTERNATIONAL CO., LTD
 * SPI Flash Low Level Driver (LLD) Sample Code
 *
 * SPI and PARELLEL interface command hex code, type definition and
 * function prototype.
 *
 * $Id: MX25_CMD.h,v 1.3 2010/02/23 05:30:28 benhuang Exp $
 */
#ifndef    __MX25_CMD_H__
#define    __MX25_CMD_H__

#include    "MX25_DEF.h"

/*** MX25 series command hex code definition ***/
//ID comands
#define    FLASH_CMD_RDID      0x9F    //RDID (Read Identification)
#define    FLASH_CMD_RES       0xAB    //RES (Read Electronic ID)
#define    FLASH_CMD_REMS      0x90    //REMS (Read Electronic & Device ID)
#define    FLASH_CMD_REMS2     0xEF    //REMS2 (Read ID for 2 x I/O mode)
#define    FLASH_CMD_REMS4     0xDF    //REMS4 (Read ID for 4 x I/O mode)
#define    FLASH_CMD_REMS4D    0xCF    //REMS4D (Read ID for 4 x I/O DT mode)

//Register comands
#define    FLASH_CMD_WRSR      0x01    //WRSR (Write Status Register)
#define    FLASH_CMD_RDSR      0x05    //RDSR (Read Status Register)
#define    FLASH_CMD_WRSCUR    0x2F    //WRSCUR (Write Security Register)
#define    FLASH_CMD_RDSCUR    0x2B    //RDSCUR (Read Security Register)
#define    FLASH_CMD_CLSR      0x30    //CLSR (Clear SR Fail Flags)

//READ comands
#define    FLASH_CMD_READ        0x03    //READ (1 x I/O)
#define    FLASH_CMD_2READ       0xBB    //2READ (2 x I/O)
#define    FLASH_CMD_4READ       0xEB    //4READ (4 x I/O)
#define    FLASH_CMD_FASTREAD    0x0B    //FAST READ (Fast read data).
#define    FLASH_CMD_FASTDTRD    0x0D    //FASTDTRD (1 x I/O DT read)
#define    FLASH_CMD_2DTRD       0xBD    //2DTRD (Dual DT read) BD
#define    FLASH_CMD_4DTRD       0xED    //4DTRD (Quad DT read)
#define    FLASH_CMD_RDDMC       0x5A    //DMCRD (Read DMC)

//Program comands
#define    FLASH_CMD_WREN     0x06    //WREN (Write Enable)
#define    FLASH_CMD_WRDI     0x04    //WRDI (Write Disable)
#define    FLASH_CMD_PP       0x02    //PP (page program)
#define    FLASH_CMD_4PP      0x38    //4PP (Quad page program)
#define    FLASH_CMD_CP       0xAD    //CP (Continously program)

//Erase comands
#define    FLASH_CMD_SE       0x20    //SE (Sector Erase)
#define    FLASH_CMD_BE32K    0x52    //BE32K (Block Erase 32kb)
#define    FLASH_CMD_BE       0xD8    //BE (Block Erase)
#define    FLASH_CMD_CE       0x60    //CE (Chip Erase) hex code: 60 or C7

//Mode setting comands
#define    FLASH_CMD_DP       0xB9    //DP (Deep Power Down)
#define    FLASH_CMD_RDP      0xAB    //RDP (Release form Deep Power Down)
#define    FLASH_CMD_ENSO     0xB1    //ENSO (Enter Secured OTP)
#define    FLASH_CMD_EXSO     0xC1    //EXSO  (Exit Secured OTP)
#define    FLASH_CMD_ESRY     0x70    //ESRY (Enable SO to output RY/BY)
#define    FLASH_CMD_DSRY     0x80    //DSRY (Enable SO to output RY/BY)
#define    FLASH_CMD_WPSEL    0x68    //WPSEL (Enable block protect mode)
#define    FLASH_CMD_ENPLM    0x55    //ENPLM( Enter Parallel Mode )
#define    FLASH_CMD_EXPLM    0x45    //EXPLM( Exit Parallel Mode )

//Reset comands

//Security comands
#define    FLASH_CMD_SBLK       0x36    //SBLK (Single Block Lock)
#define    FLASH_CMD_SBULK      0x39    //SBULK (Single Block Unlock)
#define    FLASH_CMD_RDBLOCK    0x3C    //RDBLOCK (Block Protect Read)
#define    FLASH_CMD_GBLK       0x7E    //GBLK (Gang Block Lock)
#define    FLASH_CMD_GBULK      0x98    //GBULK (Gang Block Unlock)

//Suspend/Resume comands

// Return Message
typedef enum {
    FlashOperationSuccess,
    FlashWriteRegFailed,
    FlashTimeOut,
    FlashIsBusy,
    FlashQuadNotEnable,
    FlashAddressInvalid,
}ReturnMsg;

// Flash status structure define
struct sFlashStatus{
    /* Mode Register:
     * Bit  Description
     * -------------------------
     *  7   RYBY enable
     *  6   Reserved
     *  5   Reserved
     *  4   Reserved
     *  3   Reserved
     *  2   Reserved
     *  1   Parallel mode enable
     *  0   QPI mode enable
    */
    uint8    ModeReg;
    BOOL     ArrangeOpt;
};

typedef struct sFlashStatus FlashStatus;

/* Basic functions */
void CS_High();
void CS_Low();
void InsertDummyCycle( uint8 dummy_cycle );
void SendByte( uint8 byte_value, uint8 transfer_type );
uint8 GetByte( uint8 transfer_type );

/* Utility functions */
void Wait_Flash_WarmUp();
void Initial_Spi();
BOOL WaitFlashReady( uint32 ExpectTime, FlashStatus *fsptr );
BOOL WaitRYBYReady( uint32 ExpectTime );
BOOL IsFlashBusy( FlashStatus *fsptr );
BOOL IsFlashPAR( FlashStatus *fsptr );
BOOL IsFlashQIO( FlashStatus *fsptr );
BOOL IsFlash4Byte( void );
void SendFlashAddr( uint32 flash_address, uint8 io_mode, uint8 addr_mode );

/* Flash commands */
ReturnMsg CMD_RDID( uint32 * Identification, FlashStatus *fsptr );
ReturnMsg CMD_RES( uint8 * ElectricIdentification, FlashStatus *fsptr );
ReturnMsg CMD_REMS( uint16 * REMS_Identification, FlashStatus *fsptr );
ReturnMsg CMD_REMS2( uint16 * REMS_Identification, FlashStatus *fsptr );
ReturnMsg CMD_REMS4( uint16 * REMS_Identification, FlashStatus *fsptr );
ReturnMsg CMD_REMS4D( uint16 * REMS_Identification, FlashStatus *fsptr );

ReturnMsg CMD_RDSR( uint8 *StatusReg, FlashStatus *fsptr );
ReturnMsg CMD_WRSR( uint8 UpdateValue, FlashStatus *fsptr );
ReturnMsg CMD_RDSCUR( uint8 * SecurityReg );
ReturnMsg CMD_WRSCUR( FlashStatus *fsptr );
ReturnMsg CMD_CLSR( void );

ReturnMsg CMD_READ( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
ReturnMsg CMD_2READ( uint32 flash_address, uint8 *target_address, uint32 byte_length );
ReturnMsg CMD_4READ( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
ReturnMsg CMD_FASTREAD( uint32 flash_address, uint8 *target_address, uint32 byte_length );
ReturnMsg CMD_FASTDTRD( uint32 flash_address, uint8 *target_address, uint32 byte_length );
ReturnMsg CMD_2DTRD( uint32 flash_address, uint8 *target_address, uint32 byte_length );
ReturnMsg CMD_4DTRD( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
ReturnMsg CMD_RDDMC( uint32 flash_address, uint8 *target_address, uint32 byte_length );

ReturnMsg CMD_WREN( void );
ReturnMsg CMD_WRDI( void );
ReturnMsg CMD_PP( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr );
ReturnMsg CMD_4PP( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr );
ReturnMsg CMD_CP( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr );

ReturnMsg CMD_SE( uint32 flash_address, FlashStatus *fsptr );
ReturnMsg CMD_BE32K( uint32 flash_address, FlashStatus *fsptr );
ReturnMsg CMD_BE( uint32 flash_address, FlashStatus *fsptr );
ReturnMsg CMD_CE( FlashStatus *fsptr );

ReturnMsg CMD_DP( void );
ReturnMsg CMD_RDP( void );
ReturnMsg CMD_ENSO( void );
ReturnMsg CMD_EXSO( void );
ReturnMsg CMD_ESRY( FlashStatus *fsptr );
ReturnMsg CMD_DSRY( FlashStatus *fsptr );
ReturnMsg CMD_WPSEL( FlashStatus *fsptr );
ReturnMsg CMD_ENPLM( FlashStatus *fsptr );
ReturnMsg CMD_EXPLM( FlashStatus *fsptr );

ReturnMsg CMD_SBLK( uint32 flash_address, FlashStatus *fsptr );
ReturnMsg CMD_SBULK( uint32 flash_address, FlashStatus *fsptr );
ReturnMsg CMD_RDBLOCK( uint32 flash_address, BOOL * protect_flag );
ReturnMsg CMD_GBLK( FlashStatus *fsptr );
ReturnMsg CMD_GBULK( FlashStatus *fsptr );
#endif    /* __MX25_CMD_H__ */
