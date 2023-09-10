import math
import os

NUMBER_OF_SAMPLES = 128
RESOLUTION = 2**12
FILE_NAME = "waveform.hpp"
AMPLITUDE = 0.3
MIN_VALUE = 90

out_str = """/**
 * @file waveform.hpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief
 * @date 2023-08-20
 * @copyright Copyright (c) 2023
 */

#ifndef WAVEFORM_HPP_
#define WAVEFORM_HPP_

#include <etl/array.h>

"""

out_str += "static etl::array<uint32_t, " + str(NUMBER_OF_SAMPLES) + "> SineWaveForm = {"
for i in range(NUMBER_OF_SAMPLES):
    sin_val = math.sin(i / NUMBER_OF_SAMPLES * 2 * math.pi)
    sample_val = int((sin_val + 1) * RESOLUTION / 2) * AMPLITUDE + MIN_VALUE
    out_str += str(int(sample_val)) + ", "
    if i % 10 == 0:
        out_str += "\n"
out_str += "};\n\n"

# Build the same waveform but with a 180 degree phase shift
out_str += "static etl::array<uint32_t, " + str(NUMBER_OF_SAMPLES) + "> SineWaveForm180 = {"
for i in range(NUMBER_OF_SAMPLES):
    sin_val = math.sin(i / NUMBER_OF_SAMPLES * 2 * math.pi + math.pi)
    sample_val = int((sin_val + 1) * RESOLUTION / 2) * AMPLITUDE + MIN_VALUE
    out_str += str(int(sample_val)) + ", "
    if i % 10 == 0:
        out_str += "\n"

out_str = out_str[:-2] + "};\n\n"
out_str += "#endif /* WAVEFORM_HPP_ */"

script_dir_path = os.path.dirname(os.path.abspath(__file__))

with open(script_dir_path + "/" + FILE_NAME, "w") as f:
    f.write(out_str)