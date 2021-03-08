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

/* Those variable for configure the UART 2*/
UART_HandleTypeDef huart2;
char temp_rev; /*To save one char received from PC with Interrupt*/
uint16_t number_char_receive = 0;
char buffer_receive[100];
char receive_finish = FALSE;
char * user_data = "The user data: SysClk\r\n";

/*Those variable for configure the System clock*/
RCC_OscInitTypeDef osc_int;
RCC_ClkInitTypeDef clk_int;

int main() {
    HAL_Init();

    SystemClockConfig();

    UART2_Init();
    uint16_t string_length = strlen(user_data);
    if (HAL_UART_Transmit(&huart2, (uint8_t *) user_data, string_length,
            HAL_MAX_DELAY) != HAL_OK) {
        Error_handler();
    }

    while (1) {
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
    huart2.Instance = USART2; /*! This is the base address of USART2*/
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
    memset(&osc_int, 0, sizeof(RCC_OscInitTypeDef));

    /*select the source of clock for CPU and the bus*/
    osc_int.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    osc_int.HSEState = RCC_HSE_BYPASS;
    if (HAL_RCC_OscConfig(&osc_int) != HAL_OK) {
        Error_handler();
    }

    /*Configure the clock for the bus: AHB, APB, ..*/
    clk_int.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
            | RCC_CLOCKTYPE_PCLK1 |
            RCC_CLOCKTYPE_PCLK2;
    clk_int.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
    clk_int.APB2CLKDivider = RCC_HCLK_DIV2;
    clk_int.AHBCLKDivider = RCC_SYSCLK_DIV2;
    clk_int.APB1CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&clk_int, FLASH_ACR_LATENCY_0WS) != HAL_OK) {
        Error_handler();
    }

    __HAL_RCC_HSI_DISABLE(); /*Disable the HSI source*/
}/*SystemClockConfig*/
