#!/bin/bash

# https://stackoverflow.com/questions/37644823/how-to-flash-stm32-using-only-linux-terminal

BUILD_COMMAND=cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=./lib/booster_seat/toolchains/arm_none_eabi_gcc.cmake -S./stm32_afsk -B./build/arm_none_eabi_gcc -G Ninja

ELF_PATH=build/arm_none_eabi_gcc/afsk_link.elf
BIN_PATH=build/arm_none_eabi_gcc/afsk_link.bin

echo $BUILD_ELF
if [ $BUILD_ELF = "1" ]; then
    echo "Building ELF"
    # Build ELF
    cmake -Bbuild/arm_none_eabi_gcc -DCMAKE_TOOLCHAIN_FILE=cmake/arm_none_eabi_gcc.cmake -DCMAKE_BUILD_TYPE=Debug
    cmake --build build/arm_none_eabi_gcc
fi

# Convert ELF to BIN
arm-none-eabi-objcopy -O binary $ELF_PATH $BIN_PATH

# Flash BIN to STM32
st-flash --reset write $BIN_PATH 0x8000000