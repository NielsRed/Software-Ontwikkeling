/*
 * DataParsing.h
 *
 *  Created on: Nov 21, 2024
 *      Author: dupon
 */

#ifndef INC_DATAPARSING_H_
#define INC_DATAPARSING_H_

#define ALL_COMMANDS 5
#define MAX_DATA_SIZE 50

typedef struct {
    int x;
    int y;
    int x_prime;
    int y_prime;
    char color[20]; // Adjust size as needed for color names
    int thickness;
} Lijn;

char *userCommands[] = {"lijn", "rechthoek", "tekst", "bitmap", "clearscherm"};

void matchesCommand(const char *command);
void handleUnknownCommand();
void parseCommand(const char *command, const char*fullCommand);
void parseLijn(const char *input);


#endif /* INC_DATAPARSING_H_ */
