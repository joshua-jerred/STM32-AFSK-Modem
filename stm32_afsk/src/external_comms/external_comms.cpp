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

#define COMMS_DEBUG 0 // Adds strings to the serial output

constexpr uint8_t kPacketFlag = 0xA5;

struct Message {
  MessageId id;
  etl::span<const uint8_t> payload;
};

void calculateCrc16(const etl::span<const uint8_t> &data_range, uint16_t &crc) {
  crc = 0xFFFF; // Initial value
  for (uint8_t byte : data_range) {
    crc ^= byte << 8;
    for (uint8_t i = 0; i < 8; i++) {
      if (crc & 0x8000) {
        crc = (crc << 1) ^ 0x1021;
      } else {
        crc <<= 1;
      }
    }
  }
}

void ExternalComms::process() {
  uint32_t num_bytes_received = uart_.getNumBytesReceived();
  if (num_bytes_received == 0) { // No Bytes Received
    status_led_.digitalWrite(false);
    message_rx_timeout_.reset();
    return;
  }
  status_led_.digitalWrite(true); // Indicate that some bytes were received

  message_rx_timeout_
      .resume(); // Starts if not already running, does not reset the timer

  if (message_rx_timeout_.getElapsedTicks() > kMessageProcessingTimeoutMs) {
    // A packet ID may have been received
    sendError(ErrorId::MESSAGE_TIMED_OUT);
  }

  // Minimum packet size is 7 bytes
  if (num_bytes_received < 7) {
    return;
  }

  auto &rx_buffer = uart_.receive();
  // Check for the start of a packet
  if (rx_buffer.at(0) != kPacketFlag) {
    sendError(ErrorId::INVALID_START_FLAG);
    return;
  }

  uint8_t packet_id = rx_buffer.at(1);

  uint16_t payload_size = rx_buffer.at(2) << 8 | rx_buffer.at(3);
  if (payload_size > kExternalUartPayloadMaxSize) {
    sendError(ErrorId::INVALID_PAYLOAD_SIZE);
    return;
  }

  // Check if the packet has been fully received
  if (payload_size > num_bytes_received - 7) {
    return;
  }

  // Validate the CRC
  uint16_t received_crc = rx_buffer.at(payload_size + 4) << 8 |
                          rx_buffer.at(payload_size + 5); // Get the CRC
  etl::span<uint8_t> crc_range{rx_buffer.begin() + 1,
                               rx_buffer.begin() + payload_size + 4};
  uint16_t calculated_crc = 0;
  calculateCrc16(crc_range, calculated_crc);
  if (received_crc != calculated_crc) {
    sendError(ErrorId::INVALID_CRC);
    return;
  }

  // Check for the end of packet flag
  if (rx_buffer.at(payload_size + 6) != kPacketFlag) {
    sendError(ErrorId::INVALID_END_FLAG);
    return;
  }

  // Process the packet
  switch (packet_id) {
  case bst::to_underlying(MessageId::HANDSHAKE):
    if (payload_size != 0) {
      sendError(ErrorId::INVALID_PAYLOAD_FOR_MESSAGE_TYPE);
      return;
    }
    sendAck(MessageId::HANDSHAKE);
    break;
  case bst::to_underlying(MessageId::SET_AAR):
    if (payload_size != 2) {
      sendError(ErrorId::INVALID_PAYLOAD_FOR_MESSAGE_TYPE);
      return;
    }
    processSetAarMessage(rx_buffer);
    break;
  default:
    sendError(ErrorId::INVALID_PACKET_ID);
    return;
  };

  uart_.clearBuffer(); // Clear the RX buffer
}

void ExternalComms::sendPacket(MessageId message_id,
                               const etl::span<const uint8_t> &payload) {
  uint32_t payload_size = payload.size();
  assert(payload_size <= kUartMaxPacketSize - 7);

  tx_buffer_.clear();
  tx_buffer_.push_back(kPacketFlag);
  tx_buffer_.push_back(bst::to_underlying(message_id));
  tx_buffer_.push_back(payload_size >> 8);
  tx_buffer_.push_back(payload_size & 0xFF);
  for (uint8_t byte : payload) {
    tx_buffer_.push_back(byte);
  }

  etl::span<uint8_t> crc_range{tx_buffer_.begin() + 1, tx_buffer_.end()};

  uint16_t crc = 0;
  calculateCrc16(crc_range, crc);

  tx_buffer_.push_back(crc >> 8);
  tx_buffer_.push_back(crc & 0xFF);
  tx_buffer_.push_back(kPacketFlag);

  assert(tx_buffer_.size() <= kUartMaxPacketSize);
  assert(tx_buffer_.size() >= 7);
  assert(tx_buffer_.size() == payload_size + 7);

  uart_.send(tx_buffer_);
}

void ExternalComms::sendError(ErrorId error_id) {
#if COMMS_DEBUG
  uint8_t error_id_byte = '0' + bst::to_underlying(error_id);
  etl::array<uint8_t, 11> payload = {bst::to_underlying(received_message_id),
                                     bst::to_underlying(error_id),
                                     'E',
                                     'R',
                                     'R',
                                     ' ',
                                     'I',
                                     'D',
                                     ':',
                                     error_id_byte,
                                     '\n'};
#else
  etl::array<uint8_t, 1> payload = {bst::to_underlying(error_id)};
#endif
  sendPacket(MessageId::ERROR, payload);
  uart_.clearBuffer(); // Clear the RX buffer
}

void ExternalComms::sendAck(MessageId message_to_ack) {
  etl::array<uint8_t, 1> payload = {bst::to_underlying(message_to_ack)};
  sendPacket(MessageId::ACK, payload);
}

void ExternalComms::processSetAarMessage(
    etl::array<uint8_t, kExternalUartRxBufferSize> &rx_buffer) {
  volatile uint16_t aar = rx_buffer.at(4) << 8 | rx_buffer.at(5);
  sendAck(MessageId::SET_AAR);
}
