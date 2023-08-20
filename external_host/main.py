import json
import time
from comms import Comms

config = None
with open('config.json') as f:
    config = json.load(f)

comms = Comms(config['serial_port'], config['baud_rate'], config['timeout_seconds'])
comms.connect()

while True:
    if not comms.handshake():
        print('Handshake failed')
        exit()
    time.sleep(0.05)