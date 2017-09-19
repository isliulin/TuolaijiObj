#define 	SOUND_C
#include	"sound.h"
//…˘“Ù±®æØø…∑÷Œ™“‘œ¬º∏¿‡£∫
//“ª°¢Õº±Í¿‡±®æØ
//∂˛°¢∏∫‘ÿø™¬∑∂Ã¬∑±®æØ
//¡ÌÕ‚,…˘“Ùªπ”–◊Û◊™°¢”“◊™°¢À´…¡¿‡…˘“Ù°£  –Èƒ‚ ‰≥ˆ¿‡±®æØ£¨±»»Á°¢√≈Œ¥πÿ±®æØ°£
//Œﬁƒ£øÈ±®æØ…˘“Ù°¢”Õ¡ø°¢ª˙”Õ—π¡¶±®æØ°¢≥µÀŸ±®æØ°¢◊™ÀŸ±®æØ°¢∆¯—π±®æØµ»µ»   Œﬁ∏ﬂCAN±®æØ


//…˘“Ù±®æØ÷ÆÕº±Í¿‡≈‰÷√£∫“«±Ì±Ì≈Ã…œLEDº∞LCD…œÕº±Í±®æØ°£
//”––©Õº±ÍŒ™±®æØ÷∏ æÕº±Í£¨”√¡Ω∏ˆŒª¿¥≈‰÷√“ª∏ˆÕº±Íœ‡”¶Œ™ƒƒ“ª÷÷±®æØ¿‡–Õ°£
//0b00: Œﬁ±®æØ
//0b01: “ªº∂≥§œÏ±®æØ
//0b10: ∂˛º∂¬ˆ≥Â±®æØ
//0b11: œ¬øÕ√≈±®æØ…˘
//«˝∂Ø ≤…”√œ¬Ωµ—ÿ≤…ºØ°£
/****************************************************************************************************
Õº±Í±®æØ£∫Œ™∞Â…œ16∏ˆLED÷∏ æµ∆ + ∞Â…œ8∏ˆLED÷∏ æµ∆ + 24∏ˆLCD÷∏ æµ∆ + 2∏ˆ ◊“≥±®æØ÷∏ æµ∆ = 50∏ˆ÷∏ æµ∆
”√“ª∏ˆ◊÷Ω⁄¿¥±Ì æ√ø∏ˆ÷∏ æµ∆±®æØ◊¥Ã¨°£
bit.1_0==11 -> ∏√±®æØŒ™…˘“Ù+Œƒ◊÷±®æØ
bit.1_0==01 -> ∏√±®æØŒ™…˘“Ù±®æØ
bit.1_0==10 -> ∏√±®æØŒ™Œƒ◊÷±®æØ
bit.1_0==00 -> Œﬁ»Œ∫Œ±®æØ
****************************************************************************************************/

//#pragma CONST_SEG    DEFAULT
//#pragma CODE_SEG  USER_ROM
UINT8 BackDriveLoadsSound[3];
UINT8 Gear2SondCnt;
UINT8 Gear3SondCnt;
UINT8 DashboardLoadsSound[2];
UINT8 OldDashboardLoadsSound;
UINT8 FrontDriveLoadsSound[3];
UINT8 OldFrontDriveLoadsSound[3];
UINT8 BackDriveLoadsSound[3];
UINT8 OldBackDriveLoadsSound[3];
UINT8 MiddleDriveLoadsSound[3];
UINT8 OldMiddleDriveLoadsSound[3];
UINT8 TopDriveLoadsSound[3];
UINT8 OldTopDriveLoadsSound[3];
UINT8 PowerLoadsSound[3];
UINT8 PowerLoadsFuse[3];

UINT8 PowerContorolLoadSound[3];
UINT8 OldPowerLoadsSound[3];
UINT8 J1939FaultSoundWarn;
UINT16 Cat4016DataOld;

UINT8  Gear1Warn[20];
UINT8  OldGear1Warn[20];
UINT8  ComGear1Warn[20];
UINT8  Gear2Warn[20];
UINT8  OldGear2Warn[20];
UINT8  ComGear2Warn[20];
UINT8  Gear3Warn[20];
UINT8  OldGear3Warn[20];
UINT8  ComGear3Warn[20];
UINT8  Gear4Warn[20];
UINT8  OldGear4Warn[20];
UINT8  ComGear4Warn[20]; 
UINT8  SensorFaultSound[6];
UINT8  OldSensorFaultSound[6];
UINT8  ComSensorFaultSound[6];
UINT8  ModuleErrorFlagWarn;
UINT8  OldModuleErrorFlagWarn;
UINT8  ComModuleErrorFlagWarn;

