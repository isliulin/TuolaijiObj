 #ifndef	_typedef_h
#define	_typedef_h
/****************************************/
#include "arm_cm4.h"

#define GENPTR
#define GENPTR_B
#define	EXT extern
#define ROMCONST       const 

//ap: long 32bit
typedef 	unsigned 	char		u8;	  //无符号型
typedef 	unsigned 	short int	u16;
typedef 	unsigned 	long  int	u32;
#define  BOOLEAN  unsigned char
#define	 BYTE	unsigned char	
#define  INT8   signed char
#define  INT16  short
#define  INT32  long 
#define  UINT8  unsigned char
#define  UINT16  unsigned short
#define  UINT32  unsigned long 
typedef unsigned char uchar;
typedef unsigned int short uint; 	//ap:移植16位单片机字节兼容
typedef unsigned long ulong;

//#define STATUS            unsigned char
///tft define	
#define	WORD	unsigned short	
#define word	WORD
#define	DWORD	unsigned long	
#define dword	DWORD
#define	bit unsigned char
#define	CODE const
#define FALSE_            0x01
#define TRUE_             0x00
#define CANBUF_MAXSIZE    0x0A   
#define PCDAT_MAXSIZE     0x30
#define _between(x, a, b)	(a<=x && x<=b)
#define _swap(a, b)			{ a^=b; b^=a; a^=b; }

//#define	TRUE	1
//#define	FALSE	0


//VInputStd
#define NTSC	1			
#define PAL		2
#define SECAM	3
#define NTSC4	4
#define PALM	5
#define PALN	6
#define PAL60	7

#define UNKNOWN	0xfe
#define NOINPUT	0	//0xff

#define NIL			0xff
/****************************************/

#define byte BYTE
typedef	struct
{
	byte	second;
	byte	minute;
	byte	hour;
	byte	week;
	byte	date;
	byte	month;
	byte	year;		
}M4100_TIME;
/****************************************/
typedef	union
{
	struct
	{
		unsigned short		bit0:1;
		unsigned short		bit1:1;
		unsigned short		bit2:1;
		unsigned short		bit3:1;
		unsigned short		bit4:1;
		unsigned short		bit5:1;
		unsigned short		bit6:1;
		unsigned short		bit7:1;	
		unsigned short		bit8:1;
		unsigned short		bit9:1;
		unsigned short		bit10:1;
		unsigned short		bit11:1;
		unsigned short		bit12:1;
		unsigned short		bit13:1;
		unsigned short		bit14:1;
		unsigned short		bit15:1;	
	}bits;
	unsigned	int	Data;	
}LoadStruct;
/****************************************/
typedef struct
{
   uint   PGN;
   uchar  data[8];
   uint   byte_count;
   uchar  source_addr;  
   uchar  dest_addr;    	   
} J1939_PDU_T;
/****************************************/
typedef struct
{
    ulong  identifier;
    uchar  data[8];
    uchar  byte_count;
}CAN_PACKET_T;
#define J1939_FRAME_NUM       70
/*****************************************************************************/
typedef struct
{
    UINT8 amount;
    UINT8 rear;
    UINT8 front;//读
    CAN_PACKET_T data[J1939_FRAME_NUM];
}J1939_FRAME_STRUCT;
/****************************************/
typedef	union
{
    struct
    {
    	unsigned short        DtH:1;	
    	unsigned short        Dp:2;
    	unsigned short        S0:5;             
    	unsigned short        Sa:4;
    	unsigned short        DtL:4;      
    }b;
    UINT16   ID;
}CanIDStruct;
/****************************************/
typedef	struct  
{
	UINT16  CanID;
	unsigned char Priority;
	unsigned char Data[8];
	unsigned char NoOfBytes;	
	unsigned Ext:1;
	unsigned Remote:1;
}CanMessageStruct;
/*******************************/
typedef	union
{
  struct
  {
    unsigned char         Key0:1;           
    unsigned char         Key1:1;
    unsigned char         Key2:1;           
    unsigned char         Key3:1;
    unsigned char         Key4:1;           
    unsigned char         Key5:1;
    unsigned char         Key6:1;           
    unsigned char         Key7:1;
  }b;
  unsigned char      Data;
}SelfKeyStruct;

typedef	union
{
  struct
  {
    unsigned char         led0:1;           
    unsigned char         led1:1;
    unsigned char         led2:1;           
    unsigned char         led3:1;
    unsigned char         led4:1;           
    unsigned char         led5:1;
    unsigned char         led6:1;           
    unsigned char         led7:1;
  }b;
  unsigned char      Data;
}OsdLedStruct;



/**************************************/
typedef	union
{
	struct
	{
		unsigned	char	Load0:1;
		unsigned	char	Load1:1;
		unsigned	char	Load2:1;
		unsigned	char	Load3:1;
		unsigned	char 	Load4:1;
		unsigned	char 	Load5:1;
		unsigned	char 	Load6:1;
		unsigned	char 	Load7:1;	
	}Loads;
	unsigned	char	Load;	
}PutLoadStruct;
/******************************/
typedef	union
{
    struct
    {
	    unsigned	char         S0:2;             
	    unsigned	char         S1:2;             
	    unsigned	char         S2:2;             
	    unsigned	char         S3:2;             
    }St;
       
    unsigned char    Data;
}LoadStStruct;

