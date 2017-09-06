#ifndef _TW_FontOsd_Picture_H
#define _TW_FontOsd_Picture_H
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* common defines and macros */

#include	"Typedef.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///字符的透明度
#define	ALPHA_TRANS_VALUE			0x0f///全透明
#define	ALPHA_NO_TRANS_VALUE	0X00///不透明
#define	ALPHA_HALF_TRANS_VALUE	0X07///半透明
///字符的像素
#define	FONT_OSD_PIXELS_12		0x0a///12*18字符像素
#define	FONT_OSD_PIXELS_16		0x5a///16*26字符像素
///字符的高度
#define	COMMON_CHARACTER_HEIGHT	0x0d  //26
///在字库中的位置
/*#define	FONT_WORD_QING				0X0A///清字符
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
#define	FONT_WORD_XING					0X1B//
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
}; */











#endif

