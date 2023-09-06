/**
 * @file message_ids.hpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief
 * @date 2023-08-20
 * @copyright Copyright (c) 2023
 */

#ifndef MESSAGE_IDS_HPP_
#define MESSAGE_IDS_HPP_

#include <cstdint>

/**
 * @brief Message IDs for the ExternalComms class
 * @see external_comms.md
 */
enum class MessageId : uint8_t {
  UNKNOWN = 0x00,
  TEST = 0x01,
  ERROR = 0x02,
  ACK = 0x03,
  NEW_TX = 0x04,
};

/**
 * @brief Only used in a single place, but it's here to make changes easier when
 * adding new message IDs.
 *
 * @param message_id - The message ID to convert
 * @return MessageId - The converted message ID
 */
inline MessageId getMessageIdFromValue(uint8_t message_id) {
  switch (message_id) {
  case 0x00:
    return MessageId::UNKNOWN;
  case 0x01:
    return MessageId::TEST;
  case 0x02:
    return MessageId::ERROR;
  case 0x03:
    return MessageId::ACK;
  case 0x04:
    return MessageId::NEW_TX;
  default:
    return MessageId::UNKNOWN;
  }
}

/**
 * @brief Error IDs for the ExternalComms class
 * @see external_comms.md
 */
enum class ErrorId : uint8_t {
  INVALID_START_FLAG = 0x01,
  INVALID_PACKET_ID = 0x02,
  INVALID_PAYLOAD_SIZE = 0x03,
  INVALID_CRC = 0x04,
  INVALID_END_FLAG = 0x05,
  MESSAGE_TIMED_OUT = 0x06,
  BUSY = 0x07,
  INVALID_PAYLOAD_FOR_MESSAGE_TYPE = 0x08,
};

#endif /* MESSAGE_IDS_HPP_ */