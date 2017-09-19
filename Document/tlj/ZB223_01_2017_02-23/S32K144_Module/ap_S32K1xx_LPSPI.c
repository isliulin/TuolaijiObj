#include "ap_S32K1xx_LPSPI.h"

uint8_t lpspiInit(uint8_t pspiNo,uint8_t pmaster)
{
	LPSPI_Type *baseAdd;

	if(pspiNo == 0)
	{
		PCC->LPSPI0r = PCC_LPSPI0_CGC(1)|PCC_LPSPI0_PCS(3);
		SPI0_SIN_MUX();
		SPI0_SCK_MUX();
		SPI0_SOUT_MUX();
		SPI0_PCS0_MUX();
		baseAdd = LPSPI0;
	}
	else if(pspiNo == 1)
	{
		PCC->LPSPI1r = PCC_LPSPI1_CGC(1)|PCC_LPSPI1_PCS(3);
		SPI1_SIN_MUX();
		SPI1_SCK_MUX();
		SPI1_SOUT_MUX();
		SPI1_PCS0_MUX();
		baseAdd = LPSPI1;
	}
	else if(pspiNo == 2)
	{
		PCC->LPSPI2r = PCC_LPSPI2_CGC(1)|PCC_LPSPI2_PCS(3);
		SPI2_SIN_MUX();
		SPI2_SCK_MUX();
		SPI2_SOUT_MUX();
		SPI2_PCS0_MUX();
		baseAdd = LPSPI2;
	}
	else return 1;
	
	baseAdd->CR |= LPSPI_CR_RST_MASK; 	//Master logic is reset.
	baseAdd->CR |= 0
				//| LPSPI_CR_MEN_MASK	//Module is enabled.
				| LPSPI_CR_RRF_MASK //复位发送与接收FIFO
				| LPSPI_CR_RTF_MASK
				;
	if(pmaster == MASTER)
	{
		baseAdd->CFGR1 =  LPSPI_CFGR1_MASTER_MASK	//Master mode.
						//| LPSPI_CFGR1_SAMPLE_MASK	//Input data sampled on delayed SCK edge.
						//| LPSPI_CFGR1_PCSCFG_MASK
						//| LPSPI_CFGR1_OUTCFG_MASK
						| LPSPI_CFGR1_PINCFG(0)
						| LPSPI_CFGR1_NOSTALL_MASK
						;
		baseAdd->TCR= 0
					| LPSPI_TCR_CPOL_MASK
					| LPSPI_TCR_CPHA_MASK
					| LPSPI_TCR_PRESCALE(2) //Divide by 4. SPI最大频率25M
					| LPSPI_TCR_LSBF(0)	//0 :T MSB first
					| LPSPI_TCR_BYSW(0)	//0 :Byte swap disabled
					| LPSPI_TCR_CONT(0)	//0 :continuous transter disabled
					| LPSPI_TCR_FRAMESZ(7)
					| LPSPI_TCR_PCS(3)
					;
		baseAdd->CCR = 0
					 | LPSPI_CCR_SCKDIV(100)
					 ;
	}
	else
	{
		baseAdd->CFGR1 &= ~LPSPI_CFGR1_MASTER_MASK;//slave mode.
	}
	baseAdd->SR = LPSPI_SR_DMF_MASK
				| LPSPI_SR_REF_MASK
				| LPSPI_SR_TEF_MASK
				| LPSPI_SR_TCF_MASK
				| LPSPI_SR_FCF_MASK
				| LPSPI_SR_WCF_MASK
				;
	baseAdd->CR |= 0
				| LPSPI_CR_MEN_MASK	//Module is enabled.
				//| LPSPI_CR_DBGEN_MASK
				;
	baseAdd->FCR = 0 
				 | LPSPI_FCR_TXWATER(1)
				 | LPSPI_FCR_RXWATER(1)
				 ;
	return 0;
}

void lpspi_send(uint8_t pspiNo,uint8_t pdata[],uint32_t plen)
{
	uint32_t i = 0;
    LPSPI_Type * baseAdd;
	if(pspiNo == 0)
	{
		baseAdd = LPSPI0;
	}
	else if(pspiNo == 1)
	{
		baseAdd = LPSPI1;
	}
	else if(pspiNo == 0)
	{
		baseAdd = LPSPI2;
	}
	else return;

	for(i = 0; i < plen; i++)
	{
		baseAdd->SR = LPSPI_SR_DMF_MASK
				| LPSPI_SR_REF_MASK
				| LPSPI_SR_TEF_MASK
				| LPSPI_SR_TCF_MASK
				| LPSPI_SR_FCF_MASK
				| LPSPI_SR_WCF_MASK
				;
	baseAdd->TDR = pdata[i];
	while(!(LPSPI_SR_TCF_MASK & baseAdd->SR));
	}
}