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
#include "waveform.hpp"

typedef StaticTask_t osStaticThreadDef_t;

ADC_HandleTypeDef hadc1;
DAC_HandleTypeDef hdac;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;
bst::Uart<kExternalUartRxBufferSize> g_external_uart(&huart3, &hdma_usart3_rx,
                                                     &hdma_usart3_tx);

static bst::Gpio g_primary_status_led(GPIOB, GPIO_PIN_0);
static bst::Gpio g_external_comms_status_led(GPIOB, GPIO_PIN_7);

static ExternalComms g_external_comms(g_external_uart,
                                      g_external_comms_status_led);

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

void defaultTask(void *argument) {
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
  uint16_t dac_index = 0;
  uint16_t delta = 1;
  for (;;) {
    g_primary_status_led.toggle();
    // g_primary_status_led.toggle();
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R,
                     SineWaveForm.at(dac_index));
    dac_index += delta;
    if (dac_index > SineWaveForm.size() - 1) {
      dac_index = 0;
    }
    osDelay(500);
  }
}

void externalCommsTask(void *argument) {
  while (true) {
    g_external_comms.process();
    osDelay(1);
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

void assertHandler(void) {
  __disable_irq();

#ifndef NDEBUG
  __asm("BKPT 0");
#endif

  while (1) {
  }
}