/*************************************************

*************************************************/
typedef union
{
    struct
    {
        unsigned	char     d0:2;
        unsigned	char     d1:2;
        unsigned	char     d2:2;
        unsigned	char     d3:2;
    }d;
    unsigned  char  diag;
}DIAGNOSE_STATUS_STRUCT;
/***************************/
typedef	union 
{
    struct
    {
    unsigned	char         ST0:1;             
    unsigned	char         ST1:1;             
    unsigned	char         ST2:1;                
    unsigned	char         ST3:1;             
    unsigned	char         ST4:1;             
    unsigned	char         ST5:1;             
    unsigned	char         ST6:1;             
    unsigned	char         ST7:1;                                  
    }b;
    unsigned     char    Data;
}StStruct;
typedef	union
{
  struct
  {
    unsigned	char         bit0:1;           
    unsigned	char         bit1:1;
    unsigned	char         bit2:1;           
    unsigned	char         bit3:1;
    unsigned	char         bit4:1;           
    unsigned	char         bit5:1;
    unsigned	char         bit6:1;           
    unsigned	char         bit7:1;
  }b;
  unsigned char      Data;
}Tw88xxIIcStruct;

typedef	union
{
  struct
  {
    unsigned	char         disFlag:1;           
    unsigned	char         bit1:1;
    unsigned	char         bit2:1;           
    unsigned	char         bit3:1;
    unsigned	char         bit4:1;           
    unsigned	char         bit5:1;
    unsigned	char         bit6:1;           
    unsigned	char         bit7:1;
  }b;
  unsigned char      Data;
}TftStruct;


typedef	union
{
    struct
    {
      	unsigned	char        cnt:6; 
      	unsigned	char        cf:1;
    	unsigned	char        valid:1;		                 
    }b;
    unsigned char   dat;
}J1939OverDealStruct;

/****************************************/
typedef	union
{
    struct
    {
    	unsigned	char    Request:2;	
    	unsigned	char    Work_Mode:1;
    	unsigned	char    Error_Mode:2;             
    	unsigned	char    Basic_State:3;     
    }b;
    unsigned char Data;
}DMC_STATE;
/****************************************/
typedef	union
{
    struct
    {
    	unsigned	char    Basic_State:2;
    	unsigned	char    Error_Mode:6;          	     
    }b;
    unsigned char Data;
}BMS_STATE;

typedef struct
{
    UINT16 data[4];
    UINT8 point;
    UINT8 flag;
}SPEED_DATA_BUF_STRUCT;






/*typedef struct
{
    UINT16 pgn;        //PGN
    UINT8  location;   //该字节所在报文中的位置，即第几个字节
    UINT8  startBit;   //所在位
    UINT8  bitLen;     //位长度
    UINT8  keyCode;
    UINT8  attribute;
    UINT8  keyIndex;
}J1939_BIT_STATUS_STRUCT;
*/















 


//接收J1939中的ASCII。
//本设计最多可以接收24个字符
//因此用3个PGN来表示
/*typedef struct
{
    UINT16 pgn;
    UINT8 location;
    UINT8 len;
}J1939_ASCII_STRUCT;
*/

/*typedef struct
{
    J1939_ASCII_STRUCT  ascii[3];
}J1939_ASCII_BUF_STRUCT;
*/
typedef struct
{
    UINT16 offset;     //字节的偏移量
    UINT8  bitValue;   //位值   
}INSIDE_PHYS_STRUCT;


typedef struct
{
    UINT32 offset;     //字节的偏移量
    UINT8  bitValue;   //位值   
}INSIDE_PHYS_32BIT_STRUCT;
typedef struct
{
    UINT8 text[21];
    UINT8  attribute[3];
}MAIN_PAGE_INFO_STRUCT;

typedef struct
{
    UINT8 text[67];     //显示内容
    UINT8 attribute[6]; //属性
}LCD_INFO_STRUCT;

typedef struct
{
    LCD_INFO_STRUCT info[24];
    UINT8 line;
}LCD_INFO_SCREEN_STRUCT;
typedef struct
{
    UINT8 statusCode; //状态码
    UINT8 text[21];   //状态内容
}LCD_STATUS_STRUCT;






//首页显示柱状参数结构体。
typedef struct 
{
//    UINT8 dot;            //小数点位置
    UINT16 alarmH;        //报警上限
    UINT16 alarmL;        //报警下限
    UINT16 base;          //显示参数最小值
    UINT16 max;           //显示参数最大值
    UINT8 type;
    UINT8 source;         //数据源。
}COLUMN_PARAMETER;
                                                           //    |<-       screenLen       ->|
typedef struct                                             //     ___________________________
{                                                          //    |                           |
    UINT8 picture;        //图片数据首地址                       |offset|<- len     ->|      |
    UINT8 colour;         //图片的颜色                           |   _   _____________       |
    UINT8 len;            //图片的长宽                           |      |             |      |
    UINT8 wide;           //图片的宽                             | wide |             |      |
    UINT8 screenLen;      //显示屏幕的长度                       |   _  |_____________|      |
    UINT8 offsetL;        //图片首地址在屏幕中的偏移量           |                           |
    UINT8 offsetH;        //图片的垂直偏移量              //     |___________________________|
}PICTURE_STRUCT;    

typedef struct 
{
    UINT8 disBuf[60];     //页面名称，即菜单下的名称
    UINT8 page;           //页面所对应的页面号
}DIS_PAGE_STRUCT;

