cmake_minimum_required(VERSION 3.6)

project(afsk_link CXX C ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_policy(SET CMP0076 NEW)

add_compile_definitions(
  STM32F429xx
  USE_HAL_DRIVER
)

add_subdirectory(Drivers)

add_executable(afsk_link.elf
  Core/Src/main.c
  Core/Src/stm32f4xx_hal_msp.c
  Core/Src/stm32f4xx_it.c
  Core/Src/syscalls.c
  Core/Src/sysmem.c
  Core/Src/system_stm32f4xx.c

  Core/Startup/startup_stm32f429zitx.s
)
target_include_directories(afsk_link.elf
  PRIVATE
    Core/Inc
    Drivers/STM32F4xx_HAL_Driver/Inc
    Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/CMSIS/Include
)
target_link_libraries(afsk_link.elf
    PUBLIC
      CMSIS
      STM32F4XX_HAL_DRIVER
)

set(LINKER_SCRIPT STM32F429ZITX_FLASH.ld)
configure_file(${LINKER_SCRIPT} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
set(CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT}")