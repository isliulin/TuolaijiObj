#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"
//#include "BitMacros.h"
#include "typedef.h"


//config file position
#define     CONFIG_ADDRESS      0xFC8000     //ǰ260���ֽ�Ϊ��Ϣͷ  ��Ե�ַ�� 0��id����
                                             //                                1��55���ֽ���
                                             //                                2��56���ֽ��� 
                                             //                                2��57���ֽ��� 
                                             //                                2��58���ֽ��� 
//watch dog IO config
#define   WDGDR   DDRE_DDRE4
#define   WDGIO   PORTE_PE4

//spi IO config
#define  SCK_IO                   DDRS_DDRS6
#define  SCK                      PTS_PTS6
#define  CS1_IO                   DDRS_DDRS7
#define  CS1                      PTS_PTS7
#define  SI_IO                    DDRS_DDRS5
#define  SI                       PTS_PTS5
#define  SO_IO                    DDRS_DDRS4
#define  SO                       PTS_PTS4

#define  DEBUG                    0


#endif
