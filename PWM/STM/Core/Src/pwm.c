/*!
 *  @version 1.0
 *  @file    pwm.h
 *  @date    2022-2023
 *  @author  Julien PISTRE
 *  @brief   Code source de la librairie PWM STM32
 */

#include "pwm.h"


/*!
 *  @brief Démarrer le timer pour générer le PWM
 *  @param i2c Généralement &htim1 (structure d'STM du timer configuré)
 *  @return Code d'erreur
 */
int PWM_start_timer(TIM_HandleTypeDef *timer, uint32_t channel) {
    if (HAL_TIM_PWM_Start(timer, channel) != HAL_OK)
        return EDF30_ERR_START;

    return 0;
}


/*!
 *  @brief Arrêter le timer qui génère le PWM
 *  @param i2c Généralement &htim1 (structure d'STM du timer configuré)
 *  @return Code d'erreur
 */
int PWM_stop_timer(TIM_HandleTypeDef *timer, uint32_t channel) {
    if (HAL_TIM_PWM_Stop(timer, channel) != HAL_OK)
        return EDF30_ERR_STOP;

    return 0;
}


/*!
 * @brief Activer la turbine (cycle de travail à EDF30_ON_CYCLE)
 *  @param timer Généralement &htim1 (structure d'STM du timer configuré)
 *  @return Code d'erreur
 */
int PWM_on(TIM_HandleTypeDef *timer, uint32_t channel) {
    return PWM_set_cycle(timer, channel, EDF30_ON_CYCLE);
}


/*!
 * @brief Désactiver la turbine (cycle de travail à EDF30_OFF_CYCLE)
 * @param timer Généralement &htim1 (structure d'STM du timer configuré)
 * @return
 */
int PWM_off(TIM_HandleTypeDef *timer, uint32_t channel) {
    return PWM_set_cycle(timer, channel, 0);
}


/*!
 *  @brief Définir directement le cycle de travail du PWM
 *  @param timer Généralement &htim1 (structure d'STM du timer configuré)
 *  @param count Valeur du compteur (0 à EDF30_PWM_MAX)
 *  @return Code d'erreur
 */
int PWM_set_count(TIM_HandleTypeDef *timer, uint32_t channel, uint16_t count) {
    if (count < EDF30_PWM_MIN) return EDF30_ERR_COUNT_TOO_LOW;
    if (count > EDF30_PWM_MAX) return EDF30_ERR_COUNT_TOO_HIGH;

    switch (channel) {
        case TIM_CHANNEL_1: timer->Instance->CCR1 = count; break;
        case TIM_CHANNEL_2: timer->Instance->CCR2 = count; break;
        case TIM_CHANNEL_3: timer->Instance->CCR3 = count; break;
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
int PWM_set_cycle(TIM_HandleTypeDef *timer, uint32_t channel, float duty_cycle) {
    if (duty_cycle < 0) return EDF30_ERR_DUTY_CYCLE_TOO_LOW;
    if (duty_cycle > 1) return EDF30_ERR_DUTY_CYCLE_TOO_HIGH;

    return PWM_set_count(timer, channel, (uint16_t) (duty_cycle * EDF30_PWM_MAX));
}
