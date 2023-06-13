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

// Registres et constantes

#define PCA_I2C_ADDR        0x80   //          Adresse par défaut
#define PCA_I2C_TIMEOUT     1      //          Durée du timeout
#define PCA_PRESCALER_FREQ  46.0f  //          Fréquence voulue

#define PCA_PWM_ZERO        0x133  // 1ms      Comptes calculés pour
#define PCA_PWM_MAX         0x199  // 1.5ms    des cycles de 20ms et
#define PCA_PWM_MIN         0xcd   // 2ms      un compteur 12 bits

#define PCA_REG_MODE1       0x00   //          Regarder la datasheet
#define PCA_REG_MODE2       0x01   //          page 10 à 13 pour une
#define PCA_REG_CHAN0_OFF_L 0x08   //          liste complète de tous
#define PCA_REG_ALL_ON_L    0xfa   //          les registres
#define PCA_REG_PRESCALER   0xfe   //

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
int PCA9685_set_pwm(I2C_HandleTypeDef *i2c, uint8_t channel, uint16_t on_count);
int PCA9685_set_cycle(I2C_HandleTypeDef *i2c, uint8_t channel, float duty_cycle);


#endif
