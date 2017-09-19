#include "includes.h"
#include "deepSleep.h"
#include "BacklightControl.h"
#include "TJA1145.h"




uint32_t showCnt;
uint8_t showFlag = 0;

void UpDownData(void) 
{
 // unsigned char i;
    if (UPDOWN_F)    //1s
    {
       UPDOWN_F = 0;
       ClearVersion();
    }  
}   
extern void SCG_Init8M(void);
extern void SCG_Init80M(void);
UINT8 changeFrq(UINT8 sysFrq){
		if((!SleepFlag)||(ON_LOAD == 1)){
			if(sysFrq != 80){
		  	FeedWatchDog();
			//sysInit();
		
			  SCG_Init80M();
			  sysFrq = 80;
			  //MCU电源 深度休眠
			  MCU_5VEN_MUX();MCU_5VEN_PDDR_O;//5V电源
			  MCU_5VEN_H;
			 
		  }
		}
		else 
		{
		 	if(sysFrq != 8){
				FeedWatchDog();
				SCG_Init8M();
				sysFrq = 8;
				//MCU电源 深度休眠
				MCU_5VEN_MUX();MCU_5VEN_PDDR_O;//5V电源
				MCU_5VEN_H;
				//SleepFlag = 0;
			}
		}

		return sysFrq;
}
extern void (*TW8835_I2C_delay)(void);
unsigned long abc=1;

extern void construct1(void);
extern BYTE ReadI2C(BYTE addr, BYTE index);

UINT8 sysFrq = 0;
UINT8 IS_id= 0;
UINT8 eeromBuf[8];

int main()
{  
        UINT8 i;
	sysInit();
       
	//lpspiInit(0,MASTER);
	TW8835_I2C_delay = I2Cdelay;
	OLD_ON_KEY = 0;

	sysFrq = 80;


        Init_3738();
       // FTM_PWM_init(PWMOUT2_MODULE, PWMOUT2_CH, 500,50);
      //lptmr_set_hz(100);
 
	while(1)
	{	
		
      
		FeedWatchDog();  
		KeyDeal();
		
		doJ1939Msg();
		updataRealTime();
		doLoads();
		getDriverLoadImage();
             doSensor();
		
        sysFrq = changeFrq(sysFrq);   
		//Sys_Post_Flag = 1;
		//ON_LOAD = 1;
		if(Sys_Post_Flag&&ON_LOAD)
		{
                  
		       motorPost();	
			BACKGROUD_LED_DUTY = FTM_PWM_Duty(TFT_LINGHT_FRQ,60);
		} 
		else 
		{
			FeedWatchDog();
		  	driverLoads();
			doFaultWarning();
			StepDeal();
		}
		doDiagnoseStatus();

		//深度休眠基于普通休眠之上，先进入普通休眠-> 深度休眠
		dodeepSleep((unsigned char)(DEEP_SLEEP_LOAD&&SleepFlag));
		doAccOff();
		doOnOff(); 
		sound();
		ModuleDetect();
		MesseageDeal();
		renewSound();
		    
		displayMain();
                 
		insidePhysStatus();
		SpeedPwmOut();
		 
		LoadStatusCopy();
		doJ1939Translate();
		J1939ID_OverTime();
		UpDownData();
		FindEngineDiagnose();
		EngineDiagnoseLost();
		CarMaintain();
		if(SetMileageFlag) 
		{
			SetMileageFlag = 0;
			USR_s_DeviceConfigData.TotalDistance = PcFromMileage*10000;
			PowerOffSave();  
		}   
		//setBacklightPluse();
           

	}

	return 0;
}
