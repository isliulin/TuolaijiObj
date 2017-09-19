#ifndef	_MessageDeal_h
#define		_MessageDeal_h

#include 	"hidef.h" 		/* for EnableInterrupts macro */
#include 	"derivative.h" /* include peripheral declarations */
#include	"typedef.h"
#include	"common.h"
#include	"BitMacros.h"
#include	"DriveKeyDeal.h"
#include	"CpuInit.h"

#ifndef	_NO_MessageDeal_EXTERN
#define		_MessageDeal_EXTERN	extern
#else
#define		_MessageDeal_EXTERN
#endif
/******************funtion macros*************************/
void  SendMessage(unsigned char SendKind);

void	MessageDeal(void);
/****************ram macros****************/
_MessageDeal_EXTERN	unsigned char 	FlashKey[2],BakFlashKey[2];
_MessageDeal_EXTERN	unsigned char 	SendFlashData[1];

_MessageDeal_EXTERN	unsigned char 	ReRainScratchIntermission;
_MessageDeal_EXTERN	unsigned char 	SendConfigDataNum;

_MessageDeal_EXTERN	CAN_PACKET_T 	rev_can_packet;///CAN总线接收数据包
_MessageDeal_EXTERN	unsigned char		ResponseData;///应答数据内容的变量
_MessageDeal_EXTERN  	CAN_PACKET_T   send_can_packet;///CAN总线发送数据包
_MessageDeal_EXTERN  	unsigned long		ModuleIDData;///模块ID名称变量
_MessageDeal_EXTERN  	unsigned char		SendCurrentNumber;///发送电流的序列号
/***************parameter macros*****************/

///协议中的数据功能帧定义
#define GetFunction()		rev_can_packet.data[0]
#define	 GetModuleID()		rev_can_packet.identifier
///CAN发送的状态
#define		StartBootLoader			0
#define 	SendKey1  						1
#define 	SendST1     					2
#define 	SendST2     					3
#define 	SendSensor 					4
#define		SendCurrent1				5
#define	    SendCurrent2				6
#define		SendCurrent3				7
#define		SendSleep_Danger		8
#define 	SendSpeed						9
#define		SendRotattion				10
#define		SendRainScratchTime	11
#define		SendOilConsume1			12
#define		SendOilConsume2			13
#define		SendDistanceState		14
#define		SendId1							15
#define		SendId2							16
#define		SendId3							17
#define		SendDistanceConfig     18
#define		SendKey2						19
///休眠与唤醒的指令
#define	SleepCode								0xaa
#define	WakeCode								0x55
	
#endif

