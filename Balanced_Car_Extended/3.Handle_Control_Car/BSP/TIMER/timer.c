#include "timer.h"

static float battery_All;
static uint8_t battery_count=0,battery_flag=0;

static u16 stop_time = 0;//delay time

u16 led_flag = 0; // 1: Entering flashing state, 0: Waiting for flashing
u16 led_twinkle_count = 0; // Flashing Count 
u16 led_count = 0; // Start counting 

u8 lower_power_flag = 0;

//Timer 7 has a delay of 10ms. This method is more accurate than delay
void delay_time(u16 time)
{
	stop_time = time;
	while(stop_time);//���� Wait
}

//�ӳ�1s Unit second
void my_delay(u16 s)//s
{
	for(int i = 0;i<s;i++)
	{
		delay_time(100);
	}
}

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
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) // Check whether TIM update interruption occurs
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);    // Clear TIMx update interrupt flag
		led_count++;  //LED service display logo
		battery_flag ++;		//Electricity display sign
		
		if(stop_time>0)
		{
			stop_time --;
		}
		
		
		
///////Voltage detection process
		if(battery_flag > 2)//20ms
		{		
			battery_flag = 0;
			battery_All += Get_Battery_Voltage();//Obtain the power level of the power supply
			battery_count++;
			if(battery_count == 50)//1000ms
			{
				battery = battery_All/50; //average value
				battery_All = 0;  
				battery_count = 0;
				power_decect();// Voltage processing
			}
			
		}
///////////
		
		cotrol_led();//�Ʒ���  led service
		
				
		
	}
}

void power_decect(void)
{
	static u8 normal_power_flag = 1; //Voltage recovery flag 0: not restored 1: restored
	if(battery < 9.6f) //Alarm below 9.6V
	{
		lower_power_flag = 1;
		normal_power_flag = 0;
	}
	else
	{
		if(normal_power_flag == 0)
		{
			lower_power_flag = 0;
			normal_power_flag = 1;
			BEEP_BEEP = 0;
		}
		
	}
}

void cotrol_led(void)
{
	 //The effect of the lamp and buzzer is low voltage alarm
		if(!led_flag)
		{
			if(led_count>300)//3S
			{
				led_count = 0;
				led_flag = 1;
			}
		}
		else
		{
			if(led_count>20)//200ms
			{
				led_count = 0;
				
				if(lower_power_flag == 0)
				{
					LED = !LED;//State reversal
				}
				else
				{
					BEEP_BEEP = !BEEP_BEEP;
					LED = 1; //Low voltage blue light is always on
				}
				
				led_twinkle_count++;
				if(led_twinkle_count == 6)
				{
					if(lower_power_flag == 0)
					{
						LED = 0;
					}
					else
					{
						BEEP_BEEP = 0;
					}
					
					led_twinkle_count = 0;
					led_flag = 0;
				}
				
			}
		}

}


