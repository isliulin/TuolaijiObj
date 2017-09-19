#include "ap_S32K1xx_lptmr.h"
   
//============================================================================

//============================================================================
void lptmr_init()
{
	
	PCC_LPTMR0= PCC_LPTMR0_CGC_MASK|PCC_LPTMR0_PCS(3)|PCC_LPTMR0_PCD(7);//ѡ�����ʱ�� 48/8M = 6M
	//LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; //��λLPTMRģ��
	LPTMR0_CSR = 0x00; 
	LPTMR0_PSR =0;
	LPTMR0_CMR=LPTMR_CMR_COMPARE(0xffff);//���ñȽϼĴ���ֵ   =78125*2HZ
	LPTMR0_PSR=LPTMR_PSR_PCS(0x3)|LPTMR_PSR_PRESCALE(0x5);//ʹ���ڲ�ʱ�ӣ�16Ԥ��Ƶ(2^(n+1))//LPTMR_PSR_PBYP_MASK
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
	LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; //��λLPTMRģ��
        LPTMR0_CNR =0;
	LPTMR0_CMR=LPTMR_CMR_COMPARE(fre);//���ñȽϼĴ���ֵ
	LPTMR0_CSR |=LPTMR_CSR_TEN_MASK; //����LPTģ������
	//LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;
	//while(!(LPTMR0_CSR& LPTMR_CSR_TCF_MASK));
	//LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; //��λLPTMRģ��
}

void lptmr_close(void)
{
    LPTMR0_CSR &=~LPTMR_CSR_TEN_MASK;
}




void lptmr_waitting_us(UINT16 us)
{
	//us = 3 * us;
	LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; //��λLPTMRģ��
	LPTMR0_CMR=LPTMR_CMR_COMPARE(us);//���ñȽϼĴ���ֵ
	LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //����LPTģ������
	LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;
	while(!(LPTMR0_CSR& LPTMR_CSR_TCF_MASK));
	LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; //��λLPTMRģ��
}





/*
 * LPLD_LPTMR_Init
 * LPTMR��ʼ������
 * 
 * ����:
 *    lptmr_init_structure--LPTMR��ʼ���ṹ�壬
 *                         ���嶨���LPTMR_InitTypeDef 
 * ���:
 *    0--���ô���
 *    1--���óɹ�
 */

#define LPTMR_ALT1     1
#define LPTMR_ALT2     2
#define LPTMR_ALT3     3
uint8 LPLD_LPTMR_Init(uint8 pluseacc_input)
{ 
    PCC_LPTMR0= PCC_LPTMR0_CGC_MASK|PCC_LPTMR0_PCS(3);//ѡ�����ʱ�� 
     //��ռĴ���
     LPTMR0->CSR = 0x00;                   
     LPTMR0->PSR = 0x00;
     LPTMR0->CMR = 0x00;
   
    //pluseacc_input =  LPTMR_ALT1;   //����ͨ��
   
    if(pluseacc_input == LPTMR_ALT1)            //PTE11����
    {
      PORTE->PCR11 = PORT_PCR_MUX(0x3);       //����PTE11Ϊ�����ۼ�
    }
    else if(pluseacc_input == LPTMR_ALT2)       //PTD5����
    {
      PORTD->PCR5 = PORT_PCR_MUX(0x3);        //����PTD5Ϊ�����ۼ�  
    }
    else if(pluseacc_input == LPTMR_ALT3)       //PTE2����
    {
      PORTE->PCR2 = PORT_PCR_MUX(0x3);        //����PTE2Ϊ�����ۼ�  
    }
    //�����õ��ڲ�ʱ���޹�
    //�ⲿ������ź���Ϊ����ʱ��
    LPTMR0->PSR = LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK; 
   // LPTMR0_PSR=LPTMR_PSR_PCS(0x3)|LPTMR_PSR_PRESCALE(0x3);//ʹ���ڲ�ʱ�ӣ�16Ԥ��Ƶ(2^(n+1))
    LPTMR0->CSR = LPTMR_CSR_TPS(pluseacc_input);
    LPTMR0->CSR |= LPTMR_CSR_TMS_MASK; //����ģʽ
    LPTMR0->CSR |= LPTMR_CSR_TFC_MASK; //���ɼ���ģʽ
  
  
 // if(lptmr_init_structure.LPTMR_IntEnable == TRUE && isr_func != NULL)
 // {
  //  LPTMR_ISR[0] = isr_func;
   // LPTMR0->CSR |= LPTMR_CSR_TIE_MASK;
 // }

  LPTMR0->CSR |= LPTMR_CSR_TEN_MASK; //����LPTMR
  return 1;
}

/*
 * LPLD_LPTMR_DeInit
 * LPTMR����ʼ���������ر�lptmrx
 * 
 * ����:
 *    ��
 *
 * ���:
 *    0--���ô���
 *    1--���óɹ�
 */
uint8 LPLD_LPTMR_Deinit(void)
{
  //���CSR�Ĵ���
  LPTMR0->CSR=0x00;  
  //���PSR�Ĵ���
  LPTMR0->PSR=0x00;
  //���CMR�Ĵ���
  LPTMR0->CMR=0x00;
  //��ֹLPTimer�ж�
  disable_irq(LPTMR0_IRQn);
  
   //�ر�LPTMRʱ��
  //SIM->SCGC5 &= ~(SIM_SCGC5_LPTIMER_MASK);  
  //PCC_BWR_IP_CGC(S32_PCC, LPTMR_0, FALSE);
   PCC_LPTMR0 &= ~PCC_LPTMR0_CGC_MASK;//ѡ�����ʱ�� 
  return 1;
}

/*
 *  LPLD_LPTMR_Reset
 *  �͹��Ķ�ʱ����λ����λ�Ժ���յ͹��Ķ�ʱ��Counter
 * 
 *  ����:
 *      ��
 *  ���:
 *      ��
 */
void LPLD_LPTMR_ResetCounter(void)
{
  LPTMR0->CSR&=(~LPTMR_CSR_TEN_MASK);       //Reset LPTMR0 Counter
  LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;       //Reset LPTMR0 Counter
}



/*
 *  LPLD_LPTMR_GetPulseAcc
 *  �õ������ۼӵ�ֵ
 * 
 *  ����:
 *      ��
 *  ���:
 *      �����ۼ�ֵ
 */
uint16 LPLD_LPTMR_GetPulseAcc(void)
{
   LPTMR0->CNR=111;//�����òŸ�������
   return (uint16)LPTMR0->CNR;
}
