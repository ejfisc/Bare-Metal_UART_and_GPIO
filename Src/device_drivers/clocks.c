//
// Created by Ethan Fischer on 10/13/26.
//

#include "device_drivers/clocks.h"

void clocks_16MHz_init(void) {
    // set system clock to HSI 16MHz
    RCC->CFGR &= ~(RCC_CFGR_SW);

    // set AHB1 prescaler to not divided
    RCC->CFGR &= ~(RCC_CFGR_HPRE);

    // set APB1 prescaler to not divided
    RCC->CFGR &= ~(RCC_CFGR_PPRE1);
}
