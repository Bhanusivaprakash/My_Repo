#include "uart.h"
#include "stm32f4xx.h"
#include <stdint.h>

#define USART1_EN	(1U << 4)
#define GPIOA_EN	(1U << 0)

void uart_init(){
	RCC->APB2ENR |= USART1_EN;	// Enable clock access to USART1

	// Configure the GPIO (Tx and Rx pins)
	RCC->AHB1ENR |= GPIOA_EN;	// Enable clock access to GPIOA

	GPIOA->MODER &= ~((3U << 18) | (3U << 20));

	GPIOA->MODER |= (0x02 << 18);	// PA9 Alternate Function
	GPIOA->MODER |= (0x02 << 20);	// PA10 Alternate Function

	// PA9 and PA10 Push-Pull
	GPIOA->OTYPER &= ~((1U << 9) | (1U << 10));

	// Alternate function configuration
	GPIOA->AFR[1] &= ~(0xFF << 4);
	GPIOA->AFR[1] |= (0x7 << 4);
	GPIOA->AFR[1] |= (0x7 << 8);

	// Configure the word length
	USART1->CR1 &= ~(1 << 12); // 8-bit word length

	USART1->CR1 &= ~(1U << 15);  // Over-sampling by 16
	USART1->BRR = 0x008B;

	// Configure the stop bits
	USART1->CR2 &= ~(1 << 12);
	USART1->CR2 &= ~(1 << 13);

	// Configure the transmitter
	USART1->CR1 |= (1U << 3);    // Transmitter enable

	// UART Enable
	USART1->CR1 |= (1 << 13);

}

void uart_send_char(char ch){
	// Wait until TXE is empty
	while(!(USART1->SR & (1U << 7))){}

	// Send data to the register
	USART1->DR = ch;

}

void uart_send_data(const char *data){
	while(*data){
		uart_send_char(*data++);
	}
}





