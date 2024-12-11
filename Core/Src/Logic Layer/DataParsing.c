/*
 * LL_DataParsing.c
 *
 *  Created on: Nov 21, 2024
 *      Author: dupon
 */

#include "DataParsing.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>
#include "API_Draw.h"
#include "stm32_ub_vga_screen.h"

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

void trimWhitespace(char *str) {
    char *start = str; // Pointer to the beginning of the string
    char *end;

    // Move the start pointer forward while there are leading whitespace characters
    while (*start == ' ' || *start == '\n' || *start == '\t') {
        start++;
    }

    // If there was leading whitespace, move the trimmed content to the start of the string
    if (start != str) {
        memmove(str, start, strlen(start) + 1); // Include the null terminator
    }

    // Trim whitespace at the end
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\n' || *end == '\t')) {
        *end = '\0';
        end--;
    }
}


int checkColor(const char *color) {
    char lowerColor[20];
    strncpy(lowerColor, color, sizeof(lowerColor) - 1);
    lowerColor[19] = '\0'; // Ensure null-termination

    // Convert to lowercase
    for (int i = 0; lowerColor[i]; i++) {
        lowerColor[i] = tolower(lowerColor[i]);
    }

    // Compare
    for (int i = 0; i < NUM_COLORS; i++) {
        if (strcmp(lowerColor, colors[i]) == 0) {
            sprintf(lowerColor, "found color: %s\n", color);
            UART2_SendString(lowerColor);
            return 1;
        }
    }
    UART2_SendString("unknown color detected");
    return 0;
}

void parseCommand(const char *command, const char*fullCommand) {
    // Use string hashing or conditionals for matching strings
    if (strcmp(command, "lijn") == 0) {
    	UART2_SendString("Command: Lijn\n");
    	parseLijn(fullCommand);
    } else if (strcmp(command, "rechthoek") == 0) {
    	UART2_SendString("Command: rechthoek\n");
    	parseRechthoek(fullCommand);
    } else if (strcmp(command, "tekst") == 0) {
    	UART2_SendString("Command: tekst\n");
    	parseTekst(fullCommand);
    } else if (strcmp(command, "bitmap") == 0) {
    	UART2_SendString("Command: bitmap\n");
    	parseBitmap(fullCommand);
    } else if (strcmp(command, "clearscherm") == 0) {
    	UART2_SendString("Command: clearscherm\n");
    	parseClearscherm(fullCommand);
    } else {
    	handleUnknownCommand();
    }
}

int errorHandling(int parsed, int argumentCount){
    char buffer[50]; // Convert values to strings and send over UART
    if (parsed == argumentCount) {
    }else{// if parsing failed
        sprintf(buffer, "Parsing failed. Parsed fields: %d\n", parsed);
        UART2_SendString(buffer);
        return 0;
    }
    return 1;
}

// Function to parse a comma-separated string into a Lijn struct
void parseLijn(const char *input) {
    int x, y, x_prime, y_prime, thickness, reserved;
    char color[20];

    int parsed = sscanf(input, "lijn,%d,%d,%d,%d,%19[^,],%d,%d",&x, &y, &x_prime, &y_prime, color, &thickness, &reserved);
    if(!errorHandling(parsed, 7)) return;
    trimWhitespace(color); // Remove any trailing whitespace
    checkColor(color);
//    API_draw_line(x, y, x_prime, y_prime, VGA_COL_BLUE, thickness, 0);
}

void parseRechthoek(const char *input) {
    int x_lup, y_lup, width, height, filled, reserved, reserved2;
    char color[20];

    int parsed = sscanf(input, "rechthoek,%d,%d,%d,%d,%19[^,],%d,%d,%d",&x_lup, &y_lup, &width, &height,color, &filled, &reserved, &reserved2);
    if(!errorHandling(parsed, 8)) return;
    trimWhitespace(color);
    checkColor(color);
//    API_draw_rectangle(x_lup, y_lup, width, height, VGA_COL_BLUE, filled, 0, 0);
}

void parseTekst(const char *input) {
    int x, y, fontSize;
    char color[20], text[20], fontName[20], fontStyle[20];

    int parsed = sscanf(input, "tekst,%d,%d,%19[^,],%19[^,],%19[^,],%d, %19[^,]",&x, &y, color, text,fontName, &fontSize, fontStyle);

    if(!errorHandling(parsed, 7)) return;
    trimWhitespace(color);
    checkColor(color);
//    API_draw_text (x, y, VGA_COL_BLUE, text, fontName, fontSize, 0, 0);
}

void parseBitmap(const char *input) {
    int bitmapIndex, x_lup, y_lup;

    int parsed = sscanf(input, "bitmap,%d,%d,%d",&bitmapIndex, &x_lup, &y_lup);
    if(!errorHandling(parsed, 3)) return;
}

void parseClearscherm(const char *input) {
	char color[20];

	int parsed = sscanf(input, "clearscherm,%19[^,]",color);
    if(!errorHandling(parsed, 1)) return;
    trimWhitespace(color);
    checkColor(color);
//    API_clearscreen(VGA_COL_MAGENTA);
}
