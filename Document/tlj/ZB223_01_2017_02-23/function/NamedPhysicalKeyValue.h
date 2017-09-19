//#include "NamedPhysicalKeyValue.h"
#include	"configFile.h"

#ifndef NAMED_PHYSICAL_KEY_VALUE_H
#define NAMED_PHYSICAL_KEY_VALUE_H

#define OUTPUT_TIME_MARSK    0x0700     // 0x3fff
#define OUTPUT_TIMER_MARSK_TOGGLE_LOW   0x003F   
#define OUTPUT_TYPE_MARSK    0xF800     //0xc000
/*************************************************************************************
 BIT15  BIT14 BIT13 BIT12 BIT11  BIT10  BIT9  BIT8   BIT7 BIT6 BIT5 BIT4 BIT3 BIT2 BIT1 BIT0
有效位  0="&" 0=正常     0001 = 仪表         0=物理
        1="|" 1=取反     0010 = 前驱         1=虚拟              Index
                         0011 = 后驱
                         0100 = 顶驱
                         0101 = 配电
                         1001 =电源管理
               
**************************************************************************************/
#define CONDITION_KEY_INDEX_BIT_LEN	        0x09
#define CONDITION_NODE_INDEX_MARSK	        0x1E00
#define CONDITION_KEY_INDEX_MARSK	          0x01FF//1k


#define CONDITION_KEY_PRIVATE_KEY_MARSK	    0x0200
#define CONDITION_KEY_VIRTUAL_KEY_MARSK	    0x0100

#define METER_PHYSICAL_KEYS_NUM             40

#define DASHBOARD_KEY_BYTES     5
//#pragma push                                             
//#pragma align off
//#pragma pop
//#pragma CONST_SEG    DEFAULT
   
#define MODULE_TYPE_DRIVE_MODULE        1
#define MODULE_TYPE_DASHBOARD_MODULE    2
#define MODULE_TYPE_POWER_MODULE        3
#define DASHBOARD_MODULE_PHYSICAL_KEYS_ADDRESS      ((UINT8 *)(USR_s_OV.DashboardKey ))
#define FRONT_MODULE_PHYSICAL_KEYS_ADDRESS          ((UINT8 *)(USR_s_OV.FrontDriveKey ) )
#define BACK_MODULE_PHYSICAL_KEYS_ADDRESS           ((UINT8 *)(USR_s_OV.BackDriveKey  )    )
#define MIDDLE_MODULE_PHYSICAL_KEYS_ADDRESS         ((UINT8 *)(USR_s_OV.MiddleDriveKey   )  )
#define TOP_MODULE_PHYSICAL_KEYS_ADDRESS            ((UINT8 *)(USR_s_OV.TopDriveKey    )    )
#define POWER_MODULE_PHYSICAL_KEYS_ADDRESS          ((UINT8 *)(USR_s_OV.PowerDriveKey )    )  
#define POWER_CONTOROL_MODULE_PHYSICAL_KEYS_ADDRESS ((UINT8 *)(USR_s_OV.PowerContorolKey )    )    

#define DASHBOARD_MODULE_PHYSICAL_LOAD_ADDRESS   ((UINT8 *)(USR_s_OV.DashboardLoad))
#define FRONT_MODULE_PHYSICAL_LOAD_ADDRESS       ((UINT8 *)(USR_s_OV.FrontDriveLoad ) )
#define BACK_MODULE_PHYSICAL_LOAD_ADDRESS        ((UINT8 *)(USR_s_OV.BackDriveLoad  )    )
#define MIDDLE_MODULE_PHYSICAL_LOAD_ADDRESS      ((UINT8 *)(USR_s_OV.MiddleDriveLoad   )  )
#define TOP_MODULE_PHYSICAL_LOAD_ADDRESS         ((UINT8 *)(USR_s_OV.TopDriveLoad    )    )
#define POWER_MODULE_PHYSICAL_LOAD_ADDRESS       ((UINT8 *)(USR_s_OV.PowerLoadState )    )
#define POWER_MODULE_PHYSICAL_Fuse_ADDRESS       ((UINT8 *)(USR_s_OV.PowerFuse )    )
//#define DASHBOARD_MODULE_VIRTUAL_LOAD_ADDRESS    ((UINT8 *)(&LoadLogicResult[0].Load )    )


  
#define POWER_CONTOROL_MODULE_PHYSICAL_LOAD_ADDRESS ((UINT8 *)(USR_s_OV.PowerContorolLoadState ) )  
#define LOAD_STATE_FROM_PIC_ADDRESS                    (&LoadLogicResult[0].Load)

#define DASHBOARD_MODULE_VIRTUAL_KEYS_ADDRESS          (&USR_s_OV.DashboardVirtualKey[0].Data )
#define FRONT_DRIVER_MODULE_SHORT_OPEN_KEYS_ADDRESS    (&USR_s_OV.FrontDriverVirtualKey[0].Data)
#define BACK_DRIVER_MODULE_SHORT_OPEN_KEYS_ADDRESS     (&USR_s_OV.BackDriverVirtualKey[0].Data)
#define MIDDLE_DRIVER_MODULE_SHORT_OPEN_KEYS_ADDRESS   (&USR_s_OV.MiddleDriverVirtualKey[0].Data)
#define TOP_DRIVER_MODULE_SHORT_OPEN_KEYS_ADDRESS      (&USR_s_OV.TopDriverVirtualKey[0].Data)
#define POWER_MODULE_SHORT_OPEN_KEYS_ADDRESS           (&USR_s_OV.PowerVirtualKey[0].Data)
#define POWER_CONTOROL_MODULE_SHORT_OPEN_KEYS_ADDRESS  (&USR_s_OV.PowerContorolVirtualKey[0].Data   )  


#define DASHBOARD_PHYSICAL_KEYS_NUM              40
#define DASHBOARD_OPEN_KEYS_NUM                  48 //(40+8)
#define DASHBOARD_SHORT_KEYS_NUM                 56 //(40+8+8)
#define DASHBOARD_VIRTUAL_KEYS_NUM               96//(40+8+8+40)




#endif //NAMED_PHYSICAL_KEY_VALUE_H
