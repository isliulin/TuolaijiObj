#define 	SENSOR_C
#include	"sensor.h"
#include "BacklightControl.h"


UINT16 InsideData[END_INSIDE_OFFSET1];
UINT16 OldInsideData[10];
UINT16 OldSensorData[INSIDE_16BIT_DATA_NUM];
UINT16 BatteryDataRoundBuf[BATTERY_DATA_NUM];
UINT8  BatteryDataIndex;
UINT16 BatteryValtage;

UINT8  SensorAdcValue[10];
UINT8  PwmAdcValue[2];
UINT16 RealCarSpeed;
UINT16 DashBoardTempRoundBuf[BATTERY_DATA_NUM];
UINT8  DashBoardTempIndex;
UINT16 DashBoardTemp;



/**************************************************************************************
函数名：void doSensor(void)
功能：计算各传感器的物理量

**************************************************************************************/
//#define  SENSOR_OFFSET_BYTE   2
void doSensor(void)
{
	UINT8 CarSpeedSrcTmep,EngCarSpeedSrcTemp;
	UINT8 i;
	UINT8 AdcValueTemp;
	UINT8 portIndex;

	 
	for(i=0;i<SENSOR_DASHBOARD_NUM;i++) 
	{
		if(Inside16BitAttrConfig[i].style & SENSOR_VALID) 
		{ 
			portIndex = Inside16BitAttrConfig[i].portIndex;
			if(portIndex < SENSOR_DASHBOARD_NUM) 
			{


			if(Inside16BitAttrConfig[i].style==0x80)  AdcValueTemp = SensorAdcValue[i];
			if(Inside16BitAttrConfig[i].style==0x81) AdcValueTemp = SensorAdcValue[i+5];
				//AdcValueTemp = SensorAdcValue[portIndex];
				InsideData[i] = adTophysical(AdcValueTemp,&Inside16BitAttrConfig[i]);//仪表传感器 
			} 
			if((InsideData[i]!=OldInsideData[i]) && (InsideData[i]!=0xfffe)) 
			{
				if(InsideData[i]!=OldInsideData[i]) 
				{
					InsideData[i] = (InsideData[i]+OldInsideData[i])/2+1;;
					OldInsideData[i] = InsideData[i] ;   
				} 
				else
				{
					OldInsideData[i] = InsideData[i] ;  
				}
			} 
		} 
	}
	
	if(SENSOR_1S_F)  // 请求包序列号
	{
		SENSOR_1S_F = 0;
		InsideData[9]++;
		if(InsideData[9]>22)  InsideData[9] = 1;//1~22
		J1939RequestIndex =  InsideData[9];
	}

	//仪表板温
	InsideData[5] = doDashBoardTemp(BoardTemperatureAdValue);           //仪表板温
	if(InsideData[5]!=OldInsideData[5]) 
	{
		InsideData[5] = (InsideData[5]+OldInsideData[5])/2+1;;
		OldInsideData[5] = InsideData[5] ;   
	} 
	else
	{
		OldInsideData[5] = InsideData[5] ;  
	}                             

	//电池电压
	InsideData[6] = calcVolatage(REFERENCE_VOLTAGE) + 7; //电池电压    REFERENCE_VOLTAGE
	if(InsideData[6]!=OldInsideData[6]) 
	{
		InsideData[6] = (InsideData[6]+OldInsideData[6])/2+1;;
		OldInsideData[6] = InsideData[6] ;   
	} 
	else
	{
		OldInsideData[6] = InsideData[6] ;  
	}
	
	//车速
	CarSpeedSrcTmep = SysConfigData.CarSpeedSrc;
	RealCarSpeed = getCarSpeedChannel(CarSpeedSrcTmep);  //CarSpeedSrc               //车速 
	InsideData[7] = RealCarSpeed*100;
	InsideData[7] /= 98;

	//电机转速
	EngCarSpeedSrcTemp =  SysConfigData.EngCarSpeedSrc;
	InsideData[8] = getCarEngSpeedChannel(EngCarSpeedSrcTemp);//USR_s_OV.engSpeedFromDriver; 
	//光敏
	if(GAIN_OPT_Flag){
		GAIN_OPT_Flag = 0;
		InsideData[10] =  getValueOPTO();
	}
	
	if(CarSpeedSrcTmep == 2) 
	{   
		InsideData[18] = USR_s_OV.speed_Rate;
		InsideData[19] = USR_s_OV.cycle_capture_num;
	} 
	else if(CarSpeedSrcTmep == 1)
	{
		InsideData[18] =   SysConfigData.SpeedRate;
		InsideData[19]=   SysConfigData.CycleCaptureNum;
	}

	InsideData[13] =  ModuleErrorOldFlag.Byte;    //模块丢失

	InsideData[14] = J1939CompareData(SysConfigData.J1939CompareType0,SysConfigData.J1939CompareIndex0,SysConfigData.J1939Comparelen0);
	InsideData[15] = J1939CompareData(SysConfigData.J1939CompareType1,SysConfigData.J1939CompareIndex1,SysConfigData.J1939Comparelen1);
	InsideData[16] = J1939CompareData(SysConfigData.J1939CompareType2,SysConfigData.J1939CompareIndex2,SysConfigData.J1939Comparelen2);
	InsideData[17] = J1939CompareData(SysConfigData.J1939CompareType3,SysConfigData.J1939CompareIndex3,SysConfigData.J1939Comparelen3);

}

