/*
 * API_Draw.h
 *
 *  Created on: Nov 21, 2024
 *      Author: dupon
 */

#ifndef INC_API_DRAW_H_
#define INC_API_DRAW_H_

//colors
#define VGA_BLACK        0x00
#define VGA_BLUE         0x03
#define VGA_LIGHTBLUE    0xBB
#define VGA_GREEN        0x1C
#define VGA_LIGHTGREEN   0x9E
#define VGA_CYAN         0x1F
#define VGA_LIGHTCYAN    0xDF
#define VGA_RED          0xE0
#define VGA_LIGHTRED     0xF1
#define VGA_MAGENTA      0xE3
#define VGA_LIGHTMAGENTA 0xEF
#define VGA_BROWN        0xA8
#define VGA_YELLOW       0xFC
#define VGA_GREY         0x6E
#define VGA_WHITE        0xFF


#define VGA_X_MAX 320
#define VGA_X_MIN 0
#define VGA_Y_MAX 240
#define VGA_Y_MIN 0
#define COLOR_VALUE_MAX 255
#define COLOR_VALUE_MIN 0
#define WEIGHT_VALUE_MAX 25
#define WEIGHT_VALUE_MIN 0
#define bitmapsize 16
#define bitmaptextsize 16

int API_draw_line (int x_1, int y_1, int x_2,int y_2, int color, int weight, int reserved);

int API_clearscreen (int color);

int API_draw_rectangle (int x, int y, int width, int height, int color, int filled, int reserved, int reserved1);

int API_draw_bitmap (int x_lup, int y_lup, int bm_nr);

int API_draw_text (int x_lup, int y_lup, int color, char *text, char *fontname, int fontsize, int fontstyle, int reserved);

char tobyte(int *array);

int CheckValueInt(int value, int lower, int upper);
#endif /* INC_API_DRAW_H_ */
