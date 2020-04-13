//
// Created by a on 4/1/20.
//

#include "main.h"

extern UART_HandleTypeDef huart2;
extern uint8_t UART2DATAREADY;
extern uint8_t UART2RXBUFFER[UART_BUFFER_SIZE];
extern uint8_t UARTLOCK;

uint8_t readBufferCnt=0;

const char coapIpPort[]="49.4.85.232,5683";

const char NB_AT[] = "AT\r\n";
const char NB_OK[] = "OK";
const char NB_ATCSQ[] = "AT+CSQ\r\n";
const char NB_ATCFUN[] = "AT+CFUN=%s\r\n";
const char NB_ATAUTOCONNECT[] = "AT+NCONFIG=AUTOCONNECT,%s\r\n";
const char NB_ATNCDP[] = "AT+NCDP=%s\r\n";
const char NB_REBOOT[] = "AT+NRB\r\n";
const char NB_ATNBAND[] = "AT+NBAND=%s\r\n";
const char NB_ATCPSMS[] = "AT+CPSMS=%s\r\n";
const char NB_ATCEDRXS[] = "AT+CEDRXS=%s,%s\r\n";
const char NB_CGATT[] = "AT+CGATT=%s\r\n";
const char NB_CGATTq[] = "AT+CGATT?\r\n";
const char NB_NMGS[] = "AT+NMGS=%d,%s\r\n";
const char NB_NQMGR[]="AT+NQMGR\r\n";
const char NB_NMGR[]="AT+NMGR\r\n";
const char NB_NNMI[] = "AT+NNMI=%s\r\n";

const char __NB_FALSE[]="FALSE";
const char __NB_OK[]="OK";
const char __NB_TRUE[]="TRUE";
const char __NB_ONE[]="1";
const char __NB_ZERO[]="0";


uint8_t (*InitProc[])() ={
        __NB_MODULE_STATUS,
//        __NB_SIGNAL_QUALITY,
        __NB_Manual,
        __NB_SetNCDP,
        __NB_ModReboot,
        __NB_SetBand,
        __NB_OpenCFun,
        __NB_EdrxOff,
        __NB_PSMOff,
        __NB_NETATT,
        __NB_MANNUALREPORT,
        __NB_NETCONNECTED,
};

uint8_t (*CloseProc[])()={
        __NB_NETCLOSE,
        __NB_CloseCFun,
};

uint8_t (*ReconnectProc[])()={
        __NB_CloseCFun,
        __NB_ModReboot,
        __NB_MANNUALREPORT,
        __NB_OpenCFun,
        __NB_NETATT,
        __NB_NETCONNECTED,
};

uint8_t (*ConnectProc[])()={
        __NB_OpenCFun,
        __NB_NETATT,
        __NB_NETCONNECTED,
};

uint8_t NBInit() {
    unsigned int it = 0, procNum=sizeof InitProc;
    procNum=procNum/4;
    for (it = 0; it < procNum; it++) {
        if (!InitProc[it]()) {
            NBERROR(it);
            it--;
        }
    }
    return 1;
}

uint8_t NBConnect(){

    unsigned int it = 0, procNum=sizeof ConnectProc;
    procNum=procNum/4;
    for (it = 0; it < procNum; it++) {
        if (!ConnectProc[it]()) {
            NBERROR(it);
            it--;
        }
    }
    return 1;
}

uint8_t NBReconnect(){

    unsigned int it = 0, procNum=sizeof ReconnectProc;
    procNum=procNum/4;
    for (it = 0; it < procNum; it++) {
        HAL_Delay(500);
        if (!ReconnectProc[it]()) {
            NBERROR(it);
            it--;
        }
    }
    return 1;
}

uint8_t NBClose(){
    unsigned int it = 0, procNum=sizeof CloseProc;
    procNum=procNum/4;
    for (it = 0; it < procNum; it++) {
        if (!CloseProc[it]()) {
            NBERROR(it);
            it--;
        }
    }
    return 1;
}

void NBCommand(uint8_t *com, uint8_t com_size, char *res, uint8_t *res_size) {
    while(UARTLOCK){
        HAL_Delay(500);
    }
    UARTLOCK=1;
    memset(UART2RXBUFFER, 0, sizeof UART2RXBUFFER);
    HAL_UART_Transmit(&huart2, com, com_size - 1, 0xffff);
    HAL_UART_Receive_DMA(&huart2, UART2RXBUFFER, UART_BUFFER_SIZE);
    HAL_Delay(500);
    while (!UART2DATAREADY) {
        HAL_Delay(500);
    }
    memcpy(res, UART2RXBUFFER, UART2DATAREADY);
    *res_size = UART2DATAREADY;
    UART2DATAREADY = 0;
    UARTLOCK=0;
}

