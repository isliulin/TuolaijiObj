#ifndef _type_h
#define  _type_h

#define ERR_OK           		0             /* OK */
#define ERR_SPEED        	1             /* This device does not work in the active speed mode. */
#define ERR_RANGE        	2             /* Parameter out of range. */
#define ERR_VALUE        	3             /* Parameter of incorrect value. */
#define ERR_OVERFLOW     	4             /* Timer overflow. */
#define ERR_MATH         		5             /* Overflow during evaluation. */
#define ERR_ENABLED      	6             /* Device is enabled. */
#define ERR_DISABLED     	7             /* Device is disabled. */
#define ERR_BUSY         		8             /* Device is busy. */
#define ERR_NOTAVAIL     	9             /* Requested value or method not available. */
#define ERR_RXEMPTY      	10            /* No data in receiver. */
#define ERR_TXFULL       		11            /* Transmitter is full. */
#define ERR_BUSOFF       		12            /* Bus not available. */
#define ERR_OVERRUN      	13            /* Overrun error is detected. */
#define ERR_FRAMING      	14            /* Framing error is detected. */
#define ERR_PARITY       	15            /* Parity error is detected. */
#define ERR_NOISE        	16            /* Noise error is detected. */
#define ERR_IDLE         	17            /* Idle error is detectes. */
#define ERR_FAULT        	18            /* Fault error is detected. */
#define ERR_BREAK        	19            /* Break char is received during communication. */
#define ERR_CRC          	20            /* CRC error is detected. */
#define ERR_ARBITR       	21            /* A node losts arbitration. This error occurs if two nodes start transmission at the same time. */
#define ERR_PROTEC       	22            /* Protection error is detected. */

#define	 S19_DATA_NUMBER		250						///一行S19数据的大小->byte格式
#define S19_RX_MAX_SIZE 		8 						///CAN接收的最大缓冲数组数量，所占的RAM字节数：(250+8)*10=2580
#define	 PICTURE_S19_WRITE_SIZE		55		///一次写入图片S19的最大长度
typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;

typedef struct
{
   unsigned long identifier;
   uchar  data[8];
   uchar  byte_count;
}CAN_PACKET_T;

typedef struct
{
   uint   PGN;
   uchar  data[8];
   uint   byte_count;
   uchar  source_addr;  
   uchar  dest_addr;    	   
} J1939_PDU_T;

typedef struct
{
	uchar  len;
  uchar  S19data[S19_DATA_NUMBER+6];	///数据长度+最大4个地址+最大250个数据+1校验和
  uchar  function;
}_S19_BUF;

typedef union
{
	uchar data[S19_DATA_NUMBER+8];
	_S19_BUF S19Buf;
}_S19_RX_BUF;


typedef union
{
  uchar Byte;
  struct
  {
	unsigned bit0: 1;
	unsigned bit1: 1;
	unsigned bit2: 1;
	unsigned bit3: 1;
	unsigned bit4: 1;
	unsigned bit5: 1;
	unsigned bit6: 1;
	unsigned bit7: 1;
  } Bits;
}BitCommon;
extern volatile BitCommon _CanRxFlag;
#define CanRxFlag 					_CanRxFlag.Byte
#define BootStartFlag  				_CanRxFlag.Bits.bit0///开始烧录
#define ErasureStartFlag  		_CanRxFlag.Bits.bit1///擦除主芯片FLASH首地址
#define SoftRestFlag  				_CanRxFlag.Bits.bit2///软复位
#define BootStopFlag  				_CanRxFlag.Bits.bit3///终止烧写
#define RxS19LineFlag				_CanRxFlag.Bits.bit4///接收一行S19数据
#define BootReadyFlag  			_CanRxFlag.Bits.bit5///烧录准备
#define PictureBootLoadFlag	_CanRxFlag.Bits.bit6///图片烧录标志
#define Bootloader_Mode_ChangeFlag _CanRxFlag.Bits.bit7///图片烧录模式标志

extern volatile BitCommon _CanRx1Flag;
#define CanRx1Flag 						_CanRx1Flag.Byte
#define ErasurePicStartFlag  		_CanRx1Flag.Bits.bit0///擦除图片FLASH首地址
#define	 RXChecksumErrorFlag		_CanRx1Flag.Bits.bit1///接收数据检验错误


#endif

