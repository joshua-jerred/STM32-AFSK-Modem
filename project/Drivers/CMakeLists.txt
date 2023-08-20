add_library(CMSIS INTERFACE)
target_include_directories(CMSIS INTERFACE 
  Include
  CMSIS/Device/ST/STM32F4xx/Include
  CMSIS/Core/Include
)

add_library(STM32F4XX_HAL_DRIVER INTERFACE)
target_sources(STM32F4XX_HAL_DRIVER INTERFACE
  # HAL
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac_ex.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
  STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_adc.c

  # LL
  # STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c
)
target_include_directories(STM32F4XX_HAL_DRIVER INTERFACE Inc)
target_link_libraries(STM32F4XX_HAL_DRIVER INTERFACE ${STM32_INCLUDE_LIB})