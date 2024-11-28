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
    char color[20];
    int thickness;
} Lijn;

typedef struct {
    int x_lup;
    int y_lup;
    int width;
    int height;
    char color[20];
    char filled;
} Rechthoek;

typedef struct {
    int x;
    int y;
    char color[20];
    char text[20];
    char fontName[20];
    int fontSize;
    char fontStyle[20];
} Tekst;

typedef struct {
    int bitmapIndex;
    int x_lup;
    int y_lup;
} Bitmap;

typedef struct {
    int color[20];
} Clearscherm;

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


#endif /* INC_DATAPARSING_H_ */
