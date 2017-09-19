#define	  MESSAGE_C
#include "message.h"

//#pragma CODE_SEG  USER_ROM   
UINT16 	dis_count;//故障总个数
UINT16 	dis_content[MulfunctionTotalNum];
UINT16 ModuleConfigTemp;     
UINT8 PwmLoadOutType;
UINT8 OldDashboardLoadStatusData[2];
UINT8 OldFrontLoadStatusData[5];
UINT8 OldBackLoadStatusData[5];
UINT8 OldMiddleLoadStatusData[5];
UINT8 OldTopLoadStatusData[5];
UINT8 OldPowerStatusData[5];
UINT8 OldPowerFuseStatusData[5];
UINT8 OldPowerContorolLoadState[5];
#if defined(ZB223BYY00)
const UINT8 Version[12] = {"B00_00     "};   
#elif defined(ZB223YY01)
const UINT8 Version[12] = {"00_00     "};
#else 
const UINT8 Version[12] = {"00_00     "};
#endif
UINT32 TotalConsume;
UINT16 SleepDelayCnt;
UINT8 ClrSleepCnt;



 //06 ---里程参数放外部
 //07 ---增加胎压+雨刮上电初始化
 //08 ---摄像头黑白,显示去零 多包3加入
                       //1:表示有接上该模块
 //08_01 -->JA13 JA23 pwm输出可配为负载输出
 //08_02 -->电源管理添加
 //08_03 -->优化j1939
 //08_04 -->优化声音报警
 //09_10 --> 开机自检+ 休眠延时增长
 //09_11 -->虚拟负载增加到88个  
 //09_12 -->JA23脉冲输出诊断
 //09_13 -->传感器声音报警需逻辑表配置
 //09_14 -->左右转向灯声音可配 
 //09_15 --> j1939增加多包报文超速清除值与虚拟开关
 //09_16 --> 增加五洲龙多包支持
 //09_17 --> 优化左右转向灯声音可屏蔽
 //09_18 --> 添加视频监控查询界面（界面36  通道3）
 //09_19 --> J1939 高4位索引模式 电压显示抖动
 //09_20 --> PWM负载输出开路诊断
 //09_22 --> 增大逻辑地址 
 //09_23 --> 电机负转
 //09_24 --> J1939报文超时15s
 //09_25 --> J1939虚拟开关 超时清虚拟开关出错(虚拟开关48以后)
 //09_26 --> 添加PD205模块
 //09_29 --> 转向灯同步
 //09_30 --> 黑屏
 //09_31 --> 气压抖动滤波
 //09_32 --> J1939  10超时 图文报警160个
 //09_33 --> 12v车速电源保护,多包故障溢出
 //09_37 --> 部分仪表OSD字体出现条纹
 //09_38 --> 部分仪表OSD字体出现条纹  蔡工处理
 //09_41 --> 虚拟开关增加+声音报警延时
 //09_42 --> 车速转发出错 中断零界点没保护
 //09_44 --> 30km保存一次                  
 //09_45 --> (取消ON 还原ACC)
 //09_46 --> 电池箱号请求 改翻页请求
 //09_48 --> 关ON档保存里程+累积30公里保存一次  + 胎压学习失败
 //09_51 --> 里程累积误差
 //09_52 --> 增加二进制显示
 //09_53 --> 实测误差1%
 //09-54 --> 在ON累积里程
 //09-55 --> 车速采集换传统
 
  //09-57 --> LNG车载气瓶CAN 总线变送器报文结构
  
