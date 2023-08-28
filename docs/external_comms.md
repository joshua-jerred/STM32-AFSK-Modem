# External Comms

## Overview
External Comms will be done via a hardware UART. If an ST-Link is available,
the UART will be used via ST-Link Virtual COM Port (VCP). If not, an FTDI cable
will be used.

## Basic Packet Structure

|      Field      | Size  |
| :-------------: | :---: |
|      Flag       |   2   |
|    Packet ID    |   1   |
| Payload Length  |   2   |
| ... Payload ... | 0-512 |
|       CRC       |   2   |

Packets are sent in big-endian order, with the flag being the first byte sent,
and the CRC being the last two bytes sent. The minimum packet size is 7 bytes
when there is no payload. The maximum packet size is 519 bytes when there is a
512 byte payload.

The packet structure is as follows:

### Flag
Two bytes that are always `0x0A50` to indicate the start of a packet.

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
excluding the CRC fields. CRC 1 is the MSB, CRC 2 is the LSB.