void NBERROR(uint8_t errno) {
    return;
    SetBeep(1);
    HAL_Delay(500);
    SetBeep(0);

    HAL_Delay(500);
    uint8_t it;
    for (it = 0; it < errno; it++) {
        SetBeep(1);
        HAL_Delay(100);
        SetBeep(0);
    }

    HAL_Delay(1000);
}

/*
 * REQUEST:  AT
 * RESPONSE: OK
 */
uint8_t __NB_MODULE_STATUS() {
    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    uint8_t res_size;
    NBCommand((uint8_t *)NB_AT, sizeof NB_AT, resBuff, &res_size);
    char *strx = NULL;
    strx = strstr((const char *) resBuff, __NB_OK);
    if (strx == NULL) {
        return 0;
    }
    return 1;
}

/*
 * REQUEST:  AT+CSQ
 * RESPONSE: +CSQ:a,b
 * a: rssi 0~31  99 indicate no signal
 * b: error rate
 */
uint8_t __NB_SIGNAL_QUALITY() {
    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    uint8_t res_size;
    NBCommand((uint8_t *)NB_ATCSQ, sizeof NB_ATCSQ, resBuff, &res_size);
    char *strx = NULL;
    strx = strstr((const char *) resBuff, (const char *) "+CSQ");
    if (strx == NULL) {
        return 0;
    }

    int rssi, erate;

    strx=find1stNum(strx,strx+0xffff,&rssi);
    find1stNum(strx,strx+0xffff,&erate);
    // no signal
    if (rssi == 99 || erate == 99) {
        return 0;
    }

    // signal weak
    if (rssi >= 31) {
        return 0;
    }
    return 1;
}

/*
 * AT+NCONFIG=AUTOCONNECT,FALSE
 * return OK
 */
uint8_t __NB_Manual() {
    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    char com[30];
    memset(com, 0, sizeof com);
    sprintf(com, (const char *)NB_ATAUTOCONNECT, __NB_FALSE);
    uint8_t res_size;
    NBCommand((uint8_t*)com, strlen(com), resBuff, &res_size);

    char *strx = NULL;
    strx = strstr((const char *) resBuff, NB_OK);

    if (strx == NULL) {
        return 0;
    }
    return 1;
}

uint8_t __NB_SetNCDP() {
    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    char com[30];
    memset(com, 0, sizeof com);
    sprintf(com, (const char *)NB_ATNCDP, coapIpPort);
    uint8_t res_size;
    NBCommand((uint8_t*)com, strlen(com), resBuff, &res_size);

    char *strx = NULL;
    strx = strstr((const char *) resBuff, NB_OK);

    if (strx == NULL) {
        return 0;
    }
    return 1;
}

uint8_t __NB_ModReboot() {
    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    uint8_t res_size;
    NBCommand((uint8_t*)NB_REBOOT, sizeof NB_REBOOT, resBuff, &res_size);
    HAL_Delay(5000);

//    char *strx=NULL;
//    strx = strstr((const char *) resBuff, NB_OK);
//
//    if (strx == NULL) {
//        return 0;
//    }
    return 1;
}

uint8_t __NB_SetBand() {
    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    char com[30];
    memset(com, 0, sizeof com);
    sprintf(com, NB_ATNBAND, "8");
    uint8_t res_size;
    NBCommand((uint8_t *)com, strlen(com), resBuff, &res_size);

    char *strx=NULL;
    strx = strstr((const char *) resBuff, NB_OK);

    if (strx == NULL) {
        return 0;
    }
    return 1;
}

uint8_t __NB_OpenCFun() {
    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    char com[30];
    memset(com, 0, sizeof com);
    sprintf(com, NB_ATCFUN, __NB_ONE);
    uint8_t res_size;
    NBCommand((uint8_t *)com, strlen(com), resBuff, &res_size);

    char *strx = NULL;
    strx = strstr((const char *) resBuff, NB_OK);

    if (strx == NULL) {
        return 0;
    }
    return 1;
}

uint8_t __NB_CloseCFun() {
    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    char com[30];
    memset(com, 0, sizeof com);
    sprintf(com, NB_ATCFUN, __NB_ZERO);
    uint8_t res_size;
    NBCommand((uint8_t *)com, strlen(com), resBuff, &res_size);

    char *strx = NULL;
    strx = strstr((const char *) resBuff, NB_OK);

    if (strx == NULL) {
        return 0;
    }
    return 1;
}

