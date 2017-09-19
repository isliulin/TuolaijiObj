#define		STEPDEAL_C
#include	"StepDeal.h"
#include "motorPatch.h"
#include "IS31FL3733_I2C.h" 
volatile uchar 	resetback0;
uint   	delay1s;	//ap:����Լ������ʱ����
volatile	uchar		carspeed;

uchar lastLedLeftState=0,lastLedRightState=0;
uchar oldLedLeftState=0,oldLedRightState=0;
uchar SetLedLeftState=0,SetLedRightState=0;

////////////////////////////////////////////////////////////////////////////////
//��ʱ��������


/*volatile*/  uint  	MotorFinalStep[MOTOR_NUM],MotorCurStep[MOTOR_NUM];
/*volatile*/  uint  	MotorMaxStep[MOTOR_NUM],MotorMinStep[MOTOR_NUM];

//ByteTo1bit meter;
StructFilter StepDataBuf[6];

////////////////////////////////////////////////////////////////////////////////

/*****************************************************************
������:void doOilSensorData(void)
*****************************************************************/
UINT16 filterGear3(UINT8 motorNo,UINT16 newData,UINT16 oldData)
{
    UINT16 temp;
    UINT16 stepData[MAX_NUM];
    //saveAData(StepDataBuf[motorNo],MAX_NUM,newData);
    moveBackAData(StepDataBuf[motorNo].SensorAd, MAX_NUM);
		StepDataBuf[motorNo].SensorAd[0] = newData;
    dataCopyUint(stepData, StepDataBuf[motorNo].SensorAd,MAX_NUM);
    sort(stepData, MAX_NUM);
    temp = average(&stepData[EFFECT_START],EFFECT_CNT);
    newData = adjustNewData(oldData,temp);
    return newData;
}
/*****************************************************************
������:void dataCopy(uchar *pDst, uchar *pSrc,uchar cnt)
*****************************************************************/
void dataCopyUint(UINT16 *pDst, UINT16 *pScr,UINT8 cnt)
{
    uchar i;
    for(i=0;i<cnt;i++)
    {
        *pDst++ = *pScr++;
    }
}
/*****************************************************************
������:void saveAData(uchar *pData,uchar data)
����:�洢һ������
*****************************************************************/
void saveAData(UINT16 *pData,UINT8 cnt,UINT16 data)
{
    moveBackAData(pData, cnt);
		pData[0] = data;
}
/*****************************************************************
������:void moveBackAData(uchar *pData, uchar cnt)
������:����ƶ�һ������
*****************************************************************/
void moveBackAData(UINT16 *pData, UINT8 cnt)
{
    char i;
		for(i=cnt-2;i!=0;i--)
		{
        pData[i+1] = pData[i];
		}
		pData[1] = pData[0];
}
/******************************************************************
��������void sorting(uchar *pData, uchar cnt)
����:��pData�ڵ���������                                                                 
����˵����uchar *pData -> ����С���������
          uchar cnt -> ���ݳ���
******************************************************************/
void sort(UINT16 *pData, UINT8 cnt)
{
    unsigned char i,j;
    unsigned int temp;
    for(i=0;i<cnt;i++)
    {
       for(j=i+1;j<cnt;j++)
       {
          if(pData[i] > pData[j])
          {
             temp = pData[j];                                                         
             pData[j] = pData[i];
             pData[i] = temp; 
          }
       }
    }                   
}
/*****************************************************************
������:uchar average(unsigned char *pData, unsigned char cnt)
*****************************************************************/
uint average(uint *pData, uchar cnt)
{
    uint sum = 0x00;
    uchar i;
    for(i=0;i<cnt;i++)
    {
        sum += *pData++;
    }
    return sum/cnt;
}
/*****************************************************************
������:uchar adjustNewData(uchar oldData,uchar NewData)
*****************************************************************/
uint adjustNewData(uint oldData,uint newData)
{
    uint temp;
    uint adjust;
    if(oldData > newData)
    {
        temp = oldData - newData;
    }
    else
    {
        temp = newData - oldData;
    }
    if(temp >= 400)
    {
        adjust = 20;
    }
    else if(temp >= 300 && temp < 400)
    {
        adjust = 10;
    }
    else if(temp >= 200 && temp < 300)
    {
        adjust = 7;      //10
    }
    else if(temp >= 100 && temp < 200)
    {
        adjust = 5;     //5
    }
    else if(temp >= 50 && temp <100)
    {
        adjust = 2;      //2       
    }
    else
    {
        adjust = 1;
    }
    if(oldData > newData)
    {
        if(oldData < adjust)
        {
            newData = 0x00;
        }
        else
        {  
            newData = oldData - adjust;
        }
    }
    else
    {
        newData = oldData + adjust;
        if(newData > 1080)
        {
            newData = 1080;
        }
    }
    return newData;    
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
UINT8 SensorDataToRoundBuf(UINT16 *pRoundBuf,UINT16 data,UINT8 index,UINT8 size)
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
UINT16 DataWithoutMaxAndMin(UINT16 *pRoundBuf,UINT8 size)
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
��������Bubble_Sort(UINT16 *pData, UINT8 cnt)
���ܣ�  ð������
����˵����UINT16 *pData -> ���λ�����
          UINT8 size -> ���λ�������С
          ����С���������
����ֵ:
*****************************************************************************************/
void Bubble_Sort(UINT16 *pData, UINT8 cnt)
{
    unsigned char i,j;
    unsigned int temp;
    for(i=0;i<cnt;i++)
    {
       for(j=i+1;j<cnt;j++)
       {
          if(pData[i] > pData[j])
          {
             temp = pData[j];                                                         
             pData[j] = pData[i];
             pData[i] = temp; 
          }
       }
    }                   
}
/*****************************************************************************************
��������GetBufferMedianVal(UINT16 *pData, UINT8 cnt)
���ܣ�    ��ȡ�������м�ֵ
����˵����UINT16 *pData -> ���λ�����
          UINT8 size -> ���λ�������С
          ����С���������
��ע    ���ú�����Bubble_Sort�����        
����ֵ:
*****************************************************************************************/
UINT16 GetBufferMedianVal(UINT16 *pData, UINT8 cnt) 
{
   return pData[(cnt-1)/2];
}
/*****************************************************************************************
��������void filterMedianAverage(UINT16 volatageAdValue)
���ܣ�  ��ֵƽ���˲�
����˵����
*****************************************************************************************/
UINT16 filterMedianAverage(StructFilter* stSensorBuffer ,UINT16 AdValue)
{     
    stSensorBuffer->Sensorindex = SensorDataToRoundBuf(&stSensorBuffer->SensorAd[0],AdValue,stSensorBuffer->Sensorindex,FilterBufferMax);
    return DataWithoutMaxAndMin(&stSensorBuffer->SensorAd[0],FilterBufferMax);    
}

/*****************************************************************************************
��������void filterMedianAverage(UINT16 volatageAdValue)
���ܣ�  ��ֵ�˲�
����˵����
*****************************************************************************************/
UINT16 filterMedian(StructFilter* stSensorBuffer ,UINT16 AdValue)
{     
    stSensorBuffer->Sensorindex = SensorDataToRoundBuf(&stSensorBuffer->SensorAd[0],AdValue,stSensorBuffer->Sensorindex,FilterBufferMax);
    Bubble_Sort(&stSensorBuffer->SensorAd[0],FilterBufferMax);
    return GetBufferMedianVal(&stSensorBuffer->SensorAd[0],FilterBufferMax);    
}



/*********************************************************************************************
��������UINT16 getDisParameter(UINT8 type,UINT8 index)
���ܣ���ȡ��ʾ����ָ��ָʾ������
����˵����UINT8 type -> ��������
          UINT8 index -> ��������
����ֵ����ʾ����ָ��ָʾ�Ĳ���ֵ��
*********************************************************************************************/
UINT32 getDisParameter(UINT8 type,UINT16 index, UINT8 *rDot)
{
    UINT32 data = 0x00;
    UINT32 bitValue;
    UINT16 J1939Type;
    UINT16 dataType;
    UINT8  dot; 
    UINT32 offset; //UINT16 expend[4] = {1,10,100,1000};
    
    switch(type)
    {
        case STEP_INSIDE_16BIT_TYPE:  
		{
			data = (INT32)InsideData[index];
			if(index<10) 
			{  
				if(data!=0xfffe) 
				{
					data = data * (SENSOR_DATA_CONFIG[index].bitValue & SENSOR_BIT_VALUE_MASK); 
					dot = SENSOR_DATA_CONFIG[index].bitValue >> SENSOR_DOT_GRESSION;      //2 2
					*rDot = dot;  
					offset = (UINT32)SENSOR_DATA_CONFIG[index].offset;
					if(data>offset) 
					{ 
						data = data - offset;   
					} 
					else 
					{
						data = 0 ;                 
					} 
					// data /= expend[dot];  
				}
				else data = 0;  
			} 
			else 
			{
				if(data!=0xfffe) 
				{
					data=data * (SENSOR_DATA_CONFIG[index].bitValue & SENSOR_BIT_VALUE_MASK); 
					dot = SENSOR_DATA_CONFIG[index].bitValue >> SENSOR_DOT_GRESSION;      //2 2
					*rDot = dot;  
					offset = (UINT32)SENSOR_DATA_CONFIG[index].offset;
					if(data>offset) 
					{ 
					data = data - offset;   
					} 
					else 
					{
					data = 0 ;                 
					} 
					// data /= expend[dot];  
				}
				else data = 0;  
			}
			break;            
		}  
        case STEP_J1939_TYPE:  
		{
			J1939Type  = (UINT8)(J1939WordDataConfig[index].attribute&0x0f); 
			dataType   =  J1939WordDataConfig[index].attribute&0x0f00;
			bitValue   = J1939WordDataConfig[index].bitValue & 0xffffff; 
			dot        = J1939WordDataConfig[index].bitValue >> 28; 
			*rDot = dot; 
			switch(J1939Type) 
			{           
				case J1939_Char_Type   :  data=StorageBuffer[J1939_OFFSET_BYTE+index]; break;          
				case J1939_Int_Type    :  data=*(UINT16 *)(&StorageBuffer[J1939_OFFSET_BYTE+2*index-J1939_STATUS_NUMBER-J1939_CHAR_NUMBER]);break;                
				//case J1939_Word_Type   :  data=*(UINT32 *)(&StorageBuffer[J1939_OFFSET_BYTE + 4*dataIndex-3*J1939_STATUS_NUMBER - 3*J1939_CHAR_NUMBER-2*J1939_INT_NUMBER]);break;               
				default:break;
			};

			switch(dataType)
			{
				case J1939_OFFSET_DATA_TYPE:
					data = data* bitValue; 
					if(data>J1939WordDataConfig[index].offset)  data = data - J1939WordDataConfig[index].offset;
					else  data = 0;
					break;
				case J1939_SIGNED_DATA_TYPE:
					switch(J1939Type) 
					{
						case J1939_Char_Type   :  if(data>0x7f){ data = 0x100- data; }break;               
						case J1939_Int_Type    :  if(data>0x7fff){ data = 0x10000- data;}break;           
					};

					data = data* bitValue; 
					if(J1939WordDataConfig[index].offset) 
					{
						if(data!=0) 
						{
						if(data>J1939WordDataConfig[index].offset)  data = data - J1939WordDataConfig[index].offset;
						else data = 0;
						}
					}       
					break;
				case J1939_ABSOLUTE_VALUE:
					data = data* bitValue; 
					if(J1939WordDataConfig[index].offset) 
					{
					if(data!=0) 
					{
					if(data>J1939WordDataConfig[index].offset)  data = data - J1939WordDataConfig[index].offset;
					else   data =  J1939WordDataConfig[index].offset - data;
					}
					}  
					break;
				default:
					data = data* bitValue; 
					if(data>J1939WordDataConfig[index].offset)  data = data - J1939WordDataConfig[index].offset;
					else  data = 0;
					break;
			} 
			break;        
		}
        default:break;
        
    } 
    return data; 
}
/*******************************************************************************************
��������UINT16 physicalToStep(UINT8 motorNum,UINT)
���ܣ��Ѹ�������ת��Ϊ�������
����˵����UINT8 motorNum -> �����
          UINT16 data -> ʵ���������ֵ
����ֵ��ʵ�ʵ������
*******************************************************************************************/
UINT16 physicalToStep(UINT8 motorNo,UINT32 physical,UINT32 expend)
{

/*
typedef struct
{
  //  UINT16
    UINT8   type;       //��������  
    UINT16  Index;      //����������
    UINT16  rang;       //���̣����磺��������Ϊ140��ת������Ϊ3000
                       //            ��������Ϊ100������100����
                       //            ��ѹ����Ϊ100������10����
    UINT16 maxStep;    //����ܲ�����1��2��3��4�ŵ��Ϊ1080
                       //            5��6�ŵ��Ϊ2900
    UINT8 filterGear;
    UINT16 minScale;      //ƫ���� ����̶ȵ�ֵΪ���١����磺ˮ��Ϊ40
                          //����Ϊ0
    UINT8 KeyIndex;   
}MOTO_STEP_STRUCT;


*/
    UINT32 realStep;
    UINT32 data = physical;
    UINT32 maxStep = MotorStepPrm[motorNo].maxStep;//�����
    UINT32 rang = MotorStepPrm[motorNo].rang * expend;	//����
    UINT32 minScale = MotorStepPrm[motorNo].minScale * expend;;//����ƫ��

	if((motorNo == PRES) || (motorNo == PRES2))
	{
	      minScale =0;//û��ƫ������������
		data += (rang-minScale)/120;//(rang-minScale)/20-(rang-minScale)/120); //��ѹ�Ƶ���������
	}
	
	if(data > rang)
    {
        return (UINT16)maxStep;
    } 
    else if(data > (minScale))
    {  
        realStep = (maxStep * (data -minScale)) / (rang-minScale);// �޸�(rang - minScale);
    }
    else
    {
        realStep = 0x00;       
    }
    return (UINT16)realStep;
}
/*******************************************************************************************
��������void doNo1Motor(UINT16 step)
���ܣ������һ�ŵ��
����˵����UINT16 step -> ����
����ֵ����
ע������ �Ҵ��ϵ��� ����� �ֱ�Ϊ 0��1��2��3��4��5��motorNoֻ��ȡ��Щ����
*******************************************************************************************/
extern	unsigned int speedBase,speedBios,speedDealFlag;

void doMotor(UINT8 motorNo)
{
    UINT16 step;
    UINT32 data;
    UINT8  dot;
   // UINT8  warmF;

    const UINT32 expend[6] = {1,10,100,1000,10000,100000};
    dot=0;
    data = getDisParameter(MotorStepPrm[motorNo].type,MotorStepPrm[motorNo].Index,&dot);
    //data /= expend[dot]; 
    #if 0
    if(motorNo==4) //����
    {
		if(MotorStepPrm[motorNo].type == STEP_J1939_TYPE) 
		{
			data *=100;
			data/=98;
		}
		if(data)data++;
    }
	#endif
   
    if(data != 0xfffe)
    {  
    /*
    	 //ap: ��ֵLED ����
       if(motorNo >= 4)
        {  
            if(motorNo == 4)  
            {
            
            	//L: ��==4�����ƽ���
				warmF = LogicOutPut[6].Load;
	            warmF >>= 4;
	            warmF &= 0X01;
				
                if(warmF)
                {  
                    ///RIGHT_WARMING_LED = 1;
                }
                else
                {
                    ///RIGHT_WARMING_LED = 0;
                }
            }
            if(motorNo == 5)
            {
				warmF = LogicOutPut[6].Load;
				warmF>>=5;
				warmF&=0X01;
                if(warmF)
                {  
                    ///LEFT_WARMING_LED = 1;
                }
                else
                {
                    ///LEFT_WARMING_LED = 0;
                }                
            } 
        } 
	  */ 
	
		
        step = physicalToStep(motorNo,data,expend[dot]);
        switch(MotorStepPrm[motorNo].filterGear)     
		{
			case GEAR_1:    //��ֵƽ��
				StepDataBuf[motorNo].SensorCnt++;
				if( StepDataBuf[motorNo].SensorCnt>3) 
				{
					StepDataBuf[motorNo].SensorCnt = 0;
					StepDataBuf[motorNo].SensorVal = filterMedianAverage(&StepDataBuf[motorNo] ,step);
				}
				step = StepDataBuf[motorNo].SensorVal;
				break;
			case GEAR_2:   //��λֵ
				StepDataBuf[motorNo].SensorVal = filterMedian(&StepDataBuf[motorNo] ,step);
				step = StepDataBuf[motorNo].SensorVal;       
				break;
			case GEAR_3:
			{

				if(StepDataBuf[motorNo].SensorCnt<50) 
				{
					StepDataBuf[motorNo].SensorCnt++; 
					StepDataBuf[motorNo].SensorVal= filterGear3(motorNo,step,StepDataBuf[motorNo].SensorVal); 
					step = StepDataBuf[motorNo].SensorVal;
				}
				else 
				{
					if(SECOND_F)
					{  
						SECOND_F = 0;
						StepDataBuf[motorNo].SensorVal= filterGear3(motorNo,step,StepDataBuf[motorNo].SensorVal); 
					}
					step = StepDataBuf[motorNo].SensorVal;
				}
				break;
			}
			default:break;
		}  
    }
    else
    {
        step = 0x00;
    }
	if(motorNo == Carspeed)	{
		//step = carSpeedFilter(step);
		if(speedDealFlag){
			speedBase = getCarSpeedTimes(step);
			speedDealFlag = 0;
		}
		
		
	}
    motor_move(motorNo,step); 
}

/******************************************************************************************

******************************************************************************************/
void	StepDeal(void)
{
	static UINT8 motorDly=0;
	unsigned char	i;	                              
	if(!ON_LOAD)
	{	//ap: ON����
		if(SysConfigData.post==0x01)        Sys_Post_Flag = 1; //�����Լ�
		
		//ledShow[4].Byte = 0;//H
		//ledShow[6].Byte= 0;
		//ledShow[5].Byte = 0;//H
		//ledShow[7].Byte= 0;
		motorDly =0;
		
		if(resetback0==0x00)
		{
			if(motor_rest_flag) 
			{
				motor_rest_flag = 0;
				back0_motor_off();
				for(i = 0;i < MOTOR_NUM; i++) StepDataBuf[i].SensorCnt = 0;
			}                   
		}
		return;
	} 
	else 
	{
		motor_rest_flag = 1;
	}
	
	motorDly++;//L:����������ʱ
	if(motorDly>15)
	{
		motorDly = 20;
		if(resetback0==0x00)
		{
			for(i = 0; i < MOTOR_NUM; i++)
			{  
				doMotor(i);  
			} 
		}
	}
}
/****************************************************************************************
* @brief		
* @parameter		channel
*					position:Ŀ��λ��
*****************************************************************************************/
unsigned char check_pos(unsigned char channel,unsigned int position)
{
    unsigned char flag;
    uint a,b;

	if((channel == PRES)||(channel == PRES2)){
		return OK;
	}
	//L:MotorFinalStep�ϴ�Ŀ��λ��
    if(position == MotorFinalStep[channel])
       flag = STEPNOT;
    else if(position > MotorMaxStep[channel])
    {
		//return STEPNOT;//2016/5/9 ,L:
		a = MotorFinalStep[channel];
        b = MotorMaxStep[channel];
        if(a == b)
        {
            flag = STEPNOT;
        }
        else
        {
            flag = STEPHIG;
        }
    }
    else if(position < MotorMinStep[channel])
    {
         a = MotorFinalStep[channel];
        b = MotorMinStep[channel];
        if(a == b)
        {
            flag = STEPNOT;
        }
        else
        {
            flag = STEPLOW;
        }
    }
    else
    {
        flag = OK;
    }   
    return flag;
}
/****************************************************************************************
@��������       : char motor_move(char channel,unsigned int position)
@����           : char channel,unsigned int position
@����ֵ         : ENABLE or DISABLE
@����           : �������������ָ��λ��
*****************************************************************************************/
void	motor_move(unsigned char channel,unsigned int position)
{   
	unsigned char flag;

	flag = check_pos(channel,position);
	if(flag == OK||flag == STEPHIG||flag == STEPLOW)
	{
        MotorFinalStep[channel] = position;

        //if(channel == Oilplant)
        //{

        	//UINT16 i,show = 0x0001;
		//	for(i=1; i < position; i++)
			//{
			//	show = show << 1;
			//	show |= 1;
			//}
			//if(i>2)ledShow[3].Byte = (UINT8)show;
			//else {
			//	ledShow[3].Byte = 0;
				//ledShow[3].Byte = (0xfc & ledShow[3].Byte ) | (0x03 & show);
			//}
	
	//}
              //  ;//TIE |= 0x04;
        if(channel == TACHOMETER)
               // if(!(LPIT0_SETTEN&LPIT_SETTEN_SET_T_EN_1_MASK))
					LPIT0_SETTEN = LPIT_SETTEN_SET_T_EN_1_MASK;//TACHO = 1;//TIE |= 0x08;  
        if(channel == TEMP)
        {
   			
			//UINT16 i,show = 0x0001;
			
			//for(i=1; i < position; i++)
			//{
			//	show = show << 1;
			//	show |= 1;
			//}
			//ledShow[2].Byte = (UINT8)show;
	}
            //    ;//TIE |= 0x02;
        if(channel == Carspeed)
               // if(!(LPIT0_SETTEN&LPIT_SETTEN_SET_T_EN_2_MASK))
					LPIT0_SETTEN = LPIT_SETTEN_SET_T_EN_2_MASK;// bCARSPEED = 1;//TIE |= 0x10;
        if(channel == PRES)//L: ǰ��ѹ
        {
         static UINT8 cnt = 0x100;
			if(oldLedRightState == position)
			{
				cnt++;
			}
			else 
			{
		       	oldLedRightState = position;
		        	cnt = 0;
			}
			if(cnt >=34)
			{
                         lastLedRightState = oldLedRightState;
			     cnt = 100;		 

			}	

			
			
		}
        if(channel == PRES2)//L:����ѹ
        {
        	
			static UINT8 cnt = 0x100;
			if(oldLedLeftState == position)
			{
				cnt++;
			}
			else 
			{
		       	oldLedLeftState = position;
		        	cnt = 0;
			}
			if(cnt >=34)
			{
                         lastLedLeftState = oldLedLeftState;
			     cnt = 100;		 

			}    
          }
              //  ;//TIE |=0x20;
    }
}
/**********************************************************

***********************************************************/
void back0_motor(void) 
{
	resetback0 = 1;				///set reset back flag
	delay1s=0x00;					///clear reset back time

	MotorFinalStep[PRES] = 1;
	MotorCurStep[PRES] = 0;//PRESMAXSTEP>>2;
	motor_move(PRES,0);

	MotorFinalStep[TEMP] = 1;
	MotorCurStep[TEMP] = 0;//TEMPMAXSTEP>>2;
	motor_move(TEMP,0);

	MotorFinalStep[Oilplant] = 1;
	MotorCurStep[Oilplant] = 0;//OILMAXSTEP>>2;
	motor_move(Oilplant,0);    

	MotorFinalStep[TACHOMETER] = 1;
	MotorCurStep[TACHOMETER] = TACHOMAXSTEP>>2;
	motor_move(TACHOMETER,0);  

	MotorFinalStep[Carspeed] = 1;        
	MotorCurStep[Carspeed] = CARSPEEDMAXSTEP>>2;
	motor_move(Carspeed,0);  

	MotorFinalStep[PRES2] = 1;
	MotorCurStep[PRES2] = 0;//PRES2MAXSTEP>>2;
	motor_move(PRES2,0);
} 

void back0_motor_off(void)
{
	resetback0 = 1;				///set reset back flag
	delay1s=0x00;					///clear reset back time

#if 0
	MotorFinalStep[PRES] = 1;
	MotorCurStep[PRES] += PRESMAXSTEP>>4;
	motor_move(PRES,0);

	MotorFinalStep[TEMP] = 1;
	MotorCurStep[TEMP] += TEMPMAXSTEP>>4;
	motor_move(TEMP,0);

	MotorFinalStep[Oilplant] = 1;
	MotorCurStep[Oilplant] += OILMAXSTEP>>4;
	motor_move(Oilplant,0);    
#endif
	motor_move(PRES,0);
	motor_move(PRES2,0);
	
	MotorFinalStep[TACHOMETER] = 1;
	MotorCurStep[TACHOMETER] += TACHOMAXSTEP>>4;
	motor_move(TACHOMETER,0);  

	MotorFinalStep[Carspeed] = 1;        
	MotorCurStep[Carspeed] += CARSPEEDMAXSTEP>>4;
	motor_move(Carspeed,0);  

#if 0
	MotorFinalStep[PRES2] = 1;
	MotorCurStep[PRES2] += PRES2MAXSTEP>>4;
	motor_move(PRES2,0);
#endif
}

void back0_motor_max(void)
{
  resetback0 = 1;				///set reset back flag
	delay1s=0x00;					///clear reset back time
 motor_move(PRES,PRESMAXSTEP);
 motor_move(TEMP,TEMPMAXSTEP);   
 motor_move(Oilplant,OILMAXSTEP);    
  motor_move(TACHOMETER,(TACHOMAXSTEP>>2));         
  motor_move(Carspeed,CARSPEEDMAXSTEP>>2);  
 motor_move(PRES2,PRES2MAXSTEP);
}
void back0_motor_min(void)
{
  resetback0 = 1;				///set reset back flag
	delay1s=0x00;					///clear reset back time
 motor_move(PRES,0);
 motor_move(TEMP,0);   
 motor_move(Oilplant,0);    
  motor_move(TACHOMETER,0);         
  motor_move(Carspeed,0);  
motor_move(PRES2,0);
}

extern UINT16 ShowDir,ShowCnt;
unsigned char motorStep = 0;
void motorPost(void) 
{                    
	switch(motorStep)
	{
	   case 0:  
     if(resetback0==0) { 
                                 ShowCnt= 0;
					ShowDir = 0;
			  IS31FL3738_SetLedPWM( 22/8, 22%8, 0x80);
			  IS31FL3738_SetLedPWM( 19/8,19%8, 0x80);
			  IS31FL3738_SetLedPWM(20/8, 20%8, 0x80);
			  IS31FL3738_SetLedPWM( 21/8, 21%8, 0x80);
			  IS31FL3738_SetLedPWM( 46/8, 46%8, 0x80);
			  IS31FL3738_SetLedPWM( 45/8,45%8, 0x80);
			  IS31FL3738_SetLedPWM( 43/8, 43%8, 0x80);
			  IS31FL3738_SetLedPWM(44/8, 44%8, 0x80);
			  back0_motor_max(); motorStep++;  }break;
	   case 1:  if(resetback0==0) {  back0_motor_min();  motorStep++; }break;
	   case 2:  if(resetback0==0) { 
	   	         IS31FL3738_SetLedPWM( 22/8, 22%8, 0);
			  IS31FL3738_SetLedPWM( 19/8,19%8, 0);
			  IS31FL3738_SetLedPWM(20/8, 20%8, 0);
			  IS31FL3738_SetLedPWM( 21/8, 21%8, 0);
			  IS31FL3738_SetLedPWM( 46/8, 46%8, 0);
			  IS31FL3738_SetLedPWM( 45/8,45%8, 0);
			  IS31FL3738_SetLedPWM( 43/8, 43%8, 0);
			  IS31FL3738_SetLedPWM(44/8, 44%8, 0);
	   	motorStep++; 
                Sys_Post_Flag= 0; 
                MUTE_SOUNDS_F=0;}break;
	   default:	 motorStep = 0;break; 
	}

	if(motorStep>0)
		{LoadsLedPost(); }
}
      
