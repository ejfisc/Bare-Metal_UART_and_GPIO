# STM32F446 Bare-Metal Practice Project

A small, in-progress firmware project for the STM32F446RE, written in C and programmed directly through peripheral registers, with no ST HAL or generated code. 
I built it to get hands-on, register-level experience working from the reference manual, and to practice structuring embedded software using the architecture 
patterns in *Making Embedded Systems* (Elecia White).

## What it does

- **Button input:** the user button triggers an external interrupt (EXTI, falling edge) and the press is reported over the serial console.
- **UART (USART2), both directions:** received characters are handled in an RX interrupt, and output is sent with a buffer-based write function.
- **LED control:** typing `h` or `l` in a serial terminal drives the on-board LED high or low.
- **SysTick:** configured as the system timebase and used for log timestamps

## Status and roadmap

- [x] GPIO peripheral driver for button input and LED output
- [x] clock driver to initialize 16MHz HSI as system clock
- [x] SysTick ARM core peripheral driver to blink the LED
- [x] USART peripheral driver for printing to serial console and receiving commands
- [x] Logger module with SysTick timestamps, using hand-written formatting instead of STDIO to keep it small and to practice working under resource constraints
- [ ] Event queue module
- [ ] Button double and triple click detection with software debounce
- [ ] DMA for USART driver
- [ ] FreeRTOS implementation

## Hardware

- STM32F446RE (Cortex-M4) Nucleo Board
- USB serial connection to the host at 115200, 8N1

## Build and flash

```
Clean + Build:
cmake --build --target clean --preset Debug
cmake --build --target Bare-Metal_UART_and_GPIO --preset Debug

Flash:
ST-Link_gdbserver -p 61234 -cp /opt/ST/STM32CubeCLT_1.22.0/STM32CubeProgrammer/bin --halt --apid 0 -d --frequency 8000
In a new terminal window:
arm-none-eabi-gdb build/Debug/Bare-Metal_UART_and_GPIO.elf
target extended -remote :61234
load
monitor reset
q
```

## Project layout

```
Inc > device_drivers - contains the device peripheral driver headers (i.e. gpio.h)
    > device_headers - contains the CMSIS headers for the compiler, ARM Cortex-M4, and STM32F446xx
    > modules - contains the header files for the logger and event queue modules
    > stm32f4xx_it.h - header file containing function declarations for Cortex-M4 interrupt and exception handlers
Src > application - contains the main.c application code
    > CMSIS - contains the CMSIS source files for the compiler, ARM Cortex-M4, and STM32F446xx
    > device_drivers - contains the device peripheral driver source files (i.e., gpio.c)
    > modules - contains the source files for the logger and event queue modules
    > stm32f4xx_it.c - source file defining Cortex-M4 interrupt and exception handlers
```

## References

- RM0390: STM32F446xx reference manual
- PM0214: STM32 Cortex-M4 programming manual
- STM32F446xx datasheet
- Cortex-M4 generic user guide
- Elecia White, *Making Embedded Systems*
- Israel Gbati, *Bare-Metal Embedded C Programming*
