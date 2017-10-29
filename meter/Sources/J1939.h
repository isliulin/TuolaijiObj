#undef	EXT
#ifdef	J1939_C
#define	EXT
#else
#define	EXT extern
#endif
/*****common defines and macros*******/
#include <hidef.h>      	
#include <mc9s12xhz512.h>
#include	"typedef.h"
/*****Functions Prototypes**********/
#pragma CODE_SEG USER_ROM
static void Trans_CANpkt(void);
static void Rev_J1939msg(J1939_PDU_T msg_ptr);
static void Dll_pdu(void);
void fual_value_require(void);
void BeiAn_Record_SwitchLoad_require(void);
void SwitchLoad_BitSwitch(void);
void	J1939_SignalCheck(void);
void	J1939_Speed_Send(void);
void	J1939_CCVS_Send(void);
void  TX_TPMS_LearnEnd(void);
void  TXWaitRelay(void);

#pragma CODE_SEG DEFAULT
/***************ram macros*************/
EXT LoadStStruct SendRecordSwitch[5];

EXT	CAN_PACKET_T  send_can_packet;
EXT	CAN_PACKET_T 	rev_can_packet;
EXT	J1939_PDU_T  	J1939MSG;///J1939���ݰ�   	

EXT unsigned char J1939CarSpeedSendTime;
EXT unsigned char J1939CCVSSendTime;

EXT	uint			engSpeed;///������ת��
EXT	uchar 		watTemp;	///������ˮ��		
EXT	uchar 		oilPres;	///����ѹ��	
EXT uint			fual_value; ///˲ʱ�ͺ�   
EXT ulong			total_fual_used;///���ͺ�
EXT	uchar			Fual_Require_Time;///���ͺĵ�����ͨѶʱ��

EXT uint			CatalystTankLevel;///����Һλ
EXT	uchar 	  RecordSwitchSend100ms;///���ͼ�¼��״̬ʱ��
EXT uchar 		J1939_Sa;///ԭ��ַ
EXT Diagnose_Lamp	Engine_Diagnose_Lamp,SCR_Diagnose_Lamp;///������������Ĺ��ϵ�
EXT unsigned long	EngineDiagnose_PGN,SCRDiagnose_PGN;///�������������PGN

EXT unsigned char	ResponseData;///bootloader��Ӧ����֡��־

EXT uchar 		Engine_Diagnose_Flag;
EXT uchar 		SCR_Diagnose_Flag;

//BMSЭ�� id:0x1826d0d2
EXT uint  HighestChargeVolt;
EXT uint  BatteryNominalVolt;
EXT uchar HighestWorkTemp;
EXT uchar LowestWorkTemp;
EXT uchar BatteryPitchNum;
EXT uchar LECUnumber;
//����������Ϣ2,ID:0x1827D0D2
EXT uint  SingleBatHighestVolt;
EXT uint  SingleBatLowestVolt;
EXT uint  BatteryNominalCurr;
EXT uchar BatterySOH;
//����鶯̬��Ϣ2 ID:0x1423D0D2
EXT uint  BatteryPresentVolt;
EXT uint  BatteryPresentCurr;
EXT uint  BatteryPresentSOC;
EXT uchar BatteryPresentHighestTemp;
EXT uchar BatteryPresentLowestTemp;
//����鶯̬��Ϣ3 ID:0x1424D0D2
EXT uint  SingleBatPresentHighestVolt;
EXT uint  SingleBatPresentLowestVolt;
EXT uint  PositiveBusInsulationRES;
EXT uint  MinusBusInsulationRES;
//����鵱ǰϵͳ״̬0x0C1FD0D2
EXT uchar BatteryBMSCheckSum;
EXT uchar BatteryBMSCount;
EXT BMSStatus1Struct	BatteryBMSStatus1;
EXT BMSStatus2Struct	BatteryBMSStatus2;
EXT BMSStatus3Struct	BatteryBMSStatus3;

//����CAN��Ϣ1 0x0CFF4E27
EXT uchar BMSContrllerType;
EXT uchar BMSContrllerSerial;
EXT uchar BMSContrllerSoftVersion;
EXT uchar BMSContrllerProductData;
//����CAN��Ϣ1 0x0CFF0F27
EXT uchar BMSContrllerFaultNumber;
EXT uchar BMSContrllerWarnNumber;
EXT uint  BMSContrllerCarspeed;
EXT uchar BMSContrllerGears;
EXT uchar DriverSystemFaultNumber;
EXT uchar BMSContrllerEngineSpeed;
//����ϵͳ����������4 0x0CFF0E27
EXT uchar InverterTemperature;
EXT uchar Motor1InverterRadiatorTemp;
EXT uchar Motor1WindingResTemp;
EXT uchar Motor2InverterRadiatorTemp;
EXT uchar Motor2WindingResTemp;
EXT uchar ConnecterStatus;
EXT uchar OperationStatus;


/**************Parameter macros*****************/
#define GLOBADDR 255
#define NODEADDR 254

#define RecordSwitchOn 0b01
#define RecordSwitchOff 0b00
#define UndefinedRecordSwitch 0b11

#define	SCR_SA								0x3D
#define	ENGINE_SA						0x00
#define Lamp_On							0b01

#define	MIL_SA								0x00
#define	DM1_PGN							0xFECA
#define	DM2_PGN							0xFECB
#define	DM4_PGN							0xFECD

#define	SCR_DIAGNOSE_NO_PACKAGE			  0x00
#define	ENGINE_DIAGNOSE_NO_PACKAGE		0x00
#define	ENGINE_DIAGNOSE_PACKAGE				0xff
#define	SCR_DIAGNOSE_PACKAGE					0xff

///config J1939
#ifdef	J1939_C
volatile const unsigned char  J1939_TACHO_FLAG       @0x7f03 = 0x01;
volatile const unsigned char  J1939_TEMPTURE_FLAG    @0x7F04 = 0x02;
volatile const unsigned char  J1939_oilpress_FLAG    @0x7F05 = 0x03;
volatile const unsigned char  J1939_FualTotal_FLAG   @0x7F06 = 0x04;
volatile const unsigned char  J1939_DIGNOSEMSG_FLAG  @0x7F07 = 0x05;
volatile const unsigned char  J1939_OilComsumeRate   @0x7F0F = 0x06;
#endif

