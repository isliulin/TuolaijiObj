#ifndef __INFOMANAGE_H
#define	__INFOMANAGE_H
//#include	"config.h"

#include	"typedef.h"




//数据来源
#define srcFROMJ1939		0
#define srcFROMINSIDE		1


#define	THE_SAVECNT_ADDRESS		64	//从第n个位置存储
#define	THE_BASE_ADDRESS			100
#define THE_HAVENUM_ADDRESS		74	//已存个数

//数据类型
#define STAUSTYPE			0
#define BYTE1TYPE			1
#define BYTE2TYPE			2
#define BYTE3TYPE			3
#define BYTE4TYPE			4

extern SelfKeyStruct	specialTreatmentflag;
#define	SPECIAL_DDAL0	specialTreatmentflag.b.Key0
#define	SPECIAL_DDAL1	specialTreatmentflag.b.Key1
#define	SPECIAL_DDAL2	specialTreatmentflag.b.Key2
#define	SPECIAL_DDAL3	specialTreatmentflag.b.Key3
#define	SPECIAL_DDAL4	specialTreatmentflag.b.Key4
#define	SPECIAL_DDAL5	specialTreatmentflag.b.Key5
#define	SPECIAL_DDAL6	specialTreatmentflag.b.Key6
#define	SPECIAL_DDAL7	specialTreatmentflag.b.Key7

extern   StorageManager1_struct storageManager1Cfg;
extern   StorageManager2_struct storageManager2Cfg;


extern UINT8 	storageArray[6];
extern UINT8	recordingStTime[6];
extern UINT8	saveCnt[RESOURE_TOTAL];
extern UINT8	haveNum[RESOURE_TOTAL];

void infoManageParaterInit(void);
UINT8 storage2rom(void);
void singleInfoWrite2EPROM(unsigned char addr,unsigned char* dataAddr,unsigned char dataType);



#endif
