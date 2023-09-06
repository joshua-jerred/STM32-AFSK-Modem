# External Comms

## Overview
External Comms will be done via a hardware UART. If an ST-Link is available,
the UART will be used via ST-Link Virtual COM Port (VCP). If not, an FTDI cable
will be used.

## Definitions
Controller - The external host (Linux/Windows Machine)
Peripheral - The embedded device (STM32)

## Basic Packet Structure

|      Field      | Size  |
| :-------------: | :---: |
|      Flag       |   1   |
|    Packet ID    |   1   |
| Payload Length  |   2   |
| ... Payload ... | 0-512 |
|       CRC       |   2   |
|      Flag       |   1   |

Packets are sent in big-endian order, with the flag being the first byte sent,
and the CRC being the last two bytes sent. The minimum packet size is 7 bytes
when there is no payload. The maximum packet size is 519 bytes when there is a
512 byte payload.

The packet structure is as follows:

### Flag
One byte at the start of the packet, and one byte at the end of the packet.
Both are always `0xA5` to indicate the start and end of a packet. These bytes
are not included in the CRC calculation. The value `0xA5` may be within the
payload.
Two bytes that are always `0xA5` to indicate the start of a packet.

### Packet ID
The packet ID is a 8-bit unsigned integer that identifies the type of packet.
Some packets have a payload, some do not.

### Payload Length
The payload length is a 16-bit unsigned integer that specifies the length of
the payload in bytes. The maximum payload length is currently 512 bytes.

### Payload
A variable length payload. The length is specified by the Payload Length field.

### CRC
The CRC is a 16-bit unsigned integer that is the CRC-16 of the entire packet
excluding the CRC fields and the flag fields. MSB first.

***

## Packet Types

### 0x01 - Test Packet
- Payload Length: 0
- Payload: None
- Description: This packet is sent by the controller, the peripheral will
  respond with an ACK packet if received correctly.

### 0x02 - Error Packet
- Payload Length: 2
- Payload: Packet ID of the packet that caused the error, Error Code
- Description: This packet is sent by the peripheral in response to a request
  to indicate that an error occurred.
#### Error Codes
| Error Code |           Description            |
| :--------: | :------------------------------: |
|    0x01    |        Invalid Start Flag        |
|    0x02    |        Invalid Packet ID         |
|    0x03    |       Invalid Payload Size       |
|    0x04    |           Invalid CRC            |
|    0x05    |         Invalid End Flag         |
|    0x06    |        Message Timed Out         |
|    0x07    |     Busy - (TXing or RXing)      |
|    0x08    | Invalid Payload For Message Type |
|            |                                  |

### 0x03 - ACK Packet
- Payload Length: 1
- Payload: The packet ID of the packet that is being acknowledged.
- Description: This packet is sent by the peripheral to acknowledge that a
  packet was received correctly.

### 0x04 - NEW_TX Packet
- Payload Length: 0-512
- Payload: The data to be transmitted
- Description: This packet is sent by the controller to start a new transmission
  of data. The peripheral will respond with an ACK before starting the TX.

***

## Sample Test Packet
A5 01 00 00 00 00 A5