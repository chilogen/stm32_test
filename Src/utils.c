//
// Created by a on 4/13/20.
//

#include "main.h"

char * find1stNum(char *st, char *et, int *result){
    if(result==NULL){
        return st;
    }
    if(st>=et-1){
        return NULL;
    }
    *result = 0;
    while(st!=et){
        if(*st>='0'&&*st<='9'){
            break;
        }
        st++;
    }
    while(st!=et){
        if(*st<'0'|*st>'9'){
            break;
        }
        *result=10*(*result);
        *result+=(*st-'0');
        st++;
    }
    return st;
}
