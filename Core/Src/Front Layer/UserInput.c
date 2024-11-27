/*
 * UserInput.c
 *
 *  Created on: Nov 21, 2024
 *      Author: dupon
 */
#include "main.h"
#include "UART.h"
#include "UserInput.h"

void FL_Parse_String(char String_Received[1028])
{
    char processed_string[1028]; // Temporary buffer for the current command
    int i = 0, j = 0;            // Indexes for input and output buffers

    UART2_SendString("Processing String: \n");

    while (String_Received[i] != '\0')
    {
        if (String_Received[i] == '|' || String_Received[i] == '\n') {
            // End of command detected, null-terminate and parse the command
            processed_string[j] = '\0';
            UART2_SendString("Command found: ");
            UART2_SendString(processed_string);
            UART2_SendString("\n");

            matchesCommand(processed_string); // Process the command

            // Reset the buffer index for the next command
            j = 0;
        } else {
            // Append character to the current command
            processed_string[j++] = String_Received[i];
        }
        i++;
    }

    // Handle any remaining command if the input does not end with '|' or '\n'
    if (j > 0) {
        processed_string[j] = '\0';
        UART2_SendString("Final Command: ");
        UART2_SendString(processed_string);
        UART2_SendString("\n");

        matchesCommand(processed_string);
    }
}