/*****************************************************************************************
函数名：

*****************************************************************************************
函数名：void adTophysical(UINT8 adChannel,UINT8 segment,UINT16 rang)
功能：把AD值转化为物理量
参数说明： UINT8 adChannel -> AD通道，决定数据来源，可以配置通道
           UINT16 rang -> 物理量的量程，例如油量为100，气压为100，水温为120，等等注意，有时候需要进行范围缩减 
=============================================================================================================
公式推算：电压与物理量成正比的公式推导 
         5/255 = U/AD  => AD = 51U
           5R1               5R2
        (--------    y1)  (--------  , y2)  两点确定的直线方程为：
          R1+135   ,        R2+135
          
          5r         5R1
       --------- - --------          
        r + 135     R1+135           y-y1
   ----------------------------- = --------
          5R2        5R1             y2-y1
       --------- - --------
        R2 + 135    R1+135
        
    左边分子分母同时乘以51，把电压转换为AD值。化简可得
          ad - AD1       y - y1
       -------------- = --------
         AD2 - AD1       y2- y1
         
              (y2-y1)*(ad-AD1)
         y = ------------------ + y1    (1)   电压与物理量成正比公式
                 AD2 - AD1                  
=============================================================================================================
公式推导：电阻与物理量成正比的公式推导
          (R1,y1) , (R2,y2)两点所确定的直线方程为：
          y - y1     r - R1
         -------- = -------- 
          y2- y1     R2- R1
             
            5r       ad             135 * 255
         -------- = ----   => r = ------------- - 135
          r + 135    51             255 - ad
          
               (r - R1)(y2 - y1)
          y = -------------------  + y1
                    R2 - R1
                    
                 135 * 255
              ((----------- - 135) - R1)(y2-y1)
                 255 - ad
         y =  ---------------------------------  + y1
                       R2 - R1
原理：在调用时，adValue用AD通道的方式进行调用，即可实现通道配置。
*****************************************************************************************/
UINT16 adTophysical(UINT8 adValue,const INSIDE_16BIT_ATTR_STRUCT * pSegment)
{
    UINT8 dotNum;
    UINT8 i;
    UINT16 res;
    UINT16 result;
    //UINT16 temp;
    
    dotNum = pSegment->dotNum;
    if(!dotNum)            //该路悬空
    {
        return 0x00;
    }
    else
	{  
		switch(pSegment->style) 
		{
			case 0x80:    
				if(adValue >= 195)		
					return 0xfffe; //该路开路 // 210
				res = adToRes(pSegment->inputRatio,adValue); break;
			case 0x81:    
				if(adValue <= 6)		
					return 0xfffe; //该路开路 // 210
				res = adToVoltage(pSegment->inputRatio,adValue); break;
			default:
				break;
		}   

		 if(res <= pSegment->sensorPrm[0])  return pSegment->sensorPrm[1];
		switch(dotNum) 
		{
			case 2: if(res >= pSegment->sensorPrm[2])  return pSegment->sensorPrm[3]; break;
			case 3: if(res >= pSegment->sensorPrm[4])  return pSegment->sensorPrm[5]; break;
			case 4: if(res >= pSegment->sensorPrm[6])  return pSegment->sensorPrm[7]; break;
			case 5: if(res >= pSegment->sensorPrm[8])  return pSegment->sensorPrm[9]; break;
			case 6: if(res >= pSegment->sensorPrm[10]) return pSegment->sensorPrm[11];break;
			default:
				break;
		} 
		for(i=0;i<dotNum-1;i++)  //取段dotNum-1
		{
			if((res >= pSegment->sensorPrm[2*i]) && (res <= pSegment->sensorPrm[2*i+2]))
			{
				result =  calcPhysicalDirectRatioRes(pSegment->sensorPrm[2*i],pSegment->sensorPrm[2*i+1],pSegment->sensorPrm[2*i+2],pSegment->sensorPrm[2*i+3],res);
				return result;
			}
		}
	}   
    return 0;  
}
/**********************************************************************************
函数名：UINT16 calcDashBoardTemp(UINT16 dashTempAdValue)
功能：计算仪表温度
参数说明：UINT16 dashTempAdValue -> 仪表温度的AD转换值

**********************************************************************************/
UINT16 doDashBoardTemp(UINT16 dashTempAdValue)
{
    UINT16 temperature;
    temperature = temperatureScaleChange(dashTempAdValue);
    DashBoardTempIndex = saveUINT16DataToRoundBuf(DashBoardTempRoundBuf,temperature,DashBoardTempIndex,DASHBOARD_TEMP_DATA_NUM);
    return averageUINT16DataWithoutMaxAndMin(DashBoardTempRoundBuf,DASHBOARD_TEMP_DATA_NUM);    
}
/**********************************************************************************
函数名：UINT16 temperatureScaleChange(UINT16 temperatureAdValue)
功能：温度标度转换
参数说明：UINT16 temperatureAdValue -> 温度AD转换值
返回值：温度
计算公式：t=100x-50+40;
          x=(AD_value)/256*5
          t:实际温度值
**********************************************************************************/
UINT16 temperatureScaleChange(UINT16 temperatureAdValue)
{
    return (((500 * temperatureAdValue) >> 0x08) - 10);    
}
/******************************************************************************************
函数名：void saveUINT16DataToRoundBuf(UINT16 *pRoundBuf,UINT16 data,UINT8 index,UINT8 size)
功能：保存UINT16数据到环形缓冲区
参数说明：UINT16 *pRoundBuf -> 环形缓冲区、
          UINT8 index -> 指向环形缓冲区内当前可存位置。
          UINT8 size -> 环形缓冲区大小
          UINT16 data -> 欲存入环形缓冲区内的数据
返回值：无
******************************************************************************************/
UINT8 saveUINT16DataToRoundBuf(UINT16 *pRoundBuf,UINT16 data,UINT8 index,UINT8 size)
{
    pRoundBuf[index] = data;
    if(index >= size-1)
    {
        index = 0x00;
    }
    else
    {
        index++;
    }
    return index;
}

