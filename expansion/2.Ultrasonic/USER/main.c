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

extern u32 g_distance;
extern u16 TIM2CH2_CAPTURE_STA, TIM2CH2_CAPTURE_VAL;

int main(void)
{	
	
	bsp_init();
	
	printf("Ultrasonic Class!\r\n");
	
	
	while(1)
	{
		Get_Distance();// Get distance
		printf("Status: %X, Value: %u\r\n", TIM2CH2_CAPTURE_STA, TIM2CH2_CAPTURE_VAL);
		printf("dis:%d mm\r\n",g_distance);
		LED = !LED; // LED status change
		delay_ms(500);
		
	}
}

