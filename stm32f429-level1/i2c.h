#ifndef I2C_H_
#define I2C_H_
#include <stdint.h>

void I2C1_init(void);
void I2C1_byte_read(char saddr, char maddr, char* data);
void I2C1_byte_write(char saddr, char maddr, char data);

void I2C1_burst_read(char saddr, char maddr, char* data, uint8_t n);

#endif /* I2C_H_ */

