#include "lcd_driver.h"
#include "fonts.h"
#include "functions.h"
#include "main.h"
#include "settings.h"

#if HRDW_HAS_JPEG
#include "jpeg_utils.h"
#endif

static bool _cp437 = false;
static uint16_t text_cursor_y = 0;
static uint16_t text_cursor_x = 0;
static bool wrap = false;

uint16_t LCDDriver_GetCurrentXOffset(void) { return text_cursor_x; }

void LCDDriver_SetCurrentXOffset(uint16_t x) { text_cursor_x = x; }

// Text printing functions
void LCDDriver_drawCharInMemory(uint16_t x, uint16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size, uint16_t *buffer, uint16_t buffer_w, uint16_t buffer_h) {
	uint8_t line = 0;
	uint8_t zoom = (size > 0 ? size : 1);
	uint8_t size_w = (size > 0 ? RASTR_FONT_W : RASTR_FONT_4x6_W);
	uint8_t size_h = (size > 0 ? RASTR_FONT_H : RASTR_FONT_4x6_H);

	if ((x >= buffer_w) ||           // Clip right
	    (y >= buffer_h) ||           // Clip bottom
	    ((x + size_w * zoom) < 0) || // Clip left
	    ((y + size_h * zoom) < 0)) { // Clip top
		return;
	}

	if ((x + size_w * zoom) >= buffer_w || (y + size_h * zoom) >= buffer_h) {
		return;
	}

	if (c < 32) { // non-printable
		return;
	}
	if (!_cp437 && (c >= 176)) {
		c++; // Handle 'classic' charset behavior
	}

	if (size > 0) {
		for (int8_t j = 0; j < size_h; j++) // y line out
		{
			for (int8_t s_y = 0; s_y < zoom; s_y++) // y size scale
			{
				uint16_t *point = buffer + buffer_w * (y + j * (s_y + 1)) + x;
				for (int8_t i = 0; i < size_w; i++) {
					// x line out
					if (i == 5) {
						line = 0x0;
					} else {
						if (size > 0) {
							line = rastr_font[(c * 5) + i]; // read font
						} else {
							line = rastr_font_4x6[(c * 5) + i]; // read font
						}
					}
					line >>= j;
					for (int8_t s_x = 0; s_x < zoom; s_x++) // x size scale
					{
						if (line & 0x1) {
							*point = color; // font pixel
						} else {
							*point = bg; // background pixel
						}
						point++;
					}
				}
			}
		}
	} else // size 0 4x6
	{
		for (int8_t j = 0; j < size_h; j++) // y line out
		{
			line = rastr_font_4x6[((c - 0x20) * 6) + j]; // read font
			uint16_t *point = buffer + buffer_w * (y + j) + x;
			for (int8_t i = 0; i < size_w; i++) {
				if (line & 0x8) {
					*point = color; // font pixel
				} else {
					*point = bg; // background pixel
				}
				point++;

				line <<= 1;
			}
		}
	}
}

void LCDDriver_printTextInMemory(char text[], int16_t x, int16_t y, uint16_t color, uint16_t bg, uint8_t size, uint16_t *buffer, uint16_t buffer_w, uint16_t buffer_h) {
	uint16_t i = 0;
	uint16_t offset = size * 6;
	uint16_t skipped = 0;
	for (i = 0; i < 128 && text[i] != 0; i++) {
		if (text[i] == '^' && text[i + 1] == 'o') // celsius
		{
			i++;
			skipped++;
			LCDDriver_drawCharInMemory(x + (offset * (i - skipped)), y - 3, text[i], color, bg, size, buffer, buffer_w, buffer_h);
		} else {
			LCDDriver_drawCharInMemory(x + (offset * (i - skipped)), y, text[i], color, bg, size, buffer, buffer_w, buffer_h);
		}
		text_cursor_x = x + (offset * (i + 1 - skipped));
	}
}

