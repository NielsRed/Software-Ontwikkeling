/*
 * API_Draw.c
 *
 *  Created on: Nov 21, 2024
 *      Author: dupon
 */
///////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////
#include "stm32_ub_vga_screen.h"
#include "API_Draw.h"
#include <string.h>
#include "Character_set.h"
#include <stdbool.h>

///////////////////////////////////////////////////////////////////////////////////////////
// Consts
///////////////////////////////////////////////////////////////////////////////////////////
const char *fonts[] = {"arial", "consola"};

///////////////////////////////////////////////////////////////////////////////////////////
/// API_draw_line
/// This fucntion sets pixels on the screen in order to get a line
/// @param x_1 horizontal start value
/// @param x_2 horizontal stop value
/// @param y_1 verical start value
/// @param y_2 verical start value
/// @param color set color for line
/// @param weight
///////////////////////////////////////////////////////////////////////////////////////////
int API_draw_line(int x_1, int y_1, int x_2, int y_2, int color, int weight, int reserved)
{

	//Error handling
	uint8_t test[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	test[0] = CheckValueInt(x_1, VGA_X_MIN, VGA_X_MAX);
	test[1] = CheckValueInt(y_1, VGA_Y_MIN, VGA_Y_MAX);
	test[2] = CheckValueInt(x_2, VGA_X_MIN, VGA_X_MAX);
	test[3] = CheckValueInt(y_2, VGA_Y_MIN, VGA_Y_MAX);
	test[4] = CheckValueInt(color, COLOR_VALUE_MIN, COLOR_VALUE_MAX);
	test[5] = CheckValueInt(weight, WEIGHT_VALUE_MIN, WEIGHT_VALUE_MAX);

	//Make error byte and return it
	char result = tobyte(test);
	if (result)
	{
		return result;
	}

	//Start drawline using Bresenham's line algorithm
	int delta_x = abs(x_2 - x_1);
	int delta_y = abs(y_2 - y_1);
	int step_x = (x_1 < x_2) ? 1 : -1;
	int step_y = (y_1 < y_2) ? 1 : -1;
	int error_term = delta_x - delta_y;
	int double_error;

	while (1)
	{
		for (int offset_x = -(weight / 2); offset_x <= (weight / 2); offset_x++)
		{
			for (int offset_y = -(weight / 2); offset_y <= (weight / 2);
					offset_y++)
			{
				int pixel_x = x_1 + offset_x;
				int pixel_y = y_1 + offset_y;

				if (pixel_x
						>= 0&& pixel_x < VGA_DISPLAY_X && pixel_y >= 0 && pixel_y < VGA_DISPLAY_Y)
				{
					UB_VGA_SetPixel(pixel_x, pixel_y, color);
				}
			}
		}

		if (x_1 == x_2 && y_1 == y_2)
		{
			break;
		}
		double_error = 2 * error_term;
		if (double_error > -delta_y)
		{
			error_term -= delta_y;
			x_1 += step_x;
		}
		if (double_error < delta_x)
		{
			error_term += delta_x;
			y_1 += step_y;
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
/// API_clearscreen
/// This fucntion sets all pixels to one color
/// @param color the new color value
///////////////////////////////////////////////////////////////////////////////////////////
int API_clearscreen(int color)
{
	if(CheckValueInt(color, COLOR_VALUE_MIN, COLOR_VALUE_MAX))
	{
		UB_VGA_FillScreen(color);
	}
	else
	{
		return 0b0000001;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
/// API_draw_rectangle
/// This fucntion sets pixels on the screen in order to get a rectangle
/// @param x left upper value
/// @param y left upper value
/// @param width width of rectangle
/// @param height height of rectangle
/// @param color set color for rectangle
/// @param filled set 0 for non filled rectangle and 1 for filled rectangle
///////////////////////////////////////////////////////////////////////////////////////////
int API_draw_rectangle(int x, int y, int width, int height, int color, int filled, int reserved, int reserved1)
{
	uint8_t test[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	test[0] = CheckValueInt(x, VGA_X_MIN, VGA_X_MAX);
	test[1] = CheckValueInt(y, VGA_Y_MIN, VGA_Y_MAX);
	test[2] = CheckValueInt(width, VGA_X_MIN, VGA_X_MAX);
	test[3] = CheckValueInt(height, VGA_Y_MIN, VGA_Y_MAX);
	test[4] = CheckValueInt(color, COLOR_VALUE_MIN, COLOR_VALUE_MAX);
	test[5] = CheckValueInt(filled, ISNOTFILLED, ISFILLED);

	//Make error byte and return it
	char result = tobyte(test);
	if(result)
	{
		return result;
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

///////////////////////////////////////////////////////////////////////////////////////////
/// API_draw_bitmap
/// This fucntion sets pixels on the screen in order to get a rectangle
/// @param x_lup left upper value
/// @param y_lup left upper value
/// @param bm_nr bitmap number
///////////////////////////////////////////////////////////////////////////////////////////
int API_draw_bitmap(int x_lup, int y_lup, int bm_nr)
{
	uint8_t test[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	test[0] = CheckValueInt(x_lup, VGA_X_MIN, VGA_X_MAX);
	test[1] = CheckValueInt(y_lup, VGA_Y_MIN, VGA_Y_MAX);
	test[2] = CheckValueInt(bm_nr, BITMAPZERO, BITMAPAMOUNT);

	//Make error byte and return it
	char result = tobyte(test);
	if(result)
	{
		return result;
	}

	const uint8_t (*bitmap)[bitmapsize] = NULL;
	switch (bm_nr)
	{
	case 0:
		bitmap = bitmaptest;
		break;
	case 1:
		bitmap = bitmap_angry_face;
		break;
	case 2:
		bitmap = bitmap_happy_face;
		break;
	case 3:
		bitmap = bitmap_pijl_boven;
		break;
	case 4:
		bitmap = bitmap_pijl_beneden;
		break;
	case 5:
		bitmap = bitmap_pijl_links;
		break;
	case 6:
		bitmap = bitmap_pijl_rechts;
		break;
	default:
		return 1;
	}
	for (uint8_t row = 0; row < bitmapsize; row++)
	{
		for (uint8_t col = 0; col < bitmapsize; col++)
		{
			uint8_t color = bitmap[row][col];
			UB_VGA_SetPixel(x_lup + (col), y_lup + (row), color);
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
/// API_draw_text
/// This fucntion sets pixels on the screen in order to get a rectangle
/// @param x_lup left upper value
/// @param y_lup left upper value
/// @param color set color of text
/// @param text the text for on the screen
/// @param fontname can be arial, consola or comic sans
/// @param fontsize the size of the text
/// @param fontstyle can be 0 for basic, 1 for cursief and 2 for vet
///////////////////////////////////////////////////////////////////////////////////////////
int API_draw_text(int x_lup, int y_lup, int color, char *text, char *fontname, int fontsize, int fontstyle, int reserved)
{
//	uint8_t test[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
//	test[0] = CheckValueInt(x_lup, VGA_X_MIN, VGA_X_MAX);
//	test[1] = CheckValueInt(y_lup, VGA_Y_MIN, VGA_Y_MAX);
//	test[2] = CheckValueInt(color, COLOR_VALUE_MIN, COLOR_VALUE_MAX);
//	test[4] = CheckValueChar(fontname, fonts);
//	test[5] = CheckValueInt(fontsize, MINFONTSIZE, MAXFONTSIZE);
//	test[6] = CheckValueInt(fontsize, FONTAMOUNTZERO, FONTAMOUNT-1);
//
//	//Make error byte and return it
//	char result = tobyte(test);
//	if(result)
//	{
//		return result;
//	}

	int scale = fontsize;
	const uint16_t (*font_array)[bitmaptextsize];

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

	if (!strcmp(fontname, "Consola"))
	{
		font_array = consola_basic;
	}

	for (int i = 0; i < strlen(text); i++)
	{
		const uint16_t *bitmap = font_array[(uint8_t) text[i]];
		for (uint16_t row = 0; row < bitmaptextsize; row++)
		{
			uint16_t line = bitmap[row];
			for (uint16_t col = 0; col < bitmaptextsize; col++)
			{
				if (line & (1 << ((bitmaptextsize - 1) - col)))
				{
					for (int dx = 0; dx < scale; dx++)
					{
						for (int dy = 0; dy < scale; dy++)
						{
							UB_VGA_SetPixel(x_lup + (i * bitmaptextsize * scale) + (col * scale) + dx, y_lup + (row * scale) + dy, color);
						}
					}
				}
			}
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
/// CheckValueInt
/// This fucntion is used to check a value if it is within reach
/// @param value the value u want to check
/// @param lower the max lower value
/// @param upper the max upper value
///////////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////////
/// CheckValueChar
/// This fucntion is used to check a value if it is within reach
/// @param value the value u want to check
/// @param whatcanbe is a array with al posible options
///////////////////////////////////////////////////////////////////////////////////////////
//int CheckValueChar(const char *value, const char *whatcanbe[])
//{
//    for (int i = 0; i < (sizeof(whatcanbe) / sizeof(whatcanbe[0])); i++)
//    {
//        if (strcmp(value, whatcanbe[i]) == 0)
//        {
//            return 0;
//        }
//    }
//    return 1;
//}

///////////////////////////////////////////////////////////////////////////////////////////
/// tobyte
/// This fucntion is used to convet a array[7] to a byte
/// @param *array that needs to be converted
///////////////////////////////////////////////////////////////////////////////////////////
char tobyte(uint8_t *array)
{
	char result = 0;
	for (int i = 0; i < 7; i++)
	{
		result |= (array[i] & 1) << i; // Plaats de bit op de juiste positie
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////
/// tobyte
/// This fucntion is used to test values
///////////////////////////////////////////////////////////////////////////////////////////
void testscherm()
{
	API_clearscreen(VGA_COL_BLACK);
	API_draw_text(0, 85, VGA_BROWN, "Dit Werkt", "consola", 1, 2, 1);
	API_draw_text(0, 105, VGA_GREY, "Echt waar", "arial", 1, 2, 1);
	API_draw_text(0, 130, VGA_LIGHTGREEN, "Maar echt he", "comic sans", 1, 1,1);
	API_draw_text(0, 5, VGA_LIGHTMAGENTA, "abcdefghijklmnopqrstuvwxyz","wingdings", 1, 0, 1);
	API_draw_bitmap(0, 0, 0);
	API_draw_bitmap(20, 0, 1);
	API_draw_bitmap(40, 0, 2);
	API_draw_bitmap(60, 0, 3);
	API_draw_bitmap(80, 0, 4);
	API_draw_bitmap(100, 0, 5);
	API_draw_bitmap(120, 0, 6);
	//API_draw_line (0, 160, 320, 160, VGA_COL_GREEN, 3, 0);
	//API_draw_rectangle(20, 20, 100, 150, VGA_COL_GREEN, 0,0, 0);
}

void colorpreview()
{
	API_clearscreen(VGA_WHITE);
    char str[4];
    for (char i = 0; i < 255; i++)
    {
        int locH = i * 16;
        int locV = floor(locH / 320);
        locH = locH - (locV * 320);
        int locVV = locV * 16;

        sprintf(str, "%d", i%10);

        API_draw_rectangle(locH, locVV, 16, 16, i, 1, 0, 0);

        API_draw_text(locH, locVV, VGA_WHITE, str, "arial", 1, 0, 0);
    }
}

