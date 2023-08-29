# Hardware Configuration

Using an STM32F429ZI Nucleo-144 development board with integrated ST-Link/V2-1 
debugger. The basic hardware configuration was done with STM32CubeMX.

## Pinout

This pinout is for the STM32F429ZI Nucleo-144 development board.

| Pin  | Function    | Notes                         |
| ---- | ----------- | ----------------------------- |
| PC13 | User Button | Built-in button on the Nucleo |
| PB7  | Blue LED    | Blue LED on the Nucleo        |
| PB14 | Red LED     | Red LED on the Nucleo         |
| PB0  | Green LED   | Green LED on the Nucleo       |
| PA4  | DAC_OUT1    | DAC Output                    |
| PA0  | ADC_IN0     | ADC Input                     |
|      |             |                               |

| PD9  | STLK_TX     | ST-Link Connection (UART3/VCP)  |
| PD8  | STLK_RX     | ST-Link Connection (UART3/VCP)  |
| PA14 | TCK         | For ST-Link Connection          |
| PA13 | TMS         | For ST-Link Connection          |
| PH0  | MCO         | RCC_OSC_IN Bypass Clock Source  |
| PH1  | RCC_OSC_OUT | RCC_OSC_OUT Bypass Clock Source |

## Clock Configuration

The STM32F429ZI Nucleo-144 has an external 8 MHz crystal oscillator connected
to the HSE pin. The PLL is configured to use the HSE as its source clock and
is configured to output a 180 MHz clock to SYSCLK (the system clock) and to
HCLK (the hardware clock).

## Timer Configuration
TODO:
  - microsecond timer
  - audio sample timer

## ADC Configuration
TODO

## DAC Configuration
TODO