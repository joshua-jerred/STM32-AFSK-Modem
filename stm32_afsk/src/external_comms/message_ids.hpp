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

enum class MessageId : uint8_t {
  TEST = 0x01,
  ERROR = 0x02,
  ACK = 0x03,
  NEW_TX = 0x04,
};

#endif /* MESSAGE_IDS_HPP_ */