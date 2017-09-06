#undef	EXT
#ifdef	KEYDEAL_C
#define	EXT
#else
#define	EXT extern
#endif
	/* common defines and macros */
	#include <hidef.h>      	
	#include <mc9s12xhz512.h>
	#include	"typedef.h"
	/*******************parameter macros**********************/
	#define MeterKeyNum						5
	#define	VolatageCountNum			10
  /******funtion macros****************************/
	#pragma CODE_SEG  USER_ROM
	static void PushKey_Scan(void);
	void menu_action(void);
	static void	Switch_Video(void);
	static void	Switch_Silence(void);
	static void	Switch_MainMenu(void);
	static void	Switch_SecondMenu(void);
	static void	Switch_ThirdMenu_Malfunction(void);
	static void	Switch_ThirdMenu_Diagnose(void);
	static void	Switch_ThirdMenu_SystemParameter(void);
	static void Switch_ThirdMenu_DistanceFualConsume(void);
	static void	Switch_ThirdMenu_PictureAndChannel(void);
	static void	Switch_ThirdMenu_TimeSet(void);
	static void	Switch_ThirdMenu_TyrePress(void);
	static void Switch_ThirdMenu_HelpInfomation(void);
	void VirtualMeterKeyDeal(void);
  void KeyDeal(void);
  void Meterswitchconfig(void);
  static void VolatageValueOut(void);
	static void lvi_cx(void);
	static void	BoardTempDeal(void);
	static uchar AD_Trans(uchar channel);
	void AdcResultProcess(void);
	static void	Hc4051_ChannelSelect(unsigned char Num_Hc4051,unsigned char Channel4051);
	void AdcValueExtract(void);
	void	AdcResultChangetoRealy(void);
	void AdcValueChangeToSwitchState(void);
	static void SendKeyChange(void);
	#pragma CODE_SEG DEFAULT
  /****************ram macros**************************/
  EXT unsigned char key_number;
  EXT unsigned char ad_count;
	EXT unsigned char r_index;
	EXT	SelfKeyStruct MeterKey[MeterKeyNum],SendKey[MeterKeyNum];
	EXT	SelfKeyStruct VirtualMeterKey[2],VirtualMeterKeyLast[2];	
  EXT unsigned int 	delay15ms;
  EXT unsigned char panel_temp,panel_temp_Adc;
  EXT unsigned char count_v;
  EXT unsigned int 	bat_v_buf[VolatageCountNum];///系统电压过滤缓冲寄存器
  EXT unsigned char lvi_index;
  EXT unsigned char Delay_SystemVoltage;///系统电压报警检测时间
  EXT unsigned char keyvalue;
  EXT unsigned char OldKey[MeterKeyNum], FirstKeyTmp[MeterKeyNum],SecondKeyTmp[MeterKeyNum];
  EXT unsigned char rain_scr;
  EXT unsigned char SwLogicConfiguar[MeterKeyNum];
	EXT unsigned char Last_AD[MeterKeyNum][8],AD[MeterKeyNum+1][8];
  EXT unsigned char NormAdcData;	
	EXT unsigned char KeyCounter;
	EXT unsigned char Count_Temperature_Num;
	EXT unsigned char Temperature_Num[2];
	EXT unsigned char menu_id;
	EXT unsigned char MulFunctionQueryCursor;///故障查询的翻页指针
	EXT unsigned char TimeSetCursor;///时间设置的指针位置
	EXT unsigned char	ThirdMenu_DiagnoseQuery_Cursor;///系统诊断的模块查询指针
	EXT unsigned char	ThirdMenu_DiagnoseQuery_FrontDrive_Cursor;///前控制模块查询指针
	EXT unsigned char	ThirdMenu_DiagnoseQuery_TopDrive_Cursor;///顶控制模块查询指针
	EXT unsigned char	ThirdMenu_DiagnoseQuery_BackDrive_Cursor;///后控制模块查询指针
	EXT unsigned char	ThirdMenu_DiagnoseQuery_Power_Cursor;///配电模块查询指针
	EXT unsigned char	ThirdMenu_DiagnoseQuery_Panal_Cursor;///仪表模块查询指针
	EXT unsigned char	ThirdMenu_DiagnoseQuery_PowerManage_Cursor;///电源管理模块查询指针
	EXT unsigned char	ThirdMenu_DiagnoseQuery_BMSPower_Cursor;
	EXT unsigned char MainMenuMulFunctionCursor;
	EXT unsigned char	ChannelSetCursor;
	EXT unsigned char Switch_Video_Cursor;
	EXT SelfKeyStruct MeterKeyBuf[5];
	EXT unsigned char SensorADCData[5];
	EXT unsigned char MeterSensorData[7];	
	///新定义的按键开关
	EXT unsigned char PushKeyUpDelayTime,PushKeyDownDelayTime;
	EXT unsigned char PushKeyFlag;
	///新定义的AD采集
	EXT unsigned char PWM_Out_AD[2];///两路PWM输出电源的AD值
	EXT unsigned char Video_Power[2];///两种CVBS视频图像的电源的AD值
	EXT unsigned char SPEED_PWM_AD;	///车速电源及脉冲输出电源的AD值
	EXT unsigned char V12_Power;///12V基准电压的AD值
