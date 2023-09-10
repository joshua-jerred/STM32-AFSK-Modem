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

#include <etl/span.h>

constexpr uint16_t kTxBufferSize = 256;
class Modem {
public:
  enum class Mode { kIdleWait, kRxMonitor };
  enum class State {
    kIdle,
    kTxStarting,
    kTxPreamble,
    kTxData,
    kRxMonitor,
    kRxDemodulate
  };
  enum class BaudRate { k50, k300, k1200, k2400 };

  Modem(bst::Gpio &status_led, DAC_HandleTypeDef &waveform_dac,
        TIM_HandleTypeDef &waveform_timer, DMA_HandleTypeDef &waveform_dma,
        TIM_HandleTypeDef &baudTimer, ADC_HandleTypeDef &rx_adc)
      : status_led_(status_led), waveform_dac_(waveform_dac),
        waveform_timer_(waveform_timer), waveform_dma_(waveform_dma),
        baud_timer_(baudTimer), rx_adc_(rx_adc) {
  }
  ~Modem() = default;

  bool encodeNewAfskPacket(etl::span<const uint8_t> data);

  uint16_t getAdcValue();

  bool setBaudRate(BaudRate baud_rate) {
    if (state_ != State::kIdle) {
      return false;
    }

    baud_rate_ = baud_rate;
    return true;
  }

  void setMode(Mode mode);

  bool isBusy() const {
    return state_ != State::kIdle;
  }

  bool isTx() const {
    return state_ == State::kTxStarting || state_ == State::kTxPreamble ||
           state_ == State::kTxData;
  }

  bool isRx() const {
    return state_ == State::kRxMonitor || state_ == State::kRxDemodulate;
  }

  State getState() const {
    return state_;
  }

  void baudPulse();

private:
  enum class Freq { k1200Hz, k2200Hz };

  void setBaudTimer();
  void startWaveform();
  void stopWaveform();
  void setFreqSpace();
  void setFreqMark();

  bool baud_pulse_ = false;

  Freq current_freq_ = Freq::k1200Hz;

  bst::Gpio &status_led_;
  DAC_HandleTypeDef &waveform_dac_;
  TIM_HandleTypeDef &waveform_timer_;
  DMA_HandleTypeDef &waveform_dma_;
  TIM_HandleTypeDef &baud_timer_;
  ADC_HandleTypeDef &rx_adc_;

  // configurable
  uint8_t preamble_length_ = 2;
  BaudRate baud_rate_ = BaudRate::k1200;
  Mode mode_ = Mode::kIdleWait;
  State state_ = State::kIdle;

  // counters
  etl::span<const uint8_t> tx_data_{};
  uint16_t preamble_counter_ = 0;
  uint16_t data_byte_counter_ = 0;
  uint8_t bit_counter_ = 0;
};

#endif /* WAVEDRIVER_HPP_ */