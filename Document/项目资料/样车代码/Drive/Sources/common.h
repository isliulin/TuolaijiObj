#ifndef	_common_h
#define		_common_h

#include 	"hidef.h" 		/* for EnableInterrupts macro */
#include 	"derivative.h" /* include peripheral declarations */
#include	"typedef.h"
#include	"BitMacros.h"
#include	"SensorDeal.h"
#include	"CpuInit.h"
#include	"MessageDeal.h"

#ifndef	_NO_COMMON_EXTERN
#define		_COMMON_EXTERN	extern
#else
#define		_COMMON_EXTERN
#endif
/***************Function macros********************/
void	delay_us(word delay);
void	External_WatchDog(void);
/*****************ram macros********************/
_COMMON_EXTERN	unsigned	char	Time_SendKey;

_COMMON_EXTERN	unsigned	char	Time_SendSt;

_COMMON_EXTERN	unsigned	char	Time_SendSensor;

_COMMON_EXTERN	unsigned	char	StDetectOpen_1s;	

_COMMON_EXTERN	unsigned	char	Time_Send_Speed;

_COMMON_EXTERN	unsigned	char	Time_Send_Rotation;

_COMMON_EXTERN	unsigned int	Time_Change_Oil;

_COMMON_EXTERN	unsigned char	Time_Change_FuelLevel;

_COMMON_EXTERN	unsigned char	Time_Change_GasPress1,Time_Change_GasPress2,Time_Change_Temperature;
_COMMON_EXTERN	unsigned char	Time_EngineRoomTemp,Time_EngineWaterTemp;
/*****************IO macros******************/
#define		External_Wdg_Io	PTFDD_PTFDD4
#define		External_Wdg			PTFD_PTFD4
/**************parameter macros********************/
#define		NUM_CaptureNoDetect			30		
#define		Num_StDetect						60
#define		NUM_300MS							20
#define		Num_Speed_Send					5		
#define		Num_Rotation_Send				5	
#define		Num_PowerOff						1000
#define		Num_FuelLevel_Change		50
#define		Num_OilSensor_KeyOffChange		50
#define		Num_OilSensor_KeyOnChange		1500
#define		Num_GasPress_Change					50
#define		Num_Temperature_Change				50
#define 	Num_EngineRoomTemp_Change	50
#define		Num_IntermissionTime					50
#define		Num_WashTime									400
#define		NUM_KEYADC									3
#define		NUM_2S												200
#define 	NUM_3S												300

