#define		DISTANVE_DEAL_C
#include	"config.h"
#pragma CODE_SEG USER_ROM
#pragma MESSAGE DISABLE C12056///�����ޱ�Ҫ�ı�����ʾ 
/***************************************************************************
һ���������ͣ��ײ�������Ӧ�ò㡢��ʼ��������ʼ��
��������������
1��
�����β�������
�ġ�����ֵ����
�塢���ļ�¼������ǵײ���������ʼ�����ͳ���ʱ����
1��
�������ò�����ʽ��
1��
***************************************************************************/
void	Distance_Counter(void)
{		
	if(OnKeyBit==KeyOn)					
	{
		if(DistanceSaveFlag==0)
		{
			DistanceSaveFlag=1;
			CountFuelConsumeFlag=1;			
			if(VehicleSmallDistance!=0)
				SmallFuelConsumeBlinkFlag=1;
			else
			{
				SmallFuelConsumeBlinkFlag=0;	
				CountSmallFuelConsumeFlag=1;
			}
		}
	
		if((carspeed>0)&&(CountDistanceSave/(dword)1000>=DISTANCE_SAVENUM))
		{
			PowerOffSave();
			CountDistanceSave=0;
		}
	}
	else
	{
		if(DistanceSaveFlag==1)///when key from on to acc or off;first power on not to do this
		{
			DistanceSaveFlag=0;
			CountDistanceSave=0;	
			SmallFuelConsumeBlinkFlag=0;
			PowerOffSave();
		}
	}
	if(OnKeyBit==KeyOff)
		return;	
	CountVehicleDistance();
	///����ٹ����ͺ�
	CountHundredKilometerFuelConsume();		
	///�����;�ͺ�
	CountSmallDistanceFuelConsume();
}
/***************************************************************************
һ���������ͣ��ײ�������Ӧ�ò㡢��ʼ��������ʼ��
��������������
1��
�����β�������
�ġ�����ֵ����
�塢���ļ�¼������ǵײ���������ʼ�����ͳ���ʱ����
1��
�������ò�����ʽ��
1��
***************************************************************************/
static void CountVehicleDistance(void)
{
	dword CountDistancetmp=0;
	dword CountDistancetmp1=0;	
	
	if(SmallDistanceCancelFlag==1)
	{
		uchar data_temp[4];
		SmallFuelConsumeBlinkFlag=0;
		SmallDistanceCancelFlag=0;
		/*first must save last data of overall distance and then pay afresh*/
		SaveOverallDistance();
		/*clear small distance*/
		OldVehicleSmallDistance=0;	
		VehicleSmallDistance=0;
   	/*pay afresh the last overall distance*/	
		DelayMs(1);
		Eeprom_Read_Fact(OverallDistance_Addr,data_temp,sizeof(data_temp));
		VehicleOverallDistanceLow=data_temp[1];
		VehicleOverallDistanceLow<<=8;
		VehicleOverallDistanceLow+=data_temp[0];
		VehicleOverallDistanceHigh=data_temp[3];
		VehicleOverallDistanceHigh<<=8;
		VehicleOverallDistanceHigh+=data_temp[2];
		OldVehicleOverallDistance = VehicleOverallDistanceHigh;
  	OldVehicleOverallDistance<<=16;
  	OldVehicleOverallDistance+= VehicleOverallDistanceLow;
		VehicleOverallDistance=OldVehicleOverallDistance;
	}			
	if(Vehicle1MinutueFlag==1)
	{
		Vehicle1MinutueFlag=0;
		/*count vehicle small distance*/	
		CountDistancetmp=(uint)(VehicleSpeedData/10);	///count 1 hour distance;VehicleSpeedData/3600s*10s		
		CountDistancetmp=CountDistancetmp/200;        ///add 09-2-17 change to 1.8s display

		CountDistanceSave+=CountDistancetmp;///10.03.02 add to save distance to eeprom
		HundredKilometerDistanceNumber+=CountDistancetmp;///11.11.16���Ӽ���ٹ����ͺģ��������˶�����
		
		VehicleSmallDistance=OldVehicleSmallDistance+CountDistancetmp;	
		OldVehicleSmallDistance=VehicleSmallDistance;			///bak up small distance
		
		VehicleSmallDistanceHigh=(uint)(VehicleSmallDistance>>16);///eeprom save deal
		VehicleSmallDistanceLow=(uint)VehicleSmallDistance;		
		
		CountDistancetmp=(uint)(VehicleSmallDistance/100);		///message send deal;
		VehicleSmallDistanceNum[0]=(uchar)(CountDistancetmp>>8);
		VehicleSmallDistanceNum[1]=(uchar)CountDistancetmp;						
		/*count vehicle overall distance*/
		/*******************100************************/
		
		CountDistancetmp1=VehicleSmallDistance/100;///because the small destance is "m" unit,so must change to "km"unit

		VehicleOverallDistance=OldVehicleOverallDistance+CountDistancetmp1;
		
		VehicleOverallDistanceHigh=(uint)(VehicleOverallDistance>>16);	///eeprom save deal  
		VehicleOverallDistanceLow=(uint)VehicleOverallDistance;	
		
		VehicleOverallDistanceNum[0]=(uchar)(VehicleOverallDistance>>16);		///message send deal
		VehicleOverallDistanceNum[1]=(uchar)(VehicleOverallDistance>>8);
		VehicleOverallDistanceNum[2]=(uchar)VehicleOverallDistance;		
		fual_value_require();
	}			 
   /*distribute the small distance to display in internal lcd */  
   if(VehicleSmallDistance>=999999)		///full screen will be reset to 0
	{
		uchar data_temp[4];
		SaveOverallDistance();
		VehicleSmallDistance=0;	
		OldVehicleSmallDistance=0;
		DelayMs(1);
		Eeprom_Read_Fact(OverallDistance_Addr,data_temp,4);
		VehicleOverallDistanceLow=data_temp[1];
		VehicleOverallDistanceLow<<=8;
		VehicleOverallDistanceLow+=data_temp[0];
		VehicleOverallDistanceHigh=data_temp[3];
		VehicleOverallDistanceHigh<<=8;
		VehicleOverallDistanceHigh+=data_temp[2];
		OldVehicleOverallDistance = VehicleOverallDistanceHigh;
  	OldVehicleOverallDistance<<=16;
  	OldVehicleOverallDistance+= VehicleOverallDistanceLow;
		VehicleOverallDistance=OldVehicleOverallDistance;		
		OldVehicleOverallDistance = VehicleOverallDistanceHigh;
		OldVehicleOverallDistance<<=16;
		OldVehicleOverallDistance+= VehicleOverallDistanceLow;
		VehicleOverallDistance=OldVehicleOverallDistance;
	}  
	/*distribute the overall distance to display int internal lcd */ 		
	if(VehicleOverallDistance>=9999999)///full screen will be reset to 0
	{
		VehicleOverallDistance=0;
		OldVehicleOverallDistance=0;
  }
}
/***************************************************************************
һ���������ͣ��ײ�������Ӧ�ò㡢��ʼ��������ʼ��
��������������
1��
�����β�������
�ġ�����ֵ����
�塢���ļ�¼������ǵײ���������ʼ�����ͳ���ʱ����
1��
�������ò�����ʽ��
1��
***************************************************************************/
static void CountHundredKilometerFuelConsume(void)
{
	ulong Temp=0;
	
	if(CountFuelConsumeFlag==1)
	{	
		CountFuelConsumeFlag=0;
		fual_value_require();
		HundredKilometerDistanceNumber=0;
		HundredKilometerFuelConsume=0;
		FirstTotalFuelConsume=0;
		FirstTotalFuelConsumeRequireFlag=1;
	} 
	if((TotalFuelConsumeCompleteFlag==1)&&(FirstTotalFuelConsumeRequireFlag==1))///����1������ECU�з��ͱ��ĺ���ȡ��1�ε����ͺ�
	{
		FirstTotalFuelConsume=total_fual_used*100;
		FirstTotalFuelConsume=FirstTotalFuelConsume/2;
		FirstTotalFuelConsumeRequireFlag=0;
		TotalFuelConsumeCompleteFlag=0;
	}
	if(HundredKilometerDistanceNumber!=0)///����(ON��)������б仯�ż���ٹ����ͺ�
	{
		if(LastTotalFuelConsume<FirstTotalFuelConsume)///���ݵ��쳣����
			FirstTotalFuelConsume=LastTotalFuelConsume;
		Temp=LastTotalFuelConsume-FirstTotalFuelConsume;
		Temp=Temp*100;///ת��Ϊ����;�ڽ���ʱ�Ѿ�����10�����ٳ���10�Ƕ��С���㣬���������ֻ��Ҫ�ٳ���100
		Temp=Temp*100;///�Ƚ������100Ϊ�ٹ���׼��			
		HundredKilometerFuelConsume=(unsigned int)(Temp/HundredKilometerDistanceNumber);///�ٹ�����Ͷ�����
	 	if(HundredKilometerFuelConsume>800)///11.12.29���ӣ�Ϊ����������߸�Ư����������˵˲ʱ�����
	 		HundredKilometerFuelConsume=800;
	}
}
/***************************************************************************
һ���������ͣ��ײ�������Ӧ�ò㡢��ʼ��������ʼ��
��������������
1��
�����β�������
�ġ�����ֵ����
�塢���ļ�¼������ǵײ���������ʼ�����ͳ���ʱ����
1��
�������ò�����ʽ��
1��
***************************************************************************/
static void CountSmallDistanceFuelConsume(void)
{
	ulong Temp=0;
	
	if(CountSmallFuelConsumeFlag==1)
	{	
		CountSmallFuelConsumeFlag=0;
		TotalFuelConsumeCompleteFlag=0;
		fual_value_require();
		FirstTotalFuelConsume_Small=0;
		LastTotalFuelConsume_Small=0;
		SmallDistanceFuelConsume=0;
		FirstCountSmallConsumeRequireFlag=1;
	} 
	if((TotalFuelConsumeCompleteFlag==1)&&(FirstCountSmallConsumeRequireFlag==1))///����1������ECU�з��ͱ��ĺ���ȡ��1�ε����ͺ�
	{
		FirstTotalFuelConsume_Small=total_fual_used*100;
		FirstTotalFuelConsume_Small=FirstTotalFuelConsume_Small/2;
		FirstCountSmallConsumeRequireFlag=0;
		TotalFuelConsumeCompleteFlag=0;
	}
	if(VehicleSmallDistance!=0&&SmallFuelConsumeBlinkFlag==0)///����(ON��)������б仯�ż���ٹ����ͺ�
	{
		if(LastTotalFuelConsume_Small<FirstTotalFuelConsume_Small)///���ݵ��쳣����
			FirstTotalFuelConsume_Small=LastTotalFuelConsume_Small;
		Temp=LastTotalFuelConsume_Small-FirstTotalFuelConsume_Small;
		Temp=Temp/10;
		SmallDistanceFuelConsume=(uint)Temp;
	}
}

