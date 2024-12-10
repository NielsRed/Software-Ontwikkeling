/*
 * API_Draw.c
 *
 *  Created on: Nov 21, 2024
 *      Author: dupon
 */
#include "stm32_ub_vga_screen.h"
#include "API_Draw.h"
#include <string.h>
#include "Character_set.h"
#include <stdbool.h>

#define bitmapsize 16
///////////////////////////////////////////////////////////////////////////////////////////
/// API_draw_line
/// This fucntion is sets pixels on the screen in order to get a line
/// @param x_1 horizontal start value
/// @param x_2 horizontal stop value
/// @param y_1 verical start value
/// @param y_2 verical start value
/// @param color set color for line
/// @param weight
///////////////////////////////////////////////////////////////////////////////////////////
int API_draw_line(int x_1, int y_1, int x_2, int y_2, int color, int weight, int reserved) {
	int test[7];
	test[0] = CheckValueInt(x_1,VGA_X_MIN,VGA_X_MAX);
	test[1] = CheckValueInt(y_1,VGA_Y_MIN,VGA_Y_MAX);
	test[2] = CheckValueInt(x_2,VGA_X_MIN,VGA_X_MAX);
	test[3] = CheckValueInt(y_2,VGA_Y_MIN,VGA_Y_MAX);
	test[4] = CheckValueInt(color,COLOR_VALUE_MIN,COLOR_VALUE_MAX);
	test[5] = CheckValueInt(weight,WEIGHT_VALUE_MIN,WEIGHT_VALUE_MAX);

	char result = tobyte(test);

	if(result)
	{
		return result;
	}

    int delta_x = abs(x_2 - x_1);
    int delta_y = abs(y_2 - y_1);
    int step_x = (x_1 < x_2) ? 1 : -1;
    int step_y = (y_1 < y_2) ? 1 : -1;
    int error_term = delta_x - delta_y;
    int double_error;

    while (1) {
        for (int offset_x = -(weight / 2); offset_x <= (weight / 2); offset_x++) {
            for (int offset_y = -(weight / 2); offset_y <= (weight / 2); offset_y++) {
                int pixel_x = x_1 + offset_x;
                int pixel_y = y_1 + offset_y;

                if (pixel_x >= 0 && pixel_x < VGA_DISPLAY_X && pixel_y >= 0 && pixel_y < VGA_DISPLAY_Y) {
                    UB_VGA_SetPixel(pixel_x, pixel_y, color);
                }
            }
        }

        if (x_1 == x_2 && y_1 == y_2)
            break;

        double_error = 2 * error_term;
        if (double_error > -delta_y) {
            error_term -= delta_y;
            x_1 += step_x;
        }
        if (double_error < delta_x) {
            error_term += delta_x;
            y_1 += step_y;
        }
    }
    return 0;
}



int API_clearscreen (int color)
{
	UB_VGA_FillScreen(color);
	return 0;
}

int API_draw_rectangle(int x, int y, int width, int height, int color, int filled, int reserved, int reserved1) {
    if (width <= 0 || height <= 0)
    {
        return -1;
    }


    if (filled)
    {
        for (int yp = y; yp < y + height; yp++)
        {
            for (int xp = x; xp < x + width; xp++)
            {
                UB_VGA_SetPixel(xp, yp, color);
            }
        }
    }
    else
    {
        for (int xp = x; xp < x + width; xp++)
        {
            UB_VGA_SetPixel(xp, y, color);
            UB_VGA_SetPixel(xp, y + height - 1, color);
        }

        for (int yp = y; yp < y + height; yp++)
        {
            UB_VGA_SetPixel(x, yp, color);
            UB_VGA_SetPixel(x + width - 1, yp, color);
        }
    }

    return 0;
}

