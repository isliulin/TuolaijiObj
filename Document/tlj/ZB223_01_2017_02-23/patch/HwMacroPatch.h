//2016/6/1
#ifndef __HwMACRO_H_
#define __HwMACRO_H_
#include "S32K144.h"
#include "ap_S32K1xx_FTM.h"


#define ZB223_1		0
#define BOARD		ZB223_1

#define OLD_pressure 	0
#define NEW_pressure 	1


//common define
/********************************************************************/
#define GPIO0	0x00000001
#define GPIO1	0x00000002
#define GPIO2	0x00000004
#define GPIO3	0x00000008
#define GPIO4	0x00000010
#define GPIO5	0x00000020
#define GPIO6	0x00000040
#define GPIO7	0x00000080
#define GPIO8	0x00000100
#define GPIO9	0x00000200
#define GPIO10	0x00000400
#define GPIO11	0x00000800
#define GPIO12	0x00001000
#define GPIO13	0x00002000
#define GPIO14	0x00004000
#define GPIO15	0x00008000
#define GPIO16	0x00010000
#define GPIO17	0x00020000
#define GPIO18	0x00040000
#define GPIO19	0x00080000





//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
#define	Mcu_U15					0
#define	Mcu_U22					0
#define	Mcu_U24					1
#define	Mcu_U25					1
#define	Mcu_U23					0
#define	Mcu_U26					0

//ADC  采样口
// PTC7 ----- ADC0                               ADC1_SE5
// PTA0 ----- ADC1             ADC0_SE0
// PTC2 ----- ADC2             ADC0_SE10
// PTE6 ----- ADC3                              ADC1_SE11
// PTC14 -----ADC4            ADC0_SE12
// PTA7 ----- ADC5             ADC0_SE3
// PTC6 ----- ADC_KEY                         ADC1_SE4   
// PTA16 ----- 光敏电阻                         ADC1_SE13  
// PTE2   ----- 12V总电adc                    ADC1_SE10  
// PTA6   ----- 12V摄像头电源  ADC0_SE2  
// PTC0   ----- F01                ADC0_SE8 


//按键adc口
#define BUTTON_ADC_MODULE	1
#define BUTTON_ADC_Ch	       	4
//----------------------------------------------------------------------------
//光敏电阻adc口
#define OPTO_ADC_MODULE		1
#define OPTO_ADC_Ch			13




#define HC4051_DA0_MUX()  	PORTB->PCR0 = PORT_PCR_MUX(1) 
#define HC4051_DA0_PDDR_O 	GPIOB_PDDR |= GPIO_PDDR_PDD(GPIO0)  
#define HC4051_DA0_H     	GPIOB_PSOR  = GPIO0
#define HC4051_DA0_L    	       GPIOB_PCOR  = GPIO0    
#define HC4051_DB0_MUX()  	PORTC->PCR13 = PORT_PCR_MUX(1) 
#define HC4051_DB0_PDDR_O 	GPIOC_PDDR |= GPIO_PDDR_PDD(GPIO13)  
#define HC4051_DB0_H     	GPIOC_PSOR = GPIO13
#define HC4051_DB0_L      	GPIOC_PCOR = GPIO13  
#define HC4051_DC0_MUX()  	PORTA->PCR1 = PORT_PCR_MUX(1) 
#define HC4051_DC0_PDDR_O 	GPIOA_PDDR |= GPIO_PDDR_PDD(GPIO1)  
#define HC4051_DC0_H     	GPIOA_PSOR = GPIO1
#define HC4051_DC0_L    	       GPIOA_PCOR = GPIO1  
#define HC4051_DA1_MUX()  	PORTD->PCR14 = PORT_PCR_MUX(1) 
#define HC4051_DA1_PDDR_O 	GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO14)  
#define HC4051_DA1_H     	GPIOD_PSOR = GPIO14
#define HC4051_DA1_L    	       GPIOD_PCOR = GPIO14    
#define HC4051_DB1_MUX()  	PORTD->PCR17 = PORT_PCR_MUX(1) 
#define HC4051_DB1_PDDR_O 	GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO17)  
#define HC4051_DB1_H    	       GPIOD_PSOR = GPIO17
#define HC4051_DB1_L       	GPIOD_PCOR = GPIO17  
#define HC4051_DC1_MUX()  	PORTE->PCR12 = PORT_PCR_MUX(1) 
#define HC4051_DC1_PDDR_O 	GPIOE_PDDR |= GPIO_PDDR_PDD(GPIO12)  
#define HC4051_DC1_H     	GPIOE_PSOR = GPIO12
#define HC4051_DC1_L    	       GPIOE_PCOR = GPIO12  

