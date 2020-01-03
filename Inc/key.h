//
// Created by a on 1/3/20.
//

#ifndef STM32_TEST_KEY_H
#define STM32_TEST_KEY_H

#include "main.h"

#define KEY0 GPIOE,GPIO_PIN_2
#define KEY1 GPIOE,GPIO_PIN_3
#define KEY2 GPIOE,GPIO_PIN_4
#define KEYUP GPIOA,GPIO_PIN_0

#define KEY0ACTIVE (HAL_GPIO_ReadPin(KEY0)==GPIO_PIN_RESET)
#define KEY1ACTIVE (HAL_GPIO_ReadPin(KEY1)==GPIO_PIN_RESET)
#define KEY2ACTIVE (HAL_GPIO_ReadPin(KEY2)==GPIO_PIN_RESET)
#define KEYUPACTIVE (HAL_GPIO_ReadPin(KEYUP)==GPIO_PIN_SET)

uint8_t ReadKey();

#endif //STM32_TEST_KEY_H