int API_draw_bitmap (int x_lup, int y_lup, int bm_nr)
{

    const uint8_t *bitmap = arial_basic[bm_nr];
    for (uint8_t row = 0; row < 8; row++)
    {
        uint8_t line = bitmap[row];
        for (uint8_t col = 0; col < 8; col++)
        {
            if (line & (1 << (7 - col)))
            {
                UB_VGA_SetPixel(x_lup + col, y_lup + row, VGA_COL_GREEN);
            }
        }

    }
    return 0;
}

int API_draw_text (int x_lup, int y_lup, int color, char *text, char *fontname, int fontsize, int fontstyle, int reserved)
{
	int scale = fontsize;
	const uint16_t (*font_array)[bitmapsize];

	if (!strcmp(fontname, "arial"))
	{
	    if (fontstyle == 0)
	    {
	        font_array = arial_basic;
	    }
	    else if (fontstyle == 1)
	    {
	        font_array = arial_cursief;
	    }
	    else if (fontstyle == 2)
	    {
	        font_array = arial_vet;
	    }
	    else
	    {
	        return 0;
	    }
	}
	else if (!strcmp(fontname, "consola"))
	{
	    if (fontstyle == 0)
	    {
	        font_array = consola_basic;
	    }
	    else if (fontstyle == 1)
	    {
	        font_array = consola_cursief;
	    }
	    else if (fontstyle == 2)
	    {
	        font_array = consola_vet;
	    }
	    else
	    {
	        return 0;
	    }
	}
	else if (!strcmp(fontname, "comic sans"))
		{
		    if (fontstyle == 0)
		    {
		        font_array = comic_sans_basic;
		    }
		    else if (fontstyle == 1)
		    {
		        font_array = comic_sans_vet;
		    }
		    else if (fontstyle == 2)
		    {
		        font_array = comic_sans_cursief;
		    }
		    else
		    {
		        return 0;
		    }
		}
	else if (!strcmp(fontname, "wingdings"))
		{
			if (fontstyle == 0)
			{
				font_array = wingdings_basic;
			}
			else
			{
				return 0;
			}
		}
	else
	{
	    return 0;
	}

	if(!strcmp(fontname, "Consola"))
	{
			font_array = consola_basic;
	}

    for(int i = 0; i < strlen(text); i++)
    {
        const uint16_t *bitmap = font_array[(uint8_t)text[i]];
        for (uint16_t row = 0; row < bitmapsize; row++)
        {
        	uint16_t line = bitmap[row];
            for (uint16_t col = 0; col < bitmapsize; col++)
            {
                if (line & (1 << ((bitmapsize-1) - col)))
                {
                    for (int dx = 0; dx < scale; dx++)
                    {
                        for (int dy = 0; dy < scale; dy++)
                        {
                            UB_VGA_SetPixel(x_lup + (i * bitmapsize * scale) + (col * scale) + dx, y_lup + (row * scale) + dy, color);
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int CheckValueInt(int value, int lower, int upper)
{
	if (value < lower)
	{
		return 1;
	}
	else if (value > upper)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

char tobyte(int *array)
{
	char result = 0;
	for (int i = 0; i < 7; i++)
	{
	        result |= (array[i] & 1) << i; // Plaats de bit op de juiste positie
	}
	return result;
}


void testscherm()
{
	API_clearscreen (VGA_COL_MAGENTA);
	API_draw_text (0, 85, VGA_COL_GREEN, "Dit Werkt", "consola", 1, 2,1 );
	API_draw_text (0, 105, VGA_COL_GREEN, "Echt waar", "arial", 1, 2,1 );
	API_draw_text (0, 130, VGA_COL_GREEN, "Maar echt he", "comic sans", 1, 1,1 );
	API_draw_text (0, 5, VGA_COL_GREEN, "abcdefghijklmnopqrstuvwxyz", "wingdings", 1, 0,1 );
	//API_draw_line (0, 160, 320, 160, VGA_COL_GREEN, 3, 0);
	//API_draw_rectangle(20, 20, 100, 150, VGA_COL_GREEN, 0,0, 0);
}