typedef struct 
{
    DIS_PAGE_STRUCT menu[15];   //菜单目录
    UINT8 maxMenu;           //最大目录数
}MENU_PAGE_STRUCT; 

typedef struct 
{
    DIS_PAGE_STRUCT menu[6];   //菜单目录
    UINT8 maxMenu;           //最大目录数
}DIAGNOSE_MENU_PAGE_STRUCT;    

typedef struct 
{
    UINT16 gb2312;        //字符的gb2312码
    UINT8 addr;           //字符在字库中的首地址
}CHAR_ATTRIBUTE_STRUCT; 

typedef	struct //union
{
/*	struct
	{
		unsigned  OC:7;
		unsigned  CM:1;	
		unsigned  FMI:5;
		unsigned  SPN_High_Bit:3;
		unsigned  SPN_Low_Bit:16;
	}Bit;  */
	unsigned long	Data;
}ENGINEDIAGNOSEDATA_STRUCT;





/*typedef struct
{
    void (*construct)(void);
    UINT8 (*msgProc)(LCD_MSG *pMsg);
    void (*deconstruct)(void);
}LCD_INTERFACE; */
typedef	union 
{
    struct
    {
	    unsigned char         protectLamp:2;             
	    unsigned char         amberLamp:2;             
	    unsigned char        redLamp:2;                
	    unsigned char         milLamp:2;                                           
    }b;
    unsigned     char    data;
}DIAGNOSE_LAMP_STRUCT;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef struct image_info_s {
	BYTE lut_type;	///	//1:luts(byte pointer method.default on TW8835MENU), 0:lut(address method)
	BYTE rle;				//upper:bpp bottom:rle_counter.
	WORD dx,dy;			//width, height
} image_info_t;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef struct image_item_info_s {
	BYTE type;	//0:LUT+image, 1:header+lut+image 2:image_only 3: lut_only 4:header_only	
	DWORD loc;  //absolute location on SpiFlash
	image_info_t *info;
	BYTE alpha;		//alpha index. 0xFF:ignore.
} image_item_info_t;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef struct image_header_info_s {
	DWORD lut_loc;
	DWORD image_loc;

	BYTE lut_type;///	//1:luts(byte pointer method.default on TW8835MENU), 0:lut(address method)
	BYTE bpp;
	BYTE rle;		//upper:bpp bottom:rle_counter.
	WORD dx,dy;		//width, height
	WORD lut_size;	//we don't need it. we can assume LUT size & image size.
} image_header_info_t;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef struct menu_item_info_s {
	BYTE osd_type;				//0:SpiOSD 1:FontOSD..failed. not using

	void (*p)(void);			//linked function

	WORD x,y;					//start x, start y
	BYTE align;					//align type

	//osd_type_meun_item_t osd;				//BYTE up,left,down,right;	//navi_key
	//struct sosd_menu_item_s	osd_s;
	struct image_item_info_s *image;
	struct image_item_info_s *image1;
	BYTE image_type;			//0:Background, icon, ....., fSOD info

	BYTE win;					//upper:fosd, bottom:sosd
	WORD lut;					//lut start offset	   1110207 become 9bit
	BYTE up,left,down,right;	//navi_key
} menu_item_info_t;

/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
struct RLE2_HEADER {
    unsigned char op_bmpbits;
    unsigned char rledata_cnt;

    unsigned short w;
    unsigned short h;

    unsigned long  size;
    
    unsigned char LUT_format; // bit0- 0:RGBa, 1:BGRa, bit1- 0:LUTS, 1:LUT;
    unsigned char LUT_colors; // 0xff:256 colors,  0x7F:128 colors,...
    unsigned char dummy0;
    unsigned char dummy1;
};
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
 typedef struct {    
     DWORD start;    
     DWORD length;   
 } SLIDEIMAGE;       
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef struct menu_item_info{
		UINT16	Menu_X;
		UINT16	Menu_Y;
		UINT16	Menu_Lut;
}Menu_Item_Info;
/***************************************************************************
一、	功能描述：
1、
二、	引用操作方式：
1、
***************************************************************************/
typedef struct FONT_SPI_INFO_s {
	DWORD loc;
	WORD size;				//curr max 10*1024 = 2800h
	BYTE width,height;
	WORD max;
	WORD bpp2;
	WORD bpp3;
	WORD bpp4;
} FONT_SPI_INFO_t;

typedef struct
{
	UINT16 flag;            //标志位，用于指示有无信息。
	// UINT8 interfaceCnt;
	// UINT8 menuPageCnt;  
	//UINT8 menuPagechange;  //页改变标志                      
	UINT8 cursor;          //保存第一菜单的选中位置。
	UINT8 menuOne;         //保存一级菜单光标位置
	UINT8 menuTwo;         //保存二级菜单位置  
	/**************诊断参数************************/ 
	UINT8 moduleID;         //模块 
	UINT8 dispLenght;       //诊断显示个数

	UINT16 dataTemp;         //初始化为0xff
	UINT8  dataCnt;          //解决osd与图片不一致
	UINT8 interfaceCnt;     //作为时间设置 与故障查询用
	TftStruct status;
	/**************多页参数************************/ 	  
	UINT8 osdPage;          //当前页
	UINT8 pageCnt;          //页计数
	UINT8 picPage;          //图片更新
	UINT8 curPage;          //当前页
	  
}LCD_MSG; 
/*typedef struct
{
    UINT16 flag;            //标志位，用于指示有无信息。
    UINT8 interfaceCnt;
	  UINT8 menuNum;
	  UINT8 diagnoseMenuNum;
	  UINT8 index1;           //保存第一菜单的选中位置。
	  UINT8 index2;           //保存第二菜单的选中位置。
}LCD_MSG;  */
/*********************************************************************/


