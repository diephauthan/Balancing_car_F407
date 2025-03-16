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
			int Encoder_Left = 0, Encoder_Right = 0; 
			bsp_init(); // System initialization
	

			

		while(1)
		{
			Encoder_Left+=Read_Encoder(MOTOR_ID_ML); //Read the value of the left wheel encoder, forward is positive, backward is negative
			Encoder_Right+=-Read_Encoder(MOTOR_ID_MR); //Read the value of the right wheel encoder, forward is positive, backward is negative
			printf("L:%d\t R:%d\r\n",Encoder_Left,Encoder_Right);
			USART2_Send_Float((float)Encoder_Left, 2);
			
			//delay_ms(200);		
		}
}

