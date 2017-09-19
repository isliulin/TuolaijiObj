#undef	EXT
#ifdef	LOADDEAL_C
#define	EXT
#else
#define	EXT extern
#endif
/* common defines and macros */
#include <hidef.h>      	
#include <mc9s12xhz512.h>
#include	"typedef.h"
/*****************PARAMETER MACROS*****************/
#define	NUM_CAT4016							1
#define	Num_PowerDevice					1///功率器件，共有3路
#define	HighSpeedFlashDelay			20///125=12.5ms*10
#define	LowSpeedFlashDelay			40
#define	ShortNoiseDelay					20     
#define	WaterTempFlahNum				200///水温当J1939通讯异常时的闪烁系数
///load macros
#define AD_SHORT_N 							3
#define	AD_OPEN_N								100
#define	AD_ON_OPEN_N							3 	///if load is open,st detect time 3*500MS
#define	AD_NORMAL_OPEN					10//20
#define	AD_NORMAL_CLOSE					10//20
///负载状态
#define	Load_On						0b00
#define	Load_Short					0b01
#define	Load_Open					0b10
#define	Load_Off						0b11
#ifdef	LOADDEAL_C
volatile	const unsigned char NearLampConfig	@0x7F08  = 1;
//const  uchar BuzzerDutyCONST[4]={0x1b,0x12,0x0d,0x00}; 
const  uchar BuzzerDutyCONST[4]={0x35,0x2a,0x1f,0x00}; 
const  uchar LedDutyCONST[4]={0xD0,0x70,0x30,0x00}; 
#endif

/******funtion macros****************************/
#pragma CODE_SEG  USER_ROM
void	Cat4016_Drive(unsigned	int	*Cat4016_Data);
static	void  FlashLampDeal(void);
static	void  buzzer_control(void);
static	void  buzzer_cancel(void);
static void  	LedDisplay(void);
static void 	SendMeterLedProcess(void);
static void 	StTest(void);
static void		GetSTProcess(void);
static unsigned char PowerDeviceChangeData(void);
static void	PowerDeviceDrive(unsigned char	LoadState);
static void	ErrorReset(void);
void Load_Deal(void);
#pragma CODE_SEG DEFAULT
//================ram macros=======================
EXT	byte	guache_index;
EXT	word	delay10s;
EXT	byte	FlashIndex;
EXT	byte	delayxms;
EXT	byte	dirdelay;
EXT	byte	buzzer_index;
EXT	byte	buzzer_delay;
EXT	byte	buzzer_ms;
EXT	byte  Bts724St[2];
///ram new macros
EXT	byte  FlagsADError[Num_PowerDevice];
EXT	PutLoadStruct PutData[Num_PowerDevice],OldPutDtata[Num_PowerDevice];
EXT	StStruct	StData[Num_PowerDevice];
EXT	unsigned char	FlagsADNormalOpen[Num_PowerDevice][8],FlagsADNormalClose[Num_PowerDevice][8];
EXT	unsigned char FlagsADShort[Num_PowerDevice][8],FlagsADOpen[Num_PowerDevice][8];
EXT	LoadStStruct	ReState[2*Num_PowerDevice],TempReState[2*Num_PowerDevice];///2 bit show 1 state
EXT	MeterLedStruct SendMeterLedData[6];
EXT	LoadStruct Cat4016_Data[NUM_CAT4016];
EXT	LoadStruct Old_Cat4016_Data[NUM_CAT4016];
EXT	unsigned char	WaterTempFlahTime;///水温当J1939通讯异常时的闪烁时间
///新定义
EXT unsigned char BuzzerLevel;///蜂鸣器的调节
EXT unsigned char LightLevel;///背光PWM的调节
EXT unsigned int	ACCOff_Delay;///ACC档关闭的延时时间
EXT unsigned int	StartDelay_Time;///启动电源总开关动作时间
/******************io macros*****************/	
#define Cat4016_Latch 				PORTD_PD6
#define Cat4016_Sin 					PORTD_PD4
#define Cat4016_Clk					PORTD_PD5
#define Cat4016_Blank				PORTD_PD7

