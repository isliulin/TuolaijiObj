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
///百公里油耗的相关变量的定义
EXT dword HundredKilometerDistanceNumber;///百公里油耗在各阶段所产生的里程的计算
EXT dword FirstTotalFuelConsume,LastTotalFuelConsume; ///前、后总油耗的变量
EXT word HundredKilometerFuelConsume;///百公里油耗
///短途油耗的相关变量的定义
EXT dword FirstTotalFuelConsume_Small,LastTotalFuelConsume_Small; ///前、后总油耗的变量
EXT word SmallDistanceFuelConsume;///短途油耗
/*******************parameter macros**************/
#define	DISTANCE_SAVENUM		100

