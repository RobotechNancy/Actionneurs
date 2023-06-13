#ifndef PCA9685_H
#define PCA9685_H

#include "stm32l4xx_hal.h"


#define PCA_I2C_ADDR    0x80
#define PCA_I2C_TIMEOUT 1

#define PCA_PRESCALER_FREQ  20.0f
#define PCA_PRESCALER_REG   0xfe

#define PCA_REG_MODE1     0x00
#define PCA_REG_MODE2     0x01
#define PCA_REG_CHAN0_L   0x06
#define PCA_REG_ALL_ON_L  0xfa


void PCA9685_init(I2C_HandleTypeDef *i2c);
void PCA9685_set_pwm(I2C_HandleTypeDef *i2c, uint8_t channel, uint16_t on_count, uint16_t off_count);
void PCA9685_set_cycle(I2C_HandleTypeDef *i2c, uint8_t channel, float duty_cycle);

#endif
