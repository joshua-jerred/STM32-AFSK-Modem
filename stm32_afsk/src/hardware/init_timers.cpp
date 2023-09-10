#include "hardware_init.hpp"

void configureTimers() {
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  // TIM6 is used to drive the DAC
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 0;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 585;
  htim6.Init.AutoReloadPreload =
      TIM_AUTORELOAD_PRELOAD_ENABLE; // NEED THIS TO BE ABLE TO CHANGE PERIOD
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK) {
    assertHandler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK) {
    assertHandler();
  }

  // TIM13 is used to generate the baud rate timer
  htim13.Instance = TIM13;
  htim13.Init.Prescaler = 29; // 0 = 1200 baud
  htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim13.Init.Period = 59999; // 37499 = 1200 baud
  htim13.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim13) != HAL_OK) {
    assertHandler();
  }
  __HAL_RCC_TIM13_CLK_ENABLE();
  HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 0, 0);
}