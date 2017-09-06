#ifndef __BitMacros_h
#define __BitMacros_h

#include "Typedef.h"
/*****************************************************/

//为前模块，后模块，中模块，顶模块，配电，高CAN模块  电源管理
EXT BitCommon ModuleDetectFlag;
#define  FRONT_DRIVE_MODULE_F                      ModuleDetectFlag.Bits.bit0
#define  BACK_DRIVE_MODULE_F                       ModuleDetectFlag.Bits.bit1
#define  MIDDLE_DRIVE_MODULE_F                     ModuleDetectFlag.Bits.bit2
#define  TOP_DRIVE_MODULE_F                        ModuleDetectFlag.Bits.bit3
#define  POWER_MODULE_F                            ModuleDetectFlag.Bits.bit4
#define  HIGH_CAN_MODULE_F                         ModuleDetectFlag.Bits.bit5
#define  POWER_CONTOROL_MODULE_F                   ModuleDetectFlag.Bits.bit6

EXT UINT8 ModuleDetectCnt[8];                            
//#define FRONT_DRIVE_MODULE                         0x00
//#define BACK_DRIVE_MODULE                          0x01
//#define MIDDLE_DRIVE_MODULE                        0x02
//#define TOP_DRIVE_MODULE                           0x03
//#define POWER_MODULE                               0x04

EXT BitCommon ModuleErrorFlag;
EXT BitCommon ModuleErrorOldFlag;
#define FRONT_DRIVE_MODULE_ERROR_F                 ModuleErrorFlag.Bits.bit0
#define BACK_DRIVE_MODULE_ERROR_F                  ModuleErrorFlag.Bits.bit1
#define MIDDLE_DRIVE_MODULE_ERROR_F                ModuleErrorFlag.Bits.bit2
#define TOP_DRIVE_MODULE_ERROR_F                   ModuleErrorFlag.Bits.bit3
#define POWER_MODULE_ERROR_F                       ModuleErrorFlag.Bits.bit4
#define HIGH_CAN_MODULE_ERROR_F                    ModuleErrorFlag.Bits.bit5  
#define POWER_CONTOROL_ERROR_F                     ModuleErrorFlag.Bits.bit6



EXT UINT8     CanLoseDataCnt[8];
EXT BitCommon CanLoseDataFlag;    
#define SPEED_DEVICE_F                        CanLoseDataFlag.Bits.bit0
#define ROTATION_DEVICE_F                     CanLoseDataFlag.Bits.bit1
#define EngineDiagnose0_F                     CanLoseDataFlag.Bits.bit2
#define EngineDiagnose1_F                     CanLoseDataFlag.Bits.bit3
#define EngineDiagnose2_F                     CanLoseDataFlag.Bits.bit4

EXT BitCommon CanLoseErrorFlag ;
//EXT BitCommon CanLoseErrorOldFlag;
#define SPEED_DEVICE_LOSE_F                    CanLoseErrorFlag.Bits.bit0
#define ROTATION_DEVICE_LOSE_F                 CanLoseErrorFlag.Bits.bit1
#define EngineDiagnose0_LOSE_F                 CanLoseErrorFlag.Bits.bit2
#define EngineDiagnose1_LOSE_F                 CanLoseErrorFlag.Bits.bit3
#define EngineDiagnose2_LOSE_F                 CanLoseErrorFlag.Bits.bit4
        

