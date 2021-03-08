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

void SystemClockConfig(uint8_t clock_seq);
void UART2_Init();
void Error_handler();
static void settingClockfromPLL(RCC_OscInitTypeDef * osc_int ,uint8_t clock_seq, uint8_t * latency);

/* Those variable for configure the UART 2*/
UART_HandleTypeDef huart2;
char temp_rev; /*To save one char received from PC with Interrupt*/
uint16_t number_char_receive = 0;
char buffer_receive[100];
char receive_finish = FALSE;
char * user_data = "The user data: SysClk_PLL\r\n";

int main() {
    HAL_Init();

    SystemClockConfig(84);

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
void SystemClockConfig(uint8_t clock_seq) {
    /*Those variable for configure the System clock*/
    RCC_OscInitTypeDef osc_int;
    RCC_ClkInitTypeDef clk_int;
    uint8_t Flatency = 0;

    memset(&osc_int, 0, sizeof(RCC_OscInitTypeDef));

    /*select the source of clock for CPU and the bus*/
    osc_int.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    osc_int.HSIState = RCC_HSI_ON; /*Enable the HSI*/
    osc_int.HSICalibrationValue  = 16;
    osc_int.PLL.PLLState = RCC_PLL_ON; /*Enable the PLL*/
    osc_int.PLL.PLLSource = RCC_PLLSOURCE_HSI; /*the input clock of PLL is HSI(16MHz)*/

    settingClockfromPLL(&osc_int ,clock_seq, &Flatency);

    if (HAL_RCC_OscConfig(&osc_int) != HAL_OK) {
        Error_handler();
    }

    /*Configure the clock for the bus: AHB, APB, ..*/
    clk_int.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
            | RCC_CLOCKTYPE_PCLK1 |
            RCC_CLOCKTYPE_PCLK2;
    clk_int.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clk_int.APB2CLKDivider = RCC_HCLK_DIV2;
    clk_int.AHBCLKDivider = RCC_SYSCLK_DIV2;
    clk_int.APB1CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&clk_int, Flatency) != HAL_OK) {
        Error_handler();
    }
    HAL_SYSTICK_Config(HAL_RCC_GetSysClockFreq()/1000);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}/*SystemClockConfig*/


static void settingClockfromPLL(RCC_OscInitTypeDef * osc_int, uint8_t clock_seq, uint8_t * latency){

    switch(clock_seq){
        case 50:{
            /*Calculator the PLL output*/
            osc_int->PLL.PLLM = 16;/*the HSI is 16MHz, divide by 16 -> 1MHz*/
            osc_int->PLL.PLLN = 100; /*1MHz multiple by 100 -> 100MHz*/
            osc_int->PLL.PLLP = 2; /*100MHz divide by 2 -> 50MHz*/

            *latency = FLASH_ACR_LATENCY_1WS;
            break;
        }
        case 84:{
            /*Calculator the PLL output*/
            osc_int->PLL.PLLM = 16;/*the HSI is 16MHz, divide by 16 -> 1MHz*/
            osc_int->PLL.PLLN = 168; /*1MHz multiple by 168 -> 168MHz*/
            osc_int->PLL.PLLP = 2; /*168MHz divide by 2 -> 84MHz*/

            *latency = FLASH_ACR_LATENCY_2WS;
            break;
        }
        case 120:{
            /*Calculator the PLL output*/
            osc_int->PLL.PLLM = 16;/*the HSI is 16MHz, divide by 16 -> 1MHz*/
            osc_int->PLL.PLLN = 240; /*1MHz multiple by 240 -> 240MHz*/
            osc_int->PLL.PLLP = 2; /*240MHz divide by 2 -> 120MHz*/

            *latency = FLASH_ACR_LATENCY_3WS;
            break;
        }
        default:{
            break;
        }
    }
}
