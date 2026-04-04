#ifndef __STM32L476R_NUCLEO_UART_H

#include "stm32l476xx.h"

#define __STM32L476R_NUCLEO_UART_H

#define BufferSize ((uint8_t) 18)
extern uint8_t USART2_Buffer_Rx[];
extern uint8_t USART2_Buffer_Wx[];


void USART_Init(USART_TypeDef *USARTx);

void USART_Read(USART_TypeDef *USARTx, uint8_t *buffer, uint32_t nBytes);

void USART_Write(USART_TypeDef *USARTx, uint8_t *buffer, uint32_t nBytes);

void UART_Send_Receive_Interrupts_Init(void);



#endif
