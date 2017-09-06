#ifndef __ENGINEDIAGNOSE_DM1_H_
#define __ENGINEDIAGNOSE_DM1_H_
#include "Typedef.h"	
#include "BitMacros.h"
#include "configFile.h"


#define	ENGINE_DIAGNOSE_NO_PACKAGE				0x00
#define	ENGINE_DIAGNOSE_PACKAGE					0xff
#define	ENGINE_DIAGNOSE_MAX_NUM					100//104//96
#define	ENGINE_DIAGNOSE_FAULT_CODE_BYTE			4
#define ENGINE_DIAGNOSE_AVAILABLE_FRAME_BYTE    7
extern BitCommon  EngineStatus;
#define Over_Rx_Engine0_F       EngineStatus.Bits.bit0
#define Dlay_Engine0_F          EngineStatus.Bits.bit1       //显示发动机故障

#define Over_Rx_Engine1_F       EngineStatus.Bits.bit2
#define Dlay_Engine1_F          EngineStatus.Bits.bit3       //显示发动机故障

#define Over_Rx_Engine2_F       EngineStatus.Bits.bit4
#define Dlay_Engine2_F          EngineStatus.Bits.bit5       //显示发动机故障


extern UINT32 EngineDiagnosePackageData[20];
extern UINT32 EngineDiagnosePackageData1[20];
extern UINT32 EngineDiagnosePackageData2[20];
extern UINT16  DiagnosePicIndx[20];
extern UINT16  DiagnosePicIndx1[20];
extern UINT16  DiagnosePicIndx2[20];
extern UINT16 MultiPgckage_AvailableByte_Num[3];
extern UINT16 OldMultiPgckage_AvailableByte_Num[3];

extern UINT8  MultiPgckageErrorStatus[3];






extern void EngineDiagnoseLost(void) ;
void doSignelPackageEngineDiagnose(CAN_PACKET_T *pMsg,UINT8 pgckageCnt);
void doCfgPgckageEngineDiagnose(CAN_PACKET_T *pMsg,UINT8 pgckageCnt);
void doMultiPgckageEngineDiagnose(CAN_PACKET_T *pMsg,UINT8 pgckageCnt);
void FindEngineDiagnose(void) ;
#endif