
#include "ap_S32K1xx_FTM.h"

/**************** PWM 部分*********************************/
/************  lyp   2016.1.12  ******************************/

void FTM_PWM_init(FTMn ftmn, CHn ch, uint32_t freq,uint32_t duty)
{
	uint32_t clk_hz = (80000 * 1000) >> 1;       //bus频率/2
    uint16_t mod;
    uint8_t sc_ps;
    uint16_t cv;
	//uint32_t min_val = 0xFFFF;
	
    /*       计算频率设置        */
	
    mod = (clk_hz >> 16 ) / freq ;
    for(sc_ps = 0; (mod >> sc_ps) >= 1; sc_ps++);
   if(freq < 1000)	
	  sc_ps++;
    mod = (clk_hz >> sc_ps) / freq;
	/*
	mod = (clk_hz / freq ) /  FTM_MOD_MOD_MASK;
	if((clk_hz/freq)/mod > FTM_MOD_MOD_MASK)
    {
        mod++;
    }
	 for(UINT8 i = 0; i < 7; i++)
    {
        if((ABS(mod - (1<<i))) < min_val)
        {
             sc_ps = i;
            min_val = ABS(mod - (1<<i));
        }
    }
	if(mod > (1<<sc_ps)) sc_ps++;
    if(sc_ps > 7) sc_ps = 7;
	mod = ((clk_hz/(1<<sc_ps))/freq) - 1;*/
    cv = (duty * (mod - 0 + 1)) / FTM_PRECISON;
	switch(ftmn)
	{
		case ftm0:
            PCC->FLEXTMR0 = PCC_FLEXTMR0_CGC(1)|PCC_FLEXTMR0_PCS(6);//FTM时钟
			//复用通道默认选择ALT2
			switch(ch)
			{
				case CH0:		//PD15		METER6_3	
					PCC_PORTD = PCC_PORTD_CGC(1);
					PORTD_PCR15 = PORT_PCR_MUX(0x2); //
					/******************** 选择输出模式为 边沿对齐PWM *******************/
					FTM0_C0SC &= ~FTM_C0SC_ELSA_MASK;
					FTM0_C0SC = FTM_C0SC_MSB_MASK | FTM_C0SC_ELSB_MASK;
					FTM0_SC |= FTM_SC_PWMEN0(1);
					break;
				case CH1:	//PD16		METER6_4	
					PCC_PORTD = PCC_PORTD_CGC(1);
					PORTD_PCR16 = PORT_PCR_MUX(0x2); //
					/******************** 选择输出模式为 边沿对齐PWM *******************/
					FTM0_C1SC &= ~FTM_C1SC_ELSA_MASK;
					FTM0_C1SC = FTM_C1SC_MSB_MASK | FTM_C1SC_ELSB_MASK;
					FTM0_SC |= FTM_SC_PWMEN1(1);
					break;
				case CH2:	//PB14		METER1_4	
					PCC_PORTB = PCC_PORTD_CGC(1);
					PORTB_PCR14 = PORT_PCR_MUX(0x2); //
					/******************** 选择输出模式为 边沿对齐PWM *******************/
					FTM0_C2SC &= ~FTM_C2SC_ELSA_MASK;
					FTM0_C2SC = FTM_C2SC_MSB_MASK | FTM_C2SC_ELSB_MASK;
					FTM0_SC |= FTM_SC_PWMEN2(1);
					break;
				case CH3:	//PB15	METER1_3
					PCC_PORTB = PCC_PORTB_CGC(1);
					PORTB->PCR15 = PORT_PCR_MUX(0x2); // 
						/******************** 选择输出模式为 边沿对齐PWM *******************/
					FTM0_C3SC &= ~FTM_C3SC_ELSA_MASK;
					FTM0_C3SC = FTM_C3SC_MSB_MASK | FTM_C3SC_ELSB_MASK;
					FTM0_SC |= FTM_SC_PWMEN3(1);
					break;
				case CH4:	//PB16 	METER1_2
					PCC_PORTB = PCC_PORTB_CGC(1);
					PORTB->PCR16 = PORT_PCR_MUX(0x2); // 
						/******************** 选择输出模式为 边沿对齐PWM *******************/
					FTM0_C4SC &= ~FTM_C4SC_ELSA_MASK;
					FTM0_C4SC = FTM_C4SC_MSB_MASK | FTM_C4SC_ELSB_MASK;
					FTM0_SC |= FTM_SC_PWMEN4(1);
					break;
				case CH5:	//PB17	METER1_1
					PCC_PORTB = PCC_PORTB_CGC(1);
					PORTB->PCR17 = PORT_PCR_MUX(0x2); // 
						/******************** 选择输出模式为 边沿对齐PWM *******************/
					FTM0_C5SC &= ~FTM_C0SC_ELSA_MASK;
					FTM0_C5SC = FTM_C0SC_MSB_MASK | FTM_C0SC_ELSB_MASK;
					FTM0_SC |= FTM_SC_PWMEN5(1);
					break;
				case CH6:	//PE8	METER6_1
					PCC_PORTE = PCC_PORTE_CGC(1);
					PORTE->PCR8 = PORT_PCR_MUX(0x2); // 
						/******************** 选择输出模式为 边沿对齐PWM *******************/
					FTM0_C6SC &= ~FTM_C0SC_ELSA_MASK;
					FTM0_C6SC = FTM_C0SC_MSB_MASK | FTM_C0SC_ELSB_MASK;
					FTM0_SC |= FTM_SC_PWMEN6(1);
					break;
				case CH7:	//PE9	METER6_2
					PCC_PORTE = PCC_PORTE_CGC(1);
					PORTE_PCR9 = PORT_PCR_MUX(0x2); //
					/******************** 选择输出模式为 边沿对齐PWM *******************/
					FTM0_C7SC &= ~FTM_C7SC_ELSA_MASK;
					FTM0_C7SC = FTM_C7SC_MSB_MASK | FTM_C7SC_ELSB_MASK;
					FTM0_SC |= FTM_SC_PWMEN7(1);
					break;
				default: break;
					
			}
			FTM0_SC |=  FTM_SC_CPWMS_MASK | FTM_SC_PS(sc_ps) | FTM_SC_CLKS(1);  //没有使能溢出中断
			FTM0_CNTIN = 0;  // Channel (n) Value  。设置脉冲宽度：(CnV - CNTIN).
			FTM0_MOD = mod;  //Modulo value模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
			if(ch == CH0)	FTM0_C0V = cv; //Counter Initial Value 计数器初始化值
			else if(ch == CH1)	FTM0_C1V = cv;
			else if(ch == CH2)	FTM0_C2V = cv;
			else if(ch == CH3)	FTM0_C3V = cv;
			else if(ch == CH4)	FTM0_C4V = cv;
			else if(ch == CH5)	FTM0_C5V = cv;
			else if(ch == CH6)	FTM0_C6V = cv;
			else if(ch == CH7)	FTM0_C7V = cv;
			FTM0_CNT = 0;//计数器。只有低16位可用
			break;
		
		case ftm1:
			PCC_FLEXTMR1 = PCC_FLEXTMR1_CGC(1)|PCC_FLEXTMR1_PCS(6);//FTM时钟
			//复用通道默认选择ALT2
			switch(ch)
			{
				case CH0:
					break;
				case CH1:	
					FTM1_C1SC &= ~FTM_C1SC_ELSA_MASK;
					FTM1_C1SC = FTM_C1SC_MSB_MASK | FTM_C1SC_ELSB_MASK;
					FTM1_SC |= FTM_SC_PWMEN1(1);
					break;
				case CH2:// PA15 (LCD-PWM)
					//PCC_PORTA = PCC_PORTA_CGC(1);
					//PORTA_PCR15 = PORT_PCR15_MUX(0x2); // 
				
					//PCC_PORTC = PCC_PORTC_CGC(1);
					//PORTC_PCR14 = PORT_PCR14_MUX(0x2); // 
						/******************** 选择输出模式为 边沿对齐PWM *******************/
					FTM1_C2SC &= ~FTM_C2SC_ELSA_MASK;
					FTM1_C2SC = FTM_C2SC_MSB_MASK | FTM_C2SC_ELSB_MASK;
					FTM1_SC |= FTM_SC_PWMEN2(1);
					break;
				case CH3: //PC15 		music_Pow
					//PCC_PORTC = PCC_PORTC_CGC(1);
					//PORTC_PCR15 = PORT_PCR15_MUX(0x2); // 
						/******************** 选择输出模式为 边沿对齐PWM *******************/
					FTM1_C3SC &= ~FTM_C3SC_ELSA_MASK;
					FTM1_C3SC = FTM_C3SC_MSB_MASK | FTM_C3SC_ELSB_MASK;
					FTM1_SC |= FTM_SC_PWMEN3(1);
					break;
				case CH4://Bell -02版	
					PCC_PORTD = PCC_PORTD_CGC(1);
					PORTD_PCR8 = PORT_PCR8_MUX(0x06); // 
					
					FTM1_C4SC &= ~FTM_C4SC_ELSA_MASK;
					FTM1_C4SC = FTM_C4SC_MSB_MASK | FTM_C4SC_ELSB_MASK;
					FTM1_SC |= FTM_SC_PWMEN4(1);
					break;
				case CH5: //PA11 (LED5)
					
						/******************** 选择输出模式为 边沿对齐PWM *******************/
					FTM1_C5SC &= ~FTM_C5SC_ELSA_MASK;
					FTM1_C5SC = FTM_C5SC_MSB_MASK | FTM_C5SC_ELSB_MASK;
					FTM1_SC |= FTM_SC_PWMEN5(1);
					break;
				case CH6:	
					
					break;
				case CH7:	//PC1 LEC_PWM
					PCC_PORTC = PCC_PORTC_CGC(1);
					PORTC_PCR1 = PORT_PCR1_MUX(0x6); // 
						/******************** 选择输出模式为 边沿对齐PWM *******************/
					FTM1_C7SC &= ~FTM_C7SC_ELSA_MASK;
					FTM1_C7SC = FTM_C7SC_MSB_MASK | FTM_C7SC_ELSB_MASK;
					FTM1_SC |= FTM_SC_PWMEN7(1);
					break;
					
				default: break;
					
			}
			FTM1_SC |=  FTM_SC_CPWMS_MASK | FTM_SC_PS(sc_ps) | FTM_SC_CLKS(1);  //没有使能溢出中断
			FTM1_CNTIN = 0;  // Channel (n) Value  。设置脉冲宽度：(CnV - CNTIN).
			FTM1_MOD = mod;  //Modulo value模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
			if(ch == CH0)	FTM1_C0V = cv; //Counter Initial Value 计数器初始化值
			else if(ch == CH1)	FTM1_C1V = cv;
			else if(ch == CH2)	FTM1_C2V = cv;
			else if(ch == CH3)	FTM1_C3V = cv;
			else if(ch == CH4)	FTM1_C4V = cv;
			else if(ch == CH5)	FTM1_C5V = cv;
			else if(ch == CH6)	FTM1_C6V = cv;
			else if(ch == CH7)	FTM1_C7V = cv;
			//FTM1_CNT = 0;//计数器。只有低16位可用
			break;
		case ftm2://PWM2
			PCC_FLEXTMR2 = PCC_FLEXTMR2_CGC(1)|PCC_FLEXTMR2_PCS(6);//FTM时钟
			//复用通道默认选择ALT2
			switch(ch)
			{
				case CH0:	
					break;
				case CH1:	
					break;
					
				case CH2:	
					FTM2_C2SC &= ~FTM_C2SC_ELSA_MASK;
					FTM2_C2SC = FTM_C2SC_MSB_MASK | FTM_C2SC_ELSB_MASK;
					FTM2_SC |= FTM_SC_PWMEN2(1);
					break;
				case CH3:
						/******************** 选择输出模式为 边沿对齐PWM *******************/
					FTM2_C3SC &= ~FTM_C3SC_ELSA_MASK;
					FTM2_C3SC = FTM_C3SC_MSB_MASK | FTM_C3SC_ELSB_MASK;
					FTM2_SC |= FTM_SC_PWMEN3(1);
					break;
					
				case CH4:	//PWM2
					//PCC_PORTE = PCC_PORTE_CGC(1);
					//PORTE_PCR10 = PORT_PCR_MUX(0x4); // 
						/******************** 选择输出模式为 边沿对齐PWM *******************/
					FTM2_C4SC &= ~FTM_C4SC_ELSA_MASK;
					FTM2_C4SC = FTM_C4SC_MSB_MASK | FTM_C4SC_ELSB_MASK;
					FTM2_SC |= FTM_SC_PWMEN4(1);
					break;
				case CH5:	//PD14
					FTM2_C5SC &= ~FTM_C5SC_ELSA_MASK;
					FTM2_C5SC = FTM_C5SC_MSB_MASK | FTM_C5SC_ELSB_MASK;
					FTM2_SC |= FTM_SC_PWMEN5(1);
					break;
				case CH6:	//PC12
					break;
				case CH7:	//PC13
					
					break;
				default: break;
					
			}
		       FTM2_SC &= ~FTM_SC_PS_MASK;
			FTM2_SC |=  FTM_SC_CPWMS_MASK | FTM_SC_PS(sc_ps) | FTM_SC_CLKS(1);  //没有使能溢出中断

			
			//FTM2_SC |=  FTM_SC_CPWMS_MASK | FTM_SC_PS(sc_ps) | FTM_SC_CLKS(1);  //没有使能溢出中断
			FTM2_CNTIN = 0;  // Channel (n) Value  。设置脉冲宽度：(CnV - CNTIN).
			FTM2_MOD = mod;  //Modulo value模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
			FTM2_PWMLOAD= 0xFFFFFFFF;
			if(ch == CH0)	FTM2_C0V = cv; //Counter Initial Value 计数器初始化值
			else if(ch == CH1)	FTM2_C1V = cv;
			else if(ch == CH2)	FTM2_C2V = cv;
			else if(ch == CH3)	FTM2_C3V = cv;
			else if(ch == CH4)	FTM2_C4V = cv;
			else if(ch == CH5)	FTM2_C5V = cv;
			else if(ch == CH6)	FTM2_C6V = cv;
			else if(ch == CH7)	FTM2_C7V = cv;
			//FTM2_CNT = 0;//计数器。只有低16位可用
			break;
		case ftm3:
			PCC_FLEXTMR3 = PCC_FLEXTMR3_CGC(1)|PCC_FLEXTMR3_PCS(6);//FTM时钟
			//复用通道默认选择ALT2
			switch(ch)
			{
				case CH0:	
					break;
				case CH1:	
					break;
				case CH2:
					break;
				case CH3:	
					break;
				case CH4:
					
					break;
				case CH5:	//PC11 PWM1
					//PCC_PORTC = PCC_PORTC_CGC(1);
					//PORTC_PCR11 = PORT_PCR_MUX(0x2); // 
						/******************** 选择输出模式为 边沿对齐PWM *******************/
					FTM3_C5SC &= ~FTM_C5SC_ELSA_MASK;
					FTM3_C5SC = FTM_C5SC_MSB_MASK | FTM_C5SC_ELSB_MASK;
					FTM3_SC |= FTM_SC_PWMEN5(1);
					
					break;
				case CH6:	
					break;
				case CH7:	
					break;
				default: break;
					
			}
			FTM3_SC &= ~FTM_SC_PS_MASK;
			FTM3_SC |=  FTM_SC_CPWMS_MASK | FTM_SC_PS(sc_ps) | FTM_SC_CLKS(1);  //没有使能溢出中断
			FTM3_CNTIN = 0;  // Channel (n) Value  。设置脉冲宽度：(CnV - CNTIN).
			FTM3_MOD = mod;  //Modulo value模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
			FTM3_PWMLOAD= 0xFFFFFFFF;
			if(ch == CH0)	FTM3_C0V = cv; //Counter Initial Value 计数器初始化值
			else if(ch == CH1)	FTM3_C1V = cv;
			else if(ch == CH2)	FTM3_C2V = cv;
			else if(ch == CH3)	FTM3_C3V = cv;
			else if(ch == CH4)	FTM3_C4V = cv;
			else if(ch == CH5)	FTM3_C5V = cv;
			else if(ch == CH6)	FTM3_C6V = cv;
			else if(ch == CH7)	FTM3_C7V = cv;
			//FTM3_CNT = 0;//计数器。只有低16位可用
			break;
		default:
			break;
	}
}

