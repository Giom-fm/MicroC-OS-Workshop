/*
 * vga_font.c
 *
 * Font drawing routines for VGA module.
 *
 * Provides basic font selection, virtual grid cursor positioning
 * and single character and string print routines.
 *
 *  Created on: 23.10.2015
 *      Author: Thorsten Fritzke (tinf100053)
 */

#include <stdint.h>
#include "vga.h"
#include "vga_font.h"
#include "vga_font_tables.h"

// virtual row, col for character display (pixel coordinates)
static uint16_t ch_x = 0, ch_y = 0;

// selected font
static uint8_t curFont = 0;

/**
 * vga_selFont
 *
 * select the fontsize to be used by 'vga_putc'
 *
 * @param font 0 - 6x8 font
 *             1 - 12x16 font
 */
void vga_selFont(uint8_t font) {
	// argument sanity check
	if (font >= NUM_FONTS)
		font = NUM_FONTS - 1;

	curFont = font;
}

/**
 * vga_setCurPos
 *
 * set virtual cursor position for printing characters.
 * The framebuffer is segmented into a grid defined by the
 * dimensions of the selected font. The height of a single line
 * corresponds to the height of the selected font.
 * i.e. using 6x8 font: line 0 is display line 0-7, line 1 8-15, ...
 *
 * @param x x-index of font grid position
 * @param y y-index of font grid position
 */
void vga_setCurPos(uint8_t x, uint8_t y) {
	// argument sanity check
	if (x > (DISPLAY_WIDTH / fontList[curFont].width))
		x = DISPLAY_WIDTH / fontList[curFont].width;
	if (y > (DISPLAY_HEIGHT / fontList[curFont].height))
		y = DISPLAY_HEIGHT / fontList[curFont].height;

	ch_x = x * fontList[curFont].width;
	ch_y = y * fontList[curFont].height;
}

/**
 * vga_putc
 *
 * copies single character data from font table into framebuffer
 * at the current virtual cursor position. The cursor position is advanced.
 *
 * @param c character to be displayed
 * @param color color of the character in RGB332 format
 */
void vga_putc(char c, uint8_t color) {
	// get font info
	uint8_t fx = fontList[curFont].width;
	uint8_t fy = fontList[curFont].height;
	const uint8_t *fdata = fontList[curFont].table;

	// iterate over bytes in font
	int i = 0;
	for (i = 0; i < (fx * fy / 8); i++) {
		// loop over each bit in that byte
		int bit = 0;
		for (bit = 0; bit < 8; bit++) {
			uint8_t mask = 1 << bit; // maybe slow?
			// calc address of pixel
			int adrX = ch_x + (i / (fy / 8));
			int adrY = ch_y + bit;
			if (fy > 8) {
				// adjust for multibyte font
				adrY += 8 * (1 - (i & 1));
			}
			// set or clear?
			vga_pixel(adrX, adrY, (*(fdata + (uint8_t)c * (fy * fx / 8) + i) & mask) ? color : 0x00);
		}
	}
	// adjust char position
	ch_x += fx;
	if (ch_x >= fx * (DISPLAY_WIDTH / fx)) {
		ch_x = 0;
		ch_y += fy;
		if (ch_y >= fy * (DISPLAY_HEIGHT / fy))
			ch_y = 0;
	}
}

/**
 * vga_puts
 *
 * display a 0-terminated string at the current virtual cursor position.
 *
 * @param s 0-terminated string to be displayed
 * @param color color of the string in RGB332 format
 */
void vga_puts(char *s, uint8_t color) {
	while(*s) {
		vga_putc(*s++, color);
	}

	// always flush framebuffer after printing a string
	vga_flush();
}