EXT BitCommon WarningFlag[2];
#define  FRONT_AIR_PRESS_WARN_F                    WarningFlag[0].Bits.bit0
#define  BACK_AIR_PRESS_WARN_F                     WarningFlag[0].Bits.bit1
#define  WATER_TEMP_WARN_F                         WarningFlag[0].Bits.bit2
#define  OIL_PRESS_WARN_F                          WarningFlag[0].Bits.bit3
#define  WATER_LINE_WARN_F                         WarningFlag[0].Bits.bit4
#define  OIL_MASS_WARN_F                           WarningFlag[0].Bits.bit5
#define  BATTERY_VOLTAGE_WARN_F                    WarningFlag[0].Bits.bit6
#define  FRONT_AIR_PRESS_SENSOR_WARN_F             WarningFlag[0].Bits.bit7
#define  BACK_AIR_PRESS_SENSOR_WARN_F              WarningFlag[1].Bits.bit0
#define  OIL_MASS_SENSOR_WARN_F                    WarningFlag[1].Bits.bit1
#define  OIL_PRESS_SENSOR_WARN_F                   WarningFlag[1].Bits.bit2
#define  WATER_TEMP_SENSOR_WARN_F                  WarningFlag[1].Bits.bit3

EXT BitCommon IcoWarn[5];
EXT BitCommon IcoWarnOld[5];

EXT BitCommon KeyFlag1;
#define HAVE_KEY1_F                                KeyFlag1.Bits.bit0
#define WAIT_KEY1_F                                KeyFlag1.Bits.bit1
#define HAVE_KEY2_F                                KeyFlag1.Bits.bit2
#define WAIT_KEY2_F                                KeyFlag1.Bits.bit3
#define HAVE_KEY3_F                                KeyFlag1.Bits.bit4
#define WAIT_KEY3_F                                KeyFlag1.Bits.bit5
#define HAVE_KEY4_F                                KeyFlag1.Bits.bit6
#define WAIT_KEY4_F                                KeyFlag1.Bits.bit7
EXT BitCommon KeyFlag2;
#define HAVE_KEY5_F                                KeyFlag2.Bits.bit0
#define WAIT_KEY5_F                                KeyFlag2.Bits.bit1
#define HAVE_KEY6_F                                KeyFlag2.Bits.bit2
#define WAIT_KEY6_F                                KeyFlag2.Bits.bit3
#define TPMS_PASS_MODE_F                           KeyFlag2.Bits.bit4
#define TPMS_ENTER_IN_F                            KeyFlag2.Bits.bit5


EXT BitCommon KeyFlag3;
#define WHEEL_KEY0_F                                KeyFlag3.Bits.bit0
#define WHEEL_KEY1_F                                KeyFlag3.Bits.bit1
#define WHEEL_KEY2_F                                KeyFlag3.Bits.bit2
#define WHEEL_KEY3_F                                KeyFlag3.Bits.bit3
#define WHEEL_KEY4_F                                KeyFlag3.Bits.bit4
#define WHEEL_KEY5_F                                KeyFlag3.Bits.bit5

/*********************common bit macros************************/
extern volatile BitCommon _BitMacros0;
#define BitMacros0										_BitMacros0.Byte
#define M41t00StopFlag		   					_BitMacros0.Bits.bit0
#define RainScratchChangeFlag					_BitMacros0.Bits.bit1
#define OLD_ON_KEY                    _BitMacros0.Bits.bit2
#define ID_require_flag								_BitMacros0.Bits.bit5
#define OLD_ACC_KEY                   _BitMacros0.Bits.bit3
#define OLD_SYS_SLEEP_LOAD            _BitMacros0.Bits.bit4
#define OLD_R_GEAR_VIDEO              _BitMacros0.Bits.bit6
#define OLD_MIDDLE_VIDEO              _BitMacros0.Bits.bit7
extern volatile BitCommon _BitMacros1;
#define	MUTE_SOUNDS_KEY				       	_BitMacros1.Bits.bit4
#define	OLD_4_GEAR_SOUND_F						_BitMacros1.Bits.bit5
#define	OLD_5_GEAR_SOUND_F						_BitMacros1.Bits.bit6
#define OLD_2_GEAR_SOUND_F            _BitMacros1.Bits.bit3
#define OLD_3_GEAR_SOUND_F            _BitMacros1.Bits.bit2 
#define MUTE_SOUNDS_F                 _BitMacros1.Bits.bit1 
#define GEAR_1_F                      _BitMacros1.Bits.bit0                
#define GEAR_2_F                      _BitMacros1.Bits.bit7

