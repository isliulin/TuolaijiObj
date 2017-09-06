#define 	KEYDEAL_C
#include	"KeyDeal.h"
#include "HwMacroPatch.h"




LCD_MSG LcdMsg;
UINT8 AdcValue[50];
UINT8 wheel_key[2];
BitCommon KeyFlag1;
BitCommon KeyFlag2;
BitCommon KeyFlag3;

UINT8 CheckVoltageValue[4]; //ap:电压adc检测存放,电路有改动 
UINT8  BoardTemperatureAdValue; //板温电压adc值
SelfKeyStruct MeterKeyNew[7];
SelfKeyStruct MeterKeyOld[7];
UINT8 KeyADValueTemp[40];
UINT8 OddKey;
UINT8 OldOddKey;
UINT8 OddKeyValue;
UINT8 EvenKey;
UINT8 OldEvenKey;
UINT8 EvenKeyValue;
UINT8 CheckVoltageCnt[4];
SelfKeyStruct PwmShortStNew;
SelfKeyStruct PwmShortStOld;
SelfKeyStruct PwmShortSt;

//UINT8 AN_PowCnt=0;  //电阻型与电压性选择

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
功能：本文件定义数据初始化
*******************************************************************************/
void keyDeal_dataInit(void){
	wheel_key[0] = 0;
	wheel_key[1] = 0;
	initKeyVariable();
}
/********************************************************************************************* 
函数名：void initKeyVariable(void) 
*********************************************************************************************/
void initKeyVariable(void)
{
    uchar i;
    for (i=0;i<5;i++)
    {
        MeterKeyOld[i].Data = UserKeyConfig.KeyDefaultValue[i];
        MeterKeyNew[i].Data = UserKeyConfig.KeyDefaultValue[i];
        USR_s_OV.DashboardKey[i].Data = UserKeyConfig.KeyDefaultValue[i];
    }
    for(i=0;i<2;i++)
    {
        USR_s_OV.FrontDriveKey[i].Data = 0x00;
        USR_s_OV.BackDriveKey[i].Data = 0x00;
        USR_s_OV.TopDriveKey[i].Data = 0x00;
    }
}


void PwmShortCheck(void) 
{
	PwmAdcValue[0] =  AdcValue[16];  //pwm2
	PwmAdcValue[1] =  AdcValue[17];  //pwm1
	if(PwmboardLoad.St.S0==0x03)     // ja13
	{
		if(PwmAdcValue[0]<10)//(REFERENCE_VOLTAGE/2)) 
		{
			PwmShortStNew.b.Key0  = 1;
		} 
		else  PwmShortStNew.b.Key0  = 0;
		if(PwmShortStNew.b.Key0!=PwmShortStOld.b.Key0) 
		{
			PwmShortStOld.b.Key0 =  PwmShortStNew.b.Key0;
		} 
		else
		{
			PwmShortSt.b.Key0 = PwmShortStNew.b.Key0;
		}
	}
	
	if(PwmboardLoad.St.S1==0x03)   //ja23   12v
	{
		if(PwmAdcValue[1]<31)   //62
		{
			PwmShortStNew.b.Key1  = 1;
		} 
		else  PwmShortStNew.b.Key1  = 0;
		if(PwmShortStNew.b.Key1!=PwmShortStOld.b.Key1) 
		{
			PwmShortStOld.b.Key1 =  PwmShortStNew.b.Key1;
		} 
		else
		{
			PwmShortSt.b.Key1 = PwmShortStNew.b.Key1;
		} 
	}
}

