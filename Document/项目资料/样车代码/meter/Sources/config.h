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

/***************����Ŀʱ��Ծ���ĵ�������������ϵ�Ԥ���ò���******************/
/*******************************************************************************
�������ݣ��Գ��͵��������ã�0->���ͣ�1->����
********************************************************************************/
#define	VEHICLE_TYPE			0
/*******************************************************************************
�������ݣ��Գ���ģ������������������;�����ѡ�����ж��壬������
********************************************************************************/
//////////////////////////////////��ǰ�ر�ѡ��ʱ
#define	FRONTMODULE_CONNECT
//////////////////////////////////����ر�ѡ��ʱ
#define	BACKMODULE_CONNECT
/////////////////////////////////�����ر�ѡ��ʱ
//#define	TOPMODULE_CONNECT
/////////////////////////////////����类ѡ��ʱ
#define	POWERMODULE_CONNECT
/////////////////////////////////����Դ����ģ�鱻ѡ��ʱ
//#define	POWERCONTRLMODULE_CONNECT
/*******************************************************************************
�������ݣ���J1939ģ�����������������ã������ѡ�����ж��壬������
********************************************************************************/
//////////////////////////////////������ECUģ�鱻ѡ��ʱ
#define	ENGINE_ECU_CONNECT
///////////////////////////////////����SCRģ�鱻ѡ��ʱ
//#define	SCR_CONNECT
///////////////////////////////////BMSģ�鱻ѡ��ʱ
//#define	BMS_CONNECT
///////////////////////////////////DMCģ�鱻ѡ��ʱ
//#define	DMC_CONNECT
//////////////////////////////////������������ѡ��ʱ
///#define	FULLCONTROL_CONNECT
//////////////////////////////////������������ѡ��ʱ
///#define	ASSITANT_CONNECT
///////////////////////////////////ISG��ѡ��ʱ
///#define	ISG_CONNECT
///#define	TPMSModule_CONNECT		///̥ѹģ��

///�������λ��
#define RainScratchLocation			Power_ID
#define CarSpeedCaptureLocation	BackDriver_ID					

/*******************************************************************************
�������ݣ��Դ������ھ���ģ�������:��ǰ��ʱΪ0���ں��ʱΪ1���ڶ���ʱΪ2���Ǳ�ģ��Ϊ3��������Ҫ�˴��������趨ΪOXFF��
					�Դ�������ģ���е�λ�õ����ã���������֡�ϵĵ�1����Ϊ0����2����Ϊ1����3����Ϊ2����4����Ϊ3����5����Ϊ4��
********************************************************************************/
/////////////////////////////////////////////////////ǰ��ѹ
#define	FRONT_AIR_SENSOR_IN				0///����һ��ģ��
#define	FRONT_AIR_POSITION_IN			1///��ģ���е�λ��
////////////////////////////////////////////////////����ѹ
#define	BACK_AIR_SENSOR_IN				0	
#define	BACK_AIR_POSITION_IN			2
////////////////////////////////////////////////////����������
#define	FUAL_LEVEL_SENSOR_IN			0
#define	FUAL_LEVEL_POSITION_IN		0
////////////////////////////////////////////////////����ѹ��������
#define	OIL_PRESS_SENSOR_IN			0XFF//1
#define	OIL_PRESS_POSITION_IN		3
////////////////////////////////////////////////////ˮ�´�����
#define	WATER_TEMPERATURE_SENSOR_IN		0xff//	1
#define	WATER_TEMPERATURE_POSITION_IN		4
////////////////////////////////////////////////////ģ���¶�
#define MODULE_TEMPERATURE_POSITION_IN	6
////////////////////////////////////////////////////���ٴ�����
#define SPEED_SENSOR_IN							1
////////////////////////////////////////////////////ת�ٴ�����
#define ROTATION_SENSOR_IN					0xff
/********************************************************************************
����Ƶ�ĸ��������ã��ɷ�Ϊ������Ƶ��1����Ƶ->������2����Ƶ->���������š�
										3����Ƶ->���������š�������Ƶ(������������Ƶ)
********************************************************************************/
#define	VIDOE_NUMBER	2
/*******************************************************************************
���ǳ����ź�������Ǹ�����������ã�0->���������1->�����ź����
********************************************************************************/
#define	J13_OUT				0
/*******************************************************************************
�Ƿ���Ҫ�г����ź���������ã�0->����Ҫ��1->��Ҫ
********************************************************************************/
#define	CAR_SPEED_SIGNAL_OUT				0
//����Զ�⡢���⡢ǰ��ƿ�·λ��
#define OpenLoadDisable			0xff

#define FrontDriverMoudle		0x00
#define BackDriverMoudle		0x20
#define TopDriverMoudle			0x40
#define PowerMoudle					0x60
//λ�ö���																	ģ��+λ��0��ʼ
#define	LeftFarLamp1NUM					(PowerMoudle+8)
#define	LeftFarLamp2NUM					(PowerMoudle+9)

#define	RightFarLamp1NUM				(PowerMoudle+2)	
#define	RightFarLamp2NUM				(PowerMoudle+3)

#define	LeftNearLamp1NUM				(PowerMoudle+7)	
#define	RightNearLamp1NUM				(PowerMoudle+4)

#define	LeftFrontFogLamp1NUM		(FrontDriverMoudle+8)	
#define	RightFrontFogLamp1NUM		(FrontDriverMoudle+9)

#endif