#define	Num_RainScratchOff						5
#define	Num_RainScratchRealyOff			50
#define	Num_ResetError							1000
#define	Num_Intermission							100
#define	NUMINTERMISSIONADITIONAL	2
#define	Num_SensorbilityLoadStart			50
#define	NUM_AIRER_START						100
#define NUM_SchoolBusBerth_Time		33
/*******************************************************************
功能描述：对转速是从硬接线还是CAN报文做配置
********************************************************************/
///1->硬接线；0->CAN报文
#define		SENSOR_INFORM_IN		1
/*******************************************************************
功能描述：对雨刮是否要在驱动模块输出做配置
********************************************************************/
///1->需要输出；0->不需要输出
#define		RainScratch_Out_From		0
/*******************key macros************************/
#define		KeyOn			1
#define		KeyOff			0
///Power key Redefine
#define		FrontKey1								FrontKeyData[0].b.Key0
#define		FrontKey2								FrontKeyData[0].b.Key1
#define		FrontKey3								FrontKeyData[0].b.Key2
#define		FrontKey4								FrontKeyData[0].b.Key3
#define		FrontKey5								FrontKeyData[0].b.Key4
#define		FrontKey6								FrontKeyData[0].b.Key5
#define		FrontKey7								FrontKeyData[0].b.Key6
#define		FrontKey8								FrontKeyData[0].b.Key7
#define		FrontKey9								FrontKeyData[1].b.Key0
#define		FrontKey10								FrontKeyData[1].b.Key1
#define		FrontKey11								FrontKeyData[1].b.Key2
#define		FrontKey12								FrontKeyData[1].b.Key3
#define		FrontKey13								FrontKeyData[1].b.Key4
#define		FrontKey14								FrontKeyData[1].b.Key5
#define		FrontKey15								FrontKeyData[1].b.Key6
#define		FrontKey16								FrontKeyData[1].b.Key7
///Back Key Redefine
#define		BackKey1									BackKeyData[0].b.Key0
#define		BackKey2									BackKeyData[0].b.Key1
#define		BackKey3									BackKeyData[0].b.Key2
#define		BackKey4									BackKeyData[0].b.Key3
#define		BackKey5									BackKeyData[0].b.Key4
#define		BackKey6									BackKeyData[0].b.Key5
#define		BackKey7									BackKeyData[0].b.Key6
#define		BackKey8									BackKeyData[0].b.Key7
#define		BackKey9									BackKeyData[1].b.Key0
#define		BackKey10								BackKeyData[1].b.Key1
#define		BackKey11								BackKeyData[1].b.Key2
#define		BackKey12								BackKeyData[1].b.Key3
#define		BackKey13								BackKeyData[1].b.Key4
#define		BackKey14								BackKeyData[1].b.Key5
#define		BackKey15								BackKeyData[1].b.Key6
#define		BackKey16								BackKeyData[1].b.Key7	
///Top Key Redefine
#define		TopKey1									TopKeyData[0].b.Key0
#define		TopKey2									TopKeyData[0].b.Key1
#define		TopKey3									TopKeyData[0].b.Key2
#define		TopKey4									TopKeyData[0].b.Key3
#define		TopKey5									TopKeyData[0].b.Key4
#define		TopKey6									TopKeyData[0].b.Key5
#define		TopKey7									TopKeyData[0].b.Key6
#define		TopKey8									TopKeyData[0].b.Key7
#define		TopKey9									TopKeyData[1].b.Key0
#define		TopKey10									TopKeyData[1].b.Key1
#define		TopKey11									TopKeyData[1].b.Key2
#define		TopKey12									TopKeyData[1].b.Key3
#define		TopKey13									TopKeyData[1].b.Key4
#define		TopKey14									TopKeyData[1].b.Key5
#define		TopKey15									TopKeyData[1].b.Key6
#define		TopKey16									TopKeyData[1].b.Key7
///Meter Key Redefine
#define		MeterKey1									MeterKeyData[0].b.Key0
#define		MeterKey2									MeterKeyData[0].b.Key1
#define		MeterKey3									MeterKeyData[0].b.Key2
#define		MeterKey4									MeterKeyData[0].b.Key3
#define		MeterKey5									MeterKeyData[0].b.Key4
#define		MeterKey6									MeterKeyData[0].b.Key5
#define		MeterKey7									MeterKeyData[0].b.Key6
#define		MeterKey8									MeterKeyData[0].b.Key7
#define		MeterKey9									MeterKeyData[1].b.Key0
#define		MeterKey10									MeterKeyData[1].b.Key1
#define		MeterKey11									MeterKeyData[1].b.Key2
#define		MeterKey12									MeterKeyData[1].b.Key3
#define		MeterKey13									MeterKeyData[1].b.Key4
#define		MeterKey14									MeterKeyData[1].b.Key5
#define		MeterKey15									MeterKeyData[1].b.Key6
#define		MeterKey16									MeterKeyData[1].b.Key7
#define		MeterKey17									MeterKeyData[2].b.Key0
#define		MeterKey18									MeterKeyData[2].b.Key1
#define		MeterKey19									MeterKeyData[2].b.Key2
#define		MeterKey20									MeterKeyData[2].b.Key3
#define		MeterKey21									MeterKeyData[2].b.Key4
#define		MeterKey22									MeterKeyData[2].b.Key5
#define		MeterKey23									MeterKeyData[2].b.Key6
#define		MeterKey24									MeterKeyData[2].b.Key7
#define		MeterKey25									MeterKeyData[3].b.Key0
#define		MeterKey26									MeterKeyData[3].b.Key1
#define		MeterKey27									MeterKeyData[3].b.Key2
#define		MeterKey28									MeterKeyData[3].b.Key3
#define		MeterKey29									MeterKeyData[3].b.Key4
#define		MeterKey30									MeterKeyData[3].b.Key5
#define		MeterKey31									MeterKeyData[3].b.Key6
#define		MeterKey32									MeterKeyData[3].b.Key7
#define		MeterKey33									MeterKeyData[4].b.Key0
#define		MeterKey34									MeterKeyData[4].b.Key1
#define		MeterKey35									MeterKeyData[4].b.Key2
#define		MeterKey36									MeterKeyData[4].b.Key3
#define		MeterKey37									MeterKeyData[4].b.Key4
#define		MeterKey38									MeterKeyData[4].b.Key5
#define		MeterKey39									MeterKeyData[4].b.Key6
#define		MeterKey40									MeterKeyData[4].b.Key7
///仪表虚拟开关重定义
#define		MeterDummyKey1							MeterDummyKeyData[0].b.Key0
#define		MeterDummyKey2							MeterDummyKeyData[0].b.Key1
#define		MeterDummyKey3							MeterDummyKeyData[0].b.Key2
#define		MeterDummyKey4							MeterDummyKeyData[0].b.Key3
#define		MeterDummyKey5							MeterDummyKeyData[0].b.Key4
#define		MeterDummyKey6							MeterDummyKeyData[0].b.Key5
#define		MeterDummyKey7							MeterDummyKeyData[0].b.Key6
#define		MeterDummyKey8							MeterDummyKeyData[0].b.Key7
///power control key redefine
#define		PowerControlKey1						PowerControlKeyData[0].b.Key0
#define		PowerControlKey2						PowerControlKeyData[0].b.Key1
#define		PowerControlKey3						PowerControlKeyData[0].b.Key2
#define		PowerControlKey4						PowerControlKeyData[0].b.Key3
#define		PowerControlKey5						PowerControlKeyData[0].b.Key4
#define		PowerControlKey6						PowerControlKeyData[0].b.Key5
#define		PowerControlKey7						PowerControlKeyData[0].b.Key6
#define		PowerControlKey8						PowerControlKeyData[0].b.Key7
///Multi Function key redefine
#define		MultiFunctionKey1						MultiFunctionKeyData[0].b.Key0
#define		MultiFunctionKey2						MultiFunctionKeyData[0].b.Key1
#define		MultiFunctionKey3						MultiFunctionKeyData[0].b.Key2
#define		MultiFunctionKey4						MultiFunctionKeyData[0].b.Key3
#define		MultiFunctionKey5						MultiFunctionKeyData[0].b.Key4
#define		MultiFunctionKey6						MultiFunctionKeyData[0].b.Key5
#define		MultiFunctionKey7						MultiFunctionKeyData[0].b.Key6
#define		MultiFunctionKey8						MultiFunctionKeyData[0].b.Key7

