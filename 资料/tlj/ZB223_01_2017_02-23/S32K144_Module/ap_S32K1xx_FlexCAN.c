//******************************************************************************
// author��        ap
// date:           2016/1/14
//******************************************************************************

#include "ap_S32K1xx_FlexCAN.h"

uint8 SetCANBand(uint8 CANChannel,uint32 baudrateKHz);

//============================================================================
//�������ƣ�CANInit
//�������أ�0���ɹ���1��ʧ��
//����˵����
//         CANChannel��ģ���
//		   baudrateKHz: ������
//         selfLoop: ģʽѡ��(1=�ػ�ģʽ��0=����ģʽ)
//         idMask: ID����(1=ID���ˣ�0=ID������)
//���ܸ�Ҫ��CAN��ʼ��
//============================================================================
uint8 canInit(uint8 canChannel,uint32 baudrateKHz,uint8 selfLoop,uint8 idMask)
{
	int8 i;
	CAN_Type *canBaseAddress;

	if(canChannel == 0)		
	{
		canBaseAddress = CAN0;
		PCC_FLEXCAN0 |= PCC_FLEXCAN0_CGC_MASK;	//ʱ��
		//����
		PORTE->PCR4 = PORT_PCR_MUX(5) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //Rx ����
		PORTE->PCR5 = PORT_PCR_MUX(5) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //Tx ����
	}
	else if(canChannel == 1)
	{
		canBaseAddress = CAN1;
		PCC_FLEXCAN1 |= PCC_FLEXCAN1_CGC_MASK;
		PORTA_PCR12 = PORT_PCR12_MUX(3) | PORT_PCR12_PE_MASK | PORT_PCR12_PS_MASK; //Rx ����
		PORTA_PCR13 = PORT_PCR13_MUX(3) | PORT_PCR13_PE_MASK | PORT_PCR13_PS_MASK; //Tx ����
	}
	else if(canChannel == 2)	
	{
		canBaseAddress = CAN2;
		PCC_FLEXCAN2 &= ~PCC_FLEXCAN2_CGC_MASK;
		PCC_FLEXCAN2 |= PCC_FLEXCAN2_CGC_MASK;
		PORTB_PCR12 = PORT_PCR12_MUX(4) | PORT_PCR12_PE_MASK | PORT_PCR12_PS_MASK; //Rx ����
		PORTB_PCR13 = PORT_PCR13_MUX(4) | PORT_PCR13_PE_MASK | PORT_PCR13_PS_MASK; //Tx ����
	}

	//ѡ��ʱ��Դ
	canBaseAddress->CTRL1 |=  CAN_CTRL1_CLKSRC_MASK; //The CAN engine clock source is the peripheral clock.
	canBaseAddress->MCR |= CAN_MCR_FRZ_MASK;  //ʹ�ܶ���ģʽ   
    canBaseAddress->MCR &= ~CAN_MCR_MDIS_MASK;//ʹ��CANģ��
     //ȷ�����˳�����ģʽ
    while((CAN_MCR_LPMACK_MASK & canBaseAddress->MCR));
	 //�����λ
    canBaseAddress->MCR ^= CAN_MCR_SOFTRST_MASK; 
    //�ȴ���λ���
    while(CAN_MCR_SOFTRST_MASK & canBaseAddress->MCR);   
    //�ȴ����붳��ģʽ 
    while(!(CAN_MCR_FRZACK_MASK & canBaseAddress->MCR));
	CAN_MCR_REG(canBaseAddress) |= CAN_MCR_SRXDIS_MASK;//��ֹ�Խ���
	for(i = 0; i < 64; i++)
	{
		canBaseAddress->RAMn[i]  = 0;
	}
	
	//�����������IDE�Ƚϣ�RTR���Ƚ�
    canBaseAddress->CTRL2 &= ~CAN_CTRL2_EACEN_MASK;
    //Զ������֡����
    canBaseAddress->CTRL2 &= ~CAN_CTRL2_RRS_MASK;
    //�������ȴӽ���FIFO����ƥ��Ȼ������������ƥ��
    canBaseAddress->CTRL2 &= ~CAN_CTRL2_MRP_MASK;
 
    //ʹ��һ��32λ������
    canBaseAddress->MCR |= (canBaseAddress->MCR & ~CAN_MCR_IDAM_MASK) | CAN_MCR_IDAM(0);
    //���ò�����
    if(SetCANBand(canChannel,baudrateKHz) == 1)	return 1;//�����ô���
        
    //ģʽѡ�񣺻ػ�ģʽ������ģʽ
    if(1==selfLoop)
        canBaseAddress->CTRL1 |= CAN_CTRL1_LPB_MASK;//ʹ�ûػ�ģʽ
	//��ʼ������Ĵ���
    if(1==idMask)//����ID
    {
    	canBaseAddress->RXMGMASK = 0x1FFFFFFF;
		canBaseAddress->RX14MASK = 0x1FFFFFFF;
		canBaseAddress->RX15MASK = 0x1FFFFFFF;
    }
    else//������ID
    {
    	canBaseAddress->RXMGMASK = 0x0;
		canBaseAddress->RX14MASK = 0x0;
		canBaseAddress->RX15MASK = 0x0;
    }

    //����������빦��ʹ�ܣ�Ϊÿ�����г�ʼ������������Ĵ���
    if(canBaseAddress->MCR & CAN_MCR_IRMQ_MASK)
    {
        for(i = 0; i < NUMBER_OF_MB ; i++)
        {        
            canBaseAddress->RXIMR[i] = 0x1FFFFFFFL;
        }
    }
    
    //ֻ���ڶ���ģʽ�²������ã��������Ƴ�����ģʽ
	canBaseAddress->MCR &= ~(CAN_MCR_HALT_MASK);
	//�ȴ�ֱ���˳�����ģʽ
	while( canBaseAddress->MCR & CAN_MCR_FRZACK_MASK);    
	//�ȵ����ڶ���ģʽ������ģʽ����ֹͣģʽ
	while((canBaseAddress->MCR & CAN_MCR_NOTRDY_MASK));
    
    return 0;
}

