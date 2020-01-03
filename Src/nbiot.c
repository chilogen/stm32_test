//
// Created by a on 4/1/20.
//

#include "main.h"
#include "string.h"

extern UART_HandleTypeDef huart2;
extern uint8_t UART2DATAREADY;
extern uint8_t UART2RXBUFFER[UART_BUFFER_SIZE];

uint8_t NB_AT[] = "AT\r\n";
uint8_t NB_OK[] = "OK\r\n";
uint8_t NB_ATCSQ[] = "AT+CSQ\r\n";
uint8_t NB_ATCFUN[] = "AT+CFUN=%s\r\n";;
uint8_t NB_ATCONFIG[] = "AT+NCONFIG=%s,%s\r\n";
uint8_t NB_ATNCDP[] = "AT+NCDP=%s,%s\r\n";
uint8_t NB_ATNBAND[] = "AT+NBAND=%s\r\n";
uint8_t NB_ATCPSMS[] = "AT+CPSMS=%s\r\n";
uint8_t NB_ATCEDRXS[] = "AT+CEDRXS=%s,%s\r\n";
uint8_t NB_CGATT[] = "AT+CGATT=%s\r\n";
uint8_t NB_CGATTq[] = "AT+CGATT?\r\n";

uint8_t __NB_MODULE_STATUS();

uint8_t __NB_SIGNAL_QUALITY();

uint8_t __NB_Manual();

uint8_t __NB_SetCDP();

uint8_t __NB_ModReboot();

uint8_t __NB_SetBand();

uint8_t __NB_OpenCFun();

uint8_t __NB_EdrxOff();

uint8_t __NB_PSMOff();

uint8_t __NB_NETATT();

uint8_t (*InitProc[])() ={
        __NB_MODULE_STATUS,
        __NB_SIGNAL_QUALITY,
        __NB_Manual,
        __NB_SetCDP,
        __NB_ModReboot,
        __NB_SetBand,
        __NB_SetBand,
        __NB_OpenCFun,
        __NB_EdrxOff,
        __NB_PSMOff,
        __NB_NETATT,
};

uint8_t NBInit() {
    unsigned int it = 0;
    for (it = 0; it < sizeof InitProc; it++) {
        if (!InitProc[it]()) {
            NBERROR(it);
        }
    }
    return 1;
}

void NBCommand(uint8_t *com, uint8_t com_size, char *res, uint8_t *res_size) {
    HAL_UART_Transmit(&huart2, com, com_size - 1, 0xfff);
    HAL_UART_Receive_DMA(&huart2, UART2RXBUFFER, UART_BUFFER_SIZE);
    while (!UART2DATAREADY) {
        HAL_Delay(50);
    }
    memcpy(res, UART2RXBUFFER, UART2DATAREADY);
    *res_size = UART2DATAREADY;
    UART2DATAREADY = 0;
//    uint8_t it = 0;
//    for (; it < 255; it++) {
//        if (tmpBuff[it] == '\r')continue;
//        if (tmpBuff[it] == '\n')continue;
//    }
//    memcpy(res, tmpBuff, UART_BUFFER_SIZE - it);
}

void NBERROR(uint8_t errno) {
    BeepOn();
    HAL_Delay(1000);
    BeepOff();


    uint8_t it;
    for (it = 0; it < errno; it++) {
        BeepOn();
        HAL_Delay(300);
        BeepOff();
    }
}

uint8_t __NB_MODULE_STATUS() {
    char resBuff[UART_BUFFER_SIZE];
    uint8_t res_size;
    NBCommand(NB_AT, sizeof NB_AT, resBuff, &res_size);
    char *strx = NULL;
    strx = strstr((const char *) resBuff, (const char *) "OK");
    if (strx == NULL) {
        return 0;
    }
    return 1;
}

uint8_t __NB_SIGNAL_QUALITY() {
    char resBuff[UART_BUFFER_SIZE];
    uint8_t res_size;
    NBCommand(NB_ATCSQ, sizeof NB_ATCSQ, resBuff, &res_size);

    return 0;
}

uint8_t __NB_Manual() {
    char resBuff[UART_BUFFER_SIZE];
    uint8_t res_size;
    NBCommand(NB_ATCSQ, sizeof NB_ATCSQ, resBuff, &res_size);

    return 0;
}

uint8_t __NB_SetCDP() {
    char resBuff[UART_BUFFER_SIZE];
    uint8_t res_size;
    NBCommand(NB_ATCSQ, sizeof NB_ATCSQ, resBuff, &res_size);

    return 0;
}

uint8_t __NB_ModReboot() {
    char resBuff[UART_BUFFER_SIZE];
    uint8_t res_size;
    NBCommand(NB_ATCSQ, sizeof NB_ATCSQ, resBuff, &res_size);

    return 0;
}

uint8_t __NB_SetBand() {
    char resBuff[UART_BUFFER_SIZE];
    uint8_t res_size;
    NBCommand(NB_ATCSQ, sizeof NB_ATCSQ, resBuff, &res_size);

    return 0;
}

uint8_t __NB_OpenCFun() {
    char resBuff[UART_BUFFER_SIZE];
    uint8_t res_size;
    NBCommand(NB_ATCSQ, sizeof NB_ATCSQ, resBuff, &res_size);

    return 0;
}

uint8_t __NB_EdrxOff() {
    char resBuff[UART_BUFFER_SIZE];
    uint8_t res_size;
    NBCommand(NB_ATCSQ, sizeof NB_ATCSQ, resBuff, &res_size);

    return 0;
}

uint8_t __NB_PSMOff() {
    char resBuff[UART_BUFFER_SIZE];
    uint8_t res_size;
    NBCommand(NB_ATCSQ, sizeof NB_ATCSQ, resBuff, &res_size);

    return 0;
}

uint8_t __NB_NETATT() {
    char resBuff[UART_BUFFER_SIZE];
    uint8_t res_size;
    NBCommand(NB_ATCSQ, sizeof NB_ATCSQ, resBuff, &res_size);

    return 0;
}
