
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

uint32_t LEDState = LED_OFF;
volatile uint32_t buttonState = BUTTON_RELEASED;

void led_init()
{
  // turn the clock on for GPIOA
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  GPIO_Init(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT,
    GPIO_OTYPE_PUSHPULL, GPIO_OSPEED_LOW, GPIO_PULL_NONE);

  // turn LED on - write a 1 to BS5 bit in GPIOA_BSRR
  // GPIOA->BSRR |= 0x20;
  GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
  LEDState = LED_ON;
}

void button_init()
{
  // turn on the clock for GPIOC
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

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

void toggle_led()
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
  * @brief This function handles EXTI line[15:10] interrupts.
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

  led_init();
  button_init();

  while (1)
  {
    if (buttonState == BUTTON_PRESSED)
    {
      // printf("Button Pressed\r\n");
      toggle_led();
      buttonState = BUTTON_RELEASED;
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

