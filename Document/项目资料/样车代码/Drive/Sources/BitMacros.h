#ifndef	_BitMacros_h
#define		_BitMacros_h
/*****************************************************/
typedef union 
{
  byte Byte;
  struct 
  {
    unsigned	 bit0      :1; 
    unsigned	 bit1      :1; 
    unsigned	 bit2      :1; 
    unsigned	 bit3      :1;
    unsigned	 bit4      :1; 
    unsigned	 bit5      :1;
    unsigned	 bit6      :1;
    unsigned	 bit7      :1;
  } Bits;
} BitCommon;
////===================common bit macros============================
extern	volatile	BitCommon		_BitMacros0;
#define	BitMacros0						_BitMacros0.Byte
#define	SendSpeedFlag	   			_BitMacros0.Bits.bit0
#define	LoadSTChangFlag			_BitMacros0.Bits.bit1
#define	OpenPossibleFlag			_BitMacros0.Bits.bit2
#define	PowerOff_Flag				_BitMacros0.Bits.bit3
#define	IdRequestFlag				_BitMacros0.Bits.bit4
#define	OnCheckOpen				_BitMacros0.Bits.bit5
#define	motor_st_flag				_BitMacros0.Bits.bit6
#define	ClearErrorFlag				_BitMacros0.Bits.bit7

extern	volatile	BitCommon				_BitMacros1;
#define	BitMacros1							_BitMacros1.Byte
#define	TimeSpeedFlag		   			_BitMacros1.Bits.bit0
#define	SleepStartFlags					_BitMacros1.Bits.bit1
#define	KeyChangFlag					_BitMacros1.Bits.bit2
#define	StDetectOpenFlag				_BitMacros1.Bits.bit3
#define	Capture_Start_Flag			_BitMacros1.Bits.bit4
#define	DriveSendSensorFlag		_BitMacros1.Bits.bit5
#define	MotorStartTooLongFlag	_BitMacros1.Bits.bit6
#define	Capture_Rotation_Flag	_BitMacros1.Bits.bit7

extern	volatile	BitCommon				_BitMacros2;
#define	BitMacros2							_BitMacros2.Byte
#define	EngineRoomTempFlag   	_BitMacros2.Bits.bit0
#define	AirerStartFlag					_BitMacros2.Bits.bit1
#define	AirerFlag								_BitMacros2.Bits.bit2
#define	OilChangeFlag					_BitMacros2.Bits.bit3
#define	GasPress1_ChangeFlag	_BitMacros2.Bits.bit4
#define	GasPress2_ChangeFlag	_BitMacros2.Bits.bit5
#define	WaterSprayStartFlag		_BitMacros2.Bits.bit6
#define	FrontDriveFlag					_BitMacros2.Bits.bit7	

extern	volatile	BitCommon							_BitMacros3;
#define	BitMacros3										_BitMacros3.Byte
#define	BackDriveFlag		   						_BitMacros3.Bits.bit0
///#define	MiddleDriveFlag		   						_BitMacros3.Bits.bit1
#define	TopDriveFlag			   						_BitMacros3.Bits.bit2
#define	IntermissionStartFlag					_BitMacros3.Bits.bit3
#define	IntermissionStopFlag					_BitMacros3.Bits.bit4					
#define	WashStartFlag								_BitMacros3.Bits.bit5
#define	WashStopFlag								_BitMacros3.Bits.bit6
#define	CountWashStopTimeStartFlag		_BitMacros3.Bits.bit7

extern	volatile	BitCommon							_BitMacros4;
#define	BitMacros4										_BitMacros4.Byte
#define	MeterModuleFlag	   						_BitMacros4.Bits.bit0
#define	MeterErrorFlag								_BitMacros4.Bits.bit1
#define	delayxms_flag								_BitMacros4.Bits.bit2
#define	ListenModeFlag								_BitMacros4.Bits.bit3
#define	StartBootloaderFlag						_BitMacros4.Bits.bit4
#define	BakSleepKeyOffFlag						_BitMacros4.Bits.bit5
#define	CloseFrontFlag								_BitMacros4.Bits.bit6
#define	FirstWaterSprayNumFlag				_BitMacros4.Bits.bit7

extern	volatile	BitCommon							_BitMacros5;
#define	BitMacros5										_BitMacros5.Byte
#define	RainScratchIntermissionFlag		_BitMacros5.Bits.bit0
#define	RainScratchOffFlag						_BitMacros5.Bits.bit1
#define	RainScratchRealyOffFlag				_BitMacros5.Bits.bit2
#define	StartToOffFlag								_BitMacros5.Bits.bit3
#define	RunToOffFlag									_BitMacros5.Bits.bit4
#define	ResetLineErrorFlag						_BitMacros5.Bits.bit5
#define	WaterSprayFlag								_BitMacros5.Bits.bit6
#define	FirstIntermissionNumFlag			_BitMacros5.Bits.bit7	

extern	volatile	BitCommon								_BitMacros6;
#define	BitMacros6											_BitMacros6.Byte
#define	Temperature_ChangeFlag				_BitMacros6.Bits.bit0
#define	RainScratchLowStartFlag					_BitMacros6.Bits.bit1
#define	RainScratchLowStartCountFlag		_BitMacros6.Bits.bit2
#define	RainScratchHighStartCountFlag		_BitMacros6.Bits.bit3
#define	RainScratchHighStartFlag					_BitMacros6.Bits.bit4
#define	KeyAdcFlag										_BitMacros6.Bits.bit5
#define	SendConfigDataFlag							_BitMacros6.Bits.bit6	
#define	RainScratchAckFlag							_BitMacros6.Bits.bit7		

extern	volatile	BitCommon								_BitMacros7;
#define	BitMacros7											_BitMacros7.Byte
///#define	EngineProtectStartFlag						_BitMacros7.Bits.bit0
#define	FrontDoorCloseSignalCloseFlag		_BitMacros7.Bits.bit1
#define	FuelLevel_2sFlag								_BitMacros7.Bits.bit2
#define	NoNShieldMotorStartFlag					_BitMacros7.Bits.bit3
#define	SchoolBusBerthFlag							_BitMacros7.Bits.bit4
#define	EngineControlPowerFlag					_BitMacros7.Bits.bit5
#define EngineWaterTempFlag						_BitMacros7.Bits.bit6
#define SendRotationFlag								_BitMacros7.Bits.bit7

extern	volatile	BitCommon							_BitMacros8;
#define	BitMacros8										_BitMacros8.Byte
#define	LoadSTSendFlag							_BitMacros8.Bits.bit0///发送负载状态的前、后标志
#define	IDRequestSendFlag						_BitMacros8.Bits.bit1///发送ID状态的前、后标志
#define	DistanceConfigACKFlag				_BitMacros8.Bits.bit2///里程参数配置的ACK标志

#endif			