//----------------负载输出引脚宏定义------------------------------------------------------------------
#define LOAD_DSEL_MUX()   		PORTD->PCR7 = PORT_PCR_MUX(1) 		
#define LOAD_DSEL_OUT  		       GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO7) 
#define LOAD_DSEL_H       		GPIOD_PSOR = GPIO7
#define LOAD_DSEL_L       		       GPIOD_PCOR = GPIO7
#define LOAD_DSEL_READ			(GPIOD_PDIR & GPIO7) >> 7

//223YY01 IN1 通过跳线与adc2 对调
#define JA1_LOAD_OUTPUT_MUX()   		PORTD->PCR6 = PORT_PCR_MUX(1) 			//负载1
#define JA1_LOAD_OUTPUT_OUT  			GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO6) 
#define JA1_LOAD_OUTPUT_H       		       GPIOD_PSOR = GPIO6
#define JA1_LOAD_OUTPUT_L       		       GPIOD_PCOR = GPIO6
#define JA1_LOAD_OUTPUT_READ			(GPIOD_PDIR & GPIO6) >> 6
#define JA2_LOAD_OUTPUT_MUX()   		PORTB->PCR5 = PORT_PCR_MUX(1) 			//负载2
#define JA2_LOAD_OUTPUT_OUT  			GPIOB_PDDR |= GPIO_PDDR_PDD(GPIO5) 
#define JA2_LOAD_OUTPUT_H       		       GPIOB_PSOR = GPIO5
#define JA2_LOAD_OUTPUT_L       		       GPIOB_PCOR = GPIO5
#define JA2_LOAD_OUTPUT_READ			(GPIOB_PDIR & GPIO5) >> 5
#define LOAD_ADC_MOUDLE				0	
#define LOAD_ADC_CHANNL				11


#define LEVEL1_MUX()  	        PORTC->PCR8 = PORT_PCR_MUX(1) 
#define LEVEL1_PDDR_O 	        GPIOC_PDDR |= GPIO_PDDR_PDD(GPIO8)  
#define LEVEL1_H     		        GPIOC_PSOR  = GPIO8
#define LEVEL1_L    		        GPIOC_PCOR  = GPIO8 
#define LEVEL2_MUX()  	        PORTC->PCR9 = PORT_PCR_MUX(1) 
#define LEVEL2_PDDR_O 	        GPIOC_PDDR |= GPIO_PDDR_PDD(GPIO9)  
#define LEVEL2_H     		        GPIOC_PSOR  = GPIO9
#define LEVEL2_L    		        GPIOC_PCOR  = GPIO9 
#define LEVEL3_MUX()  	        PORTD->PCR8 = PORT_PCR_MUX(1) 
#define LEVEL3_PDDR_O 	        GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO8)  
#define LEVEL3_H     		        GPIOD_PSOR  = GPIO8
#define LEVEL3_L    		        GPIOD_PCOR  = GPIO8 



//背光灯
#define LED5_MUX()                PORTA->PCR11 = PORT_PCR_MUX(1) 
#define LED5_PDDR()              GPIOA_PDDR |= GPIO_PDDR_PDD(GPIO11)  
#define LED5_ON                    GPIOA_PSOR = GPIO11
#define LED5_OFF                   GPIOA_PCOR = GPIO11       
#define LED5_TOGGLE            GPIOA_PTOR = GPIO11     

//HC595
#define HC595_CLOCK_MUX()     PORTB->PCR8 = PORT_PCR_MUX(1)|PORT_PCR_DSE(0)|PORT_PCR_ODE(0)|PORT_PCR_PE(1)|PORT_PCR_PS(0)       //clock
#define HC595_CLOCK_PDDR()   GPIOB_PDDR |= GPIO_PDDR_PDD(GPIO8) 
#define HC595_CLOCK_H            GPIOB_PSOR = GPIO8
#define HC595_CLOCK_L             GPIOB_PCOR = GPIO8

