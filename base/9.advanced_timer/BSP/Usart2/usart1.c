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
	while((USART2->SR&0X40)==0);
	USART2->DR = (u8) data;
	return data;
}

#endif

/**************************************************************************
Function: Serial port 1 initialization
Input   : bound - Baud rate
Output  : none
Description: This function initializes USART2 with the specified baud rate.
**************************************************************************/
void uart_init(uint32_t bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure; 
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  // TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART3);  // RX
    
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	
    // Configure USART2 Rx (PA3)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // Configure USART2 Tx (PA2)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // UART initialization settings
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    
    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);
		
		// NVIC configuration for USART2 interrupt
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}

void USART_SendString(const char *str) {
    while (*str) {
        while (!(USART2->SR & USART_FLAG_TXE));
        USART_SendData(USART2, *str++);
    }
}



/**
 * @Brief: UART1 sends a byte of data
 * @Note: 
 * @Parm: ch - Data to be sent
 * @Retval: none
 */
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
void USART2_Send_ArrayU8(uint8_t *BufferPtr, uint16_t Length)
{
	while (Length--)
	{
		USART2_Send_U8(*BufferPtr); // Send each byte
		BufferPtr++; // Move to the next byte
	}
}

// Serial port interrupt service function
void USART2_IRQHandler(void)
{
	uint8_t Rx1_Temp = 0;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // Check if the interrupt is triggered
	{
		Rx1_Temp = USART_ReceiveData(USART2); // Read received data
		USART2_Send_U8(Rx1_Temp); // Echo the received data
	}
}

void USART2_Send_Float(float value, u16 precision)
{
    char buffer[20]; 
    
    sprintf(buffer, "%.*f", precision, value);
    
    // Send string over USART2
    USART2_Send_ArrayU8((u8*)buffer, strlen(buffer));
}







