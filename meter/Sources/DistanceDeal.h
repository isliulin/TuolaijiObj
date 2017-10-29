#undef	EXT
#ifdef	DISTANVE_DEAL_C
#define	EXT
#else
#define	EXT extern
#endif
/* common defines and macros */
#include <hidef.h>      	
#include <mc9s12xhz512.h>
#include	"typedef.h"
/******funtion macros****************************/
#pragma CODE_SEG  USER_ROM
void	Distance_Counter(void);
static void CountVehicleDistance(void);
static void	SaveOverallDistance(void);
void	ClearDistance(void);
void	ConfigDistance(void);
static void CountHundredKilometerFuelConsume(void);
static void CountSmallDistanceFuelConsume(void);

#pragma CODE_SEG DEFAULT
/****************ram macros**************************/
EXT dword	OldVehicleSmallDistance;
EXT dword	VehicleSmallDistance;
EXT word	VehicleSmallDistanceHigh;
EXT word	VehicleSmallDistanceLow;
EXT word	VehicleTime;
EXT dword	OldVehicleOverallDistance;	
EXT dword	VehicleOverallDistance;	
EXT word	VehicleOverallDistanceHigh;
EXT word	VehicleOverallDistanceLow;
EXT dword	CountDistanceSave;
///�ٹ����ͺĵ���ر����Ķ���
EXT dword HundredKilometerDistanceNumber;///�ٹ����ͺ��ڸ��׶�����������̵ļ���
EXT dword FirstTotalFuelConsume,LastTotalFuelConsume; ///ǰ�������ͺĵı���
EXT word HundredKilometerFuelConsume;///�ٹ����ͺ�
///��;�ͺĵ���ر����Ķ���
EXT dword FirstTotalFuelConsume_Small,LastTotalFuelConsume_Small; ///ǰ�������ͺĵı���
EXT word SmallDistanceFuelConsume;///��;�ͺ�
/*******************parameter macros**************/
#define	DISTANCE_SAVENUM		100

