/**
 * @file external_comms.cpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief
 * @date 2023-08-20
 * @copyright Copyright (c) 2023
 */

#include <etl/array.h>

#include <Bst/template_tools.hpp>

#include "external_comms.hpp"
#include "message_ids.hpp"

constexpr uint8_t kPacketStart1 = 0xAA;
constexpr uint8_t kPacketStart2 = 0x55;

void ExternalComms::process() {
  if (uart_.getNumBytesReceived() == 0) { // No Bytes Received
    status_led_.digitalWrite(false);
    return;
  }
  status_led_.digitalWrite(true); // Indicate that some bytes were received

  // Minimum packet size is 7 bytes
  uint32_t num_bytes_received = uart_.getNumBytesReceived();
  if (num_bytes_received < 7) {
    return;
  }

  auto &rx_buffer = uart_.receive();
  // Check for the start of a packet
  if (rx_buffer.at(0) != kPacketStart1 || rx_buffer.at(1) != kPacketStart2) {
    sendErrorAndClearBuffer();
    return;
  }

  uint8_t packet_id = rx_buffer.at(2);
  uint16_t payload_size = rx_buffer.at(3) << 8 | rx_buffer.at(4);

  // Check if the packet has been fully received
  if (payload_size > num_bytes_received - 7) {
    return;
  }
  // TODO: Validate the length
  // TODO: Validate the checksum

  // Process the packet
  switch (packet_id) {
  case bst::to_underlying(MessageId::TEST):
    sendAckAndClearBuffer();
    break;
  case bst::to_underlying(MessageId::NEW_TX):
    sendAckAndClearBuffer();
    break;
  default:
    sendErrorAndClearBuffer();
    return;
  };
}

void ExternalComms::sendErrorAndClearBuffer() {
  etl::array<uint8_t, 7> packet = {kPacketStart1,
                                   kPacketStart2,
                                   bst::to_underlying(MessageId::ERROR),
                                   0x00,
                                   0x00,
                                   'E',
                                   'R'};
  uart_.send(packet);
  uart_.clearBuffer();
}

void ExternalComms::sendAckAndClearBuffer() {
  etl::array<uint8_t, 7> packet = {kPacketStart1,
                                   kPacketStart2,
                                   bst::to_underlying(MessageId::ACK),
                                   0x00,
                                   0x00,
                                   'A',
                                   'K'};
  uart_.send(packet);
  uart_.clearBuffer();
}

void ExternalComms::processNewTxPacket(
    etl::array<uint8_t, bst::kUartRxBufferSize> &rx_buffer) {
  etl::array<uint8_t, 7> packet = {kPacketStart1,
                                   kPacketStart2,
                                   bst::to_underlying(MessageId::ACK),
                                   0x00,
                                   0x00,
                                   'T',
                                   'X'};
  uart_.send(packet);
  uart_.clearBuffer();
}