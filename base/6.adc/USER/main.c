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
				printf("battery volt: %.2f\r\n",Get_Measure_Voltage() );
				delay_ms(500);
		}
}
