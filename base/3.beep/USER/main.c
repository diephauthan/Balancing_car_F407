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

int main(void)
{	
		bsp_init(); // System initialization

		while(1)
		{
				GPIO_SetBits(BEEP_PORT, BEEP_PIN);
				delay_ms(500);
				GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
				delay_ms(500);
		}
}