void LCDDriver_drawChar(uint16_t x, uint16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) {
	uint8_t line = 0;
	uint8_t zoom = (size > 0 ? size : 1);
	uint8_t size_w = (size > 0 ? RASTR_FONT_W : RASTR_FONT_4x6_W);
	uint8_t size_h = (size > 0 ? RASTR_FONT_H : RASTR_FONT_4x6_H);

	if ((x >= LCD_WIDTH) ||              // Clip right
	    (y >= LCD_HEIGHT) ||             // Clip bottom
	    ((x + size_w * zoom - 1) < 0) || // Clip left
	    ((y + size_h * zoom - 1) < 0)) { // Clip top
		return;
	}

	if (c < 32) { // non-printable
		return;
	}
	if (!_cp437 && (c >= 176)) {
		c++; // Handle 'classic' charset behavior
	}

	LCDDriver_SetCursorAreaPosition(x, y, x + size_w * zoom - 1, y + size_h * zoom - 1); // char area
	if (size > 0) {
		for (int8_t j = 0; j < size_h; j++) // y line out
		{
			for (int8_t s_y = 0; s_y < zoom; s_y++) // y size scale
			{
				for (int8_t i = 0; i < size_w; i++) {
					// x line out
					if (i == 5) {
						line = 0x0;
					} else {
						line = rastr_font[(c * 5) + i]; // read font
					}
					line >>= j;
					for (int8_t s_x = 0; s_x < zoom; s_x++) // x size scale
					{
						if (line & 0x1) {
							LCDDriver_SendData16(color); // font pixel
						} else {
							LCDDriver_SendData16(bg); // background pixel
						}
					}
				}
			}
		}
	} else // size 0 4x6
	{
		for (int8_t j = 0; j < size_h; j++) // y line out
		{
			line = rastr_font_4x6[((c - 0x20) * 6) + j]; // read font
			for (int8_t i = 0; i < size_w; i++) {
				if (line & 0x8) {
					LCDDriver_SendData16(color); // font pixel
				} else {
					LCDDriver_SendData16(bg); // background pixel
				}

				line <<= 1;
			}
		}
	}
}

void LCDDriver_printText(char text[], uint16_t x, uint16_t y, uint16_t color, uint16_t bg, uint8_t size) {
	uint16_t i = 0;
	uint16_t offset = size * 6;
	uint16_t skipped = 0;
	for (i = 0; i < 128 && text[i] != 0; i++) {
		if (text[i] == '^' && text[i + 1] == 'o') // celsius
		{
			i++;
			skipped++;
			LCDDriver_drawChar(x + (offset * (i - skipped)), y - 3, text[i], color, bg, size);
		} else {
			LCDDriver_drawChar(x + (offset * (i - skipped)), y, text[i], color, bg, size);
		}
		text_cursor_x = x + (offset * (i + 1 - skipped));
	}
}

void LCDDriver_drawCharFont(uint16_t x, uint16_t y, unsigned char c, uint16_t color, uint16_t bg, const GFXfont *gfxFont) {
	c -= gfxFont->first;
	const GFXglyph *glyph = (GFXglyph *)&gfxFont->glyph[c];

	uint16_t bo = glyph->bitmapOffset;
	uint8_t bits = 0;
	uint8_t bit = 0;
	int16_t ys1 = y + glyph->yOffset;
	int16_t ys2 = y + glyph->yOffset + glyph->height - 1;
	if (ys1 < 0) {
		ys1 = 0;
	}
	if (ys2 < 0) {
		ys2 = 0;
	}
	LCDDriver_SetCursorAreaPosition(x, (uint16_t)ys1, x + glyph->xAdvance - 1, (uint16_t)ys2); // char area

	for (uint8_t yy = 0; yy < glyph->height; yy++) {
		for (uint8_t xx = 0; xx < glyph->xAdvance; xx++) {
			if (xx < glyph->xOffset || xx >= (glyph->xOffset + glyph->width)) {
				LCDDriver_SendData16(bg); // background pixel
				continue;
			}
			if (!(bit++ & 7)) {
				bits = gfxFont->bitmap[bo++];
			}
			if (bits & 0x80) {
				LCDDriver_SendData16(color); // font pixel
			} else {
				LCDDriver_SendData16(bg); // background pixel
			}
			bits <<= 1;
		}
	}
}