/*****************************************************************************************
函数名：UINT16 averageUINT16DataWithoutMaxAndMin(UINT16 *pRoundBuf,UINT8 size)
功能：去掉最大值和最小值，求缓冲区内数据的平均值。
参数说明：UINT16 *pRoundBuf -> 环形缓冲区
          UINT8 size -> 环形缓冲区大小
返回值:所得平均值
*****************************************************************************************/
UINT16 averageUINT16DataWithoutMaxAndMin(UINT16 *pRoundBuf,UINT8 size)
{
    UINT16 max = pRoundBuf[0];
    UINT16 min = pRoundBuf[0];
    UINT16 sum = pRoundBuf[0];
    UINT8 i;
    for(i=1;i<size;i++)
    {
        if(pRoundBuf[i] > max)
        {
            max = pRoundBuf[i];
        }
        if(pRoundBuf[i] < min)
        {
            min = pRoundBuf[i];
        }
        sum += pRoundBuf[i];
    }
    return ((sum - max - min) / (size - 2));
}

/*****************************************************************************************
函数名：void calcVolatage(UINT16 volatageAdValue)
功能：计算电池电压
参数说明：UINT16 volatageAdValue -> 电池电压AD转换值
*****************************************************************************************/
UINT16 calcVolatage(UINT16 volatageAdValue)
{    
    //UINT16 volatageValue = REFERENCE_VOLTAGE;
    UINT16 volatage;
    volatage = volatageAdValue * 90/50;   //
    BatteryDataIndex = saveUINT16DataToRoundBuf(BatteryDataRoundBuf,volatage,BatteryDataIndex,BATTERY_DATA_NUM);
    return averageUINT16DataWithoutMaxAndMin(BatteryDataRoundBuf,BATTERY_DATA_NUM);    
}

