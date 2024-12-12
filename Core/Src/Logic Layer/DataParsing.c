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

ColorMap color_map[] = {
    {"zwart", VGA_COL_BLACK},
    {"blauw", VGA_COL_BLUE},
    {"lichtblauw", VGA_COL_LIGHTBLUE},
    {"groen", VGA_COL_GREEN},
    {"lichtgroen", VGA_COL_LIGHTGREEN},
    {"cyaan", VGA_COL_CYAN},
    {"lichtcyaan", VGA_COL_LIGHTCYAN},
    {"rood", VGA_COL_RED},
    {"lichtrood", VGA_COL_LIGHTRED},
    {"magenta", VGA_COL_MAGENTA},
    {"lichtmagenta", VGA_COL_LIGHTMAGENTA},
    {"bruin", VGA_COL_BROWN},
    {"geel", VGA_COL_YELLOW},
    {"grijs", VGA_COL_GRAY},
    {"wit", VGA_COL_WHITE}
};

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

int checkAttribute(const char *att, char *att_list[], int max_atts) {
    char lowerAtt[20];
    strncpy(lowerAtt, att, sizeof(lowerAtt) - 1);
    lowerAtt[19] = '\0'; // Ensure null-termination

    // Convert to lowercase
    for (int i = 0; lowerAtt[i]; i++) {
    	lowerAtt[i] = tolower(lowerAtt[i]);
    }

    // Compare
    for (int i = 0; i < max_atts; i++) {
        if (strcmp(lowerAtt, att_list[i]) == 0) {
            sprintf(lowerAtt, "found attribute: %s\n", att);
            UART2_SendString(lowerAtt);
            return 1;
        }
    }
    sprintf(lowerAtt, "unknown attribute: %s\n", att);
    UART2_SendString(lowerAtt);
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
        sprintf(buffer, "Parsing failed. Successfully Parsed fields: %d\n", parsed);
        UART2_SendString(buffer);
        return 0;
    }
    return 1;
}

int hasExtraCharacters(const char *input, int offset) {
    const char *remainder = input + offset;
    while (*remainder) {
        if (!isspace(*remainder)) {
            UART2_SendString("Command arguments overload, only using the protocol arguments\n");
            return 1; // Found unexpected characters
        }
        remainder++;
    }
    return 0; // No extra characters
}

// Functie om kleurstring om te zetten naar bitwaarde
int getColorValue(const char *color) {
    for (int i = 0; i < NUM_COLORS; i++) {
        if (strcmp(color_map[i].name, color) == 0) {
            return color_map[i].value;
        }
    }
    // Als kleur niet gevonden wordt, geef standaardkleur terug (zwart)
    UART2_SendString("Unknown Color\n");
    return VGA_COL_BLACK;
}

// Function to parse a comma-separated string into a Lijn struct
void parseLijn(const char *input) {
    int x, y, x_prime, y_prime, thickness, reserved;
    char color[20];
    int trailingChars;

    int parsed = sscanf(input, "lijn,%d,%d,%d,%d,%19[^,],%d,%d%n",&x, &y, &x_prime, &y_prime, color, &thickness, &reserved, &trailingChars);
    if(!errorHandling(parsed, 7)) return;
    trimWhitespace(color); // Remove any trailing whitespace
    hasExtraCharacters(input, trailingChars);

    //API_draw_line(x, y, x_prime, y_prime, getColorValue(color), thickness, 0);
}

void parseRechthoek(const char *input) {
    int x_lup, y_lup, width, height, filled, reserved, reserved2;
    char color[20];
    int trailingChars;

    int parsed = sscanf(input, "rechthoek,%d,%d,%d,%d,%19[^,],%d,%d,%d%n",&x_lup, &y_lup, &width, &height,color, &filled, &reserved, &reserved2,&trailingChars);
    if(!errorHandling(parsed, 8)) return;
    trimWhitespace(color);
    hasExtraCharacters(input, trailingChars);
//    API_draw_rectangle(x_lup, y_lup, width, height, getColorValue(color), filled, 0, 0);
}

void parseTekst(const char *input) {
    int x, y, fontSize;
    char color[20], text[20], fontName[20], fontStyle[20];
    int trailingChars;

    int parsed = sscanf(input, "tekst,%d,%d,%19[^,],%19[^,],%19[^,],%d, %19[^,]%n",&x, &y, color, text,fontName, &fontSize, fontStyle,&trailingChars);

    if(!errorHandling(parsed, 7)) return;
    trimWhitespace(color);
    //checkColor(color);
    checkAttribute(fontName, fontNames, NUM_FONTS);
    checkAttribute(fontStyle, fontStyles, NUM_STYLES);
    hasExtraCharacters(input, trailingChars);
//    API_draw_text (x, y, getColorValue(color), text, fontName, fontSize, 0, 0);
}

void parseBitmap(const char *input) {
    int bitmapIndex, x_lup, y_lup;
    int trailingChars;

    int parsed = sscanf(input, "bitmap,%d,%d,%d%n",&bitmapIndex, &x_lup, &y_lup, &trailingChars);
    if(!errorHandling(parsed, 3)) return;
    hasExtraCharacters(input, trailingChars);
}

void parseClearscherm(const char *input) {
	char color[20];
	int trailingChars;

	int parsed = sscanf(input, "clearscherm,%19[^,]%n",color, &trailingChars);
    if(!errorHandling(parsed, 1)) return;
    trimWhitespace(color);
    hasExtraCharacters(input, trailingChars);
//    API_clearscreen(getColorValue(color));
}