#define Cat4016_Latch_IO		DDRD_DDRD6
#define Cat4016_Sin_IO 			DDRD_DDRD4
#define Cat4016_Clk_IO 			DDRD_DDRD5
#define Cat4016_Blank_IO 		DDRD_DDRD7

#define OvercarspeedLEDIO      	DDRT_DDRT2
#define OvercarspeedLED         PTT_PTT2   
#define OverTachospeedLEDIO    	DDRS_DDRS0
#define OverTachospeedLED      	PTS_PTS0  

#define	FrontDoorLedIO				DDRD_DDRD0
#define	FrontDoorLed						PORTD_PD0
#define	MiddleDoorLedIO				DDRK_DDRK2
#define	MiddleDoorLed					PORTK_PK2

#define	PANNAL_LOAD1_IO					DDRS_DDRS2
#define PANNAL_LOAD1							PTS_PTS2
#define	PANNAL_LOAD2_IO					DDRS_DDRS1
#define PANNAL_LOAD2							PTS_PTS1
#define	PANNAL_LOAD3_IO					DDRP_DDRP3
#define PANNAL_LOAD3							PTP_PTP3

#define	MotorLedLeftOn					PWMDTY6=LedDutyCONST[LightLevel] 
#define	MotorLedLeftOff					PWMDTY6=LedDutyCONST[0] 

#define	MotorLedRightOn					PWMDTY7=LedDutyCONST[LightLevel] 
#define	MotorLedRightOff				PWMDTY7=LedDutyCONST[0] 

#define	SetBuzzer	PWMDTY1=BuzzerDutyCONST[BuzzerLevel]
#define	BuzzerOff	PWMDTY1=BuzzerDutyCONST[0]
///M show meter module;led1->dl201,led15->dl215
#define	TMLed1		Cat4016_Data[0].bits.bit7
#define	TMLed2		Cat4016_Data[0].bits.bit6 
#define	TMLed3		Cat4016_Data[0].bits.bit5 
#define	TMLed4		Cat4016_Data[0].bits.bit4
#define	TMLed5		Cat4016_Data[0].bits.bit3 
#define	TMLed6		Cat4016_Data[0].bits.bit2 
#define	TMLed7		Cat4016_Data[0].bits.bit1 
#define	TMLed8		Cat4016_Data[0].bits.bit0

#define	TMLed9		Cat4016_Data[0].bits.bit15 
#define	TMLed10		Cat4016_Data[0].bits.bit14 
#define	TMLed11		Cat4016_Data[0].bits.bit13
#define	TMLed12		Cat4016_Data[0].bits.bit12 
#define	TMLed13		Cat4016_Data[0].bits.bit11 
#define	TMLed14		Cat4016_Data[0].bits.bit10
#define	TMLed15		Cat4016_Data[0].bits.bit9 
#define	TMLed16		Cat4016_Data[0].bits.bit8

#define	MLed1		TMLed1
#define	MLed3		TMLed2
#define	MLed5		TMLed3 
#define	MLed7		TMLed4 
#define	MLed9		TMLed5 
#define	MLed11	TMLed6
#define	MLed13	TMLed7 
#define	MLed15	TMLed8 

#define	MLed16	TMLed9 
#define	MLed14	TMLed10 
#define	MLed12	TMLed11
#define	MLed10	TMLed12 
#define	MLed8		TMLed13 
#define	MLed6		TMLed14
#define	MLed4		TMLed15 
#define	MLed2		TMLed16
///MLoade show meter load
#define		MLoad1			PutData[0].Loads.Load0
#define		MLoad2			PutData[0].Loads.Load1
#define		MLoad3			PutData[0].Loads.Load2
#define		MLoad4			PutData[0].Loads.Load3
#define		MLoad5			PutData[0].Loads.Load4
#define		MLoad6			PutData[0].Loads.Load5
#define		MLoad7			PutData[0].Loads.Load6
#define		MLoad8			PutData[0].Loads.Load7
/*********************负载重定义*************************/
///Front Drive load state redefine
#define	FrontDriveState_1									FrontDriveLoad[0].St.S0
#define	FrontDriveState_2									FrontDriveLoad[0].St.S1
#define	FrontDriveState_3									FrontDriveLoad[0].St.S2
#define	FrontDriveState_4									FrontDriveLoad[0].St.S3

