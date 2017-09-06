#undef	EXT
#ifdef	CPUINIT_C
#define	EXT
#else
#define	EXT extern
#endif
	/* common defines and macros */
	#include <hidef.h>      	
	#include <mc9s12xhz512.h>
	#include	"typedef.h"
  /******funtion macros****************************/
	#pragma CODE_SEG  USER_ROM
  void CpuInit(void);
  void sleep_mode(void);
  void CAN0_Init(void);
	static  void CAN1Init(void);
  static void pim_init(void);
  static void CRGModuleInitial(void);
  static void RamInit(void);
  static void AD_Init(void);
  static void pwm_Init(void);
  static void init_motor(void);
  static void init_timer(void);
  static void IrqOpen(void);
  static void IrqClose(void);
  static void XirqOpen(void);
  static void SetupXGATE(void);
  static void Vector_Init(void);
	static void pwm_Init(void);
	static void	Init_Capture_Funtion(void);
	#pragma CODE_SEG DEFAULT
  /******************************************************************
  Xgate macros
   ******************************************************************/
  #define ROUTE_INTERRUPT(vec_adr, cfdata)                \
  INT_CFADDR= (vec_adr) & 0xF0;                         \
  INT_CFDATA_ARR[((vec_adr) & 0x0F) >> 1]= (cfdata)

  #define SOFTWARETRIGGER0_VEC  0x72 /* vector address= 2 * channel id */
  #define tchannel0             0xEE /* vector address=2 */
  #define tchannel1             0xEC /* vector address=2 */
  #define tchannel2             0xEA /* vector address=2 */
  #define tchannel3             0xE8 /* vector address=2 */
  #define tchannel4             0xE6 /* vector address=2 */
  #define tchannel5             0xE4 /* vector address=2 */
  #define tchannel6             0xE2 /* vector address=2 */
	#define	MSCAN1Rec_VEC				 	2*0X55
	#define	CAN1error_int_VEC		 	2*0x56
	#define	Capture6_Int_VEC			2*0x71

