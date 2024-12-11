/*
 * UserInput.c
 *
 *  Created on: Nov 21, 2024
 *      Author: dupon
 */
#include "main.h"
#include "UART.h"
#include "UserInput.h"

#include <string.h>

char FL_Parse_String(uint16_t size, char String_Received[]) {
    char processed_string[1028]; // Buffer voor huidige opdracht
    int i = 0, j = 0, spacebarCheck = 0; // Indexen voor buffers
    uint8_t stringStop = 0;

    // Controleer lengte van de string
    if (size > 1000) {
        UART2_SendString("\n\n");
        UART2_SendString("ERROR! String received is bigger than 1000, cannot complete request.");
        UART2_SendString("\n\n");
        return -3;
    }

    if (size > 100) {
        UART2_SendString("\n\n");
        UART2_SendString("Warning! String received is larger than 100 characters.");
        UART2_SendString("\n\n");
    }

    UART2_SendString("Processing String: \n");

    // Verwerk de string
    while (i < size && !stringStop) {
        // Controleer op het einde van de string
        if (String_Received[i] == '\0') {
            // Controleer of de vorige karakter geen '|' is
            if (i == 0 || String_Received[i - 1] != '|') {
                UART2_SendString("\n\n");
                UART2_SendString("ERROR! String Received does not meet requirements. Missing '|'");
                UART2_SendString("\n\n");
                return -1;
            }
        }

        // Controleer op '|' of newline, einde van een opdracht
        if (String_Received[i] == '|' || String_Received[i] == '\n') {
            processed_string[j] = '\0'; // Beëindig de huidige opdracht
            UART2_SendString("Command found: ");
            UART2_SendString(processed_string);
            UART2_SendString("\n");

            // Hier kun je de opdracht verder verwerken
            // matchesCommand(processed_string);
            stringStop = 1; // Stop met verdere verwerking
        } else {
            // Voeg karakter toe aan de opdracht
            if (String_Received[i] == ',') {
                processed_string[j++] = String_Received[i];
                if (String_Received[i + 1] == ' ') {
                    i++; // Skip spatie na een komma
                    spacebarCheck++;
                }
            } else {
                processed_string[j++] = String_Received[i];
            }
        }
        i++;
    }

    // Als we hier komen zonder errors
    return 0;
}
