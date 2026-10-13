//
// Created by Ethan Fischer on 10/11/26.
//

#ifndef LOGGER_H
#define LOGGER_H

#include "../device_headers/stm32f446xx.h"
#include "../device_drivers/usart2.h"
#include "../device_drivers/systick.h"

#define NUM_LOG_SUBSYSTEMS 3

typedef enum {
    LOG_SYS = 0,
    LOG_BTN,
    LOG_CMD,
} log_sub_t;

typedef enum {
    ERROR = 0,
    WARN,
    INFO,
    VERBOSE
} log_level_t;

void log_init(void);
void log_close(void);
void log_print(log_sub_t sys, log_level_t lvl, uint32_t timestamp, char* msg);


#endif //LOGGER_H
