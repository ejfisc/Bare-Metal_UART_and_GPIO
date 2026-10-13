//
// Created by Ethan Fischer on 10/9/26.
//

#include "device_drivers/usart2.h"

#define USART_BRR(clk, baud) (((clk) + (baud / 2U)) / (baud))

extern char usart2_rx;
extern bool usart2_rx_cplt;

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

    USART2->CR1 &= ~(USART_CR1_OVER8); // set oversampling mode to oversampling by 16
    usart2_ioctl_set_baudrate(USART2_BAUDRATE);

    USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_UE; // enable USART, transmitter, receiver, and rx interrupt
    NVIC_EnableIRQ(USART2_IRQn);
}

void usart2_close(void) {
    NVIC_DisableIRQ(USART2_IRQn);
    USART2->CR1 = 0UL; // Reset value disables USART, transmitter, receiver, and rx interrupt
    RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN; // disable USART2 peripheral
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
    // wait for data in rx data register
    while (!(USART2->SR & USART_SR_RXNE)) {}
    return ((char)USART2->DR);
}


void usart2_ioctl_set_baudrate(const uint32_t baudrate)
{
    USART2->BRR = USART_BRR(SYS_CLK_SPEED, baudrate);
}

void USART2_IRQHandler(void)
{
    usart2_rx = (char)USART2->DR;
    usart2_rx_cplt = true;
}
