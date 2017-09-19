//******************************************************************************
// author：        ap
// date:           2016/1/14
//******************************************************************************

#include "ap_S32K1xx_FlexCAN.h"

uint8 SetCANBand(uint8 CANChannel,uint32 baudrateKHz);

//============================================================================
//函数名称：CANInit
//函数返回：0：成功；1：失败
//参数说明：
//         CANChannel：模块号
//		   baudrateKHz: 波特率
//         selfLoop: 模式选择(1=回环模式；0=正常模式)
//         idMask: ID过滤(1=ID过滤；0=ID不过滤)
//功能概要：CAN初始化
//============================================================================
uint8 canInit(uint8 canChannel,uint32 baudrateKHz,uint8 selfLoop,uint8 idMask)
{
	int8 i;
	CAN_Type *canBaseAddress;

	if(canChannel == 0)		
	{
		canBaseAddress = CAN0;
		PCC_FLEXCAN0 |= PCC_FLEXCAN0_CGC_MASK;	//时钟
		//复用
		PORTE->PCR4 = PORT_PCR_MUX(5) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //Rx 上拉
		PORTE->PCR5 = PORT_PCR_MUX(5) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //Tx 上拉
	}
	else if(canChannel == 1)
	{
		canBaseAddress = CAN1;
		PCC_FLEXCAN1 |= PCC_FLEXCAN1_CGC_MASK;
		PORTA_PCR12 = PORT_PCR12_MUX(3) | PORT_PCR12_PE_MASK | PORT_PCR12_PS_MASK; //Rx 上拉
		PORTA_PCR13 = PORT_PCR13_MUX(3) | PORT_PCR13_PE_MASK | PORT_PCR13_PS_MASK; //Tx 上拉
	}
	else if(canChannel == 2)	
	{
		canBaseAddress = CAN2;
		PCC_FLEXCAN2 &= ~PCC_FLEXCAN2_CGC_MASK;
		PCC_FLEXCAN2 |= PCC_FLEXCAN2_CGC_MASK;
		PORTB_PCR12 = PORT_PCR12_MUX(4) | PORT_PCR12_PE_MASK | PORT_PCR12_PS_MASK; //Rx 上拉
		PORTB_PCR13 = PORT_PCR13_MUX(4) | PORT_PCR13_PE_MASK | PORT_PCR13_PS_MASK; //Tx 上拉
	}

	//选择时钟源
	canBaseAddress->CTRL1 |=  CAN_CTRL1_CLKSRC_MASK; //The CAN engine clock source is the peripheral clock.
	canBaseAddress->MCR |= CAN_MCR_FRZ_MASK;  //使能冻结模式   
    canBaseAddress->MCR &= ~CAN_MCR_MDIS_MASK;//使能CAN模块
     //确认已退出冻结模式
    while((CAN_MCR_LPMACK_MASK & canBaseAddress->MCR));
	 //软件复位
    canBaseAddress->MCR ^= CAN_MCR_SOFTRST_MASK; 
    //等待复位完成
    while(CAN_MCR_SOFTRST_MASK & canBaseAddress->MCR);   
    //等待进入冻结模式 
    while(!(CAN_MCR_FRZACK_MASK & canBaseAddress->MCR));
	CAN_MCR_REG(canBaseAddress) |= CAN_MCR_SRXDIS_MASK;//禁止自接受
	for(i = 0; i < 64; i++)
	{
		canBaseAddress->RAMn[i]  = 0;
	}
	
	//接收邮箱过滤IDE比较，RTR不比较
    canBaseAddress->CTRL2 &= ~CAN_CTRL2_EACEN_MASK;
    //远程请求帧产生
    canBaseAddress->CTRL2 &= ~CAN_CTRL2_RRS_MASK;
    //邮箱首先从接收FIFO队列匹配然后再在邮箱中匹配
    canBaseAddress->CTRL2 &= ~CAN_CTRL2_MRP_MASK;
 
    //使用一个32位过滤器
    canBaseAddress->MCR |= (canBaseAddress->MCR & ~CAN_MCR_IDAM_MASK) | CAN_MCR_IDAM(0);
    //设置波特率
    if(SetCANBand(canChannel,baudrateKHz) == 1)	return 1;//若设置错误
        
    //模式选择：回环模式或正常模式
    if(1==selfLoop)
        canBaseAddress->CTRL1 |= CAN_CTRL1_LPB_MASK;//使用回环模式
	//初始化掩码寄存器
    if(1==idMask)//屏蔽ID
    {
    	canBaseAddress->RXMGMASK = 0x1FFFFFFF;
		canBaseAddress->RX14MASK = 0x1FFFFFFF;
		canBaseAddress->RX15MASK = 0x1FFFFFFF;
    }
    else//不屏蔽ID
    {
    	canBaseAddress->RXMGMASK = 0x0;
		canBaseAddress->RX14MASK = 0x0;
		canBaseAddress->RX15MASK = 0x0;
    }

    //如果单独掩码功能使能，为每个队列初始化单独的掩码寄存器
    if(canBaseAddress->MCR & CAN_MCR_IRMQ_MASK)
    {
        for(i = 0; i < NUMBER_OF_MB ; i++)
        {        
            canBaseAddress->RXIMR[i] = 0x1FFFFFFFL;
        }
    }
    
    //只有在冻结模式下才能配置，配置完推出冻结模式
	canBaseAddress->MCR &= ~(CAN_MCR_HALT_MASK);
	//等待直到退出冻结模式
	while( canBaseAddress->MCR & CAN_MCR_FRZACK_MASK);    
	//等到不在冻结模式，休眠模式或者停止模式
	while((canBaseAddress->MCR & CAN_MCR_NOTRDY_MASK));
    
    return 0;
}

