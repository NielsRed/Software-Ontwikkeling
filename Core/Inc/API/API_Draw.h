/*******************************************************************************
  * @file    API_Draw.h
  * @author  Chris van Wijk
  * @version V1.0.0
  * @date    21-November-2024
  * @brief   Header file for VGA screen drawing API
  ******************************************************************************
  * @attention
  *
  * This file contains function prototypes and definitions for working with
  * the VGA screen, including drawing lines, rectangles, text, bitmaps, and
  * other utilities.
  *
  ******************************************************************************
  */

#ifndef INC_API_DRAW_H_
#define INC_API_DRAW_H_

///////////////////////////////////////////////////////////////////////////////////////////
/// Colors definitions
/// VGA color constants for use with the VGA screen API.
///////////////////////////////////////////////////////////////////////////////////////////
#define VGA_BLACK        0
#define VGA_BLUE         3
#define VGA_LIGHTBLUE    27
#define VGA_GREEN        12
#define VGA_LIGHTGREEN   25
#define VGA_CYAN         18
#define VGA_LIGHTCYAN    223
#define VGA_RED          203
#define VGA_LIGHTRED     225
#define VGA_MAGENTA      227
#define VGA_LIGHTMAGENTA 243
#define VGA_BROWN        177
#define VGA_YELLOW       252
#define VGA_GREY         146
#define VGA_WHITE        255
#define VGA_ORANGE       240

///////////////////////////////////////////////////////////////////////////////////////////
/// Screen dimensions and value constraints
/// Constants defining screen dimensions and value ranges.
///////////////////////////////////////////////////////////////////////////////////////////
#define VGA_X_MAX        320
#define VGA_X_MIN        0
#define VGA_Y_MAX        240
#define VGA_Y_MIN        0
#define COLOR_VALUE_MAX  255
#define COLOR_VALUE_MIN  0
#define WEIGHT_VALUE_MAX 25
#define WEIGHT_VALUE_MIN 0
#define BITMAPSIZE       16
#define BITMAPTEXTSIZE   16
#define ISFILLED         1
#define ISNOTFILLED      0
#define BITMAPAMOUNT     6
#define BITMAPZERO       0
#define MINFONTSIZE      0
#define MAXFONTSIZE      25
#define FONTAMOUNT       3
#define FONTAMOUNTZERO   0

///////////////////////////////////////////////////////////////////////////////////////////
/// Function Prototypes
/// API functions for drawing on the VGA screen.
///////////////////////////////////////////////////////////////////////////////////////////

int API_draw_line (int x_1, int y_1, int x_2,int y_2, int color, int weight, int reserved);

int API_clearscreen (int color);

int API_draw_rectangle (int x, int y, int width, int height, int color, int filled, int reserved, int reserved1);

int API_draw_bitmap (int x_lup, int y_lup, int bm_nr);

int API_draw_text (int x_lup, int y_lup, int color, char *text, char *fontname, int fontsize, int fontstyle, int reserved);

char tobyte(uint8_t *array);

int CheckValueInt(int value, int lower, int upper);

int CheckValueChar(const char *value, const char *whatcanbe[], int size);

void testscherm();

void colorpreview();

#endif /* INC_API_DRAW_H_ */
