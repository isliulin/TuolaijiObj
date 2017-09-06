 #ifndef	_typedef_h
#define	_typedef_h
/****************************************/
#include "arm_cm4.h"

#define GENPTR
#define GENPTR_B
#define	EXT extern
#define ROMCONST       const 

//ap: long 32bit
typedef 	unsigned 	char		u8;	  //�޷�����
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
typedef unsigned int short uint; 	//ap:��ֲ16λ��Ƭ���ֽڼ���
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
    UINT8 front;//��
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
    UINT8  location;   //���ֽ����ڱ����е�λ�ã����ڼ����ֽ�
    UINT8  startBit;   //����λ
    UINT8  bitLen;     //λ����
    UINT8  keyCode;
    UINT8  attribute;
    UINT8  keyIndex;
}J1939_BIT_STATUS_STRUCT;
*/















 


//����J1939�е�ASCII��
//����������Խ���24���ַ�
//�����3��PGN����ʾ
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
    UINT16 offset;     //�ֽڵ�ƫ����
    UINT8  bitValue;   //λֵ   
}INSIDE_PHYS_STRUCT;


typedef struct
{
    UINT32 offset;     //�ֽڵ�ƫ����
    UINT8  bitValue;   //λֵ   
}INSIDE_PHYS_32BIT_STRUCT;
typedef struct
{
    UINT8 text[21];
    UINT8  attribute[3];
}MAIN_PAGE_INFO_STRUCT;

typedef struct
{
    UINT8 text[67];     //��ʾ����
    UINT8 attribute[6]; //����
}LCD_INFO_STRUCT;

typedef struct
{
    LCD_INFO_STRUCT info[24];
    UINT8 line;
}LCD_INFO_SCREEN_STRUCT;
typedef struct
{
    UINT8 statusCode; //״̬��
    UINT8 text[21];   //״̬����
}LCD_STATUS_STRUCT;






//��ҳ��ʾ��״�����ṹ�塣
typedef struct 
{
//    UINT8 dot;            //С����λ��
    UINT16 alarmH;        //��������
    UINT16 alarmL;        //��������
    UINT16 base;          //��ʾ������Сֵ
    UINT16 max;           //��ʾ�������ֵ
    UINT8 type;
    UINT8 source;         //����Դ��
}COLUMN_PARAMETER;
                                                           //    |<-       screenLen       ->|
typedef struct                                             //     ___________________________
{                                                          //    |                           |
    UINT8 picture;        //ͼƬ�����׵�ַ                       |offset|<- len     ->|      |
    UINT8 colour;         //ͼƬ����ɫ                           |   _   _____________       |
    UINT8 len;            //ͼƬ�ĳ���                           |      |             |      |
    UINT8 wide;           //ͼƬ�Ŀ�                             | wide |             |      |
    UINT8 screenLen;      //��ʾ��Ļ�ĳ���                       |   _  |_____________|      |
    UINT8 offsetL;        //ͼƬ�׵�ַ����Ļ�е�ƫ����           |                           |
    UINT8 offsetH;        //ͼƬ�Ĵ�ֱƫ����              //     |___________________________|
}PICTURE_STRUCT;    

typedef struct 
{
    UINT8 disBuf[60];     //ҳ�����ƣ����˵��µ�����
    UINT8 page;           //ҳ������Ӧ��ҳ���
}DIS_PAGE_STRUCT;

typedef struct 
{
    DIS_PAGE_STRUCT menu[15];   //�˵�Ŀ¼
    UINT8 maxMenu;           //���Ŀ¼��
}MENU_PAGE_STRUCT; 

typedef struct 
{
    DIS_PAGE_STRUCT menu[6];   //�˵�Ŀ¼
    UINT8 maxMenu;           //���Ŀ¼��
}DIAGNOSE_MENU_PAGE_STRUCT;    

