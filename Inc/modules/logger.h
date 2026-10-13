//
// Created by Ethan Fischer on 10/11/26.
//

#ifndef LOGGER_H
#define LOGGER_H

#include "../device_headers/stm32f446xx.h"
#include "../device_drivers/usart2.h"
#include "../device_drivers/systick.h"
#include <string.h>

#define NUM_LOG_SUBSYSTEMS 3

typedef enum {
    LOG_SYS = 0,
    LOG_BTN,
    LOG_CMD,
} log_subsystem_t;

static const char *const sub_name[NUM_LOG_SUBSYSTEMS] = { "SYS", "BTN", "CMD" };

typedef enum {
    ERROR = 0,
    WARN,
    INFO,
    VERBOSE
} log_level_t;

typedef struct {
    bool log_enable;
    log_level_t output_level[NUM_LOG_SUBSYSTEMS];
} log_data;

void log_init(void);
void log_close(void);
void log_print(log_subsystem_t sys, log_level_t lvl, char* msg);
void log_set_output_level(log_subsystem_t sys, log_level_t lvl);


#endif //LOGGER_H