/***************io macros*************************/
	#define	PushKey_1_IO		DDRE_DDRE3
	#define	PushKey_2_IO		DDRE_DDRE2
	#define	PushKey_3_IO		DDRC_DDRC3
	#define	PushKey_4_IO		DDRC_DDRC2
	#define	PushKey_5_IO		DDRC_DDRC1
	#define	PushKey_6_IO		DDRC_DDRC0

	#define	PushKey_1		PORTE_PE3
	#define	PushKey_2		PORTE_PE2
	#define	PushKey_3		PORTC_PC3
	#define	PushKey_4		PORTC_PC2
	#define	PushKey_5		PORTC_PC1
	#define	PushKey_6		PORTC_PC0	
	
	#define	HC4051_DA1_IO		DDRT_DDRT3
	#define	HC4051_DB1_IO		DDRT_DDRT4
	#define	HC4051_DC1_IO		DDRT_DDRT5

	#define	HC4051_DA2_IO		DDRC_DDRC6
	#define	HC4051_DB2_IO		DDRC_DDRC7
	#define	HC4051_DC2_IO		DDRT_DDRT0

	#define	HC4051_DA3_IO		DDRP_DDRP4
	#define	HC4051_DB3_IO		DDRK_DDRK4
	#define	HC4051_DC3_IO		DDRK_DDRK5

	#define	HC4051_DA1		PTT_PTT3
	#define	HC4051_DB1		PTT_PTT4
	#define	HC4051_DC1		PTT_PTT5

	#define	HC4051_DA2		PORTC_PC6
	#define	HC4051_DB2		PORTC_PC7
	#define	HC4051_DC2		PTT_PTT0

	#define	HC4051_DA3		PTP_PTP4
	#define	HC4051_DB3		PORTK_PK4
	#define	HC4051_DC3		PORTK_PK5
	/*******************parameter macros**************/
  #define ReferenceVoltage		NormAdcData			///when power is 12v,norm voltage is 1.1v   
  #define ADCCompareLowData		NormAdcData/4		///low state is 0.7v when pull up,availability is 0.2v   
  #define ADCCompareHighData	NormAdcData/6*5 ///high state is 0.7v when pull up,availability is 1.4v   
  #define ADCCompareLow1Data	NormAdcData/5*4	///high state is 1.4v when pull up,availability is 0.7v,10.03.25modify to 5/*4
	
  #define ADCCompareHigh1Data	NormAdcData/2																			
	#define	PowerDeviceStNorm				155	///3V 	
	
  #define BATTER_HIGHER_DATA			30//14v	高电压报警值		
  #define BATTER_LOWER_DATA	    	23//10v 低电压报警值
  
  #define BatterDetectTimeNum	    60

	#define	VOLTAGE_REVISE_NUM			3
  ////push key macros	
  #define MenuTabKeyNum			0x01
  #define MoveUpKeyNum				0x02
  #define MoveDownKeyNum		0x03
	#define EnterKeyNum				0x04
  #define SilenceNum					0x05
  #define VideoChangeNum		0x06

  #define ResetKeyNum				0xff
	#define	PushKeyDelayNum		10
	///雨刮速度参数
  #define RainScratchHighSpeedNum	3
  #define RainScratchLowSpeedNum		9
  #define RainScratchInitSpeedNum	6
	///main menu
	#define	LogoMenuNum							0
	#define	MainMenuNum							1
	#define	SecondMenuNum						2
	#define	ThirdMenuNum							3
	#define	FourMenuNum							4
	#define	FiveMenuNum							5
	#define	VideoMenuNum							0xfe							
	///third menu
	#define	MalfunctionMenuNum						0
	#define	DiagnoseMenuNum								1
	#define	SystemParameterMenuNum				2
	#define	DistanceFualConsumeMenuNum		3
	#define	PictureAndChannleMenuNum			4
	#define	TimeSetMenuNum								5
	#define TyrePressMenuNum							6
	#define	HelpInfomationMenuNum				7

	///图像和视频通道设置
	#define ChannleSetSubMenuNum			0	
	#define PictureSetSubMenuNum			1
	///four menu
	#define	xianshihuamianMenuNum	0
	#define	tongdaoxuanzeMenuNum		1
	///HC4051芯片所对应序列号
	#define	Mcu_U14					0
	#define	Mcu_U15					1
	#define	Mcu_U16					2
	#define	Mcu_U17					3
	#define	Mcu_U18					4
	#define	Mcu_U19					5
	/***************key config************/
	#ifdef	KEYDEAL_C
	volatile const unsigned char  Switch_Config1   @0x7F09  = 0b11000000;
 	volatile const unsigned char  Switch_Config2	 @0x7F0A  = 0b00001011;
 	volatile const unsigned char  Switch_Config3	 @0x7F0B  = 0b00000000;
 	volatile const unsigned char  Switch_Config4	 @0x7F0C  = 0b00000000;
 	volatile const unsigned char  Switch_Config5	 @0x7F0D  = 0b00000000;
 	volatile const unsigned char  Switch_Config6	 @0x7F0E  = 0b00000000;
	//胎压密码
	volatile const uchar TyrePressPassWord_c[4]={0X0A,0X0B,0X0C,0X0D};
	#endif
	/************************按键的重定义***************************/
