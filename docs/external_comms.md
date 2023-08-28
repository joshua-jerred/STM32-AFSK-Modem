# External Comms

## Overview
External Comms will be done via a hardware UART. If an ST-Link is available,
the UART will be used via ST-Link Virtual COM Port (VCP). If not, an FTDI cable
will be used.

## Basic Packet Structure
| Flag 1 | Flag 2 | PKT ID | PL 1 | PL 2 | ... Payload ... | CRC 1 | CRC 2 |

### Flag 1
The value of this byte is always 0x55

### Flag 2

### PKT ID