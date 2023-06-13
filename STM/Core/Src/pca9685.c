/*!
 *  @version 1.0
 *  @file     pca9685.c
 *  @date    2022-2023
 *  @author  Julien PISTRE
 *  @brief   Code source pour contrôler un PCA9685
 */

#include <math.h>
#include "pca9685.h"


// Identique à Arduino : https://www.arduino.cc/reference/en/language/functions/math/map/
uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


/*!
 *  @brief Ecrire dans un seul registre (cf. page 32)
 *  @param i2c Généralement &hi2c1 (structure d'STM pour gérer l'I2C)
 *  @param reg L'adresse du registre sur lequel écrire
 *  @param val La valeur à écrire sur le registre
 *  @return Status HAL
 */
int PCA9685_write(I2C_HandleTypeDef *i2c, uint8_t reg, uint8_t val) {
	uint8_t data[2] = {reg, val};
	return HAL_I2C_Master_Transmit(i2c, PCA_I2C_ADDR, data, 2, PCA_I2C_TIMEOUT);
}


/*!
 *  @brief Ecrire dans des registres successifs
 *  @param i2c Généralement &hi2c1 (structure d'STM pour gérer l'I2C)
 *  @param reg L'adresse du premier registre sur lequel écrire
 *  @param data Les valeurs (octets) à écrire
 *  @param data_len Le nombre d'octets à écrire
 *  @return Status HAL ou code d'erreur
 */
int PCA9685_write_data(I2C_HandleTypeDef *i2c, uint8_t reg, uint8_t *data, uint8_t data_len) {
	if (data_len < 0) return PCA_ERR_DATA_TOO_SMALL;
	if( data_len > 4) return PCA_ERR_DATA_TOO_BIG;

    int status;

	for (uint8_t i=0; i < 4; i++)
		if ((status = PCA9685_write(i2c, reg, data[i])) != HAL_OK)
			return status;

	return HAL_OK;
}


/*!
 *  @brief Initialisation de la carte
 *  @param i2c Généralement &hi2c1 (structure d'STM pour gérer l'I2C)
 *  @return Si il y a eu une erreur pour l'écriture
 */
int PCA9685_init(I2C_HandleTypeDef *i2c) {
	// On désactive tous les channels (cf. page 25 registre FDh)
	uint8_t data[4] = {0x00, 0x00, 0x00, 0x10};
	if (PCA9685_write_data(i2c, PCA_REG_ALL_ON_L, data, 2) != HAL_OK)
		return PCA_ERR_INIT_RESET;

	// On active le sleep mode pour modifier le diviseur (cf. pages 13 et 14)
	if (PCA9685_write(i2c, PCA_REG_MODE1, 0b00010000) != HAL_OK)
		return PCA_ERR_INIT_SLEEP;

	// Cf. page 16
	if (PCA9685_write(i2c, PCA_REG_MODE2, 0b00000000) != HAL_OK)
		return PCA_ERR_INIT_MODE2;

	// Calcul du diviseur pour avoir la fréquence voulue (cf. page 25)
	uint8_t prescaler_val = (uint8_t) roundf(25000000.0f / (4096 * PCA_PRESCALER_FREQ)) - 1;
	if (PCA9685_write(i2c, PCA_REG_PRESCALER, prescaler_val) != HAL_OK)
		return PCA_ERR_INIT_PRESCALER;

	// On désactive le sleep mode pour pouvoir piloter les servos (cf. page 14)
	if (PCA9685_write(i2c, PCA_REG_MODE1, 0b00000000) != HAL_OK)
		return PCA_ERR_INIT_WAKEUP;

    // On attend au moins 500us pour que l'oscillateur se stabilise
	HAL_Delay(1);
	return 0;
}


/*!
 *  @brief Désactiver un channel
 *  @param i2c Généralement &hi2c1 (structure d'STM pour gérer l'I2C)
 *  @param channel Le channel à désactiver (0 à 15)
 *  @return Un code d'erreur
 */
int PCA9685_turn_off(I2C_HandleTypeDef *i2c, uint8_t channel) {
    if (channel < 0) return PCA_ERR_CHAN_TOO_SMALL;
    if (channel > 16) return PCA_ERR_CHAN_TOO_BIG;

    uint8_t data[2] = {0x00, 0x10};
    return PCA9685_write_data(i2c, PCA_REG_CHAN0_OFF_L + channel*4, data, 2);
}


/*!
 *  @brief Directement définir la valeur du PWM
 *  @param i2c Généralement &hi2c1 (structure d'STM pour gérer l'I2C)
 *  @param channel Le servomoteur à contrôler (0 à 15)
 *  @param points Le nombre de points ON (0 à PCA_PWM_RANGE)
 *  @return Un code d'erreur
 */
int PCA9685_set_pwm(I2C_HandleTypeDef *i2c, uint8_t channel, uint16_t points) {
	if (channel < 0) return PCA_ERR_CHAN_TOO_SMALL;
	if (channel > 16) return PCA_ERR_CHAN_TOO_BIG;

	uint16_t on_count = map(points, 0, PCA_PWM_RANGE, PCA_PWM_MIN, PCA_PWM_MAX);

	if (on_count > PCA_PWM_MAX) return PCA_ERR_COUNT_TOO_BIG;
	if (on_count < PCA_PWM_MIN) return PCA_ERR_COUNT_TOO_SMALL;

	uint8_t data[2] = {on_count & 0xff, (on_count >> 8) & 0xff};
	return PCA9685_write_data(i2c, PCA_REG_CHAN0_OFF_L + channel*4, data, 2);
}


/*!
 *  @brief Définir un cycle de service
 *  @param i2c Généralement &hi2c1 (structure d'STM pour gérer l'I2C)
 *  @param channel Le servomoteur à contrôler (0 à 15)
 *  @param duty_cycle La valeur du cycle (0 à 1)
 *  @return Un code d'erreur
 */
int PCA9685_set_cycle(I2C_HandleTypeDef *i2c, uint8_t channel, float duty_cycle) {
	if (channel < 0) return PCA_ERR_CHAN_TOO_SMALL;
	if (channel > 16) return PCA_ERR_CHAN_TOO_BIG;

	if (duty_cycle < 0) return PCA_ERR_CYCLE_TOO_SMALL;
	if (duty_cycle > 1) return PCA_ERR_CYCLE_TOO_BIG;

	uint16_t points = (uint16_t) (duty_cycle*PCA_PWM_RANGE);
	return PCA9685_set_pwm(i2c, channel, points);
}
