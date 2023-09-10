import serial
import time

import logging
log = logging.getLogger("external_host")

from external_comms_types import MessageId, ErrorId
from checksum import generateCrc16Ccitt
from packet import Packet, MIN_PACKET_LENGTH

PACKET_FLAG = 0xA5

PACKET_ID_HANDSHAKE = 0x01

PACKET_ID_ACK = 0x03
ACK_PACKET_SIZE = 8

PACKET_ID_NEW_TX = 0x04

class Comms:
    def __init__(self, serial_port, baud_rate, timeout_sec=1):
        log.debug(f'Creating Comms object with serial port {serial_port}, baud rate {baud_rate}, and timeout {timeout_sec}')
        self.serial_port = serial_port
        self.baud_rate = baud_rate
        self.timeout_sec = timeout_sec

    def connect(self):
        log.debug('Connecting to serial port')
        self.serial = serial.Serial(self.serial_port, self.baud_rate, timeout=self.timeout_sec)

    def disconnect(self):
        log.debug('Disconnecting from serial port')
        self.serial.close()

    def __sendPacket(self, packet:Packet):
        log.debug(f'Sending packet {str(packet)}')
        self.serial.write(packet.getPacketByteArray())

    def __receivePacket(self, expected_length):
        raw_packet = list(self.serial.read(expected_length))
        parsed = Packet()
        if not parsed.parseRawPacket(raw_packet):
            log.error(f'Failed to parse packet: {raw_packet}')
            return None
        log.debug(f'Received packet {str(parsed)}')
        return parsed

    def _sendAndReceiveAck(self, packet:Packet):
        self.__sendPacket(packet)
        ret_packet = self.__receivePacket(ACK_PACKET_SIZE)
        if ret_packet is None:
            log.error('Failed to receive ACK packet')
            return False
        if ret_packet.getPacketId() != PACKET_ID_ACK:
            log.error(f'Expected ACK packet, received {str(ret_packet)}')
            return False
        acked_packet_id = ret_packet.getPayload()[0]
        if acked_packet_id != packet.getPacketId():
            log.error(f'Expected ACK for packet ID {packet.getPacketId()}, received ACK for {acked_packet_id}')
            return False
        return True

    def handshake(self):
        test_packet = Packet()
        test_packet.generatePacket(PACKET_ID_HANDSHAKE, [])
        self._sendAndReceiveAck(test_packet)
        return True

    def setBaudRate(self, baud_rate) -> bool:
        assert(baud_rate in [50, 300, 1200])
        set_baud_rate_packet = Packet()
        baud_rate_msb = (baud_rate >> 8) & 0xFF
        baud_rate_lsb = baud_rate & 0xFF
        set_baud_rate_packet.generatePacket(int(MessageId.SET_BAUD_RATE), [baud_rate_msb, baud_rate_lsb])
        return self._sendAndReceiveAck(set_baud_rate_packet)

    def sendTestMessage(self) -> bool:
        test_packet = Packet()
        test_packet.generatePacket(int(MessageId.SEND_TEST_MESSAGE), [])
        return self._sendAndReceiveAck(test_packet)

    def sendNewTx(self, text) -> bool:
        new_tx_packet = Packet()
        new_tx_packet.generatePacket(int(MessageId.SEND_NEW_TX), [ord(c) for c in text])
        return self._sendAndReceiveAck(new_tx_packet)