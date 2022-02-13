#include <stdint.h>
#include "GPIO_Definitions.h"
#include "UART_Definitions.h"
#include "../TestModule/testing.h"
#include "myprint.h"
#include "stm32mp1xx.h"

unsigned char * str = "Hello World!\n";

int value[5];

void main()
{
	print_char('\n');
	print_string(str, 13); 
	
	value[1] = 5;
	value[1]++;
	
	RCC->MP_GCR = RCC_MP_GCR_BOOT_MCU;
	
	while(1);
}




