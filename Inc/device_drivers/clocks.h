//
// Created by Ethan Fischer on 10/13/26.

#ifndef CLOCKS_H
#define CLOCKS_H

#include "../device_headers/stm32f446xx.h"

#define SYS_CLK_SPEED 16000000U


/**
 * HSI 16MHz -> SYSCLK / 1 -> HCLK / 1 -> APB1_CLK / 1 = 16MHz
 */
void clocks_16MHz_init(void);

#endif //CLOCKS_H
