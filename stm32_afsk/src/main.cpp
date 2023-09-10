/**
 * @file main.cpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief Main function for the project.
 * @date 2023-08-28
 * @copyright Copyright (c) 2023
 */

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "hardware/macros.h"
#include "task.h"

#include <Bst/Embd/gpio.hpp>
#include <etl/array.h>

#include "assert_handler.h"
#include "external_comms/external_comms.hpp"
#include "hardware/hardware_init.hpp"
#include "modem/modem.hpp"

typedef StaticTask_t osStaticThreadDef_t;

DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_dac1;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim13;
ADC_HandleTypeDef hadc1;
DAC_HandleTypeDef hdac;
UART_HandleTypeDef huart3;

bst::Uart<kExternalUartRxBufferSize> g_external_uart(&huart3, &hdma_usart3_rx,
                                                     &hdma_usart3_tx);

static bst::Gpio g_primary_status_led(GPIOB, GPIO_PIN_0);
static bst::Gpio g_external_comms_status_led(GPIOB, GPIO_PIN_7);
static bst::Gpio g_debug_out_pin(GPIOB, GPIO_PIN_11);

static Modem g_modem(g_primary_status_led, hdac, htim6, hdma_dac1, htim13);
static ExternalComms g_external_comms(g_external_uart,
                                      g_external_comms_status_led, g_modem);

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for externalComms */
osThreadId_t externalCommsHandle;
uint32_t externalCommsBuffer[128];
osStaticThreadDef_t externalCommsControlBlock;
const osThreadAttr_t externalComms_attributes = {
    .name = "externalComms",
    .cb_mem = &externalCommsControlBlock,
    .cb_size = sizeof(externalCommsControlBlock),
    .stack_mem = &externalCommsBuffer[0],
    .stack_size = sizeof(externalCommsBuffer),
    .priority = (osPriority_t)osPriorityLow,
};

void defaultTask(void *arHAL_DAC_SetValuegument) {
  HAL_TIM_Base_Start_IT(&htim13);
  // HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
  // g_modem.start();

  while (true) {
    // g_modem.update();
    osDelay(1000);
  }
}

void externalCommsTask(void *argument) {
  while (true) {
    g_external_comms.process();
    osDelay(5);
  }
}

int main() {
  initHardware();

  osKernelInitialize();
  defaultTaskHandle = osThreadNew(defaultTask, NULL, &defaultTask_attributes);
  externalCommsHandle =
      osThreadNew(externalCommsTask, NULL, &externalComms_attributes);
  osKernelStart();

  // Should never get here
  while (1) {
    assertHandler();
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t gpio_pin) {
  if (gpio_pin == USER_Btn_Pin) {
    HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM13) {
    g_modem.baudPulse();
  }
}

void assertHandler(void) {
  __disable_irq();

  // #ifndef NDEBUG
  // __asm("BKPT 0");
  // #endif

  while (1) {
    g_primary_status_led.toggle();
    g_external_comms_status_led.toggle();

    HAL_Delay(100);
  }
}