/*********************************************************************************
函数名：void gear1WarnKey(void)
功能：一级报警图标虚拟按键。
*********************************************************************************/
void gear1WarnKey(void)
{
	UINT8   iconF;
	static UINT16 cnt=0;
	
	iconF = 0;
	if(DashboardLoadsSound[0]||DashboardLoadsSound[1])
	{
		iconF = 1;
		//Test_wain = 1;
	}
	if(ModuleConfigTemp & 0x01)
	{
		if(FrontDriveLoadsSound[0] || FrontDriveLoadsSound[1] || FrontDriveLoadsSound[2])
		{
			iconF=1;
			// Test_wain = 2;
		}
	}
	
	if(ModuleConfigTemp & 0x02)
	{
		if(BackDriveLoadsSound[0] || BackDriveLoadsSound[1] || BackDriveLoadsSound[2])
		{
			iconF=1;
			//Test_wain = 3;
		}
	}
	
	if(ModuleConfigTemp & 0x04)
	{
		if(MiddleDriveLoadsSound[0] || MiddleDriveLoadsSound[1] || MiddleDriveLoadsSound[2])
		{
			iconF=1;
			// Test_wain = 4;
		}
	}
	
	if(ModuleConfigTemp & 0x08)
	{
		if(TopDriveLoadsSound[0] || TopDriveLoadsSound[1] || TopDriveLoadsSound[2])
		{
			iconF=1;
			// Test_wain = 5;
		}
	}
	
	if(ModuleConfigTemp & 0x10)
	{
		if(PowerLoadsSound[0] || PowerLoadsSound[1] || PowerLoadsSound[2])
		{
			iconF=1;
			// Test_wain = 6;
		}
		if(PowerLoadsFuse[0] || PowerLoadsFuse[1] || PowerLoadsFuse[2]) 
		{
			// iconF=1;
			if(ACC_KEY)  cnt++;
			else  cnt = 0;
			if(cnt>25) 
			{
				cnt = 25;

				iconF=1;
			}
		} 
		else 
		{
			cnt = 0;
		}
	} 
	
	if(ModuleConfigTemp & 0x40) //电源管理
	{
		if(PowerContorolLoadSound[0] || PowerContorolLoadSound[1] || PowerContorolLoadSound[2])
		{
			iconF=1;
		}
	} 

	if( LogicOutPut[4].Loads.Load1|| iconF)
	{
		GEAR_1_F = 1;
	}
	else
	{
		GEAR_1_F = 0; 
	}  
}
/*********************************************************************************
函数名：void gear2WarnKey(void)
功能：二级报警图标虚拟按键。         hong
*********************************************************************************/
void gear2WarnKey(void)
{
    if(ModuleErrorFlag.Byte || LogicOutPut[5].Loads.Load7)
    {
        GEAR_2_F = 1;
    }
    else
    {
        GEAR_2_F = 0; 
    }
}

/********************************************************************************
函数名：void virtualKey(void)
功能：获取虚拟按键
#define   CHARGING_INDICATOR_GO_OUT_5S  VirtualLoad[2].Loads.Load3
********************************************************************************/
void doVirtualKey(void)
{
    gear1WarnKey();
    gear2WarnKey();
}




/***************************************************************************
* 原来的两个视频控制源及车速转速传感器的电源合并为一个 VV-12统一控制
***************************************************************************/
void V12_PowerDeal(void)
{
#if defined(ZB223BYY00) || defined(ZB223YY01) ||defined(ZB223YY0A)
	if(POWER_12VV_READ && (CheckVoltageCnt[0] < 3)){
		if(CheckVoltageValue[0] < 0x30)  CheckVoltageCnt[0]++;
		if(CheckVoltageCnt[0] == 3) 
		{
			//TV_POWER_L;	//关闭电源
			POWER_12VV_L;
			CheckVoltageCnt[0] = 0;
		}
	}
	else{
		CheckVoltageCnt[0] = 0;
	}

	if((TV_POWER_READ)&&(CheckVoltageCnt[1]<3)) 
	{
		if(CheckVoltageValue[1] < 0x30)  //  3.3
		CheckVoltageCnt[1]++;
		if(CheckVoltageCnt[1]==3) 
		{
			TV_POWER_L;//关闭电源
			CheckVoltageCnt[1] = 0;
		}
	}
	else 
	{
		CheckVoltageCnt[1] = 0;
	}
#elif defined(ZB206B)
if((EN_12VCP_READ)&&(CheckVoltageCnt[3]<8)) 
	{
		if(CheckVoltageValue[3]<170) CheckVoltageCnt[3]++;
		if(CheckVoltageCnt[3]==8) 
		{
			EN_12VCP_L;
			CheckVoltageCnt[3] = 0;
		}
	}
	else 
	{
		CheckVoltageCnt[3] = 0;
	}  
#endif
}

/*********************************************************************************
funtion operation:the process of deal key
funtion call:void sw_read(void);void AdcResultChange(void);void KeyProcess(void);
void lvi_cx(void);void menu_action(void);	
input parameter:void
return:void
 ********************************************************************************/
void KeyDeal(void)
{
	AdcValueExtract();
	AdcValueChangeToSwitchState();
	getInsideVirtualKey(&USR_s_OV.DashboardVirtualKey[0]);

	FeedWatchDog();
	startStatus(); 
	loadTovirtualKey();
	doVirtualKey();
	V12_PowerDeal();
	PwmShortCheck();
	if(KEY_SCANF_10MS_F) 
	{
		FeedWatchDog();
		KEY_SCANF_10MS_F = 0;
		scanKey();
	}
}

