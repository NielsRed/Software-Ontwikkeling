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

int API_draw_line (int x_1, int y_1, int x_2,int y_2, int color, int weight, int reserved)
{
	uint16_t xp,yp;
	for(yp=0;yp<VGA_DISPLAY_Y;yp++) {
		for(xp=0;xp<VGA_DISPLAY_X;xp++) {
			if((xp > x_1 && xp < x_2) && (yp > y_1 && yp < y_2))
			{
				UB_VGA_SetPixel(xp,yp,color);
			}
			else
			{
				continue;
			}
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
	const uint8_t (*font_array)[8];

	if (strcmp(fontname, "arial") == 0)
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
	else if (strcmp(fontname, "consola") == 0)
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
	else
	{
	    return 0; // Onbekend fontnaam
	}

	if(strcmp(fontname, "Consola") == 0)
	{
			font_array = consola_basic;
	}

    for(int i = 0; i < strlen(text); i++)
    {
        const uint8_t *bitmap = font_array[(uint8_t)text[i]];
        for (uint8_t row = 0; row < 8; row++)
        {
            uint8_t line = bitmap[row];
            for (uint8_t col = 0; col < 8; col++)
            {
                if (line & (1 << (7 - col)))
                {
                    for (int dx = 0; dx < scale; dx++)
                    {
                        for (int dy = 0; dy < scale; dy++)
                        {
                            UB_VGA_SetPixel(x_lup + (i * 8 * scale) + (col * scale) + dx, y_lup + (row * scale) + dy, color);
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void testscherm()
{
	API_draw_text (0, 85, VGA_COL_GREEN, "DIT WERKT", "arial", 2, 2,1 );
}

