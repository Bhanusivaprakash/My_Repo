#include "stm32f429xx.h"
#include <stdint.h>

#define WR_PIN  0
#define RST_PIN 3
#define ACK_PIN 5
#define DONE_PIN 6
#define EXECUTE_PIN 0
#define ADDRESS_ENABLE 4

static inline int  ack_is_set(void) { return (GPIOA->IDR & (1U << ACK_PIN)) != 0; }
static inline void wr_high(void)    { GPIOA->BSRR = (1U << WR_PIN); }
static inline void wr_low(void)     { GPIOA->BSRR = (1U << (WR_PIN + 16)); }

static void hold_min_pulse(void)
{
    for (volatile uint32_t i = 0; i < 100; i++);
}

int main(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;

    // PE0 to PE4 - Operation[0 : 4], output
    GPIOE->MODER &= ~(3U << (0 * 2)); GPIOE->MODER |= (1U << (0 * 2));
    GPIOE->MODER &= ~(3U << (1 * 2)); GPIOE->MODER |= (1U << (1 * 2));
    GPIOE->MODER &= ~(3U << (2 * 2)); GPIOE->MODER |= (1U << (2 * 2));
    GPIOE->MODER &= ~(3U << (3 * 2)); GPIOE->MODER |= (1U << (3 * 2));
    GPIOE->MODER &= ~(3U << (4 * 2)); GPIOE->MODER |= (1U << (4 * 2));

    // PE7 to PE11 - Address[0:4], output
	GPIOE->MODER &= ~(3U << (7 * 2));  GPIOE->MODER |= (1U << (7 * 2));
	GPIOE->MODER &= ~(3U << (8 * 2));  GPIOE->MODER |= (1U << (8 * 2));
	GPIOE->MODER &= ~(3U << (9 * 2));  GPIOE->MODER |= (1U << (9 * 2));
	GPIOE->MODER &= ~(3U << (10 * 2)); GPIOE->MODER |= (1U << (10 * 2));
	GPIOE->MODER &= ~(3U << (11 * 2)); GPIOE->MODER |= (1U << (11 * 2));


    /* PA0 WR, PA3 RESET: outputs */
    GPIOA->MODER &= ~(3U << (WR_PIN  * 2)); GPIOA->MODER |= (1U << (WR_PIN  * 2));
    GPIOA->MODER &= ~(3U << (RST_PIN * 2)); GPIOA->MODER |= (1U << (RST_PIN * 2));

    // PC0 EXECUTE: output
    GPIOC->MODER &= ~(3U << (EXECUTE_PIN * 2)); GPIOC->MODER |= (1U << (EXECUTE_PIN * 2));

    // PA4 ADDRESS_ENABLE: output
	GPIOA->MODER &= ~(3U << (ADDRESS_ENABLE * 2));
	GPIOA->MODER |=  (1U << (ADDRESS_ENABLE * 2));

    /* PA5 ACK: input */
    GPIOA->MODER &= ~(3U << (ACK_PIN * 2));

    // PA6 DONE: input
    GPIOA->MODER &= ~(3U << (DONE_PIN * 2));

    /* PD0-PD15 = data bus, output */
    GPIOD->MODER = 0x55555555;

    wr_low();
    GPIOA->BSRR = (1U << (RST_PIN + 16)); /* RESET low (inactive) */

    /* Reset pulse - held long enough to actually be seen */
    GPIOA->BSRR = (1U << RST_PIN);        /* RESET high */
    hold_min_pulse();
    GPIOA->BSRR = (1U << (RST_PIN + 16)); /* RESET low */

    /* Write one half-word: present data, raise WR, wait for ACK,
     * drop WR, wait for ACK to actually clear. */

    GPIOD->ODR = 0xCCCD;

    //while (!ack_is_set()) { wr_high(); }
    wr_high();
    hold_min_pulse();
    wr_low();
    //while (ack_is_set()) { }

    GPIOD->ODR = 0xFFFD;

  //  while (!ack_is_set()) { wr_high(); }
    wr_high();
    hold_min_pulse();
	wr_low();
	//while (ack_is_set()) { }

    //GPIOC->BSRR = (1U << EXECUTE_PIN);

    // Clear PE7-PE11
        GPIOE->ODR &= ~(0x1F << 7);
        // Set address = 0 (00000)
        GPIOE->ODR |= (0x00 << 7);

	GPIOA->BSRR = (1U << ADDRESS_ENABLE);

    GPIOE->ODR &= ~(0x1F);
    GPIOE->ODR |= 0b00110;

    GPIOC->BSRR = (1U << EXECUTE_PIN);


    while (1);
}
