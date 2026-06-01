#include "stm32f1xx.h"
#include <stdint.h>
#include <stdio.h>
#include "systick.h"
#include "uart.h"

static void gpio_init(void){

    // Enable GPIOA + AFIO clocks
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN |
                    RCC_APB2ENR_AFIOEN;

    // Disable JTAG, keep SWD
    AFIO->MAPR = (AFIO->MAPR & ~AFIO_MAPR_SWJ_CFG) |
                 AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    // PA12 = write_enable LOW
    GPIOA->BRR = (1 << 12);

    // PA8 = read_enable LOW
    GPIOA->BRR = (1 << 8);

    // PA12 output push-pull
    GPIOA->CRH &= ~(0xF << 16);
    GPIOA->CRH |=  (0x3 << 16);

    // PA8 output push-pull
    GPIOA->CRH &= ~(0xF << 0);
	GPIOA->CRH |=  (0x3 << 0);

	// PA11 output push-pull
	GPIOA->CRH &= ~(0xF << 12);
	GPIOA->CRH |= (0x3 << 12);

}

void transfer_byte(uint8_t data){

    GPIOA->ODR = (GPIOA->ODR & 0xFF00) | data;

    SysDelayMs(1);

    GPIOA->BSRR = (1 << 12);
    SysDelayMs(1);

    GPIOA->BRR = (1 << 12);
    SysDelayMs(1);
}

int main(void){

    const uint8_t seq[] = {0x40, 0x70, 0x10, 0x5, 0x0, 0x30};
    const int len = sizeof(seq) / sizeof(seq[0]);
    char buf[20];

    uart_init();

    gpio_init();

    GPIOA->BSRR = (1 << 11); // Reset signal
    SysDelayMs(10);

    GPIOA->BRR = (1 << 11);

    volatile int j = 0;

    while(j < 2){
		volatile uint8_t read_val = 0;

		// PA0–PA7 output push-pull
		GPIOA->CRL = 0x33333333;

		for (int i = 0; i < len; i++) {
			transfer_byte(seq[i]);

			SysDelayMs(2048);
		}

		// PA0–PA7 input
		GPIOA->CRL = 0x44444444;

		for (int i = 0; i < len; i++) {
			GPIOA->BSRR = (1 << 8);
			SysDelayMs(1);
			read_val = GPIOA->IDR & 0xFF;
			GPIOA->BRR = (1 << 8);
			SysDelayMs(1);
			sprintf(buf, "%d\r\n", (int)read_val);
			uart_write_string(buf);
			SysDelayMs(2048);

		}
	}
    j++;
}
