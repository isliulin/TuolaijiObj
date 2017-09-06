#undef	EXT
#ifdef	MESSAGE_C
#define	EXT
#else
#define	EXT extern
#endif

//配置电流值
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
#define	DRIVE_KEY_NUM											2		///共13个
#define	DRIVELOADNUM												5		///5*4=20
#define	POWER_LOAD_NUM										5		///5*4=20
#define	POWERCONTRL_LOAD_NUM							3		///3*4=12
#define	POWER_FUSE_NUM										5
#define	FIRST_ERROR_NUM										22	///20*8
#define MODULE_CONNECT_NUM								3		///3*8；车身设定为8个，J1939的通讯为16个
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
///HNS-ZB209新定义的函数
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
EXT	uint 		SystemVolatage;///系统电压
EXT	uchar 	VehicleRotation[2];
EXT	uchar   rains_count;     ///发送雨刮的等待其他节点的ACK时间
EXT	SelfKeyStruct 	FrontDriveKey[3],BackDriveKey[3],TopDriveKey[3],PowerKey[1],PowerContrlKey[1];///模块开关量缓冲
EXT	uchar 	FrontSensorData[7],BackSensorData[7],TopSensorData[7],PowerSensorData[7],PowerContrlSensorData[7];///模块传感器缓冲
EXT	LoadStStruct 	FrontDriveLoad[DRIVELOADNUM],FrontDriveLoadBuf[DRIVELOADNUM],///前驱模块负载状态缓冲
						BackDriveLoad[DRIVELOADNUM],BackDriveLoadBuf[DRIVELOADNUM],	///后驱模块负载状态缓冲
						TopDriveLoad[DRIVELOADNUM],TopDriveLoadBuf[DRIVELOADNUM];		///顶驱模块负载状态缓冲
EXT	LoadStStruct 	PowerLoad[POWER_LOAD_NUM],PowerLoadBuf[POWER_LOAD_NUM],///配电模块负载状态缓冲
						PowerFuse[POWER_FUSE_NUM],PowerFuseBuf[POWER_FUSE_NUM];	///配电模块保险丝状态缓冲
EXT	LoadStStruct 	PowerContrlLoad[POWERCONTRL_LOAD_NUM],PowerContrlLoadBuf[POWERCONTRL_LOAD_NUM];///电源管理模块
EXT	uint 		 PowerContrlCurrent[8];				 ///电源管理模块电流
EXT	StStruct PowerContrlOverLoadFlag;
EXT	uchar 	SmallErrorOld[FIRST_ERROR_NUM];///一类故障的缓冲
EXT	uchar 	SmallError[FIRST_ERROR_NUM];
EXT	uchar 	OpenError[FIRST_ERROR_NUM*6];///开路故障的标志缓冲；*4表示开路状态与开路所用标志的缓冲的倍数

EXT	uint 	  MainMenuLogoTime;
EXT	uchar 	MainMenuDis_count;///主界面故障
EXT	uint 	  MainMenuDis_countDelay;///主界面故障延时轮显
EXT	uchar 	dis_count;///故障的数量
EXT	uchar 	dis_content[MulfunctionTotalNum];///故障的编号
EXT	unsigned char	SendConfigDataNum;///配置的内容帧数
EXT	unsigned char	EepromConfigData[4];///里程数值配置
////新定义变量
EXT unsigned char SendSelfIdNum;
EXT unsigned char ConfigDataResult;
EXT unsigned char Video_Power_Detect_Time[2];///检测CVBS两种视频的时间变量
EXT unsigned char Speed_PWM_Detect_Time;///检测车速及脉冲输出的时间变量
EXT unsigned int	ECU_ModuleCheck_Time;///发动机ECU通讯检测时间
EXT uint	SCRModuleDetectTime;///SCR通讯检测时间
EXT uint	Delay_BMS_ModuleCheck;///BMS通讯检测时间
EXT uint	Delay_DMC_ModuleCheck;///DMC通讯检测时间
EXT uint 	FrontModuleCheck_Time,TopModuleCheck_Time,BackModuleCheck_Time,PowerModuleCheck_Time,PowerContrlModuleCheck_Time;///车身模块通讯检测时间
EXT StStruct 	VehicleModuleError,TempVehicleModuleError;///车身模块故障的标志缓冲
EXT StStruct 	J1939ModuleError[2],TempJ1939ModuleError[2];///J1939网络节点故障的标志缓冲
EXT uchar First_Error[MODULE_CONNECT_NUM],First_ErrorOld[MODULE_CONNECT_NUM];///一类故障的缓冲
EXT StStruct	SecondError[SECOND_ERROR_NUM],TempSecondError[SECOND_ERROR_NUM];///二类故障的缓冲
EXT uchar Second_Error[SECOND_ERROR_NUM],Second_ErrorOld[SECOND_ERROR_NUM];