#define RainScratchHighSpeedKeyBit				MeterKey28///雨刮高速开关
#define RainScratchIntermissionKeyBit			MeterKey27///雨刮间歇开关
#define RainScratchLowSpeedKeyBit					MeterKey19///雨刮低速开关
#define FrontDoorOpenKeyBit								MeterKey26///前门打开开关
#define FrontDoorCloseKeyBit	        		MeterKey25///前门关闭开关
#define LocationLampKeyBit             		MeterKey17///位置灯开关
#define LeftTurnLampKeyBit								MeterKey22///左转向灯开关
#define RightTurnLampKeyBit            		MeterKey18///右转向灯开关
#define BackFogKeyBit		   	 							MeterKey21///后雾灯开关
#define	ABSMulfunctionIndicateKeyBit			MeterKey32///ABS故障指示开关
#define	ASRMulfunctionIndicateKeyBit			MeterKey29///ASR故障指示开关
#define	FrontFogKeyBit	   					    	MeterKey1///前雾灯开关
#define	DelaySpeedWorkIndicateKeyBit			MeterKey16///缓速器工作指示	
#define	EngineRoomTemperatureKeyBit			 	MeterKey2///仓温报警信号
#define	Defrost1KeyBit										MeterKey15///除霜1开关
#define Defrost2KeyBit										MeterKey3///除霜2开关
#define DangerWarnKeyBit			        		MeterKey12///危急报警开关
#define SyringeKeyBit											MeterKey11///洗涤器开关
#define AccKeyBit   				        			MeterKey7///ACC档开关
#define	OnKeyBit													MeterKey10///ON档开关
#define StartKeyBit 			   	 						MeterKey8///ST 启动开关

//前驱模块
#define BakOnKeyBit												FrontKey1///备用ON档开关
#define BakStartKeyBit 			   	 					FrontKey3///备用ST启动开关
#define BakDangerWarnKeyBit			        	FrontKey5///备用危急报警开关
#define FarLampKeyBit	 					 					FrontKey9///远光灯开关
#define NearLampKeyBit										FrontKey11///近光灯开关

#define BugleKeyBit												FrontKey2///喇叭按钮
#define FrontDoorOpenSignalKeyBit					FrontKey4///前门开信号开关
#define	FrontAirPressKeyBit								FrontKey8///前气压报警开关
#define	FrontAirPressWarnKeyBit 					FrontKey8///前气压报警开关
#define BackAirPressKeyBit								FrontKey10///后气压报警
#define BackAirPressWarnKeyBit						FrontKey10///后气压报警
#define ReadLampKeyBit										FrontKey12///阅读灯开关

