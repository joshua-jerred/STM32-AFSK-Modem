# Digital to Analog Converter (DAC) and Direct Memory Access (DMA)

Reference: AN3126 (these are notes, not unique work, a lot of direct quotes here)

## DAC
A DAC, or digital to analog converter, is a digital device that outputs a voltage proportional to a digital input.

For example, the STM32F429 is a 3.3v device. It has a 12-bit DAC, which means it can output 4096 different voltages between 0 and 3.3v.

The DAC is a bit of physical hardware that is inside of the microcontroller (MCU) and it can be interfaced with through the MCU's registers. The DAC is connected to a pin on the MCU.

### DAC Data Format
There are a few configuration options, I have chosen to go with the 12-bit right aligned format.

### DAC Trigger/Timing
The STM32 has hardware timers that run independently of the CPU. These timers are configured off of the clock signal, which will be 180MHz for this application.

The DAC interface can be configured to detect a rising edge on a selected timer trigger output. When this trigger is detected, the DAC will output the value in the DAC_DHRx register to the DAC_DORx register.

There are also software triggers, but that will not be used here.

### DAC Registers

## DMA
Each DAC channel (stream) is connected to a DMA channel. When DMA is not used, the CPU is responsible for writing to the correct register.

When using the DMA, the CPU writes a series of samples to memory, and the DMA controller will write these samples to the DAC. This frees up the CPU to do other things.

### DMA under run error
> When the DMA is used to provide DAC with the waveform digital code, there are cases where the DMA transfer is slower than the DAC conversion. In these cases, the DAC detects that a part of the pattern waveform has not been received and cannot be converted, and then sets the “DMA under run error” flag.

## Sine Wave Generation
To generate a sine wave from 0 to 2pi, we need to generate some samples.

n_s = number of samples

sampling_step: 2pi / n_s

As the DAC is 12-bit, we need to scale the samples to 0-4095 (0xFFF)

Sample = (sin(x * sampling_step) + 1) * 2048

DAC Output = Vref * (Sample / 4095) = 3.3 * (Sample / 4095)

### Frequency
The frequency of the sine wave is determined by the frequency of the timer trigger output.

Fsine = Ftimer / n_s

So if the TIMx_TRGO output frequency is 1 MHz, then the sine wave frequency will be 1 MHz / n_s, with 10 samples, the sine wave frequency will be 100 kHz.

> To get close to the targeted monotone waveform, it is recommended to use the highest possible number of samples ns (the difference can be easily understood by comparing Figure 13 with Figure 14).

# Implementation

Source: https://deepbluembedded.com/stm32-dac-sine-wave-generation-stm32-dac-dma-timer-example/

Example:
n_s = 128 (samples)
Fclk = 180 MHz (clock frequency)
PSC = 0 (prescaler)
APR = 179 (auto reload register)

TriggerFrequency = Fclk / ((PSC + 1) * (APR + 1))
OutputFrequency = TriggerFrequency / n_s