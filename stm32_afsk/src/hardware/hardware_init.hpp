/**
 * @file hardware_configuration.hpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief Header file for STM32 hardware configuration
 * @date 2023-08-28
 * @copyright Copyright (c) 2023
 */

#ifndef HARDWARE_CONFIGURATION_HPP_
#define HARDWARE_CONFIGURATION_HPP_

#include "assert_handler.h"
#include "macros.h"

#include "external_comms/external_comms_config.hpp"

#include <Bst/Embd/uart.hpp>

extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim13;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern DAC_HandleTypeDef hdac;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern bst::Uart<kExternalUartRxBufferSize> g_external_uart;

void initHardware();

#endif /* HARDWARE_CONFIGURATION_HPP_ */