///front key Redefine	
#define		FrontKey1									FrontDriveKey[0].b.Key0
#define		FrontKey2									FrontDriveKey[0].b.Key1
#define		FrontKey3									FrontDriveKey[0].b.Key2
#define		FrontKey4									FrontDriveKey[0].b.Key3
#define		FrontKey5									FrontDriveKey[0].b.Key4
#define		FrontKey6									FrontDriveKey[0].b.Key5
#define		FrontKey7									FrontDriveKey[0].b.Key6
#define		FrontKey8									FrontDriveKey[0].b.Key7
#define		FrontKey9									FrontDriveKey[1].b.Key0
#define		FrontKey10								FrontDriveKey[1].b.Key1
#define		FrontKey11								FrontDriveKey[1].b.Key2
#define		FrontKey12								FrontDriveKey[1].b.Key3
#define		FrontKey13								FrontDriveKey[1].b.Key4
#define		FrontKey14								FrontDriveKey[1].b.Key5
#define		FrontKey15								FrontDriveKey[1].b.Key6
#define		FrontKey16								FrontDriveKey[1].b.Key7
#define		FrontKey17								FrontDriveKey[2].b.Key0
#define		FrontKey18								FrontDriveKey[2].b.Key1

///back key Redefine	
#define		BackKey1									BackDriveKey[0].b.Key0
#define		BackKey2									BackDriveKey[0].b.Key1
#define		BackKey3									BackDriveKey[0].b.Key2
#define		BackKey4									BackDriveKey[0].b.Key3
#define		BackKey5									BackDriveKey[0].b.Key4
#define		BackKey6									BackDriveKey[0].b.Key5
#define		BackKey7									BackDriveKey[0].b.Key6
#define		BackKey8									BackDriveKey[0].b.Key7
#define		BackKey9									BackDriveKey[1].b.Key0
#define		BackKey10								BackDriveKey[1].b.Key1
#define		BackKey11								BackDriveKey[1].b.Key2
#define		BackKey12								BackDriveKey[1].b.Key3
#define		BackKey13								BackDriveKey[1].b.Key4
#define		BackKey14								BackDriveKey[1].b.Key5
#define		BackKey15								BackDriveKey[1].b.Key6
#define		BackKey16								BackDriveKey[1].b.Key7
#define		BackKey17								BackDriveKey[2].b.Key0
#define		BackKey18								BackDriveKey[2].b.Key1

