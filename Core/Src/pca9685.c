#include <math.h>
#include <string.h>

#include "pca9685.h"


void PCA9685_write(I2C_HandleTypeDef *i2c, uint8_t reg, uint8_t val) {
	uint8_t data[2] = {reg, val};
	HAL_I2C_Master_Transmit(i2c, PCA_I2C_ADDR, data, 2, PCA_I2C_TIMEOUT);
}

void PCA9685_write_data(I2C_HandleTypeDef *i2c, uint8_t reg, uint8_t *data, uint8_t data_len) {
	if (data_len < 0 || data_len > 4)
		return;

	uint8_t i2c_data[2];

	for (uint8_t i=0; i < 4; i++) {
		i2c_data[0] = reg + i;
		i2c_data[1] = data[i];

		HAL_I2C_Master_Transmit(i2c, PCA_I2C_ADDR, i2c_data, 2, PCA_I2C_TIMEOUT);
	}
}

void PCA9685_init(I2C_HandleTypeDef *i2c) {
	PCA9685_write(i2c, PCA_REG_MODE1, 0b00010000);
	PCA9685_write(i2c, PCA_REG_MODE2, 0b00000000);

	// Set the PWM frequency
	uint8_t prescaler_val = (uint8_t) roundf(25000000.0f / (4096 * PCA_PRESCALER_FREQ)) - 1;
	PCA9685_write(i2c, PCA_PRESCALER_REG, prescaler_val);

	// Disable sleep mode to enable PWM control
	PCA9685_write(i2c, PCA_REG_MODE1, 0b00000000);
	HAL_Delay(2);

	uint8_t data[4] = {0x00, 0x00, 0x00, 0x00};
	PCA9685_write_data(i2c, PCA_REG_ALL_ON_L, data, 2);
}


void PCA9685_set_pwm(I2C_HandleTypeDef *i2c, uint8_t channel, uint16_t on_count) {
	if (channel < 0 || channel > 16)
		return;

	if (on_count < 0 || on_count > 4095)
		return;

	uint8_t data[2] = {on_count & 0xff, (on_count >> 8) & 0xff};
	PCA9685_write_data(i2c, PCA_REG_CHAN0_OFF_L + channel*4, data, 2);
}


void PCA9685_set_cycle(I2C_HandleTypeDef *i2c, uint8_t channel, float duty_cycle) {
	if (channel < 0 || channel > 16)
			return;

	if (duty_cycle < 0 || duty_cycle > 1)
		return;

	uint16_t on_count = (uint8_t) roundf(duty_cycle*4096);
	PCA9685_set_pwm(i2c, channel, on_count);
}