void LCDDriver_printTextFont(char text[], uint16_t x, uint16_t y, uint16_t color, uint16_t bg, const GFXfont *gfxFont) {
	uint8_t c = 0;
	text_cursor_x = x;
	text_cursor_y = y;
	for (uint16_t i = 0; i < 1024 && text[i] != NULL; i++) {
		c = text[i];
		if (c == '\n') {
			text_cursor_x = 0;
			text_cursor_y += gfxFont->yAdvance;
			if (text_cursor_y > LCD_HEIGHT) {
				return;
			}
		} else if (c != '\r') {
			if ((c >= gfxFont->first) && (c <= gfxFont->last)) {
				const GFXglyph *glyph = (GFXglyph *)&gfxFont->glyph[c - gfxFont->first];
				if ((glyph->width > 0) && (glyph->height > 0)) {
					if (wrap && ((text_cursor_x + (glyph->xOffset + glyph->width)) > LCD_WIDTH)) {
						text_cursor_x = 0;
						text_cursor_y += gfxFont->yAdvance;
					}
					LCDDriver_drawCharFont(text_cursor_x, text_cursor_y, c, color, bg, gfxFont);
				}
				text_cursor_x += glyph->xAdvance;
			}
		}
	}
}

/**************************************************************************/
/*!
  @brief    Helper to determine size of a character with current font/size.
     Broke this out as it's used by both the PROGMEM- and RAM-resident getTextBounds() functions.
  @param    c     The ascii character in question
  @param    x     Pointer to x location of character
  @param    y     Pointer to y location of character
  @param    minx  Minimum clipping value for X
  @param    miny  Minimum clipping value for Y
  @param    maxx  Maximum clipping value for X
  @param    maxy  Maximum clipping value for Y
*/
/**************************************************************************/
static void LCDDriver_charBounds(char c, uint16_t *x, uint16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy, const GFXfont *gfxFont) {
	if (c == '\n') { // Newline?
		*x = 0;        // Reset x to zero, advance y by one line
		*y += gfxFont->yAdvance;
	} else if (c != '\r') {
		if ((c >= gfxFont->first) && (c <= gfxFont->last)) { // Char present in this font?
			const GFXglyph *glyph = (GFXglyph *)&gfxFont->glyph[c - gfxFont->first];
			if (wrap && ((*x + (((int16_t)glyph->xOffset + glyph->width))) > LCD_WIDTH)) {
				*x = 0; // Reset x to zero, advance y by one line
				*y += gfxFont->yAdvance;
			}
			int16_t x1 = *x + glyph->xOffset, y1 = *y + glyph->yOffset, x2 = x1 + glyph->width - 1, y2 = y1 + glyph->height - 1;
			if (x1 < *minx) {
				*minx = x1;
			}
			if (y1 < *miny) {
				*miny = y1;
			}
			if (x2 > *maxx) {
				*maxx = x2;
			}
			if (y2 > *maxy) {
				*maxy = y2;
			}
			*x += glyph->xAdvance;
		}
	}
}

/**************************************************************************/
/*!
  @brief    Helper to determine size of a string with current font/size. Pass string and a cursor position, returns UL corner and W,H.
  @param    str     The ascii string to measure
  @param    x       The current cursor X
  @param    y       The current cursor Y
  @param    x1      The boundary X coordinate, set by function
  @param    y1      The boundary Y coordinate, set by function
  @param    w      The boundary width, set by function
  @param    h      The boundary height, set by function
*/
/**************************************************************************/
void LCDDriver_getTextBoundsFont(char text[], uint16_t x, uint16_t y, uint16_t *x1, uint16_t *y1, uint16_t *w, uint16_t *h, const GFXfont *gfxFont) {
	uint8_t c; // Current character

	*x1 = x;
	*y1 = y;
	*w = *h = 0;

	int16_t minx = LCD_WIDTH, miny = LCD_HEIGHT, maxx = 0, maxy = 0;

	for (uint16_t i = 0; i < 40 && text[i] != NULL; i++) {
		c = text[i];
		LCDDriver_charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy, gfxFont);
	}

	if (maxx >= minx) {
		*x1 = (uint16_t)minx;
		*w = (uint16_t)(maxx - minx + 1);
	}
	if (maxy >= miny) {
		*y1 = (uint16_t)miny;
		*h = (uint16_t)(maxy - miny + 1);
	}
}

