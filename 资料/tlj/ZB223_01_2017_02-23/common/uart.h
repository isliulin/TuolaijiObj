/*
 * File:		uart.h
 * Purpose:     Provide common ColdFire UART routines for polled serial IO
 *
 * Notes:
 */

#ifndef __UART_H__
#define __UART_H__

/********************************************************************/
#if (defined(EVB_S32K144)||defined(BACES_X_K80FN256))
//void uart_init (LPUART_MemMapPtr, int, int);
//char uart_getchar (LPUART_MemMapPtr);
//void uart_putchar (LPUART_MemMapPtr, char);
//int uart_getchar_present (LPUART_MemMapPtr);
char lpuart_getchar (LPUART_MemMapPtr channel);
void lpuart_putchar (LPUART_MemMapPtr channel, char ch);
int lpuart_getchar_present (LPUART_MemMapPtr channel);

void lpuart_init (LPUART_MemMapPtr uartch, uint32_t sysclk, uint32_t baud);

#else
void uart_init (UART_MemMapPtr, int, int);
char uart_getchar (UART_MemMapPtr);
void uart_putchar (UART_MemMapPtr, char);
int uart_getchar_present (UART_MemMapPtr);
#endif

/********************************************************************/
#endif /* __UART_H__ */
