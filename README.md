# Carte PCA9685

La carte PCA9685 permet de contrôler jusqu'à 16 sorties PWM par I2C.<br>

Quelques détails importants sur la carte :
- Clock interne à 25MHz
- Compteur 12 bits (0 à 4095)
- Fréquence du bus I2C : 100kHz
- Résistances de pull-up externes : 1 kΩ
- Output Enable (OE) à 0 pour activer les sorties

## Initialisation

L'adresse par défaut de la carte est 0x40, après avoir désactivé toutes les sorties, on définit les registres suivants :

- Mode 1 (Registre 0x00) : 0b00010000

| Bit | Symbole | Description                                            | Valeur |
|-----|---------|--------------------------------------------------------|--------|
| 7   | RESTART | Redémarrage activé                                     | 0      |
| 6   | EXTCLK  | Utilisation d'un clock externe                         | 0      |
| 5   | AI      | Ecriture des registres avec incrémentation automatique | 0      |
| 4   | SLEEP   | Mode veille, activé pour définir le prescaler          | 1      |
| 3   | SUB1    | La carte ne réagit pas à la sous-adresse 1             | 0      |
| 2   | SUB2    | La carte ne réagit pas à la sous-adresse 2             | 0      |
| 1   | SUB3    | La carte ne réagit pas à la sous-adresse 3             | 0      |
| 0   | ALLCALL | La carte ne réagit pas aux appels All Call             | 0      |

- Mode 2 (Registre 0x01) : 0b00000000

| Bit   | Symbole | Description                                  | Valeur |
|-------|---------|----------------------------------------------|--------|
| 7 - 5 | -       | Réservés                                     | 000    |
| 4     | INVRT   | L'état logique des sorties n'est pas inversé | 0      |
| 3     | OCH     | Voir page 16 de la datasheet                 | 0      |
| 2     | OUTDRV  | Les sorties sont en mode open-drain          | 0      |
| 1 - 0 | OUTNE0  | Voir page 16 de la datasheet                 | 0      |

- Prescaler (Registre 0xFE) : 0x79

On calcule le diviseur pour avoir une fréquence de 50Hz :

<img src="https://latex.codecogs.com/gif.latex?D=\frac{25MHz}{4096\times0x79}-1=50Hz" title="O_t=\frac{25MHz}{4096\times(0x79+1)}=50Hz" />

⚠️ Il faut ensuite désactiver le mode veille pour pouvoir contrôler les sorties PWM.

## Contrôle des sorties

Pour contrôler une sortie, il faut d'abord modifier certaines valeurs dans `PCA9685.h` :

| Servo-moteur | PCA_PRESCALER_FREQ | PCA_PWM_MIN_TIME | PCA_PWM_MAX_TIME |
|--------------|--------------------|------------------|------------------|
| Analogique   | 50Hz               | 1.0ms            | 2.0ms            |
| Numérique    | 125Hz              | 0.5ms            | 2.5ms            |
| Numérique    | 250Hz              | 0.266ms          | 0.533ms          |

Les comptes sont alors automatiquement calculés et stockés dans `PCA9685.h`.<br>
Après avoir initialisé la carte avec `PCA9685_init()`, on peut contrôler les sorties :

```c
// Sortie 0 à 100% dans un sens (pour f=50Hz)
PCA9685_set_pwm(&hi2c1, 0, 200);

// Sortie 0 à zéro
PCA9685_set_cycle(&hi2c1, 0, 0.5);

// Désactiver la sortie 0
PCA9685_turn_off(&hi2c1, 0);
```