#define	FrontDriveState_5									FrontDriveLoad[1].St.S0
#define	FrontDriveState_6									FrontDriveLoad[1].St.S1
#define	FrontDriveState_7									FrontDriveLoad[1].St.S2
#define	FrontDriveState_8									FrontDriveLoad[1].St.S3

#define	FrontDriveState_9									FrontDriveLoad[2].St.S0
#define	FrontDriveState_10								FrontDriveLoad[2].St.S1
#define	FrontDriveState_11								FrontDriveLoad[2].St.S2
#define	FrontDriveState_12								FrontDriveLoad[2].St.S3

#define	FrontDriveState_13								FrontDriveLoad[3].St.S0
#define	FrontDriveState_14								FrontDriveLoad[3].St.S1
#define	FrontDriveState_15								FrontDriveLoad[3].St.S2
#define	FrontDriveState_16								FrontDriveLoad[3].St.S3

#define	FrontDriveState_17								FrontDriveLoad[4].St.S0
#define	FrontDriveState_18								FrontDriveLoad[4].St.S1
#define	FrontDriveState_19								FrontDriveLoad[4].St.S2
#define	FrontDriveState_20								FrontDriveLoad[4].St.S3
///Back Drive load state redefine
#define	BackDriveState_1									BackDriveLoad[0].St.S0
#define	BackDriveState_2									BackDriveLoad[0].St.S1
#define	BackDriveState_3									BackDriveLoad[0].St.S2
#define	BackDriveState_4									BackDriveLoad[0].St.S3

#define	BackDriveState_5									BackDriveLoad[1].St.S0
#define	BackDriveState_6									BackDriveLoad[1].St.S1
#define	BackDriveState_7									BackDriveLoad[1].St.S2
#define	BackDriveState_8									BackDriveLoad[1].St.S3

#define	BackDriveState_9									BackDriveLoad[2].St.S0
#define	BackDriveState_10									BackDriveLoad[2].St.S1
#define	BackDriveState_11									BackDriveLoad[2].St.S2
#define	BackDriveState_12									BackDriveLoad[2].St.S3

#define	BackDriveState_13									BackDriveLoad[3].St.S0
#define	BackDriveState_14									BackDriveLoad[3].St.S1
#define	BackDriveState_15									BackDriveLoad[3].St.S2
#define	BackDriveState_16									BackDriveLoad[3].St.S3

#define	BackDriveState_17									BackDriveLoad[4].St.S0
#define	BackDriveState_18									BackDriveLoad[4].St.S1
#define	BackDriveState_19									BackDriveLoad[4].St.S2
#define	BackDriveState_20									BackDriveLoad[4].St.S3
///Top Drive load state redefine
#define	TopDriveState_1										TopDriveLoad[0].St.S0
#define	TopDriveState_2										TopDriveLoad[0].St.S1
#define	TopDriveState_3										TopDriveLoad[0].St.S2
#define	TopDriveState_4										TopDriveLoad[0].St.S3

#define	TopDriveState_5										TopDriveLoad[1].St.S0
#define	TopDriveState_6										TopDriveLoad[1].St.S1
#define	TopDriveState_7										TopDriveLoad[1].St.S2
#define	TopDriveState_8										TopDriveLoad[1].St.S3

#define	TopDriveState_9										TopDriveLoad[2].St.S0
#define	TopDriveState_10									TopDriveLoad[2].St.S1
#define	TopDriveState_11									TopDriveLoad[2].St.S2
#define	TopDriveState_12									TopDriveLoad[2].St.S3

#define	TopDriveState_13									TopDriveLoad[3].St.S0
#define	TopDriveState_14									TopDriveLoad[3].St.S1
#define	TopDriveState_15									TopDriveLoad[3].St.S2
#define	TopDriveState_16									TopDriveLoad[3].St.S3

#define	TopDriveState_17									TopDriveLoad[4].St.S0
#define	TopDriveState_18									TopDriveLoad[4].St.S1
#define	TopDriveState_19									TopDriveLoad[4].St.S2
#define	TopDriveState_20									TopDriveLoad[4].St.S3
///power fuse and load state redefine
#define	PowerState_01								PowerLoad[0].St.S0
#define	PowerState_02								PowerLoad[0].St.S1
#define	PowerState_03								PowerLoad[0].St.S2
#define	PowerState_04								PowerLoad[0].St.S3

