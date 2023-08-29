import serial

PACKET_FLAG = 0xA5

MESSAGE_ID_HANDSHAKE = 0x01
MESSAGE_ID_ACK = 0x03

class Comms:
    def __init__(self, serial_port, baud_rate, timeout_sec=1):
        self.serial_port = serial_port
        self.baud_rate = baud_rate
        self.timeout_sec = timeout_sec

    def connect(self):
        self.serial = serial.Serial(self.serial_port, self.baud_rate, timeout=self.timeout_sec)

    def disconnect(self):
        self.serial.close()

    def _send(self, message_id, payload:list[int]):
        length_msb = len(payload) >> 8
        length_lsb = len(payload) & 0xFF
        crc_msb = 0
        crc_lsb = 0
        packet = bytearray([PACKET_FLAG, message_id, length_msb, length_lsb] + payload + [crc_msb, crc_lsb, PACKET_FLAG])
        self.serial.write(packet)

    def _receive(self, expected_length):
        ret_packet = list(self.serial.read(expected_length))

        MINIMUM_LENGTH = 2 + 1 + 2 + 2
        if len(ret_packet) < MINIMUM_LENGTH:
            raise Exception('Invalid packet length', len(ret_packet), MINIMUM_LENGTH)

        if ret_packet[0] != PACKET_FLAG:
            raise Exception('Invalid packet start')
        message_id = ret_packet[1]
        payload_length = ret_packet[3] << 8 | ret_packet[2]
        if len(ret_packet) != payload_length + 7:
            raise Exception('Invalid packet length', len(ret_packet), payload_length)

        # TODO: Check checksum

        return (message_id, ret_packet[5:5+payload_length])

    def handshake(self):
        self._send(MESSAGE_ID_HANDSHAKE, [])
        ret = self._receive(7)
        if ret[0] != MESSAGE_ID_ACK:
            return False
        return True