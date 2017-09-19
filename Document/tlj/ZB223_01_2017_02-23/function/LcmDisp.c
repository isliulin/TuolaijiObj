#define		LCMDISP_C
#include	"LcmDisp.h"

UINT16 SpeedRateTemp;      //2200
UINT8 CycleCaptureNumTemp;
UINT8 DashBoardSpeedChannelTemp;
UINT8 DashBoardEngSpeedChannelTemp;
UINT32 FirstTotalFuelConsume,HundredKilometerDistanceNumber;

UINT16 CarSpeedFromDashboard;
UINT16 CarEngSpeedFromDashboard;
// UINT16 PulseCnt[2];
 UINT16 LowSpeed[5];
 UINT8 LowSpeedPoint;
 UINT8 LowSpeedFlag;
 SPEED_DATA_BUF_STRUCT SpeedBuf;

 UINT16 LowEngSpeed[5];
 UINT8 LowEngSpeedPoint;
 UINT8 LowEngSpeedFlag;
 SPEED_DATA_BUF_STRUCT EngSpeedBuf;
unsigned long	CountDistanceSave;
unsigned 	char  count_speed;
UINT8   speed_count;
unsigned 	char	SendSpeedDataLast;
unsigned char	SendSpeedData[1];
unsigned 	char	SendSpeedDataUpdata1Num;
UINT16 CarSpeedFromDashboard;
unsigned 	char	SendSpeedDataUpdataNum;
UINT8 SpeedDataNum,SpeedData[16];





/****************************************************************************************
函数名：void doDistance(void)
功能：计算短途里程和总里程
****************************************************************************************/
extern	UINT32 TotalDistanceTimes;
void doDistance(void)
{

CaptureSpeed(&CapturePwm0);//JC5

	//if(_1S_F)
	/* 每秒计算里程*/
	if(TotalDistanceTimes >= 10);
	{  
		uint8 cnt;
		cnt =TotalDistanceTimes/ 10;
		TotalDistanceTimes -= (10 *  cnt);
		while(cnt--){
			USR_s_DeviceConfigData.TotalDistance = calcDistance(USR_s_DeviceConfigData.TotalDistance,CARSPEED);
			USR_s_DeviceConfigData.RelativeDistance = calcDistance(USR_s_DeviceConfigData.RelativeDistance,CARSPEED); 
		}
	}
	if(USR_s_DeviceConfigData.RelativeDistance>=9999999 /*|| CLEAR_RELATIVE_DIS_F*/)
	{
		USR_s_DeviceConfigData.RelativeDistance = 0x00;
		//        SaveOverallDistance();
	}
	if(USR_s_DeviceConfigData.RelativeDistance!=0) 
	{
		ClearRelativeDistance_F = 1;
	}
	//else
	//  {           
	//nbapo:30km保存一次
	if((USR_s_DeviceConfigData.TotalDistance - CountDistanceSave) >= (unsigned long )300000)   //////注 30km
	{
		CountDistanceSave = USR_s_DeviceConfigData.TotalDistance;
		PowerOffSave();
	}
	//}   
	CountHundredKilometerFuelConsume();
}

