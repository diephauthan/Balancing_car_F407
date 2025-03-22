#include "bsp.h"

void bsp_init(void)
{
	
	DIY_NVIC_PriorityGroupConfig(2);	  // Set interrupt grouping
  SystemClockConfig();
  delay_init();
	
  Init_Led_GPIO();								// Onboard LED
	KEYAll_GPIO_Init();
	uart_init(115200);

  BalanceCar_Motor_Init();     		//Motor GPIO initialization
	BalanceCar_PWM_Init(6720,0);    // fpwm = 25khz
  Encoder_Init_TIM3();
	Encoder_Init_TIM4();
  	
  TIM7_Init();
}

// void JTAG_Set(u8 mode)
// {
//     u32 temp;
//     temp=mode;
//     temp<<=25;
//     RCC->APB2ENR|=1<<0;     // Activate auxiliary clock  
//     AFIO->MAPR&=0XF8FFFFFF; // Clear MAPR[26:24]
//     AFIO->MAPR|=temp;       // Set JTAG mode
// }

/**************************************************************************
Function: Set NVIC group
Input   : NVIC_Group
Output  : none
Description: Set interrupt grouping
Input parameter: NVIC_Group: NVIC grouping 0~4, 5 groups in total
Note: It is necessary to clear previous settings
**************************************************************************/ 
void DIY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
    u32 temp,temp1;	  
    temp1=(~NVIC_Group)&0x07; // Take the last three bits
    temp1<<=8;
    temp=SCB->AIRCR;  // Read previous settings
    temp&=0X0000F8FF; // Clear previous groups
    temp|=0X05FA0000; // Write the key
    temp|=temp1;	   
    SCB->AIRCR=temp;  // Set grouping   	   
}
