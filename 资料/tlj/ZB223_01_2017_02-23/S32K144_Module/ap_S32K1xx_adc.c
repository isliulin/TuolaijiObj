#include "ap_S32K1xx_adc.h"


//============================================================================
//函数名称：hw_adc_init
//函数返回：0 成功 ，1 失败
//参数说明：MoudelNumber：模块号
//功能概要：AD初始化
//============================================================================
uint8_t hw_adc_init(int MoudelNumber)
{
    if(MoudelNumber == 0)//模块0
    {
    	//打开ADC0模块时钟
	  	PCC->ADC0r = PCC_ADC0_CGC(0);
        PCC->ADC0r = PCC_ADC0_CGC(1) | PCC_ADC0_PCS(3);
		//SIM->ADCOPT = SIM_ADCOPT_ADC0PRETRGSEL(2) |  SIM_ADCOPT_ADC0SWPRETRG(4); 
    }
    else if(MoudelNumber == 1)//模块1
    {      
    	//打开ADC1模块时钟
	  PCC->ADC1r = PCC_ADC0_CGC(0);
        PCC->ADC1r = PCC_ADC1_CGC(1) | PCC_ADC1_PCS(3);
		//SIM->ADCOPT = SIM_ADCOPT_ADC1PRETRGSEL(2) |  SIM_ADCOPT_ADC1SWPRETRG(4); 
    }
    else
    {
        return 0;
    }
    
    return 1;
}



//============================================================================
//函数名称：hw_ad_once
//函数返回：16位无符号的AD值 
//参数说明：MoudelNumber：模块号
//               Channel：通道号
//              accuracy：精度
//功能概要：采集一次一路模拟量的AD值    
//============================================================================
uint16_t hw_adc_once(int MoudelNumber,int Channel,uint8_t accuracy)//采集某路模拟量的AD值
{
		uint16_t result = 0;
		uint8_t ADCCfg1Mode = 0;
		ADC_MemMapPtr ADCMoudel;//保存ADC模块地址指针
				
		switch(accuracy)
		{
			 case 8:
			   ADCCfg1Mode = 0x00;
			   break;
			 case 12:
			   ADCCfg1Mode = 0x01;
			   break;
			 case 10:
			   ADCCfg1Mode = 0x02;
			   break;
			 case 16:
			   ADCCfg1Mode = 0x03;
			   break;
			 default:
			   ADCCfg1Mode = 0x00;
		}
		
		
		if(MoudelNumber==0)//选择ADC模块0
		{
		   ADCMoudel = ADC0_BASE_PTR;
		}
		else               //选择ADC模块1
		{
		   ADCMoudel = ADC1_BASE_PTR;
		}
		
		//配置正常电源模式，总线时钟，总线时钟4分频，长采样时间使能，设置精度
		ADC_CFG1_REG(ADCMoudel) = ADLPC_NORMAL
								 | ADC_CFG1_ADIV(ADIV_8)
								 //| ADLSMP_LONG
								 | ADC_CFG1_MODE(ADCCfg1Mode)
								 | ADC_CFG1_ADICLK(ADICLK1);//时钟选择
		   
		//配置禁止异步时钟使能输出，ADxxat通道选择，高速配置，长采样时间   
		ADC_CFG2_REG(ADCMoudel)  = ADC_CFG2_SMPLTS(0x0C);
		
		ADC_SC2_REG(ADCMoudel) = ADC_SC2_REFSEL(0) |  ADC_SC2_ADTRG(0);
		///ADC_SC3_REG(ADCMoudel) = ADC_SC3_CAL(1);
								
		//设置通道号
		ADC_SC1_REG(ADCMoudel,A) = AIEN_OFF | ADC_SC1_ADCH(Channel)| ADC_SC1_COCO_MASK;
	    //等待转换完成
    	while (( ADC_SC1_REG(ADCMoudel,A) & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK)
		{

		}
	    //读取转化结果
		result = ADC_R_REG(ADCMoudel,A);       
		//清ADC转换完成标志
		//ADC_SC1_REG(ADCMoudel,A) &= ~ADC_SC1_COCO_MASK;

    return result;
}

//============================================================================
//函数名称：hw_ad_mid
//函数返回：16位无符号的AD值 
//参数说明：MoudelNumber：模块号
//               Channel：通道号
//              accuracy：精度
//功能概要：中值滤波后的结果(范围:0-4095) 
//============================================================================
uint16_t hw_adc_mid(int MoudelNumber,int Channel,uint8_t accuracy) 
{
	uint16_t i,j,k,tmp;
	//1.取3次A/D转换结果
	i = hw_adc_once(MoudelNumber,Channel,accuracy);
	j = hw_adc_once(MoudelNumber,Channel,accuracy);
	k = hw_adc_once(MoudelNumber,Channel,accuracy);
	//2.取中值
	if (i > j)
	{
		tmp = i; i = j; j = tmp;
	}
	if (k > j) 
	  tmp = j;
	else if(k > i) 
	  tmp = k;
    else 
      tmp = i;
	return tmp;
}

//============================================================================
//函数名称：hw_ad_ave
//函数返回：16位无符号的AD值 
//参数说明：MoudelNumber：模块号
//               Channel：通道号
//              accuracy：精度
//                     N:均值滤波次数(范围:0~255)
//功能概要：均值滤波后的结果(范围:0-4095) 
//============================================================================
uint16_t hw_adc_ave(int MoudelNumber,int Channel,uint8_t accuracy,uint8_t N) 
{
	uint32_t tmp = 0;
	uint8_t  i;
    for(i = 0; i < N; i++)
		tmp += hw_adc_mid(MoudelNumber,Channel,accuracy);
	tmp = tmp / N; 
    return (uint16_t)tmp;
}

/****************************************************************************************
函数名：static uchar AD_Trans(uchar channel) 
功能：AD转换
参数说明：uchar channel -> AD转换的通道数。
返回值：无
*****************************************************************************************/
uchar AD_Trans(uchar module,uchar channel)           
{
    unsigned int sum;
    unsigned char  Max,Min,ad_catch[5];
    unsigned char i;

	if(module>=2) return 0xfe;
	if(channel>=16) return 0xfe;
	
    for (i=0;i<5;i++)
        ad_catch[i] = hw_adc_once(module,channel,8);

    Max=ad_catch[0];
    Min=ad_catch[0];
    sum=ad_catch[0];
    for (i=1;i<5;i++)
    {
        sum=sum+ad_catch[i];
        if (ad_catch[i]>Max)  Max=ad_catch[i];
        if (ad_catch[i]<Min)  Min=ad_catch[i];
    }
    return(uchar)((sum-Max-Min)/3);
}