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

    sprintf(buffer, "x: %s\n", lijn.color);
    UART2_SendString(buffer);
}

void parseRechthoek(const char *input) {
    Rechthoek rect = {0};
    if (sscanf(input, "rechthoek,%d,%d,%d,%d,%19[^,],%d",
               &rect.x_lup, &rect.y_lup, &rect.width, &rect.height,
			   rect.color, &rect.filled) == 6) {
    }
    // Convert values to strings and send over UART
    char buffer[50];

    sprintf(buffer, "width: %s\n", rect.width);
    UART2_SendString(buffer);
}

void parseTekst(const char *input) {
    Tekst txt = {0};
    if (sscanf(input, "tekst,%d,%d,%19[^,],%19[^,],%19[^,],%d, %19[^,]",
               &txt.x, &txt.y, txt.color, txt.text,
			   txt.fontName, &txt.fontSize, txt.fontStyle) == 7) {
    }
    // Convert values to strings and send over UART
    char buffer[50];

    sprintf(buffer, "txt: %s\n", txt.text);
    UART2_SendString(buffer);
}

void parseBitmap(const char *input) {
    Bitmap bitmap = {0};
    if (sscanf(input, "bitmap,%d,%d,%d",
               &bitmap.bitmapIndex, &bitmap.x_lup, &bitmap.y_lup) == 3) {
    }
    // Convert values to strings and send over UART
    char buffer[50];

    sprintf(buffer, "index: %s\n", bitmap.bitmapIndex);
    UART2_SendString(buffer);
}

void parseClearscherm(const char *input) {
	Clearscherm clear = {0};
    if (sscanf(input, "clearscherm,%19[^,]",
    		clear.color) == 1) {
    }
    // Convert values to strings and send over UART
    char buffer[50];

    sprintf(buffer, "color: %s\n", clear.color);
    UART2_SendString(buffer);
}