#define HC595_OUT_MUX()         PORTB->PCR9 = PORT_PCR_MUX(1)      //out
#define HC595_OUT_PDDR()       GPIOB_PDDR |= GPIO_PDDR_PDD(GPIO9) 
#define HC595_OUT_H                GPIOB_PSOR = GPIO9
#define HC595_OUT_L                 GPIOB_PCOR = GPIO9

#define HC595_OE_MUX()           PORTB->PCR10 = PORT_PCR_MUX(1)       //oe
#define HC595_OE_PDDR()         GPIOB_PDDR |= GPIO_PDDR_PDD(GPIO10) 
#define HC595_OE_H                  GPIOB_PSOR = GPIO10
#define HC595_OE_L                   GPIOB_PCOR = GPIO10

#define HC595_DATA1_MUX()       PORTC->PCR17 = PORT_PCR_MUX(1)       //send_data1
#define HC595_DATA1_PDDR()     GPIOC_PDDR |= GPIO_PDDR_PDD(GPIO17) 
#define HC595_DATA1_H              GPIOC_PSOR = GPIO17
#define HC595_DATA1_L              GPIOC_PCOR = GPIO17
#define HC595_DATA2_MUX()       PORTC->PCR16 = PORT_PCR_MUX(1)       //send_data2
#define HC595_DATA2_PDDR()     GPIOC_PDDR |= GPIO_PDDR_PDD(GPIO16) 
#define HC595_DATA2_H              GPIOC_PSOR = GPIO16
#define HC595_DATA2_L              GPIOC_PCOR = GPIO16

#define HC595_DATA9_MUX()      PORTA->PCR17 = PORT_PCR_MUX(1)       //send_data9
#define HC595_DATA9_PDDR()    GPIOA_PDDR |= GPIO_PDDR_PDD(GPIO17) 
#define HC595_DATA9_H            GPIOA_PSOR = GPIO17
#define HC595_DATA9_L             GPIOA_PCOR = GPIO17



//引脚定义
#define DS1302_CE_MUX()   			PORTD->PCR13 = PORT_PCR_MUX(1) 	//RST脚
#define DS1302_CE_PDDR_OUT  		GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO13) 
#define DS1302_CE_PDDR_IN   		GPIOD_PDDR &= ~GPIO13
#define DS1302_CE_H       			       GPIOD_PSOR = GPIO13
#define DS1302_CE_L       			       GPIOD_PCOR = GPIO13 

#define DS1302_CK_MUX()   			PORTB->PCR2 = PORT_PCR_MUX(1) 
#define DS1302_CK_PDDR_OUT  		GPIOB_PDDR |= GPIO_PDDR_PDD(GPIO2) 
#define DS1302_CK_PDDR_IN   		GPIOB_PDDR &= ~GPIO2
#define DS1302_CK_H       			       GPIOB_PSOR = GPIO2
#define DS1302_CK_L       			       GPIOB_PCOR = GPIO2 

#define DS1302_IO_MUX()   			PORTB->PCR3 = PORT_PCR_MUX(1) 
#define DS1302_IO_PDDR_OUT  		GPIOB_PDDR |= GPIO_PDDR_PDD(GPIO3) 
#define DS1302_IO_PDDR_IN   		GPIOB_PDDR &= ~GPIO3 
#define DS1302_IO_H       			       GPIOB_PSOR = GPIO3
#define DS1302_IO_L       			       GPIOB_PCOR = GPIO3
#define DS1302_IO_READ    			(GPIOB_PDIR & GPIO3) >> 3



/*********IO macros**********/
#define IS31FL_SCL_MUX()   		PORTE->PCR3 = PORT_PCR_MUX(1) 
#define IS31FL_SCL_PDDR_OUT  	GPIOE_PDDR |= GPIO_PDDR_PDD(GPIO3) 
#define IS31FL_SCL_PDDR_IN   	GPIOE_PDDR &= ~GPIO3
#define IS31FL_SCL_H       		GPIOE_PSOR = GPIO3
#define IS31FL_SCL_L       		      GPIOE_PCOR = GPIO3

