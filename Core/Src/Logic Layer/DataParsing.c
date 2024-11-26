/*
 * LL_DataParsing.c
 *
 *  Created on: Nov 21, 2024
 *      Author: dupon
 */

#include "../../Inc/Logic Layer/DataParsing.h"
#include "UART.h"

void matchesCommand(const char *command) {
    for (int i = 0; i < sizeof(userCommands) / sizeof(userCommands[0]); ++i) {
        if (strcmp(command, userCommands[i]) == 0) {
            //UITVOER FUNCTION
        	UART2_SendString("KNOWN command");
            return;
        }
    }
    handleUnknownCommand();
}

void handleUnknownCommand() {
	UART2_SendString("Unknown command");
}

