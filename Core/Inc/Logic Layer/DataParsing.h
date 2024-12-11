/*
 * DataParsing.h
 *
 *  Created on: Nov 21, 2024
 *      Author: dupon
 */

#ifndef INC_DATAPARSING_H_
#define INC_DATAPARSING_H_

#define ALL_COMMANDS 5
#define NUM_COLORS 3
#define MAX_DATA_SIZE 50

char *userCommands[] = {"lijn", "rechthoek", "tekst", "bitmap", "clearscherm"};
char *colors[] = {"rood", "groen", "blauw"};
char *fontNames[] = {"arial", "consolas"};
char *fontStyles[] = {"normaal", "vet", "cursief"};

void matchesCommand(const char *command);
void handleUnknownCommand();
void parseCommand(const char *command, const char*fullCommand);
void parseLijn(const char *input);
void parseRechthoek(const char *input);
void parseTekst(const char *input);
void parseBitmap(const char *input);
void parseClearscherm(const char *input);
int checkColor(const char *color);
void trimWhitespace(char *str);
int errorHandling(int parsed, int argumentCount);


#endif /* INC_DATAPARSING_H_ */