//===========================内部函数=========================================
//============================================================================
//函数名称：SetCANBand
//函数返回：0:配置成功 1：配置失败
//参数说明：无
//功能概要：设置CAN的波特率
//============================================================================
uint8 SetCANBand(uint8 canChannel,uint32 baudrateKHz)
{    
    CAN_MemMapPtr CANBaseAdd;
    
    if(canChannel == 0)
        CANBaseAdd = CAN0_BASE_PTR;
    else if(canChannel == 1)
        CANBaseAdd = CAN1_BASE_PTR;
    else if(canChannel == 2)
        CANBaseAdd = CAN2_BASE_PTR;
	else return 1;

	 CANBaseAdd->CTRL1 &=~ ( CAN_CTRL1_PROPSEG_MASK
												| CAN_CTRL1_RJW_MASK
												| CAN_CTRL1_PSEG1_MASK 
												| CAN_CTRL1_PSEG2_MASK
												| CAN_CTRL1_PRESDIV_MASK);
	
    switch (baudrateKHz)
    {
          case (33):	// 33.33K
             if(CANBaseAdd->CTRL1 & CAN_CTRL1_CLKSRC_MASK)
             { 
				 // 48M/200= 400k sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 4, PSEG2 = 4,PRESDIV = 120
				 CANBaseAdd->CTRL1 = (0 
				 								| CAN_CTRL1_PRESDIV(199)
				 								| CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(2)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												);
             }
             else
             { 
				 // 12M/20= 600k sclock, 18Tq
				 // PROPSEG = 1, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 4, PSEG1 = 8, PSEG2 = 8,PRESDIV = 20
				CANBaseAdd->CTRL1 = ( 0  
												| CAN_CTRL1_PROPSEG(0) 
												| CAN_CTRL1_PROPSEG(3)
												| CAN_CTRL1_PSEG1(7) 
												| CAN_CTRL1_PSEG2(7)
												| CAN_CTRL1_PRESDIV(19));
             }
             break;
          case (83):	// 83.33K
             if(CANBaseAdd->CTRL1 & CAN_CTRL1_CLKSRC_MASK)
             {
				 // 80M/80= 1M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 4, PSEG2 = 4,PRESDIV = 48
				 CANBaseAdd->CTRL1 = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(2)
												| CAN_CTRL1_PSEG1(3)
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(79));
             }
             else
             { 
				 // 12M/12= 1M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 6, PSEG2 = 6,PRESDIV = 12
				 CANBaseAdd->CTRL1 = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(2)
												| CAN_CTRL1_PSEG1(5) 
												| CAN_CTRL1_PSEG2(5)
												| CAN_CTRL1_PRESDIV(11));
             }
             break;
          case (50):
             if(CANBaseAdd->CTRL1 & CAN_CTRL1_CLKSRC_MASK)
             {                
				 // 80M/100= 0.8M sclock, 16Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 6, PSEG2 = 6, PRESDIV = 40
				 CANBaseAdd->CTRL1 = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(1)
												| CAN_CTRL1_PSEG1(5) 
												| CAN_CTRL1_PSEG2(5)
												| CAN_CTRL1_PRESDIV(99));
             }
             else
             {
				 // 12M/20= 0.6M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 4, PSEG2 = 4, PRESDIV = 20                 
				 CANBaseAdd->CTRL1 = (0 | CAN_CTRL1_PROPSEG(2)
												| CAN_CTRL1_RJW(2)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(19));                   
             }
             break;
          case (100):
             if(CANBaseAdd->CTRL1 & CAN_CTRL1_CLKSRC_MASK)
             { 
				 // 80M/50= 1.6M sclock, 16Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 6, PSEG2 = 6, PRESDIV = 40
				 CANBaseAdd->CTRL1 = (0 | CAN_CTRL1_PROPSEG(2)
												| CAN_CTRL1_RJW(2)
												| CAN_CTRL1_PSEG1(5) 
												| CAN_CTRL1_PSEG2(5)
												| CAN_CTRL1_PRESDIV(49));
             }
             else
             {
				 // 12M/10= 1.2M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 4, PSEG2 = 4, PRESDIV = 10
				 CANBaseAdd->CTRL1 = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(2)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(9));                   
             }
             break;
          case (125):
             if(CANBaseAdd->CTRL1 & CAN_CTRL1_CLKSRC_MASK)
             {                 
				 // 80M/40= 2M sclock, 16Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 6, PSEG2 = 6, PRESDIV = 32
				 CANBaseAdd->CTRL1 = (0 | CAN_CTRL1_PROPSEG(2) 
											    | CAN_CTRL1_RJW(2)
											    | CAN_CTRL1_PSEG1(5) 
											    | CAN_CTRL1_PSEG2(5)
											    | CAN_CTRL1_PRESDIV(39));
             }
             else
             {
				 // 12M/8= 1.5M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 3, PSEG1 = 4, PSEG2 = 4, PRESDIV = 8
				 CANBaseAdd->CTRL1 = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(2)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(7));                  
             }
             break;
          case (250):
             if(CANBaseAdd->CTRL1 & CAN_CTRL1_CLKSRC_MASK)
             {                
				 // 40M/10= 4M sclock, 16Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 2, PSEG1 = 6, PSEG2 = 6, PRESDIV =  20


				 CANBaseAdd->CTRL1 |= (0 | CAN_CTRL1_PROPSEG(2)
														| CAN_CTRL1_RJW(1)
														| CAN_CTRL1_PSEG1(5) 
														| CAN_CTRL1_PSEG2(5)
														| CAN_CTRL1_PRESDIV(9));



				 
#if 0				

				 
				 CANBaseAdd->CTRL1 |= (0 | CAN_CTRL1_PROPSEG(2)
												| CAN_CTRL1_RJW(1)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(26));
#endif

												
             }
             else
             {
				 // 12M/4= 3M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 2, PSEG1 = 4, PSEG2 = 4, PRESDIV = 4
				 CANBaseAdd->CTRL1 = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(1)
												| CAN_CTRL1_PSEG1(3)
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(3));                   
             }
             break;
          case (500):
             if(CANBaseAdd->CTRL1 & CAN_CTRL1_CLKSRC_MASK)
             {                
				 // 80M/10=8M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 2, PSEG1 = 4, PSEG2 = 4, PRESDIV = 6
				 CANBaseAdd->CTRL1 = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(1)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(9));
             }
             else
             {
				 // 12M/2=6M sclock, 12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 2, PSEG1 = 4, PSEG2 = 4, PRESDIV = 2
				 CANBaseAdd->CTRL1 = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(1)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(1));                   
             }
             break;
          case (1000): 
             if(CANBaseAdd->CTRL1 & CAN_CTRL1_CLKSRC_MASK)
             {                  
				 // 80M/8=8M sclock
				 // PROPSEG = 4, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 1, PSEG1 = 1, PSEG2 = 2, PRESCALER = 6
				 CANBaseAdd->CTRL1 = (0 | CAN_CTRL1_PROPSEG(3) 
												| CAN_CTRL1_RJW(0)
												| CAN_CTRL1_PSEG1(0)
												| CAN_CTRL1_PSEG2(1)
												| CAN_CTRL1_PRESDIV(7));
             }
             else
             {  
				 // 12M/1=12M sclock,12Tq
				 // PROPSEG = 3, LOM = 0x0, LBUF = 0x0, TSYNC = 0x0, SAMP = 1
				 // RJW = 4, PSEG1 = 4, PSEG2 = 4, PRESCALER = 1
				 CANBaseAdd->CTRL1 = (0 | CAN_CTRL1_PROPSEG(2) 
												| CAN_CTRL1_RJW(3)
												| CAN_CTRL1_PSEG1(3) 
												| CAN_CTRL1_PSEG2(3)
												| CAN_CTRL1_PRESDIV(0));
             }
             break;
          default: 
             return 1;
       }
    return 0;
}

