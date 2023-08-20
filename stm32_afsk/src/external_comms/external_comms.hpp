/**
 * @file external_comms.hpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief
 * @date 2023-08-20
 * @copyright Copyright (c) 2023
 */

#ifndef EXTERNAL_COMMS_HPP_
#define EXTERNAL_COMMS_HPP_

#include <Bst/Embd/uart.hpp>

#include <Bst/Embd/gpio.hpp>

class ExternalComms {
public:
  ExternalComms(bst::Uart &hardware_uart, bst::Gpio &status_led)
      : uart_(hardware_uart), status_led_(status_led) {
  }

  void process();

private:
  void sendErrorAndClearBuffer();
  void sendAckAndClearBuffer();

  void
  processNewTxPacket(etl::array<uint8_t, bst::kUartRxBufferSize> &rx_buffer);

  bst::Uart &uart_;
  bst::Gpio &status_led_;
};

#endif /* EXTERNAL_COMMS_HPP_ */