EXT StStruct	SpecialSecondError[2],TempSpecialSecondError[2];///特殊二类故障的标志
EXT uchar SpecialSecond_Error[2],SpecialSecond_ErrorOld[2];///特殊二类故障的缓冲

EXT StStruct	SpecialThirdError[1],TempSpecialThirdError[1];///特殊三类故障的标志
EXT uchar SpecialThird_Error[1],SpecialThird_ErrorOld[1];///特殊三类故障的缓冲
///新定义的内部CAN的变量定义
EXT	CAN_PACKET_T 	Inside_Reveive_Can_Packet;///内CAN的接收缓冲
EXT	CAN_PACKET_T 	Inside_Send_Can_Packet;///内CAN的发送缓冲														

//胎压模块
///////////////TPMS=======================
#define     TYRE_COUNT                   6//   //  轮胎个数

#define	  WAIT_SELECT                  0X11
#define	  SELECT                        0X22
#define	  WAIT_REPLY                   0X33  
#define	  WAIT_SUCCEED                 0X44  
#define	  TPMS_SUCCEED                 0X55  
#define	  TPMS_TimeOut                 0X66  

#define    TPMS_SA                       0X33
#define    TPMS_TIMEOUT                 13672//70S
#define     TYRE_YALI                   830//   //  轮胎标准压力
#define	  TPMS_GAOYA                   1038//TYRE_YALI*125/100 
#define	  TPMS_DIYA                    622//TYRE_YALI*75/100 
#define	  TPMS_GAOWEN                  75 
#define	OSD_ROM_Pos_excursion1	     55
#define   TMPSMenuNum 				  10
#define   TMPSLearnMenuNum				  11

#define   BMS_SA			0xd2
#define   DMC_SA			0x27

/////////////TPMS////////////////////////
EXT	uchar                   CPL_Time;                 //显示闪烁用
EXT	uchar                   TPMS_ID[TYRE_COUNT][3];   //学习到的ID
EXT	uchar                   TPMSPosition;            //当前学习的轮胎位置
EXT	uchar                   TyreState[TYRE_COUNT];   //轮胎学习状态
EXT	uchar                   TyreStateold[TYRE_COUNT];   //轮胎学习状态
EXT TPMSMessageStruct       TyreGetData[TYRE_COUNT]; // 报文数据
EXT unsigned char      TPMSDisp[9];             //显示用
EXT unsigned char      TPMSDispID[9];          //显示ID用
EXT	uint                    TPMS_Learn_Time;       //学习时间
EXT	uint                     GetTPMSTime;           //接收数据时间
EXT	uchar                   TyreTempOld[TYRE_COUNT];  //上一次温度用来测试
EXT	uchar                   TPMS_BeezTime;//接收到新数据响的时间
EXT	uchar                    MiMa_Cursor;//
EXT	uchar                    MiMa[4];
EXT	uchar                    TPMS_CAN_LearnEnd_CT;
EXT	uchar                    TyreState_Flag[TYRE_COUNT];


/*******************parameter macros**************/
///休眠的时间
#define	CanEnterSleepDelayTimeNum					2930//5.12*2930=15s
///模块是否通讯的检测时间
#define	TerminalConnectDetectTimeNum			300//600
#define	RandomSetTimeNum										602
#define J1939MODULE_CONNECT_NUM						977//5.12ms*977=5s
#define WALTERsample												977//5.12ms*977=5s
///报警点
#define	FualLowNum														8//10//max=100
#define	FualHighNum														70//10//max=100
#define OVERTACHOSPEEDVALUE									2500
#define OVERCARSPEEDVALUE										100
#define	WATER_OVER_VALUE										98
#define	AIR_PRESS_LOWER_NUM									0//气压无报警点//137///0.55Mpa
#define	CAR_RUN_DOOR_OPEN_NUM								5///有车速时，门未门报警点
#define	OilLowNum														5///1/10报警
///传感器故障值
#define	SensorErorrData											0xfe
///车速信号输出的时钟周期
#define	PwmPeriodNum													16
///各传感器的最大值及接收修正值
#define	SPEEDHIGHNUM													140
#define	SPEEDRESIVENUM											2
#define	ENGINEHIGHNUM												3500
#define	ENGINERESIVENUM											50
#define	OILHIGHNUM														45
#define	OILREVISENUM													2
#define	AirPressHighNum											100
#define	FualPressHighNum										100
#define	ENGINE_WATER_TEMPERATURE_HIGH			120
///负载开路与短路的表示方法
#define	OpenError_Num										0x55
#define	ShortError_Num									0xaa
#define	OverLoadError_Num								0xee

