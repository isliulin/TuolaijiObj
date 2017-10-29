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
  /**********************���ϵ����к�*******************/
	#define	PanalLoadMulfunction_01					1			///total number 3+3=6

	#define	FrontDriveMulfunction_01				8			///total number 20

	#define	BackDriveMulfunction_01					28		///total number 20

	#define	TopDriveMulfunction_01					48		///total number 20
	
	#define	PowerLoadMulfunction_01					68		///total number 20

	#define	PowerFuseMulfunction_01					88		///total number 20		

	#define PowerContrlLoadMulfunction_01		108		///��Դ����ģ��30��
	#define PowerContrlLoadNum							7			///��Դ����ģ����������
	// ��·����10��
	#define OpenLoadMulfunction_01					138		///�ܹ�10��
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
	
	///J1939ģ��ͨѶ���� ��8��	
  #define FaDongJiECUGuZhan									164//148+16
	#define SCRECUGuZhan											165
	#define TyrePressGuZhan										(164+7)		
	//����ģ��ͨ�Ź��� ��8��
	#define WuQianQuDongMoKuai								172	//164+8
	#define	WuHouQuDongMoKuai									117	
	#define	WuZhongQuDongMoKuai								118
	#define	WuPeiDianMoKuai										119
	#define	WuDianYuanMoKuai									120
	#define	WuDianYuanGuanLiMoKuai						121	
	//���������� ��16��
	#define	ZuoSaChePian											180 //172+8
	#define	YouSaChePian											122
	#define	MenBenDiQiYa											123
	#define	BianSuXiang												124
	#define	WeiShengJianShuiWei								125
	#define	FaDongJiCangWen										126
	#define	HuanSuQiGaoWen										127
	#define	KongLvQi													128

//3��������ֱ���8��
	#define ThirdMulfunctionNum								196//180+16
	#define FrontDoorOpenAlarm								(ThirdMulfunctionNum)
	#define MiddleDoorOpenAlarm								(ThirdMulfunctionNum+1)
	#define SafeBeltOpenAlarm								  (MiddleDoorOpenAlarm+1)