UINT8 IndicatorLed[20];
UINT8 IndicatorOldLed[20];



UINT8 Gear2SondCnt;
UINT8 Gear3SondCnt;
UINT8 Gear4SondCnt;
//UINT8 LcdWarm;
//UINT8 OldLcdWarm;

/***************************************************************************************************
∫Ø ˝√˚£∫void getIcoWarn(UINT8 baseFaultCode)
π¶ƒ‹£∫¥¶¿ÌÕº±Í±®æØ
≤Œ ˝Àµ√˜£∫UINT8 baseFaultCode -> π ’œ¬Îª˘÷µ
***************************************************************************************************/
void getIcoWarn(void)
{
	UINT8 i;
	UINT8 type;
	UINT8 row;
	UINT8 line;   
	for(i=0;i<19;i++)    IndicatorLed[i] = LoadLogicResult[i+1].Load;// lcd1~8

	for(i=0;i<160;i++)   //88 75 56       
	{
		if(IcoWarnConfig.DashboardIcoWarn[i]==0) continue;//add by L:…œŒªª˙≤π0
		row = i / 0x08;
		line = i % 0x08;
		type = IcoWarnConfig.DashboardIcoWarn[i] >>4;    
		if((type&0x08)==0)          //…˘“Ù±®æØ
		{
			type&=0x07;
			if(IndicatorLed[row] & (0x01 << line))
			{
				switch(type)
				{
					case 0x01: Gear1Warn[row] |= 0x01 << line;break;
					case 0x02: Gear2Warn[row] |= 0x01 << line;break;
					case 0x03: Gear3Warn[row] |= 0x01 << line;break;
					case 0x04: Gear4Warn[row] |= 0x01 << line;break;
					default:break;                        
				}
			}
			else
			{
				switch(type)
				{
					case 0x01: Gear1Warn[row] &= ~(0x01 << line);break;
					case 0x02: Gear2Warn[row] &= ~(0x01 << line);break;
					case 0x03: Gear3Warn[row] &= ~(0x01 << line);break;
					case 0x04: Gear4Warn[row] &= ~(0x01 << line);break;
					default:break;                              
				}                 
			}

		}
		
		type = IcoWarnConfig.DashboardIcoWarn[i]&0x0f;    //ÕºŒƒ◊÷±®æØ
		if((type&0x08)==0)          
		{
			LoadWarnCnt++;
			if((IndicatorLed[row] & (0x01 << line)) != (IndicatorOldLed[row] & (0x01 << line)))
			{
				if(IndicatorLed[row] & (0x01 << line))
				{  
					push_dis(LoadWarnCnt);
				}
				else
				{
					pop_dis(LoadWarnCnt);
				}           
			}
		}
	}
	
	for(i=0;i<20;i++)
	{
		IndicatorOldLed[i] = IndicatorLed[i];
	}
}


