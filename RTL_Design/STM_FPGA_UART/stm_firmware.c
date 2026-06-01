#include "stm32f1xx.h"
#include "uart.h"
#include <stdio.h>
#include <stdint.h>

// Simple delay function to prevent flooding the UART
static void delay_ms(uint32_t ms)
{
    // Extremely crude delay for STM32F1 at default clock (approx 8MHz/72MHz)
    // For production, use a hardware timer or SysTick!
    for (volatile uint32_t i = 0; i < ms * 4000; i++);
}

// ---------------- GPIO INIT ----------------
static void gpio_init(void)
{
    // 1. Enable Clock for GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2. Configure PA0 to PA7 as Floating Inputs
    // Clear all configuration bits for the lower byte (PA0 - PA7)
    GPIOA->CRL &= 0x00000000;

    // Set PA0-PA7 to 0x4 (Input Floating mode)
    // 0x44444444 configures all 8 pins simultaneously
    GPIOA->CRL |= 0x44444444;
}

int main(void)
{
    gpio_init();
    uart_init();

    char buf[64];
    uint8_t last_val = 0xFF; // Keep track of the previous reading

    uart_write_string("System Started\r\n");

    while (1)
    {
        // 1. Read the full 8-bit port (Masking the lower 8 bits of IDR)
        uint8_t current_val = (uint8_t)(GPIOA->IDR & 0xFF);

        // 2. Only print if the value has actually changed!
        // This prevents your UART buffer from overflowing with duplicate data.
        if (current_val != last_val)
        {
            sprintf(buf, "Counter: %u (0x%02X)\r\n", current_val, current_val);
            uart_write_string(buf);

            last_val = current_val; // Update the history
        }

        // 3. Small pace delay to let the UART keep up
        delay_ms(10);
    }
}
