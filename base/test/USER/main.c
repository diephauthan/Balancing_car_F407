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

TIM_TimeBaseInitTypeDef TimStruct;


void delay_ccms(uint32_t ms) {
    for(uint32_t i = 0; i < ms * 10; i++) {  // 101µs × 10 ˜ 1ms
        TIM7->SR &= ~TIM_SR_UIF;  // Xóa c? tràn
        while(!(TIM7->SR & TIM_SR_UIF)) {}  // Ch? tràn
    }
}

void SystemClockConfig(void)
{
  // Khai báo các bi?n c?u hình
  RCC_DeInit(); // Reset c?u hình RCC v? m?c d?nh
  
  // B?t HSE (High Speed External oscillator)
  RCC_HSEConfig(RCC_HSE_ON);
  
  // Ð?i cho HSE ?n d?nh
  if (RCC_WaitForHSEStartUp() == SUCCESS)
  {
    // C?u hình Flash latency
    FLASH_SetLatency(FLASH_Latency_5); // 5 wait states cho 168MHz
    FLASH_PrefetchBufferCmd(ENABLE);
    FLASH_InstructionCacheCmd(ENABLE);
    FLASH_DataCacheCmd(ENABLE);
    
    // C?u hình các b? chia t?n s?
    RCC_HCLKConfig(RCC_SYSCLK_Div1);   // AHB  = SYSCLK/1 = 168MHz
    RCC_PCLK1Config(RCC_HCLK_Div4);    // APB1 = HCLK/4  = 42MHz (max cho APB1)
    RCC_PCLK2Config(RCC_HCLK_Div2);    // APB2 = HCLK/2  = 84MHz
    
    // C?u hình Main PLL
    RCC_PLLConfig(RCC_PLLSource_HSE,    // S? d?ng HSE làm ngu?n PLL
                 8,                     // PLLM = 8 (HSE/8 = 1MHz)
                 336,                   // PLLN = 336 (1MHz * 336 = 336MHz)
                 2,                     // PLLP = 2 (336MHz/2 = 168MHz cho SYSCLK)
                 7);                    // PLLQ = 7 (336MHz/7 = 48MHz cho USB)
    
    // B?t PLL
    RCC_PLLCmd(ENABLE);
    
    // Ð?i cho PLL ?n d?nh
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}
    
    // Ch?n PLL làm ngu?n SYSCLK
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    
    // Ð?i cho d?n khi PLL du?c s? d?ng làm ngu?n system clock
    while(RCC_GetSYSCLKSource() != 0x08) {}
  }
}



int main(void)
{	
		SystemClockConfig();
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
		//Enable HSE clock
		RCC_HSEConfig(RCC_HSE_ON);
		//Wait for clock to stabilize
		while (!RCC_WaitForHSEStartUp());
		
		//bsp_init(); // System initialization

	
		TimStruct.TIM_Prescaler = 84-1; //26.88-1
		TimStruct.TIM_Period = 99; //0xffff
		TIM_TimeBaseInit(TIM7, &TimStruct);
		TIM_Cmd(TIM7,ENABLE);
	
		Init_Led_GPIO();	

		while(1)
		{
			GPIO_SetBits(LED_PORT, LED_PIN); // Turn ON the LED
			delay_ccms(1000); // Delay for 500ms to keep the LED on
			GPIO_ResetBits(LED_PORT, LED_PIN); // Turn OFF the LED
			delay_ccms(2000); // Delay for 500ms to keep the LED off
			
		}
}




