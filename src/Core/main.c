#include <stdint.h>
#include "../ModuleDefinitions/GPIO_Definitions.h"
#include "../ModuleDefinitions/UART_Definitions.h"
#include "../TestModule/testing.h"

unsigned char * str = "Hello World!\n";
  
void setup();

void print_char(unsigned char character);
void print_string(unsigned char * string, unsigned int length);
inline void uart_wait();

void main()
{
	setup();

	GPIOA_BRR = BRR13_MASK;	//Open Drain - GND
	
	print_char('\n');
	print_string(str, 13); 
	
	int rand_number = random_function(5);
	
	print_char((char)(rand_number + 48));
	
	while(1);
}

void setup()
{
	//PA13 setup	
	SET( GPIOA_MODER, 	(MODER13_MASK & (0x1 << MODER13_POS)) );
	SET( GPIOA_OTYPER,	OTYPER13_MASK );
	CLR( GPIOA_OSPEEDR, (OSPEEDR13_MASK & (0x0 << OSPEEDR13_POS)) );
	CLR( GPIOA_PUPDR, 	(PUPDR13_MASK & (0x0 << PUPDR13_POS)) );
	
	GPIOA_BSRR = BSRR13_MASK;	//Make output HI-Z
}

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
