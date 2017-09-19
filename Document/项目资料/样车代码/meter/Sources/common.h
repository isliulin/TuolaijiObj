#undef	EXT
#ifdef	COMMON_C
	#define	EXT
#else
	#define	EXT	extern
#endif
	/* common defines and macros */
	#include <hidef.h>      	
	#include <mc9s12xhz512.h>
	#include	"typedef.h"
  /****************fution macros************************/
	#pragma CODE_SEG  USER_ROM
  void DelayMs(unsigned int Delay);
	void clrwatchdog(void);
	void ClearWatchDog(void);
	void	Clear_External_WatchDog(void);
	void Clear_256_WatchDog(void);
	#pragma CODE_SEG DEFAULT
  /******************parameter macros*******************/
	#define	OSC_Number		16
  #define KeyOn					1
  #define KeyOff				0
  /**********************故障的序列号*******************/
	#define	PanalLoadMulfunction_01					1			///total number 3+3=6

	#define	FrontDriveMulfunction_01				8			///total number 20

	#define	BackDriveMulfunction_01					28		///total number 20

	#define	TopDriveMulfunction_01					48		///total number 20
	
	#define	PowerLoadMulfunction_01					68		///total number 20

	#define	PowerFuseMulfunction_01					88		///total number 20		

	#define PowerContrlLoadMulfunction_01		108		///电源管理模块30个
	#define PowerContrlLoadNum							7			///电源管理模块大电流个数
	// 开路报警10个
	#define OpenLoadMulfunction_01					138		///总共10个
	#define LeftFarLampOpenLoad							OpenLoadMulfunction_01
	#define RightFarLampOpenLoad						(OpenLoadMulfunction_01+1)
	#define LeftNearLampOpenLoad						(OpenLoadMulfunction_01+2)
	#define RightNearLampOpenLoad						(OpenLoadMulfunction_01+3)
	#define LeftFrontFogLampOpenLoad				(OpenLoadMulfunction_01+4)
	#define RightFrontFogLampOpenLoad				(OpenLoadMulfunction_01+5)
  ///sencod kind alarm;total is 16 lines
  #define QianQiYaBaoJing									  148//138+10
	#define	HouQiYaBaoJing										105	
  #define ShuiWenBaoJing										106
  #define JiYouyaliBaoJing									107
  #define ShuiWeiBaoJing										108
  #define DianChiDianYaBaoJing							109
  ///sensor error
  #define QianQiYaChuanGangQi								110
	#define	HouQiYaChuanGangQi								111
	#define youliangchuanganqi								112
  #define JiYouyalichuanganqi								113
	
	///J1939模块通讯故障 共8个	
  #define FaDongJiECUGuZhan									164//148+16
	#define SCRECUGuZhan											165
	#define TyrePressGuZhan										(164+7)		
	//车身模块通信故障 共8个
	#define WuQianQuDongMoKuai								172	//164+8
	#define	WuHouQuDongMoKuai									117	
	#define	WuZhongQuDongMoKuai								118
	#define	WuPeiDianMoKuai										119
	#define	WuDianYuanMoKuai									120
	#define	WuDianYuanGuanLiMoKuai						121	
	//特殊二类故障 共16个
	#define	ZuoSaChePian											180 //172+8
	#define	YouSaChePian											122
	#define	MenBenDiQiYa											123
	#define	BianSuXiang												124
	#define	WeiShengJianShuiWei								125
	#define	FaDongJiCangWen										126
	#define	HuanSuQiGaoWen										127
	#define	KongLvQi													128

//3类故障文字报警8个
	#define ThirdMulfunctionNum								196//180+16
	#define FrontDoorOpenAlarm								(ThirdMulfunctionNum)
	#define MiddleDoorOpenAlarm								(ThirdMulfunctionNum+1)
	#define SafeBeltOpenAlarm								  (MiddleDoorOpenAlarm+1)

//仪表提示信息 8个
	#define MeterNoticeAlarmNum								204//196+8
	#define MeterStartAlarmNum								MeterNoticeAlarmNum					
	#define MeterNdangAlarmNum								(MeterNoticeAlarmNum+1)
	#define MeterStartTooLongAlarmNum					(MeterNoticeAlarmNum+2)
	#define MeterReStartAlarmNum							(MeterNoticeAlarmNum+3)
	
	#define	MulfunctionTotalNum								(MeterNoticeAlarmNum+8)
  /*****************sleep flash macros***************/
  #define SleepCode										0xaa
  #define WakeCode											0x55
  #define LeftFlashOpenCode						0x01
  #define RightFlashOpenCode					0x02
  #define AllFlashOpenCode						0x03
  #define AllFlashCloseCode						0x00
  #define CloseSendCode								0x04                                     			
