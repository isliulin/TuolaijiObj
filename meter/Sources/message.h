#undef	EXT
#ifdef	MESSAGE_C
#define	EXT
#else
#define	EXT extern
#endif

//���õ���ֵ
#ifdef MESSAGE_C
const uint PowerContrlCurrentConfig[8]={400,400,400,400,400,400,400,400};
#else
extern const uint PowerContrlCurrentConfig[8];
#endif

/* common defines and macros */
#include <hidef.h>      	
#include <mc9s12xhz512.h>
#include	"typedef.h"
/***********parameter macros****************/
#define	DRIVE_KEY_NUM											2		///��13��
#define	DRIVELOADNUM												5		///5*4=20
#define	POWER_LOAD_NUM										5		///5*4=20
#define	POWERCONTRL_LOAD_NUM							3		///3*4=12
#define	POWER_FUSE_NUM										5
#define	FIRST_ERROR_NUM										22	///20*8
#define MODULE_CONNECT_NUM								3		///3*8�������趨Ϊ8����J1939��ͨѶΪ16��
#define	SECOND_ERROR_NUM									4
/******funtion macros****************************/
#pragma CODE_SEG  USER_ROM
void MesseageDeal(void);
void SendCan0Message(unsigned char SendKind);
void WakeUpInit(void);
static void SystemPowerDeal(void);
static void	CanSendDeal(void);
static void SleepDeal(void);;
static void DriveErrorDisplayDeal(void);
static void PowerErrorDisplayDeal(void);
static void PowerContrlErrorDisplayDeal(void);
static void	MeterErrorDisplayDeal(void);
static void push_dis(uchar tmp);
static void pop_dis(uchar tmp);
static void SpeedPwmOut(void);
static void PowerContrlCurrentDeal(void);
///HNS-ZB209�¶���ĺ���
void err_send(uchar data);
static void CAN0_SendFrame(CAN_PACKET_T SendFrame);

static void	Sensor_rxd_process(void);
static void module_check(void);
static void Message_SpecialState_process(void);
static void ConnetDisplayDeal(void);
static void SecondErrorDisplayDeal(void);
static void SpecialSecondErrorDeal(void);
static void SpecialThirdErrorDeal(void);
static void	SecondErrorState_Logic(void);
#pragma CODE_SEG DEFAULT
/*******************ram macros*****************************/
EXT	uchar		SleepIndex;
EXT	uint		delay60s;
EXT	uchar 	SleepCodeBuf;
EXT	uchar 	FlashCodeBuf;
EXT	uchar 	MeterLedSend250ms;
EXT	uchar 	MeterKeySendTime,MeterSendLoadStTime,MeterSendDistanceTime,VirsualMeterKeySendTime;
EXT	uchar 	SendJ1939RotationTime;
EXT	uint 		MeterSendFuelConsumeTime,MeterSendFuelConsumeTime1;
EXT	uchar 	MeterSendPanelTemperatureSensorTime;
EXT	ulong 	VehicleSpeedData;
EXT	uchar 	OilNumberData;
EXT	uchar 	VehicleSmallDistanceNum[2];
EXT	uchar 	VehicleOverallDistanceNum[3];
EXT	uint 		SystemVolatage;///ϵͳ��ѹ
EXT	uchar 	VehicleRotation[2];
EXT	uchar   rains_count;     ///������εĵȴ������ڵ��ACKʱ��
EXT	SelfKeyStruct 	FrontDriveKey[3],BackDriveKey[3],TopDriveKey[3],PowerKey[1],PowerContrlKey[1];///ģ�鿪��������
EXT	uchar 	FrontSensorData[7],BackSensorData[7],TopSensorData[7],PowerSensorData[7],PowerContrlSensorData[7];///ģ�鴫��������
EXT	LoadStStruct 	FrontDriveLoad[DRIVELOADNUM],FrontDriveLoadBuf[DRIVELOADNUM],///ǰ��ģ�鸺��״̬����
						BackDriveLoad[DRIVELOADNUM],BackDriveLoadBuf[DRIVELOADNUM],	///����ģ�鸺��״̬����
						TopDriveLoad[DRIVELOADNUM],TopDriveLoadBuf[DRIVELOADNUM];		///����ģ�鸺��״̬����
