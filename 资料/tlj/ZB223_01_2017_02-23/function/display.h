#ifndef __DISPLAY_H_
#define __DISPLAY_H_
#include	"Typedef.h"
#include	"TW_FontOsd_Picture.h"
//#include	"TW_SpiOsd_Picture.h"
#include 	"configFile.h"
#include	"TW_FontOsd.h"
#include	"BitMacros.h"
#include	"Load_deal.h"
#include	"J1939.h"
#include	"KeyDeal.h"
#include "common.h"
#include "display.h"
#include "NamedPhysicalKeyValue.h"
#include "isr.h"
#include "StepDeal.h"
#include "message.h"
#include "EngineDiagnose_DM1.h"  
#include "physicalq.h"
#include "M41T00.h"
#include "infoManage.h"
#include "sensor.h"
#include "KeyProcess.h"
#include	"ISR.H"


#define  YEAR_H                 0x00
#define  YEAR_L                 0x01
#define  MONTH_H                0x02
#define  MONTH_L                0x03
#define  DATE_H                 0x04
#define  DATE_L                 0x05
#define  HOUR_H                 0x06
#define  HOUR_L                 0x07
#define  MINUTE_H               0x08
#define  MINUTE_L               0x09
#define  SECOND_H               0x0a
#define  SECOND_L               0x0b

#define  DIS_NORMAL_MODE         0x00
#define  DIS_DELAY_MODE          0x01
#define  DIS_FLICKER_MODE        0x02
#define  DIS_DELAY_FLICKER_MODE  0x03

#define  TIME_CHECK_ERROR       0x01
#define  TIME_CHECK_TRUE        0x00

#define  ONE_PAGE_ITEM          5//每页显示五条

#define  DIS_BIGGER_WARM                        0x03  //gao
#define  DIS_LOWER_WARM                         0x00
#define  DIS_MIDDLE_WARM                        0x01
#define  DIS_WARM_MASK                          0x03
/****************************osd logc type *****************************************/
#define DIS_SYS_TIME_TYPE                       0x02
#define DIS_COLUMN_WARM                         0x03
#define DIS_SYS_DAY_TYPE                        0x04
#define DIS_J1939_TYPE                          0x06
#define DIS_INSIDE_16BIT_TYPE                   0x07
#define DIS_INSIDE_32BIT_TYPE                   0x08
#define DIS_VERSION_TTPE                        0x09
#define DIS_SYS_DATA_TYPE                       0x0a
#define AUTO_ARRANGE_TYPE                       0x0b
#define MULTIPGCKAGE_TYPE                       0x0c  
#define DIS_HEX_OUT_TYPE                        0x0d
#define DIS_STATUS_WORLD_TYPE                   0x0e
#define DIS_STRING_OUT_TYPE                     0x0f

#define	DIS_INFO_MANAGE_TYPE					  0x10
//#define COMP_DATA_OUT_TYPE                      0x0d 

/**************************pic logc type *******************************************/
#define DISPLAY_PIC_GRADIENT_TYPE               0x01
#define DISPLAY_PIC_PAGE_TYPE                   0x02
#define DISPLAY_PIC_CURSOR_TYPE                 0x03
#define DISPLAY_FaultInquiry_1                  0x06
#define DISPLAY_FaultInquiry_2                  0x07
#define DISPLAY_FaultInquiry_3                  0x08
#define DISPLAY_FaultInquiry_4                  0x09
#define DISPLAY_FaultInquiry_5                  0x0a

#define DISPLAY_PIC_STATUS_TYPE                 0x11
#define DISPLAY_PIC_KEY_STATUS_TYPE             0x10
#define DISPLAY_MENU_CONFIG_TYPE                0x12
#define DISPLAY_PIC_TPMS_TYPE                   0x13




   
/***************************************************************************************/
#define DISPLAY_INVENTED_KEY_STATUS             0X01
#define DISPLAY_J1939_STATUS                    0x02
#define DISPLAY_INVENTED_LOAD_STATUS            0x03

/****************************************************************************************/


#define  NULL_PIC_X        800
#define  NULL_PIC_Y        480

#define FONT_KEY_ON        1     //开关接通
#define FONT_KEY_OFF       2     //开关断开
#define FONT_CLEAR        ' '
#define FONT_LOAD_ON       4     //负载亮
#define FONT_LOAD_OFF      3     //负载灭
#define FONT_LOAD_SHORT    5     //负载短路
#define FONT_LOAD_OPEN     6     //负载开路

