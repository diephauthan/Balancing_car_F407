#include "beep.h"

uint32_t beep_time = 0;

void Init_Beep(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(BEEP_RCC, ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
		GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
		
		BEEP_BEEP = 0;
}

// Buzzer on time
// beep_time is in 1ms units. The value of beep_time is divided by 10 to convert it into 10ms units.
void open_beep(u32 beep_time) // The time is subtracted once every 10ms
{
	beep_time = beep_time / 10;
}