EXT	LoadStStruct 	PowerLoad[POWER_LOAD_NUM],PowerLoadBuf[POWER_LOAD_NUM],///���ģ�鸺��״̬����
						PowerFuse[POWER_FUSE_NUM],PowerFuseBuf[POWER_FUSE_NUM];	///���ģ�鱣��˿״̬����
EXT	LoadStStruct 	PowerContrlLoad[POWERCONTRL_LOAD_NUM],PowerContrlLoadBuf[POWERCONTRL_LOAD_NUM];///��Դ����ģ��
EXT	uint 		 PowerContrlCurrent[8];				 ///��Դ����ģ�����
EXT	StStruct PowerContrlOverLoadFlag;
EXT	uchar 	SmallErrorOld[FIRST_ERROR_NUM];///һ����ϵĻ���
EXT	uchar 	SmallError[FIRST_ERROR_NUM];
EXT	uchar 	OpenError[FIRST_ERROR_NUM*6];///��·���ϵı�־���壻*4��ʾ��·״̬�뿪·���ñ�־�Ļ���ı���

EXT	uint 	  MainMenuLogoTime;
EXT	uchar 	MainMenuDis_count;///���������
EXT	uint 	  MainMenuDis_countDelay;///�����������ʱ����
EXT	uchar 	dis_count;///���ϵ�����
EXT	uchar 	dis_content[MulfunctionTotalNum];///���ϵı��
EXT	unsigned char	SendConfigDataNum;///���õ�����֡��
EXT	unsigned char	EepromConfigData[4];///�����ֵ����
////�¶������
EXT unsigned char SendSelfIdNum;
EXT unsigned char ConfigDataResult;
EXT unsigned char Video_Power_Detect_Time[2];///���CVBS������Ƶ��ʱ�����
EXT unsigned char Speed_PWM_Detect_Time;///��⳵�ټ����������ʱ�����
EXT unsigned int	ECU_ModuleCheck_Time;///������ECUͨѶ���ʱ��
EXT uint	SCRModuleDetectTime;///SCRͨѶ���ʱ��
EXT uint	Delay_BMS_ModuleCheck;///BMSͨѶ���ʱ��
EXT uint	Delay_DMC_ModuleCheck;///DMCͨѶ���ʱ��
EXT uint 	FrontModuleCheck_Time,TopModuleCheck_Time,BackModuleCheck_Time,PowerModuleCheck_Time,PowerContrlModuleCheck_Time;///����ģ��ͨѶ���ʱ��
EXT StStruct 	VehicleModuleError,TempVehicleModuleError;///����ģ����ϵı�־����
EXT StStruct 	J1939ModuleError[2],TempJ1939ModuleError[2];///J1939����ڵ���ϵı�־����
EXT uchar First_Error[MODULE_CONNECT_NUM],First_ErrorOld[MODULE_CONNECT_NUM];///һ����ϵĻ���
EXT StStruct	SecondError[SECOND_ERROR_NUM],TempSecondError[SECOND_ERROR_NUM];///������ϵĻ���
EXT uchar Second_Error[SECOND_ERROR_NUM],Second_ErrorOld[SECOND_ERROR_NUM];

EXT StStruct	SpecialSecondError[2],TempSpecialSecondError[2];///���������ϵı�־
EXT uchar SpecialSecond_Error[2],SpecialSecond_ErrorOld[2];///���������ϵĻ���

