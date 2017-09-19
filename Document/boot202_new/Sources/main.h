#include <hidef.h>			 /* common defines and macros */
#include <MC9S12XET256.h>  /* derivative information */
#include "my_vectors.h"
#include "flash.h"
#include "can0.h"
#include "type.h"
#include "sci.h"
#include "IFsh1.h"

#define SizeMsg		12		//Number of characteres divided by 2

///volatile unsigned char position @0xc000;
#define XMODEM_NUL 0x00
#define XMODEM_SOH 0x01
#define XMODEM_STX 0x02
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_CAN 0x18
#define XMODEM_EOF 0x1A
#define XMODEM_RECIEVING_WAIT_CHAR 'C'


void CRGModuleInitial(void);


unsigned int add;
byte gotdata1; 
//extern uint buff[4],buffa[4];






