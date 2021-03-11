/*
 * main.c
 *
 *  Created on: Jan 11, 2021
 *      Author: huong
 */
#include "main.h"
#include <string.h>

#define FALSE   0
#define TRUE    1

void SystemClockConfig(void);
void UART2_Init();
void Error_handler();

UART_HandleTypeDef huart2;
char temp_rev; /*To save one char received from PC with Interrupt*/
uint16_t number_char_receive = 0;
char buffer_receive[100];
char receive_finish = FALSE;

char * user_data = "The user data: IRQ\r\n";

int main() {

    char temp;

    HAL_Init();

    SystemClockConfig();

    UART2_Init();
    uint16_t string_length = strlen(user_data);
    if(HAL_UART_Transmit(&huart2, (uint8_t *)user_data, string_length, HAL_MAX_DELAY) != HAL_OK){
        Error_handler();
    }


    while(1){
        /*Wait until receiving whole the string*/
        while(receive_finish != TRUE){
            HAL_UART_Receive_IT(&huart2, (uint8_t *)&temp_rev, 1);
        }
        /*transferring the data was received*/
		if(HAL_UART_Transmit(&huart2, (uint8_t *)buffer_receive, number_char_receive, HAL_MAX_DELAY) != HAL_OK){
			Error_handler();
		}
        number_char_receive = 0;
        receive_finish = FALSE;
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
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(temp_rev == '\r'){
        buffer_receive[number_char_receive] = '\r';
        number_char_receive++;
        receive_finish = TRUE;
    }else{
        buffer_receive[number_char_receive] = temp_rev;
        number_char_receive++;
    }
}

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