/**************************************************************************************** 
函数名oid	AdcResultChangetoRealy(void)
功能：对*pKey中的按键值进行整理 
****************************************************************************************/
void	AdcResultChangetoRealy(void)
{
	UINT8 i;
	for(i=0;i<=15;i++)
		KeyADValueTemp[i] = AdcValue[i];
	for(i=16;i<=27;i++)
		KeyADValueTemp[i] = AdcValue[i+2]; //AdcValue[16];AdcValue[17]  pwm
	for(i=28;i<=31;i++)
		KeyADValueTemp[i] = AdcValue[i+4];
	for(i=32;i<=38;i++)
		KeyADValueTemp[i] = AdcValue[i+9];
	KeyADValueTemp[39] = 0; 
        
       
        
        




}

/*********************************************************************************************** 
 函数名：UINT8 adcValueToSwitch(UINT8 *pAdcValue, UINT8 style)
 功能：把AD转换的值转化为开关量
 参数说明：UINT8 *pAdcValue -> ADC值，8个值转化为1个字节。
           UINT8 style -> 类型，分两种：只能配高和高低均可以配置
 返回值：一个字节开关量
***********************************************************************************************/
/************************************************************************************
* @brief			
* @parameter			keyConfog
* 						*pAdcValue
*						*pBuf		
*						len			处理个数
*************************************************************************************/
void adcValueToSwitch(STRUCT_KEY_CONFIG  keyConfog ,UINT8 *pAdcValue, UINT8 *pBuf,UINT8 len)
{
    UINT8 ucI;
    UINT8 ucRow,ucLine;
    UINT8 ucStyle; 

	for(ucI=0;ucI<len;ucI++)
    {
        ucRow   =  ucI / 0x04;
        ucLine  = ucI% 0x04;
        ucStyle = keyConfog.KeyStyleConfig[ucRow];
        ucStyle = (ucStyle>>(2 * ucLine));
        //__asm("nop");
        ucStyle &=0x03;
        //__asm("nop");
        if(ucI == 38) ucStyle = LOW_ONLY;//L: 硬件更改
        else if((ucI==6))ucStyle = HIGH_ONLY;
        //else if((ucI==5)||(ucI==10)||(ucI==6))ucStyle = HIGH_ONLY;
        switch(ucStyle)
        {
            case LOW_ONLY:lowOnlyKey(pAdcValue,pBuf,ucI);break;
            case HIGH_ONLY:highonlykey(pAdcValue,pBuf,ucI);break;
            case HIGH_LOW:lowHighKey(pAdcValue,keyConfog.KeySwitchConfig,pBuf,ucI);break;
            default:     break;
        }
    }
}

