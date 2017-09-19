#ifndef	_LoadDeal_h
#define		_LoadDeal_h

#include 	"hidef.h" 		/* for EnableInterrupts macro */
#include 	"derivative.h" /* include peripheral declarations */
#include	"typedef.h"
#include	"common.h"
#include	"DriveKeyDeal.h"
#include	"BitMacros.h"
#include	"MessageDeal.h"

#ifndef	_NO_LoadDeal_EXTERN
#define		_LoadDeal_EXTERN	extern
#else
#define		_LoadDeal_EXTERN
#endif
/************funtion macros****************/
void	LoadDeal(void);

void	Out595(unsigned char	Number595,unsigned	char	Kind);

static void LoadStateMask(void);

static void	GetSTProcess(unsigned char	GetStState);

static void	StTest(void);

static void	ErrorReset(void);

static unsigned char ChangeData(void);

static void	OutputLoadChange(void);

static void	MotorReStartDetect(void);

static void MotorStartTooLongDetect(void);

static void Module_Check(void);

static void FlashLampDeal(void);

static void RelayControl_RainScratchDeal(void);

void	LoadOpenDetect(void);
/******************ram macros**********************/
_LoadDeal_EXTERN	StStruct	StData[3];
_LoadDeal_EXTERN	volatile LoadStruct	ReState[6],LastState[6],TopLoadState[6],FrontLoadState[6],PowerLoadState[5];
_LoadDeal_EXTERN	unsigned char	FlagsADNormalOpen[3][8],FlagsADOpen[3][8],FlagsADNormalClose[3][8];
_LoadDeal_EXTERN	unsigned char FlagsOnChechOpen[3][8],FlagsADShort[3][8],FirstADErrorFlag[3][8];
_LoadDeal_EXTERN	ADErrorStruct	FlagsADError[3];
_LoadDeal_EXTERN	PutLoadStruct PutData[3],OldPutData[3],LastPutData[3];
_LoadDeal_EXTERN	unsigned char motor_st_index;
_LoadDeal_EXTERN	unsigned int	RestartDetectTime;
_LoadDeal_EXTERN	unsigned char st_index,NoNShieldSt_Index;
_LoadDeal_EXTERN	unsigned int	MotorStartDetect;
_LoadDeal_EXTERN	unsigned int	PowerOff_Delay;
_LoadDeal_EXTERN	unsigned int	RotationData;
_LoadDeal_EXTERN	unsigned char	RotationError;
_LoadDeal_EXTERN	unsigned char	ReSpeedData;
_LoadDeal_EXTERN	unsigned int	IntermissionTime,WashTime;
_LoadDeal_EXTERN	unsigned int	RainScratchIntermissionTime;
_LoadDeal_EXTERN	unsigned int	delay3s_ModuleCheck;
_LoadDeal_EXTERN	unsigned char	FlashIndex,delayxms,dirdelay;
_LoadDeal_EXTERN	unsigned char	FrontDoorCloseSignalTime;
_LoadDeal_EXTERN	unsigned int	SchoolBusBerthTime;
_LoadDeal_EXTERN	unsigned int	EngineControlPowerTime;
///_LoadDeal_EXTERN	unsigned int	EngineProtectStartTime;
/******************IO Macros********************/
////bt724 drive pin                      	
#define	HC595_CLOCK			PTED_PTED5
#define	HC595_LOAD0			PTED_PTED4
#define	HC595_DATA0			PTED_PTED2	
#define	HC595_LOAD1			PTCD_PTCD0
#define	HC595_DATA1			PTBD_PTBD0		
#define	HC595_OE					PTED_PTED3

#define	HC595_CLOCK_IO	PTEDD_PTEDD5
#define	HC595_LOAD0_IO	PTEDD_PTEDD4
#define	HC595_DATA0_IO	PTEDD_PTEDD2
#define	HC595_LOAD1_IO	PTCDD_PTCDD0
#define	HC595_DATA1_IO	PTBDD_PTBDD0
#define	HC595_OE_IO			PTEDD_PTEDD3
///io direct drive bt724 pin
#define	IN5						PTDD_PTDD2
#define	IN6						PTDD_PTDD3
#define	IN7						PTDD_PTDD4
#define	IN8						PTDD_PTDD5

