#ifndef UART_H
#define UART_H

#include "stm32f1xx.h"

void uart_init(void);
void uart_write_char(char ch);
void uart_write_string(const char *str);

#endif