/***************************************************************************
һ���������ͣ��ײ�������Ӧ�ò㡢��ʼ��������ʼ��
��������������
1��
�����β�������
�ġ�����ֵ����
�塢���ļ�¼������ǵײ���������ʼ�����ͳ���ʱ����
1��
�������ò�����ʽ��
1��
***************************************************************************/
static void	SaveOverallDistance(void)
{
	uchar	data[4];

	data[0] = (unsigned char)(VehicleOverallDistanceLow);
	data[1] = (unsigned char)(VehicleOverallDistanceLow>>8);
	data[2] = (unsigned char)(VehicleOverallDistanceHigh);
	data[3] = (unsigned char)(VehicleOverallDistanceHigh>>8);
	
	Eeprom_Write_Fact(OverallDistance_Addr,data,4);
}
/***************************************************************************
һ���������ͣ��ײ�������Ӧ�ò㡢��ʼ��������ʼ��
��������������
1��
�����β�������
�ġ�����ֵ����
�塢���ļ�¼������ǵײ���������ʼ�����ͳ���ʱ����
1��
�������ò�����ʽ��
1��
***************************************************************************/
void	ClearDistance(void)
{
	uchar	data_temp[9],i;
	///clear current data
	VehicleSmallDistance=0;
	OldVehicleSmallDistance=0;
	VehicleOverallDistance=0;
	OldVehicleOverallDistance=0;
	for(i=0;i<sizeof(data_temp);i++)
		data_temp[i]=0x00;
	Eeprom_Write_Fact(SmallDistance_Addr,data_temp,sizeof(data_temp));
}
/***************************************************************************
һ���������ͣ��ײ�������Ӧ�ò㡢��ʼ��������ʼ��
��������������
1��
�����β�������
�ġ�����ֵ����
�塢���ļ�¼������ǵײ���������ʼ�����ͳ���ʱ����
1��
�������ò�����ʽ��
1��
***************************************************************************/
void	ConfigDistance(void)
{
	uchar	i;
	ulong	Data_Temp;
	///clear current data
	VehicleSmallDistance=0;
	OldVehicleSmallDistance=0;
	
	Data_Temp=EepromConfigData[3];
	Data_Temp<<=8;
	Data_Temp+=EepromConfigData[2];
	Data_Temp<<=8;
	Data_Temp+=EepromConfigData[1];
	Data_Temp<<=8;
	Data_Temp+=EepromConfigData[0];
	Data_Temp=Data_Temp*10;
	
	VehicleOverallDistance=Data_Temp;
	OldVehicleOverallDistance=Data_Temp;

	for(i=0;i<4;i++)
		write_data[i]=0;
	
	write_data[4]=(uchar)Data_Temp;
	write_data[5]=(uchar)(Data_Temp>>8);
	write_data[6]=(uchar)(Data_Temp>>16);
	write_data[7]=(uchar)(Data_Temp>>24);
	write_data[8]=0;
	
	for(i=0;i<8;i++)
		write_data[8] += write_data[i];

	_Config_write:
	Eeprom_Write_Fact(SmallDistance_Addr,write_data,sizeof(write_data));	

	DelayMs(1);

	Eeprom_Read_Fact(SmallDistance_Addr, read_data, 9);

  for (i = 0; i < 9; i++)
  {
    if (read_data[i] != write_data[i])
      goto _Config_write;
  }
  asm nop;
}
#pragma CODE_SEG DEFAULT

