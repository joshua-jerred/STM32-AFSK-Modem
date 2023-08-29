import json
import time
import os

from comms import Comms

path_to_config = os.path.join(os.path.dirname(__file__), 'config.json')
config = None
with open(path_to_config) as f:
    config = json.load(f)

comms = Comms(config['serial_port'], config['baud_rate'], config['timeout_seconds'])
comms.connect()

while True:
    if not comms.handshake():
        print('Handshake failed')
        exit()
    time.sleep(0.05)