#define IS31FL_SDA_MUX()   		PORTE->PCR14 = PORT_PCR_MUX(1) 
#define IS31FL_SDA_PDDR_OUT  	GPIOE_PDDR |= GPIO_PDDR_PDD(GPIO14) 
#define IS31FL_SDA_PDDR_IN   	GPIOE_PDDR &= ~GPIO14 
#define IS31FL_SDA_H       		GPIOE_PSOR = GPIO14
#define IS31FL_SDA_L       		GPIOE_PCOR = GPIO14
#define IS31FL_SDA_READ    		(GPIOE_PDIR & GPIO14) >> 14



#define Eeprom_SCL_MUX()   		IS31FL_SCL_MUX()
#define Eeprom_SCL_PDDR_OUT  	IS31FL_SCL_PDDR_OUT
#define Eeprom_SCL_PDDR_IN   	IS31FL_SCL_PDDR_IN
#define Eeprom_SCL_H       		IS31FL_SCL_H       		
#define Eeprom_SCL_L       		IS31FL_SCL_L
   
#define Eeprom_SDA_MUX()   		IS31FL_SDA_MUX() 
#define Eeprom_SDA_PDDR_OUT  	IS31FL_SDA_PDDR_OUT
#define Eeprom_SDA_PDDR_IN   	IS31FL_SDA_PDDR_IN
#define Eeprom_SDA_H       		IS31FL_SDA_H
#define Eeprom_SDA_L       		IS31FL_SDA_L
#define Eeprom_SDA_READ    		IS31FL_SDA_READ



#define IS31FL_SDB_MUX()            PORTB->PCR11 = PORT_PCR_MUX(1)       //send_data5
#define IS31FL_SDB_PDDR()          GPIOB_PDDR |= GPIO_PDDR_PDD(GPIO11) 
#define IS31FL_SDB_H                   GPIOB_PSOR = GPIO11
#define IS31FL_SDB_L                    GPIOB_PCOR = GPIO11

#define IS31FL_INTB_MUX()   		PORTC->PCR1 = PORT_PCR_MUX(1) //
#define IS31FL_INTB_PDDR   		GPIOC_PDDR &= ~GPIO1 
#define IS31FL_INTB_H       		GPIOC_PSOR = GPIO1
#define IS31FL_INTB_L       		GPIOC_PCOR = GPIO1
#define IS31FL_INTB_READ    		(GPIOC_PDIR & GPIO1) >> 1


#define CAN0_RS_MUX()   	      PORTD->PCR12 = PORT_PCR_MUX(1)
#define CAN0_RS_PDDR_OUT  GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO12) 
#define CAN0_RS_PDDR_IN     GPIOD_PDDR &= ~GPIO12
#define CAN0_RS_H       	      GPIOD_PSOR = GPIO12
#define CAN0_RS_L       	      GPIOD_PCOR = GPIO12
#define CAN0_RS_READ    	     ((GPIOD_PDIR & GPIO12) >> 12)


#define CAN1_RS_MUX()     	PORTA->PCR8 = PORT_PCR_MUX(1) 
#define CAN1_RS_PDDR_OUT  GPIOA_PDDR |= GPIO_PDDR_PDD(GPIO8) 
#define CAN1_RS_PDDR_IN     GPIOA_PDDR &= ~GPIO8
#define CAN1_RS_H       	      GPIOA_PSOR = GPIO8
#define CAN1_RS_L       	      GPIOA_PCOR = GPIO8
#define CAN1_RS_READ    	      ((GPIOA_PDIR & GPIO8) >> 8)

#define CAN2_RS_MUX()   	      PORTD->PCR4 = PORT_PCR_MUX(1) 
#define CAN2_RS_PDDR_OUT  GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO4) 
#define CAN2_RS_PDDR_IN     GPIOD_PDDR &= ~GPIO4 
#define CAN2_RS_H       	      GPIOD_PSOR = GPIO4
#define CAN2_RS_L       	      GPIOD_PCOR = GPIO4
#define CAN2_RS_READ    	     ((GPIOD_PDIR & GPIO4) >> 4)


