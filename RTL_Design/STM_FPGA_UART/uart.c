#include "uart.h"

#define GPIOA_EN   (1U << 2)
#define USART1_EN  (1U << 14)

void uart_init(void){
    RCC->APB2ENR |= GPIOA_EN;
    RCC->APB2ENR |= USART1_EN;

    // PA9 -> TX (AF Push-Pull)
    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |=  (0xB << 4);

    // PA10 -> RX (Floating input)
    GPIOA->CRH &= ~(0xF << 8);
    GPIOA->CRH |=  (0x4 << 8);

    USART1->BRR = 0x341;

    USART1->CR1 |= (1 << 2);   // RE
    USART1->CR1 |= (1 << 3);   // TE
    USART1->CR1 |= (1 << 13);  // UE
}

void uart_write_char(char ch){
    while(!(USART1->SR & (1 << 7))); // TXE
    USART1->DR = ch;
}

void uart_write_string(const char *str){
    while(*str){
        uart_write_char(*str++);
    }
}
