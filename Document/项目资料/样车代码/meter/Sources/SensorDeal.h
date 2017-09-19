#undef	EXT
#ifdef	SENSORDEAL_C
#define	EXT
#else
#define	EXT extern
#endif
/* common defines and macros */
#include <hidef.h>      	
#include <mc9s12xhz512.h>
#include	"typedef.h"
/***************parameter macros********************/
#define		SensorErrorNum		0xfe
#define		SensorOpenNum			175	///3.5v电压值

#ifdef	SENSORDEAL_C
const uchar OilPressure_Table[101]={
	115,	114,	112,	110,	109,	108,	106,	105,	104,	103,102,
	101,	99,	98,	96,	95,	93,	92,	91,	90,	89,
	88,	87,	85,	84,	83,	81,	80,	78,	77,	76,
	75,	74,	72,	71,	69,	68,	67,	66,	65,	64,
	63,	62,	60,	59,	57, 56,  55, 54,  53, 52,
	51, 50,  49, 48,  47, 46, 45, 44,  43,  42,
	41, 40,  39, 38,  37, 36, 35, 34,  33,  32,
	31, 30,  29, 28,  27, 26, 25, 24,  23,  22,
	21, 20,  19, 18,  17, 16, 15, 14,  13,  12,
	11, 10,  9,  8,   7,  6,  5,  4,   3,   3																			
};		
const	unsigned	char	FuelLevel_Table[46]=
{
  13, 17, 22, 27, 32, 37, 42, 47, 52, 57, 61, 
 	64, 67, 70, 73, 76, 79, 82, 85, 88, 92, 92,98,
 	101,103, 105, 107, 109, 111, 113, 115, 117,	119, 121,123,
	125,	127,	128,	129,	130,	131,132,133,	134,	135,	136
};
const uchar	GasPressure_Table[101]={
	0	,12,15,17,20,22,24,27,30,33,36,			
	38,40,44,46,48,50,52,55,57,60,
	63,65,67,69,71,73,75,76,78,80,
	82,83,85,87,89,91,93,95,97,99,
	101,102,103,104,105,106,107,108,109,110,
	111,112,113,114,115,116,117,118,119,120,
	121,122,123,124,125,126,127,128,128,129,
	129,130,130,131,131,132,132,133,133,134,
	134,134,135,135,135,136,136,136,137,137,
	137,137,138,138,138,139,139,139,140,140
};
const uchar Watertemp_Table[101]=
{
	182,180,178,176,174,172,170,168,166,164,  //40-49
	162,160,158,155,152,149,146,143,140,138,  //59
	136,133,130,127,125,123,121,119,116,113,  //69
	111,109,107,104,102,100, 98, 96,93, 90,   //79
	87, 85, 83, 81, 79, 77, 75, 74, 72, 70,  //89
	68, 66, 65, 64, 63, 62, 61, 60, 58, 57,  //99
	56, 55, 54, 53, 52, 51, 50, 49, 48, 47,
	46, 45, 44, 43, 42, 41, 40, 39, 38, 37,
	30,0
};
///input Capture Initialize
volatile const unsigned char  SPEED_PULSE  @0x7F00 = 8;
volatile const unsigned int   SPEED_DATA   @0x7F01 =990;	
volatile const unsigned int		ROTATION_PULSE @0x7F10 =106;
#endif

/******************funtion macros********************/
#pragma CODE_SEG  USER_ROM
void	SensorDealFunction(void);
static void dealWithFuelLevel(void);
static void dealWithAirPressure1(void);
static void dealWithAirPressure2(void);
static void dealWithOilPress(void);
static void dealWithWaterTemperture(void); 
void InputCaptureParameter_Initial(void);
#pragma CODE_SEG  DEFAULT
/*******************ram macros***********************/
EXT unsigned int	Time_Change_FuelLevel;
EXT unsigned char	Count_FuelLevel_Num,FuelLevel_Num[2];///油量采集的相关变量
EXT	unsigned char	Count_GasPress1_Num,Time_Change_GasPress1,GasPress1_Num[2];///前气压采集的相关变量
EXT	unsigned char	Count_GasPress2_Num,Time_Change_GasPress2,GasPress2_Num[2];///后气压采集的相关变量
EXT unsigned char	Count_OilPress_Num,Time_Change_OilPress,OilPress_Num[2];///机油压力采集的相关变量
EXT	unsigned char	Count_WaterTemp_Num,Time_Change_WaterTemp,WaterTemp_Num[2];///水温采集的相关变量
EXT unsigned char	Time_Capture_Speed_Zero; ///车速采集的归零检测变量
EXT unsigned char	Time_Capture_Rotation_Zero;///转速采集的归零检测变量
EXT volatile unsigned char	SendSpeedData[1];///采集转换成可发送在CAN网络的车速信号
EXT volatile unsigned char Capture6CountNum,SpeedDataNum,SpeedData[16];///车速采集的相关缓冲变量
EXT volatile unsigned int	Capture6TimeBuf[2];///采集车速的缓冲
EXT unsigned char	PannalSensorData[5];///仪表传感器检测到的数据值
EXT volatile	uchar			Speed_Pusle_Num;///车速脉冲个数的变量
EXT volatile	uint			Speed_Rate_Num;///车速里程参数的变量
EXT volatile	uint			Rotation_Pulse_Num;///转速齿轮个数的变量
