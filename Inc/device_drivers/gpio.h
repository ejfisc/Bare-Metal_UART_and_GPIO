//
// Created by Ethan Fischer on 9/20/26.
//

#ifndef GPIO_H_
#define GPIO_H_

#include "../device_headers/stm32f446xx.h"
#include <stdbool.h>

#define GPIO_PIN_0  0U
#define GPIO_PIN_1  1U
#define GPIO_PIN_2  2U
#define GPIO_PIN_3  3U
#define GPIO_PIN_4  4U
#define GPIO_PIN_5  5U
#define GPIO_PIN_6  6U
#define GPIO_PIN_7  7U
#define GPIO_PIN_8  8U
#define GPIO_PIN_9  9U
#define GPIO_PIN_10 10U
#define GPIO_PIN_11 11U
#define GPIO_PIN_12 12U
#define GPIO_PIN_13 13U
#define GPIO_PIN_14 14U
#define GPIO_PIN_15 15U

#define GPIO_HIGH   1U
#define GPIO_LOW    0U

#define LCK_BIT_POS (1U << 16)

typedef enum {
    GPIO_MODE_INPUT     = 0U,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_ALTERNATE,
    GPIO_MODE_ANALOG,
} GPIO_MODE_t;

typedef enum {
    GPIO_OTYPE_PUSHPULL = 0U,
    GPIO_OTYPE_OPENDRAIN
} GPIO_OTYPE_t;

typedef enum {
    GPIO_OSPEED_LOW = 0U,
    GPIO_OSPEED_MEDIUM,
    GPIO_OSPEED_HIGH,
    GPIO_OSPEED_FAST
} GPIO_OSPEED_t;

typedef enum {
    GPIO_PULL_NONE = 0U,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN,
} GPIO_PULL_t;

/**
 * Initialize a GPIO pin with basic configuration options
 * @param port GPIO port base address (e.g., GPIOA)
 * @param pin Pin number (0-15)
 * @param mode Pin mode (input, output, alternate, analog)
 * @param outputType Push-pull or open-drain
 * @param outputSpeed Output speed selection
 * @param pullUpDown Pull-up, pull-down, or no pull resistor
 */
void GPIO_Init(GPIO_TypeDef * port,
               uint8_t pin,
               GPIO_MODE_t mode,
               GPIO_OTYPE_t outputType,
               GPIO_OSPEED_t outputSpeed,
               GPIO_PULL_t pullUpDown);

/**
 * Write a GPIO pin high or low
 * @param port GPIO port base address (e.g., GPIOA)
 * @param pin Pin number (0-15)
 * @param value High, low
 */
void GPIO_WritePin(GPIO_TypeDef * port,
                   uint8_t pin,
                   uint8_t value);


/**
 * Locks GPIO pin configuration for the pins set in the mask
 * @param port GPIO port base address (e.g., GPIOA)
 * @param mask Pin 0..15
 * @return true if lock key is active
 */
bool GPIO_LockPins(GPIO_TypeDef * port, uint32_t mask);


#endif //GPIO_H_