void CountHundredKilometerFuelConsume(void) 
{
	UINT32 fuelTemp,distanceTemp;
	if(ON_LOAD != OLD_ON_LCMDISP)
	{
		//*((UINT16 *)&StorageBuffer[J1939_OFFSET_BYTE+J1939_STATUS_NUMBER+J1939_CHAR_NUMBER])=0; 

		if(!OLD_ON_LCMDISP)   //on    
		{
			/// fual_value_require(); //发送请求
			TotalConsume = 0;
			FirstTotalFuelConsume  = 0;
			USR_s_OV.HundredKilometerFuelConsume =0;
			HundredKilometerDistanceNumber = USR_s_DeviceConfigData.TotalDistance;
		} 
		else 
		{
			USR_s_OV.HundredKilometerFuelConsume = 0;
		}
		OLD_ON_LCMDISP = ON_LOAD;
	} 
	else 
	{
		if(ON_LOAD)
		{  
		if(FUAL_REQUIRE_F) 
		{
			FUAL_REQUIRE_F = 0;    
			fual_value_require();
			if((TotalConsume!=0)&&(FirstTotalFuelConsume==0)) FirstTotalFuelConsume = TotalConsume;
			if((USR_s_DeviceConfigData.TotalDistance>HundredKilometerDistanceNumber)&&(TotalConsume>=FirstTotalFuelConsume)) 
			{
				//fuelTemp = (TotalConsume-FirstTotalFuelConsume)/2;   //0.5L /2= L
				//distanceTemp = USR_s_DeviceConfigData.TotalDistance-HundredKilometerDistanceNumber
				//distanceTemp /=10;//m
				//distanceTemp /=(100*1000);//100km
				fuelTemp =  (TotalConsume-FirstTotalFuelConsume)*500000;
				distanceTemp = USR_s_DeviceConfigData.TotalDistance-HundredKilometerDistanceNumber;
				USR_s_OV.HundredKilometerFuelConsume = fuelTemp/distanceTemp;
			}

			}
		}
	}
}

/***************************************************************

****************************************************************/
void	SaveOverallDistance(void)
{
/*	uchar	data[4];
	data[0] = (unsigned char)RelativeDistance;
	data[1] = (unsigned char)(RelativeDistance>>8);
	data[2] = (unsigned char)(RelativeDistance>>16);
	data[3] = (unsigned char)(RelativeDistance>>24);	
//	Eeprom_Write_Fact(OverallDistance_Addr,data,4);
//  Eeprom_Write_Fact(0x13F101,data,4);   */
}
/***************************************************************************************
函数名：UINT32 calcDistance(UINT16 speed)
功能：计算里程值
参数说明：UINT16 speed -> 车速值。//注：这个值为扩大10倍数据，例如 5.5km/h 记为 55
返回值：累计里程，单位为米
注意：1秒调用一次该函数。
计算方法： speed * 100 把米每小时转化为米每小时。 
           (speed * 100)/3600 = speed/36 即为米/秒
           因此1秒走过的距离为：speed/36米
***************************************************************************************/
UINT32 calcDistance(UINT32 distance,UINT16 speed)
{
    UINT16 temp;
    UINT16  remainder;
    speed *= 101;
    temp = speed / 360;
    remainder = speed%360;
    if(remainder >= 330)remainder = 9; 
    else if(remainder >= 290)remainder = 8;
    else if(remainder >= 250)remainder = 7;
    else if(remainder >= 210)remainder = 6;
    else if(remainder >= 180)remainder = 5;
    else if(remainder >= 150)remainder = 4;
    else if(remainder >= 110)remainder = 3;
    else if(remainder >= 80)remainder = 2;
    else if(remainder >= 40)remainder = 1;
    else remainder = 0;
    distance += temp * 10;
    distance += remainder; 
    return distance;
}
/********************************************************************************
函数名：void calcSpeed(UINT8 speed)
功能：计算车速值。
算法：
               															1公里
	 单个脉冲距离(公里）= ---------------------------------------------------------
													车速速比（1公里走了几圈）*脉冲个数（1圈几个霍尔传感器）
	 v(km/h) /3.6 * 1s= 1000/速比*脉冲个数*f
	 => v = 3600/速比*脉冲个数 * f 
	    10v = (36000/(速比*脉冲个数)) * f
	             3600 * f
	    v = ---------------
	           速比*脉冲个数
*****************************************************************************/
UINT16 calcSpeed(UINT16 frequency)
{
    UINT32 temp;
    UINT32 speed;
    temp = (UINT32)SpeedRateTemp * (UINT32)CycleCaptureNumTemp;
    speed = 36000 * (UINT32)frequency;
    speed /= temp;
    return (UINT16)speed; 
}   