#define	PowerState_05								PowerLoad[1].St.S0
#define	PowerState_06								PowerLoad[1].St.S1
#define	PowerState_07								PowerLoad[1].St.S2
#define	PowerState_08								PowerLoad[1].St.S3

#define	PowerState_09								PowerLoad[2].St.S0
#define	PowerState_10								PowerLoad[2].St.S1
#define	PowerState_11								PowerLoad[2].St.S2
#define	PowerState_12								PowerLoad[2].St.S3

#define	PowerState_13								PowerLoad[3].St.S0
#define	PowerState_14								PowerLoad[3].St.S1
#define	PowerState_15								PowerLoad[3].St.S2
#define	PowerState_16								PowerLoad[3].St.S3

#define	PowerState_17								PowerLoad[4].St.S0
#define	PowerState_18								PowerLoad[4].St.S1

#define	PowerState_19								PowerLoad[4].St.S2
#define	PowerState_20								PowerLoad[4].St.S3

#define	PowerFuseState_01								PowerFuse[0].St.S0
#define	PowerFuseState_02								PowerFuse[0].St.S1
#define	PowerFuseState_03								PowerFuse[0].St.S2
#define	PowerFuseState_04								PowerFuse[0].St.S3

#define	PowerFuseState_05								PowerFuse[1].St.S0
#define	PowerFuseState_06								PowerFuse[1].St.S1
#define	PowerFuseState_07								PowerFuse[1].St.S2
#define	PowerFuseState_08								PowerFuse[1].St.S3

#define	PowerFuseState_09								PowerFuse[2].St.S0
#define	PowerFuseState_10								PowerFuse[2].St.S1
#define	PowerFuseState_11								PowerFuse[2].St.S2
#define	PowerFuseState_12								PowerFuse[2].St.S3

#define	PowerFuseState_13								PowerFuse[3].St.S0
#define	PowerFuseState_14								PowerFuse[3].St.S1
#define	PowerFuseState_15								PowerFuse[3].St.S2
#define	PowerFuseState_16								PowerFuse[3].St.S3

#define	PowerFuseState_17								PowerFuse[4].St.S0
#define	PowerFuseState_18								PowerFuse[4].St.S1
#define	PowerFuseState_19								PowerFuse[4].St.S2
#define	PowerFuseState_20								PowerFuse[4].St.S3
//电源管理模块负载
#define	PowerContrlState_01								PowerContrlLoad[0].St.S0
#define	PowerContrlState_02								PowerContrlLoad[0].St.S1
#define	PowerContrlState_03								PowerContrlLoad[0].St.S2
#define	PowerContrlState_04								PowerContrlLoad[0].St.S3

#define	PowerContrlState_05								PowerContrlLoad[1].St.S0
#define	PowerContrlState_06								PowerContrlLoad[1].St.S1
#define	PowerContrlState_07								PowerContrlLoad[1].St.S2
#define	PowerContrlState_08								PowerContrlLoad[1].St.S3

#define	PowerContrlState_09								PowerContrlLoad[2].St.S0
#define	PowerContrlState_10								PowerContrlLoad[2].St.S1
#define	PowerContrlState_11								PowerContrlLoad[2].St.S2
#define	PowerContrlState_12								PowerContrlLoad[2].St.S3

#define	PowerContrlState_13								PowerContrlLoad[3].St.S0
#define	PowerContrlState_14								PowerContrlLoad[3].St.S1
#define	PowerContrlState_15								PowerContrlLoad[3].St.S2
#define	PowerContrlState_16								PowerContrlLoad[3].St.S3

///仪表负载状态重定义
#define	PANNAL_OUT_STATE_1					ReState[0].St.S0
#define	PANNAL_OUT_STATE_2					ReState[0].St.S1
#define	PANNAL_OUT_STATE_3					ReState[0].St.S2
#define	PANNAL_OUT_STATE_4					ReState[0].St.S3

