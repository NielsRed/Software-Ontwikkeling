/**
  ******************************************************************************
  * @file    DataParsing.c
  * @author  Roy Dupon
  * @version V1.0.12
  * @date    17-December-2024
  * @brief   This file provides firmware functions to manage the following
  *          functionalities to parse the commands sent by the user via UART:
  *           - User input parsing
  *           - Command extraction
  *           - Command recognition
  *           - Error handling
  *           - Sending parsed data to the API layer
  ******************************************************************************
  */

#include "DataParsing.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>
#include "API_Draw.h"
#include "stm32_ub_vga_screen.h"

// Assign color strings to correct color values
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


///////////////////////////////////////////////////////////////////////////////////////
  /// @brief  Checks if the first user argument of the command is known by the system.
  /// @note   Matches the command against a predefined set of valid user commands.
  /// @param  command: The user command string.
  /// @retval None
///////////////////////////////////////////////////////////////////////////////////////
void matchesCommand(const char *command) {
    // Find the first comma in the command string
    const char *commaPos = strchr(command, ',');
    int commandLength = commaPos ? (int)(commaPos - command) : strlen(command);

    // Loop trough known commands list and compare to the user command
    for (int i = 0; i < sizeof(userCommands) / sizeof(userCommands[0]); ++i) {
        if (strncmp(command, userCommands[i], commandLength) == 0 && strlen(userCommands[i]) == commandLength) {
        	parseCommand(userCommands[i], command);
            return;
        }
    }
    handleUnknownCommand();
}

///////////////////////////////////////////////////////////////////////////////////////
  /// @brief  Handles unknown commands by notifying the user.
  /// @note   Sends an "Unknown command" message over UART.
  /// @retval None
///////////////////////////////////////////////////////////////////////////////////////
void handleUnknownCommand() {
	UART2_SendString("Unknown command");
}