//后驱模块
 
#define RShieldSignalKeyBit								BackKey1///R档信号开关
#define	NShieldSignalKeyBit								BackKey3///N档信号开关
#define	AirFiltrateWarnKeyBit							BackKey5///空滤报警开关
#define BackRoomKeyBit										BackKey7///后仓门开关
#define EngineWaterWarnKeyBit							BackKey9///水温报警开关
#define	HandStopKeyBit										BackKey11///手刹车开关

#define	BackFlameoutKeyBit								BackKey2///后熄火开关
#define	BackStartKeyBit										BackKey4///后启动开关
#define FootStopKeyBit				  	 				BackKey8///刹车灯开关
#define FualPressWarnKeyBit								BackKey10///机油压力报警开关
#define LowWaterLevelKeyBit								BackKey13///低水位报警开关
#define EngineLowerWaterKeyBit						BackKey13///低水位报警开关
#define	EngineChargeIndicateKeyBit				BackKey15///发动机充电指示灯

#if 0
////meter key realy
#define	 FrontFogKeyBit	   					    		MeterKey1///前雾灯开关
#define	 EngineRoomTemperatureKeyBit		MeterKey2///仓温报警信号
#define	 Coolant1KeyBit										MeterKey3///散热器1开关
///#define	MeetAnEmergencyKeyBit						MeterKey4
#define	 InsideRoomLamp2KeyBit					MeterKey5///室内灯2开关
#define RainScratchHighSpeedKeyBit				MeterKey6///雨刮高速开关
#define AccKeyBit   				        					MeterKey7///ACC档开关
#define BackFogKeyBit		   	 							MeterKey8///后雾灯开关
///#define	PowerTotalKeyBit									MeterKey9
#define	 OnKeyBit												MeterKey10///ON档开关
#define SyringeKeyBit										MeterKey11///洗涤器开关
#define DangerWarnKeyBit			        			MeterKey12///危急报警开关
#define InsideRoomLamp1KeyBit					MeterKey13///室内灯1开关
#define Coolant2KeyBit   									MeterKey14///散热器2开关
#define InsideRoomLamp3KeyBit					MeterKey15///室内灯3开关
///#define BackFogKeyBit										MeterKey16 
#define LocationLampKeyBit             				MeterKey17///位置灯开关
#define RightTurnLampKeyBit            			MeterKey18///右转向灯开关
#define RainScratchLowSpeedKeyBit				MeterKey19///雨刮低速开关
#define GuidepostLampKeyBit							MeterKey20///路牌灯开关
#define TVKeyBit 			   	 								MeterKey21///TV开关
#define LeftTurnLampKeyBit							MeterKey22///左转向灯开关
#define BackDoorCloseKeyBit							MeterKey23///中门关闭开关
#define BackDoorOpenKeyBit							MeterKey24///中门打开开关
#define FrontDoorCloseKeyBit	        				MeterKey25///前门关闭开关
#define FrontDoorOpenKeyBit							MeterKey26///前门打开开关
#define RainScratchIntermissionKeyBit			MeterKey27///雨刮间歇开关
#define MotorManKeyBit									MeterKey28///司机灯开关
#define	ASRMulfunctionIndicateKeyBit			MeterKey29///ASR故障指示开关
///#define	MeterBakKey3										MeterKey30
///#define	EngineMILWarnKeyBit							MeterKey31
#define	ABSMulfunctionIndicateKeyBit			MeterKey32///ABS故障指示开关
///#define	MeterBakKey6										MeterKey33
///#define	Defrost2KeyBit										MeterKey34
///#define	Defrost1KeyBit										MeterKey35
///#define	HandCoinBoxKeyBit								MeterKey36
///#define	CoinBoxLampKeyBit								MeterKey37
///#define	FannerKeyBit										MeterKey38
///#define	MeterBakKey7										MeterKey39
///仪表虚拟开关
#define		NShieldSignalKeyBit							MeterDummyKey1///空档开关
#define		RShieldSignalKeyBit							MeterDummyKey2///倒档开关
////front drive key
///#define RetarderWorkIndicateKeyBit					FrontKey1
#define LeftBackStopHoofKeyBit						FrontKey1///左后制动蹄片报警开关
#define LeftFrontStopHoofKeyBit					FrontKey2///左前制动蹄片报警开关
#define BakOnKeyBit											FrontKey3///备用ON档开关
#define	 FrontAirPressKeyBit								FrontKey4///前气压报警开关
#define RightBackStopHoofKeyBit					FrontKey5///右后制动蹄片报警开关
///#define RightFrontStopHoofKeyBit					FrontKey6
#define BackAirPressKeyBit								FrontKey6///后气压报警
#define BakDangerWarnKeyBit						FrontKey7///备用双闪报警开关
#define	 Defrost1KeyBit										FrontKey8///除霜1开关
#define FarLampKeyBit	 					 				FrontKey9///远光灯开关
#define BugleKeyBit											FrontKey10///喇叭按钮
#define NearLampKeyBit									FrontKey11///近光灯开关
#define Defrost2KeyBit										FrontKey12///除霜2开关
#define RightFrontStopHoofKeyBit					FrontKey13///右前制动蹄片报警开关
///#define FrontBakKey5									FrontKey14
///#define EngineChargeIndicate2KeyBit			FrontKey15
///#define FrontBakKey7									FrontKey16
////back drive key
///#define RShieldSignalKeyBit				  	 		BackKey1
///#define RightFrontStopHoofKeyBit				BackKey2
///#define NShieldSignalKeyBit							BackKey3
///#define LeftFrontStopHoofKeyBit					BackKey4
///#define	RightBackStopHoofKeyBit				BackKey5
///#define FootStopKeyBit									BackKey6
#define BackRoomKeyBit								BackKey7///后仓门开关
#define FootStopKeyBit									BackKey8///刹车灯开关
///#define BackAirPressKeyBit		 					BackKey9
///#define	FualPressWarnKeyBit						BackKey10
#define HandStopKeyBit								BackKey11///手刹车开关
///#define AirFiltrateWarnKeyBit						BackKey12
///#define LowWaterLevelKeyBit						BackKey13
///#define BackBakKey1										BackKey14
///#define EngineChargeIndicateKeyBit				BackKey15
///#define	BackBakKey2									BackKey16

