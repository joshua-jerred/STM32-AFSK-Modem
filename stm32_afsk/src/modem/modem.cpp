/**
 * @file modem.cpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief The modem driver.
 * @date 2023-09-09
 * @copyright Copyright (c) 2023
 */

#include "modem/modem.hpp"
#include "waveform.hpp"

#include <etl/array.h>
#include <etl/span.h>

constexpr uint16_t k1200HzPeriod = 585;
constexpr uint16_t k2200HzPeriod = 319;

void Modem::start() {
  HAL_DAC_Start_DMA(&waveform_dac_, DAC_CHANNEL_1, &SineWaveForm.at(0),
                    SineWaveForm.size(), DAC_ALIGN_12B_R);
  HAL_TIM_Base_Start(&waveform_timer_);
}

void Modem::stop() {
  HAL_DAC_Stop_DMA(&waveform_dac_, DAC_CHANNEL_1);
  HAL_TIM_Base_Stop(&waveform_timer_);
}

void Modem::update() {
  if (mode_ == Mode::kTest) {
    TestMessage();
    return;
  }
}

void Modem::setFreq1200() {
  waveform_timer_.Instance->ARR = k1200HzPeriod;
  // set the counter to 0
  waveform_timer_.Instance->CNT = 0;
  current_freq_ = CurrentFreq::k1200Hz;
}

void Modem::setFreq2200() {
  waveform_timer_.Instance->ARR = k2200HzPeriod;
  current_freq_ = CurrentFreq::k2200Hz;
}

void Modem::shift180() {
  // waveform_dma_.Instance->CR &= ~DMA_SxCR_DIR;
}

void Modem::encodeAfsk(etl::span<const uint8_t> data) {
  for (auto byte : data) {
    for (int i = 0; i < 8; i++) {
      if (byte & (1 << i)) {
        setFreq2200();
      } else {
        setFreq1200();
      }
        }
  }
}

void Modem::TestMessage() {
  static etl::array<uint8_t, 4> test_message = {'A', 'A', 'A', 'A'};
  status_led_.digitalWrite(true);

  status_led_.digitalWrite(false);
}