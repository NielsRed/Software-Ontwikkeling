/*******************************************************************************
  * @file    API_Draw.c
  * @author  Chris van Wijk
  * @version V1.0.0
  * @date    21-November-2024
  * @brief   Header file for VGA screen drawing API
  ******************************************************************************
  * @attention
  *
  * This file contains function for working with
  * the VGA screen, including drawing lines, rectangles, text, bitmaps, and
  * other utilities.
  *
  ******************************************************************************
  */
///////////////////////////////////////////////////////////////////////////////////////////
  // Includes
///////////////////////////////////////////////////////////////////////////////////////////
#include "stm32_ub_vga_screen.h"
#include "API_Draw.h"
#include <string.h>
#include "Character_set.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////////////////
  // Consts
///////////////////////////////////////////////////////////////////////////////////////////
const char *fonts[] = {"arial", "consolas", "comic sans"};
const char font_amount = sizeof(fonts) / sizeof(fonts[0]);

///////////////////////////////////////////////////////////////////////////////////////////
  /// API_draw_line
  /// This function draws a line on the screen by setting individual pixels.
  /// Uses Bresenham's line algorithm.
  /// @param x_1 Horizontal start coordinate.
  /// @param y_1 Vertical start coordinate.
  /// @param x_2 Horizontal end coordinate.
  /// @param y_2 Vertical end coordinate.
  /// @param color Color of the line.
  /// @param weight Thickness of the line.
  /// @param reserved Reserved for future use.
  /// @return Error code or 0 if successful.