#define I2C_SCL_MUX()            PORTA->PCR3 = PORT_PCR_MUX(1) 
#define I2C_SCL_PDDR_OUT    GPIOA_PDDR |= GPIO_PDDR_PDD(GPIO3) 
#define I2C_SCL_PDDR_IN       GPIOA_PDDR &= ~GPIO3
#define I2C_SCL_H                   GPIOA_PSOR = GPIO3
#define I2C_SCL_L                   GPIOA_PCOR = GPIO3 
#define I2C_SDA_MUX()            PORTA->PCR2 = PORT_PCR_MUX(1) 
#define I2C_SDA_PDDR_OUT    GPIOA_PDDR |= GPIO_PDDR_PDD(GPIO2) 
#define I2C_SDA_PDDR_IN       GPIOA_PDDR &= ~GPIO2 
#define I2C_SDA_H                   GPIOA_PSOR = GPIO2
#define I2C_SDA_L                    GPIOA_PCOR = GPIO2
#define I2C_SDA_READ             (GPIOA_PDIR & GPIO2) >> 2



#define MODE_CTRL_MUX()                  PORTE->PCR15 = PORT_PCR_MUX(1) 
#define MODE_CTRL_PDDR_OUT           GPIOE_PDDR |= GPIO_PDDR_PDD(GPIO15) 
#define MODE_CTRL_H                          GPIOE_PSOR = GPIO15
#define MODE_CTRL_L                           GPIOE_PCOR = GPIO15 
#define DOWNLOAD_CTRL_MUX()          PORTA->PCR14 = PORT_PCR_MUX(1) 
#define DOWNLOAD_CTRL_PDDR_OUT  GPIOA_PDDR |= GPIO_PDDR_PDD(GPIO14) 
#define DOWNLOAD_CTRL_H                 GPIOA_PSOR = GPIO14
#define DOWNLOAD_CTRL_L                  GPIOA_PCOR = GPIO14 

#define ISD_MISO_MUX()                       PORTD->PCR1 = PORT_PCR_MUX(1) 
#define ISD_MISO_PDDR_OUT               GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO1) 
#define ISD_MISO_PDDR_IN                  GPIOD_PDDR &= ~GPIO1 
#define ISD_MISO_H                              GPIOD_PSOR = GPIO1
#define ISD_MISO_L                              GPIOD_PCOR = GPIO1 
#define ISD_MISO_READ                       (GPIOD_PDIR & GPIO1) >> 1

#define ISD_MOSI_MUX()                       PORTD->PCR2 = PORT_PCR_MUX(1) 
#define ISD_MOSI_PDDR_OUT               GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO2) 
#define ISD_MOSI_PDDR_IN                  GPIOD_PDDR |= ~GPIO2
#define ISD_MOSI_H                             GPIOD_PSOR = GPIO2
#define ISD_MOSI_L                              GPIOD_PCOR = GPIO2

#define ISD_SCLK_MUX()                       PORTD->PCR0 = PORT_PCR_MUX(1) 
#define ISD_SCLK_PDDR_OUT                GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO0) 
#define ISD_SCLK_PDDR_IN                   GPIOD_PDDR &= ~GPIO0 
#define ISD_SCLK_H                               GPIOD_PSOR = GPIO0
#define ISD_SCLK_L                                GPIOD_PCOR = GPIO0
#define ISD_CS_MUX()                             PORTC->PCR10 = PORT_PCR_MUX(1) 
#define ISD_CS_PDDR_OUT                     GPIOC_PDDR |= GPIO_PDDR_PDD(GPIO10) 
#define ISD_CS_PDDR_IN                        GPIOC_PDDR &= ~GPIO10
#define ISD_CS_H                                    GPIOC_PSOR = GPIO10
#define ISD_CS_L                                    GPIOC_PCOR = GPIO10

/*SPI IO*/
#define SPI0_CS_MUX()             CAN0_RS_MUX()  //CAN0_RS
#define SPI0_CS_PDDR(x) 	        x?(CAN0_RS_PDDR_OUT) :(CAN0_RS_PDDR_IN)
#define SPI0_CS(x)       	        x?(CAN0_RS_H):(CAN0_RS_L)

#define SPI0_SOUT_MUX()         ISD_MOSI_MUX()  
#define SPI0_SOUT_PDDR(x)     x?(ISD_MOSI_PDDR_OUT) :(ISD_MOSI_PDDR_IN)
#define SPI0_SOUT(x)                x?(ISD_MOSI_H):(ISD_MOSI_L)