extern  volatile BitCommon _BitMacros3;
#define OLD_MIDDLE_DOOR_OPENED_LOAD   	_BitMacros3.Bits.bit0 
#define OLD_FRONT_DOOR_OPENED_LOAD   	  _BitMacros3.Bits.bit1
#define menu_back_flag							  	_BitMacros3.Bits.bit2
#define SECOND_F                        _BitMacros3.Bits.bit3
#define _1_S_F                          _BitMacros3.Bits.bit4
#define OLD_HIGH_CAN_MODULE_ERROR_F     _BitMacros3.Bits.bit5
#define key_flag							    	  	_BitMacros3.Bits.bit7

extern volatile BitCommon _BitMacros4;
#define OLD_ON_LCMDISP                _BitMacros4.Bits.bit0
#define SleepFlag											_BitMacros4.Bits.bit7
#define SetMileageFlag								_BitMacros4.Bits.bit6

#define RSleepFlag										_BitMacros4.Bits.bit3
#define OLD_INTERFACE_SWITCH_LOAD    	_BitMacros4.Bits.bit4
#define ClearRelativeDistance_F       _BitMacros4.Bits.bit1
#define GEAR_SOUND_OFF_F              _BitMacros4.Bits.bit2
#define MaintainClear_F               _BitMacros4.Bits.bit5

extern volatile BitCommon _BitMacros5;
#define BitMacros5								_BitMacros5.Byte	
#define buzzer_flag							   _BitMacros5.Bits.bit1	
#define motor_rest_flag            _BitMacros5.Bits.bit2
#define Sys_Post_Flag               _BitMacros5.Bits.bit3
#define OLD_ON_SAVE_KEY             _BitMacros5.Bits.bit4
// #define SECOND_F1                   _BitMacros5.Bits.bit4
#define MaintainClear_1F               _BitMacros5.Bits.bit5


//#define motor_delay_flag           _BitMacros5.Bits.bit3

extern volatile BitCommon _BitMacros12;
#define BitMacros12								_BitMacros12.Byte
#define RotationSensorflag				_BitMacros12.Bits.bit0
#define	Rotation_Error_Old				_BitMacros12.Bits.bit1
#define	low_flag									_BitMacros12.Bits.bit2
#define	_1MINF										_BitMacros12.Bits.bit3
#define	_1DAYF										_BitMacros12.Bits.bit4
#define	_1HOURF										_BitMacros12.Bits.bit5
#define	lcd_flag									_BitMacros12.Bits.bit6
#define BUZZER_FLAG             	_BitMacros12.Bits.bit7
    


extern volatile BitCommon _BitMacros13;
#define BitMacros13								_BitMacros13.Byte
#define BK_LED_refreshFlag				_BitMacros13.Bits.bit0	//仪表背光灯pwm刷新时间 1000ms
#define GAIN_OPT_Flag					_BitMacros13.Bits.bit1  //光敏传感器采集间隔 50ms
#define BK_LCD_refreshFlag				_BitMacros13.Bits.bit2 //LCD 背光
//#define BK_IS31FL_refreshFlag				_BitMacros13.Bits.bit3 //LCD 背光


extern volatile BitCommon _BitMacros20;
#define	PowerModuleTemperatureFlag		_BitMacros20.Bits.bit1
#define SCR_DIAGNOSE_F              	_BitMacros20.Bits.bit2
#define ENGINE_DIAGNOSE_F           	_BitMacros20.Bits.bit3
#define OLDPWMLOADST0                 	_BitMacros20.Bits.bit4
#define OLDPWMLOADST1                 	_BitMacros20.Bits.bit5
#define	CatalystTankLevelFlag			_BitMacros20.Bits.bit7


#endif


