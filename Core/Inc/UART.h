/**
  ******************************************************************************
  * @file      UART.c
  * @version   v 1.0.1
  * @date      17/12/2024
  * @author    Niels Redegeld
  ******************************************************************************
 */
#include <stdint.h>
#ifndef INC_UART_H_
#define INC_UART_H_

void UART2_Init_Interrupt(uint32_t baudrate);
void UART2_SendChar(char c);
void UART2_SendString(char *str);
char UART2_ReceiveChar(void);
void UART2_ReceiveString(char *buffer, uint16_t max_length);
void USART2_IRQHandler(void);


#endif /* INC_UART_H_ */
