/**
  ******************************************************************************
  * @file    DataParsing.c
  * @author  Roy Dupon
  * @version V1.0.12
  * @date    17-December-2024
  ******************************************************************************
  */

#ifndef INC_DATAPARSING_H_
#define INC_DATAPARSING_H_

#define ALL_COMMANDS 5
#define NUM_COLORS2 15
#define NUM_COLORS (sizeof(color_map) / sizeof(color_map[0]))
#define NUM_FONTS 2
#define NUM_STYLES 3

#define MAX_PARSED_STRING_SIZE 100
#define MAX_STRING_BUFFER_SIZE 100

char *userCommands[] = {"lijn", "rechthoek", "tekst", "bitmap", "clearscherm"};
char *fontNames[] = {"arial", "consolas"};
char *fontStyles[] = {"normaal", "vet", "cursief"};

typedef struct {
    const char *name;
    int value;
} ColorMap;

void matchesCommand(const char *command);
void handleUnknownCommand();
void parseCommand(const char *command, const char*fullCommand);
void parseLijn(const char *input);
void parseRechthoek(const char *input);
void parseTekst(const char *input);
void parseBitmap(const char *input);
void parseClearscherm(const char *input);
void trimWhitespace(char *str);
int errorHandling(int parsed, int argumentCount);
int hasExtraCharacters(const char *input, int offset);
int checkAttribute(const char *att, char *att_list[], int max_atts);
int getColorValue(const char *color);
void checkForErrorCode(int error);


#endif /* INC_DATAPARSING_H_ */