/******************************************************************************
funtion operation:compare the switch data,if it is max or min
funtion call:void
input parameter:val_max,the max data of compare; val_min,the min data of compare
return:void
******************************************************************************/
extern void  CAN0_send( CAN_PACKET_T  *pcanframe) ;
#if defined(ZB223BYY00) || defined(ZB223YY01)
#define KEY_TOTAL	39
#elif	defined(ZB206B)
#define  KEY_TOTAL	25
#endif
void AdcValueChangeToSwitchState(void)
{   
    uchar i;
    AdcResultChangetoRealy();
	
    adcValueToSwitch(UserKeyConfig,KeyADValueTemp,&MeterKeyNew[0].Data,KEY_TOTAL);
	
    if ((MeterKeyOld[0].Data != MeterKeyNew[0].Data) || (MeterKeyOld[1].Data != MeterKeyNew[1].Data) 
        || (MeterKeyOld[2].Data != MeterKeyNew[2].Data) || (MeterKeyOld[3].Data != MeterKeyNew[3].Data) 
        || (MeterKeyOld[4].Data != MeterKeyNew[4].Data))
    {
        for (i=0;i<5;i++)
        {
            MeterKeyOld[i].Data = MeterKeyNew[i].Data;
        }
    } 
    else
    {
        if ((USR_s_OV.DashboardKey[0].Data != MeterKeyOld[0].Data) || (USR_s_OV.DashboardKey[1].Data!=MeterKeyOld[1].Data)
            || (USR_s_OV.DashboardKey[2].Data!=MeterKeyOld[2].Data) || (USR_s_OV.DashboardKey[3].Data!=MeterKeyOld[3].Data)
            || (USR_s_OV.DashboardKey[4].Data!=MeterKeyOld[4].Data))
        {  
        	CAN_PACKET_T SendFrame;

			if(MeterKeyNew[1].b.Key1)  MeterKeyNew[0].b.Key6 = 1;
				//USR_s_OV.DashboardKey[0].b.Key6=1;
            for (i=0;i<5;i++)
            {
                USR_s_OV.DashboardKey[i].Data = MeterKeyNew[i].Data;
				SendFrame.data[i] = USR_s_OV.DashboardKey[i].Data;
            }
			
			SendFrame.identifier = 0x00002002;
			SendFrame.byte_count = 8;
			SendFrame.data[5] = USR_s_OV.DashboardKey[5].Data;
			SendFrame.data[6] = USR_s_OV.DashboardKey[6].Data;
			SendFrame.data[7] = 0xff;
			DisableInterrupts;
			CAN0_send(&SendFrame);
			EnableInterrupts;
            //PDO_SignalNewObjData(  (UINT8 XDATA*)&USR_s_OV.DashboardKey);
        }
    } 
   
    MeterKeyNew[5].b.Key0  = TURN_LEFT_LED;//MLed7; //左转向灯
    MeterKeyNew[5].b.Key1  = TURN_RIGHT_LED;//MLed10;//右转向灯
    MeterKeyNew[5].b.Key2  = SLIGHT_FAULT;//MLed13;//轻微故障
    MeterKeyNew[5].b.Key3  = SERIOUS_FAULT;//MLed16;//严重故障报警
    MeterKeyNew[5].b.Key4  = MUTE_SOUNDS_F;//消音开关
    MeterKeyNew[5].b.Key5  = ClearRelativeDistance_F;   
    
   if(((MultiPgckageErrorStatus[0]>>6)&0x03)==0x01)  
    MeterKeyNew[5].b.Key6  = 1;   //mil
   else  MeterKeyNew[5].b.Key6  = 0;   //mil
   if(((MultiPgckageErrorStatus[0]>>4)&0x03)==0x01)  
    MeterKeyNew[5].b.Key7  = 1;    //严重故障
   else  MeterKeyNew[5].b.Key7  = 0;  
   
   if(((MultiPgckageErrorStatus[0]>>2)&0x03)==0x01)  
    MeterKeyNew[6].b.Key0  = 1;     //轻微
   else  MeterKeyNew[6].b.Key0  = 0;  
   
  if(((MultiPgckageErrorStatus[1]>>6)&0x03)==0x01)   //mil
    MeterKeyNew[6].b.Key1  = 1;     
   else  MeterKeyNew[6].b.Key1  = 0; 
       
  if(((MultiPgckageErrorStatus[1]>>4)&0x03)==0x01)   //严重故障
    MeterKeyNew[6].b.Key2  = 1;     
   else  MeterKeyNew[6].b.Key2  = 0; 
    
  if(((MultiPgckageErrorStatus[1]>>2)&0x03)==0x01)   //轻微  
    MeterKeyNew[6].b.Key3  = 1;     
   else  MeterKeyNew[6].b.Key3  = 0;  
   
   
   if(((MultiPgckageErrorStatus[2]>>6)&0x03)==0x01)   //mil
    MeterKeyNew[6].b.Key4  = 1;     
   else  MeterKeyNew[6].b.Key4  = 0;  
    
   if(((MultiPgckageErrorStatus[2]>>4)&0x03)==0x01)   //严重故障 
    MeterKeyNew[6].b.Key5  = 1;     
   else  MeterKeyNew[6].b.Key5  = 0;  
 

    MeterKeyNew[6].b.Key6 = MaintainClear_F ;
   /*****************************实际acc状态******************/
   if(USR_s_OV.DashboardKey[0].b.Key6)
   MeterKeyNew[6].b.Key7 = 1;
   else 
   MeterKeyNew[6].b.Key7 = 0;
   
   
 
   
   if((MeterKeyOld[5].Data != MeterKeyNew[5].Data)) 
    {
      MeterKeyOld[5].Data = MeterKeyNew[5].Data;
    }
    else  USR_s_OV.DashboardKey[5].Data= MeterKeyNew[5].Data;
 
   if((MeterKeyOld[6].Data != MeterKeyNew[6].Data)) 
    {
      MeterKeyOld[6].Data = MeterKeyNew[6].Data;
    }
    else  USR_s_OV.DashboardKey[6].Data= MeterKeyNew[6].Data;    
    
   /****************ON档强制ACC******************************************/ 
    if(MeterKeyNew[1].b.Key1)  USR_s_OV.DashboardKey[0].b.Key6=1;
   
    
    
}
/******************************************************************************************
函数名：void lowOnlyKey(UINT8 *pAdcValue,UINT8 *pConfig,UINT8 *pBuf,UINT8 row,UINT8 line,UINT8 index)
功能：处理只能低 有效开关
******************************************************************************************/
void lowOnlyKey(UINT8 *pAdcValue,UINT8 *pBuf,UINT8 index)
{
    UINT8 row,line;
    row = index / 0x08;
    line = index % 0x08;
    //   10ma         10ma         50ma          50ma         120ma
    if((index==28)||(index==29)||(index==30)||(index==31)||(index==38)) 
    {
        if ((pAdcValue[index] < ADCCompareLow1Data)&&(pAdcValue[index] > 10))
        {
            //if (!((pConfig[row] >> line) & 0x01))
            //{  
            pBuf[row] |= 0x01 << line;
            //} 
            //else
            //{
            //    pBuf[row] &= ~(0x01 << line);
            //}
        }
        else
        {
            pBuf[row] &= ~(0x01 << line);
        }
    
    }
     else 
     {
        if ((pAdcValue[index] < ADCCompareLowData)&&(pAdcValue[index] > 10))
        {
            //if (!((pConfig[row] >> line) & 0x01))
            //{  
                pBuf[row] |= 0x01 << line;
            //} 
            //else
            //{
            //    pBuf[row] &= ~(0x01 << line);
            //}
        }
        else
        {
            pBuf[row] &= ~(0x01 << line);
        } 
     }
} 
/******************************************************************************************
函数名：void highonlykey(UINT8 *pAdcValue,UINT8 *pConfig,UINT8 *pBuf,UINT8 row,UINT8 line,UINT8 index)
功能：处理只能低 有效开关
******************************************************************************************/
void highonlykey(UINT8 *pAdcValue,UINT8 *pBuf,UINT8 index)
{
    UINT8 row,line;
    row = index / 0x08;
    line = index % 0x08;
    if((index==32)||(index==33))  //jc12 jc14
    {
        if (pAdcValue[index] > ADCCompareHigh1Data)
        {
             pBuf[row] |= 0x01 << line;
        }
        else
        {
            pBuf[row] &= ~(0x01 << line);
        }
    } 
    else 
    {   
      if (pAdcValue[index] > ADCCompareHighData)
      {
          //if (!((pConfig[row] >> line) & 0x01))
          //{  
              pBuf[row] |= 0x01 << line;
          //} 
          //else
          //{
          //    pBuf[row] &= ~(0x01 << line);
          //}
      }
      else
      {
          pBuf[row] &= ~(0x01 << line);
      }  
    }
} 
/******************************************************************************************
函数名：void lowHighKey(UINT8 *pAdcValue,UINT8 *pConfig,UINT8 *pBuf,UINT8 row,UINT8 line,UINT8 index)
功能：处理可高可低的开关
******************************************************************************************/
void lowHighKey(UINT8 *pAdcValue,UINT8 *pConfig,UINT8 *pBuf,UINT8 index)
{
    UINT8 row,line;
    row = index / 0x08;
    line = index % 0x08;
    if ((pAdcValue[index] > ADCCompareHighData)&&(pAdcValue[index]>ADCCompareLowData1))
    {
        if ((pConfig[row]>> line) &0x01)///the key is set to high side
        {
            pBuf[row] |= 0x01 << line;  
        }
        else
        {
            pBuf[row] &= ~(0x01 << line);
        }
    } 
    else if ((pAdcValue[index] < ADCCompareLowData)&&(pAdcValue[index]>ADCCompareLowData1))
    {
        if (!((pConfig[row] >> line) & 0x01))///the key is set to low side 
        {
            pBuf[row] |= 0x01 << line;  
        } 
        else
        {
            pBuf[row] &= ~(0x01 << line);
        }
    } 
    else
    {
        pBuf[row] &= ~(0x01 << line);
    }
} 



