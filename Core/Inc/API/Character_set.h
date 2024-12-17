/*******************************************************************************
  * @file    Character_set.h
  * @author  Chris van Wijk
  * @version V1.0.0
  * @date    27-November-2024
  * @brief   Header file for character sets and bitmaps
  ******************************************************************************
  * @attention
  *
  * This file contains the definitions of character sets and bitmap arrays
  * used for rendering text and images on the VGA screen. The character sets
  * support multiple fonts and styles, while the bitmaps include predefined
  * shapes and icons for graphical display.
  *
  ******************************************************************************
  */

#ifndef INC_API_CHARACTER_SET_H_
#define INC_API_CHARACTER_SET_H_

///////////////////////////////////////////////////////////////////////////////////////////
/// Character Sets
///
/// The character sets define 8x16 pixel bitmaps for ASCII characters (0–127)
/// in different font styles. Each font style array contains 128 characters,
/// represented as 16 rows of 16 bits each. These rows define the pixel data
/// for rendering characters on the VGA screen.
///
/// Supported Fonts and Styles:
/// - Arial: Basic, Bold (Vet), Italic (Cursief)
/// - Consola: Basic, Bold (Vet), Italic (Cursief)
/// - Comic Sans: Basic, Bold (Vet), Italic (Cursief)
///////////////////////////////////////////////////////////////////////////////////////////
extern const uint16_t arial_basic[128][16];      ///< Arial basic style
extern const uint16_t arial_vet[128][16];        ///< Arial bold style
extern const uint16_t arial_cursief[128][16];    ///< Arial italic style
extern const uint16_t consola_basic[128][16];    ///< Consola basic style
extern const uint16_t consola_vet[128][16];      ///< Consola bold style
extern const uint16_t consola_cursief[128][16];  ///< Consola italic style
extern const uint16_t comic_sans_basic[128][16]; ///< Comic Sans basic style
extern const uint16_t comic_sans_vet[128][16];   ///< Comic Sans bold style
extern const uint16_t comic_sans_cursief[128][16];///< Comic Sans italic style

///////////////////////////////////////////////////////////////////////////////////////////
/// Bitmap Definitions
///
/// The bitmap arrays define 16x16 pixel graphical icons. These include both
/// static images (e.g., happy/angry faces) and directional arrows. Each array
/// contains 16 rows, where each row represents a 16-bit row of pixel data.
///
/// Available Bitmaps:
/// - Test bitmap
/// - Happy face
/// - Angry face
/// - Directional arrows (up, down, left, right)
///////////////////////////////////////////////////////////////////////////////////////////
extern const uint8_t bitmaptest[16][16];           ///< Test bitmap
extern const uint8_t bitmap_happy_face[16][16];    ///< Happy face bitmap
extern const uint8_t bitmap_angry_face[16][16];    ///< Angry face bitmap
extern const uint8_t bitmap_pijl_boven[16][16];    ///< Up arrow bitmap
extern const uint8_t bitmap_pijl_beneden[16][16];  ///< Down arrow bitmap
extern const uint8_t bitmap_pijl_links[16][16];    ///< Left arrow bitmap
extern const uint8_t bitmap_pijl_rechts[16][16];   ///< Right arrow bitmap

#endif /* INC_API_CHARACTER_SET_H_ */
