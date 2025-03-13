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

void EXTI9_5_IRQHandler(void) // Key Interrupt Service Function
{
	 if(KEY_INT==0)		
	{   
		EXTI->PR=1<<8; 
		BEEP_BEEP = !BEEP_BEEP; // Every time the button is pressed, the buzzer status reverses
	}

}

int main(void)
{	
		bsp_init(); // System initialization

		while(1)
		{
				LED = !LED;
				delay_ms(500);
		}
}