#define DIAGNOSE_PAGE_LINE 26   // 每页显示个数

///诊断类型
#define	DIAGNOSE_BEI_YONG		0///备用
#define	DIAGNOSE_LOAD				1///负载
#define	DIAGNOSE_SWITCH			2///开关
#define	DIAGNOSE_SENSOR			3///传感器
#define	DIAGNOSE_CATCH			4///捕获信号
#define	DIAGNOSE_ENABLE_UNABLE	5///是否使能
#define DIAGNOSE_CONSTANT       6//常量绿色填充

#define  WIPERS_MAX_TIME        0x09//雨刮间歇最大时间
#define  WIPERS_MIN_TIME        0x05//雨刮间歇最小时间
#define  LIGHT_MAX_LEVEL          3
#define  LIGHT_MIN_LEVEL          1
#define  BUZZER_MAX_LEVEL         3
#define  BUZZER_MIN_LEVEL         1

#define  OSD_PIC_DELAY            1//OSD与pic同步

#define  RSHIEL_VIDEO_CHANNEL             1
#define  MIDDLE_DOOR_VIDEO_CHANNEL        2
#define  FRONT_DOOR_VIDEO_CHANNEL         3
#define  TOP_VIDEO_CHANANEL               4

/*
#define	Load_On					  	0b11
#define	Load_Short					0b01
#define	Load_Open				  	0b10
#define	Load_Off						0b00
*/
//#define KeyOn					1
//#define KeyOff				0

EXT UINT8 NewDisplayStatus;
EXT UINT8 OldDisplayStatus;
EXT UINT8 StartLogoDelay;

EXT UINT8 builtLcdText(UINT8 *pDisBuf,UINT8 type,UINT16 index,UINT32 inputDat,UINT8 bitval);


//#pragma CONST_SEG __GPAGE_SEG CONFIG_DATA_SDO
extern const   PIC_INFO_STRUCT PicInfo[700];
extern const   INTERFACE_INFO_STRUCT InterfaceInfo[60];
extern const   PIC_LOGIC_STRUCT  PicLogicInfo[630];
extern const   OSD_LOGIC_STRUCT   OsdLoicgInfo[585];
extern const   OSD_LOGIC_STRUCT   OsdLoicgInfo1[585];


//#pragma CODE_SEG  USER_ROM
//EXT const INTERFACE_INFO_STRUCT InterfaceInfo[4];
EXT void initDisParameter(void);
EXT void displayMain(void);
EXT void construct0(void);
EXT UINT8 msgProc0(LCD_MSG  *pMsg);
EXT void deconstruct0(void);
EXT void construct1(void);
EXT UINT8 msgProc1(LCD_MSG  *pMsg);
EXT void deconstruct1(void);
EXT void construct2(void);
EXT UINT8 msgProc2(LCD_MSG  *pMsg);
EXT void deconstruct2(void);
EXT void construct3(void);
EXT UINT8 msgProc3(LCD_MSG  *pMsg);
EXT void deconstruct3(void);
EXT void construct4(void);
EXT UINT8 msgProc4(LCD_MSG  *pMsg);
EXT void deconstruct4(void);

EXT void construct5(void);
EXT UINT8 msgProc5(LCD_MSG  *pMsg);
EXT void deconstruct5(void);

EXT void construct6(void);
EXT UINT8 msgProc6(LCD_MSG  *pMsg);
EXT void deconstruct6(void);
EXT void construct7(void);
EXT UINT8 msgProc7(LCD_MSG  *pMsg);
EXT void deconstruct7(void);
EXT void construct8(void);
EXT UINT8 msgProc8(LCD_MSG  *pMsg);
EXT void deconstruct8(void);
EXT void construct9(void);
EXT UINT8 msgProc9(LCD_MSG  *pMsg);
EXT void deconstruct9(void);
EXT void construct10(void);
EXT UINT8 msgProc10(LCD_MSG  *pMsg);
EXT void deconstruct10(void);
EXT void construct11(void);
EXT UINT8 msgProc11(LCD_MSG  *pMsg);
EXT void deconstruct11(void);
EXT void construct12(void);
EXT UINT8 msgProc12(LCD_MSG  *pMsg);
EXT void deconstruct12(void);
EXT void construct13(void);
EXT UINT8 msgProc13(LCD_MSG  *pMsg);
EXT void deconstruct13(void);
EXT void construct14(void);
EXT UINT8 msgProc14(LCD_MSG  *pMsg);
EXT void deconstruct14(void);
EXT void construct15(void);
EXT UINT8 msgProc15(LCD_MSG  *pMsg);
EXT void deconstruct15(void);
EXT void construct16(void);
EXT UINT8 msgProc16(LCD_MSG  *pMsg);
EXT void deconstruct16(void);
EXT void construct17(void);
EXT UINT8 msgProc17(LCD_MSG  *pMsg);
EXT void deconstruct17(void);

