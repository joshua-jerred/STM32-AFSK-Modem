def generateCrc16Ccitt(data):
    crc = 0xFFFF
    for byte in data:
        crc ^= byte << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = (crc << 1) ^ 0x1021
            else:
                crc <<= 1
    return crc & 0xFFFF

# A simple test
if __name__ == "__main__":
    data_1 = [0xa5, 0x01, 0x00, 0x00, 0xfb, 0xac, 0xa5]
    crc_1 = generateCrc16Ccitt(data_1[1:4])
    data_2 = [0x01, 0x00, 0x00]
    crc_2 = generateCrc16Ccitt(data_2)

    print(hex(crc_1), hex(crc_2))


