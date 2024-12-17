#include "UART.h"
#include "main.h"
#include "UserInput.h"
#include <math.h>
#include <string.h>

/**
  ******************************************************************************
  * @file      Uart.c
  * @version   v 1.0.2
  * @date      17/12/2024
  * @author    Niels Redegeld (Co-Author Chris van Wijk)
  * @brief     This file provides the UART connection with the laptop
  *            via the USB output on the STM32.
  ******************************************************************************
 */

/////////////////////////////////////////////////////////////////////////
  /// @brief Initialize UART2 with interrupts. Sets up baudrate, pins, and NVIC.
  /// @note  This function configures USART2 for communication and enables RX interrupts.
  /// @param baudrate Specifies the baudrate for UART communication.
/////////////////////////////////////////////////////////////////////////
void UART2_Init_Interrupt(uint32_t baudrate)
{
    // 1. Enable the clocks for USART2 and GPIOA
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // Enable USART2 clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Enable GPIOA clock

    // 2. Configure PA2 (TX) and PA3 (RX) as alternate functions
    GPIOA->MODER &= ~((3 << (2 * 2)) | (3 << (3 * 2))); // Reset mode for PA2 and PA3
    GPIOA->MODER |= (2 << (2 * 2)) | (2 << (3 * 2));    // Set alternate function mode
    GPIOA->AFR[0] |= (7 << (4 * 2)) | (7 << (4 * 3));   // Set alternate function 7 (USART2)
    GPIOA->OSPEEDR |= (3 << (2 * 2)) | (3 << (3 * 2));  // Set high speed for PA2/PA3

    // 3. Configure USART2
    USART2->CR1 = 0; // Reset all settings
    USART2->BRR = SystemCoreClock / 4 / baudrate; // Set baudrate (APB1 = HCLK/4 by default)
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;  // Enable transmitter and receiver
    USART2->CR1 |= USART_CR1_RXNEIE;            // Enable RX interrupt
    USART2->CR1 |= USART_CR1_UE;                // Enable USART2
    USART2->CR2 &= ~USART_CR2_STOP; // Set to 1 stop bit
    USART2->CR1 &= ~USART_CR1_M; // Set to 8 data bits

    // 4. Configure NVIC for USART2
    NVIC_SetPriority(USART2_IRQn, 1); // Set interrupt priority
    NVIC_EnableIRQ(USART2_IRQn);      // Enable USART2 interrupt
}

/////////////////////////////////////////////////////////////////////////
  /// @brief Sends a single character via UART2.
  /// @note  Waits until the TX buffer is empty before sending the character.
  /// @param c The character to be sent.
/////////////////////////////////////////////////////////////////////////
void UART2_SendChar(char c)
{
    while (!(USART2->SR & USART_SR_TXE)); // Wait until the TX buffer is empty
    USART2->DR = c;                      // Send a character
}

/////////////////////////////////////////////////////////////////////////
  /// @brief Sends a null-terminated string via UART2.
  /// @note  This function repeatedly calls UART2_SendChar for each character.
  /// @param str Pointer to the null-terminated string to send.
/////////////////////////////////////////////////////////////////////////
void UART2_SendString(char *str)
{
    while (*str) {
        UART2_SendChar(*str++);
    }
}

/////////////////////////////////////////////////////////////////////////
  /// @brief Receives a single character via UART2.
  /// @note  Waits until data is received in the RX buffer.
  /// @return The received character.
/////////////////////////////////////////////////////////////////////////
char UART2_ReceiveChar(void)
{
    while (!(USART2->SR & USART_SR_RXNE)); // Wait until data is received
    return (char)USART2->DR;              // Read the received character
}

/////////////////////////////////////////////////////////////////////////
  /// @brief Receives a string via UART2 until newline or buffer is full.
  /// @note  This function terminates the string with a null character.
  /// @param buffer Pointer to the buffer where the received string will be stored.
  /// @param max_length Maximum length of the string including the null terminator.
/////////////////////////////////////////////////////////////////////////
void UART2_ReceiveString(char *buffer, uint16_t max_length)
{
    uint16_t i = 0;
    while (i < max_length - 1) {
        char c = UART2_ReceiveChar();
        if (c == '\n' || c == '\r') break; // Stop at newline
        buffer[i++] = c;
    }
    buffer[i] = '\0'; // Terminate the string
}

// Static buffer and index for storing the received string
static char uart2_rx_buffer[UART_BUFFER_SIZE];
static uint16_t uart2_rx_index = 0;

void USART2_IRQHandler(void)
{
	char err;
    if (USART2->SR & USART_SR_RXNE)
    { // Check if data is received
        char received = USART2->DR;  // Read the received character
        // Check if we received a termination character
        if (received == '\n' || received == '\r')
        {
            uart2_rx_buffer[uart2_rx_index] = '\0'; // Null-terminate the string

            // Process the received string
            UART2_SendString("Received: ");
            UART2_SendString(uart2_rx_buffer);
            UART2_SendString("\n");

            // Call function to process the strings.
            uint8_t size = strlen(uart2_rx_buffer);
            err = FL_Parse_String(size, uart2_rx_buffer);

            UART2_SendString("\n\n\n");
            UART2_SendChar(err);
            UART2_SendString("\n\n\n");

            // Reset the buffer index for the next string
            uart2_rx_index = 0;
        } else
        {
            // Add the received character to the buffer if there's space
            if (uart2_rx_index < UART_BUFFER_SIZE - 1)
            {
                uart2_rx_buffer[uart2_rx_index++] = received;
            } else {
                // Handle buffer overflow (optional)
                UART2_SendString("Error: Buffer Overflow\n");
                uart2_rx_index = 0; // Reset buffer
            }
        }
    }
}
