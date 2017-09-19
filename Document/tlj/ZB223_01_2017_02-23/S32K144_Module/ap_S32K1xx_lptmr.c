#include "ap_S32K1xx_lptmr.h"
   
//============================================================================

//============================================================================
void lptmr_init()
{
	
	PCC_LPTMR0= PCC_LPTMR0_CGC_MASK|PCC_LPTMR0_PCS(3)|PCC_LPTMR0_PCD(7);//选择快速时钟 48/8M = 6M
	//LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; //复位LPTMR模块
	LPTMR0_CSR = 0x00; 
	LPTMR0_PSR =0;
	LPTMR0_CMR=LPTMR_CMR_COMPARE(0xffff);//设置比较寄存器值   =78125*2HZ
	LPTMR0_PSR=LPTMR_PSR_PCS(0x3)|LPTMR_PSR_PRESCALE(0x5);//使用内部时钟，16预分频(2^(n+1))//LPTMR_PSR_PBYP_MASK
      LPTMR0_CSR |= LPTMR_CSR_TIE_MASK;
	enable_irq(LPTMR0_IRQn);
       set_irq_priority (LPTMR0_IRQn, 0);

}


void lptmr_set_hz(UINT16 hz)
{
        UINT32 fre=46875;//78125*2/2
        fre /=hz; 
        if(fre>0xffff) fre = 0xffff;
	//us = 3 * us;
	LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; //复位LPTMR模块
        LPTMR0_CNR =0;
	LPTMR0_CMR=LPTMR_CMR_COMPARE(fre);//设置比较寄存器值
	LPTMR0_CSR |=LPTMR_CSR_TEN_MASK; //开启LPT模块设置
	//LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;
	//while(!(LPTMR0_CSR& LPTMR_CSR_TCF_MASK));
	//LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; //复位LPTMR模块
}

void lptmr_close(void)
{
    LPTMR0_CSR &=~LPTMR_CSR_TEN_MASK;
}




void lptmr_waitting_us(UINT16 us)
{
	//us = 3 * us;
	LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; //复位LPTMR模块
	LPTMR0_CMR=LPTMR_CMR_COMPARE(us);//设置比较寄存器值
	LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //开启LPT模块设置
	LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;
	while(!(LPTMR0_CSR& LPTMR_CSR_TCF_MASK));
	LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; //复位LPTMR模块
}





/*
 * LPLD_LPTMR_Init
 * LPTMR初始化函数
 * 
 * 参数:
 *    lptmr_init_structure--LPTMR初始化结构体，
 *                         具体定义见LPTMR_InitTypeDef 
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */

#define LPTMR_ALT1     1
#define LPTMR_ALT2     2
#define LPTMR_ALT3     3
uint8 LPLD_LPTMR_Init(uint8 pluseacc_input)
{ 
    PCC_LPTMR0= PCC_LPTMR0_CGC_MASK|PCC_LPTMR0_PCS(3);//选择快速时钟 
     //清空寄存器
     LPTMR0->CSR = 0x00;                   
     LPTMR0->PSR = 0x00;
     LPTMR0->CMR = 0x00;
   
    //pluseacc_input =  LPTMR_ALT1;   //设置通道
   
    if(pluseacc_input == LPTMR_ALT1)            //PTE11引脚
    {
      PORTE->PCR11 = PORT_PCR_MUX(0x3);       //设置PTE11为脉冲累加
    }
    else if(pluseacc_input == LPTMR_ALT2)       //PTD5引脚
    {
      PORTD->PCR5 = PORT_PCR_MUX(0x3);        //设置PTD5为脉冲累加  
    }
    else if(pluseacc_input == LPTMR_ALT3)       //PTE2引脚
    {
      PORTE->PCR2 = PORT_PCR_MUX(0x3);        //设置PTE2为脉冲累加  
    }
    //与所用的内部时钟无关
    //外部输入的信号作为计数时钟
    LPTMR0->PSR = LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK; 
   // LPTMR0_PSR=LPTMR_PSR_PCS(0x3)|LPTMR_PSR_PRESCALE(0x3);//使用内部时钟，16预分频(2^(n+1))
    LPTMR0->CSR = LPTMR_CSR_TPS(pluseacc_input);
    LPTMR0->CSR |= LPTMR_CSR_TMS_MASK; //捕获模式
    LPTMR0->CSR |= LPTMR_CSR_TFC_MASK; //自由计数模式
  
  
 // if(lptmr_init_structure.LPTMR_IntEnable == TRUE && isr_func != NULL)
 // {
  //  LPTMR_ISR[0] = isr_func;
   // LPTMR0->CSR |= LPTMR_CSR_TIE_MASK;
 // }

  LPTMR0->CSR |= LPTMR_CSR_TEN_MASK; //开启LPTMR
  return 1;
}

/*
 * LPLD_LPTMR_DeInit
 * LPTMR反初始化函数，关闭lptmrx
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_LPTMR_Deinit(void)
{
  //清空CSR寄存器
  LPTMR0->CSR=0x00;  
  //清空PSR寄存器
  LPTMR0->PSR=0x00;
  //清空CMR寄存器
  LPTMR0->CMR=0x00;
  //禁止LPTimer中断
  disable_irq(LPTMR0_IRQn);
  
   //关闭LPTMR时钟
  //SIM->SCGC5 &= ~(SIM_SCGC5_LPTIMER_MASK);  
  //PCC_BWR_IP_CGC(S32_PCC, LPTMR_0, FALSE);
   PCC_LPTMR0 &= ~PCC_LPTMR0_CGC_MASK;//选择快速时钟 
  return 1;
}

/*
 *  LPLD_LPTMR_Reset
 *  低功耗定时器复位，复位以后清空低功耗定时器Counter
 * 
 *  参数:
 *      无
 *  输出:
 *      无
 */
void LPLD_LPTMR_ResetCounter(void)
{
  LPTMR0->CSR&=(~LPTMR_CSR_TEN_MASK);       //Reset LPTMR0 Counter
  LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;       //Reset LPTMR0 Counter
}



/*
 *  LPLD_LPTMR_GetPulseAcc
 *  得到脉冲累加的值
 * 
 *  参数:
 *      无
 *  输出:
 *      脉冲累加值
 */
uint16 LPLD_LPTMR_GetPulseAcc(void)
{
   LPTMR0->CNR=111;//先设置才更新数据
   return (uint16)LPTMR0->CNR;
}
