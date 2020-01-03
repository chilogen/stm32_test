//
// Created by a on 4/1/20.
//

#include "main.h"
#include "string.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;

uint8_t UART2DATAREADY = 0;
uint8_t UART2RXBUFFER[UART_BUFFER_SIZE];

void USER_UART_IRQHandler(UART_HandleTypeDef *huart) {
    if (huart == &huart2) {
        if (RESET != __HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE)) {
            __HAL_UART_CLEAR_IDLEFLAG(&huart2);
            HAL_UART_DMAStop(&huart2);
            uint8_t UART2DATAREADY = UART_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
////            HAL_UART_Receive(&huart2, UART2RXBUFFER, length, 2);
//            HAL_UART_Receive_DMA(&huart2, UART2RXBUFFER, UART_BUFFER_SIZE);
//            UART2DATAREADY = 1;
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    HAL_UART_Receive_DMA(&huart2, UART2RXBUFFER, UART_BUFFER_SIZE);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
    HAL_UART_Transmit_DMA(&huart2, UART2RXBUFFER, UART_BUFFER_SIZE);
}