//
// Created by a on 1/3/20.
//

#include "beep.h"

void SetBeepState(uint8_t state){
    switch(state){
        case BEEP_ON:{
            HAL_GPIO_WritePin(BEEP,GPIO_PIN_RESET);
            break;
        }
        case BEEP_OFF:{
            HAL_GPIO_WritePin(BEEP,GPIO_PIN_SET);
            break;
        }
        default:{

        }
    }
};