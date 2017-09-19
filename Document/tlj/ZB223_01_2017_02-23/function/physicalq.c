#define 	PHYSICALQ_C
#include	"physicalq.h"
//#pragma CONST_SEG     __GPAGE_SEG        MYSEC_CONFIG_DEFVALUE
//#pragma CONST_SEG __GPAGE_SEG CONFIG_DATA_SDO

extern St_CaptureTime   CapturePwm0,CapturePwm1;

/**********************************************************************************
内部状态码配置信息。
1、启动状态
2、汽车行驶状态
**********************************************************************************/
 UINT8 InsidePhysStatus[8];
//#pragma CONST_SEG __GPAGE_SEG    CONST_DATA_ROM
const  UINT8 VersionClear[12] = {"223-00      "};

/***********************************************************************************
内部32位数据配置。
***********************************************************************************/
extern  UINT32 InsidePhys32Bit[4];
extern  INSIDE_PHYS_32BIT_STRUCT InsidePhys32BitAttr[4] = 
{ 
    {0x00,0x11},
    {0x00,0x11},
    {0x00,0x11},
    {0x00,0x11}    
};
/**********************************************************************************
内部Ascii码：仪表版本、前驱版本、后驱版本、中驱版本、顶驱版本、配电版本
**********************************************************************************/
extern UINT8 InsideAscii[10][11] = 
{ 
    {"        "},
    {"        "},
    {"        "},
    {"        "},
    {"        "},
    {"        "},
    {"        "},
    {"        "},
    {"        "},
    {"        "}
};
//#pragma CODE_SEG  USER_ROM
/*******************************************************************************************
函数名：void getVersion(void)
*******************************************************************************************/
void ClearVersion(void)
{
   UINT8 i; 
   for(i=0;i<10;i++) 
     dataCopy(&InsideAscii[i][0], (UINT8 *)VersionClear,8);   
}

void SetDashboardVersion(void) 
{
 dataCopy(USR_s_OV.DashboardVersion,(UINT8 *)Version,8);
 dataCopy(USR_s_OV.frontDriverVersion,(UINT8 *)Version,8);
 dataCopy(USR_s_OV.backDriverVersion,(UINT8 *)Version,8);
 dataCopy(USR_s_OV.topDriverVersion,(UINT8 *)Version,8);
 dataCopy(USR_s_OV.middleDriverVersion,(UINT8 *)Version,8);
 dataCopy(USR_s_OV.powerVersion,(UINT8 *)Version,8);
 dataCopy(USR_s_OV.PowerContorolVersion,(UINT8 *)Version,8);
 
 CapturePwm0.captureCount =0;
 CapturePwm0.captureToFre =0;
 CapturePwm0.highTime =0;
 CapturePwm0.nextCaptureTime =0;
 CapturePwm0.preCaptureTime =0;
 CapturePwm0.upflag = 0;
 
 CapturePwm1.captureCount =0;
 CapturePwm1.captureToFre =0;
 CapturePwm1.highTime =0;
 CapturePwm1.nextCaptureTime =0;
 CapturePwm1.preCaptureTime =0;
 CapturePwm1.upflag = 0;
}
/*******************************************************************************************
函数名ompareData(UINT8 *src,UINT8 *dst,UINT8 lenght) 
*******************************************************************************************/
UINT8 CompareData(UINT8 *src,UINT8 *dst,UINT8 lenght) 
{
   UINT8 i,status;
   status = 0;
   for(i=0;i<lenght;i++) 
   {
    if(src[i]!=dst[i]) 
    {
     status = 1; 
     dst[i] = src[i]; 
    }
   }
  return status;
}

/*******************************************************************************************
函数名：void insidePhysStatus(void)
用于获取内部状态变量。
*******************************************************************************************/
void insidePhysStatus(void)
{
    InsidePhysStatus[0] = StartStatus;
    InsidePhysStatus[1] = 0x0c;
    InsidePhysStatus[2] = 0x0c;
    InsidePhysStatus[3] = 0x0c;
    InsidePhysStatus[4] = 0x0c;
    InsidePhysStatus[5] = 0x0c;
    InsidePhysStatus[6] = 0x0c;
    InsidePhysStatus[7] = 0x0c;
}
/*******************************************************************************************
函数名：void insidePhys32Bit(void)
*******************************************************************************************/
void insidePhys32Bit(void)
{ 
    InsidePhys32Bit[0] = USR_s_DeviceConfigData.RelativeDistance/1000;
    InsidePhys32Bit[1] = USR_s_DeviceConfigData.TotalDistance/1000;
    InsidePhys32Bit[2] = 0xffffffff;
    InsidePhys32Bit[3] = 0xffffffff;
}