#define SPI0_SIN_MUX()           ISD_MISO_MUX()
#define SPI0_SIN_PDDR(x) 	 x?(ISD_MOSI_PDDR_OUT) :(ISD_MOSI_PDDR_IN)
#define SPI0_SIN()  		       ISD_MISO_READ

#define SPI0_SCK_MUX()          ISD_SCLK_MUX()  
#define SPI0_SCK_PDDR(x) 	x?(ISD_SCLK_PDDR_OUT) :(ISD_SCLK_PDDR_IN)
#define SPI0_SCK(x)       	       x?(ISD_SCLK_H):(ISD_SCLK_L)


//看门狗引脚定义
#define WDT_MUX()        PORTA->PCR9 = PORT_PCR_MUX(1) 
#define WDT_PDDR()      GPIOA_PDDR |= GPIO_PDDR_PDD(GPIO9)  
#define WDT_ON            GPIOA_PSOR = GPIO9
#define WDT_OFF           GPIOA_PCOR = GPIO9       
#define WDT_TOGGLE    GPIOA_PTOR = GPIO9 
#define FeedWatchDog() WDT_TOGGLE

//***************************电源控制**********************************//

#define V_POWER_V1_MUX()  	PORTD->PCR10 = PORT_PCR_MUX(1) 
#define V_POWER_V1_PDDR_O 	GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO10)  
#define V_POWER_V1_H     	GPIOD_PSOR = GPIO10
#define V_POWER_V1_L    	GPIOD_PCOR = GPIO10   
//////////////V-POW3->V3  +24v  开关////////////////////     
#define V_POWER_V3_MUX()  	PORTD->PCR11 = PORT_PCR_MUX(1) 
#define V_POWER_V3_PDDR_O 	GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO11)  
#define V_POWER_V3_H     	GPIOD_PSOR = GPIO11
#define V_POWER_V3_L    	GPIOD_PCOR = GPIO11   



//v-12  电源总电开关
#define POWER_12V_MUX()   		PORTE->PCR13 = PORT_PCR_MUX(1) 		//视频电源 VV-12
#define POWER_12V_OUT  			GPIOE_PDDR |= GPIO_PDDR_PDD(GPIO13) 
#define POWER_12V_H       		       GPIOE_PSOR = GPIO13
#define POWER_12V_L       		       GPIOE_PCOR = GPIO13
#define POWER_12V_READ			(GPIOE_PDIR & GPIO13) >> 13
#define POWER_12V_ON    		       POWER_12V_L 
#define POWER_12V_OFF		              POWER_12V_H

//+12VV  车速传感器 脉冲捕获电源
#define POWER_12VV_MUX()   		       PORTD->PCR3 = PORT_PCR_MUX(1) 		//视频电源 VV-12
#define POWER_12VV_OUT  			GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO3) 
#define POWER_12VV_H       		       GPIOD_PSOR = GPIO3
#define  POWER_12VV_L      		       GPIOD_PCOR = GPIO3
#define POWER_12VV_READ			(GPIOD_PDIR & GPIO3) >> 3

//ap 摄像头共用一个电源
#define TV_POWER_MUX()   		PORTE->PCR7 = PORT_PCR_MUX(1) 		//视频电源 VV-12
#define TV_POWER_OUT  			GPIOE_PDDR |= GPIO_PDDR_PDD(GPIO7) 
#define TV_POWER_H       		       GPIOE_PSOR = GPIO7
#define TV_POWER_L       		       GPIOE_PCOR = GPIO7
#define TV_POWER_READ			(GPIOE_PDIR & GPIO7) >> 7


#define TV_POWER_5V_MUX()          PORTE->PCR16 = PORT_PCR_MUX(1) 
#define TV_POWER_5V_PDDR_OUT  GPIOE_PDDR |= GPIO_PDDR_PDD(GPIO16) 
#define TV_POWER_5V_H                 GPIOE_PSOR = GPIO16
#define TV_POWER_5V_L                  GPIOE_PCOR = GPIO16 


#define MCU_5VEN_MUX()  	PORTD->PCR5 = PORT_PCR_MUX(1) 
#define MCU_5VEN_PDDR_O 	GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO5)  
#define MCU_5VEN_H     		GPIOD_PSOR  = GPIO5
#define MCU_5VEN_L    		GPIOD_PCOR  = GPIO5 


