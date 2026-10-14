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
 * initializes GPIO, enables peripheral, and configures USART
 */
void usart2_init(void);

/**
 * disables interrupts and peripheral
 */
void usart2_close(void);

/**
 * writes a single byte to the transmission data register
 * @param tx byte to write
 */
void usart2_write(uint8_t tx);

/**
 * writes len bytes from buffer
 * @param buf data buffer to send
 * @param len number of bytes from data buffer to send
 */
void usart2_write_buf(const uint8_t* buf, uint16_t len);

char usart2_read(void);
/**
 * usart2 io control function - executes the given command
 * @param cmd IO control command to execute
 * @param data optional value for specific commands (i.e. SET_BAUDRATE requires a baudrate)
 */
void usart2_ioctl(USART_IOCTL_t cmd, uint32_t data);


#endif //USART2_H
