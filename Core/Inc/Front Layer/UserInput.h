/**
  ******************************************************************************
  * @file      UserInput.h
  * @version   v 1.0.1
  * @date      17/12/2024
  * @author    Niels Redegeld
  ******************************************************************************
 */

#ifndef INC_FRONT_LAYER_USERINPUT_H_
#define INC_FRONT_LAYER_USERINPUT_H_

#define MAX_BUFFER_SIZE 1023

#define DATALENGTH_ERR -3
#define UART_ERR -2
#define USER_INPUT_ERR -1


char FL_Parse_String(uint16_t size, char String_Received[]);
#endif /* INC_FRONT_LAYER_USERINPUT_H_ */
