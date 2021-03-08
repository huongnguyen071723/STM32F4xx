/*
 * it.c
 *
 *  Created on: Jan 11, 2021
 *      Author: huong
 */
#include "main.h"
extern UART_HandleTypeDef huart2;

void SysTick_Handler() {
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

void USART2_IRQHandler() {
    HAL_UART_IRQHandler(&huart2);
}
