/*
 * main.c
 *
 *  Created on: Jan 11, 2021
 *      Author: huong
 */

#include "stm32f4xx_hal.h"
#include <string.h>

void SystemClockConfig(void);
void UART2_Init();
void Error_handler();

UART_HandleTypeDef huart2;
char * user_data = "The user data\r\n";

int main() {

    char buffer_receive[100];
    uint16_t number_char_receive = 0;
    char temp;


    HAL_Init();

    SystemClockConfig();

    UART2_Init();
    uint16_t string_length = strlen(user_data);
    if(HAL_UART_Transmit(&huart2, (uint8_t *)user_data, string_length, HAL_MAX_DELAY) != HAL_OK){
        Error_handler();
    }


    while(1){
        /*Receiving the data*/
        while(1){
            (void)HAL_UART_Receive(&huart2, (uint8_t *)&temp, 1, HAL_MAX_DELAY);
            if(temp == '\r'){
                buffer_receive[number_char_receive++] = '\n';
                break;
            }else{
                buffer_receive[number_char_receive] = temp;
                number_char_receive++;
            }
        }

        /*transferring the data was received*/
        if(HAL_UART_Transmit(&huart2, (uint8_t *)buffer_receive, number_char_receive, HAL_MAX_DELAY) != HAL_OK){
            Error_handler();
        }
        number_char_receive = 0;
    }
    return 0;
}/*main*/

/**
 * @fn		void UART2_Init()
 * @brief   Initialization the UART2 peripheral
 * @var     void
 * @retval  void
 */
void UART2_Init() {
    huart2.Instance = USART2;   /*! This is the base address of USART2*/
    huart2.Init.BaudRate = 115200; /*! the baud rate of the transfer*/
    huart2.Init.WordLength = UART_WORDLENGTH_8B; /*! Use 8-bit word length or 9-bit word length*/
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE; /*! Use the parity or not?*/
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;

    if (HAL_UART_Init(&huart2) != HAL_OK) {
        Error_handler(); /*! The initialization is not okie*/
    }
}/*UART2_Init*/

/**
 * @fn      void Error_handler()
 * @retval  void
 */
void Error_handler() {
    while (1) {

    }
}/*Error_handler*/

/**
 *
 */
void SystemClockConfig() {

}/*SystemClockConfig*/
