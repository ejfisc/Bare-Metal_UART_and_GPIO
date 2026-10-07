//
// Created by Ethan Fischer on 10/7/26.
//

#include "device_drivers/systick.h"

static volatile uint32_t ms_ticks;

void systick_init(void) {
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // stop while configuring
    SysTick->LOAD = ONE_MS_LOAD - 1;
    SysTick->VAL = 0;
    NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1); // lowest priority
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk // set clock source to internal clock
                    | SysTick_CTRL_TICKINT_Msk // enable interrupt on 0
                    | SysTick_CTRL_ENABLE_Msk; // start countdown
}

void SysTick_Handler(void) {
    ms_ticks++;
}

uint32_t systick_millis(void) {
    return ms_ticks;
}

void systick_ms_delay(const uint32_t delay) {
    uint32_t start = ms_ticks;
    while ((ms_ticks - start) < delay) { __WFI(); }
}
