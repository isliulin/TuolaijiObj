#ifndef __ap_S32K1XX_LPSPI_H_
#define __ap_S32K1XX_LPSPI_H_
#include "S32K144.h"

//模式
#define		MASTER		0//主机模式
#define		SLAVE    	1 //主机模式

//复用
//spi0
#define SPI0_SIN_MUX()			PORTB->PCR3 = PORT_PCR_MUX(3) 
#define SPI0_SCK_MUX()			PORTB->PCR2 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK
#define SPI0_SOUT_MUX()		PORTB->PCR4 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK
#define SPI0_PCS0_MUX()		PORTB->PCR0 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK
//spi1
#define SPI1_SIN_MUX()			PORTB->PCR15 = PORT_PCR_MUX(3) 
#define SPI1_SCK_MUX()			PORTB->PCR14 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK
#define SPI1_SOUT_MUX()		PORTB->PCR16 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK
#define SPI1_PCS0_MUX()		PORTB->PCR17 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK
//spi2
#define SPI2_SIN_MUX()			PORTB->PCR3 = PORT_PCR_MUX(3) 
#define SPI2_SCK_MUX()			PORTB->PCR2 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK
#define SPI2_SOUT_MUX()		PORTB->PCR4 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK
#define SPI2_PCS0_MUX()		PORTB->PCR0 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK

void lpspi_send(uint8_t pspiNo,uint8_t pdata[],uint32_t plen);
uint8_t lpspiInit(uint8_t pspiNo,uint8_t pmaster);
#endif