typedef struct 
{
    UINT16 gb2312;        //�ַ���gb2312��
    UINT8 addr;           //�ַ����ֿ��е��׵�ַ
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
һ��	����������
1��
����	���ò�����ʽ��
1��
***************************************************************************/
typedef struct image_info_s {
	BYTE lut_type;	///	//1:luts(byte pointer method.default on TW8835MENU), 0:lut(address method)
	BYTE rle;				//upper:bpp bottom:rle_counter.
	WORD dx,dy;			//width, height
} image_info_t;
/***************************************************************************
һ��	����������
1��
����	���ò�����ʽ��
1��
***************************************************************************/
typedef struct image_item_info_s {
	BYTE type;	//0:LUT+image, 1:header+lut+image 2:image_only 3: lut_only 4:header_only	
	DWORD loc;  //absolute location on SpiFlash
	image_info_t *info;
	BYTE alpha;		//alpha index. 0xFF:ignore.
} image_item_info_t;
/***************************************************************************
һ��	����������
1��
����	���ò�����ʽ��
1��
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
һ��	����������
1��
����	���ò�����ʽ��
1��
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
һ��	����������
1��
����	���ò�����ʽ��
1��
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
һ��	����������
1��
����	���ò�����ʽ��
1��
***************************************************************************/
 typedef struct {    
     DWORD start;    
     DWORD length;   
 } SLIDEIMAGE;       
/***************************************************************************
һ��	����������
1��
����	���ò�����ʽ��
1��
***************************************************************************/
typedef struct menu_item_info{
		UINT16	Menu_X;
		UINT16	Menu_Y;
		UINT16	Menu_Lut;
}Menu_Item_Info;
/***************************************************************************
һ��	����������
1��
����	���ò�����ʽ��
1��
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
	UINT16 flag;            //��־λ������ָʾ������Ϣ��
	// UINT8 interfaceCnt;
	// UINT8 menuPageCnt;  
	//UINT8 menuPagechange;  //ҳ�ı��־                      
	UINT8 cursor;          //�����һ�˵���ѡ��λ�á�
	UINT8 menuOne;         //����һ���˵����λ��
	UINT8 menuTwo;         //��������˵�λ��  
	/**************��ϲ���************************/ 
	UINT8 moduleID;         //ģ�� 
	UINT8 dispLenght;       //�����ʾ����

	UINT16 dataTemp;         //��ʼ��Ϊ0xff
	UINT8  dataCnt;          //���osd��ͼƬ��һ��
	UINT8 interfaceCnt;     //��Ϊʱ������ ����ϲ�ѯ��
	TftStruct status;
	/**************��ҳ����************************/ 	  
	UINT8 osdPage;          //��ǰҳ
	UINT8 pageCnt;          //ҳ����
	UINT8 picPage;          //ͼƬ����
	UINT8 curPage;          //��ǰҳ
	  
}LCD_MSG; 
/*typedef struct
{
    UINT16 flag;            //��־λ������ָʾ������Ϣ��
    UINT8 interfaceCnt;
	  UINT8 menuNum;
	  UINT8 diagnoseMenuNum;
	  UINT8 index1;           //�����һ�˵���ѡ��λ�á�
	  UINT8 index2;           //����ڶ��˵���ѡ��λ�á�
}LCD_MSG;  */
/*********************************************************************/


/*********************************************************************

*********************************************************************/





typedef struct
{
#define FilterBufferMax   10
  UINT16    SensorAd[FilterBufferMax];       //�߼�ģʽmode=1���� moder = 2��ĸ
  UINT8     Sensorindex;
  UINT16    SensorVal;
  UINT8     SensorCnt;
}StructFilter;

 

                                 


