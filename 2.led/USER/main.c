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
			GPIO_SetBits(LED_PORT, LED_PIN); // Turn ON the LED
			delay_ms(500); // Delay for 500ms to keep the LED on
			GPIO_ResetBits(LED_PORT, LED_PIN); // Turn OFF the LED
			delay_ms(500); // Delay for 500ms to keep the LED off
			
		}
}