/************************************************************************************************
∫Ø ˝√˚£∫void sound(void)
π¶ƒ‹£∫◊‹ª„…˘“Ù±®æØ±‰¡ø
1°¢Õº±Í±®æØ…˘“Ù
2°¢∏∫‘ÿø™¬∑∂Ã¬∑±®æØ…˘“Ù
3°¢∂™∏ﬂCAN±®æØ
4°¢∂™ƒ£øÈ±®æØ
5°¢¥´∏–∆˜π ’œ±®æØ
************************************************************************************************/
void sound(void)
{
	loadOpenOrShortSoundWarn(&USR_s_OV.DashboardLoad[0].Data,&DashboardLoadsSound[0],4);       //“«±Ì∏∫‘ÿø™¬∑∂Ã¬∑…˘“Ù°£
	loadOpenOrShortSoundWarn(&USR_s_OV.FrontDriveLoad[0].Data,FrontDriveLoadsSound,20);  //«∞«˝∏∫‘ÿø™¬∑∂Ã¬∑…˘“Ù°£
	loadOpenOrShortSoundWarn(&USR_s_OV.BackDriveLoad[0].Data,BackDriveLoadsSound,20);    //∫Û«˝∏∫‘ÿø™¬∑∂Ã¬∑…˘“Ù°£
	//loadOpenOrShortSoundWarn(&USR_s_OV.MiddleDriveLoad[0].Data,MiddleDriveLoadsSound,18);//÷–«˝∏∫‘ÿø™¬∑∂Ã¬∑…˘“Ù°£
	loadOpenOrShortSoundWarn(&USR_s_OV.TopDriveLoad[0].Data,TopDriveLoadsSound,20);      //∂•«˝∏∫‘ÿø™¬∑∂Ã¬∑…˘“Ù°

	if(SysConfigData.selectPowerCtr==1) 
	{ 
		loadOpenOrShortSoundWarn(&USR_s_OV.PowerFuse[0].Data,PowerLoadsFuse,20);  
	}
	else
	{
		loadOpenOrShortSoundWarn(&USR_s_OV.PowerLoadState[0].Data,PowerLoadsSound,20);       //≈‰µÁ∏∫‘ÿø™¬∑∂Ã¬∑…˘“Ù°£
		loadOpenOrShortSoundWarn(&USR_s_OV.PowerFuse[0].Data,PowerLoadsFuse,17); 
	}
 
	loadOpenOrShortSoundWarn(&USR_s_OV.PowerContorolLoadState[0].Data,PowerContorolLoadSound,16);                                                                    
	sensorFaultSound();
}  
/**********************************************************************************************
∫Ø ˝√˚£∫void loadOpenOrShortSoundWarn(UINT8 *pLoadStatus,UINT8 *pSoundWarn,UINT8 len)
π¶ƒ‹£∫∞—∏∫‘ÿ±®æØ◊¥Ã¨◊™ªØŒ™…˘“Ù±®æØ°£
≤Œ ˝Àµ√˜£∫UINT8 *pLoadStatus -> ∏∫‘ÿ◊¥Ã¨
          UINT8 *pSoundWarn -> ∏∫‘ÿ…˘“Ù±®æØ÷∏ æ
          UINT8 len -> ∏∫‘ÿ◊¥Ã¨≥§∂»
∑µªÿ÷µ£∫UINT8 *pSoundWarn

* note by ap ,2016/3/25
* @brief:		
* @parameter	*pLoadStatus ∏∫‘ÿ◊¥Ã¨£¨¡ΩŒª±Ì æ
*						0x01 is short circuit,0x02 is open circuit
*				pSoundWarn  ‰≥ˆ¡ø°£“ªŒª±Ì æ
*				len ∏∫‘ÿ∏ˆ ˝
**********************************************************************************************/
void loadOpenOrShortSoundWarn(UINT8 *pLoadStatus,UINT8 *pSoundWarn,UINT8 len)
{
    UINT8 i;
    UINT8 srcRow,srcLine;
    UINT8 warnRow,warnLine;
    UINT8 temp;
	
    for(i=0;i<len;i++)
    {
        srcRow = i/0x04;
        srcLine = i%0x04;
        warnRow = i/0x08;
        warnLine = i%0x08;
        temp = pLoadStatus[srcRow] >> (srcLine * 0x02);
        temp &= 0x03;
        if(temp == LOAD_OPEN || temp == LOAD_SHORT)
        {
            pSoundWarn[warnRow] |= 0x01 << warnLine;
        }
        else
        {
            pSoundWarn[warnRow] &= ~(0x01 << warnLine);
        }
    }    
}
/************************************************************************************************
∫Ø ˝√˚£∫void sensorFaultSound(void)
π¶ƒ‹£∫¥¶¿Ì¥´∏–∆˜π ’œ…˘“Ù
************************************************************************************************/
void sensorFaultSound(void)
{
	UINT8 i;
	UINT8 row,line;
	UINT8 type;
	for(i=0;i<6;i++)     //“«±Ì    6
	{
		row = i / 0x08;
		line = i % 0x08;
		type = IcoWarnConfig.DashSensorIcoWarn[i] >>4; 
		if((type&0x08)==0)          //…˘“Ù±®æØ
		{
			if(InsideData[i] == 0xfffe)
			{
				SensorFaultSound[row] |= (0x01 << line);
			}
			else
			{
				SensorFaultSound[row] &= ~(0x01 << line);
			}  
		}

	}
	for(i=6;i<14;i++)   //«∞«˝
	{
		row = i / 0x08;
		line = i % 0x08;
		type = IcoWarnConfig.FrontSensorIcoWarn[i-6] >>4; 
		if((type&0x08)==0)          //…˘“Ù±®æØ
		{
			if(InsideData[FRONT_INSIDE_OFFSET+i-6] == 0xfffe)
			{
				SensorFaultSound[row] |= (0x01 << line);
			}
			else
			{
				SensorFaultSound[row] &= ~(0x01 << line);
			} 
		}
	}
	for(i=14;i<22;i++)   //∫Û«˝
	{
		row = i / 0x08;
		line = i % 0x08;
		type = IcoWarnConfig.BackSensorIcoWarn[i-14] >>4; 
		if((type&0x08)==0)          //…˘“Ù±®æØ
		{
			if(InsideData[BACK_INSIDE_OFFSET+i-14] == 0xfffe)
			{
				SensorFaultSound[row] |= (0x01 << line);
			}
			else
			{
				SensorFaultSound[row] &= ~(0x01 << line);
			} 
		}
	}
	for(i=22;i<30;i++)   //÷–
	{
		row = i / 0x08;
		line = i % 0x08;
		type = IcoWarnConfig.MiddleSensorIcoWarn[i-22] >>4; 
		if((type&0x08)==0)          //…˘“Ù±®æØ
		{
			if(InsideData[MIDDLE_INSIDE_OFFSET+i-22] == 0xfffe)
			{
				SensorFaultSound[row] |= (0x01 << line);
			}
			else
			{
				SensorFaultSound[row] &= ~(0x01 << line);
			} 
		}
	}

	for(i=30;i<38;i++)   //∂•«˝
	{
		row = i / 0x08;
		line = i % 0x08;
		type = IcoWarnConfig.TopSensorIcoWarn[i-30] >>4; 
		if((type&0x08)==0)          //…˘“Ù±®æØ
		{
			if(InsideData[TOP_INSIDE_OFFSET+i-30] == 0xfffe)
			{
				SensorFaultSound[row] |= (0x01 << line);
			}
			else
			{
				SensorFaultSound[row] &= ~(0x01 << line);//38¬∑
			}  
		}
	}
	for(i=38;i<41;i++) 
	{
		row = i / 0x08;
		line = i % 0x08;
		type = IcoWarnConfig.PowerContorolSensorIcoWarn[i-38] >>4; 
		if((type&0x08)==0)          //…˘“Ù±®æØ
		{
			if(InsideData[POWER_CONTOROL_OFFSET+i-38] == 0xfffe)
			{
				SensorFaultSound[row] |= (0x01 << line);
			}
			else
			{
				SensorFaultSound[row] &= ~(0x01 << line);//38¬∑
			}  
		}
	}
}

