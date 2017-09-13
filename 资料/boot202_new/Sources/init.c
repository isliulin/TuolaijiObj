#include "init.h"
//#include "message.h"
//#include "mscan.h"
#include "sci.h"
//#include "misc.h"
//#include "spi.h"
//#include "ioport.h"
//#include "rtc.h"
//#include "cananalysis.h"
//#include "scianalysis.h"
//#include "Capture.h"



/***********************************************
*��������Clk_Init     
*��  �ܣ���ʼ��ϵͳʱ��
*��  ����void
*��  �أ�void
************************************************/
void Clk_Init(void)
{
    MODRR = 0X04;//PM4,PM5-->CAN4
    ECLKCTL|=0x80;//�ر�ECLK 
#if 1   
    CLKSEL_PLLSEL = 0; ///bus clock=OSC/2
    CLKSEL_RTIWAI = 1; ///rtc stop in wait mode
    //CLKSEL_COPWAI = 1; ///cop stop in wait mode   
    //COPCTL = 0x00; ///1s will reset COP  funtion 
#else 
    CLKSEL_PLLSEL = 0; ///bus clock=Fpll/2
    CLKSEL_RTIWAI = 1; ///rtc stop in wait mode
    
    PLLCTL_PLLON=1;
    SYNR_SYNDIV=2;
    SYNR_VCOFRQ=3;
    REFDV_REFDIV=1;
    REFDV_REFFRQ=3;
    while(!CRGFLG_LOCK);
    CLKSEL_PLLSEL = 1;
#endif
}



/***********************************************
*��������Timer_Init
*��  ����void
*��  �أ�void
************************************************/

/***********************************************
*��������Misc_Init    
*��  �ܣ�ϵͳ���ܳ�ʼ��
*��  ����void
*��  �أ�void
************************************************/


/***********************************************
*��������IrqOpen   
*��  �ܣ����ж�
*��  ����void
*��  �أ�void
************************************************/


/***********************************************
*��������IrqClose   
*��  �ܣ��ر��ж�
*��  ����void
*��  �أ�void
************************************************/


/***********************************************
*��������XirqOpen   
*��  �ܣ����ⲿ�ж�
*��  ����void
*��  �أ�void
************************************************/


/***********************************************
*��������Vector_Init   
*��  �ܣ��ж�������ʼ��
*��  ����void
*��  �أ�void
************************************************/
void Vector_Init(void)
{
    IVBR = 0xDF;
}




/***********************************************
*��������Cpu_Init  
*��  �ܣ�CPU��ʼ��
*��  ����void
*��  �أ�void
************************************************/
void Cpu_Init(void)
{
    //Vector_Init();
    //Clk_Init();   
    
    SCI0_Init();
    //Init_IntPriority();    
}


