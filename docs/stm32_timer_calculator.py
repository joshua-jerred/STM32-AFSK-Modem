APB1_CLK    = 90
PSC         = 0
ARR         = 159
NUM_SAMPLES = 256

trigger_freq = APB1_CLK / ((PSC+1)*(ARR+1))
output_freq = trigger_freq / NUM_SAMPLES

print("Trigger Freq: " + str(trigger_freq))
print("Output Freq (hz): " + str(output_freq*1000000))