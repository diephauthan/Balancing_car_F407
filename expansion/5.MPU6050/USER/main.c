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

int main(void)
{	
	bsp_init();
	MPU6050_EXTI_Init();
	
	while(1)
	{
	}
}

void EXTI3_IRQHandler(void)
{
	if(MPU6050_INT==0)		
	{   
		//EXTI->PR=1<<13;
		EXTI_ClearITPendingBit(EXTI_Line3);
		Read_DMP();	// Print the fused data of dmp algorithm
	}
	
}


