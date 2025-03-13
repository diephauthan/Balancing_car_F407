#ifndef __USART1_H_
#define	__USART1_H_

#include "main.h"

void uart_init(u32 bound);
void USART1_Send_U8(uint8_t ch);
void USART1_Send_ArrayU8(uint8_t *BufferPtr, uint16_t Length);
void USARTSend(char *pucBuffer);

#endif



