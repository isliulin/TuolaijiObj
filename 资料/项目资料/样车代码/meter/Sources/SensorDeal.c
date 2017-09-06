#define	SENSORDEAL_C
#include	"config.h"
#pragma CODE_SEG  USER_ROM
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void	SensorDealFunction(void)
{
	if(SleepFlag==1)
		return;
  dealWithAirPressure1();
  dealWithAirPressure2();
  dealWithOilPress();
  dealWithFuelLevel();
  dealWithWaterTemperture();
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void InputCaptureParameter_Initial(void)
{
	Speed_Pusle_Num=SPEED_PULSE;
	Speed_Rate_Num=SPEED_DATA;
	Rotation_Pulse_Num=ROTATION_PULSE;
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void dealWithAirPressure1(void) 
{
	#if			FRONT_AIR_SENSOR_IN==3
	uchar i;
	uchar GasPress1Num_Temp=0;
	if(GasPress1_ChangeFlag==1)				///0.5s extract 1 time
	{
		GasPress1_ChangeFlag=0;
		Time_Change_GasPress1=0;
		if(SensorADCData[FRONT_AIR_POSITION_IN]<SensorOpenNum)					
		{
	    for(i=0;i<100;i++)
	    {
        if((SensorADCData[FRONT_AIR_POSITION_IN]>=GasPressure_Table[i])&&(SensorADCData[FRONT_AIR_POSITION_IN]<=GasPressure_Table[i+1]))
        {
          GasPress1Num_Temp=i;	
          break;
        }		
	    }
	    if(SensorADCData[FRONT_AIR_POSITION_IN]>GasPressure_Table[100])///09.11.21add to resolve"sensor parameter out of area"
	    {
        GasPress1Num_Temp=100;
	    }
		}
		else
		{
	    GasPress1Num_Temp=SensorErrorNum; 
		}
		Count_GasPress1_Num++;
		if(Count_GasPress1_Num==1)
		{
		  GasPress1_Num[0]=GasPress1Num_Temp;
		} 
		else if(Count_GasPress1_Num==2)
		{
	    GasPress1_Num[1]=GasPress1Num_Temp;
	    Count_GasPress1_Num=0;
		  if(GasPress1_Num[0]==GasPress1_Num[1])
		  {  
        PannalSensorData[FRONT_AIR_POSITION_IN]=GasPress1_Num[1];  
				MeterSensorData[1]=PannalSensorData[FRONT_AIR_POSITION_IN];
		  }
		}
		else
		{
		  Count_GasPress1_Num=0;
		}
	}
	#else
		MeterSensorData[1]=0xff;
	#endif
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void dealWithAirPressure2(void)
{
	#if			BACK_AIR_SENSOR_IN==3
	uchar i;
	uchar GasPress2Num_Temp=0;
	if(GasPress2_ChangeFlag==1)						
	{
	  GasPress2_ChangeFlag=0;
	  Time_Change_GasPress2=0;
	  if(SensorADCData[BACK_AIR_POSITION_IN]<SensorOpenNum)					
	  {
      for(i=0;i<100;i++)
      {
        if((SensorADCData[BACK_AIR_POSITION_IN]>=GasPressure_Table[i])&&(SensorADCData[BACK_AIR_POSITION_IN]<=GasPressure_Table[i+1]))
       	{
          GasPress2Num_Temp=i;		
          break;
        }	
      }			
      if(SensorADCData[BACK_AIR_POSITION_IN]>GasPressure_Table[100])///09.11.21add to resolve"sensor parameter out of area"
      {
        GasPress2Num_Temp=100;
      }
	  }
	  else
	  {
      GasPress2Num_Temp=SensorErrorNum; 
	  }
	  Count_GasPress2_Num++;
	  if(Count_GasPress2_Num==1)
	  {
	    GasPress2_Num[0]=GasPress2Num_Temp;
	  } 
	  else if(Count_GasPress2_Num==2)
	  {
      GasPress2_Num[1]=GasPress2Num_Temp;
      Count_GasPress2_Num=0;
		  if(GasPress2_Num[0]==GasPress2_Num[1])
		  {  
        PannalSensorData[BACK_AIR_POSITION_IN]=GasPress2_Num[1];
				MeterSensorData[2]=PannalSensorData[BACK_AIR_POSITION_IN];
		  }
	  } 
	  else
	  {
		  Count_GasPress2_Num=0;  
	  }
	} 
	#else
		MeterSensorData[2]=0xff;
	#endif
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void dealWithFuelLevel(void)
{
	#if			FUAL_LEVEL_SENSOR_IN==3
  uchar i;
  uchar FuelLevelNum_Temp=0;
	
	if(FuelLevelFlag==1)	///当关闭到OFF档时0.5S采集一次，当开机后15S采集一次
	{
	  FuelLevelFlag=0;
	  Time_Change_FuelLevel=0;
	  if(SensorADCData[FUAL_LEVEL_POSITION_IN]<=SensorOpenNum)
	  {
      for(i=0;i<46;i++)
      {
        if((SensorADCData[FUAL_LEVEL_POSITION_IN]>=FuelLevel_Table[i])&&(SensorADCData[FUAL_LEVEL_POSITION_IN]<=FuelLevel_Table[i+1]))
        {
          FuelLevelNum_Temp=i;	
          break;
        }		
      }		
      if(SensorADCData[FUAL_LEVEL_POSITION_IN]>FuelLevel_Table[45])
      {  
	      FuelLevelNum_Temp=45;            
      }  
	  }
	  else
	  {
      FuelLevelNum_Temp = SensorErrorNum;
	  }
	  Count_FuelLevel_Num++;
	  if(Count_FuelLevel_Num==1)
	  {
	    FuelLevel_Num[0]=FuelLevelNum_Temp;
	  }	
	  else if(Count_FuelLevel_Num==2)
	  {
      FuelLevel_Num[1]=FuelLevelNum_Temp;
      Count_FuelLevel_Num=0;
      if(FuelLevel_Num[0]==FuelLevel_Num[1])
      {
        PannalSensorData[FUAL_LEVEL_POSITION_IN]=FuelLevel_Num[1];
				MeterSensorData[0]=PannalSensorData[FUAL_LEVEL_POSITION_IN];
      }
	  }
	  else
	  {
	    Count_FuelLevel_Num=0; 
	  }    
	}
	#else
		MeterSensorData[0]=0xff;
	#endif
} 
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void dealWithOilPress(void)
{
	#if			OIL_PRESS_SENSOR_IN==3
	uchar i;
	uchar	OilPressNum_Temp=0;
	if(oil_j1939_flag==0)
	{
	  if(OilPressFlag==1)							
		{
		  OilPressFlag=0;
		  Time_Change_OilPress=0;
		  if(SensorADCData[OIL_PRESS_POSITION_IN]<=SensorOpenNum)
		  {
	      for(i=0;i<100;i++)
	      {
          if((SensorADCData[OIL_PRESS_POSITION_IN]>=OilPressure_Table[i])&&(SensorADCData[OIL_PRESS_POSITION_IN]<=OilPressure_Table[i+1]))
          {
            OilPressNum_Temp=i;	
            break;
          }		
	      }	
	      if(SensorADCData[OIL_PRESS_POSITION_IN]>OilPressure_Table[100])	///09.11.21add to resolve"sensor parameter out of area"
	      {
	        OilPressNum_Temp=100;	
	      }
		  } 
		  else					
		  {
	      OilPressNum_Temp=SensorErrorNum;		
		  }
		  Count_OilPress_Num++;
			if(Count_OilPress_Num==1)
			{
				OilPress_Num[0]=OilPressNum_Temp;
			} 
			else if(Count_OilPress_Num==2)
			{
				OilPress_Num[1]=OilPressNum_Temp;
				Count_OilPress_Num=0;
				if(OilPress_Num[0]==OilPress_Num[1])
				{  
			    PannalSensorData[OIL_PRESS_POSITION_IN]=OilPress_Num[1];
					MeterSensorData[3]=PannalSensorData[OIL_PRESS_POSITION_IN];
				}
			} 
			else
			{
			  Count_OilPress_Num=0;    
			}
		}
	} 
	#else
		MeterSensorData[3]=0xff;
	#endif
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void dealWithWaterTemperture(void)
{
	#if			WATER_TEMPERATURE_SENSOR_IN==3
	uchar	i;
	uchar	TeperatureNum_Temp=0;
	if(WaterTemp_J1939_Flag==0)										///if not j1939 signal
	{
	  if(WaterTemperature_2sFlag==1)							
		{
		  WaterTemperature_2sFlag=0;
		  Time_Change_WaterTemp=0;
		  if(SensorADCData[WATER_TEMPERATURE_POSITION_IN]<=SensorOpenNum)
		  {
		    for(i=81;i>0;i--)
		    {
		      if((SensorADCData[WATER_TEMPERATURE_POSITION_IN]>=WaterTemp_Table[i])&&(SensorADCData[WATER_TEMPERATURE_POSITION_IN]<=WaterTemp_Table[i-1]))
		      {
	          TeperatureNum_Temp=i;	
	          break;
		      }		
		    }				    
		  }   
		  else
		  {
		    TeperatureNum_Temp=SensorErrorNum;
		  }
		  Count_WaterTemp_Num++;
		  if(Count_WaterTemp_Num==1)
		  {
		    WaterTemp_Num[0]=TeperatureNum_Temp;
		  } 
		  else if(Count_WaterTemp_Num==2)
		  {
		    WaterTemp_Num[1]=TeperatureNum_Temp;
		    Count_WaterTemp_Num=0;
			  if(WaterTemp_Num[0]==WaterTemp_Num[1])
			  {  
	        PannalSensorData[WATER_TEMPERATURE_POSITION_IN]=WaterTemp_Num[1]; 
					MeterSensorData[4]=PannalSensorData[WATER_TEMPERATURE_POSITION_IN];
			  }
		  } 
		  else
		  {
		    Count_WaterTemp_Num=0;   
		  }
		}
	}   
	#else
		MeterSensorData[4]=0xff;
	#endif
}
#pragma CODE_SEG DEFAULT 

