 #include "delay.h"

uint8_t fac_us = 0;
uint16_t fac_ms = 0;

 //SysTick initialization function for STM32F407
//void SysTick_Init(uint8_t SYSCLK)
//{
//    // SYSCLK is the system frequency in MHz
//    SysTick->CTRL &= ~(1 << 2);   // Use the HCLK (system clock) as the clock source
//    fac_us = SYSCLK;              // Set the microsecond factor
//    fac_ms = (uint16_t)SYSCLK * 1000; // Set the millisecond factor
//}

//void delay_init(void)
//{
//	uint8_t SYSCLK = SystemCoreClock / 1000000;
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); // HCLK/8  Select external clock HCLK/8
//	fac_us = SYSCLK / 8;
//	fac_ms = (uint16_t)fac_us * 1000;
//}

void delay_us(uint32_t nus){
    TIM_SetCounter(TIM7, 0);
    while(TIM_GetCounter(TIM2) < nus){}
}

void Delay1Ms(){
    TIM_SetCounter(TIM7,0);
    while (TIM_GetCounter(TIM7) <1000){}
}

void delay_ms(uint16_t nms){
    for(uint32_t i = 0; i < nms * 10; i++) {  // 101?s ? 10 ? 1ms
        TIM7->SR &= ~TIM_SR_UIF;  // X?a c? tr?n
        while(!(TIM7->SR & TIM_SR_UIF)) {}  // Ch? tr?n
		}
}

void SystemClockConfig(void)
{
  // Declare configuration variables
  RCC_DeInit(); // Reset RCC configuration to default

  // Enable HSE (High Speed External oscillator)
  RCC_HSEConfig(RCC_HSE_ON);
  
  // Wait for HSE to stabilize
  if (RCC_WaitForHSEStartUp() == SUCCESS)
  {
    // Configure Flash latency
    FLASH_SetLatency(FLASH_Latency_5); // 5 wait states for 168MHz
    FLASH_PrefetchBufferCmd(ENABLE);
    FLASH_InstructionCacheCmd(ENABLE);
    FLASH_DataCacheCmd(ENABLE);
    
    // Configure clock division factors
    RCC_HCLKConfig(RCC_SYSCLK_Div1);   // AHB = SYSCLK/1 = 168MHz
    RCC_PCLK1Config(RCC_HCLK_Div4);    // APB1 = HCLK/4  = 42MHz (max for APB1)
    RCC_PCLK2Config(RCC_HCLK_Div2);    // APB2 = HCLK/2  = 84MHz
    
    // Configure Main PLL
    RCC_PLLConfig(RCC_PLLSource_HSE,    // Use HSE as the PLL source
                 8,                     // PLLM = 8 (HSE/8 = 1MHz)
                 336,                   // PLLN = 336 (1MHz * 336 = 336MHz)
                 2,                     // PLLP = 2 (336MHz/2 = 168MHz for SYSCLK)
                 7);                    // PLLQ = 7 (336MHz/7 = 48MHz for USB)
    
    // Enable PLL
    RCC_PLLCmd(ENABLE);
    
    // Wait until PLL is ready
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}
    
    // Select PLL as the SYSCLK source
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    
    // Wait until PLL is used as the system clock source
    while(RCC_GetSYSCLKSource() != 0x08) {}
  }
}


