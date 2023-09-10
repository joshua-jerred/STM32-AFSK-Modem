/**
 * @file modem.cpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief The modem driver.
 * @date 2023-09-09
 * @copyright Copyright (c) 2023
 */

#include "modem/modem.hpp"
#include "assert_handler.h"
#include "waveform.hpp"

#include <etl/array.h>
#include <etl/span.h>

constexpr uint16_t k1200HzPeriod = 585;
constexpr uint16_t k2200HzPeriod = 319;

bool Modem::encodeNewAfskPacket(etl::span<const uint8_t> data) {
  if (state_ != State::kIdle) {
    return false;
  }

  state_ = State::kStartingTx;
  tx_data_ = data;
  preamble_counter_ = 0;
  data_byte_counter_ = 0;
  bit_counter_ = 0;

  setBaudTimer();
  HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
  return true;
}

bool Modem::setBaudRate(BaudRate baud_rate) {
  if (state_ != State::kIdle) {
    return false;
  }

  baud_rate_ = baud_rate;
  return true;
}

void Modem::startWaveform() {
  status_led_.digitalWrite(true);
  HAL_DAC_Start_DMA(&waveform_dac_, DAC_CHANNEL_1, &SineWaveForm.at(0),
                    SineWaveForm.size(), DAC_ALIGN_12B_R);
  HAL_TIM_Base_Start(&waveform_timer_);
}

void Modem::stopWaveform() {
  HAL_DAC_Stop_DMA(&waveform_dac_, DAC_CHANNEL_1);
  HAL_TIM_Base_Stop(&waveform_timer_);
  status_led_.digitalWrite(false);
}

void Modem::setBaudTimer() {
  if (baud_rate_ == BaudRate::k50) {
    baud_timer_.Instance->PSC = 29;
    baud_timer_.Instance->ARR = 59999;
  } else if (baud_rate_ == BaudRate::k300) {
    baud_timer_.Instance->PSC = 4;
    baud_timer_.Instance->ARR = 59999;
  } else if (baud_rate_ == BaudRate::k1200) {
    baud_timer_.Instance->PSC = 1;
    baud_timer_.Instance->ARR = 37499;
  } else if (baud_rate_ == BaudRate::k2400) {
    baud_timer_.Instance->PSC = 1;
    baud_timer_.Instance->ARR = 18749;
  }
}

void Modem::setFreqSpace() {
  waveform_timer_.Instance->ARR = k2200HzPeriod;
  current_freq_ = Freq::k2200Hz;
}

void Modem::setFreqMark() {
  waveform_timer_.Instance->ARR = k1200HzPeriod;
  current_freq_ = Freq::k1200Hz;
}

void Modem::baudPulse() {
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);

  if (state_ == State::kIdle) {
    HAL_NVIC_DisableIRQ(TIM8_UP_TIM13_IRQn);
    return;
  } else if (state_ == State::kStartingTx) {
    startWaveform();
    if (preamble_length_ == 0) {
      state_ = State::kTransmittingData;
    } else {
      state_ = State::kTransmittingPreamble;
    }
  }

  if (state_ == State::kTransmittingPreamble) {
    setFreqMark();
    if (preamble_counter_ < preamble_length_) {
      preamble_counter_++;
      return;
    } else {
      state_ = State::kTransmittingData;
    }
  }

  if (state_ == State::kTransmittingData) {
    if (bit_counter_ == 0) { // start bit
      setFreqSpace();
      bit_counter_++;
      return;
    } else if (bit_counter_ < 9) { // data bits
      if (tx_data_[data_byte_counter_] & (1 << (bit_counter_ - 1))) {
        setFreqMark();
      } else {
        setFreqSpace();
      }
      bit_counter_++;
      return;
    } else if (bit_counter_ == 9) { // stop bit
      setFreqMark();
      bit_counter_ = 0;
      data_byte_counter_++;
      if (data_byte_counter_ == tx_data_.size()) {
        state_ = State::kIdle;
        stopWaveform();
        HAL_NVIC_DisableIRQ(TIM8_UP_TIM13_IRQn);
      }
      return;
    }
  }
}