///车身模块的通讯与否标示
#define VehicleModuleNum				5
#define	FrontDriveErrorFlag			VehicleModuleError.b.ST0///前控
#define	TopDriveErrorFlag				VehicleModuleError.b.ST1///顶控
#define	BackDriveErrorFlag			VehicleModuleError.b.ST2///后控
#define	PowerErrorFlag					VehicleModuleError.b.ST3///配电
#define	PowerContrlErrorFlag		VehicleModuleError.b.ST4///电源管理

///J1939模块的通讯与否标示
#define	J1939ModuleNum					8
#define	ENGINE_ECU_CANErrorFlag		J1939ModuleError[0].b.ST0///发动机ECU
#define	SCRCANModuleErrorFlag			J1939ModuleError[0].b.ST1	///SCR
#define	BMS_CAN_ModuleErrorFlag		J1939ModuleError[0].b.ST2///BMS
#define	DMC_CAN_ModuleErrorFlag		J1939ModuleError[0].b.ST3///DMC
#define	FULLCONTROL_CAN_ModuleErrorFlag		J1939ModuleError[0].b.ST4///整车控制器
#define	ASSISTANT_CAN_ModuleErrorFlag			J1939ModuleError[0].b.ST5///辅助控制器
#define	ISG_CAN_ModuleErrorFlag						J1939ModuleError[0].b.ST6///ISG
#define	AirPress_ModuleErrorFlag					J1939ModuleError[0].b.ST7///胎压模块故障

///二类故障
#define	FrontAirPressWarnFlag		SecondError[0].b.ST0///前气压报警
#define	BackAirPressWarnFlag			SecondError[0].b.ST1///后气压报警
#define	WaterTemperatureWarnFlag	SecondError[0].b.ST2///水温报警
#define	FualPressWarnFlag					SecondError[0].b.ST3///机油压力报警
#define	WaterLevelWarnFlag				SecondError[0].b.ST4///水位报警
#define	BatteryPressWarnFlag			SecondError[0].b.ST5///电池电压报警
#define	FrontAirSensorWarnFlag		SecondError[0].b.ST6///前气压传感器故障
#define	BackAirSensorWarnFlag		SecondError[0].b.ST7///后气压传感器故障

#define	OilSensorWarnFlag					SecondError[1].b.ST0///油量传感器故障
#define	FualPressSensorWarnFlag	SecondError[1].b.ST1///机油压力传感器故障
///特殊二类故障
#define	LeftStopHoofWarnFlag							SpecialSecondError[0].b.ST0///左刹车片
#define	RightStopHoofWarnFlag						SpecialSecondError[0].b.ST1///右刹车片
#define	DoorBumpLowAirPressWarnFlag			SpecialSecondError[0].b.ST2///门泵低气压
#define	GearBoxWarnFlag										SpecialSecondError[0].b.ST3///变速箱故障
#define	WCIndicateWarnFlag								SpecialSecondError[0].b.ST4///卫生间水位
#define	EngineRoomTemperatureWarnFlag		SpecialSecondError[0].b.ST5///发动机仓温
#define	DelaySpeedTemperatureWarnFlag		SpecialSecondError[0].b.ST6///缓速器高温
#define	AirFiltrateWarnFlag								SpecialSecondError[0].b.ST7///空滤器

#define	RightBackStopHoofWarnFlag				SpecialSecondError[1].b.ST0///右后蹄片报警
#define	RightFrontStopHoofWarnFlag			SpecialSecondError[1].b.ST1///右前蹄片报警 
#define	LeftBackStopHoofWarnFlag				SpecialSecondError[1].b.ST2///左后蹄片报警
#define	LeftFrontStopHoofWarnFlag				SpecialSecondError[1].b.ST3///左前蹄片报警 

///特殊三类故障，只有声音没有显示
#define	SOC_LOWER_Flag										SpecialThirdError[0].b.ST0	///SOC过低报警
#define	CatalystTankLevel_LOWER_Flag		SpecialThirdError[0].b.ST1	///尿素液位过低报警
#define	FrontDoorOpenSignalFlag					SpecialThirdError[0].b.ST2	///车速>5km/h及前门未关报警
#define	MiddleDoorOpenSignalFlag					SpecialThirdError[0].b.ST3	///车速>5km/h及中门未关报警
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
///新定义的内部CAN的ID及功能
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


