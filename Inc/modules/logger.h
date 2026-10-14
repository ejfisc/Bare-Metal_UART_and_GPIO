//
// Created by Ethan Fischer on 10/11/26.
//

#ifndef LOGGER_H
#define LOGGER_H

#include "../device_headers/stm32f446xx.h"
#include "../device_drivers/usart2.h"
#include "../device_drivers/systick.h"

#define NUM_LOG_SUBSYSTEMS 3
#define LOG_MAX (MAX_BUF_LEN - 2U)

typedef enum {
    LOG_SYS = 0,
    LOG_BTN,
    LOG_CMD,
} log_subsystem_t;

static const char *const sub_name[NUM_LOG_SUBSYSTEMS] = { "SYS", "BTN", "CMD" };

typedef enum {
    LOG_LVL_NONE = 0,
    LOG_LVL_ERROR,
    LOG_LVL_WARN,
    LOG_LVL_INFO,
    LOG_LVL_DEBUG
} log_level_t;

static const char *const level_name[4] = { "ERROR", "WARNING", "INFO", "DEBUG" };

typedef struct {
    bool log_enable;
    log_level_t output_level[NUM_LOG_SUBSYSTEMS];
} log_struct_t;


/**
 * Enables Log and sets output level for each subsystem to ERROR
 */
void log_init(void);

/**
 * logs "[TIMESTAMP] LVL SYS: MSG" (if output level is > log_data.output_level[sys] it won't get printed)
 * @param sys subsystem sending the message (i.e. BTN)
 * @param lvl log level (i.e. INFO)
 * @param msg message to log
 */
void log_print(log_subsystem_t sys, log_level_t lvl, const char* msg);


/**
 * sets the output level for the subsystem - only messages with a level lower (higher priority) are actually logged
 * @param sys subsystem to set the output level for (i.e. BTN)
 * @param lvl log level (i.e. INFO)
 */
void log_set_output_level(log_subsystem_t sys, log_level_t lvl);

/**
 * enables log
 */
void log_global_on(void);

/**
 * disables log to turn off all prints regardless of output level
 */
void log_global_off(void);

#endif //LOGGER_H