/**************************************************************************************
* brief:    tmp2 channel1 capture interrupt service routine
* author:   yao qing rui
*arithmetic:															
						       1				                实际转速值
   ---------------------------- = ------------------------------------
 （飞轮齿数*单个脉冲时间（us））	 60*1000*1000（将min转换成us）
 
              
  



  公式描述：转速单位是rpm/min，即1分钟多少转；发动机1转即为飞轮动作一周，所以飞轮齿数*单个脉冲时间即为1转所用的时间。
		
**************************************************************************************/ 
UINT16 calcEngSpeed(UINT16 frequency)           //频率
{

    UINT32 temp;
    temp =  (UINT32)frequency;
   // UINT32 speed*60;
    
   // temp = (UINT32)SpeedRateTemp * (UINT32)CycleCaptureNumTemp;
   // speed = 36000 * (UINT32)frequency;
   // speed /= temp;
    return (UINT16)temp; 

}



/*****************************************************************************
函数名：void initSpeedBuf(void)
功能：初始化速度缓冲区
*****************************************************************************/
void initSpeedBuf(void)
{
    SpeedBuf.flag = 0x00;
    SpeedBuf.point = 0x00;
    SpeedBuf.data[0] = 0x00;
    SpeedBuf.data[1] = 0x00;
    SpeedBuf.data[2] = 0x00;
    SpeedBuf.data[3] = 0x00; 
    EngSpeedBuf.flag = 0x00;
    EngSpeedBuf.point = 0x00;
    EngSpeedBuf.data[0] = 0x00;
    EngSpeedBuf.data[1] = 0x00;
    EngSpeedBuf.data[2] = 0x00;
    EngSpeedBuf.data[3] = 0x00;
} 

UINT16 filter(UINT16 newData)
{ 
    UINT16 temp;
    UINT16 oldData;
    UINT16 dValue;
    UINT8 point;
    if(!SpeedBuf.flag)
    {
        SpeedBuf.data[0] = newData;
        SpeedBuf.data[1] = newData;
        SpeedBuf.data[2] = newData;
        SpeedBuf.data[3] = newData;
        SpeedBuf.point = 0x01;
        SpeedBuf.flag = 0x01;
        return newData;
    }  
    else
    {
        point = SpeedBuf.point;
        if(point)
        {
            point--;
        }
        else
        {
            point = 0x03;
        }
        oldData = SpeedBuf.data[point];
        if(oldData > newData)
        {
            temp = oldData - newData;
            dValue = BRAKE_D_VALUE;
            if(temp > dValue)
            {
                newData = oldData - dValue;
            }
        }
        else
        {
            temp = newData - oldData;
            dValue = SPEED_UP_D_VALUE;
            if(temp > dValue)
            {
                newData = oldData + dValue;
            }
        }    
        SpeedBuf.data[SpeedBuf.point] = newData;
        if(SpeedBuf.point == 0x03)
        {
            SpeedBuf.point = 0x00;
        }
        else
        {
            SpeedBuf.point++;
        }
        dValue = SpeedBuf.data[0]+SpeedBuf.data[1]+SpeedBuf.data[2]+SpeedBuf.data[3];
        dValue /= 4;
        return dValue;
    }   
}
 
UINT16 calcRealSpeed(UINT8 channel)
{
    UINT16 speedPulseCnt = PulseCnt[channel];
    UINT16 speed;
    PulseCnt[channel] = 0x00;//清零计数器
    if(speedPulseCnt <= 0x02)
    {
        if((!LowSpeedPoint) && (!LowSpeedFlag))
        {
            LowSpeed[0] = speedPulseCnt;
            LowSpeed[1] = speedPulseCnt;
            LowSpeedFlag = 0x01;
            LowSpeedPoint++;
        }
        else
        {
            LowSpeed[LowSpeedPoint] = speedPulseCnt;
            LowSpeedPoint++;
            if(LowSpeedPoint >= 0x01)
            {
                LowSpeedPoint = 0x00; 
            }
        }
        speedPulseCnt = LowSpeed[0] + LowSpeed[1];// + LowSpeed[2] + LowSpeed[3] + LowSpeed[4];
        speed = calcSpeed(speedPulseCnt);
    }  
    else
    {  

        LowSpeedPoint = 0x00;
        LowSpeedFlag = 0x00;
        speed = calcSpeed(speedPulseCnt*5);   
    }
    return filter(speed);   
}

