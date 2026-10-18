#include "stm32f4xx.h"
#include <stdint.h>

#define TIM2_EN (1 << 0)

void tim2_1hz_init(void){

	RCC->APB1ENR |= TIM2_EN;	// Enabling clock access to Timer-2

	TIM2->PSC = 1600 - 1;		// Set the pre-scaler value to 1599

	TIM2->ARR = 10000 - 1;		// Set the auto-reload value

	TIM2->CNT = 0;				// Clear the counter

	TIM2->CR1 |= (1 << 0);		// Counter/Timer enable
	
}
