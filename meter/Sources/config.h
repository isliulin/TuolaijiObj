#ifndef _CONFIG_H
	#define _CONFIG_H
/* common defines and macros */
#include <hidef.h>      	
#include <mc9s12xhz512.h>
#include <start12.h>
#include <stdlib.h>
#include 	"M41T00.H"
#include	"common.h"
#include	"CpuInit.h"
#include	"load_deal.h"
#include	"KeyDeal.h"
#include	"message.h"
#include	"DistanceDeal.h"
#include	"ISR.H"
#include 	"StepDeal.h"
#include	"Eeprom.h"
#include  "xgate.h"
#include	"TW_FontOsd.h"
#include	"TW_SpiOsd.h"
#include	"TftDisplay.h"
#include	"J1939.h"
#include	"Typedef.h"
#include  "common.h" 
#include	"TW_FontOsd.h"
#include	"TW_SpiOsd.h"
#include	"SensorDeal.h"

//#define	_INIT_TEST

/***************新项目时针对具体的电气方案，软件上的预配置操作******************/
/*******************************************************************************
配置内容：对车型的类型配置；0->柴油，1->气体
********************************************************************************/
#define	VEHICLE_TYPE			0
/*******************************************************************************
配置内容：对车身模块连接数量进行配置;如果被选择则有定义，否则无
********************************************************************************/
//////////////////////////////////当前控被选择时
#define	FRONTMODULE_CONNECT
//////////////////////////////////当后控被选择时
#define	BACKMODULE_CONNECT
/////////////////////////////////当顶控被选择时
//#define	TOPMODULE_CONNECT
/////////////////////////////////当配电被选择时
#define	POWERMODULE_CONNECT
/////////////////////////////////当电源管理模块被选用时
//#define	POWERCONTRLMODULE_CONNECT
/*******************************************************************************
配置内容：对J1939模块连接数量进行配置；如果被选择则有定义，否则无
********************************************************************************/
//////////////////////////////////发动机ECU模块被选择时
#define	ENGINE_ECU_CONNECT
///////////////////////////////////后处理SCR模块被选择时
//#define	SCR_CONNECT
///////////////////////////////////BMS模块被选择时
//#define	BMS_CONNECT
///////////////////////////////////DMC模块被选择时
//#define	DMC_CONNECT
//////////////////////////////////整车控制器被选择时
///#define	FULLCONTROL_CONNECT
//////////////////////////////////辅助控制器被选择时
///#define	ASSITANT_CONNECT
///////////////////////////////////ISG被选择时
///#define	ISG_CONNECT
///#define	TPMSModule_CONNECT		///胎压模块

///配置雨刮位置
#define RainScratchLocation			Power_ID
#define CarSpeedCaptureLocation	BackDriver_ID					

/*******************************************************************************
配置内容：对传感器在具体模块的配置:在前控时为0，在后控时为1，在顶控时为2，仪表模块为3；若不需要此传感器则设定为OXFF；
					对传感器在模块中的位置的配置：如在数据帧上的第1个设为0，第2个设为1，第3个设为2，第4个设为3，第5个设为4；
********************************************************************************/
/////////////////////////////////////////////////////前气压
#define	FRONT_AIR_SENSOR_IN				0///在哪一个模块
#define	FRONT_AIR_POSITION_IN			1///在模块中的位置
////////////////////////////////////////////////////后气压
#define	BACK_AIR_SENSOR_IN				0	
#define	BACK_AIR_POSITION_IN			2
////////////////////////////////////////////////////油量传感器
#define	FUAL_LEVEL_SENSOR_IN			0
#define	FUAL_LEVEL_POSITION_IN		0
////////////////////////////////////////////////////机油压力传感器
#define	OIL_PRESS_SENSOR_IN			0XFF//1
#define	OIL_PRESS_POSITION_IN		3
////////////////////////////////////////////////////水温传感器
#define	WATER_TEMPERATURE_SENSOR_IN		0xff//	1
#define	WATER_TEMPERATURE_POSITION_IN		4
////////////////////////////////////////////////////模块温度
#define MODULE_TEMPERATURE_POSITION_IN	6
////////////////////////////////////////////////////车速传感器
#define SPEED_SENSOR_IN							1
////////////////////////////////////////////////////转速传感器
#define ROTATION_SENSOR_IN					0xff
/********************************************************************************
对视频的个数的配置，可分为：无视频、1个视频->倒车、2个视频->倒车与中门、
										3个视频->倒车、中门、顶部视频(或其他特殊视频)
********************************************************************************/
#define	VIDOE_NUMBER	2
/*******************************************************************************
对是车速信号输出或是负载输出的配置：0->负载输出；1->车速信号输出
********************************************************************************/
#define	J13_OUT				0
/*******************************************************************************
是否需要有车速信号输出的配置：0->不需要；1->需要
********************************************************************************/
#define	CAR_SPEED_SIGNAL_OUT				0
//配制远光、近光、前雾灯开路位置
#define OpenLoadDisable			0xff

#define FrontDriverMoudle		0x00
#define BackDriverMoudle		0x20
#define TopDriverMoudle			0x40
#define PowerMoudle					0x60
//位置定义																	模块+位置0开始
#define	LeftFarLamp1NUM					(PowerMoudle+8)
#define	LeftFarLamp2NUM					(PowerMoudle+9)

#define	RightFarLamp1NUM				(PowerMoudle+2)	
#define	RightFarLamp2NUM				(PowerMoudle+3)

#define	LeftNearLamp1NUM				(PowerMoudle+7)	
#define	RightNearLamp1NUM				(PowerMoudle+4)

#define	LeftFrontFogLamp1NUM		(FrontDriverMoudle+8)	
#define	RightFrontFogLamp1NUM		(FrontDriverMoudle+9)

#endif