UINT16 filterEngSpeed(UINT16 newData)
{
    UINT16 temp;
    UINT16 oldData;
    UINT16 dValue;
    UINT8 point;
    if(!EngSpeedBuf.flag)
    {
        EngSpeedBuf.data[0] = newData;
        EngSpeedBuf.data[1] = newData;
        EngSpeedBuf.data[2] = newData;
        EngSpeedBuf.data[3] = newData;
        EngSpeedBuf.point = 0x01;
        EngSpeedBuf.flag = 0x01;
        return newData;
    }  
    else
    {
        point = EngSpeedBuf.point;
        if(point)
        {
            point--;
        }
        else
        {
            point = 0x03;
        }
        oldData = EngSpeedBuf.data[point];
        if(oldData > newData)
        {
            temp = oldData - newData;
            dValue = ENGBRAKE_D_VALUE;
            if(temp > dValue)
            {
                newData = oldData - dValue;
            }
        }
        else
        {
            temp = newData - oldData;
            dValue = ENGSPEED_UP_D_VALUE;
            if(temp > dValue)
            {
                newData = oldData + dValue;
            }
        }    
        EngSpeedBuf.data[EngSpeedBuf.point] = newData;
        if(EngSpeedBuf.point == 0x03)
        {
            EngSpeedBuf.point = 0x00;
        }
        else
        {
            EngSpeedBuf.point++;
        }
        dValue = EngSpeedBuf.data[0]+EngSpeedBuf.data[1]+EngSpeedBuf.data[2]+EngSpeedBuf.data[3];
        dValue /= 4;
        return dValue;
    }
} 


UINT16 calcRealEngSpeed(UINT8 channel)   //
{
    UINT16 EngspeedPulseCnt;
    UINT16 Engspeed;
    EngspeedPulseCnt = PulseCnt[channel];
    PulseCnt[channel] = 0x00;//清零计数器
    if(EngspeedPulseCnt <= 0x02)
    {
        if((!LowEngSpeedPoint) && (!LowEngSpeedFlag))
        {
            LowEngSpeed[0] = EngspeedPulseCnt;
            LowEngSpeed[1] = EngspeedPulseCnt;
            LowEngSpeedFlag = 0x01;
            LowEngSpeedPoint++;
        }
        else
        {
            LowEngSpeed[LowEngSpeedPoint] = EngspeedPulseCnt;
            LowEngSpeedPoint++;
            if(LowEngSpeedPoint >= 0x02)
            {
                LowEngSpeedPoint = 0x00; 
            }
        }
        EngspeedPulseCnt = LowEngSpeed[0] + LowEngSpeed[1];// + LowSpeed[2] + LowSpeed[3] + LowSpeed[4];
        Engspeed = calcEngSpeed(EngspeedPulseCnt);
    }  
    else
    {  

        LowEngSpeedPoint = 0x00;
        LowEngSpeedFlag = 0x00;
        Engspeed = calcEngSpeed(EngspeedPulseCnt*5);   
    }
    return filterEngSpeed(Engspeed); 
}
/******************************************************************************
函数名：void doSpeed(void)
功能：计算车速值
******************************************************************************/
/*void doSpeed(void)
{
    if(_500MS_F)
    {
       // CarSpeedFromDashboard = calcRealSpeed(&PulseCnt[DashboardSpeedChannel]);
        _500MS_F = 0;
    }
}*/

