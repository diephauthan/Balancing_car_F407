#include "led.h"

void Init_Led_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(LED_RCC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
	
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
	
	LED = 0;
	//printf("0.1\r\n");
}