/*********************************************************************

*********************************************************************/





typedef struct
{
#define FilterBufferMax   10
  UINT16    SensorAd[FilterBufferMax];       //逻辑模式mode=1数字 moder = 2字母
  UINT8     Sensorindex;
  UINT16    SensorVal;
  UINT8     SensorCnt;
}StructFilter;

 

                                 


/*********************************************************************

*********************************************************************/
typedef struct
{
    UINT8 x;                  //起始坐标x   
    UINT8 y;                  //起始坐标y
   // UINT8 type;
    UINT8 index;
    UINT8 szUnit[5];
    UINT8 location;           //高4位表示在哪个窗口，低4位表示在哪一页。
}OSD_PARAM_STRUCT;
/*********************************************************************

*********************************************************************/


typedef struct
{
    UINT8   name[20];    
    UINT32  address;
    UINT16  size;
    UINT16  edition;   //
}PC_CONFIG_ADDRESS_STRUCT;  
/*********************************************************************

*********************************************************************/

/*********************************************************************

*********************************************************************/


typedef struct
{
    void (*construct)(void);
    UINT8 (*msgProc)(LCD_MSG * pMsg);
    void (*deconstruct)(void);
}LCD_INTERFACE;


typedef struct
{
	UINT32 dstIdentifier;   //发送ID
	UINT32 sendCycle;
	UINT32 sendCycleBackup;
	UINT8 Flag;
}Time_Info;








typedef struct
{
	UINT8 loadDestSrcSt;
	UINT8 openShortCnt;
}St_LoadDealInfo;



#define	DRIVE_LOAD_NUM											5		///5*4=20
#define	POWER_LOAD_NUM											5		///5*4=20
#define	POWER_FUSE_NUM											6//6
#define	FIRST_ERROR_NUM											20	///20*8
#define	NUM_CAT4016							1



#define COP_k_OK      0     /* no error      */
#define COP_k_ERR     1     /* error occured */
#define COP_k_NO      2     /* CAN Open internal;described in function */

#define COP_k_ReStore        0
#define COP_k_STORE_COM_PARA 1
#define COP_k_STORE_APP_PARA 2
#define COP_k_STORE_MAN_PARA 4

#define COP_k_NO_SUBINDEX                   3
#define COP_k_UNSUPPORTED_ACCESS            4
#define COP_k_RANGE_OVERFLOW                5
#define COP_k_RANGE_UNDERFLOW               6
#define COP_k_UNKNOWN_DATATYPE              7
#define COP_k_DATATYPE_LENGTH_TO_HIGH       8
#define COP_k_DATATYPE_LENGTH_TO_LOW        9
#define COP_k_NO_MAPPING                    10
#define COP_k_PDO_TOO_LONG                  11
#define COP_k_INVALID_VALUE                 12
#define COP_k_WRONG_TOGGLEBIT               13
#define COP_k_NO_OBJECT                     14 
#define COP_k_LOCAL_ERROR                   15
#define COP_k_SERVICE_ERR                   16
#define COP_k_LOAD_ERR                      17
#define COP_k_SAVE_ERR                      17
#define COP_k_GEN_INT_ERR                   18
#define COP_k_RES_NOT_AVAIL                 19
#define COP_k_SERVICE_NOT_EXECUTED          20
#define COP_k_LOCAL_CONTROL                 21
#define COP_k_PRESENT_DEVICE_STATE          22
#define COP_k_RANGE_EXCEEDED                23
#define COP_k_INVALID_SEQ_NUM               24
#define COP_k_INVALID_BLK_SIZE              25
#define COP_k_CRC_ERR                       26
#define COP_k_UNKNOWN_SPECIFIER             27
#define COP_k_WRITE_ONLY                    28
#define COP_k_READ_ONLY                     29
#define COP_k_OUT_OF_MEMORY                 30
#define COP_k_SDO_BEFORE_WRITE_MAX_OBJLEN   8 

#define DASHBOARD_NODE_ID           1
#define FRONT_DRIVER_NODE_ID        2
#define TOP_DRIVER_NODE_ID          4
#define BACK_DRIVER_NODE_ID         3
#define MIDDLE_DRIVER_NODE_ID       6
#define POWER_MODULE_NODE_ID        5
#define POWER_CONTOROL_NODE_ID      9
#define CONFIG_VERSION_ID           0x80


/*Default Node number (could be changed if LSS_SLAVE is 1)*/
#define NODEID     DASHBOARD_NODE_ID //(1...127)

/****************************************************************************
     Indicator for Multiplexed PDO
****************************************************************************/

#define COP_k_SOURCE       0x00 //SAM-MPDO
#define COP_k_DESTINATION  0x01//DAM-MPDO

/****************************************************************************
 possible Store Signature
****************************************************************************/
#define COP_k_ReStore        0
#define COP_k_STORE_COM_PARA 1
#define COP_k_STORE_APP_PARA 2
#define COP_k_STORE_MAN_PARA 4

