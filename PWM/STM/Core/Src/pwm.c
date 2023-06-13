/*!
 *  @version 1.0
 *  @file    pwm.h
 *  @date    2022-2023
 *  @author  Julien PISTRE
 *  @brief   Code source de la librairie PWM STM32
 */

#include "pwm.h"
extern TIM_HandleTypeDef htim1;


/*!
 *  @brief Démarrer le timer pour générer le PWM
 *  @param i2c Généralement &htim1 (structure d'STM du timer configuré)
 *  @return Code d'erreur
 */
int PWM_start_timer(uint32_t channel) {
    if (HAL_TIM_PWM_Start(&htim1, channel) != HAL_OK)
        return PWM_ERR_START;

    return 0;
}


/*!
 *  @brief Arrêter le timer qui génère le PWM
 *  @param i2c Généralement &htim1 (structure d'STM du timer configuré)
 *  @return Code d'erreur
 */
int PWM_stop_timer(uint32_t channel) {
    if (HAL_TIM_PWM_Stop(&htim1, channel) != HAL_OK)
        return PWM_ERR_STOP;

    return 0;
}


/*!
 * @brief Activer la turbine (cycle de travail à PWM_ON_CYCLE)
 *  @param timer Généralement &htim1 (structure d'STM du timer configuré)
 *  @return Code d'erreur
 */
int PWM_on(uint32_t channel) {
    return PWM_set_cycle(channel, PWM_ON_CYCLE);
}


/*!
 * @brief Désactiver la turbine (cycle de travail à EDF30_OFF_CYCLE)
 * @param timer Généralement &htim1 (structure d'STM du timer configuré)
 * @return
 */
int PWM_off(uint32_t channel) {
    return PWM_set_cycle(channel, 0);
}


/*!
 *  @brief Définir directement le cycle de travail du PWM
 *  @param timer Généralement &htim1 (structure d'STM du timer configuré)
 *  @param count Valeur du compteur (0 à PWM_MAX)
 *  @return Code d'erreur
 */
int PWM_set_count(uint32_t channel, uint16_t count) {
    if (count < PWM_MIN) return PWM_ERR_COUNT_TOO_LOW;
    if (count > PWM_MAX) return PWM_ERR_COUNT_TOO_HIGH;

    switch (channel) {
        case TIM_CHANNEL_1: htim1.Instance->CCR1 = count; break;
        case TIM_CHANNEL_2: htim1.Instance->CCR2 = count; break;
        case TIM_CHANNEL_3: htim1.Instance->CCR3 = count; break;
        default: break;
    }

    return 0;
}


/*!
 *  @brief Définir le cycle de travail du PWM en pourcentage
 *  @param timer Généralement &htim1 (structure d'STM du timer configuré)
 *  @param duty_cycle Cycle de travail (0 à 1)
 *  @return Code d'erreur
 */
int PWM_set_cycle(uint32_t channel, float duty_cycle) {
    if (duty_cycle < 0) return PWM_ERR_DUTY_CYCLE_TOO_LOW;
    if (duty_cycle > 1) return PWM_ERR_DUTY_CYCLE_TOO_HIGH;

    return PWM_set_count(channel, (uint16_t) (duty_cycle * PWM_MAX));
}
