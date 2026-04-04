#include "UART.h"
#include "GPIO.h"

#include "stm32l476xx.h"
#include "stdint.h"
#include "stdio.h"

uint8_t USART2_Buffer_Rx[BufferSize];
uint8_t USART2_Buffer_Wx[BufferSize];
//volatile uint32_t Rx1_Counter = 0, Rx2_Counter = 0, Rx3_Counter = 0, Rx4_Counter = 0;
//volatile uint32_t Tx1_Counter = 0, Tx2_Counter = 0, Tx3_Counter = 0, Tx4_Counter = 0;

void USART_Read(USART_TypeDef *USARTx, uint8_t *buffer, uint32_t nBytes){
		for(uint32_t i = 0; i < nBytes; i++){
				while(!(USARTx->ISR & USART_ISR_RXNE));
				//puts input byte by byte into buffer
				buffer[i] = USARTx->RDR;
		}
}

void USART_Write(USART_TypeDef *USARTx, uint8_t *buffer, uint32_t nBytes){ 
    // TXE is cleared by a write to the USART_DR register.
    // TXE is set by hardware then the content of the TDR
    // register has been transferred into the shift register. 
		
    for (uint32_t i = 0; i < nBytes; i++){
        // wait untill TXE (TX empty) is set
        while(!(USARTx->ISR & USART_ISR_TXE));
        
				// Writing USART_TDR automatically clears the TXE flag
        USARTx->TDR = (buffer[i] & 0xFF);
    }
    
    while (!(USARTx->ISR & USART_ISR_TC));    // wait until TC bit is set
    USARTx->ICR |= USART_ICR_TCCF;
}

void UART_Send_Receive_Interrupts_Init(void){
    USART1->CR1 |= USART_CR1_RXNEIE;    // Receive register not empty interrupt
    USART1->CR1 &= -USART_CR1_TXEIE;    // Transmit register empty interrupt
    NVIC_SetPriority(USART1_IRQn, 0);   // Set the highest urgency
    NVIC_EnableIRQ(USART1_IRQn);        // Enable NVIC interrupt
}