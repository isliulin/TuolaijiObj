#ifndef	__TYPEDEF_H__
#define	__TYPEDEF_H__

//#include	"BitMacros.h"

#define DATA		data
#define PDATA		pdata
#define IDATA		idata
#define XDATA		xdata
#define CODE		code
#define CONST		code
#define CODE_P
//#define FAR
typedef unsigned char uchar;
typedef unsigned int uint;   

typedef unsigned char BOOL;
typedef signed char S8,*PS8;
typedef unsigned char BYTE,U8,*PU8;
typedef signed short int S16,*PS16;
typedef unsigned short int WORD,U16,*PU16;
typedef signed long LONG,S32,*PS32;
typedef unsigned long DWORD,U32,*PU32;
typedef unsigned char	Register;

#define	TRUE	1
#define	FALSE	0

typedef union _BYTESTRUCT
{
	struct
	{
		unsigned bit0:1;
		unsigned bit1:1;
		unsigned bit2:1;
		unsigned bit3:1;
		unsigned bit4:1;
		unsigned bit5:1;
		unsigned bit6:1;
		unsigned bit7:1;
	}b;	
	unsigned char Byte;
}BYTEST;



#endif // __TYPEDEFS_H__
