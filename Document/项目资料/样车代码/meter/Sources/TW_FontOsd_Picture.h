#ifndef _TW_FontOsd_Picture_H
#define _TW_FontOsd_Picture_H
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* common defines and macros */
#include <hidef.h>      	
#include <mc9s12xhz512.h>
#include	"typedef.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///字符的透明度
#define	ALPHA_TRANS_VALUE			0x0f///全透明
#define	ALPHA_NO_TRANS_VALUE	0X00///不透明
#define	ALPHA_HALF_TRANS_VALUE	0X07///半透明
///字符的像素
#define	FONT_OSD_PIXELS_12		0x0a///12*18字符像素
#define	FONT_OSD_PIXELS_16		0x5a///16*26字符像素
///字符的高度
#define	COMMON_CHARACTER_HEIGHT	0x0d
///在字库中的位置
#define	FONT_WORD_QING				0X0A///清字符
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
#define	FONT_WORD_ZHE_DU			0X19///摄氏度
#define	FONT_WORD_SHU					0X1A///\
#define	FONT_WORD_XING					0X1B///*
#define	FONT_WORD_ZUO_GUA			0X1C///(
#define	FONT_WORD_YOU_GUA			0X1D///)
#define	FONT_WORD_A						0X1E///A
#define	FONT_WORD_a						0X38///a
#define	FONT_WORD_BU_LIANG		0X52///负载不亮
#define	FONT_WORD_DUAN_LU			0X53///负载短路
#define	FONT_WORD_KAI_LU			0X54///负载开路
#define	FONT_WORD_BU_TONG			0X55///开关不接通
#define	FONT_WORD_JIE_TONG		0X56///开关接通
#define	FONT_WORD_LIANG				0X57///负载亮
#define	FONT_HanZi_Chang			0X58///常
#define	FONT_HanZi_Duan			  0X59///短
#define	FONT_HanZi_Duan1			0X5A///断
#define	FONT_HanZi_Guo				0X5B///过
#define	FONT_HanZi_Kai				0X5C///开
#define	FONT_HanZi_Liu				0X5D///流
#define	FONT_HanZi_Lou				0X5E///漏
#define	FONT_HanZi_Lu					0X5F///路
#define	FONT_HanZi_Qi					0X60///气
#define	FONT_HanZi_Yi					0X61///异
#define	FONT_HanZi_Zheng			0X62///正
#define	FONT_HanZi_Di					0X63///低
#define	FONT_HanZi_Gao				0X64///高
#define	FONT_HanZi_Ya					0X65///压
#define	FONT_HanZi_Wen				0X66///温
#define	FONT_HanZi_Gu					0X67///故
#define	FONT_HanZi_Zhang			0X68///障

#define	FONT_HanZi_Wei 					0X69///未
#define	FONT_HanZi_Xi  					0X6A///吸
#define	FONT_HanZi_He  					0X6B///合
#define	FONT_HanZi_Qi1  				0X6C///启
#define	FONT_HanZi_Yong  				0X6D///用
#define	FONT_HanZi_Dong  				0X6E///动
#define	FONT_HanZi_Guan  				0X6F///关
#define	FONT_HanZi_Bi  					0X70///闭
#define	FONT_HanZi_Bao  				0X71///报
#define	FONT_HanZi_Jin  				0X72///警
#define FONT_HanZi_Wu  					0X73///无
#define	FONT_HanZi_Xiao  				0X74///效
#define	FONT_HanZi_Xiu  				0X75///休
#define	FONT_HanZi_Mian  				0X76///眠
#define	FONT_HanZi_Qian  				0X77///欠
#define	FONT_HanZi_Fang  				0X78///放
#define	FONT_HanZi_Dian  				0X79///电 
#define	FONT_HanZi_Chong  			0X7A///充
#define	FONT_HanZi_OuMu  				0X7B///欧姆


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

