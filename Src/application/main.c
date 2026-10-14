
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include <stdio.h>

#include "device_headers/stm32f446xx.h"
#include "device_drivers/clocks.h"
#include "device_drivers/gpio.h"
#include "device_drivers/systick.h"
#include "device_drivers/usart2.h"
#include "modules/logger.h"
#include "modules/event_queue.h"

/* LED State */
#define LED_ON 1U
#define LED_OFF 0U
/* Button State */
#define BUTTON_PRESSED 0U // active low
#define BUTTON_RELEASED 1U

typedef struct {
  uint32_t led_state;
  volatile uint32_t button_state;
} app_state_t;

static app_state_t app = {
  .led_state = LED_OFF,
  .button_state = BUTTON_RELEASED
};

volatile uint8_t usart2_rx;
volatile bool usart2_rx_cplt = false;

static void led_init()
{
  // turn the clock on for GPIOA
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  // set PA5 as output
  GPIO_Init(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT,
    GPIO_OTYPE_PUSHPULL, GPIO_OSPEED_LOW, GPIO_PULL_NONE);
}

static void button_init()
{
  // turn on the clock for GPIOC for button
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

  // set PC13 as input
  GPIO_Init(GPIOC, GPIO_PIN_13, GPIO_MODE_INPUT, GPIO_OTYPE_PUSHPULL, GPIO_OSPEED_LOW, GPIO_PULL_NONE);

  // enable syscfg clock
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

  // connect EXTI13 to PC13
  SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

  EXTI->FTSR |= EXTI_FTSR_TR13; // interrupt on falling edge (press)
  // EXTI->RTSR |= EXTI_RTSR_TR13; // and rising edge (release)
  EXTI->IMR |= EXTI_IMR_IM13;

  NVIC_SetPriority(EXTI15_10_IRQn, 1);
  NVIC_EnableIRQ(EXTI15_10_IRQn);
}

static void led_on()
{
  GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_HIGH);
  app.led_state = LED_ON;
}

static void led_off()
{
  GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_LOW);
  app.led_state = LED_OFF;
}


static void toggle_led()
{
  if (app.led_state == LED_ON)
  {
    GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_LOW);
    app.led_state = LED_OFF;
  }
  else
  {
    GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_HIGH);
    app.led_state = LED_ON;
  }
}

/**
  * @brief This function handles EXTI line[15:10] interrupts
  */
void EXTI15_10_IRQHandler(void)
{
  // check if the interrupt happened on line 13
  if (EXTI->PR & EXTI_PR_PR13) {
    EXTI->PR = EXTI_PR_PR13; // clear using '=' not '|=' which would re-write a 1 to every other pending bit and clear it
    app.button_state = BUTTON_PRESSED;
  }
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  clocks_16MHz_init();
  systick_init();
  usart2_init();
  log_init();
  log_set_output_level(LOG_BTN, LOG_LVL_DEBUG);
  led_init();
  button_init();


  while (1)
  {
    if (usart2_rx_cplt) {
      if (usart2_rx == (uint8_t)'h') {
        led_on();
      }
      else if (usart2_rx == (uint8_t)'l') {
        led_off();
      }
      usart2_rx_cplt = false;
    }

    if (app.button_state == BUTTON_PRESSED) {
      log_print(LOG_BTN, LOG_LVL_INFO, "Button Pressed!");
      app.button_state = BUTTON_RELEASED;
    }
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }
}

