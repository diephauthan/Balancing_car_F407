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

u8 key_step = 0;
int16_t Position_Left = 10000,Position_Right = 10000; 
int16_t Left_target = -5000,Right_target = -5000; 

int main(void)
{	
	bsp_init(); // System initialization
	
	while(1)
	{	
		printf("L: %d\t R: %d\n", Position_Left, Position_Right);
		delay_ms(200);

		if(Key1_State(1))
		{
			printf("key put down\r\n");
			LED = !LED;
			key_step++;
			if(key_step >=4)
			{
				key_step = 0;
			}
		}

		if(key_step == 1)
		{
			Left_target = -5500;
			Right_target = -5500;
		}
		else if (key_step == 3)
		{
			Left_target = -4500;
			Right_target = -4500;
		}
		else{
			Left_target = -5000;
			Right_target = -5000;
		}
		

	}
}