//===========================�ڲ�����=========================================
//============================================================================
//�������ƣ�SetCANBand
//�������أ�0:���óɹ� 1������ʧ��
//����˵������
//���ܸ�Ҫ������CAN�Ĳ�����
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
//�������ƣ�CANEnableRXBuff
//�������أ���
//����˵���� CANChannel��CANģ���
//          iMB����������
//          id��id��
//���ܸ�Ҫ��ʹ�ܽ��ջ�����
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
     

    //��MB����Ϊ�Ǽ���״̬
   MBs0[iMB].CS = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_RX_INACTIVE); 	
    
    //ȡ��29λ������ID
    //id2 = id & ~(CAN_MSG_IDE_MASK | CAN_MSG_TYPE_MASK);
    //if(1)//if(id & CAN_MSG_IDE_MASK)//��չ֡
   // {
    //    MBs0[iMB].ID = id2 ;
        cs |= FLEXCAN_MB_CS_IDE;//��λIDEλ
   /// }
   // else//��׼֡
   // {
   //     MBs0[iMB].ID = id2 << FLEXCAN_MB_ID_STD_BIT_NO;          
    //}
    
    //������ջ�������codeд0100
    MBs0[iMB].CS = cs;      
}

//============================================================================
//�������ƣ�EnableCANInterrupt
//�������أ���
//����˵����
//          CANChannel:CANģ���
//          iMB:��������
//���ܸ�Ҫ��ʹ�ܻ��������պͷ����ж�
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
//�������ƣ�DisableCANInterrupt
//�������أ���
//����˵����
//         CANChannel:CANģ���
//         iMB:��������
//���ܸ�Ҫ����ֹ���������պͷ����ж�
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
//�������ƣ�CANClearFlag
//�������أ���
//����˵����
//         CANChannel:CANģ���
//         iMB:��������
//���ܸ�Ҫ���建�����жϱ�־
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
//�������ƣ�CANGetFlag
//�������أ���
//����˵����
//         CANChannel:CANģ���
//         iMB:��������
//���ܸ�Ҫ����û������жϱ�־
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
//�������ƣ�CANSendData
//�������أ�0���ɹ���1��ʧ��
//����˵����
//         CANChannel��ģ���         
//         iMB����������
//		   id: ID��
//		   length�����ݳ���
//		   Data[8]:�������ݻ�����
//���ܸ�Ҫ����������
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
    
    //�����������ݳ������ô���
    if(iMB >= NUMBER_OF_MB || length >8)
        return 1; //������Χ
    
    //ת��8���ֽ�ת����32λ��word�洢
    wno = (length-1)>>2;//�Ƿ񳬹�4�ֽ�
    bno = (length-1)%4; //
