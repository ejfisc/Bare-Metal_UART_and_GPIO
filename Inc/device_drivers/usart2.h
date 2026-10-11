//
// Created by Ethan Fischer on 10/9/26.
//

#ifndef USART2_H
#define USART2_H

#include "../device_headers/stm32f446xx.h"
#include "gpio.h"
#include <stdint.h>

#define UART_BAUDRATE   115200U
#define SYS_CLK_SPEED   16000000U
#define APB1_CLK_SPEED  SYS_CLK_SPEED

void usart2_init(void);
void usart2_write(char tx);
void usart2_write_buf(const char* tx);
char usart2_read(void);


#endif //USART2_H