#define COP_k_LOAD    1     /* parameter loaded*/
#define COP_k_DELETE  2     /* parameter reset*/
#define COP_k_LOADCOM 3     /* parameter load but only com. profile */




typedef struct {
    SelfKeyStruct   DashboardKey[7]; //
    SelfKeyStruct   FrontDriveKey[3];
    SelfKeyStruct   TopDriveKey[3];
    SelfKeyStruct   BackDriveKey[3];
    SelfKeyStruct   MiddleDriveKey[3]; 
    SelfKeyStruct   PowerDriveKey[1]; 
    SelfKeyStruct   PowerContorolKey[1];
    
    SelfKeyStruct   DashboardVirtualKey[20];//
    SelfKeyStruct   FrontDriverVirtualKey[4];
    SelfKeyStruct   BackDriverVirtualKey[4];
    SelfKeyStruct   MiddleDriverVirtualKey[4];
    SelfKeyStruct   TopDriverVirtualKey[4];
    SelfKeyStruct   PowerVirtualKey[5];   
    SelfKeyStruct   PowerContorolVirtualKey[3];
    
    LoadStStruct    DashboardLoad[1];
	LoadStStruct    DashboardPWM[1];
	
    LoadStStruct    MiddleDriveLoad[DRIVE_LOAD_NUM];
    LoadStStruct    FrontDriveLoad[DRIVE_LOAD_NUM];
    LoadStStruct    BackDriveLoad[DRIVE_LOAD_NUM]; 
    LoadStStruct    TopDriveLoad[DRIVE_LOAD_NUM];
    //LoadStStruct    BackDriveLoad[DRIVE_LOAD_NUM];
   // LoadStStruct    MiddleDriveLoad[DRIVE_LOAD_NUM];
    LoadStStruct    PowerLoadState[POWER_LOAD_NUM];
    LoadStStruct    PowerFuse[POWER_FUSE_NUM];     //  30
    LoadStStruct    PowerContorolLoadState[4];

   // uchar           FrontSensorData[7];
   // uchar           TopSensorData[7];
   // uchar           BackSensorData[7];
   // uchar           MiddleSensorData[7];
  //  uchar           PowerSensorData[3];       //31
//RainScratchWaitAckFlag on Power module or on FrontDrive see  ISR.c CAN0rxd_int 
//delete echosong20110504 uchar           RainScratchWaitAck;
//do some fileter and move the motor. Speed_Data_Flag and SleepFlag. see: ISR.c CAN0rxd_int
    UINT16          CarSpeedFromDriver;
    UINT16          engSpeedFromDriver;
    UINT16          engSpeedReceive;
    UINT8           DashboardVersion[8];
    UINT8           frontDriverVersion[8];
    UINT8           backDriverVersion[8];
    UINT8           topDriverVersion[8];
    UINT8           middleDriverVersion[8];
    UINT8           powerVersion[8];
    UINT8           PowerContorolVersion[8];
    UINT8           configVersion[8];
//200ms & event
    UINT8          cycle_capture_num;
    UINT16         speed_Rate;

//250ms
  //  UINT16          engineSpeed;
    UINT8           oilPress;
    UINT8           waterTemp;
    UINT8           dashboardTemp;
    UINT8           carSpeed;
//250ms
    UINT16          fuelRate;
    UINT32          totalConsume;  
    UINT32          HundredKilometerFuelConsume;  

    UINT32          relativeDistance;
    UINT32          totalDistance;
    UINT16          batteryValtage;
//event
    UINT8           SleepCodeBuf;
    UINT8           rain_scr;
    UINT8           FlashCodeBuf;
//200ms & event
    LoadStruct      SendMeterLedArry[NUM_CAT4016];//2bytes
//    SelfKeyStruct   DashboardShortOpenKey;

    
    ///TPDODATA_END
}USR_t_ApplicationObject;


typedef struct {
    LoadStStruct    DashboardLoad;
    LoadStStruct    MiddleDriveLoad[DRIVE_LOAD_NUM];
    LoadStStruct    FrontDriveLoad[DRIVE_LOAD_NUM];
    LoadStStruct    BackDriveLoad[DRIVE_LOAD_NUM]; 
    LoadStStruct    TopDriveLoad[DRIVE_LOAD_NUM];
    LoadStStruct    PowerLoadState[POWER_LOAD_NUM];
    LoadStStruct    PowerContorolLoadState[4];
}USR_LOAD_STATUS;
//#pragma push
//#pragma align off
typedef struct{
   /*pay attention to the follow veriable sequence,refer ot Eeprom.h i.e. MiddleDoorVideoChannelAddr*/
   dword	RelativeDistance;
   dword	TotalDistance;	
   uchar	RShieldVideoChannel;
   uchar  MiddleDoorVideoChannel;
   uchar  FrontDoorVideoChannel;
   uchar  TopVideoChannel;
   uchar	Wippers;
   uchar  LightLevel;
   uchar  BuzzerLevel;
   uchar  PictureCfg[4];
   uchar  maintain;
   word   CRC8;
   //uchar 	PictureContrastNum;
   //uchar 	PictureBrightNum;
   //uchar 	PictureChromaNum;
   //uchar 	PictureDefinitionNum;
}USR_t_DeviceConfigrationData;