//用法FTMx_CnV =  FTM_PWM_Duty( freq, duty);
uint32_t FTM_PWM_Duty(uint32_t freq,uint32_t duty)
{
	uint32_t mod,cv;
	uint32_t clk_hz = (80000 * 1000) >> 1;       //bus频率/2
	uint8_t sc_ps;
	
	mod = (clk_hz >> 16 ) / freq ;
	for(sc_ps = 0; (mod >> sc_ps) >= 1; sc_ps++);
 	if(freq < 1000)	sc_ps++;
	mod = (clk_hz >> sc_ps) / freq;

	mod = (clk_hz >> 16 ) / freq ;
    for(sc_ps = 0; (mod >> sc_ps) >= 1; sc_ps++);
   if(freq < 1000)	
	  sc_ps++;
    mod = (clk_hz >> sc_ps) / freq;
	cv = (duty * (mod - 0 + 1)) / FTM_PRECISON;	
	return cv;
}
	
void FTM_PWM_freq(FTMn ftmn, uint32_t freq)             //设置FTM的频率
{
	uint32_t clk_hz = (80000 * 1000) >> 1;       //bus频率/2
    uint16_t mod;
    uint8_t sc_ps;
	FTM_Type * ftm;

	if(ftmn == ftm0)	{
		ftm = FTM0;
	}else if(ftmn == ftm1){
		ftm = FTM1;
	}else if(ftmn == ftm2){
		ftm = FTM2;
	}else if(ftmn == ftm3){
		ftm = FTM3;
	}else 	return;
       /*       计算频率设置        */
    mod = (clk_hz >> 16 ) / freq ;
    for(sc_ps = 0; (mod >> sc_ps) >= 1; sc_ps++);
	
    mod = (clk_hz >> sc_ps) / freq;


	ftm->SC &= ~FTM_SC_PS_MASK;
	ftm->SC |=  FTM_SC_CPWMS_MASK | FTM_SC_PS(sc_ps) | FTM_SC_CLKS(1);
	ftm->CNTIN = 0;
	ftm->MOD = mod;  //Modulo value模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
	ftm->CNTIN = 0;
	//ftm->CNT = 0;
}