/*********************************************************************

*********************************************************************/
typedef struct
{
    UINT8 x;                  //��ʼ����x   
    UINT8 y;                  //��ʼ����y
   // UINT8 type;
    UINT8 index;
    UINT8 szUnit[5];
    UINT8 location;           //��4λ��ʾ���ĸ����ڣ���4λ��ʾ����һҳ��
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
	UINT32 dstIdentifier;   //����ID
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
//�������ݽṹ��
#pragma   pack(1)

typedef struct
{
 UINT16   ModuleConfig;//0b00101011;//�ӵ�0λ����7λ�ֱ�Ϊǰģ�飬��ģ�飬��ģ�飬��ģ�飬��磬��CANģ��  ��Դ����
 UINT16   speedChanelIndex;// = 20;
 unsigned char  CarSpeedSrc; //nbapo:������Դ            
 UINT16   engSpeedChanelIndex;// = 20;
 unsigned char  EngCarSpeedSrc;
 UINT16   SpeedRate;      //2200
 unsigned char  CycleCaptureNum ;  //8
 unsigned char  DashBoardSpeedChannel;   //0
 unsigned char  DashBoardEngSpeedChannel ;  //0
 UINT16   disLoadWarn[5];//L:��8λģ�飬b7-6:ͼƬ������ʽ��b5-b0����
 unsigned char  startLogoTime;      //s
 unsigned char  pwm_load_out;       //����PWMλ�������
 unsigned char  post;              //1->�����Լ�      0->��
 
 UINT16 OilPressure ; //����ѹ��j1939��Դ����
 UINT16 WaterTemperature ; //ˮ�� j1939��Դ����
 UINT16 UremicLevel;       //����Һλ j1939��Դ����
 unsigned char   selectPowerCtr;  //1->pd205  ���� pd202
 
  UINT8   J1939CompareType0;         //����λ 0x5��Ч  ����λ 1->���ֵ 2->��Сֵ 
  UINT16  J1939CompareIndex0;        //J1939����
  UINT16  J1939Comparelen0;          //�Ƚϳ���
  UINT8   J1939CompareType1;    
  UINT16  J1939CompareIndex1;   
  UINT16  J1939Comparelen1; 
  UINT8   J1939CompareType2;    
  UINT16  J1939CompareIndex2;   
  UINT16  J1939Comparelen2;
  UINT8   J1939CompareType3;    
  UINT16  J1939CompareIndex3;   
  UINT16  J1939Comparelen3;
//  UINT16 OilPressure ; //����ѹ��j1939��Դ����
// UINT16 WaterTemperature ; //ˮ�� j1939��Դ����
 unsigned char  UserReserve[60];
}STRUCT_SYS_CONFIG;

typedef struct
{
 unsigned char KeySwitchConfig[7];
 unsigned char KeyStyleConfig[14];
 unsigned char KeyDefaultValue[7];
 unsigned char UserReserve[46];
}STRUCT_KEY_CONFIG;

typedef struct             //�õ�����ֵ
{
    UINT8  style;
    UINT16 inputRatio;   //����ֵ�����
    UINT8  dotNum;
    UINT16 sensorPrm[12];
   // UINT8  attribute;    //attribute.7 == 1:��Ҫת��Ϊ���⿪�ء� attribute.5_6 == 00:��ʾ�ͱ��� ��10:���˱���  01:�м䱨�� 11���߱���  bit0 = ��Чλ
    UINT8 portIndex;
}INSIDE_16BIT_ATTR_STRUCT;    //av[6]  [6]+[20]
typedef struct 
{
   unsigned char openMask;
   unsigned char shortMask;
}OPENSHOAR;

typedef struct               //�õ���ʵֵ
{
    UINT8   attribute;    //attribute.7 == 1:��Ҫת��Ϊ���⿪�ء� attribute.5_6 == 00:��ʾ�ͱ��� ��10:���˱���  01:�м䱨�� 11���߱���
    UINT16  offset;       //ƫ�������з������ݴ˲�����Ϊ�㡣
    UINT16  bitValue;     //����λ��ʾС����λ�ã�����λ��ʾλֵ
}SENSOR_DATA_STRUCT; 

typedef struct
{

    UINT8   attribute;    //attribute.7 == 1:��Ҫת��Ϊ���⿪�ء� attribute.7-5 ��������
    UINT16  offset;       //ƫ������ 15λΪ����λ �з������ݴ˲�����Ϊ�㡣
    UINT16  bitValue;     //����λ��ʾС����λ�ã�����λ��ʾλֵ
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
    UINT32 J1939ID;    //���ֽڵ�PGN                          
    UINT16 offset;		//nbapo ǰ������ۼ�
    UINT8  number;       //����
}J1939_FIND_INDEX_STRUCT;

typedef struct                             
{
   // UINT32 J1939ID;    //���ֽڵ�PGN                         //  1byte bit0 1  (0  1)
    UINT8  location;   //���ֽ����ڱ����е�λ�ã����ڼ����ֽ�   //2 4 1     ()
    UINT32 offset;     // ƫ����                             ***  ��ʼbit
    UINT32 bitValue;   //λֵ                                     ***  λ���� 
    UINT16 attribute;  //���ԣ�����ʾ��λ��Ԫ����˼������δ��������   bit15 ��Чλ bit14 ���⿪��  bit13 ��С��  bit12 ƫ�������� bit4~7 ������ʽ   bit0~bit3 	J1939 ���ͣ�b8-11��ֵģʽ 
    UINT16 MultiPgckMode;//��8λģʽ ��  8λ����λ������
    UINT16 MultiPgckIndex; //����
    UINT32 thresholdL; //��ֵ������������Ҫ��Ϊһ�����⿪�أ�����Ҫ��ֵ***keycode
    UINT32 thresholdH;
    UINT8  keyIndex;   //����λ�á���J1939���ݵ����⿪�ؽ��б�� 
}J1939_WORD_DATA_STRUCT;

typedef struct
{
    UINT8 statusCode[8]; //״̬��
    //LCD_STATUS_STRUCT status[8]; //״̬��
}STATUS_STRUCT;

typedef struct
{
  //  UINT16
    UINT8   type;       //��������  
    UINT16  Index;      //����������
    UINT16  rang;       //���̣����磺��������Ϊ140��ת������Ϊ3000
                       //            ��������Ϊ100������100����
                       //            ��ѹ����Ϊ100������10����
    UINT16 maxStep;    //����ܲ�����1��2��3��4�ŵ��Ϊ1080
                       //            5��6�ŵ��Ϊ2900
    UINT8 filterGear;
    UINT16 minScale;      //ƫ���� ����̶ȵ�ֵΪ���١����磺ˮ��Ϊ40
                          //����Ϊ0
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
 UINT8 DashDriverIcoWarn[4];//nbapo:����λ��������λͼƬ����
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
 UINT32 dstIdentifier;   //����ID
 UINT16 mode;            //���λ0->��ʾ��Ч  ���ֽڱ�ʾҪת����������  ��������
 UINT16 sendCycle;       //�������� ms  
 UINT16 offset;          //ƫ����
 UINT8  number;           //����

}ID_Time_INFO;

typedef struct 
{

 UINT16 bitv;		//��λ+λֵ
 UINT16 offset; 	 //ƫ����
}Y_AX_B_INFO;

typedef struct 
{
 UINT16  mode;         //��4λ��Դģʽ     1->j1939  2->�ڲ�����  3->chugangqi   
                       //����λת����ģʽ  0->��     1->״̬���  2->y=ax+b ģʽ
 UINT8  tableIndex;    //ת��������
 UINT16 srcTypeIndex;  //j1939 ������Դ����   �ڲ�����ѡ��+����        ��������Դ����
 UINT8  srcbitIndex;   //j1939��ʼλ              -                    ��������ʼλ
 UINT8  srcbitLenght;  //j1939λ��             ���ݳ���                ���������ݳ���
                                                                     
 UINT8  dstbyteIndex;    //�ֽ�����
 UINT8  dstbitIndex;     //��ʼλ
 UINT8  dstbitLenght;    //λ��
}CAN_FRAME_INFO;


typedef struct 
{
 UINT8       Buffer1[20][8];    //���1
 Y_AX_B_INFO Buffer2[50];       //���2
 CAN_FRAME_INFO FRAME_INFO[500];
}J1939_DBT_INFO;


 


 typedef	struct 
{
  UINT16   attribute;        
  unsigned long  SignelPackageID;
  unsigned long  MultiPgckageDeclareID;
  unsigned long  MultiPgckageDatID;
  UINT16   FailureNumber; 
  unsigned long  FMI_SPN[400];//nbapo:��������
}
EngineDiagnoseCfgStruct;

//�洢����
//��������
#define RESOURE_TOTAL		10	//һ����������
#define SECOND_CFG_TOTAL	50	//������������
//һ������
typedef struct{
	unsigned char	attribute;//b7 ��Ч
	unsigned char   logicLocation;	//����������ʼλ��
	unsigned char	logicLenght;//������������
	unsigned char   num;//�������
	//UINT16	offest 	;//ƫ�� = �ϸ���Դƫ�� + ���� * һ����ռ�ֽ�   ״̬��һ���ֽ�
}StorageManager1_struct;

//��������
typedef struct{
	unsigned char	attribute;//b7,��Ч�ԣ� b3-b0: ����
	UINT16	srcIndex;//b15-12��������Դ(J1939���ڲ�)��b11-b0����Դ����
	UINT16   dstIndex;//����λ��= ��һ������+ �������* �ϸ������ֽ���
	unsigned char   load; //�������� �洢��־
}StorageManager2_struct;


//------------------------------------------------------------------------------
typedef struct
{
    UINT16  disX;              //��ʾ����
    UINT16  disY;             //
    UINT32  lutAddr;          //��ɫ�����ʼ��ַ  = Image_Flash_Addr_Start + MRLE_INFO_SIZE;
    UINT16  lutSize;          //��ɫ�������      = 0x004 << bitsPixel;
    UINT8   lutType;          //��ɫ������        = LUT_format & 0x01;
    UINT16  lutOffset;        //��ɫ��ƫ����      = 0x100;
    UINT32  picAddr;          //ͼƬ����ʼ��ַ    = Image_Flash_Addr_Start + lutAddr + MRLE_INFO_SIZE;
    UINT8   rle;              //�Ƿ�ѹ��          = rledata_cnt & 0x0f;
    UINT8   bitsPixel;        //����λ��          = 8(256ɫ) 4(16ɫ) 6(64ɫ)
    UINT16  picH;             //ͼƬ�ĸ�          = y
    UINT16  picW;             //ͼƬ�Ŀ�          = w

}PIC_INFO_STRUCT;

typedef struct
{
  UINT16   attribute;    //bit15 ��Чλ  bit14 ���߼�����  bit0~bit11�߼�����
  UINT8    lenght;       //�߼�������
  UINT16   picIndex; 
}PIC_WIN_STRUCT; 
typedef struct
{
    UINT16 attribute;    //bit15 ��Чλ  bit14 ���߼�����  bit0~bit11�߼�����
    UINT8  lenght;       //�߼�������
    UINT16 x;            //������ʾ��ʼ����
    UINT16 y;            //������ʾ��ʼ����
    UINT8 w;             //������ʾ���
    UINT8 h;             //������ʾ�߶�
    UINT8 charHeight;    //������ʾ�ָ߶�
    UINT8  zoom;          //�Ŵ󴰿�
    UINT8  aplha;         //����͸����
    UINT8  color;         //�����뱳����ɫ
    UINT16 charIndex;     //������ʾλ��
    UINT8  initMode;      //��ʼ��ģʽ  ������    
}OSD_WIN_STRUCT;
typedef struct
{
    PIC_WIN_STRUCT picWin[9];  //45
    OSD_WIN_STRUCT osdWin[4];    
}INTERFACE_INFO_STRUCT;





typedef struct
{
  UINT8    mode;       //�߼�����mode=1ģ���� moder = 2
  UINT16    page;
  UINT16   inputIndex; 
  UINT16   inputLenght; 
  UINT16   bitIndex;     //1->�ٷ�ҳ
  UINT16   thresholdL;   //(1)
  UINT16   thresholdH;   //(2)
  UINT16   picNum;       //(3)
  UINT16   picIndex;  
  UINT16   picLayerNumber;//ͼ���   gai
}PIC_LOGIC_STRUCT;


typedef struct
{
  UINT8    mode;       //�߼�ģʽmode=1���� moder = 2��ĸ
  UINT16    page;
  UINT16   inputIndex; 
  UINT16   inputLenght; 
  UINT16   bitIndex;     //�߰�λΪ��ʼλ  �Ͱ�λ����λ
  UINT16   thresholdL; 
  UINT16   thresholdH;
  UINT16   charIndex;   //osd ram ��ַ
  UINT16   disLenght;   //�߰�λΪ�������� �Ͱ�λΪС���㳤��
  UINT8    color;
  UINT8    szUnit[10]; //��λ������
}OSD_LOGIC_STRUCT;

#pragma   pack()

#endif
