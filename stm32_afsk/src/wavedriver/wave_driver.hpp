/**
 * @file wavedriver.hpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief Driver for the DAC
 * @date 2023-09-05
 * @copyright Copyright (c) 2023
 */

#ifndef WAVE_DRIVER_HPP_
#define WAVE_DRIVER_HPP_

#include "stm32f4xx_hal.h"

#include "waveform.hpp"

/**
 * @brief Base class for DAC control to generate waveforms.
 *
 */
class WaveDriver {
public:
  /**
   * @brief The Wave Form driver for the DAC.
   * @param hdac - STM32HAL DAC handle
   */
  WaveDriver(DAC_HandleTypeDef *hdac, uint32_t channel);

  /**
   * @brief Deconstructor for the WaveDriver
   * @details Disables the DAC and stops the DMA.
   */
  ~WaveDriver();

private:
  uint32_t frequency_;
  uint32_t channel_;
};

#endif /* WAVEDRIVER_HPP_ */