uint8 LPLD_FTM_IC_Init(FTM_Type *ftmx ,uint8 ps)
{
  
 if(ftmx == FTM0)
  {
     PCC_FLEXTMR0 = PCC_FLEXTMR0_CGC(1)|PCC_FLEXTMR0_PCS(6);//FTM时钟
  }
  else if(ftmx == FTM1)
  {
   PCC_FLEXTMR1 = PCC_FLEXTMR1_CGC(1)|PCC_FLEXTMR1_PCS(6);//FTM时钟
  }
  else if(ftmx == FTM2)
  {
    PCC_FLEXTMR2 = PCC_FLEXTMR2_CGC(1)|PCC_FLEXTMR2_PCS(6);//FTM时钟
  }
  else if(ftmx == FTM3)
  {
    PCC_FLEXTMR3 = PCC_FLEXTMR3_CGC(1)|PCC_FLEXTMR3_PCS(6);//FTM时钟
  }
  
  
  ftmx->CONF=FTM_CONF_BDMMODE(0x3);
  
  ftmx->SC = 0;
  
  ftmx->CNT = 0;
  ftmx->CNTIN = 0;
  ftmx->MOD = 0xffff;                           
  ftmx->QDCTRL = (~FTM_QDCTRL_QUADEN_MASK); //关闭正交解码
  ftmx->FILTER = 0x00;                      //关过虑器
  
  // 配置FTM控制寄存器
  // 将FTM Counter配置成Free Counter
  // 禁用中断, 加计数模式, 时钟源:System clock（Bus Clk）, 分频系数:PS
  // 假设SysClk = 50MHz, SC_PS=3, FTM Clk = 50MHz/2^3 = 6.25MHz
  ftmx->SC |= FTM_SC_CLKS(1)|FTM_SC_PS(ps);
  ftmx->SC |= FTM_SC_TOIE_MASK;             //使能计数溢出中断
  ftmx->SC &= (~FTM_SC_CPWMS_MASK);         //FTM加计数
  
  return 1;
}