EXT void construct18(void);
EXT UINT8 msgProc18(LCD_MSG  *pMsg);
EXT void deconstruct18(void);

EXT void construct19(void);
EXT UINT8 msgProc19(LCD_MSG  *pMsg);
EXT void deconstruct19(void);

EXT void construct20(void);
EXT UINT8 msgProc20(LCD_MSG  *pMsg);
EXT void deconstruct20(void);

EXT void construct21(void);
EXT UINT8 msgProc21(LCD_MSG  *pMsg);
EXT void deconstruct21(void);

EXT void construct22(void);
EXT UINT8 msgProc22(LCD_MSG  *pMsg);
EXT void deconstruct22(void);

EXT void construct23(void);
EXT UINT8 msgProc23(LCD_MSG  *pMsg);
EXT void deconstruct23(void);

EXT void construct24(void);
EXT UINT8 msgProc24(LCD_MSG  *pMsg);
EXT void deconstruct24(void);

EXT void construct25(void);
EXT UINT8 msgProc25(LCD_MSG  *pMsg);
EXT void deconstruct25(void);

EXT void construct26(void);
EXT UINT8 msgProc26(LCD_MSG  *pMsg);
EXT void deconstruct26(void);

EXT void construct27(void);
EXT UINT8 msgProc27(LCD_MSG  *pMsg);
EXT void deconstruct27(void);

EXT void construct28(void);
EXT UINT8 msgProc28(LCD_MSG  *pMsg);
EXT void deconstruct28(void);

EXT void construct29(void);
EXT UINT8 msgProc29(LCD_MSG  *pMsg);
EXT void deconstruct29(void);

EXT void construct30(void);
EXT UINT8 msgProc30(LCD_MSG  *pMsg);
EXT void deconstruct30(void);


EXT void construct31(void);
EXT UINT8 msgProc31(LCD_MSG  *pMsg);
EXT void deconstruct31(void);

EXT void construct32(void);
EXT UINT8 msgProc32(LCD_MSG  *pMsg);
EXT void deconstruct32(void);

EXT void construct33(void);
EXT UINT8 msgProc33(LCD_MSG  *pMsg);
EXT void deconstruct33(void);

EXT void construct34(void);
EXT UINT8 msgProc34(LCD_MSG  *pMsg);
EXT void deconstruct34(void);

EXT void construct35(void);
EXT UINT8 msgProc35(LCD_MSG  *pMsg);
EXT void deconstruct35(void);

EXT void construct36(void);
EXT UINT8 msgProc36(LCD_MSG  *pMsg);
EXT void deconstruct36(void);

EXT void construct37(void);
EXT UINT8 msgProc37(LCD_MSG  *pMsg);
EXT void deconstruct37(void);

EXT void construct38(void);
EXT UINT8 msgProc38(LCD_MSG  *pMsg);
EXT void deconstruct38(void);

EXT void construct39(void);
EXT UINT8 msgProc39(LCD_MSG  *pMsg);
EXT void deconstruct39(void);

#define SHOW_PIC_F      0
#define CLEAR_PIC_F     1
#define HSAM_PIC_CLEAR   0xFFFD
#define HSAM_PIC_RESERVE 0xFFFE




EXT void disInterfaceOsd(unsigned char index);
EXT UINT16 disLogicCalculatePic(PIC_LOGIC_STRUCT *index);
EXT void disOsdLogicResult(OSD_LOGIC_STRUCT * index,UINT32 dat,UINT8 bitval);
EXT void	Diagnose_State(UINT8 devModule,UINT8 * pDiagnoseType,DIAGNOSE_STATUS_STRUCT *DiagnoseContent,UINT8 pageCnt,UINT8 itemNumber,UINT8 mode);
EXT void doInterfacePiclogic(UINT8 index,UINT8 clear_delay,UINT8 curPage);


void disInterfacePic(unsigned char index);
#endif