/*******************************************************************************************
函数名：UINT8 getCarSpeedChannel(UINT8 src)
功能：获取车速通道上的车速值
                                 0        1     2     3     4     5      6
参数说明：UINT8 src -> 通道源。J1939_8，仪表，前驱，后驱，中驱，顶驱。J1939_16
*******************************************************************************************/
UINT8 getCarSpeedChannel(UINT8 src)
{   
	UINT32 data,bitValue;
	UINT8 J1939Type,dot;
	UINT16 dataType;
	const UINT32 expend[6] = {1,10,100,1000,10000,100000};     
	switch(src)
	{
		case 0:
			{
				// return J1939CharData[0];
			}
		case 1:
			{
				return   (UINT8)(CarSpeedFromDashboard/10);
			}
		case 2: 
			{
				return (UINT8)USR_s_OV.CarSpeedFromDriver;
			}
		case 3:
			{
				J1939Type = J1939WordDataConfig[SysConfigData.speedChanelIndex].attribute&0x0f;//b0-3 
				dataType  = J1939WordDataConfig[SysConfigData.speedChanelIndex].attribute&0x0f00; //b8-11
				bitValue  = J1939WordDataConfig[SysConfigData.speedChanelIndex].bitValue & 0xffffff; 
				dot       = J1939WordDataConfig[SysConfigData.speedChanelIndex].bitValue >> 28; 
				switch(J1939Type) 
				{           
					case J1939_Char_Type  : data=StorageBuffer[J1939_OFFSET_BYTE +SysConfigData.speedChanelIndex]; break;          
					case J1939_Int_Type   : data=*(UINT16 *)(&StorageBuffer[J1939_OFFSET_BYTE+2*SysConfigData.speedChanelIndex-J1939_STATUS_NUMBER-J1939_CHAR_NUMBER]);break;                             
					default:break;
				};

				switch(dataType)
				{
					case J1939_SIGNED_DATA_TYPE:
						switch(J1939Type) 
						{
							case J1939_Char_Type   :  if(data>0x7f){ data = 0x100- data; }break;               
							case J1939_Int_Type    :  if(data>0x7fff){ data = 0x10000- data;}break;
							// case J1939_Three_Type  :  if(data>0x7fffff){ data = 0x1000000- data; }break;  
							//  case J1939_Word_Type   :  if(data>0x7fffffff){ data = ((0xffffffff- data)+1); }break;               
						};
					break;
				}  

				data = data * bitValue;    
				if(J1939WordDataConfig[SysConfigData.speedChanelIndex].attribute & J1939_OFFSET_TYPE)
				{
					data += J1939WordDataConfig[SysConfigData.speedChanelIndex].offset ;
				} 
				else 
				{
					if(data>J1939WordDataConfig[SysConfigData.speedChanelIndex].offset) 
						data-=J1939WordDataConfig[SysConfigData.speedChanelIndex].offset;
					else data = 0;
				} 
				data += expend[dot]/2; //四舍五入
				data /= expend[dot]; 
				return  (UINT8)data;
				break;        
			}
		default:
			{
				return 0x00;
			}
	} 
}
UINT16 getCarEngSpeedChannel(UINT8 eng)
{   
   UINT32 data,bitValue;
   UINT8 J1939Type,dot;
   const UINT16 expend[4] = {1,10,100,1000};
    switch(eng)
    {
        case 0:
        {
           // return J1939CharData[0];
          return 0; 
        }
        case 1:
        {
			//return   (CarSpeedFromDashboard*5);  //测试
             return CarEngSpeedFromDashboard*10;  
           // return 0;
        }
        case 2: 
        {
            return USR_s_OV.engSpeedFromDriver;
        }
        case 3:
			{
				J1939Type  = J1939WordDataConfig[SysConfigData.engSpeedChanelIndex].attribute&0x0f; 
				bitValue   = J1939WordDataConfig[SysConfigData.engSpeedChanelIndex].bitValue & 0xffffff; 
				dot        = J1939WordDataConfig[SysConfigData.engSpeedChanelIndex].bitValue >> 28; 
				switch(J1939Type) 
				{           
				case J1939_Char_Type   :  data=StorageBuffer[J1939_OFFSET_BYTE+SysConfigData.engSpeedChanelIndex]; break;          
				case J1939_Int_Type    :  data=*(UINT16 *)(&StorageBuffer[J1939_OFFSET_BYTE+2*SysConfigData.engSpeedChanelIndex-J1939_STATUS_NUMBER-J1939_CHAR_NUMBER]);break;                             
				default:break;
				};
				data = data* bitValue;    
				if(J1939WordDataConfig[SysConfigData.engSpeedChanelIndex].attribute&J1939_OFFSET_TYPE)
				{
				data +=  J1939WordDataConfig[SysConfigData.engSpeedChanelIndex].offset ;
				} 
				else 
				{
				if(data>J1939WordDataConfig[SysConfigData.engSpeedChanelIndex].offset)  data-=J1939WordDataConfig[SysConfigData.engSpeedChanelIndex].offset;
				 else data = 0;
				} 
				data /= expend[dot]; 
				return (UINT16)data;
				break;        
			}
        default:
        {
            return 0x00;
        }
    } 
}