EXT StStruct	SpecialThirdError[1],TempSpecialThirdError[1];///����������ϵı�־
EXT uchar SpecialThird_Error[1],SpecialThird_ErrorOld[1];///����������ϵĻ���
///�¶�����ڲ�CAN�ı�������
EXT	CAN_PACKET_T 	Inside_Reveive_Can_Packet;///��CAN�Ľ��ջ���
EXT	CAN_PACKET_T 	Inside_Send_Can_Packet;///��CAN�ķ��ͻ���														

//̥ѹģ��
///////////////TPMS=======================
#define     TYRE_COUNT                   6//   //  ��̥����

#define	  WAIT_SELECT                  0X11
#define	  SELECT                        0X22
#define	  WAIT_REPLY                   0X33  
#define	  WAIT_SUCCEED                 0X44  
#define	  TPMS_SUCCEED                 0X55  
#define	  TPMS_TimeOut                 0X66  

#define    TPMS_SA                       0X33
#define    TPMS_TIMEOUT                 13672//70S
#define     TYRE_YALI                   830//   //  ��̥��׼ѹ��
#define	  TPMS_GAOYA                   1038//TYRE_YALI*125/100 
#define	  TPMS_DIYA                    622//TYRE_YALI*75/100 
#define	  TPMS_GAOWEN                  75 
#define	OSD_ROM_Pos_excursion1	     55
#define   TMPSMenuNum 				  10
#define   TMPSLearnMenuNum				  11

#define   BMS_SA			0xd2
#define   DMC_SA			0x27

/////////////TPMS////////////////////////
EXT	uchar                   CPL_Time;                 //��ʾ��˸��
EXT	uchar                   TPMS_ID[TYRE_COUNT][3];   //ѧϰ����ID
EXT	uchar                   TPMSPosition;            //��ǰѧϰ����̥λ��
EXT	uchar                   TyreState[TYRE_COUNT];   //��̥ѧϰ״̬
EXT	uchar                   TyreStateold[TYRE_COUNT];   //��̥ѧϰ״̬
EXT TPMSMessageStruct       TyreGetData[TYRE_COUNT]; // ��������
EXT unsigned char      TPMSDisp[9];             //��ʾ��
EXT unsigned char      TPMSDispID[9];          //��ʾID��
EXT	uint                    TPMS_Learn_Time;       //ѧϰʱ��
EXT	uint                     GetTPMSTime;           //��������ʱ��
EXT	uchar                   TyreTempOld[TYRE_COUNT];  //��һ���¶���������
EXT	uchar                   TPMS_BeezTime;//���յ����������ʱ��
EXT	uchar                    MiMa_Cursor;//
EXT	uchar                    MiMa[4];
EXT	uchar                    TPMS_CAN_LearnEnd_CT;
EXT	uchar                    TyreState_Flag[TYRE_COUNT];


/*******************parameter macros**************/
///���ߵ�ʱ��
#define	CanEnterSleepDelayTimeNum					2930//5.12*2930=15s
///ģ���Ƿ�ͨѶ�ļ��ʱ��
#define	TerminalConnectDetectTimeNum			300//600
#define	RandomSetTimeNum										602
#define J1939MODULE_CONNECT_NUM						977//5.12ms*977=5s
#define WALTERsample												977//5.12ms*977=5s
///������
#define	FualLowNum														8//10//max=100
#define	FualHighNum														70//10//max=100
#define OVERTACHOSPEEDVALUE									2500
#define OVERCARSPEEDVALUE										100
#define	WATER_OVER_VALUE										98
#define	AIR_PRESS_LOWER_NUM									0//��ѹ�ޱ�����//137///0.55Mpa
#define	CAR_RUN_DOOR_OPEN_NUM								5///�г���ʱ����δ�ű�����
#define	OilLowNum														5///1/10����
///����������ֵ
#define	SensorErorrData											0xfe
///�����ź������ʱ������
#define	PwmPeriodNum													16
///�������������ֵ����������ֵ
#define	SPEEDHIGHNUM													140
#define	SPEEDRESIVENUM											2
#define	ENGINEHIGHNUM												3500
#define	ENGINERESIVENUM											50
#define	OILHIGHNUM														45
#define	OILREVISENUM													2
#define	AirPressHighNum											100
#define	FualPressHighNum										100
#define	ENGINE_WATER_TEMPERATURE_HIGH			120
///���ؿ�·���·�ı�ʾ����
#define	OpenError_Num										0x55
#define	ShortError_Num									0xaa
#define	OverLoadError_Num								0xee