#if 0
    if(wno > 0)         //���ȴ���4(���������ݳ���4�ֽ�)
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
    // ID ��ʽ
    // B31 30 29 28 27 26 ... 11 10 9 8 7 6 5 4 3 2 1 0
    // |	|	 |									  |
    // |     |    |------------------------------------|
    // |	|					|--> 29 λ ID
    // |	|
    // |    |--> RTR  1: Զ��֡, 0: ����֡
    // |
    // |-------> IDE 1 : ��չID, 0: ��׼ID
    ///////////////////////////////////////////////////////
    
    //ͨ��id�ж�֡���͡�����չ֡
   /// wno = (id &  CAN_MSG_IDE_MASK)>>CAN_MSG_IDE_BIT_NO;  //IDE
    //ͨ��id�ж�֡���͡���Զ��֡
   // bno = (id &  CAN_MSG_TYPE_MASK)>>CAN_MSG_TYPE_BIT_NO;//RTR
    
    //���IDλ��
    //i =  wno? 0: FLEXCAN_MB_ID_STD_BIT_NO;
    
    //�����Ĳ���Ϊ���͹���
    MBs[iMB].CS = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_INACTIVE)//������д�������
							| (1<<FLEXCAN_MB_CS_IDE_BIT_NO)//������дIDEλ
							| (0<<FLEXCAN_MB_CS_RTR_BIT_NO)//������дRTRλ
                            | FLEXCAN_MB_CS_LENGTH(length);  //������д���ݳ���
    
    //������дID
    MBs[iMB].ID = (1 << FLEXCAN_MB_ID_PRIO_BIT_NO) 
							| ((id & ~(CAN_MSG_IDE_MASK|CAN_MSG_TYPE_MASK))<<0);  
    
    //������д����
    MBs[iMB].Word[0] = word[0];
    MBs[iMB].Word[1] = word[1];  
    
    //�ӳ�
    for(i = 0;i < 100;i++);
    
    //ͨ���ƶ��ķ��ʹ��뿪ʼ����
    MBs[iMB].CS = (MBs[iMB].CS & ~(FLEXCAN_MB_CS_CODE_MASK)) 
							| FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_ONCE)//д�������
							| FLEXCAN_MB_CS_LENGTH(length);//������д���ݳ��� 
    
    //��ʱ�ȴ�������ɣ����ʹ���ж�����ʱ�ȴ�����ɾ����
    i=0;
    while(!(CANBaseAdd->IFLAG1 & (1<<iMB)))
    {
    	if((i++)>0x1000)
    		return 1;
    }
    //�屨�Ļ������жϱ�־
    CANBaseAdd->IFLAG1 = (1<<iMB);
    return 0;
}


//============================================================================
//�������ƣ�CANRecData
//�������أ�0���ɹ���1��ʧ��
//����˵����CANChannel��CANģ���
//          iMB����������
//			id: ID��
//			lenght�����ݳ���
//			Data[8]: �ͽ������ݻ�����
//���ܸ�Ҫ����������
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
    
    // ����MB
    code = FLEXCAN_get_code(MBs[iMB].CS);
    
    if(code != 0x02)//������û�н��յ����ݣ����ش���
    {
        *Datalenght = 0;
        return 1;
    }
    length = FLEXCAN_get_length(MBs[iMB].CS);
    
    if(length <1)//���յ������ݳ���С��1�����ش���
    {
        *Datalenght = 0;
        return 1;
    }
   
    //�ж��Ǳ�׼֡������չ֡
   // format = (MBs[iMB].CS & FLEXCAN_MB_CS_IDE)? 1:0;
    *id = (MBs[iMB].ID & FLEXCAN_MB_ID_EXT_MASK);
	
#if 0

    if(0)//if(!format)
    {
        *id >>= FLEXCAN_MB_ID_STD_BIT_NO; // ��ñ�׼֡��
    }
    else
    { 
        *id |= CAN_MSG_IDE_MASK; //�����չ��ID        
    }

    format = (MBs[iMB].CS & FLEXCAN_MB_CS_RTR)? 1:0;  
    if(format)
    {
        *id |= CAN_MSG_TYPE_MASK; //���ΪԶ��֡����       
    }
#endif
    //��ȡ��������
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
