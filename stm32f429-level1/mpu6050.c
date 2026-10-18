#include "mpu6050.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>
#include "stm32f429xx.h"
#include "tim.h"
#include <math.h>

void FPU_enable(void)
{
    SCB->CPACR |= (0xFU << 20);

    __DSB();
    __ISB();
}

void mpu_test(void)
{
	FPU_enable();

    uint8_t data_x[2], data_y[2], data_z[2], sample_count = 0;

    int32_t prev_out_x, prev_out_y, prev_out_z, out_x, out_y, out_z;

    int64_t sum_acc_x = 0, sum_acc_y = 0, sum_acc_z = 0,
    		sum_x = 0, sum_y = 0, sum_z = 0;

    int32_t peak_x, peak_y, peak_z;

    char buffer[240];

    I2C1_init();
    uart_init();
    tim2_1hz_init();

    // Wake up the MPU6050
    I2C1_byte_write(0x68U, 0x6B, 0x00);

    // Accelerometer config for +/- 2g
    I2C1_byte_write(0x68U, 0x1C, (0x00 << 3));


    uart_send_data("____FILTER INITIALIZATION___\n");


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

	// FIR Low-Pass Filtering
	prev_out_x = 256* accel_x_scaled;
	prev_out_y = 256* accel_y_scaled;
	prev_out_z = 256* accel_z_scaled;

	out_x = prev_out_x + (256*accel_x_scaled - prev_out_x)/4;
	out_y = prev_out_y + (256*accel_y_scaled - prev_out_y)/4;
	out_z = prev_out_z + (256*accel_z_scaled - prev_out_z)/4;

	// Peak initialization
	peak_x = out_x/256; peak_y = out_y/256; peak_z = out_z/256;

    uart_send_data("____FILTER READY___\n");
    uart_send_data("____START___\n");

    while (1)
    {
    	while(!(TIM2->SR & SR_UIF)){}

    	TIM2->SR &= ~(SR_UIF);
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

        // FIR Low-Pass Filtering

        out_x = prev_out_x + (256*accel_x_scaled - prev_out_x)/4;
        out_y = prev_out_y + (256*accel_y_scaled - prev_out_y)/4;
        out_z = prev_out_z + (256*accel_z_scaled - prev_out_z)/4;

        prev_out_x = out_x; prev_out_y = out_y; prev_out_z = out_z;

        sum_x += (out_x/256);
        sum_y += (out_y/256);
        sum_z += (out_z/256);

        // RMS tracking
		sum_acc_x += (out_x/256)*(out_x/256);
		sum_acc_y += (out_y/256)*(out_y/256);
		sum_acc_z += (out_z/256)*(out_z/256);

        if((out_x)/256 > peak_x){
			peak_x = (out_x)/256;
		}
		if((out_y)/256 > peak_y){
			peak_y = (out_y)/256;
		}
		if((out_z)/256 > peak_z){
			peak_z = (out_z)/256;
		}

		sample_count++;

        if(sample_count == 10){
        	// PRINT RMS TO UART
        	// Convert the reading into a decimal string
        	snprintf(buffer, sizeof(buffer),
        	         "ACCEL  X: %ld   Y: %ld   Z: %ld\r\n"
        	         "RMS    X: %ld   Y: %ld   Z: %ld\r\n"
        	         "PEAK   X: %ld   Y: %ld   Z: %ld\r\n"
        			 "AVG    X: %ld   Y: %ld   Z: %ld\r\n",
        	         (long)(out_x / 256),
        	         (long)(out_y / 256),
        	         (long)(out_z / 256),
        	         (long)sqrt(sum_acc_x / sample_count),
        	         (long)sqrt(sum_acc_y / sample_count),
        	         (long)sqrt(sum_acc_z / sample_count),
					 (long)peak_x,
					 (long)peak_y,
					 (long)peak_z,
					 (long)(sum_x/sample_count),
					 (long)(sum_y/sample_count),
					 (long)(sum_z/sample_count));

			// Transmit the string
			uart_send_data(buffer);
			sample_count = 0;
			sum_x = 0;
			sum_y = 0;
			sum_z = 0;
			sum_acc_x = 0;
			sum_acc_y = 0;
			sum_acc_z = 0;
			peak_x = out_x/256;
			peak_y = out_y/256;
			peak_z = out_z/256;
        }

        else{
        	// Convert the reading into a decimal string
			snprintf(buffer, sizeof(buffer),
					 "ACCEL_X: %ld   ACCEL_Y: %ld   ACCEL_Z: %ld\r\n",
					 (long)out_x/256, (long)out_y/256, (long)out_z/256);

			// Transmit the string
			uart_send_data(buffer);
        }
    }
}
