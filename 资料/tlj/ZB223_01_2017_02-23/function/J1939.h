#ifndef __J1939_H_
/*****common defines and macros*******/
#include "typedef.h"
#include "configFile.h"
#include "BitMacros.h"
#include "message.h"
#include "ap_S32K1xx_FlexCAN.h"
#include "arm_cm4.h"





/*****Functions Prototypes**********/
//#pragma CODE_SEG __NEAR_SEG NON_BANKED  

EXT UINT32 rcvJ1939Id(void);


EXT void CAN1error_int(void);

//#pragma CODE_SEG USER_ROM




EXT UINT8 testPgn(UINT16 pgn);



EXT void getWaterTemp(UINT8 * pData);
EXT void getOilPress(UINT8 * pData);

EXT void getFuelRate(UINT8 * pData);
EXT void getCarbamideValue(void);
//EXT void getJ1939IntData(UINT16 pgn,UINT8 *pData,UINT16 *pDataBuf,UINT8 *pVirtualKey,UINT8 cnt);
//EXT void getJ1939CharData(UINT16 pgn,UINT8 *pData,UINT8 *pDataBuf,UINT8 *pVirtualKey,UINT8 cnt);
//EXT void getJ1939WordData(UINT16 pgn,UINT8 *pData,UINT32 *pDataBuf,UINT8 *pVirtualKey,UINT8 cnt);
//EXT void getJ1939BitFault(UINT16 pgn,UINT8 * pData,UINT8 * pBuf,UINT8 cnt);
//EXT void getJ1939BitStatus(UINT16 pgn,UINT8 *pBuf,UINT8 *pData,UINT8 *pVirtualKey,UINT8 cnt);
//EXT void getJ1939FaultCode(UINT16 pgn,UINT8 * pData);
EXT void getJ1939AsciiData(UINT16 pgn,UINT8 *pData);
EXT void debug(void);



EXT void CAN_sendMsg(UINT8 len , UINT8 *pMsg);

EXT void doSignelPackageScrAndMil(CAN_PACKET_T *pMsg);
EXT void doMultiPgckageScrAndMil(CAN_PACKET_T *pMsg);
EXT void J1939_Speed_Send1(void);
EXT void Gas_Fired_Req(void);
EXT void recorderLoad(void);
EXT void recorderKey(void);

EXT void deal_AirConditoner(void);
EXT void Auxiliary_Discrete(void); 
EXT void Auxiliary_Discrete1(void); 






EXT void  steering_wheel_key(UINT8 *dat);

//#pragma CODE_SEG DEFAULT
/***************ram macros*************/
EXT	CAN_PACKET_T send_can_packet;
EXT	CAN_PACKET_T rev_can_packet; 	
EXT	uint EngSpeed;
EXT	uchar Fual_Require_Time;
EXT J1939_FRAME_STRUCT J1939Msg;
EXT UINT16 CarSpeedFromJ1939;
EXT UINT8 J1939RequestIndex;
EXT UINT8 J1939IdNum;
//EXT UINT8                   J1939VirtualKey[8];
//EXT UINT8                   J1939CharData[80];           
 
//EXT UINT16                  J1939IntData[50];
//EXT UINT32                  J1939WordData[10];
//EXT UINT8                   J1939StatusBuf[32];
//EXT UINT8                   J1939AsciiBuf[4][25];
#define  J1939_STORAGE_SIZE  1700
EXT UINT8                   StorageBuffer[J1939_STORAGE_SIZE];

EXT J1939OverDealStruct                   J1939DetectCnt[200];
//EXT const CAN_PACKET_T            CarSpeedCanPacket;
EXT const CAN_PACKET_T            RequireTotalOilCanPacket;

//EXT const  UINT8 PgnNum;
//EXT const  UINT16 PgnText[40];
EXT DIAGNOSE_LAMP_STRUCT  	EngineDiagnoseLamp;
EXT DIAGNOSE_LAMP_STRUCT    ScrDiagnoseLamp;
EXT UINT32                  EngineDiagnosePgn;
EXT UINT32                  ScrDiagnosePgn;
EXT UINT16 J1939EngSpeedIndex;
EXT UINT8 wheel_key[2];//实声明在keyDeal.c


