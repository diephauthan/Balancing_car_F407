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
			uint32_t lastTime = 0;
			uint32_t currentTime = 0;
			bsp_init(); // System initialization
	
		while(1)
		{	
			currentTime = millis();
			
			Encoder_Left+=-Read_Encoder(MOTOR_ID_ML); //Read the value of the left wheel encoder, forward is positive, backward is negative
			Encoder_Right+=-Read_Encoder(MOTOR_ID_MR); //Read the value of the right wheel encoder, forward is positive, backward is negative

			if (currentTime - lastTime >= 200) {
            printf("L:%d\t R:%d\r\n", Encoder_Left, Encoder_Right);
            lastTime = currentTime;
        }
		}
}

