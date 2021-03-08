/*
 * msp.c
 *
 *  Created on: Jan 11, 2021
 *      Author: huong
 */
#include "stm32f4xx_hal.h"

/**
 * @fn  void HAL_MspInit()
 *
 */
void HAL_MspInit(void) {
    /*1. Set up the priority grouping of the ARM Cortex MX processor*/
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /*2. Enable the required system exceptions of the ARM Cortex MX processor*/
    SCB->SHCSR |= 0x07 << 16; /*usage fault, memory fault and bus fault system exception*/

    /*3. Configure the priority for the system exceptions*/
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}/*HAL_MspInit*/

/**
 * @brief  UART MSP Init.
 * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
 *                the configuration information for the specified UART module.
 * @retval None
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef gpio_uart;

    /*1. Enable the clock for the USART2 peripheral*/
    __HAL_RCC_USART2_CLK_ENABLE(); /*! Enable the USART 2 peripheral*/
    __HAL_RCC_GPIOA_CLK_ENABLE(); /*! Enable the GPIO port A peripheral*/
    __HAL_RCC_GPIOD_CLK_ENABLE(); /*! Enable the GPIO port D peripheral*/

    /*2. Do the pin muxing configurations*/
    gpio_uart.Pin  = GPIO_PIN_5;
    gpio_uart.Mode = GPIO_MODE_AF_PP;
    gpio_uart.Pull = GPIO_PULLUP;
    gpio_uart.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_uart.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOD, &gpio_uart); /*! Initialized the UART2: TX by using GPIO port D*/

    gpio_uart.Pin  = GPIO_PIN_6;
    HAL_GPIO_Init(GPIOD, &gpio_uart); /*! Initialized the UART2: RX by using GPIO port D*/


    /*3. Enable the IRQ and set up the priority (NVIC settings)*/
    HAL_NVIC_EnableIRQ(USART2_IRQn); /*! Enable the Interrupt for USART2 peripheral*/
    HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
}/*HAL_UART_MspInit*/
