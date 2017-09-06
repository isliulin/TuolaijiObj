#include "DS1302.h"

/* ����һ���ֽڵ� DS1302 ͨ�������� */
void DS1302ByteWrite(unsigned char dat)
{
	unsigned char mask;
	for (mask=0x01; mask!=0; mask<<=1) //��λ��ǰ����λ�Ƴ�
	{
		if ((mask&dat) != 0) //���������λ����
			DS1302_IO_H;
		else
			DS1302_IO_L;
		DS1302_DEALY(10);
		DS1302_CK_H; //Ȼ������ʱ��
		DS1302_DEALY(10);
		DS1302_CK_L; //������ʱ�ӣ����һ��λ�Ĳ���
	}
	DS1302_IO_H; //���ȷ���ͷ� IO ����
}

/* �� DS1302 ͨ�������϶�ȡһ���ֽ� */

unsigned char DS1302ByteRead()
{
	unsigned char mask;
	unsigned char dat = 0;
	DS1302_IO_PDDR_IN;
	DS1302_DEALY(5);
	for (mask=0x01; mask!=0; mask<<=1) //��λ��ǰ����λ��ȡ
	{
		if (DS1302_IO_READ != 0) //���ȶ�ȡ��ʱ�� IO ���ţ������� dat �еĶ�Ӧλ
		{
			dat |= mask;
		}
		DS1302_DEALY(10);
		DS1302_CK_H; //Ȼ������ʱ��
		DS1302_DEALY(10);
		DS1302_CK_L; //������ʱ�ӣ����һ��λ�Ĳ���
	}
	DS1302_IO_PDDR_OUT;
	return dat; //��󷵻ض������ֽ�����
}
/* �õ���д������ĳһ�Ĵ���д��һ���ֽڣ� reg-�Ĵ�����ַ�� dat-��д���ֽ� */
void DS1302SingleWrite(unsigned char reg, unsigned char dat)
{
	DS1302_CE_H; //ʹ��Ƭѡ�ź�
	DS1302ByteWrite((reg<<1)|0x80); //����д�Ĵ���ָ��
	DS1302_DEALY(10);
	DS1302ByteWrite(dat); //д���ֽ�����
	DS1302_CE_L; //����Ƭѡ�ź�
}

/* �õ��ζ�������ĳһ�Ĵ�����ȡһ���ֽڣ� reg-�Ĵ�����ַ������ֵ-�������ֽ� */
unsigned char DS1302SingleRead(unsigned char reg)
{
	unsigned char dat;
	DS1302_CE_H; //ʹ��Ƭѡ�ź�
	DS1302_DEALY(30);
	DS1302ByteWrite((reg<<1)|0x81); //���Ͷ��Ĵ���ָ��
	DS1302_DEALY(30);
	dat = DS1302ByteRead(); //��ȡ�ֽ�����
	DS1302_CE_L; //����Ƭѡ�ź�
	return dat;
}

/* ��ͻ��ģʽ����д�� 8 ���Ĵ������ݣ� dat-��д������ָ�� */
void DS1302BurstWrite(unsigned char *dat)
{
	unsigned char i;
	DS1302_CE_H;
	DS1302ByteWrite(0xBE); //����ͻ��д�Ĵ���ָ��
	DS1302_DEALY(30);
	for (i=0; i<8; i++) //����д�� 8 �ֽ�����
	{
		DS1302ByteWrite(dat[i]);
	}
	DS1302_CE_L;
}
/* ��ͻ��ģʽ������ȡ 8 ���Ĵ��������ݣ� dat-��ȡ���ݵĽ���ָ�� */
void DS1302BurstRead(unsigned char *dat)
{
	unsigned char i;
	DS1302_CE_H;
	DS1302ByteWrite(0xBF); //����ͻ�����Ĵ���ָ��
	DS1302_DEALY(30);
	for (i=0; i<8; i++) //������ȡ 8 ���ֽ�
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
	unsigned char InitTime[] = { //2013 �� 10 �� 8 �� ���ڶ� 12:30:00
	0x55,0x30,0x12, 0x10, 0x10, 0x05, 0x16
	};
	initDS1302pin();
	DS1302_DEALY(100);	
	//dat = DS1302SingleRead(0); //��ȡ��Ĵ���
	
        //DS1302BurstWrite(InitTime); //���� DS1302 ΪĬ�ϵĳ�ʼʱ��
        
        dat = DS1302SingleRead(0); //��ȡ��Ĵ���
	if ((dat & 0x80) != 0) //����Ĵ������λ CH ��ֵ�ж� DS1302 �Ƿ���ֹͣ
	{
		DS1302SingleWrite(7, 0x00); //����д����������д������
		DS1302BurstWrite(InitTime); //���� DS1302 ΪĬ�ϵĳ�ʼʱ��
	}
}