/***************************************************************************************
函数名：UINT8 resToAd(UINT16 res)
功能：把电阻值转化为ＡＤ值
参数说明：UINT16 res 已知上拉电阻 

返回值：输出外接电阻
***************************************************************************************/
UINT16 adToRes(UINT16 res,UINT16 ad)                      
{     
    dword temp; 
    temp = res;
    temp *= ad;
    temp+=(127-ad/2);             //四舍五入
    temp /=(242-ad);                                           
    return (UINT16)temp;     
}
/***************************************************************************************
函数名：UINT8 resToAd(UINT16 res)
功能：把电阻值转化为ＡＤ值
参数说明：UINT16 res -> 电阻
返回值：AD采样值
***************************************************************************************/

UINT16 adToVoltage(UINT16 Ratio,UINT16 ad)
{
    dword vtemp;
    //vtemp  = ((dword)ad*1000)/51;  //四舍五入 MV       
    //vtemp *= (Ratio&0xff);
    //vtemp /= ((Ratio>>8)&0xff);
     vtemp  = ad;  //四舍五入 MV       
     vtemp *= Ratio;
     vtemp /= 480;
    return (UINT16)vtemp;
}

/****************************************************************************************
函数名：UINT16 calcPhysicalDirectRatioRes(UINT8 r1,UINT8 y1,UINT8 r2,UINT8 y2,UINT8 ad)
功能：求电阻与物理量成正比的物理量值（电阻型传感器）
参数说明：(UINT8 r1,UINT8 y1) -> 直线段的一个端点坐标
          (UINT8 r1,UINT8 y1) -> 直线段的另一个端点坐标
          UINT8 ad -> AD采样值
返回值：物理量值
计算公式：
                 135 * 255
              ((----------- - 135) - R1)(y2-y1)
                 255 - ad
         y =  --------------------------------- + y1
         
                       R2 - R1
  ///////////////////////////////////////////////////////////

                 ( Y2 - Y1) *(Xn - X1)
        Yn =  ---------------------------- + Y1     (Y2>Y1)&&(X1<Xn<X2)
                     X2  -  X1
                 ( Y1 - Y2) *(Xn - X1)
        Yn = Y1 - ----------------------------     (Y2<Y1)&&(X1<Xn<X2)
                     X2  -  X1
****************************************************************************************/
UINT16 calcPhysicalDirectRatioRes(UINT16 r1,UINT16 y1,UINT16 r2,UINT16 y2,UINT16 ad)
{

    UINT16 result;
    if(y2>y1) 
    {  
      result = (ad-r1);
      result *= (UINT16)(y2-y1);
      result += ((r2-r1)/2);      //四舍五入
      result /= (UINT16)(r2-r1);
      result += y1;
    } 
    else 
    {
      result = (ad-r1);
      result *= (UINT16)(y1-y2);
      result += ((r2-r1)/2);     //四舍五入
      result /= (UINT16)(r2-r1);
      result = y1 - result;
    }
       return result;
}
/*********************************************************************************************
函数名：void getVirtualKey(void)
*********************************************************************************************/
//UINT16  Schmittbuffer[10];
void getInsideVirtualKey(SelfKeyStruct *virtualKey)
{
    
    UINT8 i;
    UINT8 type;
    UINT8 row;
    UINT8 line;  
    UINT16 data;
    UINT8  index;

    for(i=0;i<SENSOR_VIRTUAL_KEY_NUM;i++)
    {
        if(SENSOR_VIRTUAL_KEY_CONFIG[i].attribute & SENSOR_VIRTUAL_KEY)
        {
            index=  SENSOR_VIRTUAL_KEY_CONFIG[i].portIndex;
            row  =  SENSOR_VIRTUAL_KEY_CONFIG[i].KeyIndex / 0x08;
            line =  SENSOR_VIRTUAL_KEY_CONFIG[i].KeyIndex % 0x08;
  
                type =  SENSOR_VIRTUAL_KEY_CONFIG[i].attribute  & SENSOR_WARM_MASK;
                data = InsideData[index];
                switch(type)
                {
                    case SENSOR_LOWER_WARM:
                    {
                        if(data <= SENSOR_VIRTUAL_KEY_CONFIG[i].thresholdL)
                        {
                          virtualKey[row].Data |= (0x01 << line);
                        }
                        else
                        {
                           virtualKey[row].Data &= ~(0x01 << line);
                        }
                        break;
                    }
                    case SENSOR_BILATERAL_WARM:
                    {
                        if((data <= SENSOR_VIRTUAL_KEY_CONFIG[i].thresholdL) || (data >= SENSOR_VIRTUAL_KEY_CONFIG[i].thresholdH))
                        {
                           virtualKey[row].Data |= (0x01 << line);
                        }
                        else
                        {
                           virtualKey[row].Data &= ~(0x01 << line);
                        }
                        break;                
                    }
                    case SENSOR_MIDDLE_WARM:
                    {
                        if((data >= SENSOR_VIRTUAL_KEY_CONFIG[i].thresholdL) && (data <= SENSOR_VIRTUAL_KEY_CONFIG[i].thresholdH))
                        {
                            virtualKey[row].Data |= (0x01 << line);
                        }
                        else
                        {
                            virtualKey[row].Data &= ~(0x01 << line);
                        }
                        break;                     
                    }
                  	case SENSOR_BIGGER_WARM:
                    {
                        if(data >= SENSOR_VIRTUAL_KEY_CONFIG[i].thresholdH)
                        {
                            virtualKey[row].Data |= (0x01 << line);
                          
                        }
                        else
                        {
                            virtualKey[row].Data &= ~(0x01 << line);
                          
                        }
                        break;                    
                    }                                                               	//      ________
                  	case SCHMITT_TRIGGER_HIGH:                        // 0.6-0.8  关闭     __|___|
                      //  keyv =  (virtualKey[row].Data >>line)&0x01;  // 0.8-0.6  开启    
                        if(data<SENSOR_VIRTUAL_KEY_CONFIG[i].thresholdL) 
                        { 
                          virtualKey[row].Data &= ~(0x01 << line);
                        } 
                        else if(data>SENSOR_VIRTUAL_KEY_CONFIG[i].thresholdH) 
                        {
                          virtualKey[row].Data |= (0x01 << line);
                        }
                        else 
                         {
                          ;
                         }
                      
                                                                      //     _________
                  		break;                                             //        |___|_____
                  case SCHMITT_TRIGGER_LOW:                                
                        // keyv =  (virtualKey[row].Data >>line)&0x01;  // 0.8-0.6  开启
                        if(data<SENSOR_VIRTUAL_KEY_CONFIG[i].thresholdL) 
                        { 
                          virtualKey[row].Data |= (0x01 << line);
                        }
                        else if(data>SENSOR_VIRTUAL_KEY_CONFIG[i].thresholdH) 
                        {
                          virtualKey[row].Data &= ~(0x01 << line);
                        } 
                       {
                          ;
                        }
                     
                  break;
                      default:  break;                    
                }
            
 
        }
    }
}