uint8 LPLD_FTM_IC_Enable(FTM_Type *ftmx, uint8 chn, uint8 capture_edge)
{
  //if(!LPLD_FTM_PinInit(ftmx, chn, pin))
   // return 0;
  uint32_t * CnSC_addr;
  switch(chn)
  {
     case  0:  CnSC_addr = &ftmx->C0SC;break;
     case  1:  CnSC_addr = &ftmx->C1SC;break;
     case  2:  CnSC_addr = &ftmx->C2SC;break;
     case  3:  CnSC_addr = &ftmx->C3SC;break;
     case  4:  CnSC_addr = &ftmx->C4SC;break;
     case  5:  CnSC_addr = &ftmx->C5SC;break;
     case  6:  CnSC_addr = &ftmx->C6SC;break;
     case  7:  CnSC_addr = &ftmx->C7SC;break;
     default:CnSC_addr = &ftmx->C0SC;break;
  }
  
  *CnSC_addr = 0x00;
  
  *CnSC_addr |= capture_edge;        
  
  *CnSC_addr &= (~FTM_C0SC_CHF_MASK);
  *CnSC_addr |= FTM_C0SC_CHIE_MASK;         //使能通道捕获输入中断
  
  *CnSC_addr &= (~FTM_C0SC_MSB_MASK);
  *CnSC_addr &= (~FTM_C0SC_MSA_MASK);       //配置成Input capture模式
  
  *CnSC_addr &= (~FTM_C0SC_DMA_MASK);       //关闭DMA
  
  return 1;
}