//const UINT8*  ModuleTemperaturePoint[5] = {&FrontModuleTemperature,&BackModuleTemperature,&MiddleModuleTemperature
//                                       ,&TopModuleTemperature,&PowerModuleTemperature};
//UINT8 ModuleTemperature[5];         //模块温度从传感器数据帧中获取。
//const J1939FaultCodeBase = FAULT_CODE_BASE_AVAID;
//const J1939FaultStatusBase = FAULT_CODE_BASE_AVAID;
//#pragma CONST_SEG    DEFAULT
/**********************************************************************************
气压配置说明： 0 1 000 000
    第7位为0表示气压取自传感器
           1表示气压取自J1939
    第6为为0表示无气压参数
           1表示有气压参数
    第3-5位表示传感器所在的模块(000:仪表，001:前驱，010:后驱 011:中驱 100:顶驱 101: 备用 110:备用 111:备用)
    第0-2位表示传感器所在模块的通道。
**********************************************************************************/
/*UINT8 FrontAirPressureConfig;
UINT8 BackAirPressureConfig;
UINT8 OilMassConfig;
UINT8 OilPressConfig;  */
/**********************************************************************************
车速配置说明：0 0 000 000
    第7位为0表示车速取自车速传感器
           1表示车速取自J1939
    第6位为备用位
    第3-5位表示车速传感器所在的模块(000:仪表，001:前驱，010:后驱 011:中驱 100:顶驱 101: 备用 110:备用 111:备用)
    第0-2位表示车速传感器所在的模块的通道
**********************************************************************************/
//UINT8 CarSpeedConfig;



 /****************************************************************************************
 函数名：void doFaultWarning(void)
 功能：处理故障报警报警状态
 1、模块丢失故障报警显示		:5
 2、高CAN丢失故障报警显示		:1
 3、图标引起的故障报警显示		:48
 4、仪表负载开路短路故障报警显示:8 
 5、仪表传感器开路
 6：驱动负载开短路故障报警显示
 7、驱动传感器开路 
 8、配电负载开短路故障报警显示： 
 ****************************************************************************************/
 UINT16 LoadWarnCnt ;
 void doFaultWarning(void)
 {	  
	 LoadWarnCnt = 0;											 
	 doModuleErrorToStack(); // 模块和高CAN
	 getIcoWarn();		  //图标报警故障 ，在声音处理里面已经调用过一次，在这边不调用。
	 ModuleFault(); 		 //模块开短路，传感器故障
 }
 
 /******************************************************************************************
 函数名：void pushModuleErrorToStack(void)
 //模块丢失 故障码:1~6
 ******************************************************************************************/
 void doModuleErrorToStack(void)
{
	UINT8 i;

	for(i=0;i<8;i++)
	{
		if(ModuleConfigTemp & (0x01 << i)) 
		{	 
			LoadWarnCnt++ ;
			if((ModuleErrorFlag.Byte & (0x01 << i)) != (ModuleErrorOldFlag.Byte & (0x01 << i)))
			{		 
				if(!(ModuleErrorOldFlag.Byte & (0x01 << i)))
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
	if(ModuleErrorFlag.Byte != ModuleErrorOldFlag.Byte) 
	{
		moduleLost();
		ModuleErrorOldFlag.Byte = ModuleErrorFlag.Byte;
	}
}
 /*********************************************************************
 funtion operation:when accept new malfunction,push into stack;and diplay in the first line
 funtion call:void
 input parameter:tmp;the number which will be pushed
 return:void
 ********************************************************************/
void push_dis(UINT16 tmp)
{
	UINT16 i;
	dis_count++;						   
	for (i=dis_count;i>0;i--) {
		dis_content[i]=dis_content[i-1];
	}
	dis_content[0]=tmp;				  ///new display data push to front
}
 /**********************************************************************
 funtion operation: when malfuction is canceled,pop malfuction number;
						 the next malfuction will ascend;latter term be cleared;
 funtion call:void
 input parameter:tmp;the number which will be poped
 return:void	 
 ***********************************************************************/
 void pop_dis(UINT16 tmp)
{
	UINT16 i;
	UINT16  dis_location;
	if(dis_count>0)
	{
		for(i=0;i<dis_count;i++)
		{
			if(tmp==dis_content[i])
			{
				dis_location=i;								 ///find cancel mulfunction comment
				break;
			}
		}
		dis_content[dis_count]=0x00;				 ///cancel mulfunction

		for(i=dis_location;i<dis_count;i++)
		{
			dis_content[i]=dis_content[i+1];	 ///rear mulfunction rising in turn
		}
		dis_count--;
	}
}


/****************************************************************************************************
        Funtion:change to speed pwm period
    Arithmatics:			
                                                  					 1公里
        单个脉冲距离(公里）= --------------------------------------------------------------
                                车速速比（1公里走了几圈）*脉冲个数（1圈几个霍尔传感器）
                                                    
                                                       单个脉冲距离(公里）
        单个脉冲时间（小时）=------------------------------
                                                       速度（公里/小时）		
        单个脉冲时间（us）= 单个脉冲时间（小时）*3600*1000*1000
                                                                                                3600*1000*1000
        单个脉冲时间（us）= -------------------------------------------------------------------------
                                                车速速比（1公里走了几圈）*脉冲个数（1圈几个霍尔传感器）* 速度（公里/小时）
                                    单个脉冲时间（us）
        PWM周期 = ----------------------------
                                PWM 时钟周期(现用128us)
*****************************************************************************************************/
#include "HwMacroPatch.h"
unsigned int PWM_OUT_frq=0;
void SpeedPwmOut(void)
{
    
    ulong   SpeedPwmTemp;
    ulong   HourChangeToUsNum;
   // if(PwmLoadOutType&0x02)   return ;
   if(PwmLoadOutType&0x01)   return ;	
	//nbapo:车速不为0
    if (CARSPEED >= 3) {
		
		
        SpeedPwmTemp =SysConfigData.CycleCaptureNum * ((ulong)SysConfigData.SpeedRate);
        SpeedPwmTemp = SpeedPwmTemp * PwmPeriodNum;
        SpeedPwmTemp = SpeedPwmTemp * CARSPEED;
        HourChangeToUsNum = 3600 * ((unsigned long)1000);///hour change to us 
        HourChangeToUsNum = HourChangeToUsNum * 1000;

		PWM_OUT_frq = SpeedPwmTemp / 3600;
		PWM_OUT_frq +=5;  //误差5hz
		//PWMOUT1_PORT_MUX();
		//FTM_PWM_init(PWMOUT1_MODULE, PWMOUT1_CH, frq,50);   
                
               
		FTM_PWM_init(PWMOUT2_MODULE, PWMOUT2_CH, PWM_OUT_frq,50);   
		  if((PwmLoadOutType&0x02)==0) 
              LPTMR0_CSR |=LPTMR_CSR_TEN_MASK;   
                
                
        ///PWMPER45 = (uint)(HourChangeToUsNum/SpeedPwmTemp);
        ///PWMDTY45 = PWMPER45 / 2;   ///duty is 1/2 to period
        //PWMPER23 =  PWMPER45;
        //PWMDTY23 =  PWMPER23/2;
    } 
    else 
    {
    	if(!SleepFlag)
    	{
    		//PWMOUT1_PORT_MUX();
			//FTM_PWM_init(PWMOUT1_MODULE, PWMOUT1_CH, 100,100);
			PWMOUT2_PORT_MUX();
			//FTM_PWM_init(PWMOUT2_MODULE, PWMOUT2_CH, 100,100);
			FTM_PWM_init(PWMOUT2_MODULE, PWMOUT2_CH, 100,0);
			  if((PwmLoadOutType&0x02)==0) 
			 {
			LPTMR0_CSR &=~LPTMR_CSR_TEN_MASK;   
			PWMOUT1_L;
			}
		}
   
		else {
                  //PWMOUT1_PORT_MUX();
			PWMOUT2_PORT_MUX();
			//FTM_PWM_init(PWMOUT1_MODULE, PWMOUT1_CH, 100,0);
			FTM_PWM_init(PWMOUT2_MODULE, PWMOUT2_CH, 100,0);
	             if((PwmLoadOutType&0x02)==0) 
			{
			   LPTMR0_CSR &=~LPTMR_CSR_TEN_MASK;   
			   PWMOUT1_L;
			}
			
		}
			
        ///PWMPER45=0xfffe;                    ///all low electric
        ///PWMDTY45=0xffff;
        //PWMPER23 =  0xfffe;
        //PWMDTY23 =  0xffff;
    }
}


/*******************************************************************************************
函数名：void moduleLost(void)
功能：模块丢失处理
*******************************************************************************************/
void moduleLost(void)
{
    if(SleepFlag==1)  return;
    frontDriveLost();
    backDriveLost();
    middleDriveLost();
    topDriveLost();
    powerDriveLost();
    powerContorolDriveLost();
    j1939Lost();
}

/******************************************************************************************* 
函数名：void clearData(UINT8 *pData,UINT8 cnt)
*******************************************************************************************/
void clearData(UINT8 *pData,UINT8 cnt)
{
    UINT8 i;
    for(i=0;i<cnt;i++)
    {
        *pData++ = 0x00;
    }
}
void clearIntData(UINT16 *pData,UINT8 cnt)
{
    UINT8 i;
    for(i=0;i<cnt;i++)
    {
        *pData++ = 0x00;
    }
}

/*******************************************************************************************
函数名：void frontDriveLost(void)
功能：前模块丢失时，清除一些参数
*******************************************************************************************/
void frontDriveLost(void)
{ 
    if(FRONT_DRIVE_MODULE_ERROR_F)
    {  
        clearData(&USR_s_OV.FrontDriveKey[0].Data,sizeof(USR_s_OV.FrontDriveKey));
        clearData(&USR_s_OV.FrontDriverVirtualKey[0].Data,sizeof(USR_s_OV.FrontDriverVirtualKey));
        clearData(&USR_s_OV.FrontDriveLoad[0].Data,sizeof(USR_s_OV.FrontDriveLoad));
        clearData(&USR_LOAD_OV.FrontDriveLoad[0].Data,sizeof(USR_LOAD_OV.FrontDriveLoad));  
        clearIntData(&InsideData[FRONT_INSIDE_OFFSET],(BACK_INSIDE_OFFSET-FRONT_INSIDE_OFFSET));
        clearData(USR_s_OV.frontDriverVersion,0x08); 
        USR_s_OV.CarSpeedFromDriver = 0;
    }
}
/*******************************************************************************************
函数名：void backDriveLost(void)
功能：前模块丢失时，清除一些参数
*******************************************************************************************/
void backDriveLost(void)
{ 
    if(BACK_DRIVE_MODULE_ERROR_F)
    {  
        clearData(&USR_s_OV.BackDriveKey[0].Data,sizeof(USR_s_OV.BackDriveKey));
        clearData(&USR_s_OV.BackDriverVirtualKey[0].Data,sizeof(USR_s_OV.BackDriverVirtualKey));
        clearData(&USR_s_OV.BackDriveLoad[0].Data,sizeof(USR_s_OV.BackDriveLoad));
        clearData(&USR_LOAD_OV.BackDriveLoad[0].Data,sizeof(USR_LOAD_OV.BackDriveLoad)); 
        clearIntData(&InsideData[BACK_INSIDE_OFFSET],(MIDDLE_INSIDE_OFFSET-BACK_INSIDE_OFFSET));
        clearData(USR_s_OV.backDriverVersion,0x08);
        USR_s_OV.CarSpeedFromDriver = 0;  
    }
}
/*******************************************************************************************
函数名：void MiddleDriveLost(void)
功能：前模块丢失时，清除一些参数
*******************************************************************************************/
void middleDriveLost(void)
{ 
    if(MIDDLE_DRIVE_MODULE_ERROR_F)
    {  
        clearData(&USR_s_OV.MiddleDriveKey[0].Data,sizeof(USR_s_OV.MiddleDriveKey));
        clearData(&USR_s_OV.MiddleDriverVirtualKey[0].Data,sizeof(USR_s_OV.MiddleDriverVirtualKey));
        clearData(&USR_s_OV.MiddleDriveLoad[0].Data,sizeof(USR_s_OV.MiddleDriveLoad));
        clearData(&USR_LOAD_OV.MiddleDriveLoad[0].Data,sizeof(USR_LOAD_OV.MiddleDriveLoad));
        clearIntData(&InsideData[MIDDLE_INSIDE_OFFSET],(TOP_INSIDE_OFFSET-MIDDLE_INSIDE_OFFSET));
        clearData(USR_s_OV.middleDriverVersion,0x08);  
        USR_s_OV.CarSpeedFromDriver = 0;
    }
}
/*******************************************************************************************
函数名：void TopDriveLost(void)
功能：前模块丢失时，清除一些参数
*******************************************************************************************/
void topDriveLost(void)
{ 
    if(TOP_DRIVE_MODULE_ERROR_F)
    {  
        clearData(&USR_s_OV.TopDriveKey[0].Data,sizeof(USR_s_OV.TopDriveKey));
        clearData(&USR_s_OV.TopDriverVirtualKey[0].Data,sizeof(USR_s_OV.TopDriverVirtualKey));
        clearData(&USR_s_OV.TopDriveLoad[0].Data,sizeof(USR_s_OV.TopDriveLoad));
        clearData(&USR_LOAD_OV.TopDriveLoad[0].Data,sizeof(USR_LOAD_OV.TopDriveLoad));
        clearIntData(&InsideData[TOP_INSIDE_OFFSET],(POWER_INSIDE_OFFSET-TOP_INSIDE_OFFSET));
        clearData(USR_s_OV.topDriverVersion,0x08);   
        USR_s_OV.CarSpeedFromDriver = 0; 
    }
}
void powerDriveLost(void)
{ 
    if(POWER_MODULE_ERROR_F)
    {  
        //clearData(&USR_s_OV.PowerDriverVirtualKey[0].Data,0x04);
        clearData(&USR_s_OV.PowerLoadState[0].Data,sizeof(USR_s_OV.PowerLoadState));
        clearData(&USR_LOAD_OV.PowerLoadState[0].Data,sizeof(USR_LOAD_OV.PowerLoadState));
        clearData(&USR_s_OV.PowerFuse[0].Data,sizeof(USR_s_OV.PowerFuse));
        clearData(USR_s_OV.powerVersion,0x08);
        clearIntData(&InsideData[POWER_INSIDE_OFFSET],(POWER_CONTOROL_OFFSET-POWER_INSIDE_OFFSET));
    }
}

void powerContorolDriveLost(void)
{ 
    if(POWER_CONTOROL_ERROR_F)
    {  
        clearData(&USR_s_OV.PowerContorolKey[0].Data,sizeof(USR_s_OV.PowerContorolKey));
        clearData(&USR_s_OV.PowerContorolVirtualKey[0].Data,sizeof(USR_s_OV.PowerContorolVirtualKey));
        clearData(&USR_s_OV.PowerContorolLoadState[0].Data,sizeof(USR_s_OV.PowerContorolLoadState));
        clearData(&USR_LOAD_OV.PowerContorolLoadState[0].Data,sizeof(USR_LOAD_OV.PowerContorolLoadState));   
        clearData(USR_s_OV.PowerContorolVersion,0x08);
        clearIntData(&InsideData[POWER_CONTOROL_OFFSET],(END_INSIDE_OFFSET1-POWER_CONTOROL_OFFSET));
    }
}
/*******************************************************************************************
函数名：void j1939Lost(void)
*******************************************************************************************/
void j1939Lost(void)
{
    UINT16 i;
    if(HIGH_CAN_MODULE_ERROR_F != OLD_HIGH_CAN_MODULE_ERROR_F)
    {
    	if(HIGH_CAN_MODULE_ERROR_F){
			for(i=0;i<J1939_STORAGE_SIZE;i++)
			StorageBuffer[i]=0x00;
			for(i=0;i<10;i++)  //DashboardVirtualKey[i].Data= 0;
			USR_s_OV.DashboardVirtualKey[i].Data= 0;
		}
      
    } 
    
    
    OLD_HIGH_CAN_MODULE_ERROR_F = HIGH_CAN_MODULE_ERROR_F;
/******************* 车速 转速 丢失处理 ********************************************/
    if(SPEED_DEVICE_LOSE_F) 
    {
       USR_s_OV.CarSpeedFromDriver = 0;
    }
    if(ROTATION_DEVICE_LOSE_F) 
    {
       USR_s_OV.engSpeedFromDriver = 0;
    }
}


/*******************************************************************************************
函数名：void frontDriveLost(void)
功能：前模块丢失时，清除一些参数
******************************************************************************************/
void initModuleDetectParameter(void)
{
    UINT8 i;
    for(i=0;i<8;i++)
    {
        ModuleDetectCnt[i] = 0x00;
    }
    ModuleDetectFlag.Byte = 0x00;
    ModuleErrorFlag.Byte = 0x00;
    ModuleErrorOldFlag.Byte = 0x00; 
    dis_count = 0;
    clearData(&USR_s_OV.FrontDriveLoad[0].Data,0x05);
    clearData(&USR_s_OV.TopDriveLoad[0].Data,0x05);
    clearData(&USR_s_OV.BackDriveLoad[0].Data,0x05);
    clearData(&USR_s_OV.MiddleDriveLoad[0].Data,0x05);
    clearData(&USR_s_OV.PowerLoadState[0].Data,0x05);
    clearData(&USR_s_OV.PowerFuse[0].Data,0x05);
}

/********************************************************************************************
函数名：void calcModuleFaultBaseNum(void)
功能：模块故障代码
********************************************************************************************/
void ModuleFault(void)
{
    ExLoadStatusWarnToStack(OldDashboardLoadStatusData,(UINT8 *)&USR_s_OV.DashboardLoad[0],IcoWarnConfig.DashDriverIcoWarn,4);
    ExSensorFault(0x00,IcoWarnConfig.DashSensorIcoWarn,0x05);                       
    if(ModuleConfigTemp & 0x01)
    {
        ExLoadStatusWarnToStack(OldFrontLoadStatusData,(UINT8 *)&USR_s_OV.FrontDriveLoad[0],IcoWarnConfig.FrontDriverIcoWarn,20); 
        ExSensorFault(FRONT_INSIDE_OFFSET,IcoWarnConfig.FrontSensorIcoWarn,0x08);    //914k
    }  
    if(ModuleConfigTemp & 0x02)
    {
        ExLoadStatusWarnToStack(OldBackLoadStatusData,(UINT8 *)&USR_s_OV.BackDriveLoad[0],IcoWarnConfig.BackDriverIcoWarn,20); 
        ExSensorFault(BACK_INSIDE_OFFSET,IcoWarnConfig.BackSensorIcoWarn,0x08);    //914k
    }
    if(ModuleConfigTemp & 0x04)
    {
        ExLoadStatusWarnToStack(OldMiddleLoadStatusData,(UINT8 *)&USR_s_OV.MiddleDriveLoad[0],IcoWarnConfig.MiddleDriverIcoWarn,20); 
        ExSensorFault(MIDDLE_INSIDE_OFFSET,IcoWarnConfig.MiddleSensorIcoWarn,0x08);    
    }
    if(ModuleConfigTemp & 0x08)
    {
        ExLoadStatusWarnToStack(OldTopLoadStatusData,(UINT8 *)&USR_s_OV.TopDriveLoad[0],IcoWarnConfig.TopDriverIcoWarn,20); 
        ExSensorFault(TOP_INSIDE_OFFSET,IcoWarnConfig.TopSensorIcoWarn,0x08);       
    } 
    if(ModuleConfigTemp & 0x10)
    {
        if(SysConfigData.selectPowerCtr==1) 
        { 
          ExLoadStatusWarnToStack(OldPowerFuseStatusData,(UINT8 *)&USR_s_OV.PowerFuse[0],IcoWarnConfig.Power205FuseIcoWarn,20);
        }
        else
        {
          
          ExLoadStatusWarnToStack(OldPowerStatusData,(UINT8 *)&USR_s_OV.PowerLoadState[0],IcoWarnConfig.PowerDriverIcoWarn,18);
          ExLoadStatusWarnToStack(OldPowerFuseStatusData,(UINT8 *)&USR_s_OV.PowerFuse[0],IcoWarnConfig.PowerFuseIcoWarn,17);
        }
    } 
    if(ModuleConfigTemp & 0x40)
    {                                          
        ExLoadStatusWarnToStack(OldPowerContorolLoadState,(UINT8 *)&USR_s_OV.PowerContorolLoadState[0],IcoWarnConfig.PowerContorolIcoWarn,16);
        ExSensorFault(POWER_CONTOROL_OFFSET,IcoWarnConfig.PowerContorolSensorIcoWarn,0x03);    
    }
}

/******************************************************************************************
函数名：doLoadStatusWarnToStack(UINT8 base,UINT8 * pOldStatus,UINT8 * pNewStatus,UINT8 cnt) 
功能：把负载开路、短路状态压入故障代码堆栈里面。
参数说明：UINT8 base -> 故障代码的起始值               
          UINT8 * pOldStatus -> 旧的状态值
          UINT8 * pNewStatus -> 新的状态值
          UINT8 cnt -> 状态个数。一个字节中包含4个。
******************************************************************************************/

void ExLoadStatusWarnToStack(UINT8 *pOldStatus,UINT8 *pNewStatus,const UINT8 * pWarnByte,UINT8 cnt)
{
    UINT8 i;
   // UINT16 temp;
    UINT8 warn;   
    UINT8  line,row,type;
    UINT8 newStatus;
    UINT8 oldStatus;
    for(i=0;i<cnt;i++) 
    {
    	if( pWarnByte[i]==0) continue;//add by L:上位机补0
		warn = pWarnByte[i]&0x0f; 
		if((warn & 0x08)==0) 
		{
			row  = i/0x04; 
			line = i%0x04;
			oldStatus = (pOldStatus[row] >> (2*line)) & 0x03;
			newStatus = (pNewStatus[row] >> (2*line)) & 0x03;
			type = warn & 0x07;
			switch(type) 
			{
			  case 2: 
			        LoadWarnCnt++;
			        if(oldStatus != newStatus)
					{ 
						if((newStatus == LOAD_SHORT)&&(oldStatus != LOAD_SHORT))
						{ 
							push_dis(LoadWarnCnt);
						} 
						else if((newStatus != LOAD_SHORT)&&(oldStatus == LOAD_SHORT))
						{
							pop_dis(LoadWarnCnt);   
						}             //短路报警
					} 
					break;
			  case 1:
			        LoadWarnCnt++;
			        if(oldStatus != newStatus)
			        { 
			          if((newStatus == LOAD_OPEN)&&(oldStatus != LOAD_OPEN))
			          { 
			              push_dis(LoadWarnCnt);
			          }
			          else if((newStatus != LOAD_OPEN)&&(oldStatus == LOAD_OPEN))
			          {
			               pop_dis(LoadWarnCnt);   
			          }             //开路报警
			        } 
					break;
			  case 3: //L:开短路
			                    //开路
			          if(oldStatus != newStatus)
			          { 
			             LoadWarnCnt++;
			             if((newStatus == LOAD_OPEN) && (oldStatus != LOAD_SHORT)) 
			                {       
			                    push_dis(LoadWarnCnt);   //开路
			                    LoadWarnCnt++;
			                }
			                else if((newStatus == LOAD_SHORT) && (oldStatus != LOAD_OPEN))
			                {
			                    LoadWarnCnt++;   //短路
			                    push_dis(LoadWarnCnt);
			                } 
			                else if((newStatus == LOAD_OPEN) && (oldStatus == LOAD_SHORT))
			                {
			                    LoadWarnCnt++;    //短路切换开路
			                    pop_dis(LoadWarnCnt);   
			                    LoadWarnCnt--;
			                    push_dis(LoadWarnCnt);  
			                    LoadWarnCnt++;                 
			                }
			                else if((newStatus == LOAD_SHORT) && (oldStatus == LOAD_OPEN))
			                {
			                                           //开路切换短路
			                    pop_dis(LoadWarnCnt);   
			                    LoadWarnCnt++ ;
			                    push_dis(LoadWarnCnt);                        
			                }
			                else if(((newStatus == LOAD_RUNING) || (newStatus== LOAD_CLOSE)) && oldStatus == LOAD_OPEN)
			                {
			                                        //开路回复
			                    pop_dis(LoadWarnCnt);
			                    LoadWarnCnt++ ;
			                }
			                else if(((newStatus == LOAD_RUNING) || (newStatus == LOAD_CLOSE)) && oldStatus == LOAD_SHORT)
			                {
			                    LoadWarnCnt++;    //短路回复
			                    pop_dis(LoadWarnCnt);                        
			                }
			                else
			                {
			                   LoadWarnCnt++;  
			                }
			          } 
			          else
			          {
			             LoadWarnCnt++;
			             LoadWarnCnt++;
			          }
			          break;
			    default:break;
			}
		}         
    }
	
    row = cnt/0x04;         //计算需要几个字节来表示cnt路负载。
    if(cnt % 0x04)        //一路需要两个位来表示，所以字节数 = cnt/0x04 或者 cnt/0x04 + 1
    {                     //当路数为4的倍数时，为 cnt/4.  例如 4  8  12  16
        row++;              //当路数不为4的倍数时，为 cnt/4 + 1 例如 3(1) 7(2).等 
    }    
    for(i=0;i<row;i++)  pOldStatus[i] = pNewStatus[i];
}
/*******************************************************************************************
函数名：void sensorFault(UINT8 base,UINT8 faultNum,UINT8 cnt)
功能：检测传感器故障。若有故障则压入堆栈。
                      若故障消除，则直接出堆栈。
参数说明：UINT8 base -> 基位置，用于定位模块起始位置。例如仪表为0.
          UINT8 faultNum -> 基故障代号
          UINT8 cnt -> 此模块传感器路数。
*******************************************************************************************/
void ExSensorFault(UINT8 base,const UINT8 * pWarnByte,UINT8 cnt)
{
    UINT8 i;
    UINT8 warn;
    for(i=0;i<cnt;i++)
    {
    	if(pWarnByte[i]==0) continue;//add by L:上位机补0
         warn =  pWarnByte[i]&0x08; 
         if(warn==0)
         {
            LoadWarnCnt++;
            if((OldSensorData[i+base] == 0xfffe) && (InsideData[i+base] != 0xfffe))
            {
                pop_dis(LoadWarnCnt); 
            }
            else if((OldSensorData[i+base] != 0xfffe) && (InsideData[i+base] == 0xfffe))
            {
                push_dis(LoadWarnCnt);
            }
            OldSensorData[i+base] = InsideData[i+base];
         }

    
    }
}
/******************************************************************************************
函数名：void ModuleDetect(void)
功能：检测各模块是否丢失
******************************************************************************************/
void ModuleDetect(void) 
{
    UINT8 i;
    for(i=0;i<8;i++)
    {
        if(ModuleConfigTemp & (0x01 << i))
        {
            if(ModuleDetectFlag.Byte & (0x01 << i))
            {  
                ModuleDetectCnt[i] = 0x00;
                ModuleDetectFlag.Byte &= ~(0x01 << i);
            }
            if(ModuleDetectCnt[i] >= MODULE_DETECT_TIME_OV_NUM)
            {
                ModuleErrorFlag.Byte |= (0x01 << i);
                ModuleDetectCnt[i] = MODULE_DETECT_TIME_OV_NUM;
            }
            else       //在处理完
            {
                ModuleErrorFlag.Byte &= ~(0x01 << i);
            }
        }
    }
    for(i=0;i<8;i++) 
    {
            if(CanLoseDataFlag.Byte & (0x01 << i))
            {  
                CanLoseDataCnt[i] = 0x00;
                CanLoseDataFlag.Byte &= ~(0x01 << i);
            }
            if(CanLoseDataCnt[i] >= DMI_DETECT_TIME_OV_NUM)
            {
                CanLoseErrorFlag.Byte |= (0x01 << i);
                CanLoseDataCnt[i] = DMI_DETECT_TIME_OV_NUM;
            }
            else       //在处理完
            {
                CanLoseErrorFlag.Byte &= ~(0x01 << i);
            }

    }
}
/**********************************************************************************
funtion operation:
funtion call:
input parameter:void            
return:void
**********************************************************************************/
void  MesseageDeal(void)        
{
    PowerDeal();

	if(CAN2_RS_READ)   return;  

	if(listen_mode_flag==1) return;
	CanSendDeal();
}

/***********************************************

************************************************/
void PowerDeal(void)
{
   CAN_PACKET_T canSendFrame;
    if((SYS_SLEEP_LOAD != OLD_SYS_SLEEP_LOAD)||(SleepFlag==0))    //
    {
        if(!SYS_SLEEP_LOAD)
        {
            USR_s_OV.SleepCodeBuf = WAKE_UP_CODE;
            WakeUpInit();
            OLD_SYS_SLEEP_LOAD = SYS_SLEEP_LOAD;
            SleepDelayCnt = 0x00;
            
        }
        else 
        {
            USR_s_OV.SleepCodeBuf = SLEEP_CODE;
            if(SleepDelayCnt >= SLEEP_DELAY_TIME)
            {  
                canSendFrame.identifier=0x00007102;//CAN0_METER_MODE_ID;
                canSendFrame.byte_count = 8;
                canSendFrame.data[0] =0xaa;
                canSendFrame.data[1] =0xff;
                canSendFrame.data[2] =0xff;
                canSendFrame.data[3] =0xff;
                canSendFrame.data[4] =0xff;
                canSendFrame.data[5] =0xff;
                canSendFrame.data[6] =0xff;
                canSendFrame.data[7] =0xFF;
                SleepFlag = 1;

                DelayMs(20);
                CAN0_send(&canSendFrame);
                DelayMs(50);
                sleep_mode(); 

				///V_POWER_12V_H;//V_POWER_12V = 1;
				///V_POWER_V3_L;	//V_POWER_V3_ =0;
				///V_POWER_V1_L;	//V_POWER_V1 =0;   
				CAN0_RS_H;
				CAN1_RS_H;//CAN1_RS = 1;
				CAN2_RS_H;//CAN0_RS = 1;
				
				OLD_SYS_SLEEP_LOAD = SYS_SLEEP_LOAD;
				SleepDelayCnt = 0;
            }
        }
    }
    else
    {
        SleepDelayCnt = 0x00;
    }
}
/*****************************************

******************************************/
void WakeUpInit(void)
{   
#if 0
	CAN0CTL0_SLPRQ = 0;
#endif

	CAN2_RS_L;//CAN0_RS = 0;
	CAN1_RS_L;//CAN1_RS = 0; 
	RSleepFlag = 1;

	ClrSleepCnt =0;
}
/**********************************
 ***********************************/
void sleep_mode(void)
{
#if 0
    uint sleep_wait_time = 0;
    CAN0CTL0_WUPE = 1; /* Wake-Up Enable;must be set in front of entering sleep mode*/
    CAN0CTL0_SLPRQ = 1; /*request enter sleep mode*/
    while (!CAN0CTL1_SLPAK) {/*wait sleep mode complete*/
    sleep_wait_time++;
    if (sleep_wait_time >= 1000)
    break;
    }
    CAN0RIER_WUPIE = 1; /* Wake-up Interrupt Enable */
    CAN0RFLG_WUPIF = 1; /* Enable Wake-up Interrupt Flag */
#endif
}

/***********************************************
Funtion:Send CAN messge if need.
Notice:Because CAN net only have 3 buffer,So must send CAN message less than 3 in 1 time,
            else it will lose CAN data
************************************************/
void CanSendDeal(void)
{
    if (ID_require_flag==1) {
        ID_require_flag=0;
    } 
    else if (RainScratchChangeFlag==1) {
        RainScratchChangeFlag=0;
    } 
}

void  CopyData(UINT8 *pScr,UINT8 *pDst,UINT8 cnt) 
{
    UINT8 i;
    for(i=0;i<cnt;i++)
    {
        pDst[i] = pScr[i];
    }
}

void LoadStatusCopy(void) 
{
    //__DI();
    //CopyData(&USR_LOAD_OV.DashboardLoad.Data,&USR_s_OV.DashboardLoad.Data,1);
    CopyData(&USR_LOAD_OV.MiddleDriveLoad[0].Data,&USR_s_OV.MiddleDriveLoad[0].Data,DRIVE_LOAD_NUM);
    CopyData(&USR_LOAD_OV.FrontDriveLoad[0].Data,&USR_s_OV.FrontDriveLoad[0].Data,DRIVE_LOAD_NUM);
    CopyData(&USR_LOAD_OV.BackDriveLoad[0].Data,&USR_s_OV.BackDriveLoad[0].Data,DRIVE_LOAD_NUM);
    CopyData(&USR_LOAD_OV.TopDriveLoad[0].Data,&USR_s_OV.TopDriveLoad[0].Data,DRIVE_LOAD_NUM);
    CopyData(&USR_LOAD_OV.PowerLoadState[0].Data,&USR_s_OV.PowerLoadState[0].Data,POWER_LOAD_NUM);
    CopyData(&USR_LOAD_OV.PowerContorolLoadState[0].Data,&USR_s_OV.PowerContorolLoadState[0].Data,4); 
    //__EI();
    DisLoadWarnShow();
   
}

/******************************************
* @brief:负载报警屏蔽
********************************************/
 void DisLoadWarnShow(void) 
{
	UINT8 i,moudle,st,temp,loadSt;
	UINT16  datTemp;
	UINT8   row;
	UINT8   line;  

	for(i = 0 ;i < 4 ; i++) //modify by apo	i < 4  --> i < 5
	{
		datTemp = SysConfigData.disLoadWarn[i];
		moudle = (UINT8)(datTemp >> 8);//b15-8
		st  = (UINT8)(datTemp >> 6);
		st &= 0x03;//b7-6
		temp =  (UINT8)(datTemp & 0x3f);
		row  =  temp / 4;
		line =  temp % 4; 
		line *=2;
		switch(moudle) 
		{                               
		  	  case DASHBOARD_NODE_ID:    
		                               loadSt =  (USR_s_OV.DashboardLoad[0].Data>>line) & 0x03;
		  	                           if(st==loadSt) USR_s_OV.DashboardLoad[0].Data&=(~(0x03<<line));  break;
		      case FRONT_DRIVER_NODE_ID:  
		                               loadSt =  (USR_s_OV.FrontDriveLoad[row].Data>>line) & 0x03;
		  	                           if(st==loadSt) USR_s_OV.FrontDriveLoad[row].Data&=(~(0x03<<line));  break;
		      case TOP_DRIVER_NODE_ID:   
		           	                   loadSt =  (USR_s_OV.TopDriveLoad[row].Data>>line) & 0x03;
		  	                           if(st==loadSt) USR_s_OV.TopDriveLoad[row].Data&=(~(0x03<<line));  break;
		      case BACK_DRIVER_NODE_ID:  
		                               loadSt =  (USR_s_OV.BackDriveLoad[row].Data>>line) & 0x03;
		  	                           if(st==loadSt) USR_s_OV.BackDriveLoad[row].Data&=(~(0x03<<line));  break;
		     // case MIDDLE_DRIVER_NODE_ID: 
		                              //  loadSt =  (USR_s_OV.MiddleDriveLoad[row].Data>>line) & 0x03;
		  	                          //  if(st==loadSt) USR_s_OV.MiddleDriveLoad[row].Data&=(~(0x03<<line));  break;
		      case POWER_MODULE_NODE_ID:  
		                                loadSt =  (USR_s_OV.PowerLoadState[row].Data>>line) & 0x03;
		  	                            if(st==loadSt) USR_s_OV.PowerLoadState[row].Data&=(~(0x03<<line));  break;
		  	  case POWER_CONTOROL_NODE_ID:
		  	                            loadSt =  (USR_s_OV.PowerContorolLoadState[row].Data>>line) & 0x03;
		  	                            if(st==loadSt) USR_s_OV.PowerContorolLoadState[row].Data&=(~(0x03<<line));  break;
		      default:break;
		}
	}
}

void SendCan0Message(void)
{
    unsigned char i;       
    
    UINT32 dataTemp;
    CAN_PACKET_T SendFrame;
    static UINT8 _200MsCnt=0,_200MsSendF;

#if 0 
   	canBusyF=CAN0TFLG;
  	canBusyF&=0x7;
  	if(canBusyF!=7)		return;		// CAN发送缓存满
  	if(CAN0_RS==1)    return;
#endif

	if(CAN1_RS_READ)   return;
	if(SleepFlag==1)  return;
	
    if(_50MS_F) 
	{
		for(i=0;i<8;i++) SendFrame.data[i] = 0xff;
		switch(_200MsCnt) 
		{
			case 0:       
				SendFrame.identifier=0x00002002;
				SendFrame.byte_count = 8;
				SendFrame.data[0] =USR_s_OV.DashboardKey[0].Data;
				SendFrame.data[1] =USR_s_OV.DashboardKey[1].Data;
				SendFrame.data[2] =USR_s_OV.DashboardKey[2].Data;
				SendFrame.data[3] =USR_s_OV.DashboardKey[3].Data;
				SendFrame.data[4] =USR_s_OV.DashboardKey[4].Data;
				SendFrame.data[5] =USR_s_OV.DashboardKey[5].Data;
				SendFrame.data[6] =USR_s_OV.DashboardKey[6].Data;
				_200MsSendF = 1;_200MsCnt++; 
				break;
			case 1:       
				SendFrame.identifier = 0x1fe04002;
				SendFrame.byte_count = 8;
				SendFrame.data[0] =USR_s_OV.DashboardLoad[0].Data;
				SendFrame.data[1] = 0xFF;//USR_s_OV.DashboardLoad[1].Data;
				SendFrame.data[2] = 0xFF;//USR_s_OV.DashboardLoad[2].Data;
				SendFrame.data[3] = 0xFF;
				SendFrame.data[4] = 0xFF;
				SendFrame.data[5] = 0xFF;
				SendFrame.data[6] =LogicOutPut[10].Load;
				SendFrame.data[7] =LogicOutPut[11].Load;
				_200MsSendF = 1;_200MsCnt++; 
				break;             //1fe07302
			case 2:       
				SendFrame.identifier=0x1fe07302;
				dataTemp =   USR_s_DeviceConfigData.TotalDistance;
				dataTemp /=1000;
				SendFrame.byte_count = 8;
				SendFrame.data[0] = (UINT8)(dataTemp>>24);
				SendFrame.data[1] = (UINT8)(dataTemp>>16);
				SendFrame.data[2] = (UINT8)(dataTemp>>8);
				SendFrame.data[3] = (UINT8)(dataTemp);

				SendFrame.data[4] = (UINT8)(USR_s_OV.HundredKilometerFuelConsume);
				SendFrame.data[5] = (UINT8)(USR_s_OV.HundredKilometerFuelConsume>>8);//板温
				SendFrame.data[6] = (UINT8)(USR_s_OV.HundredKilometerFuelConsume>>16);//电池电压
				SendFrame.data[7] = (UINT8)(USR_s_OV.HundredKilometerFuelConsume>>24);
				_200MsSendF = 1;_200MsCnt++; break;
			case 3:       
				SendFrame.identifier=0x1fe07402;
				SendFrame.byte_count = 8;
				SendFrame.data[0] =(UINT8)(USR_s_OV.totalConsume);
				SendFrame.data[1] =(UINT8)(USR_s_OV.totalConsume>>8);
				SendFrame.data[2] =(UINT8)(USR_s_OV.totalConsume>>16);
				SendFrame.data[3] =(UINT8)(USR_s_OV.totalConsume>>24);

				SendFrame.data[4] =(UINT8)(InsideData[7]);    //车速
				SendFrame.data[5] =(UINT8)(InsideData[7]>>8);
				SendFrame.data[6] =0xff;
				_200MsSendF = 1;_200MsCnt++; break;   

			case 4:       
				SendFrame.identifier=0x00007002;
				SendFrame.byte_count = 8;
				SendFrame.data[0] =USR_s_OV.rain_scr;

				SendFrame.data[2] =0;
				SendFrame.data[3] =0;
				SendFrame.data[4] =0;
				SendFrame.data[5] =0;
				SendFrame.data[6] =0;
				SendFrame.data[7] =0;


				// SendFrame.data[2] =0xff;
				//SendFrame.data[3] =0xff;
				// SendFrame.data[4] =0xff;
				// SendFrame.data[5] =0xff;
				// SendFrame.data[6] =0xff;
				_200MsSendF = 1;_200MsCnt++; 
				break;       
			case 5:       
				SendFrame.identifier=0x00003002;
				SendFrame.byte_count = 8;
				SendFrame.data[0] =USR_s_OV.DashboardVirtualKey[0].Data;
				SendFrame.data[1] =USR_s_OV.DashboardVirtualKey[1].Data;
				SendFrame.data[2] =USR_s_OV.DashboardVirtualKey[2].Data;
				SendFrame.data[3] =USR_s_OV.DashboardVirtualKey[3].Data;
				SendFrame.data[4] =USR_s_OV.DashboardVirtualKey[4].Data;
				SendFrame.data[5] =USR_s_OV.DashboardVirtualKey[5].Data;
				SendFrame.data[6] =USR_s_OV.DashboardVirtualKey[6].Data;
				SendFrame.data[7] =USR_s_OV.DashboardVirtualKey[7].Data;
				_200MsSendF = 1;
				_50MS_F =  0;
				_200MsCnt = 0;
				break;

		}

		if(_200MsSendF)
		{
			_200MsSendF = 0;
			CAN0_send(&SendFrame);
		}
	} 
}

/*******************************************************************************************
函数名：void initSensorData(void)
功能：初始化传感器值
*******************************************************************************************/
void initSensorData(void)
{
    UINT8 i;
    for(i=0;i<END_INSIDE_OFFSET1;i++)
    {
        InsideData[i] = 0x00;
        OldSensorData[i] = 0x00; 
    }
}

