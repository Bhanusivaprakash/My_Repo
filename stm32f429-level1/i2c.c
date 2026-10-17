#include "stm32f4xx.h"
#include <stdint.h>

#define GPIOB_EN				(1 << 1)
#define I2C1_EN					(1 << 21)
#define I2C_100KHZ				80		// 0101 0000 = Decimal 90
#define SM_MAX_RISE_TIME		17

#define START_BIT				(1 << 8)
#define STOP_BIT				(1 << 9)

void I2C1_init(void){

	RCC->AHB1ENR |= GPIOB_EN;		// Enables GPIO-B

	GPIOB->MODER &= ~(0xF << 12);
	GPIOB->MODER |= (0x2U << 12);   // PB6 - SCL
	GPIOB->MODER |= (0x2U << 14);	// PB7 - SDA

	GPIOB->AFR[0] &= ~(0xFF << 24);		// Clearing the bits 24 to 31
	GPIOB->AFR[0] |= (0x4 << 24);		// Setting PB6 to AF4 (I2C)
	GPIOB->AFR[0] |= (0x4 << 28);		// Setting PB7 to AF4 (I2C)

	GPIOB->OTYPER |= (1 << 6); 		// PB6 as open-drain
	GPIOB->OTYPER |= (1 << 7);		// PB7 as open-drain


	GPIOB->PUPDR &= ~(0xFU << 12);

	GPIOB->PUPDR |= (0x01 << 12); 	// PB6 pull-up
	GPIOB->PUPDR |= (0x01 << 14);	// PB7 pull-up

	RCC->APB1ENR |= I2C1_EN;		// For I2C1


	// Enter reset mode and come out
	I2C1->CR1 |= (1 << 15);
	I2C1->CR1 &= ~(1 << 15);

	// Set the bus frequency
	I2C1->CR2 |= (1 << 4);

	// Set I2C to standard mode, 100 kHz clock
	I2C1->CCR = I2C_100KHZ;

	// Set max rise time
	I2C1->TRISE = SM_MAX_RISE_TIME;

	// Enable I2C
	I2C1->CR1 |= (1 << 0);

}

void I2C1_byte_write(char saddr, char maddr, char data){

	volatile int tmp;

	// Check if device is busy
	while(I2C1->SR2 & (1 << 1)){}

	//START
	I2C1->CR1 |= START_BIT;	// Start bit

	// Wait until the start flag is set
	while(!(I2C1->SR1 & (1 << 0))){}

	//Send slave address + write
	I2C1->DR = saddr << 1;

	// Wait until address flag is set
	while(!(I2C1->SR1 & (1 << 1))){}

	// Clear address flag
	tmp = I2C1->SR2;

	// Send memory address
	I2C1->DR = maddr;

	// Wait until data register (transmission) is empty
	while(!(I2C1->SR1 & (1 << 7))){}

	// Write data
	I2C1->DR = data;

	// Wait until data register (transmission) is empty
	while(!(I2C1->SR1 & (1 << 7))){}

	// Disable the acknowledge bit
	I2C1->CR1 &= ~(1 << 10);

	// Clear address flag
	tmp = I2C1->SR2;

	// Stop condition
	I2C1->CR1 |= STOP_BIT;

}

void I2C1_byte_read(char saddr, char maddr, char* data){

	volatile int tmp;

	// Check if device is busy
	while(I2C1->SR2 & (1 << 1)){}

	I2C1->CR1 |= START_BIT;	// Start bit

	// Wait until the start flag is set
	while(!(I2C1->SR1 & (1 << 0))){}

	// Transmit slave address + write
	I2C1->DR = saddr << 1;

	// Wait until address flag is set
	while(!(I2C1->SR1 & (1 << 1))){}

	// Clear address flag
	tmp = I2C1->SR2;

	// Send memory address
	I2C1->DR = maddr;

	// Wait until data register (transmission) is empty
	while(!(I2C1->SR1 & (1 << 7))){}

	// Generate re-start
	I2C1->CR1 |= START_BIT;

	// Wait until the start flag is set
	while(!(I2C1->SR1 & (1 << 0))){}

	// Transmit slave address + read
	I2C1->DR = saddr << 1 | 1;

	// Wait until address flag is set
	while(!(I2C1->SR1 & (1 << 1))){}

	// Disable the acknowledge bit
	I2C1->CR1 &= ~(1 << 10);

	// Clear address flag
	tmp = I2C1->SR2;

	// Stop condition
	I2C1->CR1 |= STOP_BIT;

	// Wait until RxNE (Data register NOT empty) flag is set
	while(!(I2C1->SR1 & (1 << 6))){}

	*data++ = I2C1->DR;

}

void I2C1_burst_read(char saddr, char maddr, char* data, uint8_t n){
	volatile int tmp;

	// Check if the bus is busy
	while(I2C1->SR2 & (1 << 1)){}

	// Start
	I2C1->CR1 |= START_BIT;

	// Wait until the start flag is set
	while(!(I2C1->SR1 & (1 << 0))){}

	// Send slave address + write
	I2C1->DR = saddr << 1;

	// Wait until address flag is set
	while(!(I2C1->SR1 & (1 << 1))){}

	// Clear address flag
	tmp = I2C1->SR2;

	// Send memory address
	I2C1->DR = maddr;

	// Wait until data register (transmission) is empty
	while(!(I2C1->SR1 & (1 << 7))){}

	// Generate re-start
	I2C1->CR1 |= START_BIT;

	// Wait until the start flag is set
	while(!(I2C1->SR1 & (1 << 0))){}

	// Transmit slave address + read
	I2C1->DR = saddr << 1 | 1;

	// Wait until address flag is set
	while(!(I2C1->SR1 & (1 << 1))){}

	// Set ack bit
	I2C1->CR1 |= (1 << 10);

	// Clear address flag
	tmp = I2C1->SR2;

	while(n > 0){
		// If one byte
		if(n == 1){
			// Disable acknowledge bit
			I2C1->CR1 &= ~(1 << 10);

			// Generate stop bit
			I2C1->CR1 |= STOP_BIT;

			// Wait until RxNE (Data register NOT empty) flag is set
			while(!(I2C1->SR1 & (1 << 6))){}

			*data++ = I2C1->DR;

			break;
		}
		else{

			// Wait until RxNE (Data register NOT empty) flag is set
			while(!(I2C1->SR1 & (1 << 6))){}
			*data++ = I2C1->DR;

			n--;

		}
	}

}

