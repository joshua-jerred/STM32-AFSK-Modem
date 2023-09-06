/**
 * @file hardware_config.hpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief Hardware configuration values
 * @date 2023-08-28
 * @copyright Copyright (c) 2023
 */

#ifndef HARDWARE_CONFIG_HPP_
#define HARDWARE_CONFIG_HPP_

#include <cstdint>

constexpr uint16_t kExternalUartPayloadMaxSize = 512;
constexpr uint16_t kExternalUartPacketMinSize = 7;
constexpr uint16_t kExternalUartRxBufferSize =
    kExternalUartPayloadMaxSize + kExternalUartPacketMinSize;

// From reception of first byte to the end of the packet
constexpr uint32_t kMessageProcessingTimeoutMs = 100;

#endif /* HARDWARE_CONFIG_HPP_ */