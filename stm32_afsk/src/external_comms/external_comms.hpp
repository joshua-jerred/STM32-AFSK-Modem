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

#include <Bst/Embd/stopwatch.hpp>

#include <etl/vector.h>

constexpr uint16_t kUartMaxPacketSize = 512 + 7;

#include "external_comms_config.hpp"
#include "external_comms_types.hpp"
#include "modem/modem.hpp"

class ExternalComms {
public:
  ExternalComms(bst::Uart<kExternalUartRxBufferSize> &hardware_uart,
                bst::Gpio &status_led, Modem &modem)
      : uart_(hardware_uart), status_led_(status_led), modem_(modem) {
  }

  void process();

private:
  void sendPacket(MessageId message_id,
                  const etl::span<const uint8_t> &payload);

  /**
   * @brief Send an error message and clear the RX buffer.
   *
   * @param error_id - The error ID
   */
  void sendError(ErrorId error_id);

  /**
   * @brief Send an ACK message and clear the RX buffer.
   * @param id_to_ack - The message ID to ACK
   */
  void sendAck(MessageId id_to_ack);

  void processSetBaudRateMessage(
      etl::array<uint8_t, kExternalUartRxBufferSize> &rx_buffer);

  void sendTestMessage();

  void transmitNewAfskMessage(
      etl::array<uint8_t, kExternalUartRxBufferSize> &rx_buffer);

  bst::Stopwatch message_rx_timeout_;

  bst::Uart<kExternalUartRxBufferSize> &uart_;
  bst::Gpio &status_led_;

  etl::vector<uint8_t, kUartMaxPacketSize> tx_buffer_;

  Modem &modem_;
};

#endif /* EXTERNAL_COMMS_HPP_ */