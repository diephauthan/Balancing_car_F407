#include "main.h"
#include "stm32f4xx.h"

// Declare the global variable TimingDelay
volatile uint32_t TimingDelay;

void TimingDelay_Decrement(void)
{
    if (TimingDelay > 0)
    {
        TimingDelay--;
    }
}


char data1[] ="asddas"; 

int main(void)
{	
		bsp_init(); // System initialization
		
		while(1)
		{
				USART_SendStringhaha(data1);
				LED = !LED;
				delay_ms(500);
		}
}

