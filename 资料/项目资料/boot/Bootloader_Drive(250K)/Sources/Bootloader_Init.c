#define		_No_Bootloader_Init_EXTERN
#include	"Bootloader_Init.h"
/*****************************************
******************************************/
#pragma MESSAGE DISABLE C4000 
/*****************************************
******************************************/
static void Init_Cop(void)
{
	SOPT2_COPCLKS=0;						
  	SOPT2_COPW=0;   
	SOPT1_COPT = 3;    					/* Watchdog enabled 1.024s*/
}
/*******************************************
********************************************/
static void Init_LVR(void)
{
	SPMSC1_LVDE=1;						/* Low-Voltage Detect Enable */
	SPMSC1_LVDRE=1;						/* Low-Voltage Detect Reset Enable */
	SPMSC2_LVDV =1;						/*LVW Trip Point 4.6V*/
	SPMSC2_LVWV =1;
}
/*******************************************
*******************************************/
static void	Init_MCG(void)
{
	/*  System clock initialization */
  MCGTRM = *(unsigned char*)0xFFAF;    /* Initialize MCGTRM register from a non volatile memory */
  MCGSC = *(unsigned char*)0xFFAE;     /* Initialize MCGSC register from a non volatile memory */
  MCGC2=0x36;/* MCGC2: BDIV=0,RANGE=1,HGO=1,LP=0,EREFS=1,ERCLKEN=1,EREFSTEN=0 */
  MCGC1=0xBA;/* MCGC1: CLKS=2,RDIV=7,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
  MCGC3=0x01;/* MCGC1: CLKS=2,RDIV=7,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
  while(!MCGSC_OSCINIT) {              /* Wait until external reference is stable */
  }
  while(MCGSC_IREFST) {                /* Wait until external reference is selected */
  }
  while((MCGSC & 0x0C) != 0x08) {      /* Wait until external clock is selected as a bus clock reference */
  }
  MCGC2=0x3E;/* MCGC2: BDIV=0,RANGE=1,HGO=1,LP=1,EREFS=1,ERCLKEN=1,EREFSTEN=0 */
  MCGC1=0x92;/* MCGC1: CLKS=2,RDIV=2,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
  MCGC3=0x41;/* MCGC3: LOLIE=0,PLLS=1,CME=0,??=0,VDIV=1 */
}
/*******************************************
********************************************/
void Bootloader_MSCAN_Init(unsigned char	CanConfig) 
{   		
	CANCTL0_INITRQ = 1;   											/* MSCAN in initialization mode */

    while (!(CANCTL1_INITAK));									/* Wait for initialization mode acknowledge */
       
    CANCTL1_CANE = 1;      										/* Enable MSCAN module */        
    
    CANCTL1_CLKSRC = Bootloader_CAN_CLK;    		/* Clock source selection */    
    CANCTL1_LOOPB = 0;          									/* Set to 1 for LoopBack Mode, 0 otherwise */    
    CANCTL1_LISTEN = 0;         									/* Not listen only mode */        
                                                                              
    CANBTR1_TSEG_10 = Bootloader_TIME_SEGM1;    /* Time Segment 1 */
    CANBTR1_TSEG_20 = Bootloader_TIME_SEGM2;    /* Time Segment 2 */                             
                                    
    CANBTR0_BRP = Bootloader_CAN_PRESC;   				/* Baud rate prescaler */  
    CANBTR0_SJW = Bootloader_CAN_SYNJW;    			/* Sinchronization jump width */    
    CANBTR1_SAMP = 0;      											/* One sample per bit */	      
             
    CANIDAC_IDAM = 0;         		 								                                                          

	if(CanConfig==MaskMode)
	{
		/*Can Id Mask*/
		if(FrontDriveFlag==1)
			BootCanMask=FrontDriveID;
		else if(BackDriveFlag==1)
			BootCanMask=BackDriveID;
		else if(TopDriveFlag==1)
			BootCanMask=TopDriveID;
		else if(PowerModuleFlag==1)
			BootCanMask=PowerModuleID;
		else if(PowerControlModuleFlag==1)
			BootCanMask=PowerControlModuleID;
		else if(MainTestBoardModuleFlag==1)
			BootCanMask=MainTestBordID;
		else if(SlaveTestBoardModuleFlag==1)
			BootCanMask=SlaveTestBordID;
					
		CANIDAR0 = (BootCanMask>>21) & 0xFF; 	///PGN change to 32bit	 
		CANIDAR1 = (BootCanMask>>18) & 0x07; 
		CANIDAR1 = CANIDAR1 << 5;
		CANIDAR1 = CANIDAR1 | ((BootCanMask>>15) & 0x07);
		CANIDAR2 = (BootCanMask>>7) & 0xFF; 
		CANIDAR3 = (BootCanMask<<1) & 0xFF; 

		CANIDAR4 =CANIDAR0;
		CANIDAR5 =CANIDAR1;
		CANIDAR6 =CANIDAR2;
		CANIDAR7 =CANIDAR3;

		CANIDMR0 = 0x00;               	///设置过滤位
		CANIDMR1 = 0x18;					///SRR及IDE位不过滤
		CANIDMR2 = 0x00;
		CANIDMR3 = 0x01;					///RTR不过滤
		CANIDMR4 = 0x00;
		CANIDMR5 = 0x18;
		CANIDMR6 = 0x00;
		CANIDMR7 = 0x01;
	}
	else if(CanConfig==NormalMode)
	{
		CANIDAR0 = 0xFF;        
 	 	CANIDMR0 = 0xFF;   
	  	CANIDAR1 = 0xFF;     
	  	CANIDMR1 = 0xFF;    
	  
	  	CANIDAR2 = 0xFF;                  
	  	CANIDMR2 = 0xFF;   
	  	CANIDAR3 = 0xFF;                   
	  	CANIDMR3 = 0xFF;    
	  
	  	CANIDAR4 = 0xFF;                   
	  	CANIDMR4 = 0xFF;   
	  	CANIDAR5 = 0xFF;                   
	  	CANIDMR5 = 0xFF;    
	  
	  	CANIDAR6 = 0xFF;                   
	  	CANIDMR6 = 0xFF;   
	  	CANIDAR7 = 0xFF;                   
	  	CANIDMR7 = 0xFF;
	}
		
    CANCTL0_INITRQ = 0;            			/* Exit initialization mode request */

    while (CANCTL1_INITAK);     				/* Wait for normal mode */   

    while(!(CANCTL0_SYNCH));  			/* Wait for CAN synchronization */

	CANRFLG |= 0x01;      						/* Clear receiver flags */     
}
/*******************************************************************

********************************************************************/
void StartStateDetect(void)
{
	unsigned long	 StartDetectTime=0;///开机等待bootloader的时间
	unsigned char	Index;///缓冲变量
	unsigned char  Module_Ok;///缓冲变量

	count_delay=0X00;///清检测报文计时变量
	
	for(;;)
	{
		__RESET_WATCHDOG(); /* 	feeds the dog */
		Bootloader_External_WatchDog();
		
		if(CANRFLG&0x01==0x01)			
		{
			///读取ID	
			rev_can_packet.identifier = CANRIDR0;
			rev_can_packet.identifier <<= 3;
			rev_can_packet.identifier |= (CANRIDR1>>5);
			rev_can_packet.identifier <<= 3;
			rev_can_packet.identifier |= (CANRIDR1&0X07);
			rev_can_packet.identifier <<= 8;
			rev_can_packet.identifier |= CANRIDR2;
			rev_can_packet.identifier <<= 7;
			rev_can_packet.identifier |= (CANRIDR3>>1);	
			CANRIDR0=0x00;///将ID的缓存清零
			CANRIDR1=0x00;  
			CANRIDR2=0x00;
			CANRIDR3=0x00;   
			///读取数据帧长度	
			rev_can_packet.byte_count = (CANRDLR & 0x0F);	
			///读取数据内容
			for (Index=0; Index < rev_can_packet.byte_count; Index++)	
  	    		rev_can_packet.data[Index] = *(&CANRDSR0 + Index); 

			ModuleID=GetID();
			Module_Ok=0;
			if(FrontDriveFlag==1)
			{
				if(ModuleID== FrontDriveID)
					Module_Ok=1;
			}
			else if(BackDriveFlag==1)
			{
				if(ModuleID== BackDriveID)
					Module_Ok=1;
			}
			else if(TopDriveFlag==1)
			{
				if(ModuleID== TopDriveID)
					Module_Ok=1;
			}
			else if(PowerModuleFlag==1)
			{
				if(ModuleID== PowerModuleID)
					Module_Ok=1;
			}
			else if(PowerControlModuleFlag==1)
			{
				if(ModuleID== PowerControlModuleID)
					Module_Ok=1;
			}
			else if(MainTestBoardModuleFlag==1)
			{
				if(ModuleID== MainTestBordID)
					Module_Ok=1;
			}
			else if(SlaveTestBoardModuleFlag==1)
			{
				if(ModuleID== SlaveTestBordID)
					Module_Ok=1;
			}
			if(Module_Ok==1)
			{
				count_delay=0X00;					///收到本模块报文，清检测报文计时变量
				ResponseData=GetFunction();
				if(ResponseData==0x00)			///待机准备
					asm nop;
				else if(ResponseData== 0x01)	///烧录准备
				{
					BootStartFlag=0;
					err_send(ResponseData);
					CAN_SendFrame();
				}
				else if(ResponseData== 0x02)	///擦除主芯片freescale的dz60的FLASH首地址
				{
					if(BootStartFlag==0)
					{
						BootStartFlag=1;
						ReStart_Flash_Write_Addres=rev_can_packet.data[2];
						ReStart_Flash_Write_Addres <<=8;
						ReStart_Flash_Write_Addres +=rev_can_packet.data[3];
						ReStart_Flash_Write_Addres <<=8;
						ReStart_Flash_Write_Addres +=rev_can_packet.data[4];			
						ReStart_Flash_Write_Addres <<=8;
						ReStart_Flash_Write_Addres +=rev_can_packet.data[5];			
						if(ReStart_Flash_Write_Addres==Flash_First_Address)
							ConfigFlag=0;
						else 
							ConfigFlag=1;
					}
					break;
				}
			}
			else
			{
				count_delay++;
				if((count_delay>=Num_Sec2)&&(Last_Position!=0xff)&&(First_Position!=0xff))	///等待4 s，跳转到客户代码
					__asm jmp UserCodeFirstAddr;
			}
			CANRFLG|=0x01;
		}
		else
		{
			count_delay++;
			if((count_delay>=Num_Sec2)&&(Last_Position!=0xff)&&(First_Position!=0xff))	///等待4 s，跳转到客户代码
				__asm jmp UserCodeFirstAddr;
		}
	}
}
/******************************************/
#pragma NO_EXIT
#pragma NO_FRAME
void _EntryPoint(void)
{ 
	INIT_SP_FROM_STARTUP_DESC();	/*initialize stack*/
	/*can io initialize*/
	PTEDD=0b01000000;	///bit7->can rx;bit6->can tx;other is set to input
	PTED=0b01000000;		///first set can tx pin to high to active can
	PTGDD=0b00001000;	///将CAN 总线的STB脚拉低才能正常收发
	PTGD=0b00000000;
	Init_Cop();
	Init_LVR();
	Init_MCG();	
	__asm jmp _Startup;
}
/* Initialization of the CPU registers in FLASH */

/* NVPROT: EPS=3->没有地址受保护;FPS=0x3C ->受保护地址：0xEE00–0xFFFF，共4.5K*/
const unsigned char NVPROT_INIT @0x0000FFBD = 0xFC;

/***** NVOPT: KEYEN=0->不允许后门密钥访问;FNORED=0->向量重定向启用;
EPGMOD=1->每个分区在一个页面中;SEC1=1,SEC0=0 ->1:0表示MCU 的安全状态:不安全*****/
const unsigned char NVOPT_INIT @0x0000FFBF = 0x3E;	
/*********entry point of bootloader********/
void (* near const _vectReset[])() @0xFFFE = 
{ 
    _EntryPoint                    /* Int.no.  0 Vreset (at FFFE),Reset vector */
};