///传感器状态（电阻型与电压型）
#define	RanYouChuanGanQiState							TopSensorData[1]
#define	QianQiYaChuanGanQiState						TopSensorData[2]
#define	HouQiYaChuanGanQiState							TopSensorData[3]
#define	CheShuChuanGanQiState							carspeed
#define	JiYouYaLiChuanGanQiState						BackSensorData[0]
#define	CheShuXinHaoShuChuState						carspeed
#define	ZhuanSuChuanGanQiState							engSpeed
///捕获传感器状态
#define	CATCH_SENSOR_SIGNAL_NO							0
/*********************common bit macros************************/
  EXT volatile BitCommon _BitMacros0;
  #define BitMacros0										_BitMacros0.Byte
  #define M41t00StopFlag		   					_BitMacros0.Bits.bit0///时间设置时,停止与M4100芯片通讯的标志
  #define RainScratchChangeFlag				_BitMacros0.Bits.bit1///雨刮间歇时间设置更新的标志
	#define BackDriveModuleFlag	   				_BitMacros0.Bits.bit2///后驱模块通讯标志
  #define TopDriveModuleFlag		   			_BitMacros0.Bits.bit3///顶驱模块通讯标志
  #define boot_identify_flag						_BitMacros0.Bits.bit4///身份识别标志
  #define ID_require_flag								_BitMacros0.Bits.bit5///仪表对内CAN的其他节点版本获取的标志
  #define SendMeterLoadFlag							_BitMacros0.Bits.bit6///发送仪表负载状态的标志
  #define	ThirdMenu_Mal_OperatFlag			_BitMacros0.Bits.bit7///故障查询界面按键操作标志

  EXT volatile BitCommon _BitMacros1;
  #define BitMacros1										_BitMacros1.Byte
  #define OilPressFlag	  	 								_BitMacros1.Bits.bit0///机油压力传感器采集标志
  #define FrontDriveModuleFlag   					_BitMacros1.Bits.bit1///前模块通讯标志
	#define SmallDistanceCancelFlag				_BitMacros1.Bits.bit2///清小计标志
  #define	SendDistanceFlag	   		_BitMacros1.Bits.bit3///仪表内CAN发送系统电压等的标志
  #define	MeterSendFuelConsumeFlag1  	 		_BitMacros1.Bits.bit4///仪表内CAN油耗1
  #define	bootloader_wait_flag						_BitMacros1.Bits.bit5///bootloader下载的等待标志
  #define WaterTemperatureFlag						_BitMacros1.Bits.bit6///水温过高标志
  #define	boot_reset_flag									_BitMacros1.Bits.bit7///bootloader下载的软复位标志

  EXT volatile BitCommon _BitMacros2;
  #define BitMacros2											_BitMacros2.Byte
  #define lvi_error											_BitMacros2.Bits.bit0///系统电压异常标志
  #define time_error_flag								_BitMacros2.Bits.bit1///时间设置错误标志
  ///#define OilChangeFlag									_BitMacros2.Bits.bit2///油量检测状态改变标志
  #define GasPress1_ChangeFlag					_BitMacros2.Bits.bit3///前气压检测状态改变标志
  #define GasPress2_ChangeFlag					_BitMacros2.Bits.bit4///后气压检测状态改变标志
  #define FuelLevelFlag									_BitMacros2.Bits.bit5///油量检测状态改变标志
 	#define	WaterTemperature_2sFlag			_BitMacros2.Bits.bit6///水温检测状态改变标志
  #define SleepFlag											_BitMacros2.Bits.bit7///系统休眠标志

  EXT volatile BitCommon _BitMacros3;
  #define BitMacros3									_BitMacros3.Byte
  #define delayxms_flag							_BitMacros3.Bits.bit0	///危急双闪时间间隔标志
  #define buzzer_flag								_BitMacros3.Bits.bit1	///蜂鸣器报警标志
  #define buzzer_ms_flag						_BitMacros3.Bits.bit2	///蜂鸣器间隔时间标志
  #define RainScratchWaitAckFlag		_BitMacros3.Bits.bit3///雨刮时间设置与驱动模块的通讯ACK标志
	#define lvi_flag										_BitMacros3.Bits.bit4///系统电压异常标志
  #define SendMeterKeyFlag					_BitMacros3.Bits.bit5///仪表开关量发送标志
  #define ClearErrorFlag						_BitMacros3.Bits.bit6///关机系统报警状态还原的标志
	#define	SendRecordSwitchFlag			_BitMacros3.Bits.bit7///发送给记录模块的状态信息标志

  EXT volatile BitCommon _BitMacros4;
  #define BitMacros4								_BitMacros4.Byte
  #define WaterTemp_J1939_Flag		_BitMacros4.Bits.bit0///水温信号是J1939还是传感器的选择标志
  #define jycgerrflag													_BitMacros4.Bits.bit1///机油传感器故障标志
  #define Vehicle1MinutueFlag									_BitMacros4.Bits.bit2///里程1.8S计算一次标志
  #define MeterSendFuelConsumeFlag	_BitMacros4.Bits.bit3///仪表内CAN发送机油等状态标志
  #define motor_st_flag							_BitMacros4.Bits.bit4///发动机是否启动标志
	#define BuzzerCancelFlag					_BitMacros4.Bits.bit5///蜂鸣器取消标志
  #define Speed_Data_Flag						_BitMacros4.Bits.bit6///车速从网络其他节点接收标志
  #define SendMeterLedFlag					_BitMacros4.Bits.bit7///发送仪表指示状态的标志

  EXT volatile BitCommon _BitMacros5;
  #define BitMacros5									_BitMacros5.Byte
  #define qy1cgerrflag 								_BitMacros5.Bits.bit0///前气压传感器故障标志
  #define qy2cgerrflag 								_BitMacros5.Bits.bit1///后气压传感器故障标志
  #define ylcgerrflag			    				_BitMacros5.Bits.bit2///油量传感器故障标志
  #define SendMeterPanelTemperatureSensorFlag	_BitMacros5.Bits.bit3///仪表内CAN发送板温等状态标志
  #define MILErrorFlag									_BitMacros5.Bits.bit4///发动机MIL灯指示的标志
  #define oil_j1939_flag							_BitMacros5.Bits.bit5///机油压力是否从J1939获得的标志			
  #define OvercarspeedFlag						_BitMacros5.Bits.bit6///是否车速超速的标志
  #define SCRSystemFlag						  	_BitMacros5.Bits.bit7///后处理通讯标志

 	EXT volatile BitCommon _BitMacros6;
  #define BitMacros6								_BitMacros6.Byte
  #define OvertachospeedFlag				_BitMacros6.Bits.bit0///转速超速的标志
	#define	TW88XX_WatchDog_Flag			_BitMacros6.Bits.bit1///TW88XX的外部看门狗的喂狗标志
	#define threeKindFlag							_BitMacros6.Bits.bit2///三类故障的标志
	#define	Rotation_Data_Flag				_BitMacros6.Bits.bit3///接收外CAN的转速信号的标志
	#define	VideoEnableFlag						_BitMacros6.Bits.bit4///视频切换使能标志
	#define	lcd_flag										_BitMacros6.Bits.bit5///LCD屏显示的状态初始化标志
	#define	ClearEepromFlag						_BitMacros6.Bits.bit6///上位机发送清EEPROM的里程值的标志
	#define	ClearEepromOkFlag					_BitMacros6.Bits.bit7///清EEPROM的里程值OK的标志
	
	EXT volatile BitCommon _BitMacros7;
  #define BitMacros7								_BitMacros7.Byte
  #define Display_SubMenu_Flag										_BitMacros7.Bits.bit0///二级菜单显示标志		
	#define	Display_ThirdMenu_Mulfunction_Flag		_BitMacros7.Bits.bit1///三级菜单故障信息查询显示标志
	#define	Display_MainMenu_Flag									_BitMacros7.Bits.bit2///主界面显示标志
	#define	Display_ThirdMenu_DiagnoseQuery_Flag	_BitMacros7.Bits.bit3///三级菜单诊断信息查询显示标志
	#define	Display_ThirdMenu_PictureSet_Flag			_BitMacros7.Bits.bit4///三级菜单图像设置标志
	#define	Display_ThirdMenu_TimeSet_Flag				_BitMacros7.Bits.bit5///三级菜单时间设置的标志
	#define	Display_ThirdMenu_YuGuaSet_Flag				_BitMacros7.Bits.bit6///三级菜单雨刮间歇时间设置的标志
	#define	Display_ThirdMenu_VersionQuery_Flag	_BitMacros7.Bits.bit7///三级菜单系统版本查询的标志

	EXT volatile BitCommon _BitMacros8;
  #define BitMacros8										_BitMacros8.Byte
  #define VideoPower1ErrorFlag					_BitMacros8.Bits.bit0///视频电源1故障标志
	#define	FirstACCOffFlag								_BitMacros8.Bits.bit1///第一次关闭ACC档的标志
	#define	VideoPower2ErrorFlag					_BitMacros8.Bits.bit2///视频电源2故障标志
	#define	Display_ThirdMenu_FualConsume_Flag		_BitMacros8.Bits.bit3///三级菜单油耗查询标志
	#define	Display_ThirdMenu_DistanceQuery_Flag	_BitMacros8.Bits.bit4///三级菜单里程查询标志
	#define	VideoOperationFlag											_BitMacros8.Bits.bit5///面板按键视频切换标志
	#define	Display_VideoMenu_Flag									_BitMacros8.Bits.bit6///是否显示视频的标志
	#define	DistanceSaveFlag												_BitMacros8.Bits.bit7///里程存储标志	

	EXT volatile BitCommon _BitMacros9;
  #define BitMacros9								_BitMacros9.Byte
	#define TopDriveModuleTemperatureFlag		_BitMacros9.Bits.bit0///顶控模块正、负温标志
	#define	V_POWE4_ErrorFlag					_BitMacros9.Bits.bit1///车速及脉冲输出电源的故障的标志
	#define	SendConfigDataFlag				_BitMacros9.Bits.bit2///发送仪表配置信息给上位机的标志
	#define	EepromClearFlag						_BitMacros9.Bits.bit3///清EEPROM的里程值或者设置里程值的选择标志，当为此标志为0时为清0，否则为设置
	#define	PanelModuleTemperatureFlag				_BitMacros9.Bits.bit5///仪表温度正、负温标志
	#define	FrontDriveModuleTemperatureFlag	_BitMacros9.Bits.bit6///前控模块正、负温标志
	#define	BackDriveModuleTemperatureFlag	_BitMacros9.Bits.bit7///后控模块正、负温标志

	EXT volatile BitCommon _BitMacros10;
  #define BitMacros10									_BitMacros10.Byte
	#define	CatalystTankLevelFlag2			_BitMacros10.Bits.bit0///尿素液位低于10/100的标志  
	#define	PowerModuleTemperatureFlag	_BitMacros10.Bits.bit1///配电模块温度正、负标志
	#define	PowerModuleFlag							_BitMacros10.Bits.bit2///配电模块通讯标志
	#define	DMC_CAN_ModuleFlag					_BitMacros10.Bits.bit3///DMC系统是否通讯的标志
	#define	BMS_CAN_ModuleFlag					_BitMacros10.Bits.bit4///BMS系统是否通讯的标志
	#define	CatalystTankLevelFlag			_BitMacros10.Bits.bit5///尿素液位低于20/100的标志
	#define	EngineECU_CANModuleFlag		_BitMacros10.Bits.bit6///发动机ECU通讯标志
	#define	WaterTempFlahFlag						_BitMacros10.Bits.bit7///水温过高的指示灯闪烁标志
	
	EXT volatile BitCommon _BitMacros11;
  #define BitMacros11										_BitMacros11.Byte
  #define FirstKindFlag									_BitMacros11.Bits.bit0///一类故障标志
  #define SecondKindFlag								_BitMacros11.Bits.bit1///二类故障标志
  #define	WaterTempSensorErrorFlag			_BitMacros11.Bits.bit2///水温传感器故障标志
  #define CountFuelConsumeFlag						    	_BitMacros11.Bits.bit3	///计算百公里油耗的开始标志
	#define FirstTotalFuelConsumeRequireFlag  	_BitMacros11.Bits.bit4///第一次请求总油耗报文的标志
	#define TotalFuelConsumeCompleteFlag				_BitMacros11.Bits.bit5///接收到总油耗的标志
	#define	StartDelayFlag												_BitMacros11.Bits.bit6///启动电源总开关动作延时标志
	#define	Display_ThirdMenu_SystemParameter_Flag		_BitMacros11.Bits.bit7///三级界面的参数设置的显示开始标志

	EXT volatile BitCommon _BitMacros12;
  #define BitMacros12										_BitMacros12.Byte
	#define	SystemParameter_CursorChange_Flag			_BitMacros12.Bits.bit0///系统参数设置的位置指针改变标志
	#define	Display_ThirdMenu_DistanceFualConsume_Flag	_BitMacros12.Bits.bit1///里程油耗查询的显示开始标志
	#define	SmallDistanceClearFlash1s_Flag								_BitMacros12.Bits.bit2///里程小计清零的小图标闪烁标志
	#define	SmallDistanceClearFlash_Flag									_BitMacros12.Bits.bit3
	#define	TyrePressSetSelChange_Flag											_BitMacros12.Bits.bit4
	#define	Display_ThirdMenu_PictureChannle_Flag				_BitMacros12.Bits.bit5		
	#define	PictureChannleSelChange_Flag									_BitMacros12.Bits.bit6
	#define	PictureChannleNumChange_Flag									_BitMacros12.Bits.bit7

	EXT volatile BitCommon _BitMacros13;
  #define BitMacros13											_BitMacros13.Byte
	#define	SystemDataTimeSelChange_Flag				_BitMacros13.Bits.bit0
	#define	SystemDataTimeSet_Flag								_BitMacros13.Bits.bit1///时间操作标志
	#define	Display_ThirdMenu_TyrePress_Flag		_BitMacros13.Bits.bit2
	#define	TyrePressPassWordErr_Flag								_BitMacros13.Bits.bit3
	#define	Display_ThirdMenu_HelpInfomation_Flag	_BitMacros13.Bits.bit4
	#define	HelpInfomationTurnPage_Flag							_BitMacros13.Bits.bit5
	#define	VersionQuery_Flag													_BitMacros13.Bits.bit6
	#define	SystemDataTimeSave_Flag									_BitMacros13.Bits.bit7///当时间被操作后才需要保存的标志

	EXT volatile BitCommon _BitMacros14;
  #define BitMacros14											_BitMacros14.Byte
	#define	TVPowerStability_Flag					_BitMacros14.Bits.bit0///视频板供电是否正常的标志
	#define	BuzzerLevelSetFlag							_BitMacros14.Bits.bit1
	#define	LightLevelSetFlag							_BitMacros14.Bits.bit2
	#define	CountSmallFuelConsumeFlag				_BitMacros14.Bits.bit3
	#define FirstCountSmallConsumeRequireFlag  	_BitMacros14.Bits.bit4///第一次请求总油耗报文的标志
	#define	SmallFuelConsumeBlinkFlag				_BitMacros14.Bits.bit5///闪烁标志
	#define	Display_LogoMenu_Flag						_BitMacros14.Bits.bit6///LOGO显示标志
	#define	VideoSignalExist_Flag						_BitMacros14.Bits.bit7///信号标志				

	EXT volatile BitCommon _BitMacros15;
  #define BitMacros15											_BitMacros15.Byte
	#define	VideoSignalExistOld_Flag				_BitMacros15.Bits.bit0///视频板供电是否正常的标志
	#define	PowerContrlModuleTemperatureFlag	_BitMacros15.Bits.bit1//电源管理模块正负温度		
	#define	PowerContrlModuleFlag							_BitMacros15.Bits.bit2//电源管理模块正负温度
	#define	SendConfigDataWaitFlag						_BitMacros15.Bits.bit3
	#define	SendVirsualMeterKeyFlag						_BitMacros15.Bits.bit4
  #define	MiddleDoorOpenSignalDelayFlag			_BitMacros15.Bits.bit5
	#define	WakeUpInitFlag										_BitMacros15.Bits.bit6
	
	EXT volatile BitCommon _BitMacros16;
  #define BitMacros16											_BitMacros16.Byte
	#define TPMS_CAN_LearnEndFlag						_BitMacros16.Bits.bit0
	#define TPMS_Err_Flag										_BitMacros16.Bits.bit1
	#define TPMS_CAN_ModuleFlag							_BitMacros16.Bits.bit2
	#define TX1939_TPMS_Flag								_BitMacros16.Bits.bit3
	#define TPMS_TX1939_Time_Flag						_BitMacros16.Bits.bit4
	#define Display_CPL500MS_Flag						_BitMacros16.Bits.bit5
	#define HNS_Enter_FL										_BitMacros16.Bits.bit6
	#define TPMS_Moudle_Err_Flag						_BitMacros16.Bits.bit7