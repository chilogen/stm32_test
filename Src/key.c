//
// Created by a on 1/3/20.
//

#include "key.h"

uint8_t ReadKey(){
    if(!KEY0ACTIVE||!KEY1ACTIVE||!KEY2ACTIVE||!KEYUPACTIVE){
        return -1;
    }
    HAL_Delay(10);
    if(KEY0ACTIVE){
        return 0;
    }
    if(KEY1ACTIVE){
        return 1;
    }
    if(KEY2ACTIVE){
        return 2;
    }
    if(KEYUPACTIVE){
        return 3;
    }
}