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

#define SERVO_MIN                   205
#define SERVO_90                    410
#define SERVO_MAX                   615
#define SERVO_BASKET_CHANNEL        2
#define SERVO_BALL_CHANNEL          3
#define TURBINE_CHANNEL             1

#define PWM_MIN                     0
#define PWM_MAX                     4095
#define PWM_ON_CYCLE                0.7f

#define PWM_ERR_START               0x01
#define PWM_ERR_STOP                0x02
#define PWM_ERR_COUNT_TOO_LOW       0x03
#define PWM_ERR_COUNT_TOO_HIGH      0x04
#define PWM_ERR_DUTY_CYCLE_TOO_LOW  0x05
#define PWM_ERR_DUTY_CYCLE_TOO_HIGH 0x06

int PWM_start_timer(uint32_t channel);
int PWM_stop_timer(uint32_t channel);

int PWM_on(uint32_t channel);
int PWM_off(uint32_t channel);
int PWM_set_count(uint32_t channel, uint16_t count);
int PWM_set_cycle(uint32_t channel, float duty_cycle);

#endif //TURBINE_PWM_H
