/*
 * API_Draw.h
 *
 *  Created on: Nov 21, 2024
 *      Author: dupon
 */

#ifndef INC_API_DRAW_H_
#define INC_API_DRAW_H_

int API_draw_line (int x_1, int y_1, int x_2,int y_2, int color, int weight, int reserved);

int API_clearscreen (int color);

int API_draw_rectangle (int x, int y, int width, int height, int color, int filled, int reserved, int reserved1);

int API_draw_bitmap (int x_lup, int y_lup, int bm_nr);

int API_draw_text (int x_lup, int y_lup, int color, char *text, char *fontname, int fontsize, int fontstyle, int reserved);

#endif /* INC_API_DRAW_H_ */
