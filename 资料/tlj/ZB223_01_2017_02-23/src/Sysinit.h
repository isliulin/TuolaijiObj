/********************************************************************/
#ifndef __SYSINIT_H_
#define __SYSINIT_H_
#include "DS1302.h"
#include "HwMacroPatch.h"

extern uint32_t bus_clk_khz;        //外围总线时钟

#define SPI3_MISO_MUX()  	PORTD->PCR1 = PORT_PCR_MUX(1)       //send_data9
#define SPI3_MISO_PDDR() 	GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO1) 
#define SPI3_MISO_H     	GPIOD_PSOR = GPIO1
#define SPI3_MISO_L    	GPIOD_PCOR = GPIO1
#define SPI3_MOSI_MUX()  	PORTD->PCR2 = PORT_PCR_MUX(1)       //send_data9
#define SPI3_MOSI_PDDR() 	GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO2) 
#define SPI3_MOSI_H     	GPIOD_PSOR = GPIO2
#define SPI3_MOSI_L    	GPIOD_PCOR = GPIO2
#define SPI3_SCK_MUX()  	PORTD->PCR0 = PORT_PCR_MUX(1)       //send_data9
#define SPI3_SCK_PDDR() 	GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO0) 
#define SPI3_SCK_H     	GPIOD_PSOR = GPIO0
#define SPI3_SCK_L    		GPIOD_PCOR = GPIO0
#define SPI3_CS_MUX()  	PORTC->PCR10 = PORT_PCR_MUX(1)       //send_data9
#define SPI3_CS_PDDR() 	GPIOC_PDDR |= GPIO_PDDR_PDD(GPIO10) 
#define SPI3_CS_H     		GPIOC_PSOR = GPIO10
#define SPI3_CS_L    		GPIOC_PCOR = GPIO10









void sysInit(void);

#endif
/********************************************************************/
