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

int main(void)
{	
		bsp_init(); // System initialization
		

		while(1)
		{
				LED = !LED;
				USART2_Send_Float(Get_Battery_Voltage(), 4);
				delay_ms(500);
		}
}
