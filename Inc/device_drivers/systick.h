//
// Created by Ethan Fischer on 10/7/26.

#ifndef SYSTICK_H
#define SYSTICK_H

#include "../device_headers/stm32f446xx.h"
#include <stdint.h>

#define INTERNAL_CLK_SPEED 16000000U // change this if AHB clock is configured differently
#define ONE_MS_LOAD ((INTERNAL_CLK_SPEED / 1000U) - 1U)


/**
 *  initializes system tick timer with internal clock source and sets the current value to 0
 */
void systick_init(void);

/**
 *  system tick countdown interrupt handler
 */
void SysTick_Handler(void);

/**
 *  provides global access to ms_ticks which needs to be static so nothing can write to it
 * @return current milliseconds on the system tick timer
 */
uint32_t systick_millis(void);

/**
 *  uses the system tick timer to delay
 * @param delay time to delay in milliseconds
 */
void systick_ms_delay(uint32_t delay);

#endif //SYSTICK_H
