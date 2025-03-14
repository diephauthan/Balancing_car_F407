#include "usart1.h"

//////////////////////////////////////////////////////////////////
// Configure USART1, enable printf support without selecting use MicroLIB
// Add the following code to support the printf function without selecting use MicroLIB
#if 0
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
//void uart_init(u32 bound)
//{
//	// GPIO pin initialization for USART
//  GPIO_InitTypeDef 	GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	//NVIC_InitTypeDef NVIC_InitStructure; 
//	
//	// Enable clocks for USART1 and GPIOA
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

//	// Configure USART1_TX (PA.9)
////  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // PA.9
////  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	// Alternate function, push-pull output
////	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
////	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
////	
////  GPIO_Init(GPIOA, &GPIO_InitStructure); // Initialize GPIOA.9
////  
////  // Configure USART1_RX (PA.10)
////  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // PA10
////  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
////	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
////	
////  GPIO_Init(GPIOA, &GPIO_InitStructure); // Initialize GPIOA.10
//	
//	
//	// Configure USART1 Rx (PA10) as input floating
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);

//	// Configure USART1 Tx (PA9) as alternate function push-pull
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_UART4);  // Cho TX
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_UART4); // Cho RX
//	
//  // USART1 initialization settings
//	USART_InitStructure.USART_BaudRate = bound;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	//USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	
//	USART_Init(UART4, &USART_InitStructure);
//	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // Enable USART receive interrupt
//  USART_Cmd(UART4, ENABLE); // Enable USART1
//	
//	// NVIC configuration for USART1 interrupt
////	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
////	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
////	NVIC_Init(&NVIC_InitStructure);
//}



void uart_init(uint32_t bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
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
    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  // Cho TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART3);  // Cho RX
    
    // UART initialization settings
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    
    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);
}

void USART_SendStringhaha(const char *str) {
    while (*str) {
        while (!(USART2->SR & USART_FLAG_TXE));
        USART_SendData(USART2, *str++);
    }
}


// Hàm g?i d? li?u qua UART4
void UART4_SendByte(char data)
{
    // Ð?i cho d?n khi buffer truy?n s?n sàng
    while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
    
    // G?i d? li?u
    USART_SendData(UART4, data);
}


/**
 * @Brief: UART1 sends a byte of data
 * @Note: 
 * @Parm: ch - Data to be sent
 * @Retval: none
 */
void USART1_Send_U8(u8 data)
{
	while(USART_GetFlagStatus(UART4,USART_FLAG_TXE) == RESET);
	
	USART_SendData(UART4, data);
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
//void USART1_IRQHandler(void)
//{
//	uint8_t Rx1_Temp = 0;
//	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // Check if the interrupt is triggered
//	{
//		Rx1_Temp = USART_ReceiveData(USART1); // Read received data
//		USART1_Send_U8(Rx1_Temp); // Echo the received data
//	}
//}

void USARTSend(char *pucBuffer)
{
   while (*pucBuffer)
    {
        while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
        USART_SendData(UART4, *pucBuffer++);
    }
}

void USARTRead(u16 data)
{
		char buffer[10];
    uint8_t receivedData;
    
    // Ki?m tra xem c? d? li?u nh?n du?c kh?ng
    if (USART_GetFlagStatus(UART4, USART_FLAG_RXNE) != RESET)
    {
        receivedData = USART_ReceiveData(UART4);  // ??c d? li?u t? USART1
        
        // G?i d? li?u d? nh?n tr? l?i (echo)
        USART1_Send_U8(receivedData);
        
        // N?u b?n mu?n g?i l?i gi? tr? nh?n du?c du?i d?ng chu?i, v? d?: n?u nh?n du?c d? li?u l? 65, g?i "65"
        sprintf(buffer, "%d\r\n", receivedData);
        USARTSend(buffer);
        
				sprintf(buffer, "%d\r\n", receivedData);
				USARTSend(buffer);
    }
}


void UART_SendChar(USART_TypeDef *USARTx, uint16_t data){
	USARTx->DR = 0x00; //delete data register
	USART_SendData(USARTx, data);
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET);
}




void UART_SendString(USART_TypeDef *USARTx, char *str){
	while(*str)
		{
		UART_SendChar(USARTx, *str);
		str++;
		}
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET){};
				delay_us(1000);
}