#define MAIN_MANU_LEFT_BAR_FULE         1//default

#define MAIN_MANU_RIGHT_BAR_BATTERY     2//default

#define MAIN_MANU_WINDOW4_CONTENT_LOGO  1
#define MAIN_MANU_WINDOW4_CONTENT_ECAR  2     

typedef struct {
    UINT8  ConfigrationFlashErase;
    UINT16 CRC16;
/*    UINT8  cycleCaptureNum;
    UINT16   speedRate;
    UINT8  SwitchLogicConfig[6];

    UINT8  oilMassWarnThreshold;
    UINT8  frontAirPressWarnThreshold;
    UINT8  backAirPressWarnThreshold;
    UINT8  waterTemperatureWarnThreshold;
    UINT8  oilPressWarnThreshold;
    UINT8  carSpeedWarnThreshold;
    UINT16 engineSpeedWarnThreshold;
    UINT8  openMask;
    UINT8  shortMask;
    UINT8  moduleConfig;
    UINT8 carSpeedSrc;
    UINT8 oilMassScr;
    UINT8 frontAirPressScr;
    UINT8 backAirPressScr;       
    UINT8 dashBoardSpeedChannel;  */
}USR_t_ManufactureConfigrationData;

typedef	union
{
	struct
	{
		unsigned char	bit0:1;
		unsigned char	bit1:1;
		unsigned char	bit2:1;
		unsigned char 	bit3:1;
		unsigned char 	bit4:1;
		unsigned char	bit5:1;
		unsigned char	bit6:1;
		unsigned char	bit7:1;	
	}bits;
	unsigned	char	Byte;	
}ByteTo1bit;

typedef union
{
	byte Byte;
	struct
	{
		unsigned char bit0: 1;
		unsigned char bit1: 1;
		unsigned char bit2: 1;
		unsigned char bit3: 1;
		unsigned char bit4: 1;
		unsigned char bit5: 1;
		unsigned char bit6: 1;
		unsigned char bit7: 1;
	} Bits;
}BitCommon;
//----------------------------





////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//配置数据结构体
#pragma   pack(1)

typedef struct
{
 UINT16   ModuleConfig;//0b00101011;//从第0位到第7位分别为前模块，后模块，中模块，顶模块，配电，高CAN模块  电源管理
 UINT16   speedChanelIndex;// = 20;
 unsigned char  CarSpeedSrc; //nbapo:车速来源            
 UINT16   engSpeedChanelIndex;// = 20;
 unsigned char  EngCarSpeedSrc;
 UINT16   SpeedRate;      //2200
 unsigned char  CycleCaptureNum ;  //8
 unsigned char  DashBoardSpeedChannel;   //0
 unsigned char  DashBoardEngSpeedChannel ;  //0
 UINT16   disLoadWarn[5];//L:高8位模块，b7-6:图片报警方式，b5-b0索引
 unsigned char  startLogoTime;      //s
 unsigned char  pwm_load_out;       //配置PWM位负载输出
 unsigned char  post;              //1->开机自检      0->无
 
 UINT16 OilPressure ; //机油压力j1939来源索引
 UINT16 WaterTemperature ; //水温 j1939来源索引
 UINT16 UremicLevel;       //尿素液位 j1939来源索引
 unsigned char   selectPowerCtr;  //1->pd205  其他 pd202
 
  UINT8   J1939CompareType0;         //高四位 0x5有效  低四位 1->最大值 2->最小值 
  UINT16  J1939CompareIndex0;        //J1939索引
  UINT16  J1939Comparelen0;          //比较长度
  UINT8   J1939CompareType1;    
  UINT16  J1939CompareIndex1;   
  UINT16  J1939Comparelen1; 
  UINT8   J1939CompareType2;    
  UINT16  J1939CompareIndex2;   
  UINT16  J1939Comparelen2;
  UINT8   J1939CompareType3;    
  UINT16  J1939CompareIndex3;   
  UINT16  J1939Comparelen3;
//  UINT16 OilPressure ; //机油压力j1939来源索引
// UINT16 WaterTemperature ; //水温 j1939来源索引
 unsigned char  UserReserve[60];
}STRUCT_SYS_CONFIG;

typedef struct
{
 unsigned char KeySwitchConfig[7];
 unsigned char KeyStyleConfig[14];
 unsigned char KeyDefaultValue[7];
 unsigned char UserReserve[46];
}STRUCT_KEY_CONFIG;

typedef struct             //得到采样值
{
    UINT8  style;
    UINT16 inputRatio;   //电阻值或比例
    UINT8  dotNum;
    UINT16 sensorPrm[12];
   // UINT8  attribute;    //attribute.7 == 1:需要转化为虚拟开关。 attribute.5_6 == 00:表示低报警 。10:两端报警  01:中间报警 11：高报警  bit0 = 有效位
    UINT8 portIndex;
}INSIDE_16BIT_ATTR_STRUCT;    //av[6]  [6]+[20]
typedef struct 
{
   unsigned char openMask;
   unsigned char shortMask;
}OPENSHOAR;

typedef struct               //得到真实值
{
    UINT8   attribute;    //attribute.7 == 1:需要转化为虚拟开关。 attribute.5_6 == 00:表示低报警 。10:两端报警  01:中间报警 11：高报警
    UINT16  offset;       //偏移量，有符号数据此参数不为零。
    UINT16  bitValue;     //高四位表示小数点位置，低四位表示位值
}SENSOR_DATA_STRUCT; 

