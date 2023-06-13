/*!
 *  @version 1.0
 *  @file    edf30.h
 *  @date    2022-2023
 *  @author  Julien PISTRE
 *  @brief   Fichier d'entête de la librairie pour la turbine EDF30
 */

#ifndef TURBINE_EDF30_H
#define TURBINE_EDF30_H

#include "stm32l432xx.h"
#include "stm32l4xx_hal.h"


#define EDF30_PWM_MIN 0
#define EDF30_PWM_MAX 4095
#define EDF30_CHANNEL TIM_CHANNEL_1

#define EDF30_ERR_INIT 0x01
#define EDF30_ERR_START 0x02
#define EDF30_ERR_COUNT_TOO_LOW 0x03
#define EDF30_ERR_COUNT_TOO_HIGH 0x04
#define EDF30_ERR_DUTY_CYCLE_TOO_LOW 0x05
#define EDF30_ERR_DUTY_CYCLE_TOO_HIGH 0x06

int EDF30_start(TIM_HandleTypeDef *timer);
int EDF30_stop(TIM_HandleTypeDef *timer);

int EDF30_set_pwm(TIM_HandleTypeDef *timer, uint16_t count);
int EDF30_set_cycle(TIM_HandleTypeDef *timer, float duty_cycle);

#endif //TURBINE_EDF30_H
