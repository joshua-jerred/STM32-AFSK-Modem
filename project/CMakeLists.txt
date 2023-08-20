cmake_minimum_required(VERSION 3.6)

project(afsk_link CXX C ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_policy(SET CMP0076 NEW)

add_compile_definitions(
  STM32F429xx
  USE_HAL_DRIVER
)

# Embedded Template Library
add_subdirectory(lib/etl)

# Add BoosterSeat
option(BOOSTERSEAT_EMBEDDED "" ON)
option(ARM_NONE_EABI "" ON)
option(BST_EMBD_STM32F429 "" ON)
add_subdirectory(lib/booster_seat)

# Add the executable
add_executable(afsk_link.elf
  src/main.c
  src/stm32f4xx_hal_msp.c
  src/stm32f4xx_it.c
  src/syscalls.c
  src/sysmem.c
  src/system_stm32f4xx.c

  src/startup_stm32f429zitx.s
)
target_include_directories(afsk_link.elf
  PRIVATE
    src
    Drivers/STM32F4xx_HAL_Driver/Inc
    Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/CMSIS/Include
)
target_link_libraries(afsk_link.elf
    PUBLIC
      BoosterSeat
)

set(LINKER_SCRIPT STM32F429ZITX_FLASH.ld)
configure_file(${LINKER_SCRIPT} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
set(CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT}")