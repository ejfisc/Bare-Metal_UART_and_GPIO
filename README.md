# STM32F446 Bare-Metal Practice Project

A small, in-progress firmware project for the STM32F446RE, written in C and programmed directly through peripheral registers, with no ST HAL or generated code. 
I built it to get hands-on, register-level experience working from the reference manual, and to practice structuring embedded software using the architecture 
patterns in *Making Embedded Systems* (Elecia White).

## What it does

- **Button input:** the user button triggers an external interrupt (EXTI, falling edge) and the press is reported over the serial console.
- **UART (USART2), both directions:** received characters are handled in an RX interrupt, and output is sent with a buffer-based write function.
- **LED control:** typing `h` or `l` in a serial terminal drives the on-board LED.
- **SysTick:** configured as the system timebase and used for logger timestamps

## Status and roadmap

- [x] GPIO peripheral driver for button input and LED output
- [x] clock driver to initialize 16MHz HSI as system clock
- [x] SysTick ARM core peripheral driver to blink the LED
- [x] USART peripheral driver for printing to serial console and receiving commands
- [x] Logger module with SysTick timestamps, using hand-written formatting instead of STDIO to keep it small and to practice working under resource constraints
- [ ] Event queue module that decouples interrupt handlers from application logic
- [ ] Button double and triple click detection with software debounce
- [ ] DMA for USART driver
- [ ] FreeRTOS implementation

## Hardware

- STM32F446RE (Cortex-M4)
- USB serial connection to the host at 115200, 8N1

## Build and flash

[Toolchain and commands, e.g. arm-none-eabi-gcc plus Make, and how you flash it]

```
[build command]
[flash command]
```

## Project layout

```
[Short tree of the main directories and files]
```

## References

- RM0390: STM32F446xx reference manual
- STM32F446xx datasheet
- Cortex-M4 generic user guide
- Elecia White, *Making Embedded Systems*
- Israel Gbati, *Bare-Metal Embedded C Programming*
