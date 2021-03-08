/*
 * it.c
 *
 *  Created on: Jan 11, 2021
 *      Author: huong
 */


void SysTick_Handler(){
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}
