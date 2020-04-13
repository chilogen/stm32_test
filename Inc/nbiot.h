//
// Created by a on 4/1/20.
//

#ifndef STM32_TEST_NBIOT_H
#define STM32_TEST_NBIOT_H



uint8_t NBInit();
uint8_t NBClose();
uint8_t NBConnect();
uint8_t NBReconnect();
void NBCommand(uint8_t *com,uint8_t com_size, char res[UART_BUFFER_SIZE],uint8_t *res_size);
uint8_t NBSendMsg(char *,int);
uint8_t NBReadMsg(char **data);
void NBERROR(uint8_t);



uint8_t __NB_MODULE_STATUS();

uint8_t __NB_SIGNAL_QUALITY();

uint8_t __NB_Manual();

uint8_t __NB_SetNCDP();

uint8_t __NB_ModReboot();

uint8_t __NB_SetBand();

uint8_t __NB_OpenCFun();

uint8_t __NB_CloseCFun();

uint8_t __NB_EdrxOff();

uint8_t __NB_PSMOff();

uint8_t __NB_NETATT();

uint8_t __NB_NETCONNECTED();

uint8_t __NB_NETCLOSE();

uint8_t __NB_AUTOREPORT();

uint8_t __NB_MANNUALREPORT();

#endif //STM32_TEST_NBIOT_H