///����ģ���ͨѶ����ʾ
#define VehicleModuleNum				5
#define	FrontDriveErrorFlag			VehicleModuleError.b.ST0///ǰ��
#define	TopDriveErrorFlag				VehicleModuleError.b.ST1///����
#define	BackDriveErrorFlag			VehicleModuleError.b.ST2///���
#define	PowerErrorFlag					VehicleModuleError.b.ST3///���
#define	PowerContrlErrorFlag		VehicleModuleError.b.ST4///��Դ����

///J1939ģ���ͨѶ����ʾ
#define	J1939ModuleNum					8
#define	ENGINE_ECU_CANErrorFlag		J1939ModuleError[0].b.ST0///������ECU
#define	SCRCANModuleErrorFlag			J1939ModuleError[0].b.ST1	///SCR
#define	BMS_CAN_ModuleErrorFlag		J1939ModuleError[0].b.ST2///BMS
#define	DMC_CAN_ModuleErrorFlag		J1939ModuleError[0].b.ST3///DMC
#define	FULLCONTROL_CAN_ModuleErrorFlag		J1939ModuleError[0].b.ST4///����������
#define	ASSISTANT_CAN_ModuleErrorFlag			J1939ModuleError[0].b.ST5///����������
#define	ISG_CAN_ModuleErrorFlag						J1939ModuleError[0].b.ST6///ISG
#define	AirPress_ModuleErrorFlag					J1939ModuleError[0].b.ST7///̥ѹģ�����

///�������
#define	FrontAirPressWarnFlag		SecondError[0].b.ST0///ǰ��ѹ����
#define	BackAirPressWarnFlag			SecondError[0].b.ST1///����ѹ����
#define	WaterTemperatureWarnFlag	SecondError[0].b.ST2///ˮ�±���
#define	FualPressWarnFlag					SecondError[0].b.ST3///����ѹ������
#define	WaterLevelWarnFlag				SecondError[0].b.ST4///ˮλ����
#define	BatteryPressWarnFlag			SecondError[0].b.ST5///��ص�ѹ����
#define	FrontAirSensorWarnFlag		SecondError[0].b.ST6///ǰ��ѹ����������
#define	BackAirSensorWarnFlag		SecondError[0].b.ST7///����ѹ����������

#define	OilSensorWarnFlag					SecondError[1].b.ST0///��������������
#define	FualPressSensorWarnFlag	SecondError[1].b.ST1///����ѹ������������
///����������
#define	LeftStopHoofWarnFlag							SpecialSecondError[0].b.ST0///��ɲ��Ƭ
#define	RightStopHoofWarnFlag						SpecialSecondError[0].b.ST1///��ɲ��Ƭ
#define	DoorBumpLowAirPressWarnFlag			SpecialSecondError[0].b.ST2///�űõ���ѹ
#define	GearBoxWarnFlag										SpecialSecondError[0].b.ST3///���������
#define	WCIndicateWarnFlag								SpecialSecondError[0].b.ST4///������ˮλ
#define	EngineRoomTemperatureWarnFlag		SpecialSecondError[0].b.ST5///����������
#define	DelaySpeedTemperatureWarnFlag		SpecialSecondError[0].b.ST6///����������
#define	AirFiltrateWarnFlag								SpecialSecondError[0].b.ST7///������

#define	RightBackStopHoofWarnFlag				SpecialSecondError[1].b.ST0///�Һ���Ƭ����
#define	RightFrontStopHoofWarnFlag			SpecialSecondError[1].b.ST1///��ǰ��Ƭ���� 
#define	LeftBackStopHoofWarnFlag				SpecialSecondError[1].b.ST2///�����Ƭ����
#define	LeftFrontStopHoofWarnFlag				SpecialSecondError[1].b.ST3///��ǰ��Ƭ���� 

///����������ϣ�ֻ������û����ʾ
#define	SOC_LOWER_Flag										SpecialThirdError[0].b.ST0	///SOC���ͱ���
#define	CatalystTankLevel_LOWER_Flag		SpecialThirdError[0].b.ST1	///����Һλ���ͱ���
#define	FrontDoorOpenSignalFlag					SpecialThirdError[0].b.ST2	///����>5km/h��ǰ��δ�ر���
#define	MiddleDoorOpenSignalFlag					SpecialThirdError[0].b.ST3	///����>5km/h������δ�ر���
/*******************send kind macros**************/
/*
#define	SendSelfID												0
#define	SendRainScratch									1
#define	SendMeterSwich									2
#define	SendSleepFlash									3
#define	SendMeterLed											4
#define	SendVolatageDistance						5
#define	SendJ1939_RotationWaterTemp		6
#define	SendRainScratchToTest					7
#define	SendPCFlash											8
#define	SendBakSwitch										9
#define bootloader_enter								10
#define sendJ1939_oilwear  							11
#define sendloadstatus									12
#define sendresetback										13
#define sendrequireID               			14
#define	SendEepromEraseOk								15
#define	SendConfigData1									16
#define	SendConfigData2									17
#define	SendConfigData3									18
#define	SendPanelTemperatureSensor    19	
#define	SendBootloaderAck								20
*/
///�¶�����ڲ�CAN��ID������
#define METER_ID 												0x10ff0117
#define FrontDriver_ID 									0x10ff0121
#define BackDriver_ID 									0x10ff0221
#define TopDriver_ID 										0x10ff0321
#define Power_ID 												0x10ff0421
#define PowerContrl_ID 									0x10ff0621

#define CarSpeedCanMessage		 					0xFF

#define CarSpeed_ID 										0x10fe6cee
#define EngineSpeed_ID 									0x10f004ee
/*
#define Meter_MOUDULE_ID								0XFE
#define FrontDriver_MOUDULE_ID					0x00
#define BackDriver_MOUDULE_ID						0x01
#define TopDriver_MOUDULE_ID						0x02
#define Power_MOUDULE_ID								0x03
#define PowerContrl_MOUDULE_ID					0x06
#define PC_MOUDULE_ID										0xAA
*/
#define	SendBootloaderAck								20
#define	SendMeterSwich									0X20
#define	SendCarBodySwich								0X20
#define Sendloadstatus									0x21
#define Sendloadstatus1									0x22
#define SendSensorstatus								0x23
#define SendCurrentstatus								0x24
#define SendCurrentstatus1							0x25
#define SendCurrentstatus2							0x26
#define	SendSleepFlash									0X27
#define	SendCarSpeed									  0X28
#define	SendRotation									  0X29
#define	SendRainScratch									0X2A
#define SendJ1939_oilwear  							0x2B
#define SendJ1939_oilwear1  						0x2C
#define SendDistance  									0x2D
#define	sendrequireID										0x2E
#define	SendSelfID 											0x2F
#define	SendSelfID1 										0x30
#define	SendConfigData									0x31
#define	SendVirtualSwich								0x32

/*********************IO macros****************************/	
#define WDT_IO				DDRK_DDRK6
#define WDT 					PORTK_PK6

#define	CAN1_RS_IO		DDRE_DDRE6
#define	CAN1_RS			PORTE_PE6

#define	CAN0_RS_IO		DDRM_DDRM1
#define	CAN0_RS			PTM_PTM1