////////////////////////////////////////////////////////////////////////////////
/////////////												////////////////////
////////////					按钮					   /////////////////////
///////////												  //////////////////////
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
函数名：void initKeyPort(void)
功能：初始化按键端口
*******************************************************************************/
UINT8 have_key;
UINT8 wait_key;

void initKeyPort(void)
{
       have_key = 0;
       wait_key  = 0;
	   
	freeButtonvoltage = AD_Trans(BUTTON_ADC_MODULE,BUTTON_ADC_Ch);
	freeButtonvoltage = AD_Trans(BUTTON_ADC_MODULE,BUTTON_ADC_Ch);
	freeButtonvoltage = AD_Trans(BUTTON_ADC_MODULE,BUTTON_ADC_Ch);
}
/******************************************************************************
函数名：void scanKey1(void)
功能：扫描 KEY1
******************************************************************************/
void  ScanKey(UINT8 keyval)
{
  UINT8 i,key; 
  UINT32  mask;
  for(i=0;i<6;i++)
  {
     mask = 0x01<<i;
     if(keyval&mask)
     	{
              have_key &= (~mask);
		 wait_key &=  (~mask);
	 }
	 else
	 {
		 if(!(wait_key&mask))
		        {
		            wait_key |=  mask;
		        }
		        else
		        {
		            if(!(have_key&mask))
		            {
		                 have_key |= mask;
		                LcdMsg.flag |= mask;
                                if(i==4)
                                {
                                 MUTE_SOUNDS_F = 1;
                                 MUTE_SOUNDS_KEY = 1;
                                }
		            }
		        }

	 }
 
  }

}





