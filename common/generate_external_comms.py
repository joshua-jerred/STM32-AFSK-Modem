# A simple generator script for creating packet ID enumerations

import os
import json5 as json # for jsonc support

from generator.output import PythonFile as PythonFile
from generator.output import CppFile as CppFile
from generator.components.enum import Enum as Enum

script_dir_path = os.path.dirname(os.path.realpath(__file__))

INPUT_JSONC_FILE = script_dir_path + '/external_coms.jsonc'
OUTPUT_PYTHON_FILE = script_dir_path + '/../external_host/external_comms_types.py'
OUTPUT_CPP_FILE = script_dir_path + '/../stm32_afsk/src/external_comms/external_comms_types.hpp'

# Read in the JSONC file
data = None
with open(INPUT_JSONC_FILE) as f:
    data = json.load(f)

packet_ids_json = data['MESSAGE_IDS']
packet_ids_enum = Enum("MessageId", enum_class=True, enum_type='uint8_t')
for token_name in packet_ids_json:
    packet_ids_enum.addToken(token_name, token_value=packet_ids_json[token_name]['id'], token_comment=packet_ids_json[token_name]['description'])

error_ids_json = data['ERROR_IDS']
error_ids_enum = Enum("ErrorId", enum_class=True, enum_type='uint8_t')
for token_name in error_ids_json:
    error_ids_enum.addToken(token_name, token_value=error_ids_json[token_name]['id'], token_comment=error_ids_json[token_name]['description'])


python = PythonFile(out_file_path=OUTPUT_PYTHON_FILE)
python.addComponents(packet_ids_enum)
python.addComponents(error_ids_enum)
python.write()

cpp = CppFile(out_file_path=OUTPUT_CPP_FILE, include_guard='PACKET_IDS_HPP_')
cpp.addIncludes('<cstdint>')
cpp.addComponents(packet_ids_enum)
cpp.addComponents(error_ids_enum)
cpp.write()