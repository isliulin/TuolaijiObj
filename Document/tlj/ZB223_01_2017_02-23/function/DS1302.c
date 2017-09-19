#include "DS1302.h"

/* 发送一个字节到 DS1302 通信总线上 */
void DS1302ByteWrite(unsigned char dat)
{
	unsigned char mask;
	for (mask=0x01; mask!=0; mask<<=1) //低位在前，逐位移出
	{
		if ((mask&dat) != 0) //首先输出该位数据
			DS1302_IO_H;
		else
			DS1302_IO_L;
		DS1302_DEALY(10);
		DS1302_CK_H; //然后拉高时钟
		DS1302_DEALY(10);
		DS1302_CK_L; //再拉低时钟，完成一个位的操作
	}
	DS1302_IO_H; //最后确保释放 IO 引脚
}

/* 由 DS1302 通信总线上读取一个字节 */

unsigned char DS1302ByteRead()
{
	unsigned char mask;
	unsigned char dat = 0;
	DS1302_IO_PDDR_IN;
	DS1302_DEALY(5);
	for (mask=0x01; mask!=0; mask<<=1) //低位在前，逐位读取
	{
		if (DS1302_IO_READ != 0) //首先读取此时的 IO 引脚，并设置 dat 中的对应位
		{
			dat |= mask;
		}
		DS1302_DEALY(10);
		DS1302_CK_H; //然后拉高时钟
		DS1302_DEALY(10);
		DS1302_CK_L; //再拉低时钟，完成一个位的操作
	}
	DS1302_IO_PDDR_OUT;
	return dat; //最后返回读到的字节数据
}
/* 用单次写操作向某一寄存器写入一个字节， reg-寄存器地址， dat-待写入字节 */
void DS1302SingleWrite(unsigned char reg, unsigned char dat)
{
	DS1302_CE_H; //使能片选信号
	DS1302ByteWrite((reg<<1)|0x80); //发送写寄存器指令
	DS1302_DEALY(10);
	DS1302ByteWrite(dat); //写入字节数据
	DS1302_CE_L; //除能片选信号
}

/* 用单次读操作从某一寄存器读取一个字节， reg-寄存器地址，返回值-读到的字节 */
unsigned char DS1302SingleRead(unsigned char reg)
{
	unsigned char dat;
	DS1302_CE_H; //使能片选信号
	DS1302_DEALY(30);
	DS1302ByteWrite((reg<<1)|0x81); //发送读寄存器指令
	DS1302_DEALY(30);
	dat = DS1302ByteRead(); //读取字节数据
	DS1302_CE_L; //除能片选信号
	return dat;
}

/* 用突发模式连续写入 8 个寄存器数据， dat-待写入数据指针 */
void DS1302BurstWrite(unsigned char *dat)
{
	unsigned char i;
	DS1302_CE_H;
	DS1302ByteWrite(0xBE); //发送突发写寄存器指令
	DS1302_DEALY(30);
	for (i=0; i<8; i++) //连续写入 8 字节数据
	{
		DS1302ByteWrite(dat[i]);
	}
	DS1302_CE_L;
}
/* 用突发模式连续读取 8 个寄存器的数据， dat-读取数据的接收指针 */
void DS1302BurstRead(unsigned char *dat)
{
	unsigned char i;
	DS1302_CE_H;
	DS1302ByteWrite(0xBF); //发送突发读寄存器指令
	DS1302_DEALY(30);
	for (i=0; i<8; i++) //连续读取 8 个字节
	{
		dat[i] = DS1302ByteRead();
	}
	DS1302_CE_L;
}

void doGetRealTimeCompatible(unsigned char *src,unsigned char *dst){
	
	dst[0] = src[6];//year
	dst[1] = src[4];//Month
	dst[2] = src[3];//date

	dst[3] = src[2];//H
	dst[4] = src[1];//m
	dst[5] = src[0];//s
}

void initDS1302pin(void){
	DS1302_CE_MUX();DS1302_CE_PDDR_OUT;DS1302_CE_L;
	DS1302_CK_MUX();DS1302_CK_PDDR_OUT;DS1302_CK_H;
	DS1302_IO_MUX();DS1302_IO_PDDR_OUT;DS1302_IO_H;
}

void InitDS1302(void)
{
	unsigned char dat;
	unsigned char InitTime[] = { //2013 年 10 月 8 日 星期二 12:30:00
	0x55,0x30,0x12, 0x10, 0x10, 0x05, 0x16
	};
	initDS1302pin();
	DS1302_DEALY(100);	
	//dat = DS1302SingleRead(0); //读取秒寄存器
	
        //DS1302BurstWrite(InitTime); //设置 DS1302 为默认的初始时间
        
        dat = DS1302SingleRead(0); //读取秒寄存器
	if ((dat & 0x80) != 0) //由秒寄存器最高位 CH 的值判断 DS1302 是否已停止
	{
		DS1302SingleWrite(7, 0x00); //撤销写保护以允许写入数据
		DS1302BurstWrite(InitTime); //设置 DS1302 为默认的初始时间
	}
}

