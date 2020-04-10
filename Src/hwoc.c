//
// Created by a on 4/9/20.
//

#include "main.h"

void msgEncode(HWOCMSG *src, char **dst){
    uint8_t it,len;
    switch(src->msgId){
        case 0:{
            len=3;
            break;
        }
        case 1:{
            len=2;
            break;
        }
        case 2:{
            len=4;
            break;
        }
    }

    *dst=(char *)malloc(len<<1);

    for(it=0;it<len;it++){
        (*dst)[it<<1]='0'+(src->data[it]>>4);
        (*dst)[(it<<1)+1]='0'+(src->data[it]&0x0f);
    }
}

void msgDecode(char *src, HWOCMSG *dst) {
    uint8_t len = strlen(src), it;
    dst->data = (uint8_t *) malloc(len >> 1);
    for (it = 0; it < len; it += 2) {
        dst->data[it >> 1] = ((src[it] - '0') << 4) ^ (src[it + 1] - '0');
    }
    dst->msgId = *(uint8_t * )(dst->data);
}
