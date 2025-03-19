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
	bsp_init();
	
	printf("start! \r\n");
	
	IWDG_Start();// Enable independent watchdog
	
	while(1)
	{
		if(Key1_State(0))
		{
			// Pressing the button will keep feeding the dog
			IWDG_ReloadCounter(); 
			LED = 1;
		}
	
	}
	
}

