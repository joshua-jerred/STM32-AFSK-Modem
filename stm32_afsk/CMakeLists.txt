cmake_minimum_required(VERSION 3.6)

project(afsk_link CXX C ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_policy(SET CMP0076 NEW)

# Add BoosterSeat
option(BOOSTERSEAT_EMBEDDED "" ON)
option(ARM_NONE_EABI "" ON)
option(BST_EMBD_STM32F429 "" ON)
add_subdirectory(lib/booster_seat)

# Add FreeRTOS
add_subdirectory(lib/FreeRTOS)

# Add the executable
add_executable(afsk_link.elf
  src/main.cpp
  src/hardware/hardware_init.cpp
  src/external_comms/external_comms.cpp

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
    src/hardware
    src/external_comms
)
target_link_libraries(afsk_link.elf
    PUBLIC
      BoosterSeat
      FreeRTOS
)

# Linker Configuration
set(LINKER_SCRIPT STM32F429ZITX_FLASH.ld)
configure_file(${LINKER_SCRIPT} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
set(CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT}")

# Launch Command
add_custom_target(launch
  COMMAND arm-none-eabi-objcopy -O binary ${CMAKE_CURRENT_BINARY_DIR}/afsk_link.elf ${CMAKE_CURRENT_BINARY_DIR}/afsk_link.bin
  COMMAND st-flash --reset write ${CMAKE_CURRENT_BINARY_DIR}/afsk_link.bin 0x8000000
  DEPENDS afsk_link.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)