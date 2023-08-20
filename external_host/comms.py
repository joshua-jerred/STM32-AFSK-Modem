import serial

PACKET_START1 = 0xAA
PACKET_START2 = 0x55

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
        # length_bytes = len(payload).to_bytes(2, 'big')
        packet = bytearray([PACKET_START1, PACKET_START2, message_id, 0, 0] + payload + [0x00, 0x00])
        self.serial.write(packet)

    def _receive(self, expected_length):
        ret_packet = list(self.serial.read(expected_length))
        if ret_packet[0] != PACKET_START1 or ret_packet[1] != PACKET_START2:
            raise Exception('Invalid packet start')
        message_id = ret_packet[2]
        payload_length = ret_packet[4] << 8 | ret_packet[3]
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