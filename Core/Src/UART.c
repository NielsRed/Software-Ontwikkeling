#include "UART.h"
#include "main.h"
#include <math.h>

/**
  ******************************************************************************
  * @file      Uart.c
  * @version   v 1.0.2
  * @date      17/12/2024
  * @author    Niels Redegeld (Co-Author Chris van Wijk)
  * @brief     This file provides the UART connection with the laptop
  *            via the usb output on the stm32.
  ******************************************************************************
 */

/////////////////////////////////////////////////////////////////////////
  /// @brief Initialize UART2 with interrupts. Sets up baudrate, pins, and NVIC.
  /// @note  This function configures USART2 for communication and enables RX interrupts.
  /// @param baudrate Specifies the baudrate for UART communication.
/////////////////////////////////////////////////////////////////////////
void UART2_Init_Interrupt(uint32_t baudrate) {
    // 1. Schakel de klokken in voor USART2 en GPIOA
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // USART2-klok aan
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // GPIOA-klok aan

    // 2. Stel PA2 (TX) en PA3 (RX) in op alternatieve functie
    GPIOA->MODER &= ~((3 << (2 * 2)) | (3 << (3 * 2))); // Reset mode PA2 en PA3
    GPIOA->MODER |= (2 << (2 * 2)) | (2 << (3 * 2));    // Alternatieve functie
    GPIOA->AFR[0] |= (7 << (4 * 2)) | (7 << (4 * 3));   // Alternatieve functie 7 (USART2)
    GPIOA->OSPEEDR |= (3 << (2 * 2)) | (3 << (3 * 2));  // High speed voor PA2/PA3

    // 3. Configureer USART2
    USART2->CR1 = 0; // Reset alle instellingen
    USART2->BRR = SystemCoreClock / 4 / baudrate; // Stel baudrate in (APB1 = HCLK/4 standaard)
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;  // Schakel transmitter en receiver in
    USART2->CR1 |= USART_CR1_RXNEIE;            // RX interrupt inschakelen
    USART2->CR1 |= USART_CR1_UE;                // Schakel USART2 in
    USART2->CR2 &= ~USART_CR2_STOP; // Zet op 1 stopbit
    USART2->CR1 &= ~USART_CR1_M; // Zet op 8 data bits

    // 4. Configureer NVIC voor USART2
    NVIC_SetPriority(USART2_IRQn, 1); // Zet interrupt prioriteit
    NVIC_EnableIRQ(USART2_IRQn);      // Schakel USART2 interrupt in
}

/////////////////////////////////////////////////////////////////////////
  /// @brief Sends a single character via UART2.
  /// @note  Waits until the TX buffer is empty before sending the character.
  /// @param c The character to be sent.
/////////////////////////////////////////////////////////////////////////
void UART2_SendChar(char c) {
    while (!(USART2->SR & USART_SR_TXE)); // Wacht tot de TX buffer leeg is
    USART2->DR = c;                      // Stuur een karakter
}

/////////////////////////////////////////////////////////////////////////
  /// @brief Sends a null-terminated string via UART2.
  /// @note  This function repeatedly calls UART2_SendChar for each character.
  /// @param str Pointer to the null-terminated string to send.
/////////////////////////////////////////////////////////////////////////
void UART2_SendString(char *str) {
    while (*str) {
        UART2_SendChar(*str++);
    }
}

/////////////////////////////////////////////////////////////////////////
  /// @brief Receives a single character via UART2.
  /// @note  Waits until data is received in the RX buffer.
  /// @return The received character.
/////////////////////////////////////////////////////////////////////////
char UART2_ReceiveChar(void) {
    while (!(USART2->SR & USART_SR_RXNE)); // Wacht tot data ontvangen is
    return (char)USART2->DR;              // Lees het ontvangen karakter
}

/////////////////////////////////////////////////////////////////////////
  /// @brief Receives a string via UART2 until newline or buffer is full.
  /// @note  This function terminates the string with a null character.
  /// @param buffer Pointer to the buffer where the received string will be stored.
  /// @param max_length Maximum length of the string including the null terminator.
/////////////////////////////////////////////////////////////////////////
void UART2_ReceiveString(char *buffer, uint16_t max_length) {
    uint16_t i = 0;
    while (i < max_length - 1) {
        char c = UART2_ReceiveChar();
        if (c == '\n' || c == '\r') break; // Stop bij newline
        buffer[i++] = c;
    }
    buffer[i] = '\0'; // Sluit de string af
}

/////////////////////////////////////////////////////////////////////////
  /// @brief Interrupt handler for USART2.
  /// @note  Called when data is received via UART2. Currently echoes data back.
/////////////////////////////////////////////////////////////////////////
void USART2_IRQHandler(void) {
    if (USART2->SR & USART_SR_RXNE) { // Controleer of data ontvangen is
        char received = USART2->DR;  // Lees de data uit
        // Voeg hier verwerking van ontvangen data toe
        UART2_SendChar(received);    // Echo bijvoorbeeld terug
    }
}
