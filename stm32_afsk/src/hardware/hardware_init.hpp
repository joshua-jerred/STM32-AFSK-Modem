/**
 * @file hardware_configuration.hpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief Header file for STM32 hardware configuration
 * @date 2023-08-28
 * @copyright Copyright (c) 2023
 */

#ifndef HARDWARE_CONFIGURATION_HPP_
#define HARDWARE_CONFIGURATION_HPP_

#include "macros.h"

#include <Bst/Embd/uart.hpp>

extern ADC_HandleTypeDef hadc1;
extern DAC_HandleTypeDef hdac;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern bst::Uart g_external_uart;

void initHardware();

#endif /* HARDWARE_CONFIGURATION_HPP_ */