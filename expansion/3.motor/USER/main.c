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
		Set_Pwm(2200,2200);
		delay_ms(1000);
		delay_ms(1000);
		
		Set_Pwm(0,0);
		delay_ms(1000);
		
		Set_Pwm(-2200,-2200);
		delay_ms(1000);
		delay_ms(1000);
		
		Set_Pwm(0,0);
		delay_ms(1000);
 
		}
}

