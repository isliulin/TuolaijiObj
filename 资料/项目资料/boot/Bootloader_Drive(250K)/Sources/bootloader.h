#ifndef	_Bootloader_h
#define		_Bootloader_h

#include 	"hidef.h" 		/* for EnableInterrupts macro */
#include 	"derivative.h" /* include peripheral declarations */
#include 	"start08.h"
#include	"S08_Flash.h"
#include	"Macros.h"
#include	"Bootloader_Init.h"

#ifndef	_NO_BOOTLOADER_EXTERN
#define		_BOOTLOADER_EXTERN	extern
#else
#define		_BOOTLOADER_EXTERN
#endif
/**************funtion macros*****************/
void	Bootloader_Deal(void);
void	Bootloader_External_WatchDog(void);
void	Erase_Flash_Zero(void);
static void CANRx_Deal(void);
static void	ClearCanBuf(void);
void err_send(unsigned char data);
void CAN_SendFrame(void);
unsigned char S19Program(void);
/**************global variable macros******************/
_BOOTLOADER_EXTERN		unsigned long		ReStart_Flash_Write_Addres;///FLASH写入的地址
_BOOTLOADER_EXTERN		unsigned long		BootCanMask;///CAN ID 屏蔽的变量
_BOOTLOADER_EXTERN		unsigned char		RealyWriteFlashData[32];///实际一次写入数据的校正
_BOOTLOADER_EXTERN		CAN_PACKET_T 	rev_can_packet;///CAN总线接收数据包
_BOOTLOADER_EXTERN		unsigned char		ResponseData;///应答数据内容的变量
_BOOTLOADER_EXTERN 	unsigned char		s19RxCount;///S19接收整行长度的变量
_BOOTLOADER_EXTERN		unsigned char 		s19LineLen;	///s19数据长度的变量
_BOOTLOADER_EXTERN 	unsigned long		count_delay;///检测当前是否有CAN数据的变量
_BOOTLOADER_EXTERN  	CAN_PACKET_T   send_can_packet;///CAN总线发送数据包
_BOOTLOADER_EXTERN  	_S19_RX_BUF  	S19DataBuf;///S19接收的数据缓存
_BOOTLOADER_EXTERN  	unsigned long		ModuleID;///模块ID 变量

////===================common bit macros============================
_BOOTLOADER_EXTERN	volatile	BitCommon	_BitMacros0;
#define	FrontDriveFlag	   					_BitMacros0.Bits.bit0
#define	BackDriveFlag						_BitMacros0.Bits.bit1
#define	MainTestBoardModuleFlag	_BitMacros0.Bits.bit2
///#define	BootloaderCanRecieveFlag	_BitMacros0.Bits.bit3
#define	PowerControlModuleFlag		_BitMacros0.Bits.bit4
#define	PowerModuleFlag					_BitMacros0.Bits.bit5
#define	TopDriveFlag							_BitMacros0.Bits.bit6
#define	BootStartFlag							_BitMacros0.Bits.bit7///bootloader烧录中标志

_BOOTLOADER_EXTERN	volatile	BitCommon	_BitMacros1;
#define	ConfigFlag									_BitMacros1.Bits.bit0///是烧写程序还是配置的标志
///#define	RecordModuleFlag						_BitMacros1.Bits.bit1
#define	SlaveTestBoardModuleFlag		_BitMacros1.Bits.bit2
#define RxS19LineFlag							_BitMacros1.Bits.bit3///接收一行S19数据
/*****************IO macros******************/
#define		Bootloader_External_Wdg_Io		PTFDD_PTFDD4
#define		Bootloader_External_Wdg				PTFD_PTFD4			
/* Mask Code Definitions */				
#define MASK_CODE_ST_ID (BootCanMask << 5)
#define MASK_CODE_ST_ID_HIGH ((MASK_CODE_ST_ID&0xFF00)>>8)
#define MASK_CODE_ST_ID_LOW (MASK_CODE_ST_ID&0xFF)
/* Acceptance Code Definitions */ 
#define ACC_CODE_ID (BootCanMask << 5)   /* Standard Id */
#define ACC_CODE_ID_HIGH ((ACC_CODE_ID&0xFF00)>>8)
#define ACC_CODE_ID_LOW (ACC_CODE_ID&0xFF)
///协议中的数据功能帧定义
#define GetFunction()		rev_can_packet.data[0]
#define	 GetID()					rev_can_packet.identifier
#define GetS19Len()			rev_can_packet.data[1]
#endif

