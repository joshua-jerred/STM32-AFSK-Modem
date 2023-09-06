import unittest
import serial
import time
import sys
import os

dir_path = os.path.dirname(os.path.realpath(__file__)) + '/../external_host'
# Add to path
sys.path.append(dir_path)
from checksum import generateCrc16Ccitt

SERIAL_PORT = '/dev/ttyACM0'
BAUD_RATE = 115200
serial_port = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.5)
serial_port.flush()

class Packet:
    def __init__(self):
        self.start_flag = 0xa5
        self.packet_id = -1
        self.payload_size_msb = -1
        self.payload_size_lsb = -1
        self.payload_size = -1
        self.payload = []
        self.crc_msb = -1
        self.crc_lsb = -1
        self.end_flag = 0xa5

    def getByteList(self):
        return [self.start_flag, self.packet_id, self.payload_size_msb, self.payload_size_lsb] + self.payload + [self.crc_msb, self.crc_lsb, self.end_flag]

    def generateValidTestPacket(self):
        self.start_flag = 0xa5
        self.packet_id = 0x01
        self.payload_size_msb = 0x00
        self.payload_size_lsb = 0x00
        self.payload_size = 0x0000
        self.payload = []
        self.crc_msb = 0xfb
        self.crc_lsb = 0xac

    def updateCrc(self):
        self.payload_size = self.payload_size_msb << 8 | self.payload_size_lsb
        checksumed_data_range = self.getByteList()[1:self.payload_size + 4]
        crc = generateCrc16Ccitt(checksumed_data_range)
        self.crc_msb = crc >> 8
        self.crc_lsb = crc & 0xff

    def parse(self, byte_list):
        self.start_flag = byte_list[0]
        self.packet_id = byte_list[1]
        self.payload_size_msb = byte_list[2]
        self.payload_size_lsb = byte_list[3]
        self.payload_size = self.payload_size_msb << 8 | self.payload_size_lsb
        self.payload = byte_list[4:-3]
        self.crc_msb = byte_list[-3]
        self.crc_lsb = byte_list[-2]
        self.end_flag = byte_list[-1]

    def __str__(self):
        return f'Packet ID: {self.packet_id}, Payload Size: {self.payload_size_msb << 8 | self.payload_size_lsb}, Payload: {self.payload}, CRC: {hex(self.crc_msb)}  {hex(self.crc_lsb)}'

class TestExternalComms(unittest.TestCase):
    def getResponsePacket(self, expected_payload_size):
        expected_size = expected_payload_size + 7
        # time.sleep(0.1)
        received = serial_port.read(expected_size)
        if len(received) != expected_size:
            print('Received Bytes: ', received)
            self.fail('Expected {} bytes, received {}'.format(expected_size, len(received)))
        received_packet = Packet()
        received_packet.parse(received)
        return received_packet

    def validateErrorPacket(self, expected_packet_id, expected_error_code):
        received_packet = self.getResponsePacket(2)
        self.assertEqual(received_packet.packet_id, 0x02)

        error_packet_id = received_packet.payload[0]
        error_code = received_packet.payload[1]
        print("Received Error Packet:", received_packet, "Error Packet ID:", error_packet_id, "Error Code:", error_code)

        self.assertEqual(error_packet_id, expected_packet_id)
        self.assertEqual(error_code, expected_error_code)

    def validateAckPacket(self, expected_packet_id):
        received_packet = self.getResponsePacket(1)
        self.assertEqual(received_packet.packet_id, 0x03)

        acked_packet_id = received_packet.payload[0]
        print("Received Ack Packet:", received_packet, "Acked Packet ID:", acked_packet_id)

        self.assertEqual(acked_packet_id, expected_packet_id)

    def sendPacket(self, packet):
        print("Sending Packet:", packet)
        serial_port.write(packet.getByteList())

    def test_timeoutError(self):
        serial_port.write([0xA5])

        error_packet_id = 0x00 # Unknown packet (only flag was sent)
        error_code = 0x06 # Timeout error
        self.validateErrorPacket(error_packet_id, error_code)

    def test_invalidStartFlag(self):
        packet = Packet()
        packet.generateValidTestPacket()
        packet.start_flag = 0xaa
        self.sendPacket(packet)

        expected_error_packet_id = 0x00 # Unknown packet as the start flag is invalid
        expected_error_code = 0x01 # Invalid start flag
        self.validateErrorPacket(expected_error_packet_id, expected_error_code)

    def test_invalidPacketId(self):
        packet = Packet()
        packet.generateValidTestPacket()
        packet.packet_id = 0xff
        packet.updateCrc()
        self.sendPacket(packet)

        expected_error_packet_id = 0x00 # Unknown packet as the start flag is invalid
        expected_error_code = 0x02 # Invalid packet ID
        self.validateErrorPacket(expected_error_packet_id, expected_error_code)

    def test_invalidPayloadSize(self):
        packet = Packet()
        packet.generateValidTestPacket()
        invalid_size = 512 + 8
        packet.payload_size_msb = invalid_size >> 8
        packet.payload_size_lsb = invalid_size & 0xff
        packet.updateCrc()
        self.sendPacket(packet)

        expected_error_packet_id = 0x01 # Test packet
        expected_error_code = 0x03 # Invalid payload size
        self.validateErrorPacket(expected_error_packet_id, expected_error_code)

    def test_invalidCrcLsb(self):
        packet = Packet()
        packet.generateValidTestPacket()
        packet.crc_lsb = 0x00
        self.sendPacket(packet)

        expected_error_packet_id = 0x01 # Test packet
        expected_error_code = 0x04 # Invalid crc
        self.validateErrorPacket(expected_error_packet_id, expected_error_code)

    def test_invalidCrcMsb(self):
        packet = Packet()
        packet.generateValidTestPacket()
        packet.crc_msb = 0x00
        self.sendPacket(packet)

        expected_error_packet_id = 0x01 # Test packet
        expected_error_code = 0x04 # Invalid crc
        self.validateErrorPacket(expected_error_packet_id, expected_error_code)

    def test_invalidEndFlag(self):
        packet = Packet()
        packet.generateValidTestPacket()
        packet.end_flag = 0x00
        self.sendPacket(packet)

        expected_error_packet_id = 0x01 # Test packet
        expected_error_code = 0x05 # Invalid end flag
        self.validateErrorPacket(expected_error_packet_id, expected_error_code)

    def test_invalidPayloadForMessageType(self):
        packet = Packet()
        packet.generateValidTestPacket()
        packet.payload = [0x01]
        packet.payload_size_lsb = 0x01
        packet.updateCrc()
        self.sendPacket(packet)

        expected_error_packet_id = 0x01 # Test packet
        expected_error_code = 0x08 # Invalid payload for message type
        self.validateErrorPacket(expected_error_packet_id, expected_error_code)

    def test_validTestMessage(self):
        packet = Packet()
        packet.generateValidTestPacket()
        self.sendPacket(packet)

        self.validateAckPacket(0x01)


if __name__ == '__main__':
    unittest.main()