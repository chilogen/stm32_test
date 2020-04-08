//
// Created by a on 4/1/20.
//

#include "main.h"

void BeepOn(){
    HAL_GPIO_WritePin(BEEP,GPIO_PIN_SET);
}

void BeepOff(){
    HAL_GPIO_WritePin(BEEP,GPIO_PIN_RESET);
}
