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
  enum class Mode { kAfsk };
  enum class State {
    kIdle,
    kStartingTx,
    kTransmittingPreamble,
    kTransmittingData
  };
  enum class BaudRate { k50, k300, k1200, k2400 };

  Modem(bst::Gpio &status_led, DAC_HandleTypeDef &waveform_dac,
        TIM_HandleTypeDef &waveform_timer, DMA_HandleTypeDef &waveform_dma,
        TIM_HandleTypeDef &baudTimer)
      : status_led_(status_led), waveform_dac_(waveform_dac),
        waveform_timer_(waveform_timer), waveform_dma_(waveform_dma),
        baud_timer_(baudTimer) {
  }
  ~Modem() = default;

  bool encodeNewAfskPacket(etl::span<const uint8_t> data);

  bool setBaudRate(BaudRate baud_rate);

  void setMode(Mode mode);

  void baudPulse();

  bool isBusy() const {
    return state_ != State::kIdle;
  }

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

  // configurable
  uint8_t preamble_length_ = 2;
  BaudRate baud_rate_ = BaudRate::k1200;
  Mode mode_ = Mode::kAfsk;
  State state_ = State::kIdle;

  // counters
  etl::span<const uint8_t> tx_data_{};
  uint16_t preamble_counter_ = 0;
  uint16_t data_byte_counter_ = 0;
  uint8_t bit_counter_ = 0;
};

#endif /* WAVEDRIVER_HPP_ */