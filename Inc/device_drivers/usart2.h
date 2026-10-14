//
// Created by Ethan Fischer on 10/9/26.
//

#ifndef USART2_H
#define USART2_H

#include "../device_headers/stm32f446xx.h"
#include "../device_drivers/clocks.h"
#include "../device_drivers/gpio.h"

#define USART2_BAUDRATE   115200U
#define PERIPH_CLK_SPEED  SYS_CLK_SPEED
#define MAX_BUF_LEN 256U

typedef enum {
    SET_BAUDRATE = 0
} USART_IOCTL_t;

/**
 * initializes GPIO, enables peripheral,
 */
void usart2_init(void);
void usart2_close(void);
void usart2_write(uint8_t tx);
void usart2_write_buf(const uint8_t* buf, uint16_t len);
char usart2_read(void);
void usart2_ioctl(USART_IOCTL_t cmd, uint32_t data);


#endif //USART2_H