/**************************************************************************************************
∫Ø ˝√˚£∫void renewSound(void)
π¶ƒ‹£∫ª÷∏¥…˘“Ù
**************************************************************************************************/
void renewSound(void)
{
	UINT8 soundF;
	
	soundF = 0;
	FeedWatchDog();
	ModuleErrorFlagWarn  = ModuleErrorFlag.Byte;
	soundF |= sound_byte_compare(Gear1Warn,ComGear1Warn,sizeof(Gear1Warn)); 
	if(!soundF) 
	{
		soundF |= sound_byte_compare(Gear2Warn,ComGear2Warn,sizeof(Gear2Warn));
		if(!soundF) 
		{ 
			soundF |= sound_byte_compare(Gear3Warn,ComGear3Warn,sizeof(Gear3Warn));
			if(!soundF) 
			{  
				soundF |= sound_byte_compare(Gear4Warn,ComGear4Warn,sizeof(Gear4Warn)); 
				if(!soundF) 
				{ 
					soundF |= sound_byte_compare(SensorFaultSound,ComSensorFaultSound,sizeof(SensorFaultSound)); 
					if(!soundF) 
					{  
						soundF |= sound_byte_compare(&ModuleErrorFlagWarn,&ComModuleErrorFlagWarn,1);  
					}
				}
			}
		}
	}

	if(soundF) 
	{
		CopyDataSound(Gear1Warn,ComGear1Warn,sizeof(Gear1Warn)); 
		CopyDataSound(Gear2Warn,ComGear2Warn,sizeof(Gear2Warn)); 
		CopyDataSound(Gear3Warn,ComGear3Warn,sizeof(Gear3Warn)); 
		CopyDataSound(Gear4Warn,ComGear4Warn,sizeof(Gear4Warn)); 
		CopyDataSound(SensorFaultSound,ComSensorFaultSound,sizeof(SensorFaultSound)); 
		CopyDataSound(&ModuleErrorFlagWarn,&ComModuleErrorFlagWarn,1); 
		MUTE_SOUNDS_F = 0;
		GEAR_SOUND_OFF_F = 1;       
	}

	if(MUTE_SOUNDS_KEY) //ap æ≤“Ù
	{
		CopyDataSound(Gear1Warn,OldGear1Warn,sizeof(Gear1Warn)); 
		CopyDataSound(Gear2Warn,OldGear2Warn,sizeof(Gear2Warn)); 
		CopyDataSound(Gear3Warn,OldGear3Warn,sizeof(Gear3Warn)); 
		CopyDataSound(Gear4Warn,OldGear4Warn,sizeof(Gear4Warn)); 
		CopyDataSound(SensorFaultSound,OldSensorFaultSound,sizeof(SensorFaultSound)); 
		CopyDataSound(&ModuleErrorFlagWarn,&OldModuleErrorFlagWarn,1); 
		MUTE_SOUNDS_KEY = 0;
		MUTE_SOUNDS_F = 1;
	}   
}

