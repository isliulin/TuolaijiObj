/*
 * File:		io.c
 * Purpose:		Serial Input/Output routines
 *
 * Notes:       TERMINAL_PORT defined in <board>.h
 */

#include "common.h"
#include "uart.h"

#if (defined(BACES_X_K80FN256) || defined(EVB_S32K144))

/********************************************************************/
char
in_char (void)
{
	return lpuart_getchar(TERM_PORT);
}
/********************************************************************/
void
out_char (char ch)
{
	lpuart_putchar(TERM_PORT, ch);
}
/********************************************************************/
int
char_present (void)
{
	return lpuart_getchar_present(TERM_PORT);
}
/********************************************************************/

#else
/********************************************************************/
char
in_char (void)
{
	return uart_getchar(TERM_PORT);
}
/********************************************************************/
void
out_char (char ch)
{
	uart_putchar(TERM_PORT, ch);
}
/********************************************************************/
int
char_present (void)
{
	return uart_getchar_present(TERM_PORT);
}
/********************************************************************/
#endif
