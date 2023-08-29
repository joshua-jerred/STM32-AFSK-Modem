HCLK = 180 # MHz
PRESCALER = 180 # 16-bit
TIMER_SIZE = 16 # bits

time_between_ticks_us = HCLK / PRESCALER
max_time_us = (2**TIMER_SIZE - 1) * time_between_ticks_us

print(f"Time between ticks: {time_between_ticks_us} us {time_between_ticks_us / 1000} ms {time_between_ticks_us / 1000000} s")
print(f"Max time: {max_time_us} us {max_time_us / 1000} ms {max_time_us / 1000000} s")