/*
 * vga_font.h
 *
 * Font drawing routines for VGA module.
 *
 * Provides basic font selection, virtual grid cursor positioning
 * and single character and string print routines.
 *
 *  Created on: 23.10.2015
 *      Author: Thorsten Fritzke (tinf100053)
 */

#ifndef VGA_FONT_H_
#define VGA_FONT_H_

#include <stdint.h>

enum {
	FONT_6x8,
	FONT_12x16
};

/**
 * vga_selFont
 *
 * select the fontsize to be used by 'vga_putc'.
 * Allowed parameters defined by font size enum.
 *
 * @param font 0 - 6x8 font
 *             1 - 12x16 font
 */
void vga_selFont(uint8_t font);

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
void vga_setCurPos(uint8_t x, uint8_t y);

/**
 * vga_putc
 *
 * copies single character data from font table into framebuffer
 * at the current virtual cursor position. The cursor position is advanced.
 *
 * @param c character to be displayed
 * @param color color of the character in RGB332 format
 */
void vga_putc(char c, uint8_t color);

/**
 * vga_puts
 *
 * display a 0-terminated string at the current virtual cursor position.
 *
 * @param s 0-terminated string to be displayed
 * @param color color of the string in RGB332 format
 */
void vga_puts(char *s, uint8_t color);

#endif /* VGA_FONT_H_ */
