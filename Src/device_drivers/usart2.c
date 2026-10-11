//
// Created by Ethan Fischer on 10/9/26.
//

#include "device_drivers/usart2.h"

#define UART_BRR(clk, baud) (((clk) + (baud / 2U)) / (baud))

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);

void usart2_init(void)
{
    // turn on GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // configure GPIO pins
    GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_ALTERNATE, GPIO_OTYPE_PUSHPULL, GPIO_OSPEED_LOW, GPIO_PULL_NONE);
    GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_ALTERNATE, GPIO_OTYPE_PUSHPULL, GPIO_OSPEED_LOW, GPIO_PULL_NONE);
    GPIO_SetAlternateFunction(GPIOA, GPIO_PIN_2, GPIO_AF7); // AF7 on PA2 is USART2_TX
    GPIO_SetAlternateFunction(GPIOA, GPIO_PIN_3, GPIO_AF7); // AF7 on PA3 is USART2_RX

    // enable USART2 peripheral
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // set oversampling mode to oversampling by 16
    USART2->CR1 &= ~(USART_CR1_OVER8);

    uart_set_baudrate(APB1_CLK_SPEED, UART_BAUDRATE);

    // enable USART and turn on transmitter and receiver
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_UE;
    NVIC_EnableIRQ(USART2_IRQn);
}

void usart2_write(const char tx)
{
    // make sure transmit data register is empty
    while (!(USART2->SR & USART_SR_TXE)) {}

    // write to transmit data register
    USART2->DR = tx;
}

void usart2_write_buf(const char* tx)
{
    while (*tx != '\0') {
        usart2_write(*tx);
        tx++;
    }
}

char usart2_read(void)
{
    while (!(USART2->SR & USART_SR_RXNE)) {}
    return ((char)USART2->DR);
}



static void uart_set_baudrate(const uint32_t periph_clk, const uint32_t baudrate)
{
    USART2->BRR = UART_BRR(periph_clk, baudrate);
}