///////////////////////////////////////////////////////////////////////////////////////////
int API_draw_line(int x_1, int y_1, int x_2, int y_2, int color, int weight, int reserved)
{
	//Error handling all params are checked for their max and min value see CheckValueInt
	uint8_t Error[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	Error[0] = CheckValueInt(x_1, VGA_X_MIN, VGA_X_MAX);
	Error[1] = CheckValueInt(y_1, VGA_Y_MIN, VGA_Y_MAX);
	Error[2] = CheckValueInt(x_2, VGA_X_MIN, VGA_X_MAX);
	Error[3] = CheckValueInt(y_2, VGA_Y_MIN, VGA_Y_MAX);
	Error[4] = CheckValueInt(color, COLOR_VALUE_MIN, COLOR_VALUE_MAX);
	Error[5] = CheckValueInt(weight, WEIGHT_VALUE_MIN, WEIGHT_VALUE_MAX);

	//convert Error array to one single error byte return this if any error is present
	char result = tobyte(Error);
	if (result)
	{
		return result;
	}

	//Start drawline using Bresenham's line algorithm
	//see https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/
	int delta_x = abs(x_2 - x_1); //calculate deltas
	int delta_y = abs(y_2 - y_1);
	int step_x = (x_1 < x_2) ? 1 : -1; //determine directions
	int step_y = (y_1 < y_2) ? 1 : -1;
	int error_term = delta_x - delta_y; //error term
	int double_error;

	while (1)
	{
		// Draw the line with thickness by setting surrounding pixels
		for (int offset_x = -(weight / 2); offset_x <= (weight / 2); offset_x++)
		{
			for (int offset_y = -(weight / 2); offset_y <= (weight / 2);
					offset_y++)
			{
				int pixel_x = x_1 + offset_x; //calculate coordinates of x pixel
				int pixel_y = y_1 + offset_y; //calculate coordinates of y pixel

				// Check if the width does not exceed the screen boundary
				if (pixel_x >= 0&& pixel_x < VGA_DISPLAY_X && pixel_y >= 0 && pixel_y < VGA_DISPLAY_Y)
				{
					UB_VGA_SetPixel(pixel_x, pixel_y, color); //set pixel
				}
			}
		}

		//Stop the loop when at the endpoint
		if (x_1 == x_2 && y_1 == y_2)
		{
			break;
		}

		//update error term
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
  /// This function fills the entire screen with a specified color.
  /// @param color The color to fill the screen with.
  /// @return Error code or 0 if successful.
///////////////////////////////////////////////////////////////////////////////////////////
int API_clearscreen(int color)
{
	//Check the one param of this function
	if(!CheckValueInt(color, COLOR_VALUE_MIN, COLOR_VALUE_MAX))
	{
		//fill with the give color
		UB_VGA_FillScreen(color);
	}
	else
	{
		//return the error term when error is present
		return 0b0000001;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
  /// API_draw_rectangle
  /// This function draws a rectangle on the screen, filled or outlined.
  /// @param x X-coordinate of the upper-left corner.
  /// @param y Y-coordinate of the upper-left corner.
  /// @param width Width of the rectangle.
  /// @param height Height of the rectangle.
  /// @param color Color of the rectangle.
  /// @param filled 1 for filled rectangle, 0 for outline only.
  /// @param reserved Reserved for future use.
  /// @param reserved1 Reserved for future use.
  /// @return Error code or 0 if successful.
///////////////////////////////////////////////////////////////////////////////////////////
int API_draw_rectangle(int x, int y, int width, int height, int color, int filled, int reserved, int reserved1)
{
	//Error handling all params are checked for their max and min value see CheckValueInt
	uint8_t test[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	test[0] = CheckValueInt(x, VGA_X_MIN, VGA_X_MAX);
	test[1] = CheckValueInt(y, VGA_Y_MIN, VGA_Y_MAX);
	test[2] = CheckValueInt(width, VGA_X_MIN, VGA_X_MAX);
	test[3] = CheckValueInt(height, VGA_Y_MIN, VGA_Y_MAX);
	test[4] = CheckValueInt(color, COLOR_VALUE_MIN, COLOR_VALUE_MAX);
	test[5] = CheckValueInt(filled, ISNOTFILLED, ISFILLED);

	//convert Error array to one single error byte return this if any error is present
	char result = tobyte(test);
	if(result)
	{
		return result;
	}

	//draw filled cube
	if (filled)
	{
		for (int yp = y; yp < y + height; yp++) //loop through rows
		{
			for (int xp = x; xp < x + width; xp++) //loop through coloms
			{
				UB_VGA_SetPixel(xp, yp, color); //set pixel
			}
		}
	}
	//draw rectangle outline
	else
	{
		//draw top bottom edge
		for (int xp = x; xp < x + width; xp++)
		{
			UB_VGA_SetPixel(xp, y, color);
			UB_VGA_SetPixel(xp, y + height - 1, color);
		}

		//draw left and right edge
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
  /// This function draws a predefined bitmap image at a specified location.
  /// @param x_lup X-coordinate of the upper-left corner.
  /// @param y_lup Y-coordinate of the upper-left corner.
  /// @param bm_nr Bitmap index number.
  /// @return Error code or 0 if successful.
///////////////////////////////////////////////////////////////////////////////////////////
int API_draw_bitmap(int x_lup, int y_lup, int bm_nr)
{
	//Error handling all params are checked for their max and min value see CheckValueInt
	uint8_t test[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	test[0] = CheckValueInt(x_lup, VGA_X_MIN, VGA_X_MAX);
	test[1] = CheckValueInt(y_lup, VGA_Y_MIN, VGA_Y_MAX);
	test[2] = CheckValueInt(bm_nr, BITMAPZERO, BITMAPAMOUNT);

	//convert Error array to one single error byte return this if any error is present
	char result = tobyte(test);
	if(result)
	{
		return result;
	}

	//match bitmap number with the right bitmap
	const uint8_t (*bitmap)[BITMAPSIZE] = NULL;
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
		bitmap = bitmap_arrow_up;
		break;
	case 4:
		bitmap = bitmap_arrow_down;
		break;
	case 5:
		bitmap = bitmap_arrow_left;
		break;
	case 6:
		bitmap = bitmap_arrow_right;
		break;
	default:
		return 1;
	}

	//draw the bitmap
	for (uint8_t row = 0; row < BITMAPSIZE; row++) //loop through rows
	{
		for (uint8_t col = 0; col < BITMAPSIZE; col++) //loop through coloms
		{
			uint8_t color = bitmap[row][col]; //get color from bitmap
			UB_VGA_SetPixel(x_lup + (col), y_lup + (row), color); //set pixel
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
  /// API_draw_text
  /// This function draws text on the screen using a specified font and style.
  /// @param x_lup X-coordinate of the upper-left corner.
  /// @param y_lup Y-coordinate of the upper-left corner.
  /// @param color Color of the text.
  /// @param text Pointer to the string to display.
  /// @param fontname Name of the font (e.g., "arial", "consolas", "comic sans").
  /// @param fontsize Size of the font.
  /// @param fontstyle Text style: 0 for basic, 1 for italic, 2 for bold.
  /// @param reserved Reserved for future use.
  /// @return Error code or 0 if successful.
///////////////////////////////////////////////////////////////////////////////////////////
int API_draw_text(int x_lup, int y_lup, int color, char *text, char *fontname, int fontsize, int fontstyle, int reserved)
{
	//Error handling all params are checked for their max and min value see CheckValueInt
    uint8_t test[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    test[0] = CheckValueInt(x_lup, VGA_X_MIN, VGA_X_MAX);
    test[1] = CheckValueInt(y_lup, VGA_Y_MIN, VGA_Y_MAX);
    test[2] = CheckValueInt(color, COLOR_VALUE_MIN, COLOR_VALUE_MAX);
    test[4] = CheckValueChar(fontname, fonts, font_amount);
    test[5] = CheckValueInt(fontsize, MINFONTSIZE, MAXFONTSIZE);
    test[6] = CheckValueInt(fontsize, FONTAMOUNTZERO, FONTAMOUNT - 1);

    //convert Error array to one single error byte return this if any error is present
    char result = tobyte(test);
    if (result)
    {
        return result;
    }

    //make pointer array for pointing to bitmaps
    int scale = fontsize;
    const uint16_t (*font_array)[BITMAPTEXTSIZE];

    //check for fontname and style first it checks fontname in their it checks for the style
    if (!strcmp(fontname, "arial"))
    {
        if (fontstyle == 0)
        {
            font_array = arial_basic;
        }
        else if (fontstyle == 1)
        {
            font_array = arial_vet;
        }
        else if (fontstyle == 2)
        {
            font_array = arial_cursief;
        }
        else
        {
            return 0;
        }
    }
    else if (!strcmp(fontname, "consolas"))
    {
        if (fontstyle == 0)
        {
            font_array = consolas_basic;
        }
        else if (fontstyle == 1)
        {
            font_array = consolas_vet;
        }
        else if (fontstyle == 2)
        {
            font_array = consolas_cursief;
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
    else
    {
        return 0;
    }



    int x_pos = x_lup; //current x pos
    int y_pos = y_lup; //current y pos
    int char_width = BITMAPTEXTSIZE * scale; // Width of each character in pixels

    //loop through the text in the string
    for (int i = 0; i < strlen(text); i++)
    {
        // Check if we exceed the screen width
        if (x_pos + char_width > VGA_X_MAX)
        {
            x_pos = x_lup;                   // back to the start of the row
            y_pos += BITMAPTEXTSIZE * scale; // new row
        }

        // Retrieve the bitmap for the current character
        const uint16_t *bitmap = font_array[(uint8_t) text[i]];
        for (uint16_t row = 0; row < BITMAPTEXTSIZE; row++) // loop through rows of the character bitmap
        {
            uint16_t line = bitmap[row];
            for (uint16_t col = 0; col < BITMAPTEXTSIZE; col++) // loop through coloms of the character bitmap
            {
                if (line & (1 << ((BITMAPTEXTSIZE - 1) - col))) // Check if the pixel should be set
                {
                	// Scale each pixel according to the font size
                    for (int dx = 0; dx < scale; dx++)
                    {
                        for (int dy = 0; dy < scale; dy++)
                        {
                            UB_VGA_SetPixel(x_pos + (col * scale) + dx, y_pos + (row * scale) + dy, color);
                        }
                    }
                }
            }
        }
        x_pos += char_width; // Verplaats de x-positie naar het volgende karakter
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
  /// CheckValueInt
  /// This function verifies if a value falls within a specified range.
  /// @param value The value to check.
  /// @param lower Minimum valid value.
  /// @param upper Maximum valid value.
  /// @return 0 if valid, 1 if out of range.
///////////////////////////////////////////////////////////////////////////////////////////
int CheckValueInt(int value, int lower, int upper)
{
	//Check if value is lower then the given lower value
	if (value < lower)
	{
		return 1;
	}
	//check if vlaue is higher then the give upper value
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
  /// This function checks if a string matches one of the valid options.
  /// @param value The string to check.
  /// @param whatcanbe Array of valid options.
  /// @param size Size of the array of valid options.
  /// @return 0 if valid, 1 if invalid.
///////////////////////////////////////////////////////////////////////////////////////////
int CheckValueChar(const char *value, const char *whatcanbe[], int size)
{
	//Check if the value matches with an option of the array
	//loop through all the options
    for (int i = 0; i < size; i++)
    {
    	//compare strings
        if (strcmp(value, whatcanbe[i]) == 0)
        {
            return 0;
        }
    }
    return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////
  /// tobyte
  /// This function converts an array of binary values to a byte.
  /// @param array Pointer to the array to convert.
  /// @return The resulting byte.
///////////////////////////////////////////////////////////////////////////////////////////
char tobyte(uint8_t *array)
{
	char result = 0;
	//loop through the 8 bits of the byte
	for (int i = 0; i < 7; i++)
	{
		result |= (array[i] & 1) << i; // Plaats de bit op de juiste positie
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////
  /// testscherm
  /// This function tests various drawing functions by rendering elements on the screen.
  /// Demonstrates line, text, and bitmap drawing.
///////////////////////////////////////////////////////////////////////////////////////////
void testscherm()
{
	API_clearscreen(VGA_WHITE);
	API_draw_text(0, 18, VGA_BROWN, "Dit werkt -> Echt waar -> maar echt he! ,abcdefghijklmnopqrstuvwxyz1234567890", "arial", 1, 2, 1);
	API_draw_bitmap(0, 0, 0);
	API_draw_bitmap(20, 0, 1);
	API_draw_bitmap(40, 0, 2);
	API_draw_bitmap(60, 0, 3);
	API_draw_bitmap(80, 0, 4);
	API_draw_bitmap(100, 0, 5);
	API_draw_bitmap(120, 0, 6);
}

///////////////////////////////////////////////////////////////////////////////////////////
  /// colorpreview
  /// This function displays all colors on the screen for testing.
  /// Each color is shown with its numerical value in text.
///////////////////////////////////////////////////////////////////////////////////////////
void colorpreview()
{
	API_clearscreen(VGA_WHITE);
    char str[4];
    //loop through all color and set these in a 16x16 on the screen with number in it
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

