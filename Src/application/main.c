
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
#include "device_drivers/gpio.h"
#include "device_drivers/systick.h"
#include "device_drivers/usart2.h"

/* LED State */
#define LED_ON 1U
#define LED_OFF 0U
/* Button State */
#define BUTTON_PRESSED 0U // active low
#define BUTTON_RELEASED 1U
// #define DEBOUNCE_MS 20
// #define GAP_MS 300
// #define MAX_PRESSES 3
//
// typedef enum { BTN_NONE, BTN_SINGLE, BTN_DOUBLE, BTN_TRIPLE } btn_event_t;


static uint32_t LED_state = LED_OFF;
static volatile uint32_t button_state = BUTTON_RELEASED;
static volatile uint32_t buttonPressCount = 0;
static volatile char usart2_rx;
static volatile bool usart2_rx_cplt = false;

/*
 * HSI 16MHz -> SYSCLK / 1 -> HCLK / 1 -> APB1_CLK / 1 = 16MHz
 */
static void configure_clocks(void)
{
  // set system clock to HSI 16MHz
  RCC->CFGR &= ~(RCC_CFGR_SW);

  // set AHB1 prescaler to not divided
  RCC->CFGR &= ~(RCC_CFGR_HPRE);

  // set APB1 prescaler to not divided
  RCC->CFGR &= ~(RCC_CFGR_PPRE1);
}


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
  LED_state = LED_ON;
}

static void led_off()
{
  GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_LOW);
  LED_state = LED_OFF;
}


static void toggle_led()
{
  if (LED_state == LED_ON)
  {
    GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_LOW);
    LED_state = LED_OFF;
  }
  else
  {
    GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_HIGH);
    LED_state = LED_ON;
  }
}

/**
  * @brief This function handles EXTI line[15:10] interrupts. - cannot be made static, can't be called by NVIC if it's static
  */
void EXTI15_10_IRQHandler(void)
{
  // check if the interrupt happened on line 13
  if (EXTI->PR & EXTI_PR_PR13) {
    EXTI->PR = EXTI_PR_PR13; // clear using '=' not '|=' which would re-write a 1 to every other pending bit and clear it
    button_state = BUTTON_PRESSED;
  }
}

void USART2_IRQHandler(void)
{
  usart2_rx = (char)USART2->DR;
  usart2_rx_cplt = true;
}


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  configure_clocks();
  led_init();
  button_init();
  systick_init();
  usart2_init();

  while (1)
  {
    if (usart2_rx_cplt) {
      // usart2_write_buf("Received: ");
      // usart2_write(usart2_rx);
      // usart2_write_buf("\r\n");
      if (usart2_rx == 'h') {
        led_on();
      }
      else if (usart2_rx == 'l') {
        led_off();
      }
      usart2_rx_cplt = false;
    }

    if (button_state == BUTTON_PRESSED) {
      usart2_write_buf("button pressed!\r\n");
      button_state = BUTTON_RELEASED;
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