//============================================================================
//函数名称：CANEnableRXBuff
//函数返回：无
//参数说明： CANChannel：CAN模块号
//          iMB：缓冲区号
//          id：id号
//功能概要：使能接收缓冲区
//============================================================================
void CANEnableRXBuff(uint8 CANChannel,uint16 iMB)
{
    //uint32 id2;
    uint32 cs = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_RX_EMPTY); 
    //CAN_MemMapPtr CANBaseAdd;
    canMBsType0	*MBs0;
    if(CANChannel == 0)
	{
		//CANBaseAdd = CAN0_BASE_PTR;
		MBs0 = (canMBsType0	*)(CAN0_BASE + 0x80); 
	}        
    else if(CANChannel == 1)
	{
		MBs0 = (canMBsType0	*)(CAN1_BASE + 0x80); 
	}
	else if(CANChannel == 2)
	{
		MBs0 = (canMBsType0	*)(CAN2_BASE + 0x80); 
	}
     

    //将MB配置为非激活状态
   MBs0[iMB].CS = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_RX_INACTIVE); 	
    
    //取出29位单独的ID
    //id2 = id & ~(CAN_MSG_IDE_MASK | CAN_MSG_TYPE_MASK);
    //if(1)//if(id & CAN_MSG_IDE_MASK)//扩展帧
   // {
    //    MBs0[iMB].ID = id2 ;
        cs |= FLEXCAN_MB_CS_IDE;//置位IDE位
   /// }
   // else//标准帧
   // {
   //     MBs0[iMB].ID = id2 << FLEXCAN_MB_ID_STD_BIT_NO;          
    //}
    
    //激活接收缓冲区，code写0100
    MBs0[iMB].CS = cs;      
}

