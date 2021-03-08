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
void TIM2_Init();
void GPIO_Init();
static void settingClockfromPLL(RCC_OscInitTypeDef * osc_int ,uint8_t clock_seq, uint8_t * latency);

/* Those variable for configure the UART 2*/
UART_HandleTypeDef huart2;
char temp_rev; /*To save one char received from PC with Interrupt*/
uint16_t number_char_receive = 0;
char buffer_receive[100];
char receive_finish = FALSE;
char * user_data = "The user data: Output compare timer 2\r\n";

/*! GPIO port A variable*/
GPIO_InitTypeDef igpio;

/*! The variable for TIMER purpose*/
TIM_HandleTypeDef htime2;/*! the variable to handle the timer 6(basic timer)*/

/**
 * @fn      main()
 * @return  int
 */
int main() {
    HAL_Init();

    SystemClockConfig(SYS_CLK_50MHZ);

    UART2_Init();
    GPIO_Init();
    TIM2_Init();

    uint16_t string_length = strlen(user_data);
    if (HAL_UART_Transmit(&huart2, (uint8_t *) user_data, string_length,
            HAL_MAX_DELAY) != HAL_OK) {
        Error_handler();
    }

    if(HAL_TIM_OC_Start_IT(&htime2, TIM_CHANNEL_1) != HAL_OK){
       Error_handler();
    }

    if(HAL_TIM_OC_Start_IT(&htime2, TIM_CHANNEL_2) != HAL_OK){
       Error_handler();
    }

    if(HAL_TIM_OC_Start_IT(&htime2, TIM_CHANNEL_3) != HAL_OK){
       Error_handler();
    }

    if(HAL_TIM_OC_Start_IT(&htime2, TIM_CHANNEL_4) != HAL_OK){
       Error_handler();
    }

    while (1) {
        __WFI();
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
 * @fn      SystemClockConfig
 * @param   clock_seq
 */
void SystemClockConfig(uint8_t clock_seq) {
    /*Those variable for configure the System clock*/
    RCC_OscInitTypeDef osc_int;
    RCC_ClkInitTypeDef clk_int;
    uint8_t Flatency = 0;

    memset(&osc_int, 0, sizeof(RCC_OscInitTypeDef));
    memset(&clk_int, 0, sizeof(RCC_ClkInitTypeDef));

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

/**
 * @fn      settingClockfromPLL
 * @param   osc_int
 * @param   clock_seq
 * @param   latency
 * @retval  void
 */
static void settingClockfromPLL(RCC_OscInitTypeDef * osc_int, uint8_t clock_seq, uint8_t * latency){

    switch(clock_seq){
        case SYS_CLK_50MHZ:{
            /*Calculator the PLL output*/
            osc_int->PLL.PLLM = 16;/*the HSI is 16MHz, divide by 16 -> 1MHz*/
            osc_int->PLL.PLLN = 100; /*1MHz multiple by 100 -> 100MHz*/
            osc_int->PLL.PLLP = 2; /*100MHz divide by 2 -> 50MHz*/

            *latency = FLASH_ACR_LATENCY_1WS;
            break;
        }
        case SYS_CLK_84MHZ:{
            /*Calculator the PLL output*/
            osc_int->PLL.PLLM = 16;/*the HSI is 16MHz, divide by 16 -> 1MHz*/
            osc_int->PLL.PLLN = 168; /*1MHz multiple by 168 -> 168MHz*/
            osc_int->PLL.PLLP = 2; /*168MHz divide by 2 -> 84MHz*/

            *latency = FLASH_ACR_LATENCY_2WS;
            break;
        }
        case SYS_CLK_120MHZ:{
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
}/*settingClockfromPLL*/


/**
 * @fn      void TIM2_Init()
 * @brief   Initialization high level for the TIM2 peripheral
 * @var     void
 * @retval  void
 */
void TIM2_Init() {
    TIM_OC_InitTypeDef ichannelConfig;
    memset(&htime2, 0U, sizeof(TIM_HandleTypeDef));
    memset(&ichannelConfig, 0U, sizeof(TIM_OC_InitTypeDef));

    htime2.Instance = TIM2;/*The address of timer 2*/
    htime2.Init.Prescaler = 0; /*the prescaler for clock of timer 2*/
    htime2.Init.CounterMode = TIM_COUNTERMODE_UP;/*Default: the counter mode of timer 2 is TIM_COUNTERMODE_UP*/
    htime2.Init.Period = 0xFFFFFFFF;
    htime2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    /*Initialized the timer 2*/
    if(HAL_TIM_OC_Init(&htime2) != HAL_OK){
        Error_handler();
    }

    /*Setting the output capture for channel*/
    ichannelConfig.OCMode = TIM_OCMODE_TOGGLE; /*! Select the toggle output mode*/
    ichannelConfig.Pulse = 25000U;
    ichannelConfig.OCPolarity = TIM_OCPOLARITY_HIGH;

    if(HAL_TIM_OC_ConfigChannel(&htime2, &ichannelConfig, TIM_CHANNEL_1) != HAL_OK){
        Error_handler();
    }

    ichannelConfig.Pulse = 12500U;
    if(HAL_TIM_OC_ConfigChannel(&htime2, &ichannelConfig, TIM_CHANNEL_2) != HAL_OK){
        Error_handler();
    }

    ichannelConfig.Pulse = 6250U;
    if(HAL_TIM_OC_ConfigChannel(&htime2, &ichannelConfig, TIM_CHANNEL_3) != HAL_OK){
        Error_handler();
    }

    ichannelConfig.Pulse = 3125U;
    if(HAL_TIM_OC_ConfigChannel(&htime2, &ichannelConfig, TIM_CHANNEL_4) != HAL_OK){
        Error_handler();
    }
}/*TIM2_Init*/

/**
  * @brief  Output Compare callback in non-blocking mode
  * @param  htim TIM OC handle
  * @retval None
  */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim){
    uint32_t temp = 0;
    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
        temp = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, temp + 25000U);
    }
    else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){
        temp = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_2);
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, temp + 12500U);
    }
    else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){
        temp = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_3);
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, temp + 6250U);
    }
    else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4){
        temp = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_4);
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, temp + 3125U);
    }
    else{
        /*Do nothing*/
    }
}

/**
 * @fn      GPIO_Init()
 * @brief   Initialized the GPIO A to toggle the LED
 */
void GPIO_Init(){
    igpio.Mode = GPIO_MODE_OUTPUT_PP; /*! Select the output mode for the pin of MCU*/
    igpio.Pin = GPIO_PIN_12; /*! Select the pin number*/
    igpio.Speed = GPIO_SPEED_FREQ_HIGH; /*! Select the speed*/
    igpio.Pull = GPIO_PULLUP; /*! Select the */

    /*Init*/
    HAL_GPIO_Init(GPIOD, &igpio);
}/*GPIO_Init*/