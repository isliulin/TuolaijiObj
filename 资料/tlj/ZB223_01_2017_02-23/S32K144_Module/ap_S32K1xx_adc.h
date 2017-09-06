#ifndef __AP_S32K1XX_ADC_H__
#define __AP_S32K1XX_ADC_H__

#include "S32K144.h"
#include "Typedef.h"

#define ADC0_irq_no 57
#define ADC1_irq_no 58
#define A                 0x0
//#define B                 0x1

#define COCO_COMPLETE     ADC_SC1_COCO_MASK
#define COCO_NOT          0x00

//ADC 中断: 使能或者禁止
#define AIEN_ON           ADC_SC1_AIEN_MASK
#define AIEN_OFF          0x00


//ADC电源设置 
#define ADLPC_LOW         ADC_CFG1_ADLPC_MASK
#define ADLPC_NORMAL      0x00

//时钟分频
#define ADIV_1            0x00
#define ADIV_2            0x01
#define ADIV_4            0x02
#define ADIV_8            0x03

//长采样时间或者短采样时间
#define ADLSMP_LONG       ADC_CFG1_ADLSMP_MASK
#define ADLSMP_SHORT      0x00


//ADC输入时钟源选择 总线，总线/2，”altclk“或者ADC自身异步时钟以减少噪声
#define ADICLK1       0x00	//FIRC
#define ADICLK2       0x01 //SIRC
#define ADICLK3       0x02 //OSC
#define ADICLK4       0x03 //PLL

//选择通道A，通道B
#define MUXSEL_ADCB       ADC_CFG2_MUXSEL_MASK
#define MUXSEL_ADCA       0x00

//异步时钟输出使能：使能，或者禁止输出
#define ADACKEN_ENABLED   ADC_CFG2_ADACKEN_MASK
#define ADACKEN_DISABLED  0x00

//高速低速转换时间
#define ADHSC_HISPEED     ADC_CFG2_ADHSC_MASK
#define ADHSC_NORMAL      0x00

//长采样时间选择：20,12,6或者2个额外的时钟对于长采样时间
#define ADLSTS_20          0x00
#define ADLSTS_12          0x01
#define ADLSTS_6           0x02
#define ADLSTS_2           0x03

//函数接口声明

//============================================================================
//函数名称：hw_adc_init
//函数返回：0 成功 ，1 失败
//参数说明：MoudelNumber：模块号
//功能概要：AD初始化
//============================================================================
uint8_t hw_adc_init(int MoudelNumber);


//============================================================================
//函数名称：hw_adc_once
//函数返回：16位无符号的AD值
//参数说明：MoudelNumber：模块号
//               Channel：通道号
//              accuracy：精度
//功能概要：采集一次一路模拟量的AD值    
//============================================================================
uint16_t hw_adc_once(int MoudelNumber,int Channel,uint8_t accuracy);

//============================================================================
//函数名称：hw_adc_mid
//函数返回：16位无符号的AD值 
//参数说明：MoudelNumber：模块号
//               Channel：通道号
//              accuracy：精度
//功能概要：中值滤波后的结果(范围:0-4095) 
//============================================================================
uint16_t hw_adc_mid(int MoudelNumber,int Channel,uint8_t accuracy); 

//============================================================================
//函数名称：hw_adc_ave
//函数返回：16位无符号的AD值 
//参数说明：MoudelNumber：模块号
//               Channel：通道号
//              accuracy：精度
//                     N:均值滤波次数(范围:0~255)
//功能概要：均值滤波后的结果(范围:0-4095) 
//============================================================================
uint16_t hw_adc_ave(int MoudelNumber,int Channel,uint8_t accuracy,uint8_t N); 

uchar AD_Trans(uchar module,uchar channel); 

#endif