typedef struct
{

    UINT8   attribute;    //attribute.7 == 1:需要转化为虚拟开关。 attribute.7-5 报警类型
    UINT16  offset;       //偏移量， 15位为符号位 有符号数据此参数不为零。
    UINT16  bitValue;     //高四位表示小数点位置，低四位表示位值
    UINT16  thresholdL;
    UINT16  thresholdH; 
    UINT8   KeyIndex;  
    UINT8   portIndex;   
}SENSOR_VIRTUAL_KEY_STRUCT;   


typedef struct 
{
 UINT16 J1939_Offset_Byte;  
 UINT16 J1939_Status_Number;       //1k
 UINT16 J1939_Char_Number;    
 UINT16 J1939_Int_Number;     
 UINT16 J1939_Word_Number;     //       8
 UINT16 J1939_String_Number;
}J1939_NUMBER;
typedef struct                             
{
    UINT32 J1939ID;    //该字节的PGN                          
    UINT16 offset;		//nbapo 前面个数累加
    UINT8  number;       //个数
}J1939_FIND_INDEX_STRUCT;

typedef struct                             
{
   // UINT32 J1939ID;    //该字节的PGN                         //  1byte bit0 1  (0  1)
    UINT8  location;   //该字节所在报文中的位置，即第几个字节   //2 4 1     ()
    UINT32 offset;     // 偏移量                             ***  起始bit
    UINT32 bitValue;   //位值                                     ***  位长度 
    UINT16 attribute;  //属性，来表示各位单元的意思，及如何处理该数据   bit15 有效位 bit14 虚拟开关  bit13 大小端  bit12 偏移正负号 bit4~7 报警方式   bit0~bit3 	J1939 类型，b8-11数值模式 
    UINT16 MultiPgckMode;//高8位模式 低  8位两个位置索引
    UINT16 MultiPgckIndex; //索引
    UINT32 thresholdL; //阈值，若该数据需要整为一个虚拟开关，则需要阈值***keycode
    UINT32 thresholdH;
    UINT8  keyIndex;   //开关位置。对J1939数据的虚拟开关进行编号 
}J1939_WORD_DATA_STRUCT;

typedef struct
{
    UINT8 statusCode[8]; //状态码
    //LCD_STATUS_STRUCT status[8]; //状态码
}STATUS_STRUCT;

typedef struct
{
  //  UINT16
    UINT8   type;       //参数类型  
    UINT16  Index;      //参数的索引
    UINT16  rang;       //量程，例如：车速量程为140，转速量程为3000
                       //            油量量程为100（扩大100倍）
                       //            气压量程为100（扩大10倍）
    UINT16 maxStep;    //电机总步数，1、2、3、4号电机为1080
                       //            5、6号电机为2900
    UINT8 filterGear;
    UINT16 minScale;      //偏移量 即零刻度的值为多少。例如：水温为40
                          //车速为0
    UINT8 KeyIndex;   
}MOTO_STEP_STRUCT;
typedef struct
{
 UINT8 DashboardDiagnoseInfo[36];
 UINT8 FrontDriverDiagnoseInfo[26];
 UINT8 BackDriverDiagnoseInfo[26];
 UINT8 MiddleDriverDiagnoseInfo[26];
 UINT8 TopDriverDiagnoseInfo[26];
 UINT8 PowerDiagnoseInfo[26];
 UINT8 PowerContorolDiagnoseInfo[13];    
 UINT8  UserReserve[90];
}DIAGNOSE_STRUCT; 

typedef struct                   //EB83CF
{
 UINT8 DashboardIcoWarn[200];
 UINT8 DashDriverIcoWarn[4];//nbapo:高四位声音低四位图片报警
 UINT8 DashSensorIcoWarn[5];
 UINT8 FrontDriverIcoWarn[20];
 UINT8 FrontSensorIcoWarn[8];
 
 UINT8 BackDriverIcoWarn[20];
 UINT8 BackSensorIcoWarn[8];
  
 UINT8 MiddleDriverIcoWarn[20];
 UINT8 MiddleSensorIcoWarn[8];
  
 UINT8 TopDriverIcoWarn[20];
 UINT8 TopSensorIcoWarn[8];

 UINT8 PowerDriverIcoWarn[18];
 UINT8 PowerFuseIcoWarn[17];
 
 UINT8 PowerContorolIcoWarn[16];   //356
 UINT8 PowerContorolSensorIcoWarn[3];
 
 UINT8 Power205FuseIcoWarn[20];
 
 UINT8 UserReserve[100];  //
}SIcoWarnConfig;
typedef struct 
{
 UINT32 dstIdentifier;   //发送ID
 UINT16 mode;            //最高位0->表示有效  低字节表示要转发几包数据  其他保留
 UINT16 sendCycle;       //发送周期 ms  
 UINT16 offset;          //偏移量
 UINT8  number;           //数量

}ID_Time_INFO;

typedef struct 
{

 UINT16 bitv;		//单位+位值
 UINT16 offset; 	 //偏移量
}Y_AX_B_INFO;

