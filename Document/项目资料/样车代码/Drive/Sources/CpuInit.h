#ifndef	_CpuInit_h
#define	_CpuInit_h

#include 	"hidef.h" 		/* for EnableInterrupts macro */
#include 	"derivative.h" /* include peripheral declarations */
#include	"typedef.h"
#include	"common.h"
#include	"BitMacros.h"
#include	"SelfID.h"


#ifndef	_NO_CPUINIT_EXTERN
#define	_CPUINIT_EXTERN	extern
#else
#define	_CPUINIT_EXTERN
#endif
/*******************Function macros*****************/
void	CpuInit(void);

void MSCAN_Init(unsigned char	ConfigureCanMode) ;

static void MCG_Init(void);

static void GPIO_Init(void);

static void Rti_Init(void);

static void	ADC_Init(void);

static void	Capture_Init(void);

static void	RamInit(void);

static void	Capture_Ja59_Init(void);

static void	Capture_Ja48_Init(void);

static void Init_Cop(void);

static void Init_LVD(void);
/*********************parameter macros***************************/
/* External crystal oscillator = 8MHz */
/* FOR PEE mode:Bus_Clock = (1/2)*[(Osc_clock/Ref_div)*VCO_div]/(Bus_div) = 8MHz */   
#define BUS_DIV 				0       /* Bus divider = 0, divide by 1 */
/* Clock source options */
#define BUS_CLK     								1 
/* MSCAN module configuration constants */
/* Baud rate = CANCLK/(Prescaler * time quantas) */
/* TSEG1 + TSEG2 + SYNCH_SEG = 4 time quantas */
#define CAN_CLK  	  		0 /* Clock source: 0 for external oscillator, 1 for bus clock */
#define TIME_SEGM1  		4   /* Time Segment 1 = 5 */
#define TIME_SEGM2  		1   /* Time Segment 2 = 2 */
/* Prescaler = CANCLK/(Baud rate * time quantas) = 8MHz/(250kHz * 4) = 8 */                                               
#define CAN_PRESC   		3   /* Baud rate prescaler = 4 */ 
#define CAN_SYNJW   		0   /* Sinchronization jump width = 1 clock cycles */ 
/****can mode macros****/
#define	NormalCanMode		0
#define	ListenCanMode		1
/*Can Id Definitions*/
#define	 PannalModuleID				0x10ff0117///仪表
#define	 FrontDriveID						0x10FF0121///前控
#define	 BackDriveID						0x10FF0221///后控
#define	 TopDriveID						0x10FF0321///顶控
#define	 PowerModuleID				0x10FF0421///配电
#define	 MultiFunctionModuleID	0x10ff0521///多功能主机
#define	 PowerControlModuleID	0x10FF0621///电源管理

#define	 SpeedID							0x10fe6cee
#define	 RotationId							0x10f004ee
/*协议中帧功能定义*/
#define	 BootloaderWaitFrame				0x00///待机准备
#define	 BootloaderStartFrame				0x01///烧录准备
#define	 BootloaderResetFrame			0x08///软复位
#define	 BootloaderStopFrame				0x09///烧写中止

#define	 Key1StateFrame						0x20
#define LoadState1Frame						0x21
#define	 LoadState2Frame						0x22
#define	 SensorStateFrame					0x23
#define	 CurrentState1Frame				0x24
#define	 CurrentState2Frame				0x25
#define	 CurrentState3Frame				0x26
#define	 Sleep_DangerFrame					0x27
#define	 SpeedFrame								0x28
#define	 RotationFrame							0x29
#define	 RainScratchTimeFrame			0x2A
#define OilConsumeState1Frame			0x2B
#define	 OilConsumeState2Frame			0x2C
#define DistanceStateFrame					0x2D
#define	 IDState1Frame							0x2E
#define	 IDState2Frame							0x2F
#define	 IDState3Frame							0x30
#define	 DistanceConfigStateFrame		0x31
#define	 Key2StateFrame						0x32


///全局变量定义
///_CPUINIT_EXTERN		unsigned long		BootCanMask;///CAN ID 屏蔽的变量


#endif

