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
        port->BSRR |= (1UL << pin);
    }
    else {
        // turn the pin off
        port->BSRR |= (1UL << (pin + 16U));
    }
}