#define	IN5_IO				PTDDD_PTDDD2
#define	IN6_IO				PTDDD_PTDDD3
#define	IN7_IO				PTDDD_PTDDD4
#define	IN8_IO				PTDDD_PTDDD5
////st detect pin
#define	HC165_CLOCK			PTCD_PTCD1
#define	HC165_LOAD				PTAD_PTAD0
#define	HC165_DATA1			PTCD_PTCD6
#define	HC165_DATA2			PTCD_PTCD2

#define	HC165_CLOCK_IO	PTCDD_PTCDD1 
#define	HC165_LOAD_IO		PTADD_PTADD0 
#define	HC165_DATA1_IO	PTCDD_PTCDD6 
#define	HC165_DATA2_IO	PTCDD_PTCDD2 
///ST IO pin
#define	St17_IO					PTBDD_PTBDD6
#define	St18_IO					PTCDD_PTCDD5

#define	St17							StAdcData[0]
#define	St18							StAdcData[1]
///BT724 IO IN
#define	Bt724In5					PTDD_PTDD2
#define	Bt724In6					PTDD_PTDD3
#define	Bt724In7					PTDD_PTDD4
#define	Bt724In8					PTDD_PTDD5

#define	Bt724In5_IO			PTDDD_PTDDD2
#define	Bt724In6_IO			PTDDD_PTDDD3
#define	Bt724In7_IO			PTDDD_PTDDD4
#define	Bt724In8_IO			PTDDD_PTDDD5
///MOSFET IO 
#define	Mosfet1_IO				PTGDD_PTGDD3
#define	Mosfet2_IO				PTGDD_PTGDD2

#define	Mosfet1					PTGD_PTGD3
#define	Mosfet2					PTGD_PTGD2
///speed power io
#define	SpeedSenorPower_IO   PTDDD_PTDDD7
#define	SpeedSenorPower      PTDD_PTDD7
/*******************parameter macros***********************/
#define 	AD_SHORT_N 							15	///short,st detect time
#define		AD_ON_OPEN_N						3 	///open,st detect time
#define		AD_OPEN_N								100	///close,st detect time
#define		AD_NORMAL_OPEN					20
#define		AD_NORMAL_CLOSE					20
#define		OnOpenClose_Num					30	
#define		ReStartDelayNum					500	
#define		EngineStartDelayNum			500
#define		NoNShieldStartDelayNum	500

#define		Close_All					0
#define		Open_All					1
#define		Close_First595		2
#define		Close_Second595		3
#define		Close_Third595		4
#define		Close_Pwm					4

#define		StStateAll					1
#define		StState0						2
#define		StState1						3
#define		StState2						4

#define		ErrorRotationNum			0xcc
#define		OkRotationNum					0xbb

#define		OpenStateNum					0b10
#define		NormalOpenStateNum		0b00
#define		ShortStateNum					0b01
#define		NormalCloseStateNum		0b11

#define	ConnectDetectTimeNum		300
#define	RandomSetTimeNum				302
#define	LoadOpen								0x02
#define	LoadShort								0x01
#define	HighSpeedFlashDelay			25
#define	LowSpeedFlashDelay			43///50
#define	LeftFlashOpenCode				0x01
#define	RightFlashOpenCode			0x02
#define	AllFlashOpenCode				0x03
#define	AllFlashCloseCode				0x00
#define	SleepCode								0xaa
#define	MeterErrorState					MeterErrorFlag==1
#define	MeterOkState							MeterErrorFlag==0
#define	LeftFlashCode						FlashKey[0]
#define	RightFlashCode					FlashKey[1]
#define	BakLeftFlashCode				BakFlashKey[0]
#define	BakRightFlashCode				BakFlashKey[1]

#define	RainScratchHighOpen			DLoad8=1;
#define	RainScratchHighClose		DLoad8=0;
#define	RainScratchLowOpen			DLoad4=1;
#define	RainScratchLowClose			DLoad4=0;

#define	StartupStateDelayNum		50

/***********************io st detect macros******************/
#define	ST17Detcet			StData[2].b.ST0
#define	ST18Detcet			StData[2].b.ST1
#define	ST19Detcet			StData[2].b.ST2
#define	ST20Detcet			StData[2].b.ST3
#define	ST21Detcet			StData[2].b.ST4
#define	ST22Detcet			StData[2].b.ST5
#define	ST23Detcet			StData[2].b.ST6
#define	ST24Detcet			StData[2].b.ST7

#endif