void LCDDriver_getTextBounds(char text[], uint16_t x, uint16_t y, uint16_t *x1, uint16_t *y1, uint16_t *w, uint16_t *h, uint8_t size) {
	*w = 6 * size * strlen(text);
	*h = 8 * size;
}

// Image print (RGB 565, 2 bytes per pixel)
void LCDDriver_printImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data) {
	uint32_t n = w * h * 2;
	LCDDriver_SetCursorAreaPosition(x, y, w + x - 1, h + y - 1);
	for (uint32_t i = 0; i < n; i += 2) {
		LCDDriver_SendData16((uint16_t)((data[i] << 8) | data[i + 1]));
	}
}

void LCDDriver_printImage_RLECompressed(uint16_t x, uint16_t y, const tIMAGE *image, uint16_t transparent_color, uint16_t bg_color) {
	uint32_t pixels = image->width * image->height;
	uint32_t i = 0;
	uint32_t decoded = 0;

	LCDDriver_SetCursorAreaPosition(x, y, image->width + x - 1, image->height + y - 1);
	while (true) {
		if ((int16_t)image->data[i] < 0) // no repeats
		{
			uint16_t count = (-(int16_t)image->data[i]);
			i++;
			for (uint16_t p = 0; p < count; p++) {
				if (image->data[i] == transparent_color) {
					LCDDriver_SendData16(bg_color);
				} else {
					LCDDriver_SendData16(image->data[i]);
				}
				decoded++;
				i++;
				if (pixels <= decoded) {
					return;
				}
			}
		} else // repeats
		{
			uint16_t count = ((int16_t)image->data[i]);
			i++;
			for (uint16_t p = 0; p < count; p++) {
				if (image->data[i] == transparent_color) {
					LCDDriver_SendData16(bg_color);
				} else {
					LCDDriver_SendData16(image->data[i]);
				}
				decoded++;
				if (pixels <= decoded) {
					return;
				}
			}
			i++;
		}
	}
}

static uint32_t RLEStream_pixels = 0;
static uint32_t RLEStream_decoded = 0;
static uint8_t RLEStream_state = 0;
void LCDDriver_printImage_RLECompressed_StartStream(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	RLEStream_pixels = width * height;
	RLEStream_decoded = 0;
	RLEStream_state = 0;

	LCDDriver_SetCursorAreaPosition(x, y, width + x - 1, height + y - 1);
}

void LCDDriver_printImage_RLECompressed_ContinueStream(int16_t *data, uint16_t len) {
	static uint16_t nr_count = 0;
	static uint16_t nr_count_p = 0;
	static uint16_t r_count = 0;
	static uint16_t r_count_p = 0;
	uint32_t processed = 0;
	while (processed < len) {
		if ((((int16_t)data[processed] < 0) && (RLEStream_state == 0)) || (RLEStream_state == 1)) // no repeats
		{
			if (RLEStream_state == 0) {
				nr_count = (-(int16_t)data[processed]);
				nr_count_p = 0;
				processed++;
			}
			RLEStream_state = 1;

			if (processed >= len) {
				return;
			}

			for (; nr_count_p < nr_count;) {
				LCDDriver_SendData16(data[processed]);
				RLEStream_decoded++;
				nr_count_p++;
				processed++;
				if (RLEStream_pixels <= RLEStream_decoded) {
					return;
				}
				if (processed >= len && nr_count_p < nr_count) {
					return;
				}
			}
			RLEStream_state = 0;
		} else if ((((int16_t)data[processed] > 0) && (RLEStream_state == 0)) || (RLEStream_state == 2)) // repeats
		{
			if (RLEStream_state == 0) {
				r_count = ((int16_t)data[processed]);
				r_count_p = 0;
				processed++;
			}
			RLEStream_state = 2;

			if (processed >= len) {
				return;
			}

			for (; r_count_p < r_count;) {
				LCDDriver_SendData16(data[processed]);
				r_count_p++;
				RLEStream_decoded++;
				if (RLEStream_pixels <= RLEStream_decoded) {
					return;
				}
			}
			processed++;
			RLEStream_state = 0;
		} else {
			processed++;
		}
	}
}