#define AN_POW_MUX()  	       PORTD->PCR9 = PORT_PCR_MUX(1) 
#define AN_POW_PDDR_O 	GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO9)  
#define AN_POW_H     		GPIOD_PSOR = GPIO9
#define AN_POW_L    		      GPIOD_PCOR = GPIO9 
#define AN_POW_ON     	      AN_POW_H
#define AN_POW_OFF    	      AN_POW_L 

#define AN1_POW_MUX()  	       PORTB->PCR1 = PORT_PCR_MUX(1) 
#define AN1_POW_PDDR_O 	GPIOB_PDDR |= GPIO_PDDR_PDD(GPIO1)  
#define AN1_POW_H     		GPIOB_PSOR = GPIO1
#define AN1_POW_L    	      GPIOB_PCOR = GPIO1
#define AN1_POW_ON     	      AN1_POW_H
#define AN1_POW_OFF    	      AN1_POW_L 

/***************************** PWM *******************************************/

//PWM 频率定义HZ
#define SOUND_FRQ					       2550//1250		//喇叭
#define TFT_LINGHT_FRQ				       SOUND_FRQ	//lcd背光
#define METER_BACK_LIGHT_FRQ		       SOUND_FRQ	//仪表背光
#define PRESSURE_BACKLIGHT_FRQ		SOUND_FRQ
#define STEP_MOTOR_FRQ				30000		//仪表指针频率

////////////////////////////////////////////////////////////////////////////
/*** PWM 定义 **/
/* 仪表背光灯  led5*/
#define   BACKGROUD_LED_PORT_MUX() 	   PORTA_PCR11 = PORT_PCR_MUX(0x2) // 
#define   BACKGROUD_LED_MODULE  	          ftm1
#define	BACKGROUD_LED_CHANNL  	          CH5
#define	BACKGROUD_LED_DUTY		 	    FTM1->C5V
/* 声音*/
#define   SOUND_POW_PORT_MUX() 	          PORTC_PCR15 = PORT_PCR15_MUX(0x2); //  // 
#define   SOUND_POW_MODULE  			   ftm1
#define	SOUND_POW_CHANNL  			   CH3
#define	SOUND_POW_DUTY		 		   FTM1->C3V

/* LCD背光 */
#define   LCDBACKLIGHT_PORT_MUX() 	         PORTA_PCR15 = PORT_PCR15_MUX(0x2);
#define	LCDBACKLIGHT_MODULE			  ftm1
#define	LCDBACKLIGHT_CH				  CH2
#define	LCDBACKLIGHT_DUTY			  FTM1->C2V 

//----------------------------------------------------------------------------
//气压背光控制LED-PWM
//#define	PRESSURE_BACKLIGHT_MODULE	     ftm1
//#define	PRESSURE_BACKLIGHT_CH		     CH7
//#define   PRESSURE_PWMVALUE		            FTM1->C7V 


/* PWM 输出*/
//----------------------------------------------------------------------------
//PWM1
//#define PWMOUT1_PORT_MUX() 	    PORTC_PCR11 = PORT_PCR_MUX(0x2)
//#define	PWMOUT1_MODULE			ftm2                    //   捕获3
//#define	PWMOUT1_CH				CH5

#define PWMOUT1_PORT_MUX()   	       PORTE->PCR11 = PORT_PCR_MUX(1) 
#define PWMOUT1_PDDR_O 	             GPIOE_PDDR |= GPIO_PDDR_PDD(GPIO11)  
#define PWMOUT1_H     		             GPIOE_PSOR = GPIO11
#define PWMOUT1_L    	                    GPIOE_PCOR = GPIO11
#define PWMOUT1_Toggle   	             GPIOE_PTOR = GPIO11



//----------------------------------------------------------------------------
//PWM2
#define   PWMOUT2_PORT_MUX() 	    PORTE_PCR10 = PORT_PCR_MUX(0x4);
#define	PWMOUT2_MODULE			ftm2
#define	PWMOUT2_CH				CH4


// 捕获3  pc12_2   6通道
// 捕获3  pc11_2   5通道
#define PWMIN_CPSR_PORT_MUX() 	    PORTC_PCR11 = PORT_PCR_MUX(0x2);
#define PWMIN_CPRPM_PORT_MUX() 	    PORTC_PCR12 = PORT_PCR_MUX(0x2); 












#endif

