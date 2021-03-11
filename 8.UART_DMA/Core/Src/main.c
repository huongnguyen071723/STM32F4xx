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
void DMA_Init();
void Error_handler();

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma;
char temp_rev; /*To save one char received from PC with Interrupt*/
uint16_t number_char_receive = 0;
char buffer_receive[100];
char receive_finish = FALSE;

char * user_data = "The user data: DMA\r\n";

int main() {
    HAL_Init();

    SystemClockConfig();

    DMA_Init();
    UART2_Init();
    uint16_t string_length = strlen(user_data);
    if(HAL_UART_Transmit_DMA(&huart2, (uint8_t *)user_data, string_length) != HAL_OK){
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

    huart2.hdmatx = &hdma;
    huart2.hdmarx = &hdma;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        Error_handler(); /*! The initialization is not okie*/
    }
}/*UART2_Init*/

/**
 * @fn          DMA_Init
 * @brief       Initialized the DMA peripheral
 */
void DMA_Init(){
    __HAL_RCC_DMA1_CLK_ENABLE();/*Enable the clock for DMA1*/

    hdma.Instance = DMA1_Stream6;
    hdma.StreamBaseAddress = DMA1_Stream6_BASE;
    hdma.Init.Channel = DMA_CHANNEL_4;
    hdma.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma.Init.MemInc = DMA_MINC_ENABLE;
    hdma.Init.Mode = DMA_NORMAL;
    hdma.State = HAL_DMA_STATE_READY;
    hdma.Parent = &huart2; /*DMA transfer data from SRAM to USART2*/

    /*Initialized the DMA for USART2_TX*/
    if(HAL_DMA_Init(&hdma) != HAL_OK){
        Error_handler();
    }
}

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
