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

float battery_vol = 12; //Fully charged state

int main(void)
{	
		bsp_init(); // System initialization

		printf("Battery detect Class!\r\n");

		while(1)
		{
			battery_vol = Get_Battery_Voltage();

			if(battery_vol < 9.6f)
			{
				BEEP_BEEP = !BEEP_BEEP;
				LED = 0;
			}else{
				BEEP_BEEP = 0;
				LED = !LED;
			}
				
			printf("battery: %.2fV\r\n",Get_Battery_Voltage() );
			delay_ms(500); 
		}
}
