/*!
 *  @version 1.0
 *  @file    edf30.h
 *  @date    2022-2023
 *  @author  Julien PISTRE
 *  @brief   Code source de la librairie pour la turbine EDF30
 */

#include "edf30.h"


/*!
 *  @brief Démarrer le timer pour générer le PWM
 *  @param i2c Généralement &htim1 (structure d'STM du timer configuré)
 *  @return Code d'erreur
 */
int EDF30_init(TIM_HandleTypeDef *timer) {
    if (HAL_TIM_PWM_Start(timer, EDF30_CHANNEL) != HAL_OK)
        return EDF30_ERR_START;

    return 0;
}


/*!
 *  @brief Définir directement le cycle de travail du PWM
 *  @param timer Généralement &htim1 (structure d'STM du timer configuré)
 *  @param count Valeur du compteur (0 à EDF30_PWM_MAX)
 *  @return Code d'erreur
 */
int EDF30_set_pwm(TIM_HandleTypeDef *timer, uint16_t count) {
    if (count < EDF30_PWM_MIN) return EDF30_ERR_COUNT_TOO_LOW;
    if (count > EDF30_PWM_MAX) return EDF30_ERR_COUNT_TOO_HIGH;

    timer->Instance->CCR1 = count;
    return 0;
}


/*!
 *  @brief Définir le cycle de travail du PWM en pourcentage
 *  @param timer Généralement &htim1 (structure d'STM du timer configuré)
 *  @param duty_cycle Cycle de travail (0 à 1)
 *  @return Code d'erreur
 */
int EDF30_set_cycle(TIM_HandleTypeDef *timer, float duty_cycle) {
    if (duty_cycle < 0) return EDF30_ERR_DUTY_CYCLE_TOO_LOW;
    if (duty_cycle > 1) return EDF30_ERR_DUTY_CYCLE_TOO_HIGH;

    return EDF30_set_pwm(timer, (uint16_t) (duty_cycle*EDF30_PWM_MAX));
}
