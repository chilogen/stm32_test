//
// Created by a on 4/1/20.
//

#ifndef STM32_TEST_NBIOT_H
#define STM32_TEST_NBIOT_H



uint8_t NBInit();
void NBCommand(uint8_t *com,uint8_t com_size, char res[UART_BUFFER_SIZE],uint8_t *res_size);
void NBGetMsg(char *,uint8_t);
void NBSendMsg(char *,uint8_t);
void NBERROR(uint8_t);

#endif //STM32_TEST_NBIOT_H
