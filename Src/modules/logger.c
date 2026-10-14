//
// Created by Ethan Fischer on 10/11/26.
//

#include "modules/logger.h"

static log_struct_t log_data;
static uint8_t format_time(char* buf, uint32_t ms);
static void put_digits(char* p, uint32_t v, uint32_t n);
static uint16_t concat(char *dst, uint32_t pos, uint32_t max, const char *src);

void log_init(void) {
    for (int i = 0; i < NUM_LOG_SUBSYSTEMS; i++) {
        log_data.output_level[i] = LOG_LVL_ERROR;
    }
    log_global_on();
}

void log_print(const log_subsystem_t sys, const log_level_t lvl, const char* msg) {
    if (log_data.log_enable == false) { return; }
    if (lvl > log_data.output_level[sys]) { return; }
    char line[MAX_BUF_LEN];
    uint8_t len = format_time(line, systick_millis());
    len = concat(line, len, MAX_BUF_LEN, " ");
    len = concat(line, len, LOG_MAX, level_name[lvl]);
    len = concat(line, len, MAX_BUF_LEN, " ");
    len = concat(line, len, LOG_MAX, sub_name[sys]);
    len = concat(line, len, MAX_BUF_LEN, ":");
    len = concat(line, len, MAX_BUF_LEN, " ");
    len = concat(line, len, LOG_MAX, msg);
    line[len++] = '\r';
    line[len++] = '\n';
    usart2_write_buf((uint8_t*)line, len);
}

void log_set_output_level(const log_subsystem_t sys, const log_level_t lvl) {
    log_data.output_level[sys] = lvl;
}

void log_global_on(void) {
    log_data.log_enable = true;
}

void log_global_off(void) {
    log_data.log_enable = false;
}

// appends src onto destination
uint16_t concat(char *dst, uint32_t pos, uint32_t max, const char *src) {
    while (*src != '\0' && pos < max) {
        dst[pos++] = *src++;
    }
    return pos;
}

// Writes n decimal digits of v into p, zero padded.
static void put_digits(char* p, uint32_t v, uint32_t n)
{
    while (n > 0U) {
        n--;
        p[n] = (uint8_t)('0' + (v % 10U));
        v /= 10U;
    }
}

// Writes "[HH:MM:SS.mmm]" (14 bytes, no terminator) into buf. Returns 14.
uint8_t format_time(char* buf, const uint32_t ms) {
    uint32_t total_s = ms / 1000U;

    buf[0] = '[';
    put_digits(&buf[1], (total_s / 3600U) % 100U, 2U);
    buf[3] = ':';
    put_digits(&buf[4], (total_s / 60U) % 60U, 2U);
    buf[6] = ':';
    put_digits(&buf[7], total_s % 60U, 2U);
    buf[9] = '.';
    put_digits(&buf[10], ms % 1000U, 3U);
    buf[13] = ']';
    return 14U;
}