uint8_t __NB_EdrxOff() {
    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    char com[30];
    memset(com, 0, sizeof com);
    sprintf(com, NB_ATCEDRXS, __NB_ZERO, "5");
    uint8_t res_size;
    NBCommand((uint8_t *) com, strlen(com), resBuff, &res_size);

    char *strx = NULL;
    strx = strstr((const char *) resBuff, NB_OK);

    if (strx == NULL) {
        return 0;
    }
    return 1;
}

uint8_t __NB_PSMOff() {
    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    char com[30];
    memset(com, 0, sizeof com);
    sprintf(com, NB_ATCPSMS, __NB_ZERO);
    uint8_t res_size;
    NBCommand((uint8_t *)com, strlen(com), resBuff, &res_size);

    char *strx = NULL;
    strx = strstr((const char *) resBuff, NB_OK);

    if (strx == NULL) {
        return 0;
    }
    return 1;
}

uint8_t __NB_NETATT() {
    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    uint8_t res_size;
    char com[30];
    memset(com, 0, sizeof com);
    sprintf(com, NB_CGATT, __NB_ONE);
    NBCommand((uint8_t*)com, strlen(com), resBuff, &res_size);

    char *strx = NULL;
    strx = strstr((const char *) resBuff, NB_OK);

    if (strx == NULL) {
        __NB_OpenCFun();
        return 0;
    }
    return 1;
}

uint8_t __NB_NETCONNECTED(){
    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    uint8_t res_size;
    HAL_Delay(5000);
    NBCommand((uint8_t*)NB_CGATTq, sizeof NB_CGATTq, resBuff, &res_size);

    char *strx=NULL;
    strx = strstr((const char *) resBuff, __NB_ONE);

    if (strx == NULL) {
        return 0;
    }
    return 1;
}

uint8_t __NB_NETCLOSE(){
    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    uint8_t res_size;
    char com[30];
    memset(com, 0, sizeof com);
    sprintf(com, NB_CGATT, __NB_ZERO);
    NBCommand((uint8_t*)com, strlen(com), resBuff, &res_size);

    char *strx=NULL;
    strx = strstr((const char *) resBuff, __NB_OK);

    if (strx == NULL) {
        return 0;
    }
    return 1;
}

uint8_t __NB_AUTOREPORT(){

    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    uint8_t res_size;
    char com[30];
    memset(com, 0, sizeof com);
    sprintf(com, NB_NNMI, __NB_ONE);
    NBCommand((uint8_t*)com, strlen(com), resBuff, &res_size);

    return 1;
}

uint8_t __NB_MANNUALREPORT(){

    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    uint8_t res_size;
    char com[30];
    memset(com, 0, sizeof com);
    sprintf(com, NB_NNMI, __NB_ZERO);
    NBCommand((uint8_t*)com, strlen(com), resBuff, &res_size);

    return 1;
}

uint8_t NBSendMsg(char * data,int length){
    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    uint8_t res_size;
    char com[50],d[20];
    memset(com, 0, sizeof com);
    memset(d,0,length*2);
    memcpy(d,data,length*2);
    sprintf(com, NB_NMGS, length, (const char*)d);
    NBCommand((uint8_t*)com, strlen(com), resBuff, &res_size);

    char *strx=NULL;
    strx = strstr((const char *) resBuff, __NB_OK);

    if (strx == NULL) {
        return 0;
    }
    return 1;
}

uint8_t NBReadMsg(char **data){
    char resBuff[UART_BUFFER_SIZE];
    memset(resBuff,0,sizeof resBuff);
    uint8_t res_size;
    NBCommand((uint8_t *)NB_NQMGR, sizeof NB_NQMGR, resBuff, &res_size);
    char *strx = NULL;
    strx = strstr((const char *) resBuff, __NB_OK);
    if (strx == NULL) {
        return 0;
    }

    strx = strstr((const char *) resBuff,"RECEIVED");
    if (strx == NULL) {
        return 0;
    }

    int received,it,msgLength;

    find1stNum(strx,strx+0xffff,&received);
    if(received==readBufferCnt){
        return 0;
    }

    *data=(char *)malloc((received-readBufferCnt)*50);
    char *tmp=*data;
    for(it=0;it<received-readBufferCnt;it++){
        memset(resBuff,0,sizeof resBuff);
        NBCommand((uint8_t*)NB_NMGR,sizeof NB_NMGR,resBuff,&res_size);
        strx = strstr((const char *) resBuff, __NB_OK);
        if (strx == NULL) {
            readBufferCnt=received;
            return 0;
        }
        strx=find1stNum(resBuff,resBuff+0xffff,&msgLength);
        memcpy(tmp,strx-1,2+msgLength*2);
        tmp+=(2+msgLength*2);
    }
    char end_flag[]="command_end";
    memcpy(tmp,end_flag,sizeof end_flag);
    readBufferCnt=received;
    return 1;
}