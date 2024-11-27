/*
 * UART.h
 *
 *  Created on: Nov 21, 2024
 *      Author: niels
 */
#include <stdint.h>
#ifndef INC_UART_H_
#define INC_UART_H_

#define UART_BUFFER_SIZE 128  // Define buffer size

void UART2_Init_Interrupt(uint32_t baudrate);
void UART2_SendChar(char c);
void UART2_SendString(char *str);
char UART2_ReceiveChar(void);
void UART2_ReceiveString(char *buffer, uint16_t max_length);
void USART2_IRQHandler(void);


#endif /* INC_UART_H_ */
