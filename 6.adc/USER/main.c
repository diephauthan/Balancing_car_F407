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

char mess[] = "Hello MCU ";

int main(void)
{	
		bsp_init(); // System initialization
		

		while(1)
		{
				LED = !LED;
	
				//printf("%.2fV\r\n", Get_Battery_Volotage());
				USART2_Send_Float(Get_Battery_Voltage(), 4);
				delay_ms(500);
		}
}
