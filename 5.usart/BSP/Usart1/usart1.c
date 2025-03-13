#include "usart1.h"

//////////////////////////////////////////////////////////////////
// Configure USART1, enable printf support without selecting use MicroLIB
// Add the following code to support the printf function without selecting use MicroLIB

#if 1
#pragma import(__use_no_semihosting)

// Support functions required by the standard library
struct __FILE
{
	int handle;
};

FILE __stdout;

// Define _sys_exit() to avoid using semihosting mode
void _sys_exit(int x)
{
	x = x;
}

// Redefine fputc function
int fputc(int data, FILE *f)
{
	while((USART1->SR&0X40)==0);
	USART1->DR = (u8) data;
	return data;
}

#endif

/**************************************************************************
Function: Serial port 1 initialization
Input   : bound - Baud rate
Output  : none
Description: This function initializes USART1 with the specified baud rate.
**************************************************************************/
void uart_init(u32 bound)
{
	// GPIO pin initialization for USART
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	// Enable clocks for USART1 and GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_AHB1Periph_GPIOA, ENABLE);

	// Configure USART1_TX (PA.9)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	// Alternate function, push-pull output
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	
  GPIO_Init(GPIOA, &GPIO_InitStructure); // Initialize GPIOA.9
  
  // Configure USART1_RX (PA.10)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; // Floating input
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
  GPIO_Init(GPIOA, &GPIO_InitStructure); // Initialize GPIOA.10
	
  // USART1 initialization settings
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // Enable USART receive interrupt
  USART_Cmd(USART1, ENABLE); // Enable USART1
	
	// NVIC configuration for USART1 interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @Brief: UART1 sends a byte of data
 * @Note: 
 * @Parm: ch - Data to be sent
 * @Retval: none
 */
void USART1_Send_U8(u8 data)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	
	USART_SendData(USART1, data);
}

/**
 * @Brief: UART1 sends an array of data
 * @Note: 
 * @Parm: BufferPtr - Pointer to the data buffer to be sent
 * @Parm: Length - Length of the data buffer
 * @Retval: none
 */
void USART1_Send_ArrayU8(u8 *bufferptr, u16 Length)
{
	while(Length--)
	{
		USART1_Send_U8(*bufferptr);
		bufferptr++;
	}
}

// Serial port interrupt service function
void USART1_IRQHandler(void)
{
	uint8_t Rx1_Temp = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // Check if the interrupt is triggered
	{
		Rx1_Temp = USART_ReceiveData(USART1); // Read received data
		USART1_Send_U8(Rx1_Temp); // Echo the received data
	}
}

void USARTSend(char *pucBuffer)
{
    while (*pucBuffer)
    {
        USART_SendData(USART1, *pucBuffer++);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        {
        }
    }
}

