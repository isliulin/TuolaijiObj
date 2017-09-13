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
*函数名：Clk_Init     
*功  能：初始化系统时钟
*参  数：void
*返  回：void
************************************************/
void Clk_Init(void)
{
    MODRR = 0X04;//PM4,PM5-->CAN4
    ECLKCTL|=0x80;//关闭ECLK 
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
*函数名：Timer_Init
*参  数：void
*返  回：void
************************************************/

/***********************************************
*函数名：Misc_Init    
*功  能：系统功能初始化
*参  数：void
*返  回：void
************************************************/


/***********************************************
*函数名：IrqOpen   
*功  能：打开中断
*参  数：void
*返  回：void
************************************************/


/***********************************************
*函数名：IrqClose   
*功  能：关闭中断
*参  数：void
*返  回：void
************************************************/


/***********************************************
*函数名：XirqOpen   
*功  能：打开外部中断
*参  数：void
*返  回：void
************************************************/


/***********************************************
*函数名：Vector_Init   
*功  能：中断向量初始化
*参  数：void
*返  回：void
************************************************/
void Vector_Init(void)
{
    IVBR = 0xDF;
}




/***********************************************
*函数名：Cpu_Init  
*功  能：CPU初始化
*参  数：void
*返  回：void
************************************************/
void Cpu_Init(void)
{
    //Vector_Init();
    //Clk_Init();   
    
    SCI0_Init();
    //Init_IntPriority();    
}


