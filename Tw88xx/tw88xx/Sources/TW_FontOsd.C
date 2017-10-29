#define		TW_FontOsd_C
#pragma CODE_SEG  USER_ROM

#include "TW_FontOsd.h"
#include "TW_SpiOsd.h"

///#define	Innolux_Panal
const unsigned char	Tw8835_Innolux_Pannal_Init_Data[]=
{
	0xFF, 0x00,//; Page 0
	0x06, 0x00,0x07, 0x02,0x08, 0x89,0x1F, 0x00,0x40, 0x10,0x41, 0x0C,0x42, 0x02,0x43, 0x01,0x44, 0xEB,0x45, 0x76,
	0x46, 0xD9,0x47, 0x80,0x48, 0x00,0x50, 0x00,0x51, 0x00,0x52, 0x00,0x53, 0x03,0x54, 0x00,0x56, 0x00,0x57, 0x00,
	0x5F, 0x00,0x80, 0x00,0x81, 0x00,0x82, 0x00,0x83, 0x00,0x84, 0x0C,0x85, 0x00,0x86, 0x00,0x88, 0x00,0x89, 0x00,
	0x8A, 0x00,0x8B, 0x00,0x8C, 0x0C,0x8D, 0x00,0x8E, 0x00,0x90, 0x00,0x91, 0x00,0x92, 0x00,0x93, 0x00,0x94, 0x0C,
	0x95, 0x00,0x96, 0x00,0x98, 0x60,0x99, 0x00,0x9A, 0x00,0x9B, 0x00,0x9C, 0x0C,0x9D, 0x00,0x9E, 0x00,0xD4, 0x00,
	0xE0, 0x71,0xE1, 0x77,0xE2, 0x04,0xE3, 0x40,0xE4, 0x84,0xE5, 0x80,0xE6, 0x20,0xE8, 0x71,0xE9, 0x0A,0xEA, 0x0F,
	0xEB, 0x40,0xEC, 0x20,0xED, 0x40,0xEE, 0x20,0xF6, 0x00,0xF7, 0x16,0xF8, 0x01,0xF9, 0x86,0xFA, 0xA0,0xFB, 0x40,
	0xFC, 0x23,0xFD, 0x34,
	
	0xFF, 0x01,  //; Page 1
	0x01, 0x83,0x02, 0x4C,0x04, 0x00,0x05, 0x29,0x06, 0x03,0x07, 0x02,0x08, 0x14,0x09, 0xF0,0x0A, 0x0F,0x0B, 0xD9,
	0x0C, 0xCC,0x0D, 0x00,0x10, 0xA0,0x11, 0x80,0x12, 0x11,0x13, 0x80,0x14, 0x80,0x15, 0x00,0x17, 0x30,0x18, 0x44,
	0x1C, 0x2F,0x1D, 0x7F,0x1E, 0x00,0x20, 0x50,0x21, 0x22,0x22, 0xF0,0x23, 0xD8,0x24, 0xBC,0x25, 0xB8,0x26, 0x44,
	0x27, 0x38,0x28, 0x00,0x29, 0xE0,0x2A, 0x78,0x2B, 0x44,0x2C, 0x30,0x2D, 0x14,0x2E, 0xA5,0x2F, 0xE4,0x33, 0x85, //0X29 00改E0
	0x34, 0x1A,0x35, 0x00,0xC0, 0x01,0xC2, 0x01,0xC3, 0x03,0xC4, 0x5A,0xC5, 0x00,0xC6, 0x20,0xC7, 0x04,0xC8, 0x00,
	0xC9, 0x06,0xCA, 0x06,0xCB, 0x10,0xCC, 0x00,0xCD, 0x54,0xD0, 0x00,0xD1, 0xF0,0xD2, 0xF0,0xD3, 0xF0,0xD4, 0x00,
	0xD5, 0x00,0xD6, 0x10,0xD7, 0x70,0xD8, 0x00,0xD9, 0x04,0xDA, 0x80,0xDB, 0x80,0xDC, 0x20,0xE0, 0x00,0xE1, 0x05,
	0xE2, 0xD9,0xE3, 0x07,0xE4, 0x33,0xE5, 0x31,0xE6, 0x00,0xE7, 0x2A,
	
	0xFF, 0x02,  //; Page 2
	0x01, 0x00,0x02, 0x14,0x03, 0x80,0x04, 0x1B,0x05, 0xA5,0x06, 0x0F,0x07, 0x80,0x08, 0x12,0x09, 0x06,0x0A, 0x14,
	0x0B, 0x12,0x0C, 0xC7,0x0D, 0x91,0x0E, 0x20,0x0F, 0x53,0x10, 0x14,0x11, 0x22,0x12, 0x23,0x13, 0x0D,0x14, 0x09,
	0x15, 0x28,0x16, 0xF0,0x17, 0x01,0x18, 0x10,0x19, 0x22,0x1A, 0x05,0x1B, 0x02,0x1C, 0x50,0x1D, 0x27,0x1E, 0x03,
	0x40, 0x10,0x41, 0x00,0x42, 0x05,0x43, 0x01,0x44, 0x64,0x45, 0xF4,0x46, 0x00,0x47, 0x0A,0x48, 0x36,0x49, 0x10,
	0x4A, 0x00,0x4B, 0x00,0x4C, 0x00,0x4D, 0x44,0x4E, 0x04,0xE0, 0x00,0xE4, 0x21,0xF8, 0x00,0xF9, 0x80,
	
	0xFF, 0x03,  //; Page 3
	0x00, 0x00,0x01, 0x06,0x02, 0x06,0x03, 0x00,0x04, 0x8C,0x05, 0x00,0x06, 0x00,0x07, 0x00,0x08, 0x0F,0x09, 0x00,
	0x0A, 0x00,0x0B, 0x60,0x0C, 0x40,0x0D, 0x00,0x0E, 0x00,0x10, 0x40,0x11, 0x00,0x12, 0x00,0x13, 0x96,0x14, 0x00,
	0x15, 0x01,0x16, 0x28,0x17, 0x00,0x18, 0x00,0x19, 0x00,0x1A, 0x00,0x1B, 0x00,0x1C, 0x00,0x1D, 0x00,0x1E, 0x00,
	0x1F, 0x00,0x20, 0x00,0x21, 0x00,0x22, 0x00,0x23, 0x96,0x24, 0x64,0x25, 0x01,0x26, 0x28,0x27, 0x01,0x28, 0x00,
	0x29, 0x00,0x2A, 0x00,0x2B, 0x00,0x2C, 0x00,0x2D, 0x00,0x2E, 0x00,0x2F, 0x28,0x30, 0x00,0x31, 0x00,0x32, 0x00,
	0x33, 0x96,0x34, 0xC8,0x35, 0x01,0x36, 0x28,0x37, 0x00,0x38, 0x00,0x39, 0x00,0x3A, 0x00,0x3B, 0x00,0x3C, 0x00,
	0x3D, 0x00,0x3E, 0x00,0x3F, 0x50,0x40, 0x00,0x41, 0x00,0x42, 0x01,0x43, 0x96,0x44, 0x2C,0x45, 0x01,0x46, 0x28,
	0x47, 0x01,0x48, 0x00,0x49, 0x00,0x4A, 0x00,0x4B, 0x00,0x4C, 0x00,0x4D, 0x00,0x4E, 0x00,0x4F, 0x78,0x50, 0x0D,
	0x51, 0x34,0x52, 0x01,0x53, 0x6C,0x54, 0x90,0xff, 0x04,0x0e, 0x00,0x0f, 0x34,

	//0xFF, 0x04, //; Page 4
	//0x00, 0x04
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
const BYTE	FontOsdWinBase[4] = { 0x10, 0x20, 0x30, 0x40};

void I2Cdelay(void) {
	BYTE i;
	
	for(i=0; i<I2cDelay_Num; i++)
		asm nop;
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/
void I2CStart()
{
	I2C_SCL_IO=1;
	I2C_SDA_IO=1;
	
	I2C_SDA = 1;	dd();
	I2C_SCL = 1;	dd();
	I2C_SDA = 0;	dd();
	I2C_SCL = 0;
	
}
/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/
void I2CStop()
{
	I2C_SDA = 0;	dd();
	I2C_SCL = 1;
	I2C_SDA = 0;	dd();
	I2C_SCL = 1;	dd();
	I2C_SDA = 1;
}
/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/
void I2CWriteData(BYTE value) 
{
	I2C_SCL_IO=1;
	I2C_SDA_IO=1;

	I2C_SCL=0;	I2C_SDA=(value & 0x80)? 1:0;	I2C_SCL=1;	dd();
	I2C_SCL=0;	I2C_SDA=(value & 0x40)? 1:0;	I2C_SCL=1;	dd();
	I2C_SCL=0;	I2C_SDA=(value & 0x20)? 1:0;	I2C_SCL=1;	dd();
	I2C_SCL=0;	I2C_SDA=(value & 0x10)? 1:0;	I2C_SCL=1;	dd();

	I2C_SCL=0;	I2C_SDA=(value & 0x08)? 1:0;	I2C_SCL=1;	dd();
	I2C_SCL=0;	I2C_SDA=(value & 0x04)? 1:0;	I2C_SCL=1;	dd();
	I2C_SCL=0;	I2C_SDA=(value & 0x02)? 1:0;	I2C_SCL=1;	dd();
	I2C_SCL=0;	I2C_SDA=(value & 0x01)? 1:0;	I2C_SCL=1;	dd();


	I2C_SCL = 0;	
	I2C_SCL = 0;	
	I2C_SCL = 0;	
	I2C_SDA = 1;	

	I2C_SCL = 1;	
	I2C_SCL = 1;	
	I2C_SCL = 1;	
	dd();
	I2C_SCL = 0;
}
/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/
BYTE I2CReadData()
{
	BYTE i;
	BYTE value=0;
	
	I2C_SDA_IO=0;				///as input
	
	I2C_SCL = 0;
	
	for(i=0; i<8; i++) {
		I2C_SCL = 1;
		value <<= 1;
		if(I2C_SDA) value |= 1;
		I2C_SCL = 0;
	}
	
	I2C_SCL = 1;	
	I2C_SCL = 1;	
	I2C_SCL = 1;
	I2C_SCL = 0;
	
	I2C_SDA_IO=1;			///as output
	
	return value;

}
/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/
void WriteI2C(BYTE addr, BYTE index, BYTE val)
{
	I2CStart();
	I2CWriteData(addr);
	I2CWriteData(index);
	I2CWriteData(val);
	I2CStop();
}
/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/
BYTE ReadI2C(BYTE addr, BYTE index)
{
	BYTE val;

	I2CStart();
	I2CWriteData(addr);
	I2CWriteData(index);
	I2CStart();
	I2CWriteData(addr | 0x01);
	val = I2CReadData();
	I2CStop();

	return val;
}
/*************************************************************************

*************************************************************************/
void	Tw_Pannal_Parameter_Init(void)
{
	unsigned int i;

	for(i=0;i<sizeof(Tw8835_Innolux_Pannal_Init_Data)/2;i++)
	{
		WriteTW88(Tw8835_Innolux_Pannal_Init_Data[i*2],Tw8835_Innolux_Pannal_Init_Data[i*2+1]);
		asm nop;
		asm nop;
	}
}
/*************************************************************************

*************************************************************************/
void	BackLightCmd(BYTE Cmd)
{
	WriteTW88Page(0x00);	
	if(Cmd==BackLightOpen)
	{
		WriteTW88(0x84,0x1c);
		WriteTW88(0x8c,0x1c);

		WriteTW88(0x94,0x1c);
	}
	else if(Cmd==BackLightOff)
	{
		WriteTW88(0x84,0x0c);
		WriteTW88(0x8c,0x0c);

		WriteTW88(0x94,0x0c);
	}
}

/*************************************************************************

*************************************************************************/
void	Osd_Video_Change(BYTE Cmd)
{
	WriteTW88(0xff,0x02);
	if(Cmd==Osd_State)
		WriteTW88(0x1e, 0x03);///不显示视频
	else if(Cmd==Video_State)
		WriteTW88(0x1e, 0x02);///显示视频
}

/*************************************************************************

*************************************************************************/
void	OsdCmd(unsigned char Osd_Cmd)
{	
	if(Osd_Cmd==OsdOpen)
	{
		WriteTW88(0xff,0x04);
		///WriteTW88(0x00,ReadTW88(0x00)|0x04);
		WriteTW88(0x00,0x04);///spi osd open
		
		WriteTW88(0xff,0x03);
		WriteTW88(0x0c,ReadTW88(0x0c)&0xBf);///font osd open
	}
	else if(Osd_Cmd==OsdClose)
	{
		WriteTW88(0xff,0x04);
		///WriteTW88(0x00,ReadTW88(0x00)&0xfb);
		WriteTW88(0x00,0x00);///spi osd close
		
		WriteTW88(0xff,0x03);
		WriteTW88(0x0c,ReadTW88(0x0c)|0x40);///font osd close
	}
}
/************************************************************************
*************************************************************************/	
void	Init_PictureState(void)
{
	unsigned char Temp[4];
	 #if 0
	Eeprom_Read_Fact(FrontDoorVideoChannelAddr, &FrontDoorVideoChannel, 1);
	if(FrontDoorVideoChannel>=4)
		FrontDoorVideoChannel=0;
	
	Eeprom_Read_Fact(MiddleDoorVideoChannelAddr, &MiddleDoorVideoChannel, 1);
	if(MiddleDoorVideoChannel>=4)
		MiddleDoorVideoChannel=1;

	Eeprom_Read_Fact(RShieldVideoChannel_Addr, &RShieldVideoChannel, 1);
	if(RShieldVideoChannel>=4)
		RShieldVideoChannel=2;
	
	Eeprom_Read_Fact(TopVideoChannel_Addr, &TopVideoChannel, 1);
	if(TopVideoChannel>=4)
		TopVideoChannel=3;

	Eeprom_Read_Fact(FrontDoorVideoSignalStateAddr, &FrontDoorVideoSignalNum, 1);
	if(FrontDoorVideoSignalNum>1)
		FrontDoorVideoSignalNum=0;
	
	Eeprom_Read_Fact(MiddleDoorVideoSignalStateAddr, &MiddleDoorVideoSignalNum, 1);
	if(MiddleDoorVideoSignalNum>1)
		MiddleDoorVideoSignalNum=0;
	
	Eeprom_Read_Fact(RShieldVideoSignalStateAddr, &RShieldVideoSignalNum, 1);
	if(RShieldVideoSignalNum>1)
		RShieldVideoSignalNum=0;

	Eeprom_Read_Fact(TopVideoSignalStateAddr, &TopVideoSignalNum, 1);
	if(TopVideoSignalNum>1)
		TopVideoSignalNum=0;
	
	Eeprom_Read_Fact(LcdShine_Addr, Temp, sizeof(Temp));
	if((Temp[0]>12))
		Temp[0]=6;
	if((Temp[1]>12))
		Temp[1]=5;
	if((Temp[2]>12))
		Temp[2]=2;
	if(Temp[3]>12)
		Temp[3]=0;
	PictureContrastNum=Temp[0];
	PictureBrightNum=Temp[1];
	PictureChromaNum=Temp[2];
	PictureDefinitionNum=Temp[3];
	#endif
}

/************************************************************************
*************************************************************************/	
unsigned char	VideoSignalDetect(void)
{
	unsigned char VideoSignal;
	WriteTW88(0xff,0x01);	
	VideoSignal=ReadTW88(0x01);
	if(VideoSignal&0x80)
		return SignalNo;
	else
		return SignalExist;
}
/************************************************************************
*************************************************************************/	
void	VideoChannelSelect(unsigned char Channel)
{
	unsigned char ChannelControl_Data;
	
	switch(Channel)
	{
		case 0:
			ChannelControl_Data=0x48;//ChannelControl_Data=0x40;//
			break;
		case 1:
			ChannelControl_Data=0x4C;//ChannelControl_Data=0x44;//
			break;
		case 2:
			ChannelControl_Data=0x44;//ChannelControl_Data=0x48;//
			break;
		case 3:
			ChannelControl_Data=0x40;//ChannelControl_Data=0x4C;//
			break;
		default:
			break;
	}
	WriteTW88(0xff,0x01);			
	WriteTW88(0x02,ChannelControl_Data);
}
/************************************************************************
*************************************************************************/	
void	ContrastCmd(unsigned char Data)
{
	unsigned char Contrast_Data;
		
	switch(Data)
	{
		case 0:// GAO 修改
		case 1:
			Contrast_Data=0x00;
			break;
		case 2:
			Contrast_Data=0x10;
			break;
		case 3:
			Contrast_Data=0x20;
			break;
		case 4:
			Contrast_Data=0x40;
			break;
		case 5:
			Contrast_Data=0x50;
			break;
		case 6:
			Contrast_Data=0x80;
			break;
		case 7:
			Contrast_Data=0xa0;
			break;
		case 8:
			Contrast_Data=0xc0;
			break;
		case 9:
			Contrast_Data=0xe0;
			break;
		case 10:
			Contrast_Data=0xf0;
			break;
		default:
			break;
	}
	WriteTW88(0xff,0x01);
	WriteTW88(0x11,Contrast_Data);
}
/************************************************************************
*************************************************************************/	
void	BrightnessCmd(unsigned char Data)
{
	unsigned char Brightness_Data;
		
	switch(Data)
	{		
		case 0:
			Brightness_Data=0x90;
			break;
		case 1:
			Brightness_Data=0xa0;
			break;
		case 2:
			Brightness_Data=0xb0;
			break;
		case 3:
			Brightness_Data=0xc0;
			break;
		case 4:
			Brightness_Data=0xe0;
			break;
		case 5:
			Brightness_Data=0xf0;
			break;
		case 6:
			Brightness_Data=0x00;
			break;
		case 7:
			Brightness_Data=0x10;
			break;
		case 8:
			Brightness_Data=0x20;
			break;
		case 9:
			Brightness_Data=0x40;
			break;
		case 10:
			Brightness_Data=0x50;
			break;
		default:
			break;
	}
	WriteTW88(0xff,0x01);
	WriteTW88(0x10,Brightness_Data);
}
/************************************************************************
*************************************************************************/	
void	HueCmd(unsigned char Data)
{
	unsigned char Hue_Data;
		
	switch(Data)
	{
		case 0:// GAO 修改
		case 1:
			Hue_Data=0x00;
			break;
		case 2:
			Hue_Data=0x10;
			break;
		case 3:
			Hue_Data=0x20;
			break;
		case 4:
			Hue_Data=0x40;
			break;
		case 5:
			Hue_Data=0x50;
			break;
		case 6:
			Hue_Data=0xf0;
			break;
		case 7:
			Hue_Data=0xe0;
			break;
		case 8:
			Hue_Data=0xc0;
			break;
		case 9:
			Hue_Data=0xa0;
			break;
		case 10:
			Hue_Data=0x80;
			break;
		default:
			break;
	}
	WriteTW88(0xff,0x01);
	WriteTW88(0x15,Hue_Data);
}
/************************************************************************
*************************************************************************/	
void	SharpnessCmd(unsigned char Data)
{
	unsigned char Sharpness_Data;
		
	switch(Data)
	{
		case 0:// GAO 修改
		case 1:
			Sharpness_Data=0x30;
			break;
		case 2:
			Sharpness_Data=0x32;
			break;
		case 3:
			Sharpness_Data=0x34;
			break;
		case 4:
			Sharpness_Data=0x36;
			break;
		case 5:
			Sharpness_Data=0x38;
			break;
		case 6:
			Sharpness_Data=0x3a;
			break;
		case 7:
			Sharpness_Data=0x3b;
			break;
		case 8:
			Sharpness_Data=0x3c;
			break;
		case 9:
			Sharpness_Data=0x3e;
			break;
		case 10:
			Sharpness_Data=0x3f;
			break;
		default:
			break;
	}
	WriteTW88(0xff,0x01);
	WriteTW88(0x12,Sharpness_Data);
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void	TW88XX_Power(unsigned char	State)
{
	///SPI下载程序的切换
	DOWNLOAD_CTRL_IO = 1;
	DOWNLOAD_CTRL = 0;
	MODE_CTRL_IO = 1;
	MODE_CTRL = 0;
	///信号输入
	ISD_MISO_IO=0;
	///信号输出脚
	ISD_MOSI_IO=1;
	ISD_MOSI=0;
	///时钟脚
	ISD_SCLK_IO=1;
	ISD_SCLK=0;
	///使能脚
	ISD_CS_IO=1; 
	ISD_CS=0;
	///I2C程序的端口
	I2C_SCL_IO=1;
	I2C_SDA_IO=1;
	if(State==0)
	{
		I2C_SDA = 0;
		I2C_SCL = 0;
	}
	///视频板5V供电
	V_5V_CTRL_IO = 1;
	if(State==0)
		V_5V_CTRL = 0;
	else
		V_5V_CTRL = 1;
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void FontOsdWinEnable(BYTE winno, BYTE en)
{
	BYTE dat;
	BYTE 	index= FontOsdWinBase[winno];

//	WriteTW88Page( PAGE3_FOSD); //0x03
WriteTW88Page( 0x03);
	dat = ReadTW88(index);
	
	if( en ) WriteTW88(index, dat | 0x80);
	else     WriteTW88(index, dat & 0x7F);
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void FontOsdWinDisplay(BYTE winno,BYTE en)
{
	if(winno==FONT_OSD_ALL_WIN)
	{
		FontOsdWinEnable(FONT_OSD_WIN_1,en);
		FontOsdWinEnable(FONT_OSD_WIN_2,en);
		FontOsdWinEnable(FONT_OSD_WIN_3,en);
		FontOsdWinEnable(FONT_OSD_WIN_4,en);
	}
	else if(winno==FONT_OSD_WIN_1)
		FontOsdWinEnable(FONT_OSD_WIN_1,en);
	else if(winno==FONT_OSD_WIN_2)
		FontOsdWinEnable(FONT_OSD_WIN_2,en);
	else if(winno==FONT_OSD_WIN_3)
		FontOsdWinEnable(FONT_OSD_WIN_3,en);
	else if(winno==FONT_OSD_WIN_4)
		FontOsdWinEnable(FONT_OSD_WIN_4,en);
}
/********************************************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
*******************************************************************************************************/
void FontOsdSetOsdRamAddrAttr(BYTE Winno,WORD OsdRamAddrStart)
{
	BYTE index=FontOsdWinBase[Winno];

	WriteTW88Page( PAGE3_FOSD);
	WriteTW88(index+0x07, (OsdRamAddrStart>>8) ? 0x10 : 0x00 );
	WriteTW88(index+0x0F, (BYTE)OsdRamAddrStart);
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void WriteColor_StringToAddr
(WORD OsdRamAddr, BYTE Back_Front_Color, const BYTE *str,BYTE Cnt, BYTE mode)
{
	BYTE i;
	
	WriteTW88Page( PAGE3_FOSD);
	if(mode)
		WriteTW88( 0x04, 0x80 );///以1HZ频率闪烁，当超过256个字符时，5bit设为1->upper256 
	else
		WriteTW88( 0x04, 0x00 );///正常显示,3~2BIT->11,Font Data auto mode；00: Normal mode 01: Font Data or Attribute Address auto mode
	WriteTW88( 0x05, (ReadTW88(0x05) & 0xFE) | (BYTE)(OsdRamAddr >> 8));
	///set address and assign the attribute
	for(i=0;i<Cnt;i++)
	{
		WriteTW88( 0x06, (BYTE)(OsdRamAddr+i) ); 
		WriteTW88( 0x08, Back_Front_Color ); 	///bgColor + fgColor in 1BPP；高4位为背景色，低4位为前景色
		WriteTW88(0x07,*(str+i));
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void FontOsd_Interface_CharacterHeight_Pixels(BYTE Character_Height,BYTE Character_Pixels)
{
	WriteTW88Page( PAGE3_FOSD);
	WriteTW88(0x00,Character_Pixels);///4BIT->Width = 16 pixels
	WriteTW88(0x50,Character_Height);///0~4BIT->Font OSD Character Height;
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void FontOsdWinScreen(BYTE winno, WORD x, WORD y, BYTE w, BYTE h)
{
	BYTE 	index= FontOsdWinBase[winno];
	
	BYTE temp;

	WriteTW88Page( PAGE3_FOSD);

	x=x+33;///FONT OSD在行的初始位置上少了33，所以须做补偿
	temp = x >> 8;
	temp <<= 4;
	temp += ( y >> 8 );
	WriteTW88( index+2,  temp );		// upper bit for position x, y
	WriteTW88( index+3, (BYTE)x );				// position x
	WriteTW88( index+4, (BYTE)y );				// position y
	WriteTW88( index+5, h );
	WriteTW88( index+6, w );
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void FontOsdAplha(BYTE winno,BYTE alpha_color, BYTE alpha_value)
{
	BYTE	index_Alpha = FontOsdWinBase[winno] + 0x01;

	WriteTW88Page( PAGE3_FOSD );

	WriteTW88( 0x52,  alpha_color );	 			// first select color index;4~0->数据值位
	WriteTW88( index_Alpha, alpha_value );	// second write alpha value；0~3->数据值位
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
2、共有三种放大值：0->no zoom,1->*2,2->*3,3->*4
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void FontOsdWinZoom(BYTE winno, BYTE zoomH, BYTE zoomV)
{
	BYTE index, temp;

	index = FontOsdWinBase[winno];

	WriteTW88Page( PAGE3_FOSD);

	temp = zoomH*4 + zoomV;
	temp += (ReadTW88( index ) & 0xf0);
	WriteTW88( index, temp );				// write Zoom value
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void DownloadFontByDMA(WORD dest_font_index, DWORD src_loc, WORD size, BYTE width, BYTE height)
{
	BYTE value;
	WORD address;
	BYTE Loop_Temp;

	//save clock mode & select PCLK
	WaitVBlank(1);	

	WriteTW88Page( PAGE3_FOSD );
	value = ReadTW88(0x00);	
	if(width==16)	value |= 0x10;	   	//width 16
	else			value &= 0xEF;						//   or 12
	value |= 0x02;											//Enable Bypass FontRam FIFO
	WriteTW88( 0x00, value );  

	WriteTW88( 0x04, 0x0d ); 													//FontRAM access
	WriteTW88( 0x09, (BYTE)dest_font_index ); 			//Font Addr
	WriteTW88( 0x50, height >> 1 ); 									//Font height(2~32)
	WriteTW88( 0x51, (width >> 2) * (height >> 1));	//sub-font total count.

	WriteTW88Page( PAGE4_SPI );
	WriteTW88( TWBASE_SPI+0x03, 0x00 | SPICMD_BYTES ); //e4	FontRAM,Inc.7Commands		 //remove or call SPI_SetModeByRegister

	address = dest_font_index * (width >> 2) * (height >> 1);
	WriteTW88( TWBASE_SPI+0x06, (BYTE)(address >> 8) ); 	//03	DMA Page
	WriteTW88( TWBASE_SPI+0x07, (BYTE)address ); 					//0a	DMA index

	//set SPI command
	//READ
	//with source address
	WriteTW88( TWBASE_SPI+0x0a, SPICMD_READ ); 					//0x0b	
	WriteTW88( TWBASE_SPI+0x0b, (BYTE)(src_loc>>16) ); 	// SPI Addr
	WriteTW88( TWBASE_SPI+0x0c, (BYTE)(src_loc>>8) );
	WriteTW88( TWBASE_SPI+0x0d, (BYTE)(src_loc) ); 		
	// set DMA size        
	WriteTW88( TWBASE_SPI+0x1a, 0x00 );
	WriteTW88( TWBASE_SPI+0x08, (BYTE)(size>>8) );
	WriteTW88( TWBASE_SPI+0x09, (BYTE)size );

	WriteTW88Page( PAGE4_SPI );
	WriteTW88( TWBASE_SPI+0x04, 0x01); 									//DMA Start

	Loop_Temp=0;
	while(ReadTW88(0xff) != PAGE4_SPI)									//trick. check DONE. BusyWait
	{
		Loop_Temp++;
		if(Loop_Temp>0xfe)
			break;
	}

	WriteTW88Page( PAGE3_FOSD );
	WriteTW88( 0x04, ReadTW88(0x04) & 0xFE ); 				//OsdRam Access Mode
	WriteTW88(0x00,0x5b);																//mirror 0424
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void InitFontRam(WORD dest_font_index, const FONT_SPI_INFO_t *font)
{
	BYTE value;

	//download font
	asm nop;
	DownloadFontByDMA(dest_font_index, font->loc, font->size, font->width, font->height);

	//assign Multi-Color start address.
	asm nop;
    WriteTW88Page( PAGE3_FOSD);
	asm nop;
	value = ReadTW88(0x05) & 0xF1;
	if(font->bpp2 & 0x100)	value |= 0x02;	// 2bit-multi-font start. 8th address
	if(font->bpp3 & 0x100)	value |= 0x04;	// 3bit-multi-font start. 8th address
	if(font->bpp4 & 0x100)	value |= 0x08;	// 4bit-multi-font start. 8th address
	asm nop;
	WriteTW88( 0x05, value);	
	WriteTW88( 0x0b, (BYTE)font->bpp2 ); 			// 2bit-multi-font start
	WriteTW88( 0x53, (BYTE)font->bpp3 ); 			// 3bit-multi-font start
	WriteTW88( 0x54, (BYTE)font->bpp4 ); 			// 4bit-multi-font start
}
#pragma CODE_SEG DEFAULT

