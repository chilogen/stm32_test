//
// Created by a on 4/9/20.
//

#include "main.h"

extern uint8_t resouce_state[3];


void msgEncode(HWOCMSG *src, char **dst,int *dstLen){
    uint8_t it;
    int len;
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
    *dstLen=len;

    for(it=0;it<len;it++){
        (*dst)[it<<1]='0'+(src->data[it]>>4);
        (*dst)[(it<<1)+1]='0'+(src->data[it]&0x0f);
    }
}

void msgDecode(char *src,uint8_t len, HWOCMSG *dst) {
    uint8_t it;
    dst->data = (uint8_t *) malloc(len >> 1);
    for (it = 0; it < len; it += 2) {
        dst->data[it >> 1] = ((src[it] - '0') << 4) ^ (src[it + 1] - '0');
    }
    dst->msgId = *(uint8_t * )(dst->data);
}

void HWCommand(){
    char *data=NULL,*strx=NULL;
    char tmpBuffer[20];
    HWOCMSG msg1;
    HWOCSetMSG msg2;
    uint8_t it;
    NBReadMsg(&data);
    if(data==NULL){
        return;
    }

    strx=strstr(data,"command_end");
    if(strx==NULL){
        free(data);
        return ;
    }

    int length,d;
    while(data!=NULL) {
        data = find1stNum(data, strx, &length);
        msgDecode(data+1,length*2,&msg1);
        switch(msg1.msgId){
            case 1:{
                HWReport();
                break;
            }
            case 0:{
                memcpy(&msg2,msg1.data,sizeof msg2);
                resouce_state[msg2.num]=msg2.state;
                break;
            }
        }
    }
    free(data);
}

void HWReport(){
    HWOCReportMSG msg1={
            2,
            resouce_state[0],
            resouce_state[1],
            resouce_state[2],
    };

    HWOCMSG msg2={
            2,
    };
    msg2.data=malloc(sizeof msg1);
    memcpy(msg2.data,&msg1,sizeof msg1);

    char *data;
    int len;
    msgEncode(&msg2,&data,&len);
    NBSendMsg(data,len);
    free(data);
}