///top key Redefine	
#define		TopKey1									TopDriveKey[0].b.Key0
#define		TopKey2									TopDriveKey[0].b.Key1
#define		TopKey3									TopDriveKey[0].b.Key2
#define		TopKey4									TopDriveKey[0].b.Key3
#define		TopKey5									TopDriveKey[0].b.Key4
#define		TopKey6									TopDriveKey[0].b.Key5
#define		TopKey7									TopDriveKey[0].b.Key6
#define		TopKey8									TopDriveKey[0].b.Key7
#define		TopKey9									TopDriveKey[1].b.Key0
#define		TopKey10								TopDriveKey[1].b.Key1
#define		TopKey11								TopDriveKey[1].b.Key2
#define		TopKey12								TopDriveKey[1].b.Key3
#define		TopKey13								TopDriveKey[1].b.Key4
#define		TopKey14								TopDriveKey[1].b.Key5
#define		TopKey15								TopDriveKey[1].b.Key6
#define		TopKey16								TopDriveKey[1].b.Key7
#define		TopKey17								TopDriveKey[2].b.Key0
#define		TopKey18								TopDriveKey[2].b.Key1

///power key redefine
#define		PowerKey1								PowerKey[0].b.Key0
#define		PowerKey2								PowerKey[0].b.Key1
#define		PowerKey3								PowerKey[0].b.Key2
#define		PowerKey4								PowerKey[0].b.Key3
#define		PowerKey5								PowerKey[0].b.Key4
#define		PowerKey6								PowerKey[0].b.Key5
#define		PowerKey7								PowerKey[0].b.Key6
#define		PowerKey8								PowerKey[0].b.Key7
//电源管理模块开关
#define		PowerContrlKey1					PowerContrlKey[0].b.Key0
#define		PowerContrlKey2					PowerContrlKey[0].b.Key1
#define		PowerContrlKey3					PowerContrlKey[0].b.Key2
#define		PowerContrlKey4					PowerContrlKey[0].b.Key3
#define		PowerContrlKey5					PowerContrlKey[0].b.Key4
#define		PowerContrlKey6					PowerContrlKey[0].b.Key5
#define		PowerContrlKey7					PowerContrlKey[0].b.Key6
#define		PowerContrlKey8					PowerContrlKey[0].b.Key7

///Meter Key Redefine
#define		MeterKey1									MeterKey[0].b.Key0
#define		MeterKey2									MeterKey[0].b.Key1
#define		MeterKey3									MeterKey[0].b.Key2
#define		MeterKey4									MeterKey[0].b.Key3
#define		MeterKey5									MeterKey[0].b.Key4
#define		MeterKey6									MeterKey[0].b.Key5
#define		MeterKey7									MeterKey[0].b.Key6
#define		MeterKey8									MeterKey[0].b.Key7

#define		MeterKey9								MeterKey[1].b.Key0
#define		MeterKey10								MeterKey[1].b.Key1
#define		MeterKey11								MeterKey[1].b.Key2
#define		MeterKey12								MeterKey[1].b.Key3
#define		MeterKey13								MeterKey[1].b.Key4
#define		MeterKey14								MeterKey[1].b.Key5
#define		MeterKey15								MeterKey[1].b.Key6
#define		MeterKey16								MeterKey[1].b.Key7

#define		MeterKey17								MeterKey[2].b.Key0
#define		MeterKey18								MeterKey[2].b.Key1
#define		MeterKey19								MeterKey[2].b.Key2
#define		MeterKey20								MeterKey[2].b.Key3
#define		MeterKey21								MeterKey[2].b.Key4
#define		MeterKey22								MeterKey[2].b.Key5
#define		MeterKey23								MeterKey[2].b.Key6
#define		MeterKey24								MeterKey[2].b.Key7

