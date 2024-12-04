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
            return 1;
        }
    }
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
    } else if (strcmp(command, "clearscherm") == 0) {
    	UART2_SendString("Command: clearscherm\n");
    	parseClearscherm(fullCommand);
    } else {
    	handleUnknownCommand();
    }
}

// Function to parse a comma-separated string into a Lijn struct
void parseLijn(const char *input) {
    Lijn lijn = {0};

    // Convert values to strings and send over UART
    char buffer[50];

    int parsed = sscanf(input, "lijn,%d,%d,%d,%d,%19[^,],%d",
            &lijn.x, &lijn.y, &lijn.x_prime, &lijn.y_prime,
            lijn.color, &lijn.thickness);

    if (parsed == 6) {
    }else{// if parsing failed
        sprintf(buffer, "Parsing failed. Parsed fields: %d\n", parsed);
        UART2_SendString(buffer);
        return;
    }

    trimWhitespace(lijn.color); // Remove any trailing whitespace

    if(checkColor(lijn.color)){
        sprintf(buffer, "found color: %s\n", lijn.color);
        UART2_SendString(buffer);
    }else{
    	UART2_SendString("unknown color detected");
    }
//    API_draw_line(lijn.x, lijn.y, lijn.x_prime, lijn.y_prime, VGA_COL_BLUE, lijn.thickness, 0);
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
//    API_draw_rectangle(rect.x_lup, rect.y_lup, rect.width, rect.height, VGA_COL_BLUE, rect.filled, 0, 0);
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
//    API_draw_text (txt.x, txt.y, VGA_COL_BLUE, txt.text, txt.fontName, txt.fontSize, 0, 0);
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
//    API_clearscreen(VGA_COL_MAGENTA);
}
