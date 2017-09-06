#undef	EXT
#ifdef	ISR_C
#define	EXT
#else
#define	EXT extern
#endif
	/* common defines and macros */
	#include <hidef.h>      	
	#include <mc9s12xhz512.h>
	#include	"typedef.h"
  /*******************parameter macros**************************/
  #define WatchDogResetParameter1				    	0x55
  #define WatchDogResetParameter2				    	0xaa
	#define	NUM_1S																200
	#define	NUM_2S																400	
	#define	NUM_30S																6000		
  #define M41T00ReadDelayTimeNum								100
  #define AdcTimeNum															3
  #define CanSendMeterSwitchNormalTimeNum	  	60
  #define MeterSendLedToPcTimeNum							49 //5.12ms*49=250ms
  #define CanSendMeterLedTimeNum								50
  #define VehicleSmallDistance1Minute					352	///1.8s=0.125km/250km/h*3600
  #define Delay250MsTimeNum											50
	#define Delay350MsTimeNum											69
  #define Delay200MsNum													40
	#define	AdcDelayNum														10
	#define	Capture6ZeroNUM												200
  #define CanSendBENANRecordSwitchNum					20 //5.12ms*20=100ms
  #define	Num_ACCOff						1954///5.12*1954=10S
  #define	Num_StartOn					1563///5.12*1563=8S

	///speed send time
	#define	NUM_CARSPEED_SEND_TIME			10
	
	#define		Num_OilPress_Change												50
	#define		Num_FuelLevelSensor_KeyOffChange				100
	#define		Num_FuelLevelOilSensor_KeyOnChange			3000
	#define		Num_GasPress_Change							50
	#define		Num_Temperature_Change					50
	#define		NUM_CaptureNoDetect			 200

  #define CAN_STATUS_TX1_MASK      		0X04 /* Bus-Off state mask in register */
  #define CAN_STATUS_RX1_MASK     		0X10 /* Bus-Off state mask in error flag */
  #define CAN_STATUS_TX_MASK      		0X0C /* Bus-Off state mask in register */
  #define CAN_STATUS_RX_MASK     	 		0X30 /* Bus-Off state mask in error flag */
	