/*
 * it.c
 *
 *  Created on: Jan 11, 2021
 *      Author: huong
 */
#include "main.h"
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htime2;/*! the variable to handle the timer 2(general timer)*/

void SysTick_Handler() {
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

void USART2_IRQHandler() {
    HAL_UART_IRQHandler(&huart2);
}


void TIM2_IRQHandler(){
    HAL_TIM_IRQHandler(&htime2);
}


/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
    TIM6->SR = 0x00;
}
