//
// Created by Ethan Fischer on 9/20/26.
//

#include "device_drivers/gpio.h"

void GPIO_Init(GPIO_TypeDef * port,
                const uint8_t pin,
                const GPIO_MODE_t mode,
                const GPIO_OTYPE_t outputType,
                const GPIO_OSPEED_t outputSpeed,
                const GPIO_PULL_t pullUpDown) {

    port->MODER &= ~(0x3UL << (pin * 2U)); // clear mode bits
    port->MODER |= ((uint32_t)(mode << (pin * 2U))); // set mode

    port->OTYPER &= ~(0x1UL << pin);
    port->OTYPER |= ((uint32_t)(outputType << pin));

    port->OSPEEDR &= ~(0x3UL << (pin * 2U));
    port->OSPEEDR |= ((uint32_t)(outputSpeed << (pin * 2U)));

    port->PUPDR &= ~(0x3UL << (pin * 2U));
    port->PUPDR |= ((uint32_t)(pullUpDown << (pin * 2U)));
}

void GPIO_WritePin(GPIO_TypeDef * port,
                    const uint8_t pin,
                    const uint8_t value) {
    if (value == 1) {
        // turn the pin on
        port->BSRR = (1UL << pin);
    }
    else {
        // turn the pin off
        port->BSRR = (1UL << (pin + 16U));
    }
}

bool GPIO_LockPins(GPIO_TypeDef * port, uint32_t mask) {
    uint32_t gpio_lock = 0;

    gpio_lock = LCK_BIT_POS | mask;
    port->LCKR = gpio_lock;     // LCKR[16] = 1 + LCKR{15:0]
    port->LCKR = mask;          // LCKR[16] = 0 + LCKR[15:0]
    port->LCKR = gpio_lock;     // LCKR[16] = 1 + LCKR[15:0]
    port->LCKR;                 // dummy read to finish lock key write sequence

    if (port->LCKR & LCK_BIT_POS) {
        return true; // lock key active
    }

    return false; // lock key inactive something went wrong

}