#define		MeterKey25								MeterKey[3].b.Key0
#define		MeterKey26								MeterKey[3].b.Key1
#define		MeterKey27								MeterKey[3].b.Key2
#define		MeterKey28								MeterKey[3].b.Key3
#define		MeterKey29								MeterKey[3].b.Key4
#define		MeterKey30								MeterKey[3].b.Key5
#define		MeterKey31								MeterKey[3].b.Key6
#define		MeterKey32								MeterKey[3].b.Key7

#define		MeterKey33								MeterKey[4].b.Key0
#define		MeterKey34								MeterKey[4].b.Key1
#define		MeterKey35								MeterKey[4].b.Key2
#define		MeterKey36								MeterKey[4].b.Key3
#define		MeterKey37								MeterKey[4].b.Key4
#define		MeterKey38								MeterKey[4].b.Key5
#define		MeterKey39								MeterKey[4].b.Key6
#define		MeterKey40								MeterKey[4].b.Key7

//虚拟仪表开关
#define		VirtualMeterKey1									VirtualMeterKey[0].b.Key0
#define		VirtualMeterKey2									VirtualMeterKey[0].b.Key1
#define		VirtualMeterKey3									VirtualMeterKey[0].b.Key2
#define		VirtualMeterKey4									VirtualMeterKey[0].b.Key3
#define		VirtualMeterKey5									VirtualMeterKey[0].b.Key4
#define		VirtualMeterKey6									VirtualMeterKey[0].b.Key5
#define		VirtualMeterKey7									VirtualMeterKey[0].b.Key6
#define		VirtualMeterKey8									VirtualMeterKey[0].b.Key7

#define		VirtualMeterKey9									VirtualMeterKey[1].b.Key0
#define		VirtualMeterKey10									VirtualMeterKey[1].b.Key1
#define		VirtualMeterKey11									VirtualMeterKey[1].b.Key2
#define		VirtualMeterKey12									VirtualMeterKey[1].b.Key3
#define		VirtualMeterKey13									VirtualMeterKey[1].b.Key4
#define		VirtualMeterKey14									VirtualMeterKey[1].b.Key5
#define		VirtualMeterKey15									VirtualMeterKey[1].b.Key6
#define		VirtualMeterKey16									VirtualMeterKey[1].b.Key7

#if 0
////meter key realy
#define FrontFogKeyBit	   					    	MeterKey1///前雾灯开关
#define	EngineRoomTemperatureKeyBit				MeterKey2///仓温报警信号开关
///#define BackDoorOpenKeyBit								MeterKey3
///#define	MeetAnEmergencyKeyBit						MeterKey4
#define	InsideRoomLamp2KeyBit							MeterKey5///室内灯2 开关
///#define FrontDoorOpenKeyBit							MeterKey6
#define AccKeyBit   				        			MeterKey7///ACC档开关
#define StartKeyBit 			   	 						MeterKey8///ST 启动开关
#define GearBoxMalfunctionKeyBit					MeterKey9///变速箱故障信号开关
#define GearBoxWarnKeyBit									MeterKey9///变速箱故障信号开关
#define OnKeyBit													MeterKey10///ON档开关
#define SyringeKeyBit											MeterKey11///洗涤器开关
#define DangerWarnKeyBit			        		MeterKey12///危急报警开关
#define InsideRoomLamp1KeyBit							MeterKey13///室内灯1 开关
///#define BackDoorCloseKeyBit   							MeterKey14
///#define ParkingVehicleSignalOpenKeyBit			MeterKey15
///#define BackFogKeyBit											MeterKey16 
#define LocationLampKeyBit             		MeterKey17///位置灯开关
#define RightTurnLampKeyBit            		MeterKey18///右转向灯开关
#define RainScratchLowKeyBit							MeterKey19///雨刮低速开关
#define ReadLampKeyBit										MeterKey20///阅读灯开关
#define BackFogKeyBit 			   	 					MeterKey21///后雾灯开关
#define LeftTurnLampKeyBit								MeterKey22///左转向灯开关
#define BackDoorCloseKeyBit								MeterKey23///中门关闭开关
#define MiddleDoorCloseKeyBit							MeterKey23///中门关闭开关
#define BackDoorOpenKeyBit								MeterKey24///中门打开开关
#define MiddleDoorOpenKeyBit							MeterKey24///中门打开开关
#define FrontDoorCloseKeyBit			        MeterKey25///前门关闭开关
#define	FrontDoorOpenKeyBit								MeterKey26///前门打开开关
#define RainScratchIntermissionKeyBit			MeterKey27///雨刮间歇开关
#define RainScratchHighKeyBit							MeterKey28///雨刮高速开关
#define	ASRMulfunctionIndicateKeyBit			MeterKey29///ASR故障指示开关
///#define	MeterBakKey3										MeterKey30
///#define	EngineMILWarnKeyBit							MeterKey31
#define	ABSMulfunctionIndicateKeyBit			MeterKey32///ABS故障指示开关
///#define	MeterBakKey6										MeterKey33
///#define	Defrost2KeyBit									MeterKey34
///#define	Defrost1KeyBit									MeterKey35
///#define	HandCoinBoxKeyBit								MeterKey36
///#define	CoinBoxLampKeyBit								MeterKey37
///#define	FannerKeyBit										MeterKey38
///#define	MeterBakKey7										MeterKey39
////front drive key

