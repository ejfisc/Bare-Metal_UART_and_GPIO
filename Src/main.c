
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
#include "device_headers/stm32f446xx.h"
#include "device_drivers/gpio.h"

/* LED State */
#define LED_ON 1U
#define LED_OFF 0U
/* Button State */
#define BUTTON_PRESSED 0U // active low
#define BUTTON_RELEASED 1U

static uint32_t LEDState = LED_OFF;
static volatile uint32_t buttonState = BUTTON_RELEASED;
static volatile uint32_t buttonPressCount = 0;

static void gpio_init() {
  // turn the clock on for GPIOA
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  // turn on the clock for GPIOC
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
}

static void led_init()
{
  // set PA5 as output
  GPIO_Init(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT,
    GPIO_OTYPE_PUSHPULL, GPIO_OSPEED_LOW, GPIO_PULL_NONE);
}

static void button_init()
{
  // set PC13 as input
  GPIO_Init(GPIOC, GPIO_PIN_13, GPIO_MODE_INPUT, GPIO_OTYPE_PUSHPULL, GPIO_OSPEED_LOW, GPIO_PULL_NONE);

  // enable syscfg clock
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

  // connect EXTI13 to PC13
  SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

  // enable falling trigger on exti line 13
  EXTI->FTSR |= EXTI_FTSR_TR13;

  // unmask exti line 13
  EXTI->IMR |= EXTI_IMR_IM13;

  // enable exti line 13 interrupt in the NVIC
  NVIC_EnableIRQ(EXTI15_10_IRQn);
}

static void led_on()
{
  GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_HIGH);
  LEDState = LED_ON;
}

static void led_off()
{
  GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_LOW);
  LEDState = LED_OFF;
}


static void toggle_led()
{
  if (LEDState == LED_ON)
  {
    GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_LOW);
    LEDState = LED_OFF;
  }
  else
  {
    GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_HIGH);
    LEDState = LED_ON;
  }
}

/**
  * @brief This function handles EXTI line[15:10] interrupts. - cannot be made static, can't be called by NVIC if it's static
  */
void EXTI15_10_IRQHandler(void)
{
  EXTI->PR |= EXTI_PR_PR13; // clear pending register bit so the interrupt can fire again

  buttonState = BUTTON_PRESSED;
}


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  bool gpio_lock = 0;

  gpio_init();
  led_init();
  button_init();

  gpio_lock = GPIO_LockPins(GPIOC, (1UL << GPIO_PIN_13));

  if (GPIOC->LCKR & LCK_BIT_POS) {
    // blink
    led_on();
    for (uint32_t i = 0; i < 10000U; i++) {}
    led_off();
    for (uint32_t i = 0; i < 10000U; i++) {}
    led_on();
  }

  for (uint32_t i = 0; i < 50000U; i++) {}

  gpio_lock = GPIO_LockPins(GPIOA, (1UL << GPIO_PIN_5));

  if (GPIOA->LCKR & LCK_BIT_POS) {
    // blink
    led_on();
    for (uint32_t i = 0; i < 10000U; i++) {}
    led_off();
    for (uint32_t i = 0; i < 10000U; i++) {}
    led_on();
  }

  while (1)
  {
    // if (buttonState == BUTTON_PRESSED)
    // {
    //   // printf("Button Pressed\r\n");
    //   buttonPressCount++;
    //   if (buttonPressCount == 2) {
    //     toggle_led();
    //     buttonPressCount = 0;
    //   }
    //
    //
    //   buttonState = BUTTON_RELEASED;
    // }
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

