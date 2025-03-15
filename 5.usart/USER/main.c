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


char data1[] ="HELLO CIRCLE\r\n";

int main(void)
{	
		bsp_init(); // System initialization
		
		while(1)
		{
				//USART1_Send_U8((u8)*data1);
				USART1_Send_ArrayU8((u8*)data1, strlen(data1));
				LED = !LED;
				delay_ms(500);

		}
}