//============================================================================
//函数名称：EnableCANInterrupt
//函数返回：无
//参数说明：
//          CANChannel:CAN模块号
//          iMB:缓冲区号
//功能概要：使能缓冲区接收和发送中断
//============================================================================
void EnableCANInterrupt(uint8 CANChannel,uint16 iMB)
{
    CAN_MemMapPtr CANBaseAdd;
    
    if(CANChannel == 0){
		CANBaseAdd = CAN0_BASE_PTR;
		enable_irq (81);
	}
    else if(CANChannel == 1){
		CANBaseAdd = CAN1_BASE_PTR;
		enable_irq(88);  
	}  
	else if(CANChannel == 2){
		CANBaseAdd = CAN2_BASE_PTR;
		enable_irq(95);  
	}
        
    CAN_IMASK1_REG(CANBaseAdd) = (1<<iMB);

	
}

//============================================================================
//函数名称：DisableCANInterrupt
//函数返回：无
//参数说明：
//         CANChannel:CAN模块号
//         iMB:缓冲区号
//功能概要：禁止缓冲区接收和发送中断
//============================================================================
void DisableCANInterrupt(uint8 CANChannel,uint16 iMB)
{
    CAN_MemMapPtr CANBaseAdd;
    
    if(CANChannel == 0)
        CANBaseAdd = CAN0_BASE_PTR;
    else if(CANChannel == 1)
        CANBaseAdd = CAN1_BASE_PTR;
	else if(CANChannel == 2)
        CANBaseAdd = CAN2_BASE_PTR;

    
    CAN_IMASK1_REG(CANBaseAdd) &= ~(1<<iMB);
}

