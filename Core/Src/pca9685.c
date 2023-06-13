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

	if (reg < 0 || reg > 255)
		return;

	if (reg > 69 && reg < 250)
		return;

	uint8_t mess[5];
	mess[0] = reg;
	memcpy(&mess[1], data, data_len);

	HAL_I2C_Master_Transmit(i2c, PCA_I2C_ADDR, mess, data_len+1, PCA_I2C_TIMEOUT);
}


void PCA9685_init(I2C_HandleTypeDef *i2c) {
	uint8_t mode1_default_val = 0b00110000;
	uint8_t mode2_default_val = 0b00000000;

	uint8_t prescaler_val = (uint8_t) roundf(25000000.0f / (4096 * PCA_PRESCALER_FREQ)) - 1;

	PCA9685_write(i2c, PCA_REG_MODE1, mode1_default_val);
	PCA9685_write(i2c, PCA_REG_MODE2, mode2_default_val);
	PCA9685_write(i2c, PCA_PRESCALER_REG, prescaler_val);

	uint8_t data[4] = {0x00, 0x00, 0x00, 0x10};
	PCA9685_write_data(i2c, PCA_REG_ALL_ON_L, data, 4);
}


void PCA9685_set_pwm(I2C_HandleTypeDef *i2c, uint8_t channel, uint16_t on_count, uint16_t off_count) {
	if (on_count < 0 || on_count > 4095)
		return;

	if (off_count < 0 || off_count > 4095)
		return;

	PCA9685_write_data(i2c, 0x06, {on_count & 0xf0}, 2);
	PCA9685_write(i2c, 0x07, on_count & 0xf0);
	PCA9685_write(i2c, 0x08, off_count & 0x0f);
	PCA9685_write(i2c, 0x09, off_count & 0xf0);
}


void PCA9685_set_cycle(I2C_HandleTypeDef *i2c, uint8_t channel, float duty_cycle) {
	if (duty_cycle < 0 || duty_cycle > 1)
		return;

	uint16_t on_count = (uint8_t) roundf(duty_cycle*4096);
	PCA9685_set_pwm(i2c, channel, on_count, 1-on_count);
}
