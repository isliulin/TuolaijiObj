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
������������ת���Ǵ�Ӳ���߻���CAN����������
********************************************************************/
///1->Ӳ���ߣ�0->CAN����
#define		SENSOR_INFORM_IN		1
/*******************************************************************
����������������Ƿ�Ҫ������ģ�����������
********************************************************************/
///1->��Ҫ�����0->����Ҫ���
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
///�Ǳ����⿪���ض���
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

#define RainScratchHighSpeedKeyBit				MeterKey28///��θ��ٿ���
#define RainScratchIntermissionKeyBit			MeterKey27///��μ�Ъ����
#define RainScratchLowSpeedKeyBit					MeterKey19///��ε��ٿ���
#define FrontDoorOpenKeyBit								MeterKey26///ǰ�Ŵ򿪿���
#define FrontDoorCloseKeyBit	        		MeterKey25///ǰ�Źرտ���
#define LocationLampKeyBit             		MeterKey17///λ�õƿ���
#define LeftTurnLampKeyBit								MeterKey22///��ת��ƿ���
#define RightTurnLampKeyBit            		MeterKey18///��ת��ƿ���
#define BackFogKeyBit		   	 							MeterKey21///����ƿ���
#define	ABSMulfunctionIndicateKeyBit			MeterKey32///ABS����ָʾ����
#define	ASRMulfunctionIndicateKeyBit			MeterKey29///ASR����ָʾ����
#define	FrontFogKeyBit	   					    	MeterKey1///ǰ��ƿ���
#define	DelaySpeedWorkIndicateKeyBit			MeterKey16///����������ָʾ	
#define	EngineRoomTemperatureKeyBit			 	MeterKey2///���±����ź�
#define	Defrost1KeyBit										MeterKey15///��˪1����
#define Defrost2KeyBit										MeterKey3///��˪2����
#define DangerWarnKeyBit			        		MeterKey12///Σ����������
#define SyringeKeyBit											MeterKey11///ϴ��������
#define AccKeyBit   				        			MeterKey7///ACC������
#define	OnKeyBit													MeterKey10///ON������
#define StartKeyBit 			   	 						MeterKey8///ST ��������

//ǰ��ģ��
#define BakOnKeyBit												FrontKey1///����ON������
#define BakStartKeyBit 			   	 					FrontKey3///����ST��������
#define BakDangerWarnKeyBit			        	FrontKey5///����Σ����������
#define FarLampKeyBit	 					 					FrontKey9///Զ��ƿ���
#define NearLampKeyBit										FrontKey11///����ƿ���

#define BugleKeyBit												FrontKey2///���Ȱ�ť
#define FrontDoorOpenSignalKeyBit					FrontKey4///ǰ�ſ��źſ���
#define	FrontAirPressKeyBit								FrontKey8///ǰ��ѹ��������
#define	FrontAirPressWarnKeyBit 					FrontKey8///ǰ��ѹ��������
#define BackAirPressKeyBit								FrontKey10///����ѹ����
#define BackAirPressWarnKeyBit						FrontKey10///����ѹ����
#define ReadLampKeyBit										FrontKey12///�Ķ��ƿ���

//����ģ��
 
#define RShieldSignalKeyBit								BackKey1///R���źſ���
#define	NShieldSignalKeyBit								BackKey3///N���źſ���
#define	AirFiltrateWarnKeyBit							BackKey5///���˱�������
#define BackRoomKeyBit										BackKey7///����ſ���
#define EngineWaterWarnKeyBit							BackKey9///ˮ�±�������
#define	HandStopKeyBit										BackKey11///��ɲ������

#define	BackFlameoutKeyBit								BackKey2///��Ϩ�𿪹�
#define	BackStartKeyBit										BackKey4///����������
#define FootStopKeyBit				  	 				BackKey8///ɲ���ƿ���
#define FualPressWarnKeyBit								BackKey10///����ѹ����������
#define LowWaterLevelKeyBit								BackKey13///��ˮλ��������
#define EngineLowerWaterKeyBit						BackKey13///��ˮλ��������
#define	EngineChargeIndicateKeyBit				BackKey15///���������ָʾ��

#if 0
////meter key realy
#define	 FrontFogKeyBit	   					    		MeterKey1///ǰ��ƿ���
#define	 EngineRoomTemperatureKeyBit		MeterKey2///���±����ź�
#define	 Coolant1KeyBit										MeterKey3///ɢ����1����
///#define	MeetAnEmergencyKeyBit						MeterKey4
#define	 InsideRoomLamp2KeyBit					MeterKey5///���ڵ�2����
#define RainScratchHighSpeedKeyBit				MeterKey6///��θ��ٿ���
#define AccKeyBit   				        					MeterKey7///ACC������
#define BackFogKeyBit		   	 							MeterKey8///����ƿ���
///#define	PowerTotalKeyBit									MeterKey9
#define	 OnKeyBit												MeterKey10///ON������
#define SyringeKeyBit										MeterKey11///ϴ��������
#define DangerWarnKeyBit			        			MeterKey12///Σ����������
#define InsideRoomLamp1KeyBit					MeterKey13///���ڵ�1����
#define Coolant2KeyBit   									MeterKey14///ɢ����2����
#define InsideRoomLamp3KeyBit					MeterKey15///���ڵ�3����
///#define BackFogKeyBit										MeterKey16 
#define LocationLampKeyBit             				MeterKey17///λ�õƿ���
#define RightTurnLampKeyBit            			MeterKey18///��ת��ƿ���
#define RainScratchLowSpeedKeyBit				MeterKey19///��ε��ٿ���
#define GuidepostLampKeyBit							MeterKey20///·�Ƶƿ���
#define TVKeyBit 			   	 								MeterKey21///TV����
#define LeftTurnLampKeyBit							MeterKey22///��ת��ƿ���
#define BackDoorCloseKeyBit							MeterKey23///���Źرտ���
#define BackDoorOpenKeyBit							MeterKey24///���Ŵ򿪿���
#define FrontDoorCloseKeyBit	        				MeterKey25///ǰ�Źرտ���
#define FrontDoorOpenKeyBit							MeterKey26///ǰ�Ŵ򿪿���
#define RainScratchIntermissionKeyBit			MeterKey27///��μ�Ъ����
#define MotorManKeyBit									MeterKey28///˾���ƿ���
#define	ASRMulfunctionIndicateKeyBit			MeterKey29///ASR����ָʾ����
///#define	MeterBakKey3										MeterKey30
///#define	EngineMILWarnKeyBit							MeterKey31
#define	ABSMulfunctionIndicateKeyBit			MeterKey32///ABS����ָʾ����
///#define	MeterBakKey6										MeterKey33
///#define	Defrost2KeyBit										MeterKey34
///#define	Defrost1KeyBit										MeterKey35
///#define	HandCoinBoxKeyBit								MeterKey36
///#define	CoinBoxLampKeyBit								MeterKey37
///#define	FannerKeyBit										MeterKey38
///#define	MeterBakKey7										MeterKey39
///�Ǳ����⿪��
#define		NShieldSignalKeyBit							MeterDummyKey1///�յ�����
#define		RShieldSignalKeyBit							MeterDummyKey2///��������
////front drive key
///#define RetarderWorkIndicateKeyBit					FrontKey1
#define LeftBackStopHoofKeyBit						FrontKey1///����ƶ���Ƭ��������
#define LeftFrontStopHoofKeyBit					FrontKey2///��ǰ�ƶ���Ƭ��������
#define BakOnKeyBit											FrontKey3///����ON������
#define	 FrontAirPressKeyBit								FrontKey4///ǰ��ѹ��������
#define RightBackStopHoofKeyBit					FrontKey5///�Һ��ƶ���Ƭ��������
///#define RightFrontStopHoofKeyBit					FrontKey6
#define BackAirPressKeyBit								FrontKey6///����ѹ����
#define BakDangerWarnKeyBit						FrontKey7///����˫����������
#define	 Defrost1KeyBit										FrontKey8///��˪1����
#define FarLampKeyBit	 					 				FrontKey9///Զ��ƿ���
#define BugleKeyBit											FrontKey10///���Ȱ�ť
#define NearLampKeyBit									FrontKey11///����ƿ���
#define Defrost2KeyBit										FrontKey12///��˪2����
#define RightFrontStopHoofKeyBit					FrontKey13///��ǰ�ƶ���Ƭ��������
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
#define BackRoomKeyBit								BackKey7///����ſ���
#define FootStopKeyBit									BackKey8///ɲ���ƿ���
///#define BackAirPressKeyBit		 					BackKey9
///#define	FualPressWarnKeyBit						BackKey10
#define HandStopKeyBit								BackKey11///��ɲ������
///#define AirFiltrateWarnKeyBit						BackKey12
///#define LowWaterLevelKeyBit						BackKey13
///#define BackBakKey1										BackKey14
///#define EngineChargeIndicateKeyBit				BackKey15
///#define	BackBakKey2									BackKey16

////top drive key
#define FrontDoorOpenSignalKeyBit									TopKey1///ǰ�ſ��źſ���
///#define GotOffDoorBellKeyBit												TopKey2
#define BackDoorOpenSignalKeyBit									TopKey3///���ſ��źſ���
///#define FrontDoorPreventNipKeyBit										TopKey4
#define FrontDoorPreventNipKeyBit									TopKey5///ǰ�ŷ��п���
///#define MeetAnEmergencyCliqueOpenSignalKeyBit				TopKey6
#define	 BackDoorPreventNipKeyBit										TopKey7///���ŷ��п���
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

