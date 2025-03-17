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
    RCC_HCLKConfig(RCC_SYSCLK_Div1);   // AHB  = SYSCLK/1 = 168MHz
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


uint32_t GetAPB1Clock(void) {
  uint32_t pclk1 = 0;
  uint32_t sysclk = 0;
  uint32_t pllm, plln, pllp;
  uint32_t presc = 0;
  uint32_t source = 0;
  uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
  uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};
  
  // Xác d?nh ngu?n clock h? th?ng
  source = RCC->CFGR & RCC_CFGR_SWS;
  
  // Tính SYSCLK d?a trên ngu?n
  if (source == 0x00) { // HSI làm ngu?n
    sysclk = HSI_VALUE;
  } else if (source == 0x04) { // HSE làm ngu?n
    sysclk = HSE_VALUE;
  } else if (source == 0x08) { // PLL làm ngu?n
    // Ð?c giá tr? PLLM, PLLN, PLLP t? thanh ghi RCC_PLLCFGR
    pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
    plln = (RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6;
    pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> 16) + 1) * 2;
    
    // Xác d?nh ngu?n PLL (HSI ho?c HSE)
    if (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) {
      // HSE làm ngu?n PLL
      sysclk = (HSE_VALUE / pllm) * plln / pllp;
    } else {
      // HSI làm ngu?n PLL
      sysclk = (HSI_VALUE / pllm) * plln / pllp;
    }
  }
  
  // Tính b? chia cho APB1
  presc = APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> 10];
  pclk1 = sysclk >> presc;
  
  return pclk1;
}

void USART2_Send_U8(u8 data)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
	
	USART_SendData(USART2, data);
}

/**
 * @Brief: UART1 sends an array of data
 * @Note: 
 * @Parm: BufferPtr - Pointer to the data buffer to be sent
 * @Parm: Length - Length of the data buffer
 * @Retval: none
 */
void USART2_Send_ArrayU8(u8 *bufferptr, u16 Length)
{
	while(Length--)
	{
		USART2_Send_U8(*bufferptr);
		bufferptr++;
	}
}
void USART2_Send_Float(float value, u16 precision)
{
    char buffer[20]; // K?ch thu?c d? cho h?u h?t s? float
    
    // Chuy?n d?i float th?nh chu?i v?i d? ch?nh x?c c? th?
    sprintf(buffer, "%.*f", precision, value);
    
    // G?i chu?i qua USART2
    USART2_Send_ArrayU8((u8*)buffer, strlen(buffer));
}

char data1[] ="HELLO CIRCLE\r\n";

int main(void)
{	
		bsp_init(); // System initialization
		SystemClockConfig();
		uint32_t sysCoreClock = SystemCoreClock;
		uint32_t apb1Clock = GetAPB1Clock();
		
		while(1)
		{
				//USART1_Send_U8((u8)*data1);
				USART2_Send_Float((float)apb1Clock, 4);
				//USART2_Send_Float((float)sysCoreClock, 4);
				LED = !LED;
				delay_ms(500);

		}
}


