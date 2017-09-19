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
��������void doSensor(void)
���ܣ��������������������

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
				InsideData[i] = adTophysical(AdcValueTemp,&Inside16BitAttrConfig[i]);//�Ǳ����� 
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
	
	if(SENSOR_1S_F)  // ��������к�
	{
		SENSOR_1S_F = 0;
		InsideData[9]++;
		if(InsideData[9]>22)  InsideData[9] = 1;//1~22
		J1939RequestIndex =  InsideData[9];
	}

	//�Ǳ����
	InsideData[5] = doDashBoardTemp(BoardTemperatureAdValue);           //�Ǳ����
	if(InsideData[5]!=OldInsideData[5]) 
	{
		InsideData[5] = (InsideData[5]+OldInsideData[5])/2+1;;
		OldInsideData[5] = InsideData[5] ;   
	} 
	else
	{
		OldInsideData[5] = InsideData[5] ;  
	}                             

	//��ص�ѹ
	InsideData[6] = calcVolatage(REFERENCE_VOLTAGE) + 7; //��ص�ѹ    REFERENCE_VOLTAGE
	if(InsideData[6]!=OldInsideData[6]) 
	{
		InsideData[6] = (InsideData[6]+OldInsideData[6])/2+1;;
		OldInsideData[6] = InsideData[6] ;   
	} 
	else
	{
		OldInsideData[6] = InsideData[6] ;  
	}
	
	//����
	CarSpeedSrcTmep = SysConfigData.CarSpeedSrc;
	RealCarSpeed = getCarSpeedChannel(CarSpeedSrcTmep);  //CarSpeedSrc               //���� 
	InsideData[7] = RealCarSpeed*100;
	InsideData[7] /= 98;

	//���ת��
	EngCarSpeedSrcTemp =  SysConfigData.EngCarSpeedSrc;
	InsideData[8] = getCarEngSpeedChannel(EngCarSpeedSrcTemp);//USR_s_OV.engSpeedFromDriver; 
	//����
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

	InsideData[13] =  ModuleErrorOldFlag.Byte;    //ģ�鶪ʧ

	InsideData[14] = J1939CompareData(SysConfigData.J1939CompareType0,SysConfigData.J1939CompareIndex0,SysConfigData.J1939Comparelen0);
	InsideData[15] = J1939CompareData(SysConfigData.J1939CompareType1,SysConfigData.J1939CompareIndex1,SysConfigData.J1939Comparelen1);
	InsideData[16] = J1939CompareData(SysConfigData.J1939CompareType2,SysConfigData.J1939CompareIndex2,SysConfigData.J1939Comparelen2);
	InsideData[17] = J1939CompareData(SysConfigData.J1939CompareType3,SysConfigData.J1939CompareIndex3,SysConfigData.J1939Comparelen3);

}

