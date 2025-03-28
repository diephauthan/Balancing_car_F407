#include "timer.h"

u16 led_flag = 0; // 1: Entering flashing state, 0: Waiting for flashing
u16 led_twinkle_count = 0; // Flashing Count 
u16 led_count = 0; // Start counting 
volatile uint32_t _millis = 0;

/**************************************************************************
Function function: TIM7 initialization, timed for 10 milliseconds
Entrance parameters: None
Return value: None
Description: Initialize TIM7 to trigger an interrupt every 10ms
**************************************************************************/
void TIM7_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1; //27-1
	TIM_TimeBaseStructure.TIM_Period = 99; //9999
	
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);                   // Clear the update flag of TIM
  TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);              // Enable TIM7 update interrupt
	
	// Interrupt priority NVIC setting
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // Preemption priority level 4
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      // Sub priority level 2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         // Enable IRQ channel
  
	NVIC_Init(&NVIC_InitStructure);                         // Initialize NVIC registers
	
	TIM_Cmd(TIM7, ENABLE);
}

//// TIM7 interrupt service
void TIM7_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) // Check whether TIM update interrupt occurs
    {
       _millis += 10; //Timer overflow every 10ms
       TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    }
}

uint32_t millis(void)
{
    return _millis;
}