//============================================================================
//函数名称：CANClearFlag
//函数返回：无
//参数说明：
//         CANChannel:CAN模块号
//         iMB:缓冲区号
//功能概要：清缓冲区中断标志
//============================================================================
void CANClearFlag(uint8 CANChannel,uint16 iMB)
{
    CAN_MemMapPtr CANBaseAdd;
    
    if(CANChannel == 0)
        CANBaseAdd = CAN0_BASE_PTR;
    else if(CANChannel == 1)
        CANBaseAdd = CAN1_BASE_PTR;
	else if(CANChannel == 2)
        CANBaseAdd = CAN2_BASE_PTR;


    CANBaseAdd->IFLAG1 = (1<<iMB);
}
//============================================================================
//函数名称：CANGetFlag
//函数返回：无
//参数说明：
//         CANChannel:CAN模块号
//         iMB:缓冲区号
//功能概要：获得缓冲区中断标志
//============================================================================
uint32 CANGetFlag(uint8 CANChannel,uint16 iMB)
{
    CAN_MemMapPtr CANBaseAdd;
    
    if(CANChannel == 0)
        CANBaseAdd = CAN0_BASE_PTR;
    else if(CANChannel == 1)
        CANBaseAdd = CAN1_BASE_PTR;
	else if(CANChannel == 2)
        CANBaseAdd = CAN2_BASE_PTR;


    return (CANBaseAdd->IFLAG1 & (1<<iMB));
}


//============================================================================
//函数名称：CANSendData
//函数返回：0：成功，1：失败
//参数说明：
//         CANChannel：模块号         
//         iMB：缓冲区号
//		   id: ID号
//		   length：数据长度
//		   Data[8]:发送数据缓冲区
//功能概要：发送数据
//============================================================================
uint8 CANSendData(uint8 CANChannel,uint16 iMB, uint32 id,uint8 length,uint8 Data[])
{
    int16  i,wno,bno;
    uint32 word[2] = {0};
    CAN_MemMapPtr CANBaseAdd;
    canMBsType0	*MBs;
	
    if(CANChannel == 0)
	{
		CANBaseAdd = CAN0_BASE_PTR;
		MBs = (canMBsType0	*)(CAN0_BASE + 0x80); 
	}        
    else if(CANChannel == 1)
	{
		CANBaseAdd = CAN1_BASE_PTR;
		MBs = (canMBsType0	*)(CAN1_BASE + 0x80); 
	}
	else if(CANChannel == 2)
	{
		CANBaseAdd = CAN2_BASE_PTR;
		MBs = (canMBsType0	*)(CAN2_BASE + 0x80); 
	}
	else return 1;
    
    //缓冲区和数据长度设置错误
    if(iMB >= NUMBER_OF_MB || length >8)
        return 1; //超出范围
    
    //转换8个字节转换成32位的word存储
    wno = (length-1)>>2;//是否超过4字节
    bno = (length-1)%4; //
#if 0
    if(wno > 0)         //长度大于4(即发送数据超过4字节)
    {
    	word[0] = (
    			     (Data[0]<<24) 
    			   | (Data[1]<<16) 
    			   | (Data[2]<< 8) 
    			   |  Data[3]
    			 );
    }
#endif
	for(uint8 j = 0; j <= wno; j++)
    	for(i=0;i<=bno;i++)
       		word[j] |= Data[(j<<2)+i] << (24-(i<<3)); 

    ///////////////////////////////////////////////////////  
    // ID 格式
    // B31 30 29 28 27 26 ... 11 10 9 8 7 6 5 4 3 2 1 0
    // |	|	 |									  |
    // |     |    |------------------------------------|
    // |	|					|--> 29 位 ID
    // |	|
    // |    |--> RTR  1: 远程帧, 0: 数据帧
    // |
    // |-------> IDE 1 : 扩展ID, 0: 标准ID
    ///////////////////////////////////////////////////////
    
    //通过id判断帧类型——扩展帧
   /// wno = (id &  CAN_MSG_IDE_MASK)>>CAN_MSG_IDE_BIT_NO;  //IDE
    //通过id判断帧类型——远程帧
   // bno = (id &  CAN_MSG_TYPE_MASK)>>CAN_MSG_TYPE_BIT_NO;//RTR
    
    //获得ID位数
    //i =  wno? 0: FLEXCAN_MB_ID_STD_BIT_NO;
    
    //以下四步骤为发送过程
    MBs[iMB].CS = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_INACTIVE)//缓冲区写激活代码
							| (1<<FLEXCAN_MB_CS_IDE_BIT_NO)//缓冲区写IDE位
							| (0<<FLEXCAN_MB_CS_RTR_BIT_NO)//缓冲区写RTR位
                            | FLEXCAN_MB_CS_LENGTH(length);  //缓冲区写数据长度
    
    //缓冲区写ID
    MBs[iMB].ID = (1 << FLEXCAN_MB_ID_PRIO_BIT_NO) 
							| ((id & ~(CAN_MSG_IDE_MASK|CAN_MSG_TYPE_MASK))<<0);  
    
    //缓冲区写内容
    MBs[iMB].Word[0] = word[0];
    MBs[iMB].Word[1] = word[1];  
    
    //延迟
    for(i = 0;i < 100;i++);
    
    //通过制定的发送代码开始发送
    MBs[iMB].CS = (MBs[iMB].CS & ~(FLEXCAN_MB_CS_CODE_MASK)) 
							| FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_ONCE)//写激活代码
							| FLEXCAN_MB_CS_LENGTH(length);//缓冲区写数据长度 
    
    //限时等待发送完成（如果使用中断则限时等待语句可删除）
    i=0;
    while(!(CANBaseAdd->IFLAG1 & (1<<iMB)))
    {
    	if((i++)>0x1000)
    		return 1;
    }
    //清报文缓冲区中断标志
    CANBaseAdd->IFLAG1 = (1<<iMB);
    return 0;
}