/*****************************************************************************************
��������

*****************************************************************************************
��������void adTophysical(UINT8 adChannel,UINT8 segment,UINT16 rang)
���ܣ���ADֵת��Ϊ������
����˵���� UINT8 adChannel -> ADͨ��������������Դ����������ͨ��
           UINT16 rang -> �����������̣���������Ϊ100����ѹΪ100��ˮ��Ϊ120���ȵ�ע�⣬��ʱ����Ҫ���з�Χ���� 
=============================================================================================================
��ʽ���㣺��ѹ�������������ȵĹ�ʽ�Ƶ� 
         5/255 = U/AD  => AD = 51U
           5R1               5R2
        (--------    y1)  (--------  , y2)  ����ȷ����ֱ�߷���Ϊ��
          R1+135   ,        R2+135
          
          5r         5R1
       --------- - --------          
        r + 135     R1+135           y-y1
   ----------------------------- = --------
          5R2        5R1             y2-y1
       --------- - --------
        R2 + 135    R1+135
        
    ��߷��ӷ�ĸͬʱ����51���ѵ�ѹת��ΪADֵ������ɵ�
          ad - AD1       y - y1
       -------------- = --------
         AD2 - AD1       y2- y1
         
              (y2-y1)*(ad-AD1)
         y = ------------------ + y1    (1)   ��ѹ�������������ȹ�ʽ
                 AD2 - AD1                  
=============================================================================================================
��ʽ�Ƶ��������������������ȵĹ�ʽ�Ƶ�
          (R1,y1) , (R2,y2)������ȷ����ֱ�߷���Ϊ��
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
ԭ���ڵ���ʱ��adValue��ADͨ���ķ�ʽ���е��ã�����ʵ��ͨ�����á�
*****************************************************************************************/
UINT16 adTophysical(UINT8 adValue,const INSIDE_16BIT_ATTR_STRUCT * pSegment)
{
    UINT8 dotNum;
    UINT8 i;
    UINT16 res;
    UINT16 result;
    //UINT16 temp;
    
    dotNum = pSegment->dotNum;
    if(!dotNum)            //��·����
    {
        return 0x00;
    }
    else
	{  
		switch(pSegment->style) 
		{
			case 0x80:    
				if(adValue >= 195)		
					return 0xfffe; //��·��· // 210
				res = adToRes(pSegment->inputRatio,adValue); break;
			case 0x81:    
				if(adValue <= 6)		
					return 0xfffe; //��·��· // 210
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
		for(i=0;i<dotNum-1;i++)  //ȡ��dotNum-1
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
��������UINT16 calcDashBoardTemp(UINT16 dashTempAdValue)
���ܣ������Ǳ��¶�
����˵����UINT16 dashTempAdValue -> �Ǳ��¶ȵ�ADת��ֵ

**********************************************************************************/
UINT16 doDashBoardTemp(UINT16 dashTempAdValue)
{
    UINT16 temperature;
    temperature = temperatureScaleChange(dashTempAdValue);
    DashBoardTempIndex = saveUINT16DataToRoundBuf(DashBoardTempRoundBuf,temperature,DashBoardTempIndex,DASHBOARD_TEMP_DATA_NUM);
    return averageUINT16DataWithoutMaxAndMin(DashBoardTempRoundBuf,DASHBOARD_TEMP_DATA_NUM);    
}
/**********************************************************************************
��������UINT16 temperatureScaleChange(UINT16 temperatureAdValue)
���ܣ��¶ȱ��ת��
����˵����UINT16 temperatureAdValue -> �¶�ADת��ֵ
����ֵ���¶�
���㹫ʽ��t=100x-50+40;
          x=(AD_value)/256*5
          t:ʵ���¶�ֵ
**********************************************************************************/
UINT16 temperatureScaleChange(UINT16 temperatureAdValue)
{
    return (((500 * temperatureAdValue) >> 0x08) - 10);    
}
/******************************************************************************************
��������void saveUINT16DataToRoundBuf(UINT16 *pRoundBuf,UINT16 data,UINT8 index,UINT8 size)
���ܣ�����UINT16���ݵ����λ�����
����˵����UINT16 *pRoundBuf -> ���λ�������
          UINT8 index -> ָ���λ������ڵ�ǰ�ɴ�λ�á�
          UINT8 size -> ���λ�������С
          UINT16 data -> �����뻷�λ������ڵ�����
����ֵ����
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
��������UINT16 averageUINT16DataWithoutMaxAndMin(UINT16 *pRoundBuf,UINT8 size)
���ܣ�ȥ�����ֵ����Сֵ���󻺳��������ݵ�ƽ��ֵ��
����˵����UINT16 *pRoundBuf -> ���λ�����
          UINT8 size -> ���λ�������С
����ֵ:����ƽ��ֵ
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
��������void calcVolatage(UINT16 volatageAdValue)
���ܣ������ص�ѹ
����˵����UINT16 volatageAdValue -> ��ص�ѹADת��ֵ
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
��������UINT8 resToAd(UINT16 res)
���ܣ��ѵ���ֵת��Ϊ����ֵ
����˵����UINT16 res ��֪�������� 

����ֵ�������ӵ���
***************************************************************************************/
UINT16 adToRes(UINT16 res,UINT16 ad)                      
{     
    dword temp; 
    temp = res;
    temp *= ad;
    temp+=(127-ad/2);             //��������
    temp /=(242-ad);                                           
    return (UINT16)temp;     
}
/***************************************************************************************
��������UINT8 resToAd(UINT16 res)
���ܣ��ѵ���ֵת��Ϊ����ֵ
����˵����UINT16 res -> ����
����ֵ��AD����ֵ
***************************************************************************************/

UINT16 adToVoltage(UINT16 Ratio,UINT16 ad)
{
    dword vtemp;
    //vtemp  = ((dword)ad*1000)/51;  //�������� MV       
    //vtemp *= (Ratio&0xff);
    //vtemp /= ((Ratio>>8)&0xff);
     vtemp  = ad;  //�������� MV       
     vtemp *= Ratio;
     vtemp /= 480;
    return (UINT16)vtemp;
}

/****************************************************************************************
��������UINT16 calcPhysicalDirectRatioRes(UINT8 r1,UINT8 y1,UINT8 r2,UINT8 y2,UINT8 ad)
���ܣ�������������������ȵ�������ֵ�������ʹ�������
����˵����(UINT8 r1,UINT8 y1) -> ֱ�߶ε�һ���˵�����
          (UINT8 r1,UINT8 y1) -> ֱ�߶ε���һ���˵�����
          UINT8 ad -> AD����ֵ
����ֵ��������ֵ
���㹫ʽ��
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
      result += ((r2-r1)/2);      //��������
      result /= (UINT16)(r2-r1);
      result += y1;
    } 
    else 
    {
      result = (ad-r1);
      result *= (UINT16)(y1-y2);
      result += ((r2-r1)/2);     //��������
      result /= (UINT16)(r2-r1);
      result = y1 - result;
    }
       return result;
}
/*********************************************************************************************
��������void getVirtualKey(void)
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
                  	case SCHMITT_TRIGGER_HIGH:                        // 0.6-0.8  �ر�     __|___|
                      //  keyv =  (virtualKey[row].Data >>line)&0x01;  // 0.8-0.6  ����    
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
                        // keyv =  (virtualKey[row].Data >>line)&0x01;  // 0.8-0.6  ����
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