#define  ENGINE_SPEED_ID                    0x0cf00400
#define  ENGINE_WATER_TEMP_ID               0x18feee00
#define  OIL_PRESS_ID                       0x18feef00
#define  TOTAL_CONSUME_ID                   0x18fee900
#define  FUEL_RATE_ID                       0x18fef200
#define  CARBAMIDE_ID                       0x18fe563d
#define  STEERING_WHEEL_ID                  0x18FFE013  
#define  START_ELECTRICIZE                  0x100af456   //  开始
#define  START_ELECTRICIZE1                 0x100956f4
#define  END_ELECTRICIZE                    0x181df456

#define  PGN_TRUE                           0x00
#define  PGN_FALSE                          0x01
#define  J1939_NORMAL_MODE                  0x00
#define  J1939_CLEAN_MODE                   0x01	
#define  J1939_DEAL_NUM                     (0x40+60)   //15s



#define  J1939_VALID                        0x8000
#define  J1939_KEY                          0x4000
#define  J1939_BIG_ENDION                   0x2000      //高位在前 1
#define  J1939_OFFSET_TYPE                  0x1000      //0->SUB  1- ADD                                           
#define  J1939_WARM_MASK                    0x00F0
#define  J1939_LOWER_WARM                   0x0000
#define  J1939_MIDDLE_WARM                  0x0010
#define  J1939_BILATERAL_WARM               0x0020
#define  J1939_BIGGER_WARM                  0x0030
#define  J1939_SCHMITT_TRIGGER_LOW          0x0040
#define  J1939_SCHMITT_TRIGGER_HIGH         0x0050

#define  J1939_SIGNED_DATA_TYPE             0x0100
#define  J1939_OFFSET_DATA_TYPE             0x0000
#define  J1939_ABSOLUTE_VALUE               0x0300
#define  J1939_FiltrateZero_VALUE           0x0400
#define  CLEAN_HIGH_DATA_VALUE              0x0500

#define  J1939_Status_Type                  0x0000
#define  J1939_Char_Type                    0x0001
#define  J1939_Int_Type                     0x0002
#define  J1939_Three_Type                   0x0003
#define  J1939_Word_Type                    0x0004

#define  J1939_BIT_VALUE_MASK               0x00FFFFFF 
#define  J1939_DOT_VALUE_MASK               0xFF000000 


                          

#define  ENGINE_SPEED_PGN                   0xf004
#define  WATER_TEMPERATURE_PGN              0xfeee
#define  OIL_PRESS_PGN                      0xfeef
#define  TOTAL_CONSUME_PGN                  0xfee9
#define  FUEL_RATE_PGN                      0xfef2
#define  DM1_PGN                            0xfeca
#define  CNFBAM_PGN                         0xecff
#define  CNFPACK_PGN                        0xebff

#define	SCR_SA								              0x3D
#define	ENGINE_SA						                0x00

#define J1939_BIT_VKEY                      0x00
#define J1939_CHAR_VKEY                     0x01
#define J1939_INT_VKEY                      0x02
#define J1939_WORD_VKEY                     0x03

UINT16 J1939CompareData(UINT8 type,UINT16 index,UINT16 len);
void  TXWaitRelay(UINT8 TPMSPosition);
void  TX_TPMS_LearnEnd(UINT8 TPMSPosition);
void Trans_CANpkt(void);
void doJ1939Msg(void);
void getEngineSpeed(UINT8 * pData);
void getTotalConsume(UINT8 * pData);
void getJ1939MsgFromRd(CAN_PACKET_T *pbuf,J1939_FRAME_STRUCT *pJ1939RoundBuf);
void  doJ1939Data(CAN_PACKET_T *msg,UINT8 *pDataBuf,UINT8 *pVirtualKey,UINT16 cnt);
void J1939IdStartRx(UINT8 cnt); 
void getJ1939Data(CAN_PACKET_T *msg,UINT8 *pDataBuf,UINT16 cnt,UINT8 flag);
void getJ1939VirtualKey(UINT8 *pDataBuf,UINT8 *pVirtualKey,UINT16 cnt,UINT8 flag);
void fual_value_require(void);
void CAN0_send( CAN_PACKET_T  *pcanframe);
void MSCAN1Rec(UINT8 ch);
void J1939ID_OverTime(void);
void J1939_CanMsgOut_Send(CAN_PACKET_T  *pcanframe);
void J1939_CanMsgOut2_Send(CAN_PACKET_T  *pcanframe)   ;
void battery_req(UINT8 n);
void J1939_Speed_Send(void);
UINT8 testJ1939IdNum(void);


#endif
