import logging
log = logging.getLogger("external_host")

from checksum import generateCrc16Ccitt
from enum import Enum

PACKET_FLAG = 0xA5
MAX_PAYLOAD_SIZE = 512

MIN_PACKET_LENGTH = 7 # with no payload
MAX_PACKET_LENGTH = MIN_PACKET_LENGTH + MAX_PAYLOAD_SIZE

class PacketIds:
    UNKNOWN = 0x00
    HANDSHAKE = 0x01
    ERROR = 0x02
    ACK = 0x03
    NEW_TX = 0x04

    @staticmethod
    def getName(packet_id):
        if packet_id == PacketIds.UNKNOWN:
            return 'Unknown'
        elif packet_id == PacketIds.HANDSHAKE:
            return 'Handshake'
        elif packet_id == PacketIds.ERROR:
            return 'Error'
        elif packet_id == PacketIds.ACK:
            return 'ACK'
        elif packet_id == PacketIds.NEW_TX:
            return 'New TX'
        else:
            return 'Unknown'

PACKET_ID_MAP = { # payload_size of -1 means variable length
    0x00: {
        'name': 'Unknown',
        'payload_size': 0,
    },
    0x01: {
        'name': 'Handshake',
        'payload_size': 0,
    },
    0x02: {
        'name': 'Error',
        'payload_size': 2,
    },
    0x03: {
        'name': 'ACK',
        'payload_size': 1,
    },
    0x04: {
        'name': 'New TX',
        'payload_size': -1,
    }
}

class Packet:
    def __init__(self):
        self.packet_id = -1
        self.payload_size_msb = -1
        self.payload_size_lsb = -1
        self.payload = []
        self.crc_msb = -1
        self.crc_lsb = -1

    def __setPacketId(self, packet_id:int):
        if packet_id not in PACKET_ID_MAP:
            log.error(f'Invalid packet ID: {packet_id}')
            return False
        self.packet_id = packet_id
        return True

    def __setPayloadSize(self, payload_size_msb:int, payload_size_lsb:int):
        if payload_size_msb < 0 or payload_size_msb > 255:
            log.error(f'Invalid payload size MSB: {payload_size_msb}')
            return False
        if payload_size_lsb < 0 or payload_size_lsb > 255:
            log.error(f'Invalid payload size LSB: {payload_size_lsb}')
            return False
        calculated_payload_size = payload_size_msb << 8 | payload_size_lsb
        if calculated_payload_size > MAX_PAYLOAD_SIZE:
            log.error(f'Invalid payload size: {calculated_payload_size}')
            return False
        self.payload_size_msb = payload_size_msb
        self.payload_size_lsb = payload_size_lsb
        return True

    def __setPayload(self, payload:list):
        if len(payload) > MAX_PAYLOAD_SIZE:
            log.error(f'Invalid payload size: {len(payload)}')
            return False
        self.payload = payload
        return True

    def __setCrc(self, crc_msb:int, crc_lsb:int):
        if crc_msb < 0 or crc_msb > 255:
            log.error(f'Invalid CRC MSB: {crc_msb}')
            return False
        if crc_lsb < 0 or crc_lsb > 255:
            log.error(f'Invalid CRC LSB: {crc_lsb}')
            return False
        self.crc_msb = crc_msb
        self.crc_lsb = crc_lsb
        return True

    def __calculateCrc(self):
        data_to_checksum = [self.packet_id, self.payload_size_msb, self.payload_size_lsb] + self.payload
        return generateCrc16Ccitt(data_to_checksum)

    def getPacketId(self):
        return self.packet_id

    def getPayload(self):
        return self.payload

    def getSizeOfPayload(self):
        return self.payload_size_msb << 8 | self.payload_size_lsb

    def parseRawPacket(self, packet_bytes:list):
        # Validate that the packet is at least the minimum length
        if len(packet_bytes) < MIN_PACKET_LENGTH:
            log.error(f'Packet too short: {packet_bytes}')
            return False

        # Validate the start and end flags
        if packet_bytes[0] != PACKET_FLAG:
            log.error(f'Invalid start flag: {packet_bytes}')
            return False
        if packet_bytes[-1] != PACKET_FLAG:
            log.error(f'Invalid end flag: {packet_bytes}')
            return False

        success = True
        success &= self.__setPacketId(packet_bytes[1])
        success &= self.__setPayloadSize(packet_bytes[2], packet_bytes[3])
        if len(packet_bytes) != self.getSizeOfPayload() + 7:
            log.error(f'Invalid payload size: {packet_bytes}')
            return False
        success &= self.__setPayload(packet_bytes[4:-3])
        success &= self.__setCrc(packet_bytes[-3], packet_bytes[-2])
        if not self.validateCrc():
            log.error(f'Invalid CRC: {packet_bytes}')
            return False
        log.debug(f'Parsed packet: {str(self)}')
        return True

    def generatePacket(self, packet_id, payload):
        log.debug(f'Generating packet with ID {packet_id} and payload {payload}')
        self.__setPacketId(packet_id)
        self.__setPayload(payload)
        self.__setPayloadSize(len(payload) >> 8, len(payload) & 0xff)
        self.updateCrc()

    def updateCrc(self):
        crc = self.__calculateCrc()
        self.__setCrc(crc >> 8, crc & 0xff)

    def validateCrc(self):
        crc = self.__calculateCrc()
        return self.crc_msb == crc >> 8 and self.crc_lsb == crc & 0xff

    def getPacketByteArray(self):
        return bytearray([PACKET_FLAG, self.packet_id, self.payload_size_msb, self.payload_size_lsb] + self.payload + [self.crc_msb, self.crc_lsb, PACKET_FLAG])

    def __str__(self):
        packet_id_str = "UNKNOWN"
        if self.packet_id in PACKET_ID_MAP:
            packet_id_str = PACKET_ID_MAP[self.packet_id]['name']
        return f'PACKET - ID: {packet_id_str}, Payload Size: {self.payload_size_msb << 8 | self.payload_size_lsb}, Payload: {self.payload}, CRC: {hex(self.crc_msb)}  {hex(self.crc_lsb)}'