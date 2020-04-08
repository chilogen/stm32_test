//
// Created by a on 4/1/20.
//

#include "main.h"

void SetLed0(uint8_t state) {
    switch (state) {
        case 1: {
            HAL_GPIO_WritePin(LED0, GPIO_PIN_RESET);
            break;
        }
        case 0: {
            HAL_GPIO_WritePin(LED0, GPIO_PIN_SET);
            break;
        }
        default: {
        }
    }
}

void SetLed1(uint8_t state){
    switch (state) {
        case 1: {
            HAL_GPIO_WritePin(LED1, GPIO_PIN_RESET);
            break;
        }
        case 0: {
            HAL_GPIO_WritePin(LED1, GPIO_PIN_SET);
            break;
        }
        default: {
        }
    }
}