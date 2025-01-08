#include "main.h"
#include "UART.h"
#include "UserInput.h"
#include <string.h>

/**
  ******************************************************************************
  * @file      UserInput.c
  * @version   v 1.0.6
  * @date      17/12/2024
  * @author    Niels Redegeld
  * @brief     This file parses the string that is received via UART
  *            it also checks if strings are too long for the function.
  *            It ends by sending the command to the next function 'matchesCommand'
  ******************************************************************************
 */

/////////////////////////////////////////////////////////////////////////
  /// @brief Processes a received string and extracts commands.
  /// @note  Ensures string meets specific requirements and processes commands.
  /// @param size The length of the received string.
  /// @param String_Received The received string to be processed.
  /// @return Returns 0 on success, -1 on format error, -3 if the string is too long.
/////////////////////////////////////////////////////////////////////////
char FL_Parse_String(uint16_t size, char String_Received[])
{
    char processed_string[MAX_BUFFER_SIZE]; // Buffer for the current command
    int i = 0, j = 0, spacebarCheck = 0; // Indexes for buffers
    uint8_t stringStop = 0;

    // Check the length of the string
    if (size > MAX_STRING_SIZE)
    {
        UART2_SendString("\n\n");
        UART2_SendString("ERROR! String received is bigger than 1000, cannot complete request.");
        UART2_SendString("\n\n");
        return DATALENGTH_ERR;
    }

    if (size > WARNING_STRING_SIZE)
    {
        UART2_SendString("\n\n");
        UART2_SendString("Warning! String received is larger than 100 characters.");
        UART2_SendString("\n\n");
    }

    UART2_SendString("Processing String: \n");

    // Process the string
    while (i < size && !stringStop)
    {

//    	char log_buffer[64];
//    	sprintf(log_buffer, "Char: %c ASCII: %d\n", String_Received[i], String_Received[i]);
//    	UART2_SendString(log_buffer);

        // Check for the end of the string
        if (String_Received[i] == '\0')
        {
            // Check if the previous character is not '\r'
            if (i == 0 || String_Received[i - 1] != '\n' || String_Received[i - 1] != '\r')
            {
                UART2_SendString("\n\n");
                UART2_SendString("ERROR! String Received does not meet requirements. Missing 'Enter'");
                UART2_SendString("\n\n");
                return USER_INPUT_ERR;
            }
        }

        // Check for '|' or newline, end of a command
        if (String_Received[i] == '\r' || String_Received[i] == '\n')
        {
            processed_string[j] = '\0'; // End the current command
            UART2_SendString("Command found: ");
            UART2_SendString(processed_string);
            UART2_SendString("\n");

            // Here you can further process the command
            matchesCommand(processed_string);
            stringStop = 1; // Stop further processing
        } else
        {
            // Add character to the command
            if (String_Received[i] == ',')
            {
                processed_string[j++] = String_Received[i];
                if (String_Received[i + 1] == ' ')
                {
                    i++; // Skip space after a comma
                    spacebarCheck++;
                }
            } else
            {
                processed_string[j++] = String_Received[i];
            }
        }
        i++;
    }

    // If we reach here without errors
    return 0;
}