/***************************************************************************************************
∫Ø ˝√˚£∫sound_byte_compare(UINT8 *newSt UINT8 *oldSt UINT8 len) 
π¶ƒ‹£∫¥¶¿ÌÕº±Í±®æØ
≤Œ ˝Àµ√˜£∫UINT8 baseFaultCode -> π ’œ¬Îª˘÷µ
* note by ap,2016/3/25
* @return		1 ∆Ù∂Ø
***************************************************************************************************/
UINT8 sound_byte_compare(UINT8 *newSt,UINT8 *oldSt,UINT8 len) 
{
    UINT8 i,j;
    UINT8 newTemp,oldTemp;
    UINT8 reSt;

	reSt = 0;  
    for(i=0;i<len;i++) 
	{
		if(newSt[i] == oldSt[i])  continue;
		for(j=0;j<8;j++) 
		{
			newTemp = newSt[i]&(1<<j);
			oldTemp = oldSt[i]&(1<<j);
			if(newTemp > oldTemp) //(newTemp==1,oldTemp==0
			{
				reSt = 1;  
			} 
			else if(oldTemp > newTemp) 
			{
				oldSt[i] &= (~(1<<j));
			}
		}  
	}
    return   reSt;
}

void  CopyDataSound(UINT8 *pScr,UINT8 *pDst,UINT8 cnt) 
{
    UINT8 i;
    for(i=0;i<cnt;i++)
    {
        pDst[i] = pScr[i];
    }
}  