///////////////////////////////////////////////////////////////////////////////////////
/// @brief  Trims leading and trailing whitespace from a given string.
/// @note   Modifies the string in place.
/// @param  str: The string to trim.
/// @retval None
///////////////////////////////////////////////////////////////////////////////////////
void trimWhitespace(char *str) {
    char *start = str; // Pointer to the beginning of the string
    char *end;

    // Move the start pointer forward when there are leading whitespace characters
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

///////////////////////////////////////////////////////////////////////////////////////
  /// @brief  Checks if an attribute is in the allowed list.
  /// @note   Compares the attribute with a list of valid attributes and sends feedback.
  /// @param  att: The attribute to check.
  /// @param  att_list: List of valid attributes.
  /// @param  max_atts: Maximum number of valid attributes.
  /// @retval 1 if the attribute is found, 0 otherwise.
///////////////////////////////////////////////////////////////////////////////////////
int checkAttribute(const char *att, char *att_list[], int max_atts) {
    char lowerAtt[MAX_PARSED_STRING_SIZE];
    strncpy(lowerAtt, att, sizeof(lowerAtt) - 1);
    lowerAtt[MAX_PARSED_STRING_SIZE-1] = '\0'; // Ensure null-termination

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

///////////////////////////////////////////////////////////////////////////////////////
  /// @brief  Parses the given command and calls the appropriate handler function.
  /// @note   Uses string comparisons to identify and dispatch the command.
  /// @param  command: The command to parse.
  /// @param  fullCommand: The full user input string.
  /// @retval None
///////////////////////////////////////////////////////////////////////////////////////
void parseCommand(const char *command, const char*fullCommand) {
    // compare user command with system commands
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

///////////////////////////////////////////////////////////////////////////////////////
  /// @brief  Handles parsing errors by sending a message to the user.
  /// @note   Compares parsed and expected argument counts.
  /// @param  parsed: Number of successfully parsed arguments.
  /// @param  argumentCount: Expected number of arguments.
  /// @retval 1 if parsing succeeded, 0 otherwise.
///////////////////////////////////////////////////////////////////////////////////////
int errorHandling(int parsed, int argumentCount){
    char buffer[MAX_STRING_BUFFER_SIZE]; // Convert values to strings and send over UART
    if (parsed == argumentCount) {
    }else{// if parsing failed
    	// return the amount of succesfully parsed fields
        sprintf(buffer, "Parsing failed. Successfully Parsed fields: %d\n", parsed);
        UART2_SendString(buffer);
        return 0;
    }
    return 1;
}

///////////////////////////////////////////////////////////////////////////////////////
  /// @brief  Checks for unexpected extra characters after the parsed input.
  /// @note   Sends a warning message if extra characters are found.
  /// @param  input: The full input string.
  /// @param  offset: Offset to start checking for extra characters.
  /// @retval 1 if extra characters are found, 0 otherwise.
///////////////////////////////////////////////////////////////////////////////////////
int hasExtraCharacters(const char *input, int offset) {
    const char *remainder = input + offset;
    while (*remainder) {
        if (!isspace(*remainder)) { // check if the remainder is not a whitespace or something like \n or \r
            UART2_SendString("Command arguments overload, only using the protocol arguments\n");
            return 1; // Found unexpected characters
        }
        remainder++;
    }
    return 0; // No extra characters
}

///////////////////////////////////////////////////////////////////////////////////////
  /// @brief  Converts a color name to its corresponding VGA color value.
  /// @note   Returns black if the color name is not found.
  /// @param  color: The name of the color.
  /// @retval VGA color value.
///////////////////////////////////////////////////////////////////////////////////////
int getColorValue(const char *color) {
	// find the color associated with the found parsed color
    for (int i = 0; i < NUM_COLORS; i++) {
        if (strcmp(color_map[i].name, color) == 0) {
            return color_map[i].value;
        }
    }
    // if the color does not get found, return default (black)
    UART2_SendString("Unknown Color\n");
    return VGA_COL_BLACK;
}

///////////////////////////////////////////////////////////////////////////////////////
  /// @brief  Parses a command string to draw a line.
  /// @note   Extracts line properties like coordinates, color, and thickness.
  /// @param  input: The full user input string.
  /// @retval None
///////////////////////////////////////////////////////////////////////////////////////
void parseLijn(const char *input) {
    int x, y, x_prime, y_prime, thickness, reserved;
    char color[MAX_PARSED_STRING_SIZE];
    int trailingChars;

    // parse input using scanf for later parsing controls
    int parsed = sscanf(input, "lijn,%d,%d,%d,%d,%19[^,],%d,%d%n",&x, &y, &x_prime, &y_prime, color, &thickness, &reserved, &trailingChars);
    if(!errorHandling(parsed, 7)) return;
    trimWhitespace(color);
    hasExtraCharacters(input, trailingChars);

    //API_draw_line(x, y, x_prime, y_prime, getColorValue(color), thickness, 0);
}

///////////////////////////////////////////////////////////////////////////////////////
  /// @brief  Parses a command string to draw a rectangle.
  /// @note   Extracts rectangle properties like coordinates, dimensions, and color.
  /// @param  input: The full user input string.
  /// @retval None
///////////////////////////////////////////////////////////////////////////////////////
void parseRechthoek(const char *input) {
    int x_lup, y_lup, width, height, filled, reserved, reserved2;
    char color[MAX_PARSED_STRING_SIZE];
    int trailingChars;

    // parse input using scanf for later parsing controls
    int parsed = sscanf(input, "rechthoek,%d,%d,%d,%d,%19[^,],%d,%d,%d%n",&x_lup, &y_lup, &width, &height,color, &filled, &reserved, &reserved2,&trailingChars);
    if(!errorHandling(parsed, 8)) return;
    trimWhitespace(color);
    hasExtraCharacters(input, trailingChars);
//    API_draw_rectangle(x_lup, y_lup, width, height, getColorValue(color), filled, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////////////
  /// @brief  Parses a command string to draw text.
  /// @note   Extracts text properties like coordinates, color, font, and content.
  /// @param  input: The full user input string.
  /// @retval None
///////////////////////////////////////////////////////////////////////////////////////
void parseTekst(const char *input) {
    int x, y, fontSize;
    char color[MAX_PARSED_STRING_SIZE], text[MAX_PARSED_STRING_SIZE], fontName[MAX_PARSED_STRING_SIZE], fontStyle[MAX_PARSED_STRING_SIZE];
    int trailingChars;

    // parse input using scanf for later parsing controls
    int parsed = sscanf(input, "tekst,%d,%d,%19[^,],%19[^,],%19[^,],%d, %19[^,]%n",&x, &y, color, text,fontName, &fontSize, fontStyle,&trailingChars);

    if(!errorHandling(parsed, 7)) return;
    trimWhitespace(color);
    checkAttribute(fontName, fontNames, NUM_FONTS);
    checkAttribute(fontStyle, fontStyles, NUM_STYLES);
    hasExtraCharacters(input, trailingChars);
//    API_draw_text (x, y, getColorValue(color), text, fontName, fontSize, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////////////
  /// @brief  Parses a command string to display a bitmap.
  /// @note   Extracts bitmap index and coordinates for placement.
  /// @param  input: The full user input string.
  /// @retval None
///////////////////////////////////////////////////////////////////////////////////////
void parseBitmap(const char *input) {
    int bitmapIndex, x_lup, y_lup;
    int trailingChars;

    // parse input using scanf for later parsing controls
    int parsed = sscanf(input, "bitmap,%d,%d,%d%n",&bitmapIndex, &x_lup, &y_lup, &trailingChars);
    if(!errorHandling(parsed, 3)) return;
    hasExtraCharacters(input, trailingChars);
}

///////////////////////////////////////////////////////////////////////////////////////
  /// @brief  Parses a command string to clear the screen with a specified color.
  /// @note   Extracts the color value and clears the screen.
  /// @param  input: The full user input string.
  /// @retval None
///////////////////////////////////////////////////////////////////////////////////////
void parseClearscherm(const char *input) {
	char color[MAX_PARSED_STRING_SIZE];
	int trailingChars;

	// parse input using scanf for later parsing controls
	int parsed = sscanf(input, "clearscherm,%19[^,]%n",color, &trailingChars);
    if(!errorHandling(parsed, 1)) return;
    trimWhitespace(color);
    hasExtraCharacters(input, trailingChars);
//    API_clearscreen(getColorValue(color));
}