////front drive key
#define BakOnKeyBit											FrontKey1///备用ON 档开关
#define BakStartKeyBit									FrontKey2///备用ST档开关
#define BakDangerWarnKeyBit							FrontKey3///备用危急报警开关
#define	FrontAirPressKeyBit							FrontKey4///前气压报警开关
#define	FrontAirPressWarnKeyBit					FrontKey4///前气压报警开关
#define	RetarderWorkIndicateKeyBit			FrontKey5///缓速器工作指示
#define	DelaySpeedWorkIndicateKeyBit		FrontKey5///缓速器工作指示
#define RainScratchResetKeyBit					FrontKey6///雨刮复位开关
///#define LeftFrontStopHoofKeyBit					FrontKey7
#define	NightSteppedKeyBit							FrontKey8///夜行灯开关
#define FarLampKeyBit										FrontKey9///喇叭开关
#define	NearLampKeyBit									FrontKey10///前门开信号开关
#define BugleKeyBit											FrontKey11///远光灯开关
#define FrontDoorOpenSignalKeyBit				FrontKey12///近光灯开关
#define	BackDoorOpenSignalKeyBit				FrontKey13///中门开信号开关
#define	MiddleDoorOpenSignalKeyBit			FrontKey13///中门开信号开关
#define	BackAirPressKeyBit							FrontKey14///后气压报警开关
#define	BackAirPressWarnKeyBit					FrontKey14///后气压报警开关
///#define BackDoorOpenSignalKeyBit				FrontKey15
#define LeftFrontStopHoofKeyBit					FrontKey16///左前蹄片报警开关
#define	RightFrontStopHoofKeyBit				FrontKey17///右前蹄片报警开关

#define FootStopKeyBit				  	 			BackKey1///刹车灯开关
#define EngineWaterWarnKeyBit						BackKey2///水温报警开关
#define	HandStopKeyBit									BackKey3///手刹车开关
#define PackingRoomLampKeyBit						BackKey4///行李仓灯开关
#define FualPressWarnKeyBit							BackKey5///机油压力报警 开关
#define LowWaterLevelKeyBit							BackKey6///低水位报警开关
#define EngineLowerWaterKeyBit					BackKey6///低水位报警开关
#define RShieldSignalKeyBit							BackKey7///R档信号开关
#define AirFiltrateWarnKeyBit						BackKey9///空滤器报警开关
#define BackRoomKeyBit									BackKey10///后仓门开关
#define LeftBackStopHoofKeyBit					BackKey16///左后蹄片报警开关
#define	RightBackStopHoofKeyBit					BackKey17///右后蹄片报警开关
#define EngineChargeIndicateKeyBit			BackKey18///发电机充电指示开关

