//
// Created by a on 1/3/20.
//

#ifndef STM32_TEST_BEEP_H
#define STM32_TEST_BEEP_H

#include "main.h"

#define BEEP GPIOB,GPIO_PIN_8
#define BEEP_ON 0
#define BEEP_OFF 1

void SetBeepState(uint8_t state);

#endif //STM32_TEST_BEEP_H
