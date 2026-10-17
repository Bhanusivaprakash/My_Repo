#include "mpu6050.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>
#include "stm32f429xx.h"

void FPU_enable(void)
{
    SCB->CPACR |= (0xFU << 20);

    __DSB();
    __ISB();
}

void mpu_test(void)
{
	FPU_enable();

    uint8_t data_x[2];
    uint8_t data_y[2];
    uint8_t data_z[2];

    char buffer[240];

    I2C1_init();
    uart_init();

    // Wake up the MPU6050
    I2C1_byte_write(0x68U, 0x6B, 0x00);

    // Accelerometer config for +/- 2g
    I2C1_byte_write(0x68U, 0x1C, (0x00 << 3));


    uart_send_data("____MPU START___\n");


    while (1)
    {
        // Read X-axis upper and lower bytes
        I2C1_burst_read(0x68U, 0x3B, (char *)data_x, 2);
        I2C1_burst_read(0x68U, 0x3D, (char *)data_y, 2);
        I2C1_burst_read(0x68U, 0x3F, (char *)data_z, 2);

        // Combine bytes into a signed 16-bit reading
        int16_t value_x = (int16_t)(((uint16_t)data_x[0] << 8) | data_x[1]);
        int16_t value_y = (int16_t)(((uint16_t)data_y[0] << 8) | data_y[1]);
        int16_t value_z = (int16_t)(((uint16_t)data_z[0] << 8) | data_z[1]);

        float accel_x_g = (float)value_x / 16384.0f;
        int32_t accel_x_scaled = (int32_t)(accel_x_g * 100.0f);

        float accel_y_g = (float)value_y / 16384.0f;
        int32_t accel_y_scaled = (int32_t)(accel_y_g * 100.0f);

        float accel_z_g = (float)value_z / 16384.0f;
        int32_t accel_z_scaled = (int32_t)(accel_z_g * 100.0f);


        // Convert the reading into a decimal string
        snprintf(buffer, sizeof(buffer),
                 "ACCEL_X: %ld   ACCEL_Y: %d   ACCEL_Z: %d\r\n",
                 (long)accel_x_scaled, (int)accel_y_scaled, (int)accel_z_scaled);

        // Transmit the string
		uart_send_data(buffer);
    }
}

