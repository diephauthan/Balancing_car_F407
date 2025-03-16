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
	
//			SystemClockConfig();
//			Init_Led_GPIO();								// Onboard LED
//			TIM7_Init();
			

		while(1)
		{
//			GPIO_SetBits(GPIOD,GPIO_Pin_12);
//			delay_ccms(500);
//			GPIO_ResetBits(GPIOD,GPIO_Pin_12);
//			delay_ccms(500);
			
		}
}

