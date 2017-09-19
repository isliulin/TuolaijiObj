#ifndef __CONFIGFILE_H_
#define	__CONFIGFILE_H_
/*****common defines and macros*******/
#include	"typedef.h"

//原本在其他文件里的定义
#define NUMBER_OF_OUTPUT_CHANNELS				160//(Num_PowerDevice+NUM_CAT4016*2)*8+NUM_MAIN_MANU_LED_OUTPUT_CHANNELS+NUM_BUZZER_CHANNELS+ 30
#define CONDITION_MAX_CONDITION_UNITS			23//是一个逻辑表达式中最小项个数的最大值,用两个字节存储一个最小项





extern USR_t_ApplicationObject                 USR_s_OV;
extern USR_LOAD_STATUS                         USR_LOAD_OV;
extern USR_t_DeviceConfigrationData            USR_s_DeviceConfigData;
extern USR_t_ManufactureConfigrationData       USR_s_ManufactureConfigData;
//#pragma CONST_SEG __GPAGE_SEG CONFIG_DATA_SDO
/**********************************开关配置********************************************/
 #define J1939WordDataNum 675

/********************************传感器配置********************************************/
#define  FRONT_INSIDE_OFFSET              20
#define  BACK_INSIDE_OFFSET               40
#define  MIDDLE_INSIDE_OFFSET             60
#define  TOP_INSIDE_OFFSET                80
#define  POWER_INSIDE_OFFSET              100
//#define  POWER_CONTOROL_OFFSET            110
#define  POWER_CONTOROL_OFFSET            120
//#define  END_INSIDE_OFFSET                140
#define  END_INSIDE_OFFSET1               160

#define SENSOR_DASHBOARD_NUM       5   //板载6路传感器
#define INSIDE_16BIT_DATA_NUM      END_INSIDE_OFFSET1  //总共的内部数据
#define SENSOR_VIRTUAL_KEY_NUM     40  //传感器转虚拟开关数


extern const  STRUCT_SYS_CONFIG  SysConfigData;   //系统配置
extern const  STRUCT_KEY_CONFIG  UserKeyConfig;


extern const  INSIDE_16BIT_ATTR_STRUCT Inside16BitAttrConfig[SENSOR_DASHBOARD_NUM];
extern const  SENSOR_DATA_STRUCT  SENSOR_DATA_CONFIG[INSIDE_16BIT_DATA_NUM];
extern const  SENSOR_VIRTUAL_KEY_STRUCT   SENSOR_VIRTUAL_KEY_CONFIG[SENSOR_VIRTUAL_KEY_NUM];

//extern const  OPENSHOAR  OpenMask ;//= 0x00;
//extern const  OPENSHOAR  ShortMask;//= 0x00;
extern const  OPENSHOAR  OpenShortMask;

extern const  J1939_NUMBER J1939_Number_Config;
#define J1939_OFFSET_BYTE    J1939_Number_Config.J1939_Offset_Byte
#define J1939_STATUS_NUMBER  J1939_Number_Config.J1939_Status_Number
#define J1939_CHAR_NUMBER    J1939_Number_Config.J1939_Char_Number
#define J1939_INT_NUMBER     J1939_Number_Config.J1939_Int_Number
#define J1939_WORD_NUMBER    J1939_Number_Config.J1939_Word_Number
extern const  J1939_WORD_DATA_STRUCT   J1939WordDataConfig[J1939WordDataNum]; 
extern const  J1939_FIND_INDEX_STRUCT J1939FindIndexConfig[200];
extern const  UINT16 J1939IndexConfig[J1939WordDataNum];  

extern const  PC_CONFIG_ADDRESS_STRUCT  PcConfigAddress[30];
extern  const MOTO_STEP_STRUCT  MotorStepPrm[6];
extern  const   SIcoWarnConfig IcoWarnConfig;

extern const  DIAGNOSE_STRUCT  ModuleDiagnose;

extern UINT16 ROMCONST  LoadOutputConditions[NUMBER_OF_OUTPUT_CHANNELS][CONDITION_MAX_CONDITION_UNITS+1+2]; 

extern const  ID_Time_INFO J1939ZFFindIndexConfig[20];
extern const  J1939_DBT_INFO J1939ZFDataConfig;

extern const  STATUS_STRUCT  StatusPrara[80];
extern const  EngineDiagnoseCfgStruct  EngineDiagnoseCfg;
extern const  EngineDiagnoseCfgStruct  EngineDiagnoseCfg1;
extern const  EngineDiagnoseCfgStruct  EngineDiagnoseCfg2;

extern const  StorageManager1_struct StorageManagerCONST[RESOURE_TOTAL];
extern const  StorageManager2_struct InformationCONST[SECOND_CFG_TOTAL]; 
extern const  UINT8 PgnNum;
extern const  UINT16 PgnText[40];

//#pragma CONST_SEG    DEFAULT
#endif