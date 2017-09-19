/*
 * File:        uart.c
 * Purpose:     Provide common UART routines for serial IO
 *
 * Notes:       
 *              
 */

#include "common.h"
#include "uart.h"


   
#if (defined(BACES_X_K80FN256) || defined(EVB_S32K144))
void lpuart_init (LPUART_MemMapPtr uartch, uint32_t sysclk, uint32_t baud)
{
    static uint32_t sbr;
    static uint32_t temp;
          
      /* Make sure that the transmitter and receiver are disabled while we 
       * change settings.
       */
      LPUART_CTRL_REG(uartch) &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);

      /* Configure the uart for 8-bit mode, no parity */
      LPUART_BAUD_REG(uartch) &= ~LPUART_BAUD_M10_MASK;	
      /* We need all default settings, so entire register is cleared */
      LPUART_CTRL_REG(uartch) &= ~LPUART_CTRL_M_MASK; 
      LPUART_BAUD_REG(uartch) &= ~LPUART_BAUD_SBNS_MASK;
      LPUART_CTRL_REG(uartch) &= ~LPUART_CTRL_PE_MASK;       
      
      sbr = (uint16_t)((sysclk)/(baud * 16));
        
      /* set LPUART_BAUD_SBR field to sbr */
      temp = LPUART_BAUD_REG(uartch) & ~LPUART_BAUD_SBR_MASK;
      LPUART_BAUD_REG(uartch) = temp | LPUART_BAUD_SBR(sbr); 
  
      /* Enable receiver and transmitter */
      LPUART_CTRL_REG(uartch) |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK );       
}
/********************************************************************/
/*
 * Wait for a character to be received on the specified uart
 *
 * Parameters:
 *  channel      uart channel to read from
 *
 * Return Values:
 *  the received character
 */
char lpuart_getchar (LPUART_MemMapPtr channel)
{
      /* Wait until character has been received */
      while (!(LPUART_STAT_REG(channel) & LPUART_STAT_RDRF_MASK));
    
      /* Return the 8-bit data from the receiver */
      return LPUART_DATA_REG(channel);
}
/********************************************************************/
/*
 * Wait for space in the uart Tx FIFO and then send a character
 *
 * Parameters:
 *  channel      uart channel to send to
 *  ch			 character to send
 */ 
void lpuart_putchar (LPUART_MemMapPtr channel, char ch)
{
      /* Wait until space is available in the FIFO */
      while(!(LPUART_STAT_REG(channel) & LPUART_STAT_TDRE_MASK));
    
      /* Send the character */
      LPUART_DATA_REG(channel) = (uint8_t)ch;
    
 }
/********************************************************************/
/*
 * Check to see if a character has been received
 *
 * Parameters:
 *  channel      uart channel to check for a character
 *
 * Return values:
 *  0       No character received
 *  1       Character has been received
 */
int lpuart_getchar_present (LPUART_MemMapPtr channel)
{
    return (LPUART_STAT_REG(channel) & LPUART_STAT_RDRF_MASK);
}
/********************************************************************/

#else
/********************************************************************/
/*
 * Initialize the UART for 8N1 operation, interrupts disabled, and
 * no hardware flow-control
 *
 * NOTE: Since the UARTs are pinned out in multiple locations on most
 *       Kinetis devices, this driver does not enable UART pin functions.
 *       The desired pins should be enabled before calling this init function.
 *
 * Parameters:
 *  uartch      UART channel to initialize
 *  sysclk      UART module Clock in kHz(used to calculate baud)
 *  baud        UART baud rate
 */
void uart_init (UART_MemMapPtr uartch, int sysclk, int baud)
{
    register uint16_t sbr, brfa;
    uint8_t temp;
    
	/* Enable the clock to the selected UART */

    if(uartch == UART0_BASE_PTR)
		SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
    else    
    	if (uartch == UART1_BASE_PTR)
			SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
#if (defined(KM34Z50M_BACES) || defined(TWR_K70F120M)) 
    	else         
    		if (uartch == UART2_BASE_PTR)
    			SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
#endif        
#if (defined(BACES_K21) || defined(TWR_K53N512) || defined(BACES_ASB817))
    		else
    			if(uartch == UART3_BASE_PTR)
    				SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;
#endif
#if (defined(TWR_K70F120M))
    			else
    				if(uartch == UART4_BASE_PTR)
    					SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;
    				else
    					SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;
#endif
                                
    /* Make sure that the transmitter and receiver are disabled while we 
     * change settings.
     */
    UART_C2_REG(uartch) &= ~(UART_C2_TE_MASK
				| UART_C2_RE_MASK );

    /* Configure the UART for 8-bit mode, no parity */
    UART_C1_REG(uartch) = 0;	/* We need all default settings, so entire register is cleared */
    
    /* Calculate baud settings */
    sbr = (uint16_t)((sysclk*1000)/(baud * 16));
        
    /* Save off the current value of the UARTx_BDH except for the SBR field */
    temp = UART_BDH_REG(uartch) & ~(UART_BDH_SBR(0x1F));
    
    UART_BDH_REG(uartch) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
    UART_BDL_REG(uartch) = (uint8_t)(sbr & UART_BDL_SBR_MASK);
    
    /* Determine if a fractional divider is needed to get closer to the baud rate */
    brfa = (((sysclk*32000)/(baud * 16)) - (sbr * 32));
     
    /* Save off the current value of the UARTx_C4 register except for the BRFA field */
    temp = UART_C4_REG(uartch) & ~(UART_C4_BRFA(0x1F));
    
    UART_C4_REG(uartch) = temp |  UART_C4_BRFA(brfa);    

    /* Enable receiver and transmitter */
	UART_C2_REG(uartch) |= (UART_C2_TE_MASK
				| UART_C2_RE_MASK );
}
/********************************************************************/
/*
 * Wait for a character to be received on the specified UART
 *
 * Parameters:
 *  channel      UART channel to read from
 *
 * Return Values:
 *  the received character
 */
char uart_getchar (UART_MemMapPtr channel)
{
    /* Wait until character has been received */
    while (!(UART_S1_REG(channel) & UART_S1_RDRF_MASK));
    
    /* Return the 8-bit data from the receiver */
    return UART_D_REG(channel);
}
/********************************************************************/
/*
 * Wait for space in the UART Tx FIFO and then send a character
 *
 * Parameters:
 *  channel      UART channel to send to
 *  ch			 character to send
 */ 
void uart_putchar (UART_MemMapPtr channel, char ch)
{
	/* Wait until space is available in the FIFO */
    while(!(UART_S1_REG(channel) & UART_S1_TDRE_MASK));
    
    /* Send the character */
    UART_D_REG(channel) = (uint8_t)ch;
 }
/********************************************************************/
/*
 * Check to see if a character has been received
 *
 * Parameters:
 *  channel      UART channel to check for a character
 *
 * Return values:
 *  0       No character received
 *  1       Character has been received
 */
int uart_getchar_present (UART_MemMapPtr channel)
{
    return (UART_S1_REG(channel) & UART_S1_RDRF_MASK);
}
/********************************************************************/
#endif    