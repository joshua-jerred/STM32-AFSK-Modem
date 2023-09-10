import json
import time
import os
import logging
import logging.config

from comms import Comms

path_to_log_config = os.path.join(os.path.dirname(__file__), 'log.conf')
logging.config.fileConfig(path_to_log_config)
log = logging.getLogger("external_host")

log.debug('Starting external host')

path_to_config = os.path.join(os.path.dirname(__file__), 'config.json')
log.debug(f'Loading config from {path_to_config}')
config = None
with open(path_to_config) as f:
    config = json.load(f)

comms = Comms(config['serial_port'], config['baud_rate'], config['timeout_seconds'])
comms.connect()

def getInt(prompt, min, max):
    while True:
        try:
            ret = int(input(prompt))
            if ret < min or ret > max:
                print('Invalid input')
                continue
            return ret
        except:
            print('Invalid input')

def getOptions(prompt, options):
    while True:
        print(prompt)
        for i, option in enumerate(options):
            print(f'{i+1}) {option}')
        ret = getInt('Enter choice: ', 1, len(options))
        return options[ret-1]

while True:
    options = ['Handshake', 'Set AAR', 'Quit']
    selection = getOptions('Select an option:', options)
    print()
    if selection == 'Quit':
        break
    elif selection == 'Handshake':
        if comms.handshake():
            print('Handshake successful')
        else:
            print('Handshake failed')
    elif selection == 'Set AAR':
        new_val = getInt("Enter new AAR value for the timer 1-2000:", 1, 2000)
        if comms.setAar(new_val):
            print('New TX successful')
        else:
            print('New TX failed')
    print()