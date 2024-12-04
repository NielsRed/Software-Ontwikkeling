///**
//  ******************************************************************************
//  * @file      main.c
//  * @author    Niels
//  * @version   V1.0
//  * @date      03/05/2012
//  * @brief     STM32F4xx Devices vector table for RIDE7 toolchain.
//  *            This module performs:
//  *                - Set the initial SP
//  *                - Set the initial PC == Reset_Handler,
//  *                - Set the vector table entries with the exceptions ISR address
//  *                - Configure the clock system and the external SRAM mounted on
//  *                  STM324xG-EVAL board to be used as data memory (optional,
//  *                  to be enabled by user)
//  *                - Branches to main in the C library (which eventually
//  *                  calls main()).
//  *            After Reset the Cortex-M4 processor is in Thread mode,
//  *            priority is Privileged, and the Stack is set to Main.
//  ******************************************************************************
//  */

#include "main.h"
#include "stm32_ub_vga_screen.h"
#include <math.h>

// @brief : Main function of the code.
// @Param : void
int main(void)
{
	SystemInit(); // System speed to 168MHz

	UB_VGA_Screen_Init(); // Init VGA-Screen
	UART2_Init_Interrupt(9600);
	uint16_t xp,yp;

	    for(yp = 0; yp < VGA_DISPLAY_Y; yp++) {
	      for(xp = 0; xp < VGA_DISPLAY_X; xp++) {
	    	  if(xp < 200 && xp > 198)
	    	  {
	    		  UB_VGA_SetPixel(xp, yp, VGA_COL_MAGENTA);
	    	  }
	    	  else
	    	  {
	    	       UB_VGA_SetPixel(xp, yp, VGA_COL_CYAN);
	    	  }
	      }
	    }
  while(1)
  {


  }
}

void delay_us(uint32_t us) {
    uint32_t count = us * (SystemCoreClock / 1000000) / 5; // Adjust division based on compiler optimization
    while (count--) {
        __NOP(); // No operation (assembly instruction)
    }
}

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


void UART2_SendChar(char c) {
    while (!(USART2->SR & USART_SR_TXE)); // Wacht tot de TX buffer leeg is
    USART2->DR = c;                      // Stuur een karakter
}

void UART2_SendString(char *str) {
    while (*str) {
        UART2_SendChar(*str++);
    }
}

char UART2_ReceiveChar() {
    while (!(USART2->SR & USART_SR_RXNE)); // Wacht tot data ontvangen is
    return (char)USART2->DR;              // Lees het ontvangen karakter
}

void UART2_ReceiveString(char *buffer, uint16_t max_length) {
    uint16_t i = 0;
    while (i < max_length - 1) {
        char c = UART2_ReceiveChar();
        if (c == '\n' || c == '\r') break; // Stop bij newline
        buffer[i++] = c;
    }
    buffer[i] = '\0'; // Sluit de string af
}

void USART2_IRQHandler(void) {
    if (USART2->SR & USART_SR_RXNE) { // Controleer of data ontvangen is
        char received = USART2->DR;  // Lees de data uit
        // Voeg hier verwerking van ontvangen data toe
        UART2_SendChar(received);    // Echo bijvoorbeeld terug
    }
}

