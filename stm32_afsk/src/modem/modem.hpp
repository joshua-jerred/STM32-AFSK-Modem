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

#include <Bst/Embd/gpio.hpp>

class Modem {
public:
  enum class Mode { kAfsk, kBpsk, kTest };

  Modem(bst::Gpio &status_led, DAC_HandleTypeDef &waveform_dac,
        TIM_HandleTypeDef &waveform_timer, DMA_HandleTypeDef &waveform_dma)
      : status_led_(status_led), waveform_dac_(waveform_dac),
        waveform_timer_(waveform_timer), waveform_dma_(waveform_dma) {
  }
  ~Modem() = default;

  void start();
  void stop();
  void update();
  void setMode(Mode mode) {
    mode_ = mode;
  }

private:
  enum class CurrentFreq { k1200Hz, k2200Hz };

  void encodeAfsk(etl::span<const uint8_t> data);

  void TestMessage();

  void setFreq1200();
  void setFreq2200();
  void shift180();

  Mode mode_ = Mode::kTest;
  CurrentFreq current_freq_ = CurrentFreq::k1200Hz;

  bst::Gpio &status_led_;
  DAC_HandleTypeDef &waveform_dac_;
  TIM_HandleTypeDef &waveform_timer_;
  DMA_HandleTypeDef &waveform_dma_;
};

#endif /* WAVEDRIVER_HPP_ */