inline uint16_t addColor(uint16_t color, uint8_t add_r, uint8_t add_g, uint8_t add_b) {
	uint8_t r = ((color >> 11) & 0x1F) + add_r;
	uint8_t g = ((color >> 5) & 0x3F) + (uint8_t)(add_g << 1);
	uint8_t b = ((color >> 0) & 0x1F) + add_b;
	if (r > 31) {
		r = 31;
	}
	if (g > 63) {
		g = 63;
	}
	if (b > 31) {
		b = 31;
	}
	return (uint16_t)((r << 11) | (g << 5) | b);
}

inline uint16_t mixColors(uint16_t color1, uint16_t color2, float32_t opacity) {
	uint8_t r = (uint8_t)((float32_t)((color1 >> 11) & 0x1F) * (1.0f - opacity) + (float32_t)((color2 >> 11) & 0x1F) * opacity);
	uint8_t g = (uint8_t)((float32_t)((color1 >> 5) & 0x3F) * (1.0f - opacity) + (float32_t)((color2 >> 5) & 0x3F) * opacity);
	uint8_t b = (uint8_t)((float32_t)((color1 >> 0) & 0x1F) * (1.0f - opacity) + (float32_t)((color2 >> 0) & 0x1F) * opacity);
	if (r > 31) {
		r = 31;
	}
	if (g > 63) {
		g = 63;
	}
	if (b > 31) {
		b = 31;
	}
	return (uint16_t)(r << 11) | (uint16_t)(g << 5) | (uint16_t)b;
}

#if LCD_WIDTH > 300 && HRDW_HAS_JPEG

#define JPEG_chunk_size_out 384
IRAM2 uint8_t JPEG_out_buffer[JPEG_chunk_size_out];
uint32_t JPEG_blockIndex = 0;
JPEG_YCbCrToRGB_Convert_Function JPEG_ConvertFunction = NULL;

void LCDDriver_printImage_JPEGCompressed(uint16_t x, uint16_t y, const uint8_t *image, uint32_t size) {
	JPEG_blockIndex = 0;
	uint8_t res = HAL_JPEG_Decode(&HRDW_JPEG_HANDLE, (uint8_t *)image, size, JPEG_out_buffer, JPEG_chunk_size_out, HAL_MAX_DELAY);
}

void HAL_JPEG_InfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo) {
	// println("HAL_JPEG_InfoReadyCallback ", pInfo->ImageHeight, " ", pInfo->ImageWidth, " ", pInfo->ImageQuality, " ", pInfo->ChromaSubsampling,
	// " ", pInfo->ColorSpace);

	uint32_t JPEG_ImageNbMCUs = 0;
	JPEG_GetDecodeColorConvertFunc(pInfo, &JPEG_ConvertFunction, &JPEG_ImageNbMCUs);
}

void HAL_JPEG_DataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength) {
	// println("HAL_JPEG_DataReadyCallback");

	if (JPEG_ConvertFunction != NULL) {
		uint8_t JPEG_out_buffer_rgb[1];
		uint32_t ConvertedDataCount = 0;

		JPEG_blockIndex += JPEG_ConvertFunction(pDataOut, JPEG_out_buffer_rgb, JPEG_blockIndex, OutDataLength, &ConvertedDataCount);
		// println(JPEG_ImageNbMCUs, " ", JPEG_blockIndex, " ", OutDataLength, " ", ConvertedDataCount);
	}

	/* Update JPEG encoder output buffer address*/
	HAL_JPEG_ConfigOutputBuffer(hjpeg, JPEG_out_buffer, JPEG_chunk_size_out);
}

#endif
