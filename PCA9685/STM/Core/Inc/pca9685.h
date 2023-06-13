/*!
 *  @version 1.0
 *  @file     pca9685.c
 *  @date    2022-2023
 *  @author  Julien PISTRE
 *  @brief   Fichier d'entête pour contrôler un PCA9685
 */

#ifndef PCA9685_H
#define PCA9685_H

#include "stm32l4xx_hal.h"

// Registres et constantes (cycles de 20ms, clock à 25MHz)

#define PCA_I2C_ADDR        0x80   //  Adresse par défaut
#define PCA_I2C_TIMEOUT     1.0f   //  Durée du timeout
#define PCA_PRESCALER_FREQ  46.0f  //  Fréquence voulue

#define PCA_PWM_MIN_TIME    0.8f   // 205 pour un cycle de 20ms
#define PCA_PWM_MAX_TIME    2.2f   // 409 pour un cycle de 20ms
#define PCA_PWM_CYCLE_TIME  20.0f  // Fréquence de 50Hz

#define PCA_PWM_MAX         ((uint16_t) (PCA_PWM_MAX_TIME/(PCA_PWM_CYCLE_TIME/4096)))
#define PCA_PWM_MIN         ((uint16_t) (PCA_PWM_MIN_TIME/(PCA_PWM_CYCLE_TIME/4096)))
#define PCA_PWM_RANGE       (PCA_PWM_MAX - PCA_PWM_MIN)

#define PCA_REG_MODE1       0x00
#define PCA_REG_MODE2       0x01
#define PCA_REG_CHAN0_OFF_L 0x08
#define PCA_REG_ALL_ON_L    0xfa
#define PCA_REG_PRESCALER   0xfe

// Codes d'erreur

#define PCA_ERR_INIT_RESET      0x01
#define PCA_ERR_INIT_SLEEP      0x02
#define PCA_ERR_INIT_MODE2      0x03
#define PCA_ERR_INIT_PRESCALER  0x04
#define PCA_ERR_INIT_WAKEUP     0x05
#define PCA_ERR_CHAN_TOO_SMALL  0x06
#define PCA_ERR_CHAN_TOO_BIG    0x07
#define PCA_ERR_COUNT_TOO_SMALL 0x08
#define PCA_ERR_COUNT_TOO_BIG   0x09
#define PCA_ERR_CYCLE_TOO_SMALL 0x10
#define PCA_ERR_CYCLE_TOO_BIG   0x11
#define PCA_ERR_DATA_TOO_SMALL  0x12
#define PCA_ERR_DATA_TOO_BIG    0x13

// Signatures des fonctions publiques

int PCA9685_init(I2C_HandleTypeDef *i2c);
int PCA9685_turn_off(I2C_HandleTypeDef *i2c, uint8_t channel);
int PCA9685_set_pwm(I2C_HandleTypeDef *i2c, uint8_t channel, float points);
int PCA9685_set_cycle(I2C_HandleTypeDef *i2c, uint8_t channel, float duty_cycle);


#endif
