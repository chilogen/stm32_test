//
// Created by a on 4/1/20.
//

#include "main.h"

void SetBeep(uint8_t state){
    switch (state){
        case 0:{
            HAL_GPIO_WritePin(BEEP,GPIO_PIN_RESET);
            break;
        }
        case 1:{
            HAL_GPIO_WritePin(BEEP,GPIO_PIN_SET);
            break;
        }
        default:{}
    }
}