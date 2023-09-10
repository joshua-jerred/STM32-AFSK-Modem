APB1_CLK    = 90
PSC         = 0
ARR         = 319 # 585 = 1200, 319 = 2200
NUM_SAMPLES = 128

trigger_freq = APB1_CLK / ((PSC+1)*(ARR+1))
output_freq = trigger_freq / NUM_SAMPLES

print("Trigger Freq: " + str(trigger_freq))
print("Output Freq (hz): " + str(output_freq*1000000))

## Baud Rate Timer
PSC = 0
ARR = 1200

baud_rate = APB1_CLK / ((PSC+1)*(ARR+1))
print("Baud Rate: " + str(baud_rate))