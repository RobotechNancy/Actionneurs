# Turbines EDF30

Paramétrage de la clock :
- Clock utilisée : TIM1 (PA8)
- Channel 1 : PWM Generation CH1
- Fréquence : 4MHz (APB2)
- Prescaler : 8
- Counter Period : 4096

On obtient un signal d'environ 100Hz avec des cycles divisés en 4096 comptes. 

## Utilisation de la librairie

Si vous votre clock est configurée différemment, vous devez modifier les variables suivantes dans `edf30.h` :
- `EDF30_PWM_MAX ...` : Nombre de comptes maximum du timer
- `EDF30_CHANNEL TIM_CHANNEL_x` : Channel utilisé pour le PWM

La librairie contient les fonctions suivantes :
```c
/*!
 *  @brief Démarrer le timer pour générer le PWM
 *  @param i2c Généralement &htim1 (structure d'STM du timer configuré)
 *  @return Code d'erreur
 */
int EDF30_start(TIM_HandleTypeDef *timer);


/*!
 *  @brief Arrêter le timer qui génère le PWM
 *  @param i2c Généralement &htim1 (structure d'STM du timer configuré)
 *  @return Code d'erreur
 */
int EDF30_stop(TIM_HandleTypeDef *timer);


/*!
 *  @brief Définir directement le cycle de travail du PWM
 *  @param timer Généralement &htim1 (structure d'STM du timer configuré)
 *  @param count Valeur du compteur (0 à EDF30_PWM_MAX)
 *  @return Code d'erreur
 */
int EDF30_set_pwm(TIM_HandleTypeDef *timer, uint16_t count);


/*!
 *  @brief Définir le cycle de travail du PWM en pourcentage
 *  @param timer Généralement &htim1 (structure d'STM du timer configuré)
 *  @param duty_cycle Cycle de travail (0 à 1)
 *  @return Code d'erreur
 */
int EDF30_set_cycle(TIM_HandleTypeDef *timer, float duty_cycle);
```