//============================================================================
//函数名称：CANRecData
//函数返回：0：成功，1：失败
//参数说明：CANChannel：CAN模块号
//          iMB：缓冲区号
//			id: ID号
//			lenght：数据长度
//			Data[8]: 和接收数据缓冲区
//功能概要：接收数据
//============================================================================
uint8 CANRecData(uint8 CANChannel,uint16 iMB, uint32 *id,uint8 *Datalenght,uint8 Data[])
{
    int8   i,wno,bno;
    uint16 code;
    uint8  *pMBData;
    int16  length;
    //int8   format;
    uint8 *pBytes = Data;
   // CAN_MemMapPtr CANBaseAdd;
    canMBsType0	*MBs;
	
    if(CANChannel == 0)
	{
		//CANBaseAdd = CAN0_BASE_PTR;
		MBs = (canMBsType0	*)(CAN0_BASE + 0x80); 
	}        
    else if(CANChannel == 1)
	{
		//CANBaseAdd = CAN1_BASE_PTR;
		MBs = (canMBsType0	*)(CAN1_BASE + 0x80); 
	}
	else if(CANChannel == 2)
	{
		//CANBaseAdd = CAN2_BASE_PTR;
		MBs = (canMBsType0	*)(CAN2_BASE + 0x80); 
	}
	else return 1;
    
    // 锁定MB
    code = FLEXCAN_get_code(MBs[iMB].CS);
    
    if(code != 0x02)//缓冲区没有接收到数据，返回错误
    {
        *Datalenght = 0;
        return 1;
    }
    length = FLEXCAN_get_length(MBs[iMB].CS);
    
    if(length <1)//接收到的数据长度小于1，返回错误
    {
        *Datalenght = 0;
        return 1;
    }
   
    //判断是标准帧还是扩展帧
   // format = (MBs[iMB].CS & FLEXCAN_MB_CS_IDE)? 1:0;
    *id = (MBs[iMB].ID & FLEXCAN_MB_ID_EXT_MASK);
	
#if 0

    if(0)//if(!format)
    {
        *id >>= FLEXCAN_MB_ID_STD_BIT_NO; // 获得标准帧号
    }
    else
    { 
        *id |= CAN_MSG_IDE_MASK; //标记扩展的ID        
    }

    format = (MBs[iMB].CS & FLEXCAN_MB_CS_RTR)? 1:0;  
    if(format)
    {
        *id |= CAN_MSG_TYPE_MASK; //标记为远程帧类型       
    }
#endif
    //读取报文数据
    wno = (length-1)>>2;
    bno = length-1;
    if(wno>0)
    {  
        (*(uint32*)pBytes) = MBs[iMB].Word[0];
        swap_4bytes(pBytes);
        bno -= 4;
        pMBData = (uint8*)&MBs[iMB].Word[1]+3;
    }
    else
    {
        pMBData = (uint8*)&MBs[iMB].Word[0]+3;
    }
    
    for(i=0; i <= bno; i++)
        pBytes[i+(wno<<2)] = *pMBData--;	

    *Datalenght = length;
     return 0;

}
