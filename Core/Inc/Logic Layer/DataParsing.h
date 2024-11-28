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
    int x_prime;
    int width;
    int height;
    char color[20];
    char filled;
} Rechthoek;

typedef struct {
    int x;
    int y;
    int color;
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
} Bitmap;

char *userCommands[] = {"lijn", "rechthoek", "tekst", "bitmap", "clearscherm"};

void matchesCommand(const char *command);
void handleUnknownCommand();
void parseCommand(const char *command, const char*fullCommand);
void parseLijn(const char *input);


#endif /* INC_DATAPARSING_H_ */