////top drive key
#define FrontDoorOpenSignalKeyBit									TopKey1///前门开信号开关
///#define GotOffDoorBellKeyBit												TopKey2
#define BackDoorOpenSignalKeyBit									TopKey3///中门开信号开关
///#define FrontDoorPreventNipKeyBit										TopKey4
#define FrontDoorPreventNipKeyBit									TopKey5///前门防夹开关
///#define MeetAnEmergencyCliqueOpenSignalKeyBit				TopKey6
#define	 BackDoorPreventNipKeyBit										TopKey7///中门防夹开关
///#define ReadLampKeyBit														TopKey8
///#define FrontChargeGasReceiveKeyBit									TopKey9
///#define	RetarderHighTempKeyBit											TopKey10
///#define BackChargeGasReceiveKeyBit									TopKey11
///#define TVKeyBit																	TopKey12
///#define	TopBakKey6																TopKey13
///#define TopBakKey7																TopKey14
///#define TopBakKey8																TopKey15
///#define	TopBakKey9																TopKey16
#endif
/***********************Load macros************************/
///DLoade show drive loade
#define		DLoad1		PutData[0].Loads.Load0
#define		DLoad2		PutData[0].Loads.Load1
#define		DLoad3		PutData[0].Loads.Load2
#define		DLoad4		PutData[0].Loads.Load3
#define		DLoad5		PutData[0].Loads.Load4
#define		DLoad6		PutData[0].Loads.Load5
#define		DLoad7		PutData[0].Loads.Load6
#define		DLoad8		PutData[0].Loads.Load7

#define		DLoad9		PutData[1].Loads.Load0
#define		DLoad10		PutData[1].Loads.Load1
#define		DLoad11		PutData[1].Loads.Load2
#define		DLoad12		PutData[1].Loads.Load3
#define		DLoad13		PutData[1].Loads.Load4
#define		DLoad14		PutData[1].Loads.Load5
#define		DLoad15		PutData[1].Loads.Load6
#define		DLoad16		PutData[1].Loads.Load7

#define		DLoad17		PutData[2].Loads.Load0
#define		DLoad18		PutData[2].Loads.Load1
#define		DLoad19		PutData[2].Loads.Load2
#define		DLoad20		PutData[2].Loads.Load3
#define		DLoad21		PutData[2].Loads.Load4
#define		DLoad22		PutData[2].Loads.Load5
#define		DLoad23		PutData[2].Loads.Load6
#define		DLoad24		PutData[2].Loads.Load7
                           
#endif

