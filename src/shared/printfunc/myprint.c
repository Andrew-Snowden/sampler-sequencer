#include "myprint.h"
#include "UART_Definitions.h"

void print_char(unsigned char character)
{
	uart_wait();
	UART4_TDR = character;
	
	if (character == '\n')
	{
		uart_wait();
		UART4_TDR = '\r';
	}
}

void print_char_nl(unsigned char character)
{
	uart_wait();
	UART4_TDR = character;
	uart_wait();
	UART4_TDR = '\n';
	uart_wait();
	UART4_TDR = '\r';
}

void print_string(unsigned char * string, unsigned int length)
{
	uint8_t newline = 0;
	for (unsigned int i = 0; i < length; i++)
	{
		if (*(string + i) == '\n') newline = 1;
		uart_wait();
		UART4_TDR = *(string + i) & TDR_TDR_MASK;
	}
	if (newline)
	{
		uart_wait();
		UART4_TDR = '\r';
	}
}

void uart_wait()
{
	while((UART4_ISR & ISR_TXFT_MASK) == 0);
}
