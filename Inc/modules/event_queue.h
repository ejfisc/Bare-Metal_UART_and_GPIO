//
// Created by Ethan Fischer on 10/11/26.
//

#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "../device_headers/stm32f446xx.h"
#include <stdint.h>

#define MAX_SIZE 100

typedef enum {
    EVT_BTN_EDGE = 0,
    EVT_CLICK_SINGLE,
    EVT_CLICK_DOUBLE,
    EVT_CLICK_TRIPLE,
    EVT_COMMAND
} event_type_t;

typedef struct {
    event_type_t type;
    uint32_t t_ms;
    uint8_t data;
} event_t;

typedef struct {
    event_t items[MAX_SIZE];
    event_t front;
    event_t rear;
} event_queue_t;



#endif //EVENT_QUEUE_H
