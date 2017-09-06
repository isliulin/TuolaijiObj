
/*******************************************************************************
*File Name          : 2124_i2c_.h 
*Author             : MCD Application Team 
* Version            : V2.0.1 
* Date               : 06/10/2015
* Description        : Header for i2c_ee.c module 
********************************************************************************/ 
#ifndef __IS31FL_H__
#define __IS31FL_H__
#include "common.h"






#define Addr_GND_GND  0xA0
#define Addr_GND_SCL  0xA2
#define Addr_GND_SDA  0xA4
#define Addr_GND_VCC  0xA6
#define Addr_SCL_GND  0xA8
#define Addr_SCL_SCL  0xAA
#define Addr_SCL_SDA  0xAC
#define Addr_SCL_VCC  0xAE
#define Addr_SDA_GND  0xB0
#define Addr_SDA_SCL  0xB2
#define Addr_SDA_SDA  0xB4
#define Addr_SDA_VCC  0xB6
#define Addr_VCC_GND  0xB8
#define Addr_VCC_SCL  0xBA
#define Addr_VCC_SDA  0xBC
#define Addr_VCC_VCC  0xAE

#define g_MOVIE_fireworks 0x01 					
#define g_MOVIE_love      0x02 						
#define g_MOVIE_water	  0x03 	


#define	IS31FL_OK			0x80
#define	IS31FL_ERROR			0x00
#define	IS31FL_ADDR			Addr_GND_GND

extern void  IS31FL_IO_MACROS(void);
extern void  IS_IIC_WriteByte(unsigned char internal_addr,unsigned char dat);
extern void  IS_IIC_ReadByte(unsigned char internal_addr,unsigned char *dat);

#define LEDDRV_SW1      0
#define LEDDRV_SW2      1
#define LEDDRV_SW3      2
#define LEDDRV_SW4      3
#define LEDDRV_SW5      4
#define LEDDRV_SW6      5

#define LEDDRV_CS1      0
#define LEDDRV_CS2      1
#define LEDDRV_CS3      2
#define LEDDRV_CS4      3
#define LEDDRV_CS5      4
#define LEDDRV_CS6      5
#define LEDDRV_CS7      6
#define LEDDRV_CS8      7

extern void Init_3738(void);
extern void IS31FL3738_SetLedPWM(uint8 sw,uint8 cs, uint8 pwmVal);
extern void IS31fL_SetColumnLed(uint16 oldLedSt,uint16 newLedSt,uint8 startLedNumber, const uint8 *PWM_Val);

extern void IS31fL_SetLeftLeds(uint8 LedVal,uint8 redMax,  uint8 pwmVal);//¹Ì¶¨pwm  sw=1~5//×ó±ß
extern void IS31fL_SetRightLeds(uint8 LedVal,uint8 redMax,  uint8 pwmVal);//¹Ì¶¨pwm  sw=1~5//×ó±ß

extern void IS31FL3738_mode1(void);//white LED
extern void IS31FL3738_mode2(void);//rainbow
extern void IS31FL3738_mode3(void);//white LED
extern void IS31FL3738_mode4(void);//rainbow
extern void IS31FL3738_mode5(void);//white LED
extern void IS31FL3738_mode6(void);//rainbow
extern void IS31FL3738_mode7(void);//white LED
extern void IS31FL3738_mode8(void);//rainbow



#endif /* __SN3733_I2C_H */ 

