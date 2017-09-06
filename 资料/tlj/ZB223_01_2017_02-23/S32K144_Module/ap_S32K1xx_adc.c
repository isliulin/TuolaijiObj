#include "ap_S32K1xx_adc.h"


//============================================================================
//�������ƣ�hw_adc_init
//�������أ�0 �ɹ� ��1 ʧ��
//����˵����MoudelNumber��ģ���
//���ܸ�Ҫ��AD��ʼ��
//============================================================================
uint8_t hw_adc_init(int MoudelNumber)
{
    if(MoudelNumber == 0)//ģ��0
    {
    	//��ADC0ģ��ʱ��
	  	PCC->ADC0r = PCC_ADC0_CGC(0);
        PCC->ADC0r = PCC_ADC0_CGC(1) | PCC_ADC0_PCS(3);
		//SIM->ADCOPT = SIM_ADCOPT_ADC0PRETRGSEL(2) |  SIM_ADCOPT_ADC0SWPRETRG(4); 
    }
    else if(MoudelNumber == 1)//ģ��1
    {      
    	//��ADC1ģ��ʱ��
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
//�������ƣ�hw_ad_once
//�������أ�16λ�޷��ŵ�ADֵ 
//����˵����MoudelNumber��ģ���
//               Channel��ͨ����
//              accuracy������
//���ܸ�Ҫ���ɼ�һ��һ·ģ������ADֵ    
//============================================================================
uint16_t hw_adc_once(int MoudelNumber,int Channel,uint8_t accuracy)//�ɼ�ĳ·ģ������ADֵ
{
		uint16_t result = 0;
		uint8_t ADCCfg1Mode = 0;
		ADC_MemMapPtr ADCMoudel;//����ADCģ���ַָ��
				
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
		
		
		if(MoudelNumber==0)//ѡ��ADCģ��0
		{
		   ADCMoudel = ADC0_BASE_PTR;
		}
		else               //ѡ��ADCģ��1
		{
		   ADCMoudel = ADC1_BASE_PTR;
		}
		
		//����������Դģʽ������ʱ�ӣ�����ʱ��4��Ƶ��������ʱ��ʹ�ܣ����þ���
		ADC_CFG1_REG(ADCMoudel) = ADLPC_NORMAL
								 | ADC_CFG1_ADIV(ADIV_8)
								 //| ADLSMP_LONG
								 | ADC_CFG1_MODE(ADCCfg1Mode)
								 | ADC_CFG1_ADICLK(ADICLK1);//ʱ��ѡ��
		   
		//���ý�ֹ�첽ʱ��ʹ�������ADxxatͨ��ѡ�񣬸������ã�������ʱ��   
		ADC_CFG2_REG(ADCMoudel)  = ADC_CFG2_SMPLTS(0x0C);
		
		ADC_SC2_REG(ADCMoudel) = ADC_SC2_REFSEL(0) |  ADC_SC2_ADTRG(0);
		///ADC_SC3_REG(ADCMoudel) = ADC_SC3_CAL(1);
								
		//����ͨ����
		ADC_SC1_REG(ADCMoudel,A) = AIEN_OFF | ADC_SC1_ADCH(Channel)| ADC_SC1_COCO_MASK;
	    //�ȴ�ת�����
    	while (( ADC_SC1_REG(ADCMoudel,A) & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK)
		{

		}
	    //��ȡת�����
		result = ADC_R_REG(ADCMoudel,A);       
		//��ADCת����ɱ�־
		//ADC_SC1_REG(ADCMoudel,A) &= ~ADC_SC1_COCO_MASK;

    return result;
}

//============================================================================
//�������ƣ�hw_ad_mid
//�������أ�16λ�޷��ŵ�ADֵ 
//����˵����MoudelNumber��ģ���
//               Channel��ͨ����
//              accuracy������
//���ܸ�Ҫ����ֵ�˲���Ľ��(��Χ:0-4095) 
//============================================================================
uint16_t hw_adc_mid(int MoudelNumber,int Channel,uint8_t accuracy) 
{
	uint16_t i,j,k,tmp;
	//1.ȡ3��A/Dת�����
	i = hw_adc_once(MoudelNumber,Channel,accuracy);
	j = hw_adc_once(MoudelNumber,Channel,accuracy);
	k = hw_adc_once(MoudelNumber,Channel,accuracy);
	//2.ȡ��ֵ
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
//�������ƣ�hw_ad_ave
//�������أ�16λ�޷��ŵ�ADֵ 
//����˵����MoudelNumber��ģ���
//               Channel��ͨ����
//              accuracy������
//                     N:��ֵ�˲�����(��Χ:0~255)
//���ܸ�Ҫ����ֵ�˲���Ľ��(��Χ:0-4095) 
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
��������static uchar AD_Trans(uchar channel) 
���ܣ�ADת��
����˵����uchar channel -> ADת����ͨ������
����ֵ����
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