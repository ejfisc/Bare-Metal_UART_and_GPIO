//
// Created by Ethan Fischer on 10/9/26.
//

#ifndef USART2_H
#define USART2_H

#include "../device_headers/stm32f446xx.h"
#include "../device_drivers/clocks.h"
#include "../device_drivers/gpio.h"
#include <stdint.h>

#define USART2_BAUDRATE   115200U
#define PERIPH_CLK_SPEED  SYS_CLK_SPEED

void usart2_init(void);
void usart2_close(void);
void usart2_write(char tx);
void usart2_write_buf(const char* tx);
char usart2_read(void);
void usart2_ioctl_set_baudrate(uint32_t baudrate);


#endif //USART2_H