typedef struct 
{
 UINT16  mode;         //高4位来源模式     1->j1939  2->内部数据  3->chugangqi   
                       //低四位转化表模式  0->无     1->状态查表  2->y=ax+b 模式
 UINT8  tableIndex;    //转化表索引
 UINT16 srcTypeIndex;  //j1939 数据来源索引   内部数据选择+索引        传感器来源索引
 UINT8  srcbitIndex;   //j1939起始位              -                    传感器起始位
 UINT8  srcbitLenght;  //j1939位长             数据长度                传感器数据长度
                                                                     
 UINT8  dstbyteIndex;    //字节索引
 UINT8  dstbitIndex;     //起始位
 UINT8  dstbitLenght;    //位长
}CAN_FRAME_INFO;


typedef struct 
{
 UINT8       Buffer1[20][8];    //查表1
 Y_AX_B_INFO Buffer2[50];       //查表2
 CAN_FRAME_INFO FRAME_INFO[500];
}J1939_DBT_INFO;


 


 typedef	struct 
{
  UINT16   attribute;        
  unsigned long  SignelPackageID;
  unsigned long  MultiPgckageDeclareID;
  unsigned long  MultiPgckageDatID;
  UINT16   FailureNumber; 
  unsigned long  FMI_SPN[400];//nbapo:故障闪码
}
EngineDiagnoseCfgStruct;

//存储配置
//配置数量
#define RESOURE_TOTAL		10	//一级索引数量
#define SECOND_CFG_TOTAL	50	//二级索引数量
//一级索引
typedef struct{
	unsigned char	attribute;//b7 有效
	unsigned char   logicLocation;	//二级索引开始位置
	unsigned char	logicLenght;//二级索引长度
	unsigned char   num;//保存次数
	//UINT16	offest 	;//偏移 = 上个资源偏移 + 次数 * 一次所占字节   状态算一个字节
}StorageManager1_struct;

//二级索引
typedef struct{
	unsigned char	attribute;//b7,有效性， b3-b0: 类型
	UINT16	srcIndex;//b15-12，数据来源(J1939或内部)，b11-b0：来源索引
	UINT16   dstIndex;//索引位置= 上一个索引+ 保存次数* 上个类型字节数
	unsigned char   load; //二级索引 存储标志
}StorageManager2_struct;


//------------------------------------------------------------------------------
typedef struct
{
    UINT16  disX;              //显示坐标
    UINT16  disY;             //
    UINT32  lutAddr;          //调色板的起始地址  = Image_Flash_Addr_Start + MRLE_INFO_SIZE;
    UINT16  lutSize;          //调色板的容量      = 0x004 << bitsPixel;
    UINT8   lutType;          //调色板类型        = LUT_format & 0x01;
    UINT16  lutOffset;        //调色板偏移量      = 0x100;
    UINT32  picAddr;          //图片的起始地址    = Image_Flash_Addr_Start + lutAddr + MRLE_INFO_SIZE;
    UINT8   rle;              //是否压缩          = rledata_cnt & 0x0f;
    UINT8   bitsPixel;        //像素位数          = 8(256色) 4(16色) 6(64色)
    UINT16  picH;             //图片的高          = y
    UINT16  picW;             //图片的宽          = w

}PIC_INFO_STRUCT;

typedef struct
{
  UINT16   attribute;    //bit15 有效位  bit14 需逻辑索引  bit0~bit11逻辑索引
  UINT8    lenght;       //逻辑处理长度
  UINT16   picIndex; 
}PIC_WIN_STRUCT; 
typedef struct
{
    UINT16 attribute;    //bit15 有效位  bit14 需逻辑索引  bit0~bit11逻辑索引
    UINT8  lenght;       //逻辑处理长度
    UINT16 x;            //界面显示起始坐标
    UINT16 y;            //界面显示起始坐标
    UINT8 w;             //界面显示宽度
    UINT8 h;             //界面显示高度
    UINT8 charHeight;    //界面显示字高度
    UINT8  zoom;          //放大窗口
    UINT8  aplha;         //窗口透明度
    UINT8  color;         //字体与背景颜色
    UINT16 charIndex;     //字体显示位置
    UINT8  initMode;      //初始化模式  如清屏    
}OSD_WIN_STRUCT;
typedef struct
{
    PIC_WIN_STRUCT picWin[9];  //45
    OSD_WIN_STRUCT osdWin[4];    
}INTERFACE_INFO_STRUCT;





typedef struct
{
  UINT8    mode;       //逻辑索引mode=1模拟量 moder = 2
  UINT16    page;
  UINT16   inputIndex; 
  UINT16   inputLenght; 
  UINT16   bitIndex;     //1->假翻页
  UINT16   thresholdL;   //(1)
  UINT16   thresholdH;   //(2)
  UINT16   picNum;       //(3)
  UINT16   picIndex;  
  UINT16   picLayerNumber;//图层号   gai
}PIC_LOGIC_STRUCT;


typedef struct
{
  UINT8    mode;       //逻辑模式mode=1数字 moder = 2字母
  UINT16    page;
  UINT16   inputIndex; 
  UINT16   inputLenght; 
  UINT16   bitIndex;     //高八位为起始位  低八位结束位
  UINT16   thresholdL; 
  UINT16   thresholdH;
  UINT16   charIndex;   //osd ram 地址
  UINT16   disLenght;   //高八位为整数长度 低八位为小数点长度
  UINT8    color;
  UINT8    szUnit[10]; //单位或其他
}OSD_LOGIC_STRUCT;

#pragma   pack()

#endif
