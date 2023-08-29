# Toolchain

Using CMake and ARM GCC toolchain (none-eabi).

The toolchain can be installed on Ubuntu type systems with:

```bash
sudo apt install gcc-arm-none-eabi
```
There is configuration for the toolchain in the `CMakeLists.txt` file.

There is also a configuration setup for VSCode that will allow you to build the project using the CMake extension. This is recommended as it takes care of all of the details for you.

## CMake Configuration
```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=./lib/booster_seat/toolchains/arm_none_eabi_gcc.cmake -S./stm32_afsk -B./build/arm_none_eabi_gcc -G Ninja
```

## Build
```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=./lib/booster_seat/toolchains/arm_none_eabi_gcc.cmake -S./stm32_afsk -B./build/arm_none_eabi_gcc -G Ninja
```

## Flash
Currently just using the STM32CubeIDE to flash and debug. Set `./stm32_cubeIDE_workspace` as your workspace for debug configurations.