/
///power control key
#define	BackStartKeyBit								PowerContrlKey1///后启动开关
#define	BackFlameoutKeyBit						PowerContrlKey2///后熄火开关
#define	NShieldSignalKeyBit						PowerContrlKey3///N档信号开关
#endif

#define RainScratchHighSpeedKeyBit				MeterKey28///雨刮高速开关
#define RainScratchIntermissionKeyBit			MeterKey27///雨刮间歇开关
#define RainScratchLowSpeedKeyBit					MeterKey19///雨刮低速开关
#define FrontDoorOpenKeyBit								MeterKey26///前门打开开关
#define FrontDoorCloseKeyBit	        		MeterKey25///前门关闭开关
#define LocationLampKeyBit             		MeterKey17///位置灯开关
#define LeftTurnLampKeyBit								MeterKey22///左转向灯开关
#define RightTurnLampKeyBit            		MeterKey18///右转向灯开关
#define BackFogKeyBit		   	 							MeterKey21///后雾灯开关
#define	ABSMulfunctionIndicateKeyBit			MeterKey32///ABS故障指示开关
#define	ASRMulfunctionIndicateKeyBit			MeterKey29///ASR故障指示开关
#define	FrontFogKeyBit	   					    	MeterKey1///前雾灯开关
#define	DelaySpeedWorkIndicateKeyBit			MeterKey16///缓速器工作指示	
#define	EngineRoomTemperatureKeyBit			 	MeterKey2///仓温报警信号
#define	Defrost1KeyBit										MeterKey15///除霜1开关
#define Defrost2KeyBit										MeterKey3///除霜2开关
#define DangerWarnKeyBit			        		MeterKey12///危急报警开关
#define SyringeKeyBit											MeterKey11///洗涤器开关
#define AccKeyBit   				        			MeterKey7///ACC档开关
#define	OnKeyBit													MeterKey10///ON档开关
#define StartKeyBit 			   	 						MeterKey8///ST 启动开关

//前驱模块
#define BakOnKeyBit												FrontKey1///备用ON档开关
#define BakStartKeyBit 			   	 					FrontKey3///备用ST启动开关
#define BakDangerWarnKeyBit			        	FrontKey5///备用危急报警开关
#define FarLampKeyBit	 					 					FrontKey9///远光灯开关
#define NearLampKeyBit										FrontKey11///近光灯开关

#define BugleKeyBit												FrontKey2///喇叭按钮
#define FrontDoorOpenSignalKeyBit					FrontKey4///前门开信号开关
#define	FrontAirPressKeyBit								FrontKey8///前气压报警开关
#define	FrontAirPressWarnKeyBit 					FrontKey8///前气压报警开关
#define BackAirPressKeyBit								FrontKey10///后气压报警
#define BackAirPressWarnKeyBit						FrontKey10///后气压报警
#define ReadLampKeyBit										FrontKey12///阅读灯开关

//后驱模块
 
#define RShieldSignalKeyBit								BackKey1///R档信号开关
#define	NShieldSignalKeyBit								BackKey3///N档信号开关
#define	AirFiltrateWarnKeyBit							BackKey5///空滤报警开关
#define BackRoomKeyBit										BackKey7///后仓门开关
#define EngineWaterWarnKeyBit							BackKey9///水温报警开关
#define	HandStopKeyBit										BackKey11///手刹车开关

#define	BackFlameoutKeyBit								BackKey2///后熄火开关
#define	BackStartKeyBit										BackKey4///后启动开关
#define FootStopKeyBit				  	 				BackKey8///刹车灯开关
#define FualPressWarnKeyBit								BackKey10///机油压力报警开关
#define LowWaterLevelKeyBit								BackKey13///低水位报警开关
#define EngineLowerWaterKeyBit						BackKey13///低水位报警开关
#define	EngineChargeIndicateKeyBit				BackKey15///发动机充电指示灯

///此开关如需使用，要另做配置
#define	UpVideoSignalKeyBit							KeyOff
#define	GearBoxWarnKeyBit								KeyOff
#define BackDoorOpenSignalKeyBit				KeyOff///中门开信号开关
#define	MiddleDoorOpenSignalKeyBit			KeyOff///中门开信号开关

