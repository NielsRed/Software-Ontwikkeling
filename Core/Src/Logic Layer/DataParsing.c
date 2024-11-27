/*
 * LL_DataParsing.c
 *
 *  Created on: Nov 21, 2024
 *      Author: dupon
 */

#include "../../Inc/Logic Layer/DataParsing.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>

void matchesCommand(const char *command) {
    // Find the first comma in the command string
    const char *commaPos = strchr(command, ',');
    int commandLength = commaPos ? (int)(commaPos - command) : strlen(command);

    for (int i = 0; i < sizeof(userCommands) / sizeof(userCommands[0]); ++i) {
        if (strncmp(command, userCommands[i], commandLength) == 0 && strlen(userCommands[i]) == commandLength) {
        	parseCommand(userCommands[i], command);
            return;
        }
    }
    handleUnknownCommand();
}

void handleUnknownCommand() {
	UART2_SendString("Unknown command");
}

void parseCommand(const char *command, const char*fullCommand) {
    // Use string hashing or conditionals for matching strings
    if (strcmp(command, "lijn") == 0) {
    	UART2_SendString("Command: Lijn\n");
    	parseLijn(fullCommand);
    } else if (strcmp(command, "rechthoek") == 0) {
    	UART2_SendString("Command: rechthoek\n");
    } else if (strcmp(command, "tekst") == 0) {
    	UART2_SendString("Command: tekst\n");
    } else if (strcmp(command, "bitmap") == 0) {
    	UART2_SendString("Command: bitmap\n");
    } else if (strcmp(command, "clearscherm") == 0) {
    	UART2_SendString("Command: clearscherm\n");
    } else {
    	handleUnknownCommand();
    }
}

// Function to parse a comma-separated string into a Lijn struct
void parseLijn(const char *input) {
    Lijn lijn = {0};
    if (sscanf(input, "lijn,%d,%d,%d,%d,%19[^,],%d",
               &lijn.x, &lijn.y, &lijn.x_prime, &lijn.y_prime,
               lijn.color, &lijn.thickness) == 6) {
    }
    // Convert values to strings and send over UART
    char buffer[50];

    // Send x
    sprintf(buffer, "x: %s\n", lijn.color);
    UART2_SendString(buffer);
}
