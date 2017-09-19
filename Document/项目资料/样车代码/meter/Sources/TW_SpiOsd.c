#define		TW_SpiOsd_C
#include	"config.h"
#pragma CODE_SEG  USER_ROM
#pragma MESSAGE DISABLE C12056///屏蔽无必要的编译提示 
///////////////////////////////////////////////////////////////////////
const BYTE SpiOsdWinBase[9] = { 0x20, 0x40, 0x50, 0x60, 0x70,0x80, 0x90, 0xA0, 0xB0};
static WORD SOsdHwBuff_alpha;
static BYTE SOsdHwBuff_rle_win;
static BYTE SOsdHwBuff_rle_bpp;
static BYTE SOsdHwBuff_rle_count;
static BYTE SOsdHwBuff_win[9*8];
static BYTE SpiWinBuff[10*0x10];
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
#define	TW88XX_PRODUCT_ID_CODE			0x74///当为TW8825时
void	WaitTW8825SystemStability(void)
{
	unsigned char ID_Code,Count,i;
	
	WriteTW88Page( 0 );
	for(i=0,Count=0;i<5;i++)
	{
		ID_Code = ReadTW88(0);
		if(ID_Code==TW88XX_PRODUCT_ID_CODE)
			Count ++;
	}
//	ID_Code = ReadTW88(0);
	if((ID_Code==TW88XX_PRODUCT_ID_CODE)&&(Count>3))
	{
		Tw_Pannal_Parameter_Init();	
		TVPowerStability_Flag=1;
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
void WaitVBlank(BYTE cnt)///等待显示处理结束
{
	BYTE i;
	WORD loop;
	BYTE page;

	page = ReadTW88(0xff);
	WriteTW88Page( 0 );

	asm nop;

	for ( i=0; i<cnt; i++ ) 
	{
		WriteTW88( 0x02, 0xff );
		loop = 0;
		while (!( ReadTW88( 0x02 ) & 0x40 ) ) 
		{
			// wait VBlank
			loop++;
			if(loop > 0xFE) ///等待时间；初始值为0xFFFE
			{
				break;
			}
		}		
	}
	asm nop;
	WriteTW88Page(page);
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
void InitCPU(void)
{
	//---------- Initialize Timer Divider ---------
	WriteTW88Page(4);

	WriteTW88(0xe2, 0x69);	// Timer0 Divider : system tic 0.
	WriteTW88(0xe3, 0x78);	// 27M/27000 = 1msec

	WriteTW88(0xe4, 0x01);	// Timer1 Divider for 10usec
	WriteTW88(0xe5, 0x0e);	// 	

	asm nop;
	WriteTW88(0xe6, 0);			// Timer2 Divider : remo timer
	WriteTW88(0xe7, 0x1b);	// 27M/27 = 1usec

	WriteTW88(0xe8, 0);			// Timer3 Divider : baudrate for UART0
	WriteTW88(0xe9, 0x0c);	// (22.1184M/16) / 12 = 115200bps	

	WriteTW88(0xea, 0);			// Timer4 Divider : baudrate for UART1
	WriteTW88(0xeb, 0x18);	// (22.1184M/16) / 24 = 57600bps	
}
///////////////////////////////////////////////////////////////////////
//return
//	0: fail or MX. default
//	1: EON
//
#if 0
BYTE SPI_QUADInit(void)
{
	BYTE	temp;
	BYTE ret=0;

	WriteTW88Page( PAGE4_SPI );

	WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
	WriteTW88( 0xca, 0x9f );					// cmd, read Jedec ID
	WriteTW88( 0xc6, 0x04 );
	WriteTW88( 0xc7, 0xd0 );					// read buffer size 3

	WriteTW88( 0xda, 0 );
	WriteTW88( 0xc8, 0 );
	WriteTW88( 0xc9, 3 );							// read buffer size 3
	WriteTW88( 0xc4, 0x01 );					// start

	temp = ReadTW88(0xd0);
	
	if ( (temp == 0xC2) || ( temp == 0x1c)) {		// for MX	&& EON  && STM
		if(temp ==0x1C)
			ret = SFLASH_VENDOR_EON;
		else
			ret = SFLASH_VENDOR_MX;

		WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
		WriteTW88( 0xca, 0x05 );					// cmd, read Status
		WriteTW88( 0xc9, 1 );							// read buffer size 1
		WriteTW88( 0xc4, 0x01 );					// start
		temp = ReadTW88( 0xd0 );						// read status regisiter value
		if ( temp == 0 ) {
			WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( 0xca, 0x06 );					// cmd, write enable
			WriteTW88( 0xc9, 0 );							// read buffer size 1
			WriteTW88( 0xc4, 0x03 );					// start

			WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( 0xca, 0x05 );					// cmd, read Status
			WriteTW88( 0xc9, 1 );							// read buffer size 1
			WriteTW88( 0xc4, 0x01 );					// start
			temp = ReadTW88( 0x10 );						// read status regisiter value

			WriteTW88( 0xc3, 0x42 );					// mode, cmd-len 2
			WriteTW88( 0xca, 0x01 );					// cmd, write Status
			WriteTW88( 0xcb, 0x40 );					// cmd, en QAUD mode；对于旺宏的MX25L12845E芯片，一定要有此操作，否则无法正常通讯
			WriteTW88( 0xc9, 0 );							// read buffer size 1
			WriteTW88( 0xc4, 0x07 );					// start + write + busycheck

			WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( 0xca, 0x05 );					// cmd, read Status
			WriteTW88( 0xc9, 1 );							// read buffer size 1
			WriteTW88( 0xc4, 0x01 );					// start
			temp = ReadTW88( 0xd0 );						// read status regisiter value

			WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( 0xca, 0x06 );					// cmd, write disable
			WriteTW88( 0xc4, 0x01 );					// start
		}
	}
	else if ( temp == 0xEF ) {						// for WB
		ret = SFLASH_VENDOR_WB;
		WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
		WriteTW88( 0xca, 0x05 );					// cmd, read Status
		WriteTW88( 0xc9, 2 );							// read buffer size 1
		WriteTW88( 0xc4, 0x01 );					// start
		temp = ReadTW88( 0xd1 );						// read status regisiter value
		if ( temp == 0 ) {
			WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( 0xca, 0x06 );					// cmd, write enable
			WriteTW88( 0xc4, 0x01 );					// start

			WriteTW88( 0xc3, 0x43 );					// mode, cmd-len 1
			WriteTW88( 0xca, 0x01 );					// cmd, write Status
			WriteTW88( 0xcb, 0x00 );					// cmd, en QAUD mode
			WriteTW88( 0xcc, 0x02 );					// cmd, en QAUD mode
			WriteTW88( 0xc4, 0x01 );					// start
		}
	}
	else if(temp == 0x20 ) {								// for STM
		ret = SFLASH_VENDOR_NUM;

		WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
		WriteTW88( 0xca, 0x06 );					// cmd, write enable
		WriteTW88( 0xc9, 0 );							// read buffer size 1
		WriteTW88( 0xc4, 0x03 );					// start

		WriteTW88( 0xc3, 0x43 );					// mode, cmd-len 1
		WriteTW88( 0xca, 0xB1 );					// cmd, write enable
		WriteTW88( 0xcb, 0x6F );
		WriteTW88( 0xcc, 0xFF );
		WriteTW88( 0xc9, 0 );							// read buffer size 1
		WriteTW88( 0xc4, 0x07 );					// start

		WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
		WriteTW88( 0xca, 0x06 );					// cmd, write enable
		WriteTW88( 0xc9, 0 );							// read buffer size 1
		WriteTW88( 0xc4, 0x03 );					// start

		WriteTW88( 0xc3, 0x42 );					// mode, cmd-len 1
		WriteTW88( 0xca, 0x81 );					// cmd, write enable
		WriteTW88( 0xcb, 0x6B );
		WriteTW88( 0xc9, 0 );							// read buffer size 1
		WriteTW88( 0xc4, 0x07 );					// start
	}
	return ret;
}
#else
BYTE SPI_QUADInit(void)
{
	BYTE	temp,temp1;
	BYTE ret=0;

	WriteTW88Page( PAGE4_SPI );

	WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
	WriteTW88( 0xca, 0x9f );					// cmd, read Jedec ID
	WriteTW88( 0xc6, 0x04 );
	WriteTW88( 0xc7, 0xd0 );					// read buffer size 3

	WriteTW88( 0xda, 0 );
	WriteTW88( 0xc8, 0 );
	WriteTW88( 0xc9, 3 );							// read buffer size 3
	WriteTW88( 0xc4, 0x01 );					// start

	temp = ReadTW88(0xd0);
	
	if ( (temp == 0xC2) || ( temp == 0x1c)) {		// for MX	&& EON  && STM
		if(temp ==0x1C)
			ret = SFLASH_VENDOR_EON;
		else
			ret = SFLASH_VENDOR_MX;

		WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
		WriteTW88( 0xca, 0x05 );					// cmd, read Status
		WriteTW88( 0xc9, 1 );							// read buffer size 1
		WriteTW88( 0xc4, 0x01 );					// start
		temp = ReadTW88( 0xd0 );						// read status regisiter value
		if ( temp == 0 ) {
			WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( 0xca, 0x06 );					// cmd, write enable
			WriteTW88( 0xc9, 0 );							// read buffer size 1
			WriteTW88( 0xc4, 0x03 );					// start

			WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( 0xca, 0x05 );					// cmd, read Status
			WriteTW88( 0xc9, 1 );							// read buffer size 1
			WriteTW88( 0xc4, 0x01 );					// start
			temp = ReadTW88( 0x10 );						// read status regisiter value

			WriteTW88( 0xc3, 0x42 );					// mode, cmd-len 2
			WriteTW88( 0xca, 0x01 );					// cmd, write Status
			WriteTW88( 0xcb, 0x40 );					// cmd, en QAUD mode；对于旺宏的MX25L12845E芯片，一定要有此操作，否则无法正常通讯
			WriteTW88( 0xc9, 0 );							// read buffer size 1
			WriteTW88( 0xc4, 0x07 );					// start + write + busycheck

			WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( 0xca, 0x05 );					// cmd, read Status
			WriteTW88( 0xc9, 1 );							// read buffer size 1
			WriteTW88( 0xc4, 0x01 );					// start
			temp = ReadTW88( 0xd0 );						// read status regisiter value

			WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( 0xca, 0x06 );					// cmd, write disable
			WriteTW88( 0xc4, 0x01 );					// start
		}

		WriteTW88( 0xc3, 0x45); // mode, cmd-len 1 
		WriteTW88( 0xca, 0x5A); // cmd, read Status
		WriteTW88(0xcb,  0x00);
		WriteTW88(0xcc,  0x00);
		WriteTW88(0xcd,  0x32);
		WriteTW88(0xce,  0x0A);	 // 
		WriteTW88( 0xc9, 2 ); 	 // read buffer size 1
		WriteTW88( 0xc4, 0x01 ); // start
		temp = ReadTW88( 0xd1 ); // read status regisiter value


		if(temp==0xF1)
		{
		//MX35F的初始化
		///////2013-4-19增加:测试图片抖动
			WriteTW88( 0xc3, 0x41 ); // mode, cmd-len 1
			WriteTW88( 0xca, 0x15 ); // cmd, read Status
			WriteTW88( 0xc9, 1 ); // read buffer size 1
			WriteTW88( 0xc4, 0x01 ); // start
			temp = ReadTW88( 0xd0 ); // read status regisiter value
			if ( temp != 0xc0) 
			{
				WriteTW88( 0xc3, 0x41 ); // mode, cmd-len 1
				WriteTW88( 0xca, 0x06 ); // cmd, write enable
				WriteTW88( 0xc9, 0 ); // read buffer size 1
				WriteTW88( 0xc4, 0x03 ); // start
				 
				//Puts("\nWrite ENABLED" );
				 
				WriteTW88( 0xc3, 0x41 ); // mode, cmd-len 1
				WriteTW88( 0xca, 0x05 ); // cmd, read Status
				WriteTW88( 0xc9, 1 ); // read buffer size 1
				WriteTW88( 0xc4, 0x01 ); // start
				temp = ReadTW88( 0x10 ); // read status regisiter value
				//Printf("\nStatus after Write enable %02bx", temp );
				 
				WriteTW88( 0xc3, 0x43 ); // mode, cmd-len 2
				WriteTW88( 0xca, 0x01 ); // cmd, write Status
				WriteTW88( 0xcb, 0x40 ); // cmd, en QAUD mode
				WriteTW88( 0xcc, 0xC0 ); // cmd, set configure
				WriteTW88( 0xc9, 0 ); // read buffer size 1
				WriteTW88( 0xc4, 0x07 ); // start + write + busycheck
				 
				//Puts("\nQUAD ENABLED" );
				 
				WriteTW88( 0xc3, 0x41 ); // mode, cmd-len 1
				WriteTW88( 0xca, 0x15 ); // cmd, read Status
				WriteTW88( 0xc9, 1 ); // read buffer size 1
				WriteTW88( 0xc4, 0x01 ); // start
				temp = ReadTW88( 0xd0 ); // read status regisiter value
				
				//Printf("\nStatus after QUAD enable %02bx", temp );
				 
				WriteTW88( 0xc3, 0x41 ); // mode, cmd-len 1
				WriteTW88( 0xca, 0x06 ); // cmd, write disable
				WriteTW88( 0xc4, 0x01 ); // start
				///////2013-4-19增加:测试图片抖动			
			}
		}	
	}
	else if ( temp == 0xEF ) {						// for WB
		ret = SFLASH_VENDOR_WB;
		WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
		WriteTW88( 0xca, 0x05 );					// cmd, read Status
		WriteTW88( 0xc9, 2 );							// read buffer size 1
		WriteTW88( 0xc4, 0x01 );					// start
		temp = ReadTW88( 0xd1 );						// read status regisiter value
		if ( temp == 0 ) {
			WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
			WriteTW88( 0xca, 0x06 );					// cmd, write enable
			WriteTW88( 0xc4, 0x01 );					// start

			WriteTW88( 0xc3, 0x43 );					// mode, cmd-len 1
			WriteTW88( 0xca, 0x01 );					// cmd, write Status
			WriteTW88( 0xcb, 0x00 );					// cmd, en QAUD mode
			WriteTW88( 0xcc, 0x02 );					// cmd, en QAUD mode
			WriteTW88( 0xc4, 0x01 );					// start
		}
	}
	else if(temp == 0x20 ) {								// for STM
		ret = SFLASH_VENDOR_NUM;

		WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
		WriteTW88( 0xca, 0x06 );					// cmd, write enable
		WriteTW88( 0xc9, 0 );							// read buffer size 1
		WriteTW88( 0xc4, 0x03 );					// start

		WriteTW88( 0xc3, 0x43 );					// mode, cmd-len 1
		WriteTW88( 0xca, 0xB1 );					// cmd, write enable
		WriteTW88( 0xcb, 0x6F );
		WriteTW88( 0xcc, 0xFF );
		WriteTW88( 0xc9, 0 );							// read buffer size 1
		WriteTW88( 0xc4, 0x07 );					// start

		WriteTW88( 0xc3, 0x41 );					// mode, cmd-len 1
		WriteTW88( 0xca, 0x06 );					// cmd, write enable
		WriteTW88( 0xc9, 0 );							// read buffer size 1
		WriteTW88( 0xc4, 0x03 );					// start

		WriteTW88( 0xc3, 0x42 );					// mode, cmd-len 1
		WriteTW88( 0xca, 0x81 );					// cmd, write enable
		WriteTW88( 0xcb, 0x6B );
		WriteTW88( 0xc9, 0 );							// read buffer size 1
		WriteTW88( 0xc4, 0x07 );					// start
	}	
	return ret;
}
#endif
///////////////////////////////////////////////////////////////////////
void SPI_SetModeByRegister( void )
{
	BYTE mod;

	WriteTW88Page( PAGE4_SPI );
	mod = ReadTW88( 0xc0 ) & 0x07;

	switch( mod ) {
		case 0:	//--- Slow
			DMAMODE_RW_FIX_REG = 0x64;
			DMAMODE_RW_INC_REG = 0x44;
			DMAMODE_R_XDATA    = 0xc4;
			DMAMODE_W_XDATA    = 0xc4;
			SPICMD_READ        = 0x03;	
			SPICMD_BYTES = 4;
			SPICMD_PROGRAM     = 0x02;
			break;

		case 1:	//--- Fast
			DMAMODE_RW_FIX_REG = 0x65;
			DMAMODE_RW_INC_REG = 0x45;
			DMAMODE_R_XDATA    = 0xc5;
			DMAMODE_W_XDATA    = 0xc4;
			SPICMD_READ        = 0x0b;	
			SPICMD_BYTES = 5;
			SPICMD_PROGRAM     = 0x02;
			break;

		case 2:	//--- Dual
			DMAMODE_RW_FIX_REG = 0x65;
			DMAMODE_RW_INC_REG = 0x45;
			DMAMODE_R_XDATA    = 0xc5;
			DMAMODE_W_XDATA    = 0xc4;
			SPICMD_READ        = 0x3b;	
			SPICMD_BYTES = 5;
			SPICMD_PROGRAM     = 0x02;
			break;

		case 3:	//--- Quad
			DMAMODE_RW_FIX_REG = 0x65;
			DMAMODE_RW_INC_REG = 0x45;
			DMAMODE_R_XDATA    = 0xc5;
			DMAMODE_W_XDATA    = 0xc4;
			SPICMD_READ        = 0x6b;	
			SPICMD_BYTES = 5;
			SPICMD_PROGRAM     = 0x02;
			break;

		case 4:	//--- Dual-IO
			DMAMODE_RW_FIX_REG = 0x65;
			DMAMODE_RW_INC_REG = 0x45;
			DMAMODE_R_XDATA    = 0xc5;
			DMAMODE_W_XDATA    = 0xc4;
			SPICMD_READ        = 0xbb;	
			SPICMD_BYTES = 5;
			SPICMD_PROGRAM     = 0x02;
			break;

		case 5:	//--- Quad-IO
			DMAMODE_RW_FIX_REG = 0x67;
			DMAMODE_RW_INC_REG = 0x47;
			DMAMODE_R_XDATA    = 0xc7;	
			DMAMODE_W_XDATA    = 0xc4;
			SPICMD_READ        = 0xeb;	 
			SPICMD_BYTES = 7;
			SPICMD_PROGRAM     = 0x02;
			break;
 	}
}

///////////////////////////////////////////////////////////////////////
void SPI_Init(void)
{

	WriteTW88( 0xff, PAGE4_SPI );

	///WriteTW88(0x01,ReadTW88( 0x01 )|0B01000000);

	WriteTW88( TWBASE_SPI, 5);		// SPI Mode:	0=Slow, 1=fast, 2=dual, 3=quad, 4=dual-io, 5=quad-io
																// R4C0	SPI mode QuadIO
	SPI_SetModeByRegister();		// Match DMA mode with SPI-read

}
///////////////////////////////////////////////////////////////////////
void InitCore(void)
{	
	///Set SPI mode
	BYTE num;
	num = SPI_QUADInit();
	asm nop;
	SPI_Init();

	WriteTW88Page( 0 );
	WriteTW88( 0x02, 0xFF );	// Clear Pending Interrupts
	WriteTW88( 0x03, 0xFE );	// enable SW. disable all other interrupts
}
///////////////////////////////////////////////////////////////////////
void FP_GpioDefault(void)
{
	WriteTW88Page(0);
	WriteTW88(0x84, 0x00);	//FPBiasOnOff(OFF);
	WriteTW88(0x8C, 0x00);	//FPPWC_OnOff(OFF);
	WriteTW88(0x94, 0x00);	//output
}
///////////////////////////////////////////////////////////////////////
void FPPWC_OnOff(BYTE fOn)
{
	WriteTW88Page(0);
	if(fOn) {
		WriteTW88(0x8C, ReadTW88(0x8C) | 0x04);	//output FPPWC enable
		WriteTW88(0x84, ReadTW88(0x84) | 0x04);	//enable GPIO
	}
	else {
		WriteTW88(0x8C, ReadTW88(0x8C) & ~0x04);	//output FPPWC disable
		WriteTW88(0x84, ReadTW88(0x84) & ~0x04);	//disable GPIO
	}
}
///////////////////////////////////////////////////////////////////////
void SSPLL_PowerUp(BYTE fOn)
{
	WriteTW88Page(0);
	if(fOn) {
		WriteTW88(0xFC, ReadTW88(0xFC) & ~0x80);
	}
	else {
		WriteTW88( 0xFC, (ReadTW88(0xFC) | 0x80));
	}
}
///////////////////////////////////////////////////////////////////////
void DCDC_On(BYTE step)
{
	BYTE i;

	//DCDC ON
	WriteTW88Page(0);
	switch(step) {
	case 0:
		WriteTW88(0xE8, 0x72);	//default. & disable OverVoltage
		WriteTW88(0xE8, 0x12);	//disable OverCurrent, disable UnderCurrent
		WriteTW88(0xE8, 0x13);	//enable DC convert digital block
		break;
	case 1:
		WriteTW88(0xE8, 0x11);	//powerup DC sense block
		break;
	case 2:
		WriteTW88(0xE8, 0x71);	//turn on under current feedback control
		break;
	default:
		return;
	}
	for(i=0; i < 128; i++) 
	{
		if((ReadTW88(0xEA) & 0x30)==0x30) {
			break;
		}
		DelayMs(2);
	}
}
///////////////////////////////////////////////////////////////////////
void FPBiasOnOff(BYTE fOn)
{

	WriteTW88Page(0);
	if(fOn) 
	{
		WriteTW88(0x84, ReadTW88(0x84) | 0x08);		//enable 
		WriteTW88(0x8C, ReadTW88(0x8C) | 0x08);		//output enable
		DelayMs(2);
	}
	else 
	{
		WriteTW88(0x8C, ReadTW88(0x8C) & ~0x08);	//output disable
		WriteTW88(0x84, ReadTW88(0x84) & ~0x08);	//disable
	}
}

///////////////////////////////////////////////////////////////////////
void DCDC_StartUP(void)
{
	//FPPWC ON - GPIO42.  TW8832.  p2 bf ;FPPWC_On
	FPPWC_OnOff(ON);

	SSPLL_PowerUp(ON);
	DelayMs(20);

	DCDC_On(0);
	DCDC_On(1);
	
	// wait
	DelayMs(20);

	//Data Out
	//b 8a 08 55 00 ; Data out
	WriteTW88Page(0);
	WriteTW88(0x08, 0x89);	//Output enable
	DelayMs(2);

	//-------------
	//FPBIAS ON - GPIO43. TW8832: p2 3f ; FPBIAS_On
	FPBiasOnOff(ON);

	//DCDC final
	DCDC_On(2);

	//disable Blank
	//WriteTW88Page(PAGE2_SCALER);
	//WriteTW88(0x1E, ReadTW88(0x1E) & ~0x01);
}
///////////////////////////////////////////////////////////////////////
void McuSpiClkSet(BYTE PCLK_SEL,BYTE SPI_CK_SEL,BYTE SPI_CK_DIV) 
{
	WriteTW88Page(PAGE4_CLOCK);
	if(PCLK_SEL)	WriteTW88(0xE0, ReadTW88(0xE0) |  0x01);
	else			WriteTW88(0xE0, ReadTW88(0xE0) & ~0x01); 
	WriteTW88(0xE1, (SPI_CK_SEL << 4) | SPI_CK_DIV);
}
//////////////////////////////////////////////////////////////////////
const FONT_SPI_INFO_t consolas16x26_606C90 	= {0x402000, 0x2080, 16, 26, 0x0A0, 0x060, 0x06C, 0x090 };
///0x23c是通过BIN文件的属性中查询到，0x058是字符库的个数，其他的信息从_INF的文件查询，字库最大是10K
const FONT_SPI_INFO_t WORD_16_26_NEW 	= {0xFE0000, 0x1930, 16, 26, 0x07C, 0x07C, 0x07C, 0x07C };//0x11e0
void InitSystem(BYTE special)
{
	if(special==0) 
	{
		FP_GpioDefault();
		DCDC_StartUP();
	}

	McuSpiClkSet(PCLK_SEL_PCLK,SPI_CK_SEL_PCLK,SPI_CK_DIV_1P0);

	InitFontRam(0, &WORD_16_26_NEW);
}
////////////////////////////////////////////////////////////////////////

void SpiOsdEnableRLC(BYTE en)
{
	WriteTW88Page( PAGE4_SOSD );
	if(en)		WriteTW88(0x04, ReadTW88(0x04) | 0x01);
	else		WriteTW88(0x04, ReadTW88(0x04) & 0xFE);
}
///////////////////////////////////////////////////////////////////////
void SpiOsdWinHWEnable(BYTE winno, BYTE en)
{
	BYTE index;
	BYTE dat;

	index = SpiOsdWinBase[winno] + 0x00;

	WriteTW88Page( PAGE4_SOSD );
	dat = ReadTW88(index);
	if( en ) {
		WriteTW88(index, dat | 0x01);
	}
	else     WriteTW88(index, dat & 0xfe);
}
/////////////////////////////////////////////////
void SOsdWinBuffAllClean(void)
{
	BYTE i,j;

	for(i=0; i < 10; i++) {
		for(j=0; j < 0x0E; j++)
			SpiWinBuff[i*16+j]=0;
	}
}
/////////////////////////////////////////////////
void SOsdWinBuffClean(unsigned char winno)
{
	BYTE j;
	
	if(winno)winno++;
	
	for(j=0; j < 0x0E; j++)
		SpiWinBuff[winno*16+j]=0;
}
/////////////////////////////////////////////////
void SpiOsdEnable(BYTE en)
{
	BYTE dat;
	WriteTW88Page( PAGE4_SOSD );
	dat = ReadTW88(0x00);
	if( en ) 
	{
		WriteTW88(0x00, dat | 0x04);///Disable all SPIOSD windows
	}
	else 
	{
	  WriteTW88(0x00, dat & 0xfb);///SPIOSD overall enable/disable
	}
}
/////////////////////////////////////////////////
void SpiOsdResetRLC(BYTE reset)
{
	WriteTW88Page( PAGE4_SOSD );
	if(reset)	WriteTW88(0x04, ReadTW88(0x04) | 0x02);
	else		WriteTW88(0x04, ReadTW88(0x04) & 0xFD);
}
/////////////////////////////////////////////////
void SpiOsdRLC(BYTE winno,BYTE dcnt, BYTE ccnt)
{
	//7 means 8BPP with 128 color.
	if(dcnt==7)
		dcnt++;

	WriteTW88Page( PAGE4_SOSD );
	WriteTW88(0x05, (dcnt << 4) | (ccnt));
	WriteTW88(0x06, winno);
}
/////////////////////////////////////////////////
void SpiOsdWinImageLocBit(BYTE winno, BYTE start)
{
	BYTE *data_p;

	if(winno) winno++;
	data_p = &SpiWinBuff[winno << 4];

	data_p += 0x0a;
	*data_p &= 0x3F;
	*data_p |= (start << 6);
}
/////////////////////////////////////////////////
void SpiOsdWinLutOffset( BYTE winno, WORD table_offset )
{
	BYTE *data_p;

	if(winno) winno++;
	data_p = &SpiWinBuff[winno << 4];
	data_p += 0x0d;
	if(!winno) data_p += 4;
	
	//LUT offset use 5bit & 16 unit
	*data_p = (BYTE)(table_offset >> 4);
}
/////////////////////////////////////////////////
void	SpiOsdWinFillColor( BYTE winno, BYTE color )
{
	BYTE	index;

	index = SpiOsdWinBase[winno];
	WriteTW88Page( PAGE4_SOSD );

	if ( color ) {
		WriteTW88( index, (ReadTW88( index ) | 0x04));				/// en Alpha & Global
	}
	else {
		WriteTW88( index, ( ReadTW88( index ) & 0xFB ) );		/// dis Alpha & Global
	}
	index = SpiOsdWinBase[winno] + 0x0e;
	if(!winno)	index += 8;
	WriteTW88( index, color );
}
/////////////////////////////////////////////////
void SpiOsdWinImageLoc(BYTE winno, DWORD start)///图片起始地址 
{	
	BYTE *data_p;

	if(winno) winno++;
	data_p = &SpiWinBuff[winno << 4];

	data_p += 0x07;

	*data_p++ =  (BYTE)(start>>16);	//+0x07
	*data_p++ =  (BYTE)(start>>8);		//+0x08
	*data_p++ =  (BYTE)start;					//+0x09
}
/////////////////////////////////////////////////
void SpiOsdWinImageSizeW(BYTE winno, WORD w)///给窗口1-8图片的宽度 
{
	BYTE *data_p;
	BYTE value;

	data_p = &SpiWinBuff[(winno+1) << 4];	//No WIN0
	data_p += 0x0a;

 	value = *data_p & 0xC0;

	*data_p++ = (BYTE)(w>>8 | value);		//+0x0A
	*data_p++ = (BYTE)w;					//+0x0B
}
/////////////////////////////////////////////////
void SpiOsdWinImageSizeWH (BYTE winno, WORD w, WORD h)///	给窗口0图片的宽度和高度 
{
	BYTE *data_p;
	BYTE value;

	//WIN1to8 need only Width.
	if(winno) {
		SpiOsdWinImageSizeW(winno,w);
		return; 
	}
	//now only for WIN0
	data_p = SpiWinBuff;				   //Only WIN0
	data_p += 0x0a;

	value = (BYTE)(h >> 8);
	value <<= 4;
	value |= (BYTE)( w>>8 );
	*data_p++ = value; 		//42A
	*data_p++ = (BYTE)w;	//42B
	*data_p++ = (BYTE)h;	//42C
}

/////////////////////////////////////////////////
void SpiOsdWinScreen(BYTE winno, WORD x, WORD y, WORD w, WORD h)///窗口的位置和大小   
{
	BYTE *data_p;
	BYTE value;

	if(winno) winno++;
	data_p = &SpiWinBuff[winno << 4];

	data_p += 0x01;
	value = (y >> 8);
	value <<= 4;
	value |= (x >> 8);
	*data_p++ = value;		//421	441...
	*data_p++ = (BYTE)x;	//422	442... 	
	*data_p++ = (BYTE)y;	//423	443...
	
	value = (h >> 8);
	value <<= 4;
	value |= (w >> 8);
	*data_p++ = value;		//424	444...
	*data_p++ = (BYTE)w;	//425	445...	 	
	*data_p++ = (BYTE)h;	//426	446...	 
}
/////////////////////////////////////////////////
void SpiOsdWin0Animation(BYTE mode, BYTE FrameH, BYTE FrameV, BYTE Duration)///设置动画的方式，横，纵的方向动画，时间 
{
 	BYTE *data_p;

	data_p = SpiWinBuff;	  			//Only WIN0
	data_p += 0x12;

	*data_p++ = FrameH;
	*data_p++ = FrameV;
	*data_p++ = Duration;

	*data_p &= 0x3f;
	*data_p |= (mode << 6);
}
/////////////////////////////////////////////////
void	SpiOsdWinPixelAlpha( BYTE winno, BYTE alpha )	///透明度  
{
	BYTE  *data_p;

	if(winno) winno++;
	data_p = &SpiWinBuff[winno << 4];

	if(alpha)	*data_p |= 0x30;
	else		*data_p &= 0xCF;

	data_p += 0x0c;
	if(!winno)	data_p += 4;
	*data_p = alpha;
}
/////////////////////////////////////////////////
void SpiOsdWinPixelWidth(BYTE winno, BYTE bpp)///存储宽度  
{
  BYTE  *data_p;
	BYTE mode;

	if(bpp==4)	mode=0;
	else if(bpp==6) mode=1;
	else mode=2;	//7 and 8 use mode 2

	if(winno) 	winno++;
	data_p = &SpiWinBuff[winno << 4];

	*data_p &= 0x3f;
	*data_p |= (mode <<6);
}
/////////////////////////////////////////////////
void SpiOsdWinBuffEnable(BYTE winno, BYTE en)
{
	BYTE *data_p;

	if(winno) winno++;
	data_p = &SpiWinBuff[winno << 4];

	if(en) *data_p |= 0x01;
	else   *data_p &= 0xfe;
}
/////////////////////////////////////////////////
void SOsdHwBuffSetRle(BYTE win, BYTE bpp, BYTE count)
{
	SOsdHwBuff_rle_win = win;
	SOsdHwBuff_rle_bpp = bpp;
	SOsdHwBuff_rle_count = count;
}
/////////////////////////////////////////////////
void SOsdHwBuffSetLut(BYTE win, WORD LutOffset, WORD size, DWORD address)
{
  SOsdHwBuff_win[win*8+0] = 1;
	SOsdHwBuff_win[win*8+1] = (BYTE)(size >> 8);
	SOsdHwBuff_win[win*8+2] = (BYTE)size;

	SOsdHwBuff_win[win*8+3] = (BYTE)(LutOffset >> 6);
	SOsdHwBuff_win[win*8+4] = (BYTE)(LutOffset << 2);

	SOsdHwBuff_win[win*8+5] = (BYTE)(address>>16);
	SOsdHwBuff_win[win*8+6] = (BYTE)(address>>8) ;
	SOsdHwBuff_win[win*8+7] = (BYTE)(address) ;
}
/////////////////////////////////////////////////
void SOsdHwBuffSetAlpha(WORD alpha_index)
{
	SOsdHwBuff_alpha = alpha_index;
}
/////////////////////////////////////////////////
BYTE check_cvbs(BYTE num)
{
	BYTE dat;
	asm nop;
	WriteTW88Page( 2 );
	dat = ReadTW88(0x1c);
	WriteTW88(0x1c,dat&0xf9);
	dat = ReadTW88(0x1e);
	WriteTW88(0x1e,dat&0xfd);

	WriteTW88Page( 1 );//通道选择
	if(num==0)
	WriteTW88(0x02,0xc0);
	else if(num==1)
		WriteTW88(0x02,0xc4);
	else if(num==2)
		WriteTW88(0x02,0xc8);
	else if(num==3)
		WriteTW88(0x02,0xcc);
	asm nop;
			
	dat = ReadTW88(0x01);

	if(dat&0x80)
		return 0;
	else
		return 1;
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
void SOsdHwBuffWrite2Hw(void)
{
	BYTE win;
	BYTE *data_p;
	BYTE i,j;
	BYTE reg_i;
	BYTE B0, B;	 //bugrate random 
	WORD LutOffset;

	//to fix LUT0 B0 debug.
	WriteTW88Page( PAGE4_SOSD );
	WriteTW88(0x10,0xa0);	//read B
	WriteTW88(0x11,0);		//addr 0
	B0=ReadTW88(0x12);			//read twice
	B0=ReadTW88(0x12);		
	//update LUT
	WriteTW88Page( PAGE4_SOSD );
	WriteTW88( 0x10, 0xa0 );    	// LUT Write Mode, En & byte ptr inc. DMA needs it.
	WriteTW88( 0x11, 0); 					// LUT addr. set 0 on DMA

	for(win=0; win <= 8; win++) 
	{
		if(SOsdHwBuff_win[win*8] == 0) 
			continue;
		data_p = &SOsdHwBuff_win[win*8+1];
		//Spi Flash DMA
		asm nop;
		WriteTW88( TWBASE_SPI+0x04, 0x00 );	// DMA stop	
		WriteTW88( TWBASE_SPI+0x03, 0x80 | SPICMD_BYTES );
		WriteTW88( TWBASE_SPI+0x0a, SPICMD_READ ); // SPI Command=R
		asm nop;
		WriteTW88( TWBASE_SPI+0x08, *data_p++ );		//size0
		WriteTW88( TWBASE_SPI+0x09, *data_p++ );		//size1
		LutOffset = *data_p;
		LutOffset <<= 6;
		WriteTW88( TWBASE_SPI+0x06, *data_p++ ); 	//LutOffset[8:6] -> R4C6[2:0]
		LutOffset |= (*data_p & 0x3F);
		WriteTW88( TWBASE_SPI+0x07, *data_p++ );		//LutOffset[5:0] -> R4C7[7:2] 
		WriteTW88( TWBASE_SPI+0x0b, *data_p++); 		//address0
		WriteTW88( TWBASE_SPI+0x0c, *data_p++ );		//address1
		WriteTW88( TWBASE_SPI+0x0d, *data_p++ ); 	//address2
		WaitVBlank(1);///防止在同一界面下，当一个图片在切换时的条纹现象
		WriteTW88( TWBASE_SPI+0x04, 0x01 ); // DMA Start
		WriteTW88(0x10,0xa0);		//read B
		WriteTW88(0x11,0);			//addr 0
		B=ReadTW88(0x12);				//read twice
		B=ReadTW88(0x12);		

		asm nop;
		if(LutOffset) 
		{
			if(B0 != B) 
			{
				WriteTW88(0x11, 0);			//addr 0
			}
		}
		else 
		{
			B0 = B;
		}
		asm nop;
	}
	WriteTW88Page( PAGE4_SOSD );
	if(SOsdHwBuff_rle_win) 
	{
		WriteTW88(0x04, ReadTW88(0x04) | 0x01);
		WriteTW88(0x05, ((SOsdHwBuff_rle_bpp==7?8:SOsdHwBuff_rle_bpp) << 4) | (SOsdHwBuff_rle_count));
		WriteTW88(0x06, SOsdHwBuff_rle_win);
	}
	else 
	{
		WriteTW88(0x04, ReadTW88(0x04) & 0xFE);
		WriteTW88(0x05, 0);
		WriteTW88(0x06, 0);
	}

	if(SOsdHwBuff_alpha != 0xFFFF) 
	{
		WriteTW88( 0x10, 0xc3 );    		// LUT Write Mode, En & byte ptr inc.
		if(SOsdHwBuff_alpha >> 8)	WriteTW88( 0x10, ReadTW88(0x10) | 0x08);	//support 512 palette
		else            			WriteTW88( 0x10, ReadTW88(0x10) & 0xF7);
		WriteTW88( 0x11, (BYTE)SOsdHwBuff_alpha ); 	// alpha index
		WriteTW88( 0x12, 0x7F); 			// alpha value
	}
	data_p = &SpiWinBuff[ 2 << 4];
	for(i=2; i <= 9; i++) 
	{
		reg_i = (i << 4) + 0x20;
		for(j=0; j < 16; j++) {
			WriteTW88(reg_i++, *data_p++);	
		}
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
void SOsdHwBuffWrite_Win(unsigned char winno,unsigned char WaitFlag)
{
	///BYTE win;
	BYTE *data_p;
	BYTE i,j;
	BYTE reg_i;
	BYTE B0, B;	 //bugrate random 
	WORD LutOffset;

	//to fix LUT0 B0 debug.
	WriteTW88Page( PAGE4_SOSD );
	WriteTW88(0x10,0xa0);	//read B
	WriteTW88(0x11,0);		//addr 0
	B0=ReadTW88(0x12);			//read twice
	B0=ReadTW88(0x12);		
	//update LUT
	WriteTW88Page( PAGE4_SOSD );
	WriteTW88( 0x10, 0xa0 );    	// LUT Write Mode, En & byte ptr inc. DMA needs it.
	WriteTW88( 0x11, 0); 					// LUT addr. set 0 on DMA

	///if(SOsdHwBuff_win[winno*8] == 0) 
		///continue;
	data_p = &SOsdHwBuff_win[winno*8+1];
	//Spi Flash DMA
	asm nop;
	WriteTW88( TWBASE_SPI+0x04, 0x00 );	// DMA stop	
	WriteTW88( TWBASE_SPI+0x03, 0x80 | SPICMD_BYTES );
	WriteTW88( TWBASE_SPI+0x0a, SPICMD_READ ); // SPI Command=R
	asm nop;
	WriteTW88( TWBASE_SPI+0x08, *data_p++ );		//size0
	WriteTW88( TWBASE_SPI+0x09, *data_p++ );		//size1
	LutOffset = *data_p;
	LutOffset <<= 6;
	WriteTW88( TWBASE_SPI+0x06, *data_p++ ); 	//LutOffset[8:6] -> R4C6[2:0]
	LutOffset |= (*data_p & 0x3F);
	WriteTW88( TWBASE_SPI+0x07, *data_p++ );		//LutOffset[5:0] -> R4C7[7:2] 
	WriteTW88( TWBASE_SPI+0x0b, *data_p++); 		//address0
	WriteTW88( TWBASE_SPI+0x0c, *data_p++ );		//address1
	WriteTW88( TWBASE_SPI+0x0d, *data_p++ ); 	//address2
	WaitVBlank(1);///防止在同一界面下，当一个图片在切换时的条纹现象
	WriteTW88( TWBASE_SPI+0x04, 0x01 ); // DMA Start
	WriteTW88(0x10,0xa0);		//read B
	WriteTW88(0x11,0);			//addr 0
	B=ReadTW88(0x12);				//read twice
	B=ReadTW88(0x12);		

	asm nop;
	if(LutOffset) 
	{
		if(B0 != B) 
		{
			WriteTW88(0x11, 0);			//addr 0
		}
	}
	else 
	{
		B0 = B;
	}
	asm nop;
	
	WriteTW88Page( PAGE4_SOSD );
	if(SOsdHwBuff_rle_win) 
	{
		WriteTW88(0x04, ReadTW88(0x04) | 0x01);
		WriteTW88(0x05, ((SOsdHwBuff_rle_bpp==7?8:SOsdHwBuff_rle_bpp) << 4) | (SOsdHwBuff_rle_count));
		WriteTW88(0x06, SOsdHwBuff_rle_win);
	}
	else 
	{
		WriteTW88(0x04, ReadTW88(0x04) & 0xFE);
		WriteTW88(0x05, 0);
		WriteTW88(0x06, 0);
	}

	if(SOsdHwBuff_alpha != 0xFFFF) 
	{
		WriteTW88( 0x10, 0xc3 );    		// LUT Write Mode, En & byte ptr inc.
		if(SOsdHwBuff_alpha >> 8)	WriteTW88( 0x10, ReadTW88(0x10) | 0x08);	//support 512 palette
		else            			WriteTW88( 0x10, ReadTW88(0x10) & 0xF7);
		WriteTW88( 0x11, (BYTE)SOsdHwBuff_alpha ); 	// alpha index
		WriteTW88( 0x12, 0x7F); 			// alpha value
	}
	if(WaitFlag!=0)
		WaitVBlank(1);
	data_p = &SpiWinBuff[ 2 << 4];
	for(i=2; i <= 9; i++) 
	{
		reg_i = (i << 4) + 0x20;
		for(j=0; j < 16; j++) {
			WriteTW88(reg_i++, *data_p++);	
		}
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
void SOsdWinBuffWrite2Hw(BYTE start, BYTE end)
{
	BYTE win;
	BYTE *data_p;
	BYTE i,j;
	BYTE reg_i;
	BYTE B0, B;	 //bugrate random 
	WORD LutOffset;

	//to fix LUT0 B0 debug.
	WriteTW88Page( PAGE4_SOSD );
	WriteTW88(0x10,0xa0);	//read B
	WriteTW88(0x11,0);		//addr 0
	B0=ReadTW88(0x12);			//read twice
	B0=ReadTW88(0x12);		
	//update LUT
	WriteTW88Page( PAGE4_SOSD );
	WriteTW88( 0x10, 0xa0 );    	// LUT Write Mode, En & byte ptr inc. DMA needs it.
	WriteTW88( 0x11, 0); 					// LUT addr. set 0 on DMA

	for(win=start; win <= end; win++) 
	{
		if(SOsdHwBuff_win[win*8] == 0) 
			continue;
		data_p = &SOsdHwBuff_win[win*8+1];
		//Spi Flash DMA
		asm nop;
		WriteTW88( TWBASE_SPI+0x04, 0x00 );	// DMA stop	
		WriteTW88( TWBASE_SPI+0x03, 0x80 | SPICMD_BYTES );
		WriteTW88( TWBASE_SPI+0x0a, SPICMD_READ ); // SPI Command=R
		asm nop;
		WriteTW88( TWBASE_SPI+0x08, *data_p++ );		//size0
		WriteTW88( TWBASE_SPI+0x09, *data_p++ );		//size1
		LutOffset = *data_p;
		LutOffset <<= 6;
		WriteTW88( TWBASE_SPI+0x06, *data_p++ ); 	//LutOffset[8:6] -> R4C6[2:0]
		LutOffset |= (*data_p & 0x3F);
		WriteTW88( TWBASE_SPI+0x07, *data_p++ );		//LutOffset[5:0] -> R4C7[7:2] 
		WriteTW88( TWBASE_SPI+0x0b, *data_p++); 		//address0
		WriteTW88( TWBASE_SPI+0x0c, *data_p++ );		//address1
		WriteTW88( TWBASE_SPI+0x0d, *data_p++ ); 	//address2
		WaitVBlank(1);///防止在同一界面下，当一个图片在切换时的条纹现象
		WriteTW88( TWBASE_SPI+0x04, 0x01 ); // DMA Start
		WriteTW88(0x10,0xa0);		//read B
		WriteTW88(0x11,0);			//addr 0
		B=ReadTW88(0x12);				//read twice
		B=ReadTW88(0x12);		

		asm nop;
		if(LutOffset) 
		{
			if(B0 != B) 
			{
				WriteTW88(0x11, 0);			//addr 0
			}
		}
		else 
		{
			B0 = B;
		}
		asm nop;
	}
	WriteTW88Page( PAGE4_SOSD );
	if(SOsdHwBuff_rle_win) 
	{
		WriteTW88(0x04, ReadTW88(0x04) | 0x01);
		WriteTW88(0x05, ((SOsdHwBuff_rle_bpp==7?8:SOsdHwBuff_rle_bpp) << 4) | (SOsdHwBuff_rle_count));
		WriteTW88(0x06, SOsdHwBuff_rle_win);
	}
	else 
	{
		WriteTW88(0x04, ReadTW88(0x04) & 0xFE);
		WriteTW88(0x05, 0);
		WriteTW88(0x06, 0);
	}

	if(SOsdHwBuff_alpha != 0xFFFF) 
	{
		WriteTW88( 0x10, 0xc3 );    		// LUT Write Mode, En & byte ptr inc.
		if(SOsdHwBuff_alpha >> 8)	WriteTW88( 0x10, ReadTW88(0x10) | 0x08);	//support 512 palette
		else            			WriteTW88( 0x10, ReadTW88(0x10) & 0xF7);
		WriteTW88( 0x11, (BYTE)SOsdHwBuff_alpha ); 	// alpha index
		WriteTW88( 0x12, 0x7F); 			// alpha value
	}
	data_p = &SpiWinBuff[ 2 << 4];
	for(i=2; i <= 9; i++) 
	{
		reg_i = (i << 4) + 0x20;
		for(j=0; j < 16; j++) {
			WriteTW88(reg_i++, *data_p++);	
		}
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
void MenuPrepareImageHeader(const SLIDEIMAGE *Image_Flash_Addr_Start,struct image_header_info_s *header,const struct RLE2_HEADER *rle_header)
{
	///const struct image_item_info_s *image;
	WORD colors;
	BYTE bpp;

	///image->loc=SPI_FLASH_Addr->start;
	///image->type=IMAGE_ITEM_TYPE_1;
	
	///if(image->type==IMAGE_ITEM_TYPE_1) 
	///{
		//if we are using MREL, we need to skip 0x10 size.  
		header->lut_type = rle_header->LUT_format & 0x01;
		colors = (WORD)rle_header->LUT_colors+1; //word
		for(bpp=0; bpp < 9 ;) {
			if(colors & 0x01)
				break;
			colors >>= 1;
			bpp++;
		}
		header->bpp = bpp;
		header->rle = rle_header->rledata_cnt & 0x0F;
		header->dx = rle_header->w;
		header->dy = rle_header->h;
		header->lut_size = 0x004 << bpp;
		
		header->lut_loc = Image_Flash_Addr_Start->start + MRLE_INFO_SIZE;
		header->image_loc = Image_Flash_Addr_Start->start + header->lut_size + MRLE_INFO_SIZE;
		asm nop;
	///}
	/*else if(image->type==IMAGE_ITEM_TYPE_2)
	{
		//if we are using MREL, we need to skip 0x10 size.  
		//data from spiflash 
		header->lut_type = image->info->lut_type;
		header->bpp = image->info->rle >> 4;
		header->rle = image->info->rle & 0x0F;
		header->dx = image->info->dx;
		header->dy = image->info->dy;
		header->lut_size = 0x004 << header->bpp;
		
		header->lut_loc = Image_Flash_Addr_Start->start + MRLE_INFO_SIZE;
		header->image_loc = Image_Flash_Addr_Start->start + header->lut_size + MRLE_INFO_SIZE;
		asm nop;
	}
	else 
	{
		header->lut_type = image->info->lut_type;
		header->bpp = image->info->rle >> 4;
		header->rle = image->info->rle & 0x0F;
		header->dx = image->info->dx;
		header->dy = image->info->dy;
		header->lut_size = 0x004 << header->bpp;
		
		header->lut_loc = Image_Flash_Addr_Start->start;
		header->image_loc = Image_Flash_Addr_Start->start + header->lut_size;
		asm nop;
	}*/
	
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
void	Display_Layer_Graphics
(const SLIDEIMAGE *Image_Flash_Addr_Start,struct image_header_info_s *header,const struct menu_item_info *image_Disp,
	const struct RLE2_HEADER *rle_header,unsigned char Window)
{
	MenuPrepareImageHeader(Image_Flash_Addr_Start,header,rle_header);
	
	SpiOsdWinImageLoc( Window, header->image_loc); 
	SpiOsdWinImageSizeWH( Window, header->dx, header->dy );
	SpiOsdWinScreen( Window, image_Disp->Menu_X, image_Disp->Menu_Y, header->dx, header->dy );
	///SpiOsdWinPixelAlpha(Window, ON );
	SpiOsdWinPixelWidth(Window, header->bpp);
	SpiOsdWinLutOffset(Window,image_Disp->Menu_Lut);

	SpiOsdWinBuffEnable( Window, ON );
	
	if(header->rle)
		SOsdHwBuffSetRle(Window,header->bpp,header->rle);
	///WaitVBlank(1);
	///当image_Disp->Menu_Lut为0时通过DMA方式获取LUT，否则非DMA
	SOsdHwBuffSetLut(Window, image_Disp->Menu_Lut, header->lut_size, header->lut_loc);
	///WaitVBlank(1);
	//pixel alpha blending. after load Palette
	///if(image->alpha != 0xFF)
		///SOsdHwBuffSetAlpha(image_Disp->Menu_Lut+image->alpha);
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
void	SpiOsdWinAllOff(void)
{
	unsigned char i;

	SOsdWinBuffAllClean();
	SpiOsdEnableRLC(OFF);		//disable RLE
	for(i=0;i<=8;i++)
		SpiOsdWinHWEnable(i, OFF);

	///SpiOsdEnable(ON);
	SpiOsdResetRLC(0);
	for(i=0;i<=8;i++)
	{
		SpiOsdWinImageLocBit(i,0);
		SpiOsdWinLutOffset( i, 0);  
		SpiOsdWinFillColor( i, 0);
	}
	///SOsdHwBuffWrite2Hw();
}
#pragma CODE_SEG DEFAULT