//�Ǳ���ʾ��Ϣ 8��
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
///������״̬�����������ѹ�ͣ�
#define	RanYouChuanGanQiState							TopSensorData[1]
#define	QianQiYaChuanGanQiState						TopSensorData[2]
#define	HouQiYaChuanGanQiState							TopSensorData[3]
#define	CheShuChuanGanQiState							carspeed
#define	JiYouYaLiChuanGanQiState						BackSensorData[0]
#define	CheShuXinHaoShuChuState						carspeed
#define	ZhuanSuChuanGanQiState							engSpeed
///���񴫸���״̬
#define	CATCH_SENSOR_SIGNAL_NO							0
/*********************common bit macros************************/
  EXT volatile BitCommon _BitMacros0;
  #define BitMacros0										_BitMacros0.Byte
  #define M41t00StopFlag		   					_BitMacros0.Bits.bit0///ʱ������ʱ,ֹͣ��M4100оƬͨѶ�ı�־
  #define RainScratchChangeFlag				_BitMacros0.Bits.bit1///��μ�Ъʱ�����ø��µı�־
	#define BackDriveModuleFlag	   				_BitMacros0.Bits.bit2///����ģ��ͨѶ��־
  #define TopDriveModuleFlag		   			_BitMacros0.Bits.bit3///����ģ��ͨѶ��־
  #define boot_identify_flag						_BitMacros0.Bits.bit4///���ʶ���־
  #define ID_require_flag								_BitMacros0.Bits.bit5///�Ǳ����CAN�������ڵ�汾��ȡ�ı�־
  #define SendMeterLoadFlag							_BitMacros0.Bits.bit6///�����Ǳ���״̬�ı�־
  #define	ThirdMenu_Mal_OperatFlag			_BitMacros0.Bits.bit7///���ϲ�ѯ���水��������־

  EXT volatile BitCommon _BitMacros1;
  #define BitMacros1										_BitMacros1.Byte
  #define OilPressFlag	  	 								_BitMacros1.Bits.bit0///����ѹ���������ɼ���־
  #define FrontDriveModuleFlag   					_BitMacros1.Bits.bit1///ǰģ��ͨѶ��־
	#define SmallDistanceCancelFlag				_BitMacros1.Bits.bit2///��С�Ʊ�־
  #define	SendDistanceFlag	   		_BitMacros1.Bits.bit3///�Ǳ���CAN����ϵͳ��ѹ�ȵı�־
  #define	MeterSendFuelConsumeFlag1  	 		_BitMacros1.Bits.bit4///�Ǳ���CAN�ͺ�1
  #define	bootloader_wait_flag						_BitMacros1.Bits.bit5///bootloader���صĵȴ���־
  #define WaterTemperatureFlag						_BitMacros1.Bits.bit6///ˮ�¹��߱�־
  #define	boot_reset_flag									_BitMacros1.Bits.bit7///bootloader���ص���λ��־

  EXT volatile BitCommon _BitMacros2;
  #define BitMacros2											_BitMacros2.Byte
  #define lvi_error											_BitMacros2.Bits.bit0///ϵͳ��ѹ�쳣��־
  #define time_error_flag								_BitMacros2.Bits.bit1///ʱ�����ô����־
  ///#define OilChangeFlag									_BitMacros2.Bits.bit2///�������״̬�ı��־
  #define GasPress1_ChangeFlag					_BitMacros2.Bits.bit3///ǰ��ѹ���״̬�ı��־
  #define GasPress2_ChangeFlag					_BitMacros2.Bits.bit4///����ѹ���״̬�ı��־
  #define FuelLevelFlag									_BitMacros2.Bits.bit5///�������״̬�ı��־
 	#define	WaterTemperature_2sFlag			_BitMacros2.Bits.bit6///ˮ�¼��״̬�ı��־
  #define SleepFlag											_BitMacros2.Bits.bit7///ϵͳ���߱�־

  EXT volatile BitCommon _BitMacros3;
  #define BitMacros3									_BitMacros3.Byte
  #define delayxms_flag							_BitMacros3.Bits.bit0	///Σ��˫��ʱ������־
  #define buzzer_flag								_BitMacros3.Bits.bit1	///������������־
  #define buzzer_ms_flag						_BitMacros3.Bits.bit2	///���������ʱ���־
  #define RainScratchWaitAckFlag		_BitMacros3.Bits.bit3///���ʱ������������ģ���ͨѶACK��־
	#define lvi_flag										_BitMacros3.Bits.bit4///ϵͳ��ѹ�쳣��־
  #define SendMeterKeyFlag					_BitMacros3.Bits.bit5///�Ǳ��������ͱ�־
  #define ClearErrorFlag						_BitMacros3.Bits.bit6///�ػ�ϵͳ����״̬��ԭ�ı�־
	#define	SendRecordSwitchFlag			_BitMacros3.Bits.bit7///���͸���¼ģ���״̬��Ϣ��־

  EXT volatile BitCommon _BitMacros4;
  #define BitMacros4								_BitMacros4.Byte
  #define WaterTemp_J1939_Flag		_BitMacros4.Bits.bit0///ˮ���ź���J1939���Ǵ�������ѡ���־
  #define jycgerrflag													_BitMacros4.Bits.bit1///���ʹ��������ϱ�־
  #define Vehicle1MinutueFlag									_BitMacros4.Bits.bit2///���1.8S����һ�α�־
  #define MeterSendFuelConsumeFlag	_BitMacros4.Bits.bit3///�Ǳ���CAN���ͻ��͵�״̬��־
  #define motor_st_flag							_BitMacros4.Bits.bit4///�������Ƿ�������־
	#define BuzzerCancelFlag					_BitMacros4.Bits.bit5///������ȡ����־
  #define Speed_Data_Flag						_BitMacros4.Bits.bit6///���ٴ����������ڵ���ձ�־
  #define SendMeterLedFlag					_BitMacros4.Bits.bit7///�����Ǳ�ָʾ״̬�ı�־

  EXT volatile BitCommon _BitMacros5;
  #define BitMacros5									_BitMacros5.Byte
  #define qy1cgerrflag 								_BitMacros5.Bits.bit0///ǰ��ѹ���������ϱ�־
  #define qy2cgerrflag 								_BitMacros5.Bits.bit1///����ѹ���������ϱ�־
  #define ylcgerrflag			    				_BitMacros5.Bits.bit2///�������������ϱ�־
  #define SendMeterPanelTemperatureSensorFlag	_BitMacros5.Bits.bit3///�Ǳ���CAN���Ͱ��µ�״̬��־
  #define MILErrorFlag									_BitMacros5.Bits.bit4///������MIL��ָʾ�ı�־
  #define oil_j1939_flag							_BitMacros5.Bits.bit5///����ѹ���Ƿ��J1939��õı�־			
  #define OvercarspeedFlag						_BitMacros5.Bits.bit6///�Ƿ��ٳ��ٵı�־
  #define SCRSystemFlag						  	_BitMacros5.Bits.bit7///����ͨѶ��־

 	EXT volatile BitCommon _BitMacros6;
  #define BitMacros6								_BitMacros6.Byte
  #define OvertachospeedFlag				_BitMacros6.Bits.bit0///ת�ٳ��ٵı�־
	#define	TW88XX_WatchDog_Flag			_BitMacros6.Bits.bit1///TW88XX���ⲿ���Ź���ι����־
	#define threeKindFlag							_BitMacros6.Bits.bit2///������ϵı�־
	#define	Rotation_Data_Flag				_BitMacros6.Bits.bit3///������CAN��ת���źŵı�־
	#define	VideoEnableFlag						_BitMacros6.Bits.bit4///��Ƶ�л�ʹ�ܱ�־
	#define	lcd_flag										_BitMacros6.Bits.bit5///LCD����ʾ��״̬��ʼ����־
	#define	ClearEepromFlag						_BitMacros6.Bits.bit6///��λ��������EEPROM�����ֵ�ı�־
	#define	ClearEepromOkFlag					_BitMacros6.Bits.bit7///��EEPROM�����ֵOK�ı�־
	
	EXT volatile BitCommon _BitMacros7;
  #define BitMacros7								_BitMacros7.Byte
  #define Display_SubMenu_Flag										_BitMacros7.Bits.bit0///�����˵���ʾ��־		
	#define	Display_ThirdMenu_Mulfunction_Flag		_BitMacros7.Bits.bit1///�����˵�������Ϣ��ѯ��ʾ��־
	#define	Display_MainMenu_Flag									_BitMacros7.Bits.bit2///��������ʾ��־
	#define	Display_ThirdMenu_DiagnoseQuery_Flag	_BitMacros7.Bits.bit3///�����˵������Ϣ��ѯ��ʾ��־
	#define	Display_ThirdMenu_PictureSet_Flag			_BitMacros7.Bits.bit4///�����˵�ͼ�����ñ�־
	#define	Display_ThirdMenu_TimeSet_Flag				_BitMacros7.Bits.bit5///�����˵�ʱ�����õı�־
	#define	Display_ThirdMenu_YuGuaSet_Flag				_BitMacros7.Bits.bit6///�����˵���μ�Ъʱ�����õı�־
	#define	Display_ThirdMenu_VersionQuery_Flag	_BitMacros7.Bits.bit7///�����˵�ϵͳ�汾��ѯ�ı�־

	EXT volatile BitCommon _BitMacros8;
  #define BitMacros8										_BitMacros8.Byte
  #define VideoPower1ErrorFlag					_BitMacros8.Bits.bit0///��Ƶ��Դ1���ϱ�־
	#define	FirstACCOffFlag								_BitMacros8.Bits.bit1///��һ�ιر�ACC���ı�־
	#define	VideoPower2ErrorFlag					_BitMacros8.Bits.bit2///��Ƶ��Դ2���ϱ�־
	#define	Display_ThirdMenu_FualConsume_Flag		_BitMacros8.Bits.bit3///�����˵��ͺĲ�ѯ��־
	#define	Display_ThirdMenu_DistanceQuery_Flag	_BitMacros8.Bits.bit4///�����˵���̲�ѯ��־
	#define	VideoOperationFlag											_BitMacros8.Bits.bit5///��尴����Ƶ�л���־
	#define	Display_VideoMenu_Flag									_BitMacros8.Bits.bit6///�Ƿ���ʾ��Ƶ�ı�־
	#define	DistanceSaveFlag												_BitMacros8.Bits.bit7///��̴洢��־	

	EXT volatile BitCommon _BitMacros9;
  #define BitMacros9								_BitMacros9.Byte
	#define TopDriveModuleTemperatureFlag		_BitMacros9.Bits.bit0///����ģ���������±�־
	#define	V_POWE4_ErrorFlag					_BitMacros9.Bits.bit1///���ټ����������Դ�Ĺ��ϵı�־
	#define	SendConfigDataFlag				_BitMacros9.Bits.bit2///�����Ǳ�������Ϣ����λ���ı�־
	#define	EepromClearFlag						_BitMacros9.Bits.bit3///��EEPROM�����ֵ�����������ֵ��ѡ���־����Ϊ�˱�־Ϊ0ʱΪ��0������Ϊ����
	#define	PanelModuleTemperatureFlag				_BitMacros9.Bits.bit5///�Ǳ��¶��������±�־
	#define	FrontDriveModuleTemperatureFlag	_BitMacros9.Bits.bit6///ǰ��ģ���������±�־
	#define	BackDriveModuleTemperatureFlag	_BitMacros9.Bits.bit7///���ģ���������±�־

	EXT volatile BitCommon _BitMacros10;
  #define BitMacros10									_BitMacros10.Byte
	#define	CatalystTankLevelFlag2			_BitMacros10.Bits.bit0///����Һλ����10/100�ı�־  
	#define	PowerModuleTemperatureFlag	_BitMacros10.Bits.bit1///���ģ���¶���������־
	#define	PowerModuleFlag							_BitMacros10.Bits.bit2///���ģ��ͨѶ��־
	#define	DMC_CAN_ModuleFlag					_BitMacros10.Bits.bit3///DMCϵͳ�Ƿ�ͨѶ�ı�־
	#define	BMS_CAN_ModuleFlag					_BitMacros10.Bits.bit4///BMSϵͳ�Ƿ�ͨѶ�ı�־
	#define	CatalystTankLevelFlag			_BitMacros10.Bits.bit5///����Һλ����20/100�ı�־
	#define	EngineECU_CANModuleFlag		_BitMacros10.Bits.bit6///������ECUͨѶ��־
	#define	WaterTempFlahFlag						_BitMacros10.Bits.bit7///ˮ�¹��ߵ�ָʾ����˸��־
	
	EXT volatile BitCommon _BitMacros11;
  #define BitMacros11										_BitMacros11.Byte
  #define FirstKindFlag									_BitMacros11.Bits.bit0///һ����ϱ�־
  #define SecondKindFlag								_BitMacros11.Bits.bit1///������ϱ�־
  #define	WaterTempSensorErrorFlag			_BitMacros11.Bits.bit2///ˮ�´��������ϱ�־
  #define CountFuelConsumeFlag						    	_BitMacros11.Bits.bit3	///����ٹ����ͺĵĿ�ʼ��־
	#define FirstTotalFuelConsumeRequireFlag  	_BitMacros11.Bits.bit4///��һ���������ͺı��ĵı�־
	#define TotalFuelConsumeCompleteFlag				_BitMacros11.Bits.bit5///���յ����ͺĵı�־
	#define	StartDelayFlag												_BitMacros11.Bits.bit6///������Դ�ܿ��ض�����ʱ��־
	#define	Display_ThirdMenu_SystemParameter_Flag		_BitMacros11.Bits.bit7///��������Ĳ������õ���ʾ��ʼ��־

	EXT volatile BitCommon _BitMacros12;
  #define BitMacros12										_BitMacros12.Byte
	#define	SystemParameter_CursorChange_Flag			_BitMacros12.Bits.bit0///ϵͳ�������õ�λ��ָ��ı��־
	#define	Display_ThirdMenu_DistanceFualConsume_Flag	_BitMacros12.Bits.bit1///����ͺĲ�ѯ����ʾ��ʼ��־
	#define	SmallDistanceClearFlash1s_Flag								_BitMacros12.Bits.bit2///���С�������Сͼ����˸��־
	#define	SmallDistanceClearFlash_Flag									_BitMacros12.Bits.bit3
	#define	TyrePressSetSelChange_Flag											_BitMacros12.Bits.bit4
	#define	Display_ThirdMenu_PictureChannle_Flag				_BitMacros12.Bits.bit5		
	#define	PictureChannleSelChange_Flag									_BitMacros12.Bits.bit6
	#define	PictureChannleNumChange_Flag									_BitMacros12.Bits.bit7

	EXT volatile BitCommon _BitMacros13;
  #define BitMacros13											_BitMacros13.Byte
	#define	SystemDataTimeSelChange_Flag				_BitMacros13.Bits.bit0
	#define	SystemDataTimeSet_Flag								_BitMacros13.Bits.bit1///ʱ�������־
	#define	Display_ThirdMenu_TyrePress_Flag		_BitMacros13.Bits.bit2
	#define	TyrePressPassWordErr_Flag								_BitMacros13.Bits.bit3
	#define	Display_ThirdMenu_HelpInfomation_Flag	_BitMacros13.Bits.bit4
	#define	HelpInfomationTurnPage_Flag							_BitMacros13.Bits.bit5
	#define	VersionQuery_Flag													_BitMacros13.Bits.bit6
	#define	SystemDataTimeSave_Flag									_BitMacros13.Bits.bit7///��ʱ�䱻���������Ҫ����ı�־

	EXT volatile BitCommon _BitMacros14;
  #define BitMacros14											_BitMacros14.Byte
	#define	TVPowerStability_Flag					_BitMacros14.Bits.bit0///��Ƶ�幩���Ƿ������ı�־
	#define	BuzzerLevelSetFlag							_BitMacros14.Bits.bit1
	#define	LightLevelSetFlag							_BitMacros14.Bits.bit2
	#define	CountSmallFuelConsumeFlag				_BitMacros14.Bits.bit3
	#define FirstCountSmallConsumeRequireFlag  	_BitMacros14.Bits.bit4///��һ���������ͺı��ĵı�־
	#define	SmallFuelConsumeBlinkFlag				_BitMacros14.Bits.bit5///��˸��־
	#define	Display_LogoMenu_Flag						_BitMacros14.Bits.bit6///LOGO��ʾ��־
	#define	VideoSignalExist_Flag						_BitMacros14.Bits.bit7///�źű�־				

	EXT volatile BitCommon _BitMacros15;
  #define BitMacros15											_BitMacros15.Byte
	#define	VideoSignalExistOld_Flag				_BitMacros15.Bits.bit0///��Ƶ�幩���Ƿ������ı�־
	#define	PowerContrlModuleTemperatureFlag	_BitMacros15.Bits.bit1//��Դ����ģ�������¶�		
	#define	PowerContrlModuleFlag							_BitMacros15.Bits.bit2//��Դ����ģ�������¶�
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