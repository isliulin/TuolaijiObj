#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"
//#include "BitMacros.h"
#include "typedef.h"


//config file position
#define     CONFIG_ADDRESS      0xFC8000     //前260个字节为信息头  相对地址： 0：id个数
                                             //                                1：55的字节数
                                             //                                2：56的字节数 
                                             //                                2：57的字节数 
                                             //                                2：58的字节数 
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