unsigned char freeButtonvoltage = 0;
unsigned char getButtonNum(unsigned char adV){
	static unsigned char cnt,sw;
	freeButtonvoltage = 255;
	if(adV <= MaxSW1){
		
		if(sw == SW1)cnt++;
		else {cnt = 0;sw = SW1;}
		if(cnt>=WAITBOTTON) return ~0x20;
	}else if((adV >= MinSW2) && (adV <= MaxSW2)){
		
		if(sw == SW2)cnt++;
		else {cnt = 0;sw = SW2;}
		if(cnt>=WAITBOTTON) return ~0x10;
	}else if((adV >= MinSW3) && (adV <= MaxSW3)){
		
		if(sw == SW3)cnt++;
		else {cnt = 0;sw = SW3;}
		if(cnt>=WAITBOTTON)return ~0x08;
	}else if((adV >= MinSW4) && (adV <= MaxSW4)){
		
		if(sw == SW4)cnt++;
		else {cnt = 0;sw = SW4;}
		if(cnt>=WAITBOTTON)return ~0x04;
	}else if((adV >= MinSW5) && (adV <= MaxSW5)){
		
		if(sw == SW5)cnt++;
		else {cnt = 0;sw = SW5;}
		if(cnt>=WAITBOTTON)return ~0x02;
	}else if((adV >= MinSW6) && (adV <= MaxSW6)){
		
		if(sw == SW6)cnt++;
		else {cnt = 0;sw = SW6;}
		if(cnt>=WAITBOTTON)return ~0x01;
	}
        return 0xff;
}

/*****************************************************************************
功能：void scanKey(UINT8 oldKey)
功能：扫描按键
*****************************************************************************/


void scanKey(void)
{
   uint8 key;
    unsigned char adv = AD_Trans(BUTTON_ADC_MODULE,BUTTON_ADC_Ch);
    key = getButtonNum(adv);
    ScanKey(key);
   
    rGearKey();
    middleDoorKey();
    middleDoorOpenKey();
    frontDoorOpenKey();
    InterfaceSwitchKey();
}

