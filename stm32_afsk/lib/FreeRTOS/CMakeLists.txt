add_library(FreeRTOS INTERFACE)
target_include_directories(BoosterSeat INTERFACE 
  "include"
  "portable/GCC/ARM_CM4F"
  "CMSIS_RTOS_V2"
)
target_sources(FreeRTOS INTERFACE
  "CMSIS_RTOS_V2/cmsis_os2.c"
  "portable/GCC/ARM_CM4F/port.c"
  "portable/MemMang/heap_4.c"
  "croutine.c"
  "event_groups.c"
  "list.c"
  "queue.c"
  "stream_buffer.c"
  "tasks.c"
  "timers.c"
)