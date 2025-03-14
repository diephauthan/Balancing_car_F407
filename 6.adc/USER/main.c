#include "main.h"

// Declare the global variable TimingDelay
volatile uint32_t TimingDelay;

void TimingDelay_Decrement(void)
{
    if (TimingDelay > 0)
    {
        TimingDelay--;
    }
}

char mess[] = "Hello MCU ";

int main(void)
{	
		bsp_init(); // System initialization
		

		while(1)
		{
				LED = !LED;
	
				UART_SendString(USART1, mess);
				delay_ms(500);
		}
}