/*******************************************************************************
函数名：void rGearKey(void)
功能：处理倒档视频按钮
*******************************************************************************/
void rGearKey(void)
{
    if(R_GEAR_VIDEO_LOAD != OLD_R_GEAR_VIDEO)
    {
        if(R_GEAR_VIDEO_LOAD)
        {
            LcdMsg.flag |= R_GEAR_KEY;
        }
        else
        {
            LcdMsg.flag |= R_GEAR_OVER_KEY; 
        }
        OLD_R_GEAR_VIDEO = R_GEAR_VIDEO_LOAD;
    }
}
/*******************************************************************************
函数名：void middleDoorKey(void)
功能：处理倒档视频按钮
*******************************************************************************/
void middleDoorKey(void)
{
    if(MIDDLE_VIDEO_LOAD != OLD_MIDDLE_VIDEO)
    {
        if(MIDDLE_VIDEO_LOAD)
        {
            LcdMsg.flag |= MIDDLE_DOOR_OPEN_KEY;
        }
        else
        {
            LcdMsg.flag |= MIDDLE_DOOR_OPEN_OVER_KEY; 
        }
        OLD_MIDDLE_VIDEO = MIDDLE_VIDEO_LOAD;
    }
    
}
/*******************************************************************************
函数名：void middleDoorOpenKey(void)
功能：处理中门未关按钮
#define MIDDLE_DOOR_NOT_CHOSE_KEY 0x0800
#define MIDDLE_DOOR_NOR_KEY       0x1000
#define FRONT_DOOR_NOT_CHOSE_KEY  0x2000
#define FRONT_DOOR_NOR_KEY        0x4000
*******************************************************************************/
void middleDoorOpenKey(void)
{
    if(MIDDLE_DOOR_OPENED_LOAD != OLD_MIDDLE_DOOR_OPENED_LOAD)
    {
        if(MIDDLE_DOOR_OPENED_LOAD)
        {
            LcdMsg.flag |= MIDDLE_DOOR_NOT_CHOSE_KEY;
        }
        else
        {
            LcdMsg.flag |= MIDDLE_DOOR_NOR_KEY; 
        }
        OLD_MIDDLE_DOOR_OPENED_LOAD = MIDDLE_DOOR_OPENED_LOAD;
    }
    
}
/*******************************************************************************
函数名：void frontDoorOpenKey(void)
功能：处理倒档视频按钮
*******************************************************************************/
void frontDoorOpenKey(void)
{
    if(FRONT_DOOR_OPENED_LOAD != OLD_FRONT_DOOR_OPENED_LOAD)
    {
        if(FRONT_DOOR_OPENED_LOAD)
        {
            LcdMsg.flag |= FRONT_DOOR_NOT_CHOSE_KEY;
        }
        else
        {
            LcdMsg.flag |= FRONT_DOOR_NOR_KEY; 
        }
        OLD_FRONT_DOOR_OPENED_LOAD = FRONT_DOOR_OPENED_LOAD;
    }
    
}
/**********************************************************************************
函数名：void frontDoorOpenKey(void)
功能：画面切换
***********************************************************************************/
void InterfaceSwitchKey(void) 
{
	if(INTERFACE_SWITCH_LOAD!=OLD_INTERFACE_SWITCH_LOAD) 
	{
		if(INTERFACE_SWITCH_LOAD)
		{
			LcdMsg.flag |= INTERFACE_SWITCH_KEY;
		}
		else
		{
			LcdMsg.flag |= INTERFACE_SWITCH_NOT_KEY; 
		}
		OLD_INTERFACE_SWITCH_LOAD = INTERFACE_SWITCH_LOAD;
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
///////////////////										////////////////////////
//////////////////			adc值采集				   /////////////////////////
/////////////////									  //////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define HC4051NUM	6


/*****************************************************
函数名：void AdcValueExtract(void)
功能：选择通道进行AD转换。
*****************************************************/
void AdcValueExtract(void)
{ 
	uchar i, j;

	UINT8 hc4051Channel[6] = {Mcu_U15,Mcu_U22,Mcu_U24,Mcu_U25,Mcu_U23,Mcu_U26}; 
	UINT8 adcNum[6] = {5,0,10,11,12,3};
	UINT8 adcMoudle[6] = {1,0,0,1,0,0};
	if(!SleepFlag)   //
	{  
		turnOnConsumePin();   //SleepFlag

	}
	else
	{
		closeConsumePin();
	}

    for (i=0;i<HC4051NUM;i++)
    {	
        for (j = 0; j < 8; j++)
        {	
            Hc4051_ChannelSelect(hc4051Channel[i],j); ///select hc4051 number and channel  
            AdcValue[8*i+j] = AD_Trans(adcMoudle[i],adcNum[i]);   
        }
    }

      //AN1_POW_L;      //采样电压传感器
      
	
	CheckVoltageValue[0] = AD_Trans(1,10);   //总12v开关 ap:这个有不一样
       CheckVoltageValue[1] = AD_Trans(0,2);    //摄像头12电源

       // if(AN_PowCnt==5)
       // {
          SensorAdcValue[0]    =  AD_Trans(0,8);  //JC15
	   SensorAdcValue[1] = AdcValue[36];        //JC09
	   SensorAdcValue[2] = AdcValue[37];        //JC11



	  if((Inside16BitAttrConfig[3].style==0x81)||(Inside16BitAttrConfig[4].style==0x81))
	  {
	   SensorAdcValue[8] = AdcValue[38]*2;        //JC13
	   SensorAdcValue[9] = AdcValue[39]*2;        //JC15
	   }
	  else
	  {
           SensorAdcValue[3] = AdcValue[38]*2;        //JC13
	    SensorAdcValue[4] = AdcValue[39]*2;        //JC15

	  }
	  
       // }
	 /*if(AN_PowCnt==19)
	 {
          SensorAdcValue[5]    =  AD_Trans(0,8);  //JC15
          SensorAdcValue[6] = AdcValue[36];        //JC09
	   SensorAdcValue[7] = AdcValue[37];        //JC11
	   SensorAdcValue[8] = AdcValue[38]*2;        //JC13
	   SensorAdcValue[9] = AdcValue[39]*2;        //JC15
	  

	 }*/

			

	  BoardTemperatureAdValue = AdcValue[31];
	
	//CheckVoltageValue[1] = AD_Trans(7);   //12v-1
	//CheckVoltageValue[2] = AD_Trans(6);   //12v-2
	//CheckVoltageValue[3] = AD_Trans(12);  //12v-3车速电源
	//BoardTemperatureAdValue = AD_Trans(0,5);
	//Bts724St[0] = AD_Trans(3); //BST724 ST脚检测
	//Bts724St[1] = AD_Trans(4); //BST724 ST脚检测 

}


/*********************************************************************************************** 
 函数名:void initHc4051(void)
 功能：初始化HC4051芯片
***********************************************************************************************/
void initHc4051(void)
{

	HC4051_DA0_MUX();  	
	HC4051_DA0_PDDR_O;
	HC4051_DB0_MUX();  	
	HC4051_DB0_PDDR_O;
	HC4051_DC0_MUX();  	
	HC4051_DC0_PDDR_O;

	HC4051_DA1_MUX();  	
	HC4051_DA1_PDDR_O;
	HC4051_DB1_MUX();  	
	HC4051_DB1_PDDR_O;
	HC4051_DC1_MUX();  	
	HC4051_DC1_PDDR_O;

	
}


void Hc4051_ChannelSelect(unsigned char Num_Hc4051,unsigned char Channel4051)
{


    if (Num_Hc4051==0)
    {
        switch (Channel4051)
        {
			case 0:HC4051_DA0_L;HC4051_DB0_L;HC4051_DC0_L;break;
			case 1:HC4051_DA0_H;HC4051_DB0_L;HC4051_DC0_L;break;
			case 2:HC4051_DA0_L;HC4051_DB0_H;HC4051_DC0_L;break;
			case 3:HC4051_DA0_H;HC4051_DB0_H;HC4051_DC0_L;break;
			case 4:HC4051_DA0_L;HC4051_DB0_L;HC4051_DC0_H;break;
			case 5:HC4051_DA0_H;HC4051_DB0_L;HC4051_DC0_H;break;
			case 6:HC4051_DA0_L;HC4051_DB0_H;HC4051_DC0_H;break;
			case 7:HC4051_DA0_H;HC4051_DB0_H;HC4051_DC0_H;break;
        	default:break;    
        } 
    } 
    else if (Num_Hc4051==1)
    {
        switch (Channel4051)
        {
			case 0:HC4051_DA1_L;HC4051_DB1_L;HC4051_DC1_L;break;
			case 1:HC4051_DA1_H;HC4051_DB1_L;HC4051_DC1_L;break;
			case 2:HC4051_DA1_L;HC4051_DB1_H;HC4051_DC1_L;break;
			case 3:HC4051_DA1_H;HC4051_DB1_H;HC4051_DC1_L;break;
			case 4:HC4051_DA1_L;HC4051_DB1_L;HC4051_DC1_H;break;
			case 5:HC4051_DA1_H;HC4051_DB1_L;HC4051_DC1_H;break;
			case 6:HC4051_DA1_L;HC4051_DB1_H;HC4051_DC1_H;break;
			case 7:HC4051_DA1_H;HC4051_DB1_H;HC4051_DC1_H;break;
        	default:break;    
        } 
    } 
   

}


/****************************************************************************************
函数名：void turnOnConsumePin(void)
功能：开启消耗控制引脚
****************************************************************************************/
void turnOnConsumePin(void)
{	

    //V_POWER_12V = 0;
	V_POWER_V1_H;
	V_POWER_V3_H;
	POWER_12V_ON;

	//if(AN_PowCnt==0)  //0~10~10
	//{
                 AN_POW_H;
	         
	//}
	//if(AN_PowCnt==9)
	//{
                
	         
	//}

     if((Inside16BitAttrConfig[3].style==0x81)||(Inside16BitAttrConfig[4].style==0x81))
     	{
           AN1_POW_L;
     	}
	 else
	 {
	    AN1_POW_H;
	 }
	
     //AN_PowCnt++;
     // if(AN_PowCnt>=20)
	//{
     //     AN_PowCnt = 0;
	//  }
}


/****************************************************************************************
函数名：void closeConsumePin(void)
功能：关闭消耗控制引脚
****************************************************************************************/
#include "xgate.h"
void closeConsumePin(void)
{

        V_POWER_V1_L;
	// V_POWER_V3 = 0;
	V_POWER_V3_L;//add by ap 20160301
	POWER_12V_OFF ;
	 init_motor_io();
        AN_POW_L;
	 AN1_POW_L;

}


