/*!
 *  @version 1.0
 *  @file    pwm.h
 *  @date    2022-2023
 *  @author  Julien PISTRE
 *  @brief   Fichier d'entête de la librairie PWM STM32
 */

#ifndef TURBINE_PWM_H
#define TURBINE_PWM_H

#include "stm32l432xx.h"
#include "stm32l4xx_hal.h"


#define PWM_MIN  0
#define PWM_MAX  4095
#define PWM_ON_CYCLE 0.7f

#define PWM_ERR_START               0x01
#define PWM_ERR_STOP                0x02
#define PWM_ERR_COUNT_TOO_LOW       0x03
#define PWM_ERR_COUNT_TOO_HIGH      0x04
#define PWM_ERR_DUTY_CYCLE_TOO_LOW  0x05
#define PWM_ERR_DUTY_CYCLE_TOO_HIGH 0x06

int PWM_start_timer(TIM_HandleTypeDef *timer, uint32_t channel);
int PWM_stop_timer(TIM_HandleTypeDef *timer, uint32_t channel);

int PWM_on(TIM_HandleTypeDef *timer, uint32_t channel);
int PWM_off(TIM_HandleTypeDef *timer, uint32_t channel);
int PWM_set_count(TIM_HandleTypeDef *timer, uint32_t channel, uint16_t count);
int PWM_set_cycle(TIM_HandleTypeDef *timer, uint32_t channel, float duty_cycle);

#endif //TURBINE_PWM_H