/************************************************************************************************
∫Ø ˝√˚£∫UINT8 doLoadOpenShortSound(void)
************************************************************************************************/
UINT8 do1GearSound(void)
{
    BOOLEAN soundF=0;
    ModuleErrorFlagWarn  = ModuleErrorFlag.Byte;
    soundF |= sound_byte_compare(Gear1Warn,OldGear1Warn,sizeof(Gear1Warn)); 
    soundF |= sound_byte_compare(SensorFaultSound,OldSensorFaultSound,sizeof(SensorFaultSound)); 
    soundF |= sound_byte_compare(&ModuleErrorFlagWarn,&OldModuleErrorFlagWarn,1); 
    if(ACC_LOAD&&GEAR_SOUND_OFF_F)
    {  
        if(soundF && !MUTE_SOUNDS_F&& !GEAR_SOUND_LOAD)
        {
            SetBuzzer;
        }
        else
        {
            BuzzerOff;
        }
    }
    return (UINT8)(soundF);  
}
/************************************************************************************************************************
∫Ø ˝√˚£∫UINT8 do2GearSound(void)
π¶ƒ‹£∫¥¶¿Ì∂˛º∂…˘“Ù
************************************************************************************************************************/
UINT8 do2GearSound(void)
{
    BOOLEAN soundF=0; 
    soundF |= sound_byte_compare(Gear2Warn,OldGear2Warn,sizeof(Gear2Warn)); 
    if(soundF)
    {
       Gear2SondCnt++;
       if(Gear2SondCnt >= 50)       //500ms
       {
           Gear2SondCnt = 0x00;
       }
    }
    else
    {
        return 0;
    }
    if((soundF != OLD_2_GEAR_SOUND_F))
    {
        Gear2SondCnt = 0x00;
        OLD_2_GEAR_SOUND_F = soundF;
    }
    if(ACC_LOAD&&GEAR_SOUND_OFF_F)
    {  
        if(soundF)
        {
            if((Gear2SondCnt/25 == 0x00)&&!MUTE_SOUNDS_F&&!GEAR_SOUND_LOAD)
            {
              SetBuzzer; 
            }
            else
            {
               BuzzerOff;
            }
        }
        else
        {
              BuzzerOff;
        }
    }
    return (UINT8)soundF;
}
/************************************************************************************************************************
∫Ø ˝√˚£∫UINT8 do2GearSound(void)
π¶ƒ‹£∫¥¶¿Ì∂˛º∂…˘“Ù
************************************************************************************************************************/
UINT8 do3GearSound(void)
{
    BOOLEAN soundF=0;
    soundF |= sound_byte_compare(Gear3Warn,OldGear3Warn,sizeof(Gear3Warn)); 
    if(soundF)
    {
       Gear3SondCnt++;
       if(Gear3SondCnt >= 25)
       {
           Gear3SondCnt = 0x00;
       }
    }
    else
    {
        return 0;
    }
    if((soundF != OLD_3_GEAR_SOUND_F))
    {
        Gear3SondCnt = 0x00;
        OLD_3_GEAR_SOUND_F = soundF;
    }
    if(ACC_LOAD&&GEAR_SOUND_OFF_F)
    {  
        if(soundF)
        {
            if((Gear3SondCnt/12 == 0x00)&&!MUTE_SOUNDS_F)
            {
               SetBuzzer;
            }
            else
            {
               BuzzerOff;
            }
        }
        else
        {
              BuzzerOff;
        }
    }
    return (UINT8)soundF;    
}


/************************************************************************************************************************
∫Ø ˝√˚£∫UINT8 do2GearSound(void)
π¶ƒ‹£∫¥¶¿Ì∂˛º∂…˘“Ù
************************************************************************************************************************/
UINT8 do4GearSound(void)
{
    BOOLEAN soundF=0;
    soundF |= sound_byte_compare(Gear4Warn,OldGear4Warn,sizeof(Gear4Warn)); 
    if(soundF)
    {
       Gear4SondCnt++;
       if(Gear4SondCnt >= 15)     //250ms
       {
           Gear4SondCnt = 0x00;
       }
    }
    else
    {
        return 0;
    }
    if((soundF != OLD_4_GEAR_SOUND_F))
    {
        Gear4SondCnt = 0x00;
        OLD_4_GEAR_SOUND_F = soundF;
    }
    if(ACC_LOAD&&GEAR_SOUND_OFF_F)
    {  
        if(soundF)
        {
            if((Gear4SondCnt/7 == 0x00)&&!MUTE_SOUNDS_F)
            {
               SetBuzzer;
            }
            else
            {
               BuzzerOff;
            }
        }
        else
        {
              BuzzerOff;
        }
    }
    return (UINT8)soundF;    
}

void BuzzerOut(UINT8 mode,UINT8 on_flag)
{                
	switch(mode)
	{
		case 1: 
			if(BUZZER_FLAG) 
			{
				if(!SleepFlag)
					SetBuzzerDb;
				else
					BuzzerOffDb;
                                if(USR_s_DeviceConfigData.BuzzerLevel==1)  LEVEL3_H;
                                else  LEVEL3_L;
                                if(USR_s_DeviceConfigData.BuzzerLevel==2)  LEVEL2_H;
                                else  LEVEL2_L;
                                if(USR_s_DeviceConfigData.BuzzerLevel==3)  LEVEL1_H;
                                else  LEVEL1_L;       
			}
			else 
			{
                                LEVEL1_L;
                                LEVEL2_L;
                                LEVEL3_L;    
				BuzzerOffDb;
	
			}      
			break;
		case 4: 

                                LEVEL1_L;
                                LEVEL2_L;
                                LEVEL3_L;
			        BuzzerOffDb;
			break;

	}; 
}

