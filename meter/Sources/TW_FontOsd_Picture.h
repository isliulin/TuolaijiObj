#ifndef _TW_FontOsd_Picture_H
#define _TW_FontOsd_Picture_H
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* common defines and macros */
#include <hidef.h>      	
#include <mc9s12xhz512.h>
#include	"typedef.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///�ַ���͸����
#define	ALPHA_TRANS_VALUE			0x0f///ȫ͸��
#define	ALPHA_NO_TRANS_VALUE	0X00///��͸��
#define	ALPHA_HALF_TRANS_VALUE	0X07///��͸��
///�ַ�������
#define	FONT_OSD_PIXELS_12		0x0a///12*18�ַ�����
#define	FONT_OSD_PIXELS_16		0x5a///16*26�ַ�����
///�ַ��ĸ߶�
#define	COMMON_CHARACTER_HEIGHT	0x0d
///���ֿ��е�λ��
#define	FONT_WORD_QING				0X0A///���ַ�
#define	FONT_WORD_HENG				0X0B///-
#define	FONT_WORD_GAN_TAN			0X0C///!
#define	FONT_WORD_JING				0X0D///#
#define	FONT_WORD_YUAN				0X0E///$
#define	FONT_WORD_BAI_FEN			0x0F///%
#define	FONT_WORD_YU						0X10///&
#define	FONT_WORD_PIE					0X11/// /
#define	FONT_WORD_ZUO_ZHONG		0X12///[
#define	FONT_WORD_YOU_ZHONG		0X13///]
#define	FONT_WORD_FEI					0X14///~
#define	FONT_WORD_JIA					0X15///+
#define	FONT_WORD_DENG				0X16///=
#define	FONT_WORD_DIAN				0X17///.
#define	FONT_WORD_ZHUANG_DIAN	0X18///:
#define	FONT_WORD_ZHE_DU			0X19///���϶�
#define	FONT_WORD_SHU					0X1A///\
#define	FONT_WORD_XING					0X1B///*
#define	FONT_WORD_ZUO_GUA			0X1C///(
#define	FONT_WORD_YOU_GUA			0X1D///)
#define	FONT_WORD_A						0X1E///A
#define	FONT_WORD_a						0X38///a
#define	FONT_WORD_BU_LIANG		0X52///���ز���
#define	FONT_WORD_DUAN_LU			0X53///���ض�·
#define	FONT_WORD_KAI_LU			0X54///���ؿ�·
#define	FONT_WORD_BU_TONG			0X55///���ز���ͨ
#define	FONT_WORD_JIE_TONG		0X56///���ؽ�ͨ
#define	FONT_WORD_LIANG				0X57///������
#define	FONT_HanZi_Chang			0X58///��
#define	FONT_HanZi_Duan			  0X59///��
#define	FONT_HanZi_Duan1			0X5A///��
#define	FONT_HanZi_Guo				0X5B///��
#define	FONT_HanZi_Kai				0X5C///��
#define	FONT_HanZi_Liu				0X5D///��
#define	FONT_HanZi_Lou				0X5E///©
#define	FONT_HanZi_Lu					0X5F///·
#define	FONT_HanZi_Qi					0X60///��
#define	FONT_HanZi_Yi					0X61///��
#define	FONT_HanZi_Zheng			0X62///��
#define	FONT_HanZi_Di					0X63///��
#define	FONT_HanZi_Gao				0X64///��
#define	FONT_HanZi_Ya					0X65///ѹ
#define	FONT_HanZi_Wen				0X66///��
#define	FONT_HanZi_Gu					0X67///��
#define	FONT_HanZi_Zhang			0X68///��

#define	FONT_HanZi_Wei 					0X69///δ
#define	FONT_HanZi_Xi  					0X6A///��
#define	FONT_HanZi_He  					0X6B///��
#define	FONT_HanZi_Qi1  				0X6C///��
#define	FONT_HanZi_Yong  				0X6D///��
#define	FONT_HanZi_Dong  				0X6E///��
#define	FONT_HanZi_Guan  				0X6F///��
#define	FONT_HanZi_Bi  					0X70///��
#define	FONT_HanZi_Bao  				0X71///��
#define	FONT_HanZi_Jin  				0X72///��
#define FONT_HanZi_Wu  					0X73///��
#define	FONT_HanZi_Xiao  				0X74///Ч
#define	FONT_HanZi_Xiu  				0X75///��
#define	FONT_HanZi_Mian  				0X76///��
#define	FONT_HanZi_Qian  				0X77///Ƿ
#define	FONT_HanZi_Fang  				0X78///��
#define	FONT_HanZi_Dian  				0X79///�� 
#define	FONT_HanZi_Chong  			0X7A///��
#define	FONT_HanZi_OuMu  				0X7B///ŷķ


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const unsigned char ClearFontOsd[]=
{
	FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,
	FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,
	FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,
	FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,
	FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,
	FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,
	FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,
	FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,
	FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,
	FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING,FONT_WORD_QING
};











#endif

